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
#include <stdlib.h>
#include <string.h>

#include "meHTVRuleMultiMath.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleMultiMath
/////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleMultiMath::meHTVRuleMultiMath() : meHTVRule(ertMultiMath, 2){
	pOperator = eopAdd;
	
	GetSlots().GetAt(eisValues)->SetIsInput(true);
	
	GetSlots().GetAt(eosResult)->SetIsInput(false);
}

meHTVRuleMultiMath::meHTVRuleMultiMath(const meHTVRuleMultiMath &rule) :
meHTVRule(rule),
pOperator(rule.pOperator){
}

meHTVRuleMultiMath::~meHTVRuleMultiMath(){
}



// Management
///////////////

void meHTVRuleMultiMath::SetOperator(eOperators oper){
	pOperator = oper;
}



float meHTVRuleMultiMath::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot != eosResult) DETHROW(deeInvalidParam);
	
	meHTVRSlot &inputValues = GetSlots().GetAt(eisValues);
	
	int l, linkCount = inputValues.GetLinks().GetCount();
	float value, result = 0.0f;
	
	switch(pOperator){
	case eopAdd:
		for(l=0; l<linkCount; l++){
			meHTVRLink &link = *inputValues.GetLinks().GetAt(l);
			
			if(l == 0){
				result = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
				
			}else{
				result += link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
			}
		}
		
	case eopMultiply:
		for(l=0; l<linkCount; l++){
			meHTVRLink &link = *inputValues.GetLinks().GetAt(l);
			
			if(l == 0){
				result = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
				
			}else{
				result *= link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
			}
		}
		
	case eopMinimum:
		for(l=0; l<linkCount; l++){
			meHTVRLink &link = *inputValues.GetLinks().GetAt(l);
			
			if(l == 0){
				result = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
				
			}else{
				value = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
				
				if(value < result) result = value;
			}
		}
		
	case eopMaximum:
		for(l=0; l<linkCount; l++){
			meHTVRLink &link = *inputValues.GetLinks().GetAt(l);
			
			if(l == 0){
				result = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
				
			}else{
				value = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
				
				if(value > result) result = value;
			}
		}
		
	case eopAverage:
		for(l=0; l<linkCount; l++){
			meHTVRLink &link = *inputValues.GetLinks().GetAt(l);
			
			if(l == 0){
				result = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
				
			}else{
				result += link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
			}
		}
		
		if(linkCount > 0){
			result /= (float)linkCount;
		}
	}
	
	return result;
}

decVector meHTVRuleMultiMath::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	float value = GetOutputSlotValueAt(slot, evalEnv);
	
	return decVector(value, value, value);
}

meHTVRule::Ref meHTVRuleMultiMath::Copy() const{
	return meHTVRuleMultiMath::Ref::New(*this);
}
