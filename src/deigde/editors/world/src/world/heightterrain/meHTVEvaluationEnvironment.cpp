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

#include "meHTVEvaluationEnvironment.h"
#include "meHTVegetationLayer.h"
#include "rules/meHTVRule.h"
#include "rules/meHTVRuleClosestProp.h"
#include "rules/meHTVRulePropCount.h"
#include "../meWorld.h"
#include "../object/meObject.h"
#include "../terrain/meHeightTerrain.h"
#include "../terrain/meHeightTerrainSector.h"
#include "../terrain/meHeightTerrainTexture.h"
#include "../terrain/meHeightTerrainPropField.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/propfield/dePropField.h>



// Class meHTVEvaluationEnvironment
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVEvaluationEnvironment::meHTVEvaluationEnvironment(){
	pWorld = NULL;
	pHTSector = NULL;
	pHTDominantTexture = NULL;
	pVLayer = NULL;
	pPropField = NULL;
	pOccupation = NULL;
	pProbability = 0.0f;
	pVariation = 0;
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
}

meHTVEvaluationEnvironment::~meHTVEvaluationEnvironment(){
	RemoveAllObjects();
	if(pObjects) delete [] pObjects;
}



// Management
///////////////

void meHTVEvaluationEnvironment::SetWorld(meWorld *world){
	pWorld = world;
}



void meHTVEvaluationEnvironment::SetPosition(const decDVector &position){
	pPosition = position;
}

void meHTVEvaluationEnvironment::SetNormal(const decVector &normal){
	pNormal = normal;
}

void meHTVEvaluationEnvironment::SetParameters(const decDVector &position, const decVector &normal){
	pPosition = position;
	pNormal = normal;
}



void meHTVEvaluationEnvironment::SetHTSector(meHeightTerrainSector *htsector){
	pHTSector = htsector;
}

void meHTVEvaluationEnvironment::SetHTDominantTexture(meHeightTerrainTexture *texture){
	pHTDominantTexture = texture;
}

void meHTVEvaluationEnvironment::SetHTCoordinates(const decVector2 &coordinates){
	pHTCoordinates = coordinates;
	
	pHTCoordInt.x = (int)coordinates.x;
	pHTCoordInt.y = (int)coordinates.y;
	
	pHTCoordFract.x = coordinates.x - (float)pHTCoordInt.x;
	pHTCoordFract.y = coordinates.y - (float)pHTCoordInt.y;
}

void meHTVEvaluationEnvironment::SetHTCoordinates(float x, float y){
	pHTCoordinates.Set(x, y);
	
	pHTCoordInt.x = (int)x;
	pHTCoordInt.y = (int)y;
	
	pHTCoordFract.x = x - (float)pHTCoordInt.x;
	pHTCoordFract.y = y - (float)pHTCoordInt.y;
}



void meHTVEvaluationEnvironment::SetVLayer(meHTVegetationLayer *vlayer){
	pVLayer = vlayer;
}

void meHTVEvaluationEnvironment::SetPropField(meHeightTerrainPropField *propField){
	pPropField = propField;
}

void meHTVEvaluationEnvironment::SetOccupation(meBitArray *occupation){
	pOccupation = occupation;
}



void meHTVEvaluationEnvironment::SetProbability(float probability){
	pProbability = probability;
}

void meHTVEvaluationEnvironment::SetVariation(int variation){
	pVariation = variation;
}



void meHTVEvaluationEnvironment::Prepare(){
	PopulateWithObjects();
}

void meHTVEvaluationEnvironment::EvaluateRules(){
	if(pVLayer){
		pVLayer->EvaluateRules(*this);
	}
}



// Objects
////////////

meObject *meHTVEvaluationEnvironment::GetObjectAt(int index) const{
	if(index < 0 || index >= pObjectCount) DETHROW(deeInvalidParam);
	
	return pObjects[index];
}

void meHTVEvaluationEnvironment::AddObject(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	
	if(pObjectCount == pObjectSize){
		int newSize = pObjectSize * 3 / 2 + 1;
		meObject **newArray = new meObject*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pObjects){
			memcpy(newArray, pObjects, sizeof(meObject*) * pObjectSize);
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = newSize;
	}
	
	pObjects[pObjectCount++] = object;
}

void meHTVEvaluationEnvironment::RemoveAllObjects(){
	pObjectCount = 0;
}



void meHTVEvaluationEnvironment::PopulateWithObjects(){
	RemoveAllObjects();
	
	if(pWorld && pPropField && pVLayer){
		double pfsize, sectorDim = (double)pHTSector->GetHeightTerrain()->GetSectorSize();
		dePropField *engPF = pPropField->GetEnginePropField();
		int r, ruleCount = pVLayer->GetRuleCount();
		float maxObjectSearchRadius = 0.0f;
		double oa1x, oa1z, oa2x, oa2z;
		int sa1x, sa1z, sa2x, sa2z;
		const meHTVRule *rule;
		decDVector objpos;
		decPoint3 s;
		
		// determine the maximum search radius used in the rules
		for(r=0; r<ruleCount; r++){
			rule = pVLayer->GetRuleAt(r);
			
			if(rule->GetType() == meHTVRule::ertClosestProp){
				const meHTVRuleClosestProp &ruleCP = *((meHTVRuleClosestProp*)rule);
				
				if(ruleCP.GetSearchRadius() > maxObjectSearchRadius){
					maxObjectSearchRadius = ruleCP.GetSearchRadius();
				}
				
			}else if(rule->GetType() == meHTVRule::ertPropCount){
				const meHTVRulePropCount &rulePC = *((meHTVRulePropCount*)rule);
				
				if(rulePC.GetSearchRadius() > maxObjectSearchRadius){
					maxObjectSearchRadius = rulePC.GetSearchRadius();
				}
			}
		}
		
		// determine the range of sectors to test. usually this is only the
		// sector the prop field is in but potentially could be more if the
		// prop field is on the edge of a sector or the object search radius
		// is large
		if(engPF){
			const decDVector &pfpos = engPF->GetPosition();
			
			pfsize = (sectorDim / (double)pHTSector->GetPropFieldCellCount()) + maxObjectSearchRadius;
			
			oa1x = pfpos.x - pfsize;
			oa1z = pfpos.z - pfsize;
			oa2x = pfpos.x + pfsize;
			oa2z = pfpos.z + pfsize;
			
			sa1x = (int)floor((oa1x - 0.01) / sectorDim + 0.5);
			sa1z = (int)floor((oa1z - 0.01) / sectorDim + 0.5);
			sa2x = (int)floor((oa2x + 0.01) / sectorDim + 0.5);
			sa2z = (int)floor((oa2z + 0.01) / sectorDim + 0.5);
			
			if(sa1x < 0) sa1x = 0;
			if(sa1z < 0) sa1z = 0;
			if(sa2x >= 0/*worldSize.x*/) sa2x = 0/*worldSize.x - 1*/;
			if(sa2z >= 0/*worldSize.z*/) sa2z = 0/*worldSize.z - 1*/;
			
		}else{
			return;
		}
		
		// test all objects. we do not care if the objects are affected by the rules or not as
		// the rules itself are going to check this already. this could be improved later on.
		const meObjectList &objects = pWorld->GetObjects();
		const int count = objects.GetCount();
		int i;
		for(i=0; i<count; i++){
			meObject * const object = objects.GetAt(i);
			objpos = object->GetPosition();
			
			if(objpos.x >= oa1x && objpos.z >= oa1z && objpos.x <= oa2x && objpos.z <= oa2z){
				AddObject(object);
			}
		}
	}
}
