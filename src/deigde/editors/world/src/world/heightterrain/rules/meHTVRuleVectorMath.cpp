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

#include "meHTVRuleVectorMath.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleVectorMath
//////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleVectorMath::meHTVRuleVectorMath() : meHTVRule(ertVectorMath, 4){
	pVectorA.Set(0.5f, 0.5f, 0.5f);
	pVectorB.Set(0.5f, 0.5f, 0.5f);
	pOperator = eopAdd;
	
	GetSlots().GetAt(eisVectorA)->SetIsInput(true);
	GetSlots().GetAt(eisVectorB)->SetIsInput(true);
	
	GetSlots().GetAt(eosValue)->SetIsInput(false);
	GetSlots().GetAt(eosVector)->SetIsInput(false);
}

meHTVRuleVectorMath::meHTVRuleVectorMath(const meHTVRuleVectorMath &rule) :
meHTVRule(rule),
pVectorA(rule.pVectorA),
pVectorB(rule.pVectorB),
pOperator(rule.pOperator){
}

meHTVRuleVectorMath::~meHTVRuleVectorMath(){
}



// Management
///////////////

void meHTVRuleVectorMath::SetVectorA(const decVector &vector){
	pVectorA = vector;
}

void meHTVRuleVectorMath::SetVectorB(const decVector &vector){
	pVectorB = vector;
}

void meHTVRuleVectorMath::SetOperator(eOperators oper){
	pOperator = oper;
}



float meHTVRuleVectorMath::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot < 0 || slot > 1) DETHROW(deeInvalidParam);
	
	meHTVRSlot &inputVectorA = GetSlots().GetAt(eisVectorA);
	meHTVRSlot &inputVectorB = GetSlots().GetAt(eisVectorB);
	
	decVector vectorA(pVectorA);
	decVector vectorB(pVectorB);
		
	if(inputVectorA.GetLinks().IsNotEmpty()){
		meHTVRLink &link = *inputVectorA.GetLinks().GetAt(0);
		
		vectorA = link.GetSourceRule()->GetOutputSlotVectorAt(link.GetSourceSlot(), evalEnv);
	}
	
	if(inputVectorB.GetLinks().IsNotEmpty()){
		meHTVRLink &link = *inputVectorB.GetLinks().GetAt(0);
		
		vectorB = link.GetSourceRule()->GetOutputSlotVectorAt(link.GetSourceSlot(), evalEnv);
	}
	
	if(slot == eosVector){ // invalid case, return 0
		return 0.0f;
		
	}else{ // slot == eosValue
		if(pOperator == eopDot){
			return vectorA * vectorB;
			
		}else{ // all other operators are invalid, return 0 in that case
			return 0.0f;
		}
	}
}

decVector meHTVRuleVectorMath::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot < 0 || slot > 1) DETHROW(deeInvalidParam);
	
	meHTVRSlot &inputVectorA = GetSlots().GetAt(eisVectorA);
	meHTVRSlot &inputVectorB = GetSlots().GetAt(eisVectorB);
	
	decVector vectorA(pVectorA);
	decVector vectorB(pVectorB);
	
	if(inputVectorA.GetLinks().IsNotEmpty()){
		meHTVRLink &link = *inputVectorA.GetLinks().GetAt(0);
		
		vectorA = link.GetSourceRule()->GetOutputSlotVectorAt(link.GetSourceSlot(), evalEnv);
	}
	
	if(inputVectorB.GetLinks().IsNotEmpty()){
		meHTVRLink &link = *inputVectorB.GetLinks().GetAt(0);
		
		vectorB = link.GetSourceRule()->GetOutputSlotVectorAt(link.GetSourceSlot(), evalEnv);
	}
	
	if(slot == eosVector){
		switch(pOperator){
		case eopAdd:
			return vectorA + vectorB;
			
		case eopSubtract:
			return vectorA - vectorB;
			
		case eopAverage:
			return (vectorA + vectorB) * 0.5f;
			
		case eopNormalize:{
			const float len = vectorA.Length();
			
			if(len == 0.0f){
				return decVector(0.0f, 0.0f, 0.0f);
				
			}else{
				return vectorA /= len;
			}
			}
			
		case eopDot:{
			const float dot = vectorA * vectorB;
			return decVector(dot, dot, dot);
			}
		
		case eopCross:
			return vectorA % vectorB;
			
		default:
			return decVector();
		}
		
	}else{ // slot == eosValue
		float result = 0.0f;
		
		if(pOperator == eopDot){
			result = vectorA * vectorB;
		} // all other operators are invalid, return 0 in that case
		
		return decVector(result, result, result);
	}
}

meHTVRule::Ref meHTVRuleVectorMath::Copy() const{
	return meHTVRuleVectorMath::Ref::New(*this);
}
