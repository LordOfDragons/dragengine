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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglCollideList.h"
#include "deoglCollideListComponent.h"
#include "deoglCollideListLight.h"
#include "deoglCollideListPropField.h"
#include "deoglCollideListPropFieldType.h"
#include "deoglCollideListPropFieldCluster.h"
#include "deoglCollideListHTSector.h"
#include "deoglCollideListHTSCluster.h"
#include "deoglCLVisitorElements.h"

#include "../component/deoglRComponent.h"
#include "../billboard/deoglRBillboard.h"
#include "../light/deoglRLight.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../propfield/deoglPropFieldCluster.h"
#include "../propfield/deoglRPropField.h"
#include "../propfield/deoglRPropFieldType.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../skin/deoglRSkin.h"
#include "../terrain/heightmap/deoglHTSCluster.h"
#include "../terrain/heightmap/deoglHTView.h"
#include "../terrain/heightmap/deoglHTViewSector.h"
#include "../terrain/heightmap/deoglRHTSector.h"
#include "../visitors/deoglTransformVolume.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"
#include "../utils/collision/deoglDCollisionVolume.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionFrustum.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decGlobalFunctions.h>



// Class deoglCollideList
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideList::deoglCollideList(){
	pTransformVolume = nullptr;
	
	try{
		pTransformVolume = new deoglTransformVolume;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglCollideList::~deoglCollideList(){
	pCleanUp();
}



// Management
///////////////

void deoglCollideList::Clear(){
	RemoveAllPropFieldClusters();
	RemoveAllPropFields();
	RemoveAllHTSClusters();
	RemoveAllHTSectors();
	RemoveAllBillboards();
	RemoveAllLights();
	RemoveAllComponents();
	pParticleEmitterList.RemoveAll();
	pEnvMapList.RemoveAll();
}

void deoglCollideList::SortLinear(const decVector &pos, const decVector &view){
}

void deoglCollideList::SortRadial(const decVector &pos){
}

void deoglCollideList::FlagAllLit(bool lit){
	pComponents.Visit([&](deoglCollideListComponent &component){
		component.GetComponent()->SetLit(lit);
	});
}

void deoglCollideList::AddElementsColliding(deoglWorldOctree *octree, deoglDCollisionVolume *volume){
	if(!octree || !volume) DETHROW(deeInvalidParam);
	deoglCLVisitorElements visitor(this, volume);
	decDVector minExtend, maxExtend;
	deoglDCollisionBox box;
	
	volume->GetEnclosingBox(&box);
	minExtend = box.GetCenter() - box.GetHalfSize();
	maxExtend = box.GetCenter() + box.GetHalfSize();
	
	visitor.SetVisitAll(true);
	
	octree->VisitNodesColliding(&visitor, minExtend, maxExtend);
}

void deoglCollideList::AddElementsColliding(deoglWorldOctree *octree, deoglDCollisionFrustum *volume){
	if(!octree || !volume) DETHROW(deeInvalidParam);
	// HACK: frustum does not support GetEnclosingBox yet so we cheat in this particular case
	// TODO: fix the enclosing box to improve speed
	deoglCLVisitorElements visitor(this, volume);
//	decVector minExtend, maxExtend;
//	deoglDCollisionBox box;
	
//	volume->GetEnclosingBox( &box );
//	minExtend = box.GetCenter() - box.GetHalfSize();
//	maxExtend = box.GetCenter() + box.GetHalfSize();
	
	visitor.SetVisitAll(true);
	
//	octree->VisitNodesColliding( &visitor, minExtend, maxExtend );
	octree->VisitNodesCollidingVolume(&visitor, volume);
}

void deoglCollideList::RemoveCulledElements(){
	RemoveCulledComponents();
	RemoveCulledLights();
	RemoveCulledBillboards();
}

void deoglCollideList::UpdateCubeFaceMasks(const decDVector &position) const{
	pComponents.Visit([&](deoglCollideListComponent &component){
		component.UpdateCubeFaceMask(position);
		component.SetSpecialFlags(component.GetCubeFaceMask());
	});
	
	// TODO same for billboard
}

void deoglCollideList::UpdateOccMeshCubeFaceMasks(const decDVector &position) const{
	pComponents.Visit([&](deoglCollideListComponent &component){
		if(component.GetComponent()->GetOcclusionMesh()){
			component.UpdateCubeFaceMask(position);
			component.SetSpecialFlags(component.GetCubeFaceMask());
		}
	});
}



void deoglCollideList::AddParticleEmittersColliding(deoglWorldOctree &octree, deoglDCollisionVolume *volume){
	if(!volume){
		DETHROW(deeInvalidParam);
	}
	
	deoglCLVisitorElements visitor(this, volume);
	
	visitor.SetVisitAll(false);
	visitor.SetVisitParticleEmitters(true);
	
	octree.VisitNodesCollidingVolume(&visitor, volume);
}



void deoglCollideList::AddEnvironmentMapsColliding(deoglWorldOctree &octree, deoglDCollisionVolume *volume){
}

void deoglCollideList::DebugSingleLine(deoglRTLogger &logger, bool sorted) const{
	decStringList pointers;
	decString string;
	
	pComponents.Visit([&](const deoglCollideListComponent &component){
		decString temp;
		temp.Format("%p", component.GetComponent());
		pointers.Add(temp);
	});
	if(sorted) pointers.SortAscending();
	string.AppendFormat("c[%s]", DEJoin(pointers, ", ").GetString());
	
	pBillboards.VisitIndexed([&](int i, const deoglRBillboard *billboard){
		decString temp;
		temp.Format("%p", billboard);
		pointers.Add(temp);
	});
	if(sorted) pointers.SortAscending();
	string.AppendFormat(" b[%s]", DEJoin(pointers, ", ").GetString());
	
	logger.LogInfoFormat("CollideList %p: %s", this, string.GetString());
}



// Components
///////////////

deoglCollideListComponent &deoglCollideList::GetComponentAt(int index){
	return pComponents.GetAt(index);
}

const deoglCollideListComponent &deoglCollideList::GetComponentAt(int index) const{
	return pComponents.GetAt(index);
}

int deoglCollideList::IndexOfComponent(deoglRComponent *component) const{
	DEASSERT_NOTNULL(component)
	
	return pComponents.IndexOfMatching([&](const deoglCollideListComponent &clc){
		return clc.GetComponent() == component;
	});
}

bool deoglCollideList::HasComponent(deoglRComponent *component) const{
	DEASSERT_NOTNULL(component)
	
	return pComponents.HasMatching([&](const deoglCollideListComponent &clc){
		return clc.GetComponent() == component;
	});
}

deoglCollideListComponent &deoglCollideList::AddComponent(deoglRComponent *component){
	DEASSERT_NOTNULL(component)
	
	pComponents.Add(deoglCollideListComponent(component));
	return pComponents.Last();
}

void deoglCollideList::RemoveComponent(deoglRComponent *component){
	RemoveComponentFrom(IndexOfComponent(component));
}

void deoglCollideList::RemoveComponentFrom(int index){
	DEASSERT_TRUE(index >= 0)
	pComponents.Move(pComponents.GetCount() - 1, index);
	pComponents.RemoveFrom(pComponents.GetCount() - 1);
}

void deoglCollideList::RemoveAllComponents(){
	pComponents.RemoveAll();
}



void deoglCollideList::AddComponentsColliding(deoglWorldOctree &octree, deoglDCollisionVolume *volume){
	if(!volume){
		DETHROW(deeInvalidParam);
	}
	
	deoglCLVisitorElements visitor(this, volume);
	visitor.SetVisitAll(false);
	visitor.SetVisitComponents(true);
	
	octree.VisitNodesCollidingVolume(&visitor, volume);
}

void deoglCollideList::RemoveCulledComponents(){
	pComponents.RemoveIf([](deoglCollideListComponent &component){
		return component.GetCulled();
	});
}

void deoglCollideList::RemoveSolidComponents(){
	pComponents.RemoveIf([](deoglCollideListComponent &component){
		const deoglRSkin * const skin = component.GetComponent()->GetSkin();
		return skin && !skin->GetIsSolid();
	});
}

void deoglCollideList::SortComponentsByModels(){
	pComponents.Sort([](const deoglCollideListComponent &a, const deoglCollideListComponent &b){
		return DECompare(a.GetComponent()->GetModel().Pointer(), b.GetComponent()->GetModel().Pointer());
	});
}

void deoglCollideList::SortComponentsByDistance(const decVector &pos, const decVector &view){
	if(pComponents.IsEmpty()){
		return;
	}
	
	// calculate distance to reference point using view direction as projection axis
	const decVector normView(view.Normalized());
	pComponents.Visit([&](deoglCollideListComponent &c){
		c.GetComponent()->SetSortDistance(normView *
			(c.GetComponent()->GetMatrix().GetPosition().ToVector() - pos));
	});
	
	// sort by distance
	pComponents.Sort([](const deoglCollideListComponent &a, const deoglCollideListComponent &b){
		return DECompare(a.GetComponent()->GetSortDistance(), b.GetComponent()->GetSortDistance());
	});
}

void deoglCollideList::MarkComponents(bool marked) const{
	pComponents.Visit([&](deoglCollideListComponent &component){
		component.GetComponent()->SetMarked(marked);
	});
}



void deoglCollideList::LogComponents() const{
	if(pComponents.IsEmpty()){
		return;
	}
	
	deoglRenderThread &renderThread = pComponents.First().GetComponent()->GetRenderThread();
	pComponents.VisitIndexed([&](int i, const deoglCollideListComponent &c){
		const deoglRComponent * const component = c.GetComponent();
		const deoglRModel * const model = component->GetModel();
		const decDVector pos(component->GetMatrix().GetPosition());
		
		if(model){
			const deoglModelLOD &modelLOD = model->GetLODAt(c.GetLODLevel());
			renderThread.GetLogger().LogInfoFormat("%i: comp=%p pos=(%.3f,%.3f,%.3f) model=%s tex=%i faces=%i",
				i, component, pos.x, pos.y, pos.z, model->GetFilename().GetString(),
				modelLOD.GetTextureCount(), modelLOD.GetFaceCount());
			
		}else{
			renderThread.GetLogger().LogInfoFormat("%i: comp=%p pos=(%.3f,%.3f,%.3f) model=-",
				i, component, pos.x, pos.y, pos.z);
		}
	});
}



// Lights
///////////

deoglCollideListLight &deoglCollideList::GetLightAt(int index){
	return pLights.GetAt(index);
}

const deoglCollideListLight &deoglCollideList::GetLightAt(int index) const{
	return pLights.GetAt(index);
}

int deoglCollideList::IndexOfLight(deoglRLight *light) const{
	DEASSERT_NOTNULL(light)
	
	return pLights.IndexOfMatching([&](const deoglCollideListLight &l){
		return l.GetLight() == light;
	});
}

bool deoglCollideList::HasLight(deoglRLight *light) const{
	DEASSERT_NOTNULL(light)
	
	return pLights.HasMatching([&](const deoglCollideListLight &l){
		return l.GetLight() == light;
	});
}

deoglCollideListLight &deoglCollideList::AddLight(deoglRLight *light){
	DEASSERT_NOTNULL(light)
	
	pLights.Add({light});
	return pLights.Last();
}

void deoglCollideList::RemoveLight(deoglRLight *light){
	RemoveLightFrom(IndexOfLight(light));
}

void deoglCollideList::RemoveLightFrom(int index){
	DEASSERT_TRUE(index >= 0)
	
	pLights.Move(pLights.GetCount() - 1, index);
	pLights.RemoveFrom(pLights.GetCount() - 1);
}

void deoglCollideList::RemoveAllLights(){
	pLights.RemoveAll();
}

void deoglCollideList::AddLightsColliding(deoglWorldOctree *octree, deoglDCollisionVolume *colVol){
	if(!octree || !colVol){
		DETHROW(deeInvalidParam);
	}
	
	deoglCLVisitorElements visitor(this, colVol);
	visitor.SetVisitAll(false);
	visitor.SetVisitLights(true);
	
	octree->VisitNodesCollidingVolume(&visitor, colVol);
}

void deoglCollideList::MarkLightsCulled(bool culled){
	pLights.Visit([&](deoglCollideListLight &light){
		light.SetCulled(culled);
	});
}

void deoglCollideList::RemoveCulledLights(){
	pLights.RemoveIf([](const deoglCollideListLight &light){
		return light.GetCulled();
	});
}



// Billboards
///////////////

deoglRBillboard *deoglCollideList::GetBillboardAt(int index) const{
	return pBillboards.GetAt(index);
}

void deoglCollideList::AddBillboard(deoglRBillboard *billboard){
	DEASSERT_NOTNULL(billboard)
	
	pBillboards.Add(billboard);
}

void deoglCollideList::RemoveAllBillboards(){
	pBillboards.RemoveAll();
}

void deoglCollideList::AddBillboardsColliding(deoglWorldOctree &octree, deoglDCollisionVolume *colVol){
	if(!colVol){
		DETHROW(deeInvalidParam);
	}
	deoglCLVisitorElements visitor(this, colVol);
	
	visitor.SetVisitAll(false);
	visitor.SetVisitBillboards(true);
	
	octree.VisitNodesCollidingVolume(&visitor, colVol);
}

void deoglCollideList::RemoveCulledBillboards(){
	pBillboards.RemoveIf([](const deoglRBillboard *billboard){
		return !billboard->GetVisible();
	});
}



// Height Terrain Sectors
///////////////////////////

deoglCollideListHTSector &deoglCollideList::GetHTSectorAt(int index){
	return pHTSectors.GetAt(index);
}

const deoglCollideListHTSector &deoglCollideList::GetHTSectorAt(int index) const{
	return pHTSectors.GetAt(index);
}

deoglCollideListHTSector &deoglCollideList::AddHTSector(deoglHTViewSector *sector){
	DEASSERT_NOTNULL(sector)
	
	pHTSectors.Add({sector});
	return pHTSectors.Last();
}

void deoglCollideList::AddHTSector(deoglHTViewSector *sector, deoglDCollisionVolume *volume){
	deoglCollideListHTSector &clsector = AddHTSector(sector);
	bool empty = true;
	
	if(volume){
		const deoglRHTSector &htsector = sector->GetSector();
		const int count = htsector.GetClusterCount();
		const deoglHTSCluster *cluster = htsector.GetClusters();
		deoglDCollisionBox box;
		decPoint i;
		
		for(i.y=0; i.y<count; i.y++){
			for(i.x=0; i.x<count; i.x++, cluster++){
				box.SetCenter(cluster->GetCenter());
				box.SetHalfSize(cluster->GetHalfExtends());
				
				if(volume->BoxHitsVolume(&box)){
					clsector.AddCluster(i);
					empty = false;
				}
			}
		}
	}
	
	if(empty){
		pHTSectors.RemoveFrom(pHTSectors.GetCount() - 1);
	}
}

void deoglCollideList::RemoveAllHTSectors(){
	pHTSectors.RemoveAll();
}

void deoglCollideList::AddHTSectorsColliding(deoglHTView *htview, deoglDCollisionVolume *volume){
	if(!htview || !volume){
		DETHROW(deeInvalidParam);
	}
	
	//htview->DetermineVisibilityUsing( volume );
	
	const int count = htview->GetSectorCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglHTViewSector * const sector = htview->GetSectorAt(i);
		
		// TODO test if the sector is visible using a bounding box test
		
		AddHTSector(sector, volume);
	}
}



// Height Terrain Sector Clusters
///////////////////////////////////

deoglCollideListHTSCluster &deoglCollideList::GetHTSClusterAt(int index){
	return pHTSClusters.GetAt(index);
}

const deoglCollideListHTSCluster &deoglCollideList::GetHTSClusterAt(int index) const{
	return pHTSClusters.GetAt(index);
}

deoglCollideListHTSCluster &deoglCollideList::AddHTSCluster(deoglHTViewSectorCluster *cluster){
	DEASSERT_NOTNULL(cluster)
	
	pHTSClusters.Add({cluster});
	return pHTSClusters.Last();
}

void deoglCollideList::RemoveAllHTSClusters(){
	pHTSClusters.RemoveAll();
}



// Prop Fields
////////////////

deoglCollideListPropField &deoglCollideList::GetPropFieldAt(int index){
	return pPropFields.GetAt(index);
}

const deoglCollideListPropField &deoglCollideList::GetPropFieldAt(int index) const{
	return pPropFields.GetAt(index);
}

deoglCollideListPropField &deoglCollideList::AddPropField(deoglRPropField *propField){
	DEASSERT_NOTNULL(propField)
	
	pPropFields.Add({propField});
	return pPropFields.Last();
}

void deoglCollideList::AddPropField(deoglRPropField *propField, deoglDCollisionVolume &volume){
	const decDVector &pfpos = propField->GetPosition();
	const int typeCount = propField->GetTypeCount();
	deoglDCollisionBox box;
	int i, j;
	
	deoglCollideListPropField *clpropfield = nullptr;
	
	for(i=0; i<typeCount; i++){
		deoglRPropFieldType &type = propField->GetTypeAt(i);
		const int clusterCount = type.GetClusterCount();
		
		deoglCollideListPropFieldType *cltype = nullptr;
		
		for(j=0; j<clusterCount; j++){
			deoglPropFieldCluster &cluster = *type.GetClusterAt(j);
			
			const decDVector clusterMinExtend(pfpos + cluster.GetMinimumExtend());
			const decDVector clusterMaxExtend(pfpos + cluster.GetMaximumExtend());
			box.SetFromExtends(clusterMinExtend, clusterMaxExtend);
			
			if(!volume.BoxHitsVolume(&box)){
				continue;
			}
			
			if(!clpropfield){
				clpropfield = &AddPropField(propField);
			}
			if(!cltype){
				cltype = clpropfield->AddType(&type);
			}
			cltype->AddCluster(&cluster);
		}
	}
}

void deoglCollideList::RemoveAllPropFields(){
	pPropFields.RemoveAll();
}

void deoglCollideList::AddPropFieldsColliding(deoglRWorld &world, deoglDCollisionVolume &volume){
	const int count = world.GetPropFieldCount();
	deoglDCollisionBox box;
	int i;
	
	for(i=0; i<count; i++){
		deoglRPropField &propField = *world.GetPropFieldAt(i);
		const decDVector &minExtend = propField.GetMinimumExtend();
		const decDVector &maxExtend = propField.GetMaximumExtend();
		
		box.SetFromExtends(minExtend, maxExtend);
		if(volume.BoxHitsVolume(&box)){
			AddPropField(&propField, volume);
		}
	}
}



// Prop Field Clusters
////////////////////////

deoglCollideListPropFieldCluster &deoglCollideList::GetPropFieldClusterAt(int index){
	return pPropFieldClusters.GetAt(index);
}

const deoglCollideListPropFieldCluster &deoglCollideList::GetPropFieldClusterAt(int index) const{
	return pPropFieldClusters.GetAt(index);
}

deoglCollideListPropFieldCluster &deoglCollideList::AddPropFieldCluster(deoglPropFieldCluster *cluster){
	DEASSERT_NOTNULL(cluster)
	
	pPropFieldClusters.Add({cluster});
	return pPropFieldClusters.Last();
}

void deoglCollideList::RemoveAllPropFieldClusters(){
	pPropFieldClusters.RemoveAll();
}



#if 0
void deoglCollideList::AddParticlesColliding(deoglRWorld *world, deoglDCollisionVolume *volume){
	if(!world || !volume) DETHROW(deeInvalidParam);
	
	deWorld *engWorld = world->GetWorld();
	int p, psysCount = engWorld->GetParticleEmitterCount();
	deoglParticleEmitter *particleSystem;
	deParticleEmitter *engParticleEmitter;
	deParticleInstance *engInstances;
	deoglDCollisionSphere sphere;
	int i, instanceCount;
	
	for(p=0; p<psysCount; p++){
		engParticleEmitter = engWorld->GetParticleEmitterAt(p);
		particleSystem = (deoglParticleEmitter*)engParticleEmitter->GetPeerGraphic();
		
		if(particleSystem){
			instanceCount = engParticleEmitter->GetInstanceCount();
			engInstances = engParticleEmitter->GetInstances();
			
			for(i=0; i<instanceCount; i++){
				sphere.SetCenter(engParticleEmitter->GetEmitterPosition() + decDVector(engInstances[i].position));
				sphere.SetRadius((double)engInstances[i].size);
				
				if(volume->SphereHitsVolume(&sphere)){
					AddParticle(particleSystem, i);
				}
			}
		}
	}
}
#endif



// Private functions
//////////////////////

void deoglCollideList::pCleanUp(){
	Clear();
	
	if(pTransformVolume) delete pTransformVolume;
}
