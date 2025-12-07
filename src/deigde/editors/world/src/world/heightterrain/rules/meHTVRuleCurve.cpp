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

#include "meHTVRuleCurve.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>



// Class meHTVRuleCurve
/////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleCurve::meHTVRuleCurve() : meHTVRule(ertCurve, 2){
	pCurve.SetDefaultLinear();
	
	GetSlotAt(eisValue).SetIsInput(true);
	
	GetSlotAt(eosValue).SetIsInput(false);
}

meHTVRuleCurve::meHTVRuleCurve(const meHTVRuleCurve &rule) :
meHTVRule(rule),
pCurve(rule.pCurve){
}

meHTVRuleCurve::~meHTVRuleCurve(){
}



// Management
///////////////

void meHTVRuleCurve::SetCurve(const decCurveBezier &curve){
	pCurve = curve;
}



float meHTVRuleCurve::GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	if(slot != eosValue){
		DETHROW(deeInvalidParam);
	}
	
	const meHTVRSlot &inputValue = GetSlotAt(eisValue);
	float value = 0.0f;
	
	if(inputValue.GetLinkCount() > 0){
		meHTVRLink &link = *inputValue.GetLinkAt(0);
		value = link.GetSourceRule()->GetOutputSlotValueAt(link.GetSourceSlot(), evalEnv);
	}
	
	return decCurveBezierEvaluator(pCurve).EvaluateAt(value);
}

decVector meHTVRuleCurve::GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv){
	const float value = GetOutputSlotValueAt(slot, evalEnv);
	return decVector(value, value, value);
}

meHTVRule *meHTVRuleCurve::Copy() const{
	return new meHTVRuleCurve(*this);
}
