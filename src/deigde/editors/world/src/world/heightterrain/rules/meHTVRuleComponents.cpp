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

#include "meHTVRuleComponents.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleComponents
//////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleComponents::meHTVRuleComponents() : meHTVRule(ertComponents, 4){
	GetSlots().GetAt(eisVector)->SetIsInput(true);
	
	GetSlots().GetAt(eosX)->SetIsInput(false);
	GetSlots().GetAt(eosY)->SetIsInput(false);
	GetSlots().GetAt(eosZ)->SetIsInput(false);
}

meHTVRuleComponents::meHTVRuleComponents(const meHTVRuleComponents &rule) :
meHTVRule(rule),
pVector(rule.pVector){
}

meHTVRuleComponents::~meHTVRuleComponents(){
}



// Management
///////////////

void meHTVRuleComponents::SetVector(const decVector &vector){
	pVector = vector;
}



float meHTVRuleComponents::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot < 0 || slot > 2) DETHROW(deeInvalidParam);
	
	meHTVRSlot &slotResult = GetSlots().GetAt(eisVector);
	
	if(slotResult.GetLinks().IsNotEmpty()){
		meHTVRLink &link = *slotResult.GetLinks().GetAt(0);
		
		decVector vector = link.GetSourceRule()->GetOutputSlotVectorAt(link.GetSourceSlot(), evalEnv);
		
		if(slot == eosX){
			return vector.x;
			
		}else if(slot == eosY){
			return vector.y;
			
		}else{ // slot == eosZ
			return vector.z;
		}
		
	}else{
		if(slot == eosX){
			return pVector.x;
			
		}else if(slot == eosY){
			return pVector.y;
			
		}else{ // slot == eosZ
			return pVector.z;
		}
	}
}

decVector meHTVRuleComponents::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	float value = GetOutputSlotValueAt(slot, evalEnv);
	
	return decVector(value, value, value);
}

meHTVRule::Ref meHTVRuleComponents::Copy() const{
	return meHTVRuleComponents::Ref::New(*this);
}
