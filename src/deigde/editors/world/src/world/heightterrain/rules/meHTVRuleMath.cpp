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

#include "meHTVRuleMath.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleMath
////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleMath::meHTVRuleMath() : meHTVRule(ertMath, 3){
	pValueA = 0.5f;
	pValueB = 0.5f;
	pOperator = eopAdd;
	
	GetSlots().GetAt(eisValueA)->SetIsInput(true);
	GetSlots().GetAt(eisValueB)->SetIsInput(true);
	
	GetSlots().GetAt(eosResult)->SetIsInput(false);
}

meHTVRuleMath::meHTVRuleMath(const meHTVRuleMath &rule) :
meHTVRule(rule),
pValueA(rule.pValueA),
pValueB(rule.pValueB),
pOperator(rule.pOperator){
}

meHTVRuleMath::~meHTVRuleMath(){
}



// Management
///////////////

void meHTVRuleMath::SetValueA(float value){
	pValueA = value;
}

void meHTVRuleMath::SetValueB(float value){
	pValueB = value;
}

void meHTVRuleMath::SetOperator(eOperators oper){
	pOperator = oper;
}



float meHTVRuleMath::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot != eosResult) DETHROW(deeInvalidParam);
	
	meHTVRSlot &inputValueA = GetSlots().GetAt(eisValueA);
	meHTVRSlot &inputValueB = GetSlots().GetAt(eisValueB);
	
	float valueA = pValueA;
	float valueB = pValueB;
	
	if(inputValueA.GetLinks().IsNotEmpty()){
		meHTVRLink &link = *inputValueA.GetLinks().GetAt(0);
		
		valueA = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
	}
	
	if(inputValueB.GetLinks().IsNotEmpty()){
		meHTVRLink &link = *inputValueB.GetLinks().GetAt(0);
		
		valueB = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
	}
	
	switch(pOperator){
	case eopAdd:
		return valueA + valueB;
		
	case eopSubtract:
		return valueA - valueB;
		
	case eopMultiply:
		return valueA * valueB;
		
	case eopDivide:
		if(valueB == 0.0f){
			return 0.0f;
			
		}else{
			return valueA / valueB;
		}
		
	case eopSine:
		return sinf(valueA * DEG2RAD);
		
	case eopCosine:
		return cosf(valueA * DEG2RAD);
		
	case eopTangent:
		return tanf(valueA * DEG2RAD);
		
	case eopArcSine:
		return asinf(valueA) * RAD2DEG;
		
	case eopArcCosine:
		return acosf(valueA) * RAD2DEG;
		
	case eopArcTangent:
		return atanf(valueA) * RAD2DEG;
		
	case eopPower:
		return powf(valueA, valueB);
		
	case eopExponential:
		return expf(valueA);
		
	case eopLogarithm:
		return logf(valueA);
		
	case eopMinimum:
		if(valueA < valueB){
			return valueA;
			
		}else{
			return valueB;
		}
		
	case eopMaximum:
		if(valueA > valueB){
			return valueA;
			
		}else{
			return valueB;
		}
		
	case eopRound:
		if(valueB < 1e-5f){
			return valueA;
			
		}else{
			// dirty and not that precise but should do the trick for what we need it
			float halfStep = valueB * 0.5f;
			return valueA - (fmodf(valueA + halfStep, valueB) - halfStep);
		}
		
	case eopLessThan:
		if(valueA <= valueB){
			return 1.0f;
			
		}else{
			return 0.0f;
		}
		
	case eopGreaterThan:
		if(valueA >= valueB){
			return 1.0f;
			
		}else{
			return 0.0f;
		}
		
	case eopEqual:
		if(fabsf(valueA - valueB) < 1e-5f){
			return 1.0f;
			
		}else{
			return 0.0f;
		}
		
	case eopNotEqual:
		if(fabsf(valueA - valueB) > 1e-5f){
			return 1.0f;
			
		}else{
			return 0.0f;
		}
		
	case eopAverage:
		return (valueA + valueB) * 0.5f;
		
	default:
		return 0.0f;
	}
}

decVector meHTVRuleMath::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	float value = GetOutputSlotValueAt(slot, evalEnv);
	
	return decVector(value, value, value);
}

meHTVRule::Ref meHTVRuleMath::Copy() const{
	return meHTVRuleMath::Ref::New(*this);
}
