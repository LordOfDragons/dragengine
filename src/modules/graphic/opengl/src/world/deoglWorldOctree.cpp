/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglWorldOctree.h"
#include "deoglWorldOctreeVisitor.h"
#include "deoglWorldCSOctree.h"

#include "../billboard/deoglRBillboard.h"
#include "../component/deoglRComponent.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../light/deoglRLight.h"
#include "../particle/deoglRParticleEmitterInstance.h"
#include "../sensor/deoglRLumimeter.h"
#include "../utils/collision/deoglDCollisionVolume.h"
#include "../utils/collision/deoglDCollisionDetection.h"
#include "../utils/collision/deoglDCollisionBox.h"

#include <dragengine/common/exceptions.h>



// Class deoglWorldOctree
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglWorldOctree::deoglWorldOctree(const decDVector &center, const decDVector &halfSize, int insertDepth) :
deoglDOctree(center, halfSize),
pInsertDepth(insertDepth),
pCSChildCount(0),
pCSLocalElementCount(0),
pCSNodeCount(0),
pCSElementCount(0){
}

deoglWorldOctree::~deoglWorldOctree(){
	// these are not called since world cleaning up uses a fast clean up call which drops
	// references quickly without further processing. doing the RemoveAll* calls here
	// would update these objects although cleaned up by now
	//RemoveAllLumimeters();
	//RemoveAllLights();
	//RemoveAllComponents();
	//RemoveAllBillboards();
	//RemoveAllEnvMaps();
	//RemoveAllParticleEmitters();
}



// Management
///////////////

deoglDOctree *deoglWorldOctree::CreateOctree(int octant) const{
	const decDVector halfSize(GetHalfSize() * 0.5);
	const decDVector &center = GetCenter();
	decDVector nc;
	
	// determine the smallest and largest coordinates
	if((octant & 4) == 4){
		nc.x = center.x + halfSize.x;
		
	}else{
		nc.x = center.x - halfSize.x;
	}
	
	if((octant & 2) == 2){
		nc.y = center.y + halfSize.y;
		
	}else{
		nc.y = center.y - halfSize.y;
	}
	
	if((octant & 1) == 1){
		nc.z = center.z + halfSize.z;
		
	}else{
		nc.z = center.z - halfSize.z;
	}
	
	// create child node
	return new deoglWorldOctree(nc, halfSize, pInsertDepth - 1);
}

void deoglWorldOctree::ClearNodeContent(){
	RemoveAllLights();
	RemoveAllComponents();
	RemoveAllLumimeters();
	RemoveAllBillboards();
	RemoveAllEnvMaps();
	RemoveAllParticleEmitters();
}

void deoglWorldOctree::ClearBillboards(){
	RemoveAllBillboards();
	
	int i;
	for(i=0; i<8; i++){
		deoglDOctree * const node = GetNodeAt(i);
		if(node){
			((deoglWorldOctree*)node)->ClearBillboards();
		}
	}
}

void deoglWorldOctree::ClearComponents(){
	RemoveAllComponents();
	
	int i;
	for(i=0; i<8; i++){
		deoglDOctree * const node = GetNodeAt(i);
		if(node){
			((deoglWorldOctree*)node)->ClearComponents();
		}
	}
}

void deoglWorldOctree::ClearEnvMaps(){
	RemoveAllEnvMaps();
	
	int i;
	for(i=0; i<8; i++){
		deoglDOctree * const node = GetNodeAt(i);
		if(node){
			((deoglWorldOctree*)node)->ClearEnvMaps();
		}
	}
}

void deoglWorldOctree::ClearLights(){
	RemoveAllLights();
	
	int i;
	for(i=0; i<8; i++){
		deoglDOctree * const node = GetNodeAt(i);
		if(node){
			((deoglWorldOctree*)node)->ClearLights();
		}
	}
}

void deoglWorldOctree::ClearLumimeters(){
	RemoveAllLumimeters();
	
	int i;
	for(i=0; i<8; i++){
		deoglDOctree * const node = GetNodeAt(i);
		if(node){
			((deoglWorldOctree*)node)->ClearLumimeters();
		}
	}
}

void deoglWorldOctree::ClearParticleEmitters(){
	RemoveAllParticleEmitters();
	
	int i;
	for(i=0; i<8; i++){
		deoglDOctree * const node = GetNodeAt(i);
		if(node){
			((deoglWorldOctree*)node)->ClearParticleEmitters();
		}
	}
}

void deoglWorldOctree::InsertBillboardIntoTree(deoglRBillboard *billboard){
	if(!billboard){
		DETHROW(deeInvalidParam);
	}
	
	const decDVector &maxExtend = billboard->GetMaximumExtend();
	const decDVector &minExtend = billboard->GetMinimumExtend();
	deoglWorldOctree *currentNode = billboard->GetOctreeNode();
	
	if(currentNode){
		if(!currentNode->ContainsBox(minExtend, maxExtend)){
			currentNode->RemoveBillboard(billboard);
			currentNode = NULL;
		}
	}
	
	if(!currentNode){
		currentNode = pGetNodeFor(minExtend, maxExtend, pInsertDepth);
		currentNode->AddBillboard(billboard);
	}
}

void deoglWorldOctree::InsertComponentIntoTree(deoglRComponent *component){
	if(!component) DETHROW(deeInvalidParam);
	
	const decDVector &maxExtend = component->GetMaximumExtend();
	const decDVector &minExtend = component->GetMinimumExtend();
	
	deoglWorldOctree *currentNode = component->GetOctreeNode();
	
	// if the component is already in the octree relocate it if required
	if(currentNode){
		// if the component is not in the same node as before relocate it
		if(!currentNode->ContainsBox(minExtend, maxExtend)){
			// remove from the current node
			currentNode->RemoveComponent(component);
			currentNode = NULL;
		}
	}
	
	// add component if not in the tree
	if(!currentNode){
		currentNode = pGetNodeFor(minExtend, maxExtend, pInsertDepth);
		currentNode->AddComponent(component);
	}
}

void deoglWorldOctree::InsertEnvMapIntoTree(deoglEnvironmentMap *envmap){
	if(!envmap){
		DETHROW(deeInvalidParam);
	}
	
	const decDVector &boxCenter = envmap->GetPosition();
	const decDVector boxHalfExtends = decDVector(0.01f, 0.01f, 0.01f);
	const decDVector minExtend(boxCenter - boxHalfExtends);
	const decDVector maxExtend(boxCenter + boxHalfExtends);
	deoglWorldOctree *currentNode = envmap->GetOctreeNode();
	
	if(currentNode){
		if(!currentNode->ContainsBox(minExtend, maxExtend)){
			currentNode->RemoveEnvMap(envmap);
			currentNode = NULL;
		}
	}
	
	if(!currentNode){
		currentNode = pGetNodeFor(minExtend, maxExtend, pInsertDepth);
		currentNode->AddEnvMap(envmap);
	}
}

void deoglWorldOctree::InsertLightIntoTree(deoglRLight *light){
	if(!light){
		DETHROW(deeInvalidParam);
	}
	
	const decDVector &maxExtend = light->GetMaximumExtend();
	const decDVector &minExtend = light->GetMinimumExtend();
	
	deoglWorldOctree *currentNode = light->GetOctreeNode();
	
	// if the light is already in the octree relocate it if required
	if(currentNode){
		// if the light is not in the same node as before relocate it
		if(!currentNode->ContainsBox(minExtend, maxExtend)){
			// remove from the current node
			currentNode->RemoveLight(light);
			currentNode = NULL;
		}
	}
	
	// add light if not in the tree
	if(!currentNode){
		currentNode = pGetNodeFor(minExtend, maxExtend, pInsertDepth);
		currentNode->AddLight(light);
	}
}

void deoglWorldOctree::InsertParticleEmitterIntoTree(deoglRParticleEmitterInstance *instance){
	if(!instance){
		DETHROW(deeInvalidParam);
	}
	
	const decDVector &maxExtend = instance->GetMaxExtend();
	const decDVector &minExtend = instance->GetMinExtend();
	deoglWorldOctree *currentNode = instance->GetOctreeNode();
	
	if(currentNode){
		if(!currentNode->ContainsBox(minExtend, maxExtend)){
			currentNode->RemoveParticleEmitter(instance);
			currentNode = NULL;
		}
	}
	
	if(!currentNode){
		currentNode = pGetNodeFor(minExtend, maxExtend, pInsertDepth);
		currentNode->AddParticleEmitter(instance);
	}
}

deoglWorldOctree *deoglWorldOctree::InsertLumimeterIntoTree(deoglRLumimeter *lumimeter){
	if(!lumimeter){
		DETHROW(deeInvalidParam);
	}
	
	deoglWorldOctree *node = pGetNodeFor(lumimeter->GetPosition(), pInsertDepth);
	node->AddLumimeter(lumimeter);
	
	return node;
}



void deoglWorldOctree::VisitNodesCollidingVolume(deoglWorldOctreeVisitor *visitor, deoglDCollisionVolume *volume){
	if(!visitor || !volume){
		DETHROW(deeInvalidParam);
	}
	deoglDCollisionBox colBox(GetCenter(), GetHalfSize());
	int i;
	
	// exit if this node is not in the collision volume
	if(!volume->BoxHitsVolume(&colBox)) return;
	
	// visit
	visitor->VisitNode(this, deoglDCollisionDetection::eirPartial);
	
	// test each child node
	for(i=0; i<8; i++){
		deoglDOctree * const node = GetNodeAt(i);
		if(node){
			((deoglWorldOctree*)node)->VisitNodesCollidingVolume(visitor, volume);
		}
	}
}



void deoglWorldOctree::UpdateCSCounts(){
	int i;
	
	pCSChildCount = 0;
	pCSLocalElementCount = pComponents.GetCount() + pBillboards.GetCount()
		+ pParticleEmitters.GetCount() + pLights.GetCount();
	
	pCSNodeCount = 0;
	pCSElementCount = pCSLocalElementCount;
	
	for(i=0; i<8; i++){
		deoglWorldOctree * const node = (deoglWorldOctree *)GetNodeAt(i);
		if(!node){
			continue;
		}
		
		node->UpdateCSCounts();
		if(node->pCSElementCount == 0){
			continue;
		}
		
		pCSChildCount++;
		pCSNodeCount += node->pCSNodeCount;
		pCSElementCount += node->pCSElementCount;
	}
	
	if(pCSElementCount > 0){
		pCSElementCount = decMath::min(pCSElementCount, 0xfffffff);
		pCSNodeCount++;
	}
}

void deoglWorldOctree::WriteCSData(deoglWorldCSOctree &csoctree, int nodeIndex){
	if(pCSElementCount == 0){
		return;
	}
	
	const decDVector &refpos = csoctree.GetReferencePosition();
	int i, nextChildNode = csoctree.GetNextNode();
	
	deoglWorldCSOctree::sCSNode &node = csoctree.GetNodeAt(nodeIndex);
	node.SetExtends(GetMinimumExtend() - refpos, GetMaximumExtend() - refpos);
	node.firstNode = nextChildNode;
	node.childNodeCount = (uint32_t)pCSChildCount;
	node.firstElement = (uint32_t)csoctree.GetNextElement();
	node.elementCount = (uint32_t)pCSLocalElementCount;
	
	csoctree.AdvanceNextData(pCSChildCount);
	
	for(i=0; i<pComponents.GetCount(); i++){
		deoglRComponent &component = *(deoglRComponent*)pComponents.GetAt(i);
		uint32_t flags = deoglWorldCSOctree::ecsefComponent;
		if(!component.GetRenderStatic()){
			flags |= deoglWorldCSOctree::ecsefComponentDynamic;
		}
		
		component.SetCSOctreeIndex((uint32_t)csoctree.NextElement(deoglWorldCSOctree::ecsetComponent, &component));
		deoglWorldCSOctree::sCSElement &element = csoctree.GetElementAt(component.GetCSOctreeIndex());
		element.SetExtends(component.GetMinimumExtend() - refpos, component.GetMaximumExtend() - refpos);
		element.elementindex = component.GetCSOctreeIndex();
		element.flags = flags;
		element.SetLayerMask(component.GetLayerMask());
	}
	
	for(i=0; i<pBillboards.GetCount(); i++){
		deoglRBillboard &billboard = *(deoglRBillboard*)pBillboards.GetAt(i);
		
		billboard.SetCSOctreeIndex((uint32_t)csoctree.NextElement(deoglWorldCSOctree::ecsetBillboard, &billboard));
		deoglWorldCSOctree::sCSElement &element = csoctree.GetElementAt(billboard.GetCSOctreeIndex());
		element.SetExtends(billboard.GetMinimumExtend() - refpos, billboard.GetMaximumExtend() - refpos);
		element.elementindex = billboard.GetCSOctreeIndex();
		element.flags = (uint32_t)deoglWorldCSOctree::ecsefBillboard;
		element.SetLayerMask(billboard.GetLayerMask());
	}
	
	for(i=0; i<pParticleEmitters.GetCount(); i++){
		deoglRParticleEmitterInstance &emitter = *(deoglRParticleEmitterInstance*)pParticleEmitters.GetAt(i);
		
		emitter.SetCSOctreeIndex((uint32_t)csoctree.NextElement(deoglWorldCSOctree::ecsetParticleEmitter, &emitter));
		deoglWorldCSOctree::sCSElement &element = csoctree.GetElementAt(emitter.GetCSOctreeIndex());
		element.SetExtends(emitter.GetMinExtend() - refpos, emitter.GetMaxExtend() - refpos);
		element.elementindex = emitter.GetCSOctreeIndex();
		element.flags = (uint32_t)deoglWorldCSOctree::ecsefParticleEmitter;
		element.SetLayerMask(emitter.GetLayerMask());
	}
	
	for(i=0; i<pLights.GetCount(); i++){
		deoglRLight &light = *(deoglRLight*)pLights.GetAt(i);
		
		light.SetCSOctreeIndex((uint32_t)csoctree.NextElement(deoglWorldCSOctree::ecsetLight, &light));
		deoglWorldCSOctree::sCSElement &element = csoctree.GetElementAt(light.GetCSOctreeIndex());
		element.SetExtends(light.GetMinimumExtend() - refpos, light.GetMaximumExtend() - refpos);
		element.elementindex = light.GetCSOctreeIndex();
		element.flags = (uint32_t)deoglWorldCSOctree::ecsefLight;
		element.SetLayerMask(light.GetLayerMask());
	}
	
	for(i=0; i<8; i++){
		deoglWorldOctree * const childNode = (deoglWorldOctree *)GetNodeAt(i);
		if(childNode && childNode->pCSElementCount > 0){
			childNode->WriteCSData(csoctree, nextChildNode++);
		}
	}
}



// Components
///////////////

int deoglWorldOctree::GetComponentCount() const{
	return pComponents.GetCount();
}

deoglRComponent *deoglWorldOctree::GetComponentAt(int index) const{
	return (deoglRComponent*)pComponents.GetAt(index);
}

int deoglWorldOctree::IndexOfComponent(deoglRComponent *component) const{
	return pComponents.IndexOf(component);
}

void deoglWorldOctree::AddComponent(deoglRComponent *component){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	pComponents.Add(component);
	
	component->SetOctreeNode(this);
}

void deoglWorldOctree::RemoveComponent(deoglRComponent *component){
	const int index = pComponents.IndexOf(component);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pComponents.RemoveFrom(index);
	
	component->SetOctreeNode(NULL);
}

void deoglWorldOctree::RemoveAllComponents(){
	const int count = pComponents.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRComponent*)pComponents.GetAt(i))->SetOctreeNode(NULL);
	}
	
	pComponents.RemoveAll();
}



// Lights
///////////

int deoglWorldOctree::GetLightCount() const{
	return pLights.GetCount();
}

deoglRLight *deoglWorldOctree::GetLightAt(int index) const{
	return (deoglRLight*)pLights.GetAt(index);
}

int deoglWorldOctree::IndexOfLight(deoglRLight *light) const{
	return pLights.IndexOf(light);
}

void deoglWorldOctree::AddLight(deoglRLight *light){
	if(!light){
		DETHROW(deeInvalidParam);
	}
	
	pLights.Add(light);
	
	light->SetOctreeNode(this);
}

void deoglWorldOctree::RemoveLight(deoglRLight *light){
	const int index = pLights.IndexOf(light);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pLights.RemoveFrom(index);
	
	light->SetOctreeNode(NULL);
}

void deoglWorldOctree::RemoveAllLights(){
	const int count = pLights.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRLight*)pLights.GetAt(i))->SetOctreeNode(NULL);
	}
	
	pLights.RemoveAll();
}



// Lumimeters
///////////////

int deoglWorldOctree::GetLumimeterCount() const{
	return pLumimeters.GetCount();
}

deoglRLumimeter *deoglWorldOctree::GetLumimeterAt(int index) const{
	return (deoglRLumimeter*)pLumimeters.GetAt(index);
}

int deoglWorldOctree::IndexOfLumimeter(deoglRLumimeter *lumimeter) const{
	return pLumimeters.IndexOf(lumimeter);
}

void deoglWorldOctree::AddLumimeter(deoglRLumimeter *lumimeter){
	if(!lumimeter){
		DETHROW(deeInvalidParam);
	}
	pLumimeters.Add(lumimeter);
	
	lumimeter->SetOctreeNode(this);
}

void deoglWorldOctree::RemoveLumimeter(deoglRLumimeter *lumimeter){
	const int index = IndexOfLumimeter(lumimeter);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	pLumimeters.RemoveFrom(index);
	
	lumimeter->SetOctreeNode(NULL);
}

void deoglWorldOctree::RemoveAllLumimeters(){
	const int count = pLumimeters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRLight*)pLumimeters.GetAt(i))->SetOctreeNode(NULL);
	}
	
	pLumimeters.RemoveAll();
}



// Billboards
///////////////

void deoglWorldOctree::AddBillboard(deoglRBillboard *billboard){
	if(!billboard){
		DETHROW(deeInvalidParam);
	}
	
	pBillboards.Add(billboard);
	
	billboard->SetOctreeNode(this);
}

void deoglWorldOctree::RemoveBillboard(deoglRBillboard *billboard){
	const int index = pBillboards.IndexOf(billboard);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pBillboards.RemoveFrom(index);
	
	billboard->SetOctreeNode(NULL);
}

void deoglWorldOctree::RemoveAllBillboards(){
	const int count = pBillboards.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRBillboard*)pBillboards.GetAt(i))->SetOctreeNode(NULL);
	}
	
	pBillboards.RemoveAll();
}



// Environment maps
/////////////////////

void deoglWorldOctree::AddEnvMap(deoglEnvironmentMap *envmap){
	if(!envmap){
		DETHROW(deeInvalidParam);
	}
	
	pEnvMaps.Add(envmap);
	
	envmap->SetOctreeNode(this);
}

void deoglWorldOctree::RemoveEnvMap(deoglEnvironmentMap *envmap){
	const int index = pEnvMaps.IndexOf(envmap);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pEnvMaps.RemoveFrom(index);
	
	envmap->SetOctreeNode(NULL);
}

void deoglWorldOctree::RemoveAllEnvMaps(){
	const int count = pEnvMaps.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglEnvironmentMap*)pEnvMaps.GetAt(i))->SetOctreeNode(NULL);
	}
	
	pEnvMaps.RemoveAll();
}



// Particle emitters
//////////////////////

void deoglWorldOctree::AddParticleEmitter(deoglRParticleEmitterInstance *emitter){
	if(!emitter){
		DETHROW(deeInvalidParam);
	}
	
	pParticleEmitters.Add(emitter);
	
	emitter->SetOctreeNode(this);
}

void deoglWorldOctree::RemoveParticleEmitter(deoglRParticleEmitterInstance *emitter){
	const int index = pParticleEmitters.IndexOf(emitter);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pParticleEmitters.RemoveFrom(index);
	
	emitter->SetOctreeNode(NULL);
}

void deoglWorldOctree::RemoveAllParticleEmitters(){
	const int count = pParticleEmitters.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRParticleEmitterInstance*)pParticleEmitters.GetAt(i))->SetOctreeNode(NULL);
	}
	
	pParticleEmitters.RemoveAll();
}



// Private Functions
//////////////////////

deoglWorldOctree *deoglWorldOctree::pGetNodeFor(const decDVector &minExtend,
const decDVector &maxExtend, int maxDepth){
	deoglDOctree *curNode = this;
	int d;
	
	for(d=0; d<maxDepth; d++){
		deoglDOctree * const nextNode = curNode->GetNodeAtBox(minExtend, maxExtend);
		if(!nextNode){
			break;
		}
		curNode = nextNode;
	}
	
	return (deoglWorldOctree*)curNode;
}

deoglWorldOctree *deoglWorldOctree::pGetNodeFor(const decDVector &position, int maxDepth){
	deoglDOctree *curNode = this;
	deoglDOctree *nextNode;
	int d, octant;
	
	for(d=0; d<maxDepth; d++){
		octant = curNode->FindOctantAtPoint(position);
		if(octant == deoglDOctree::eoNotFound) break;
		
		nextNode = curNode->GetNodeAt(octant);
		if(!nextNode){
			nextNode = curNode->CreateOctree(octant);
			curNode->SetNodeAt(octant, nextNode);
		}
		
		curNode = nextNode;
	}
	
	return (deoglWorldOctree*)curNode;
}
