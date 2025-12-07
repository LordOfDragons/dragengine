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

#include "meHTVRuleClosestVegetation.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVInstance.h"
#include "../meHTVEvaluationEnvironment.h"
#include "../../terrain/meHeightTerrain.h"
#include "../../terrain/meHeightTerrainSector.h"
#include "../../terrain/meHeightTerrainPropField.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/world/deWorld.h>



// Class meHTVRuleClosestVegetation
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleClosestVegetation::meHTVRuleClosestVegetation() : meHTVRule(ertClosestVegetation, 2){
	pSearchRadius = 1.0f;
	
	GetSlotAt(eosDirection).SetIsInput(false);
	GetSlotAt(eosDistance).SetIsInput(false);
	
	Reset();
}

meHTVRuleClosestVegetation::meHTVRuleClosestVegetation(const meHTVRuleClosestVegetation &rule) :
meHTVRule(rule),
pVegetationType(rule.pVegetationType),
pSearchRadius(rule.pSearchRadius),
pDistance(rule.pDistance),
pDirection(rule.pDirection),
pDirty(false){
}

meHTVRuleClosestVegetation::~meHTVRuleClosestVegetation(){
}



// Management
///////////////

void meHTVRuleClosestVegetation::SetVegetationType(const char *vegetationType){
	if(!vegetationType) DETHROW(deeInvalidParam);
	
	pVegetationType = vegetationType;
	
	Reset();
}

void meHTVRuleClosestVegetation::SetSearchRadius(float searchRadius){
	pSearchRadius = searchRadius;
	
	Reset();
}

void meHTVRuleClosestVegetation::UpdateResult(meHTVEvaluationEnvironment &evalEnv){
	if(!pDirty){
		return;
	}
	
	float srSquared = pSearchRadius * pSearchRadius;
	float bestDistSquared = srSquared;
	
	// for the time being we will restrict ourselves on looking in the current
	// prop field. this is not the best solution but works well enough
#if 0
	meHeightTerrainSector *htsector = evalEnv.GetHTSector();
	
	if(htsector && htsector->GetHeightTerrain() && false){
		const decDVector &ipos = evalEnv.GetPosition();
		meHeightTerrainPropField *htpf;
		int i, pfiCount = 0;
		int p, pfCount = 0;
		decVector testIPos;
		float distSquared;
		decVector posDiff;
		float pfDropDist;
		
		pfDropDist = htsector->GetHeightTerrain()->GetSectorSize() / (float)htsector->GetPropFieldCellCount();
		pfCount = htsector->GetPropFieldCount();
		
		for(p=0; p<pfCount; p++){
			htpf = htsector->GetPropFieldAt(p);
			if(htpf->GetEnginePropField()){
				testIPos = (ipos - htpf->GetEnginePropField()->GetPosition()).ToVector();
				
				if(testIPos.x >= -pfDropDist && testIPos.x <= pfDropDist && testIPos.z >= -pfDropDist && testIPos.z <= pfDropDist){
					pfiCount = htpf->GetVInstanceCount();
					
					for(i=0; i<pfiCount; i++){
						posDiff = htpf->GetVInstanceAt(i).GetPosition() - testIPos;
						distSquared = posDiff * posDiff;
						
						if(distSquared < bestDistSquared){
							bestDistSquared = distSquared;
						}
					}
				}
			}
		}
	}
#endif
	
	pDistance = sqrtf(bestDistSquared);
	
	// no more dirty
	pDirty = false;
}



void meHTVRuleClosestVegetation::Reset(){
	if(pSearchRadius < 0.001f){
		pDistance = 1.0f;
		pDirection.Set(0.0f, 0.0f, 1.0f);
		pDirty = false;
		
	}else{
		pDistance = pSearchRadius;
		pDirection.Set(0.0f, 0.0f, 1.0f);
		pDirty = true;
	}
}

float meHTVRuleClosestVegetation::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot < 0 || slot > 1) DETHROW(deeInvalidParam);
	
	UpdateResult(evalEnv);
	
	if(slot == eosDistance){
		return pDistance;
		
	}else{ // slot == eosDirection // invalid usage. in this case we return the y coordinate
		return pDirection.y;
	}
}

decVector meHTVRuleClosestVegetation::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot < 0 || slot > 1) DETHROW(deeInvalidParam);
	
	UpdateResult(evalEnv);
	
	if(slot == eosDistance){
		return decVector(pDistance, pDistance, pDistance);
		
	}else{ // slot == eosDirection
		return pDirection;
	}
}

meHTVRule *meHTVRuleClosestVegetation::Copy() const{
	return new meHTVRuleClosestVegetation(*this);
}
