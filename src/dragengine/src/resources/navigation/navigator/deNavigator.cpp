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

#include "deNavigator.h"
#include "deNavigatorPath.h"
#include "deNavigatorManager.h"
#include "deNavigatorType.h"
#include "../space/deNavigationSpace.h"
#include "../../../common/exceptions.h"
#include "../../../systems/modules/ai/deBaseAINavigator.h"



// Class deNavigator
//////////////////////

// Constructor, destructor
////////////////////////////

deNavigator::deNavigator(deNavigatorManager *manager) :
deResource(manager),

pLayer(0),
pSpaceType(deNavigationSpace::estGrid),
pMaxOutsideDistance(0.5f),

pDefFixCost(0.0f),
pDefCostPerMeter(1.0f),
pBlockingCost(1000.0f),

pTypes(NULL),
pTypeCount(0),
pTypeSize(0),

pPeerAI(NULL),

pParentWorld(NULL),
pLLWorldPrev(NULL),
pLLWorldNext(NULL){
}

deNavigator::~deNavigator(){
	if(pPeerAI){
		delete pPeerAI;
		pPeerAI = NULL;
	}
	
	if(pTypes){
		delete [] pTypes;
	}
}



// Management
///////////////

void deNavigator::SetLayer(int layer){
	if(layer == pLayer){
		return;
	}
	
	pLayer = layer;
	
	if(pPeerAI){
		pPeerAI->LayerChanged();
	}
}

void deNavigator::SetSpaceType(deNavigationSpace::eSpaceTypes spaceType){
	if(spaceType < deNavigationSpace::estGrid || spaceType > deNavigationSpace::estVolume){
		DETHROW(deeInvalidParam);
	}
	
	if(spaceType == pSpaceType){
		return;
	}
	
	pSpaceType = spaceType;
	
	if(pPeerAI){
		pPeerAI->SpaceTypeChanged();
	}
}

void deNavigator::SetMaxOutsideDistance(float maxDistance){
	maxDistance = decMath::max(maxDistance, 0.0f);
	
	if(fabsf(maxDistance - pMaxOutsideDistance) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxOutsideDistance = maxDistance;
	
	if(pPeerAI){
		pPeerAI->ParametersChanged();
	}
}



void deNavigator::SetDefaultFixCost(float cost){
	if(fabsf(cost - pDefFixCost) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pDefFixCost = cost;
	
	if(pPeerAI){
		pPeerAI->CostsChanged();
	}
}

void deNavigator::SetDefaultCostPerMeter(float costPerMeter){
	if(fabsf(costPerMeter - pDefCostPerMeter) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pDefCostPerMeter = costPerMeter;
	
	if(pPeerAI){
		pPeerAI->CostsChanged();
	}
}

void deNavigator::SetBlockingCost(float cost){
	if(fabsf(cost - pBlockingCost) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBlockingCost = cost;
	
	if(pPeerAI){
		pPeerAI->CostsChanged();
	}
}



deNavigatorType *deNavigator::GetTypeAt(int index) const{
	if(index < 0 || index >= pTypeCount){
		DETHROW(deeInvalidParam);
	}
	return pTypes + index;
}

deNavigatorType *deNavigator::GetTypeWith(int typeValue) const{
	int i;
	
	for(i=0; i<pTypeCount; i++){
		if(pTypes[i].GetType() == typeValue){
			return pTypes + i;
		}
	}
	
	return NULL;
}

int deNavigator::IndexOfType(deNavigatorType *type) const{
	int i;
	
	for(i=0; i<pTypeCount; i++){
		if(pTypes + i == type){
			return i;
		}
	}
	
	return -1;
}

int deNavigator::IndexOfTypeWith(int typeValue) const{
	int i;
	
	for(i=0; i<pTypeCount; i++){
		if(pTypes[i].GetType() == typeValue){
			return i;
		}
	}
	
	return -1;
}

bool deNavigator::HasType(deNavigatorType *type) const{
	int i;
	
	for(i=0; i<pTypeCount; i++){
		if(pTypes + i == type){
			return true;
		}
	}
	
	return false;
}

bool deNavigator::HasTypeWith(int typeValue) const{
	int i;
	
	for(i=0; i<pTypeCount; i++){
		if(pTypes[i].GetType() == typeValue){
			return true;
		}
	}
	
	return false;
}

deNavigatorType *deNavigator::AddType(int type){
	deNavigatorType *rtype = GetTypeWith(type);
	
	if(!rtype){
		if(pTypeCount == pTypeSize){
			int newSize = pTypeCount + 1;
			deNavigatorType *newArray = new deNavigatorType[newSize];
			if(pTypes){
				memcpy(newArray, pTypes, sizeof(deNavigatorType) * pTypeCount);
				delete [] pTypes;
			}
			pTypes = newArray;
			pTypeSize = newSize;
		}
		
		pTypes[pTypeCount].SetType(type);
		pTypes[pTypeCount].SetFixCost(0.0f);
		pTypes[pTypeCount].SetCostPerMeter(1.0f);
		rtype = pTypes + pTypeCount;
		pTypeCount++;
	}
	
	return rtype;
}

void deNavigator::RemoveType(deNavigatorType *type){
	const int index = IndexOfType(type);
	int i;
	
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	for(i=index+1; i<pTypeCount; i++){
		pTypes[i - 1] = pTypes[i];
	}
	pTypeCount--;
}

void deNavigator::RemoveTypeWith(int typeValue){
	const int index = IndexOfTypeWith(typeValue);
	int i;
	
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	for(i=index+1; i<pTypeCount; i++){
		pTypes[i - 1] = pTypes[i];
	}
	pTypeCount--;
}

void deNavigator::RemoveAllTypes(){
	pTypeCount = 0;
}

void deNavigator::NotifyTypesChanged(){
	if(pPeerAI){
		pPeerAI->TypesChanged();
	}
}



void deNavigator::FindPath(deNavigatorPath &path, const decDVector &start, const decDVector &goal){
	if(pPeerAI){
		pPeerAI->FindPath(path, start, goal);
	}
}



// Testing
////////////

bool deNavigator::NearestPoint(const decDVector &point, float radius,
decDVector &nearestPoint, int &nearestType) const{
	if(!pPeerAI){
		return false;
	}
	return pPeerAI->NearestPoint(point, radius, nearestPoint, nearestType);
}

bool deNavigator::LineCollide(const decDVector &origin, const decVector &direction,
float &distance) const{
	if(!pPeerAI){
		return false;
	}
	return pPeerAI->LineCollide(origin, direction, distance);
}



// Path Collision
///////////////////

bool deNavigator::PathCollideRay(const deNavigatorPath &path, deCollider &collider,
int &hitAfterPoint, float &hitDistance) const{
	if(!pPeerAI){
		return false;
	}
	return pPeerAI->PathCollideRay(path, collider, hitAfterPoint, hitDistance);
}

bool deNavigator::PathCollideRay(const deNavigatorPath &path, deCollider &collider,
const decDVector &startPosition, int nextPoint, float maxDistance, int &hitAfterPoint,
float &hitDistance) const{
	if(!pPeerAI){
		return false;
	}
	return pPeerAI->PathCollideRay(path, collider, startPosition, nextPoint, maxDistance,
		hitAfterPoint, hitDistance);
}

bool deNavigator::PathCollideShape(const deNavigatorPath &path, deCollider &collider,
deCollider &agent, int &hitAfterPoint, float &hitDistance) const{
	if(!pPeerAI){
		return false;
	}
	return pPeerAI->PathCollideShape(path, collider, agent, hitAfterPoint, hitDistance);
}

bool deNavigator::PathCollideShape(const deNavigatorPath &path, deCollider &collider,
deCollider &agent, const decDVector &startPosition, int nextPoint, float maxDistance,
int &hitAfterPoint, float &hitDistance) const{
	if(!pPeerAI){
		return false;
	}
	return pPeerAI->PathCollideShape(path, collider, agent, startPosition, nextPoint,
		maxDistance, hitAfterPoint, hitDistance);
}



// System Peers
/////////////////

void deNavigator::SetPeerAI(deBaseAINavigator *peer){
	if(pPeerAI){
		delete pPeerAI;
	}
	pPeerAI = peer;
}



// Linked List
////////////////

void deNavigator::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deNavigator::SetLLWorldPrev(deNavigator *navigator){
	pLLWorldPrev = navigator;
}

void deNavigator::SetLLWorldNext(deNavigator *navigator){
	pLLWorldNext = navigator;
}
