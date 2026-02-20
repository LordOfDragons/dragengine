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

#include "meHTVRuleClosestProp.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../../meWorld.h"
#include "../../object/meObject.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleClosestProp
///////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleClosestProp::meHTVRuleClosestProp() : meHTVRule(ertClosestProp, 2){
	pSearchRadius = 1.0f;
	
	GetSlots().GetAt(eosDirection)->SetIsInput(false);
	GetSlots().GetAt(eosDistance)->SetIsInput(false);
	
	Reset();
}

meHTVRuleClosestProp::meHTVRuleClosestProp(const meHTVRuleClosestProp &rule) :
meHTVRule(rule),
pPropClass(rule.pPropClass),
pSearchRadius(rule.pSearchRadius),
pDistance(rule.pDistance),
pDirection(rule.pDirection),
pDirty(false){
}

meHTVRuleClosestProp::~meHTVRuleClosestProp(){
}



// Management
///////////////

void meHTVRuleClosestProp::SetPropClass(const char *propClass){
	if(!propClass) DETHROW(deeInvalidParam);
	
	pPropClass = propClass;
	
	Reset();
}

void meHTVRuleClosestProp::SetSearchRadius(float searchRadius){
	pSearchRadius = searchRadius;
	
	Reset();
}

void meHTVRuleClosestProp::UpdateResult(meHTVEvaluationEnvironment &evalEnv){
	if(pDirty){
		const decDVector &ipos = evalEnv.GetPosition();
		meObject *bestObject = nullptr;
		decVector direction, bestDireciton;
		float distance, bestDistance = 0;
		
		evalEnv.GetObjects().Visit([&](meObject *object){
			if(pPropClass.Equals(object->GetClassName())){
				direction = (object->GetPosition() - ipos).ToVector();
				distance = direction.Length();
				if(distance <= pSearchRadius){
					if(!bestObject || distance < bestDistance){
						bestObject = object;
						bestDistance = distance;
						bestDireciton = direction;
					}
				}
			}
		});
		
		if(bestObject){
			pDistance = bestDistance;
			if(bestDistance == 0.0f){
				pDirection.Set(0.0f, 1.0f, 0.0f);
				
			}else{
				pDirection = bestDireciton / bestDistance;
			}
			
		}else{
			pDistance = pSearchRadius;
			pDirection.Set(0.0f, 1.0f, 0.0f);
		}
		
		// no more dirty
		pDirty = false;
	}
}



void meHTVRuleClosestProp::Reset(){
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

float meHTVRuleClosestProp::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot < 0 || slot > 1) DETHROW(deeInvalidParam);
	
	UpdateResult(evalEnv);
	
	if(slot == eosDistance){
		return pDistance;
		
	}else{ // slot == eosDirection // invalid usage. in this case we return the y coordinate
		return pDirection.y;
	}
}

decVector meHTVRuleClosestProp::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot < 0 || slot > 1) DETHROW(deeInvalidParam);
	
	UpdateResult(evalEnv);
	
	if(slot == eosDistance){
		return decVector(pDistance, pDistance, pDistance);
		
	}else{ // slot == eosDirection
		return pDirection;
	}
}

meHTVRule::Ref meHTVRuleClosestProp::Copy() const{
	return meHTVRuleClosestProp::Ref::New(*this);
}
