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

#include "meHTVRuleMapping.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleMapping
///////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleMapping::meHTVRuleMapping() : meHTVRule(ertMapping, 4){
	pValue = 0.5f;
	pLower = 0.0f;
	pUpper = 1.0f;
	pInversed = false;
	
	GetSlotAt(eisLower).SetIsInput(true);
	GetSlotAt(eisUpper).SetIsInput(true);
	GetSlotAt(eisValue).SetIsInput(true);
	
	GetSlotAt(eosValue).SetIsInput(false);
}

meHTVRuleMapping::meHTVRuleMapping(const meHTVRuleMapping &rule) :
meHTVRule(rule),
pValue(rule.pValue),
pLower(rule.pLower),
pUpper(rule.pUpper),
pInversed(rule.pInversed){
}

meHTVRuleMapping::~meHTVRuleMapping(){
}



// Management
///////////////

void meHTVRuleMapping::SetValue(float value){
	pValue = value;
}

void meHTVRuleMapping::SetLower(float lower){
	pLower = lower;
}

void meHTVRuleMapping::SetUpper(float upper){
	pUpper = upper;
}

void meHTVRuleMapping::SetInversed(bool inversed){
	pInversed = inversed;
}



float meHTVRuleMapping::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot != eosValue) DETHROW(deeInvalidParam);
	
	meHTVRSlot &inputValue = GetSlotAt(eisValue);
	meHTVRSlot &inputLower = GetSlotAt(eisLower);
	meHTVRSlot &inputUpper = GetSlotAt(eisUpper);
	
	float value = pValue;
	float lower = pLower;
	float upper = pUpper;
	
	if(inputValue.GetLinkCount() > 0){
		meHTVRLink &link = *inputValue.GetLinkAt(0);
		
		value = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
	}
	
	if(inputLower.GetLinkCount() > 0){
		meHTVRLink &link = *inputLower.GetLinkAt(0);
		
		lower = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
	}
	
	if(inputUpper.GetLinkCount() > 0){
		meHTVRLink &link = *inputUpper.GetLinkAt(0);
		
		upper = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
	}
	
	float diff = upper - lower;
	
	if(diff < 1e-5f){
		return 0.0f;
		
	}else{
		if(pInversed){
			if(value <= lower){
				return 1.0f;
				
			}else if(value >= upper){
				return 0.0f;
				
			}else{
				return (upper - value) / diff;
			}
			
		}else{
			if(value <= lower){
				return 0.0f;
				
			}else if(value >= upper){
				return 1.0f;
				
			}else{
				return (value - lower) / diff;
			}
		}
	}
}

decVector meHTVRuleMapping::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	float value = GetOutputSlotValueAt(slot, evalEnv);
	
	return decVector(value, value, value);
}

meHTVRule *meHTVRuleMapping::Copy() const{
	return new meHTVRuleMapping(*this);
}
