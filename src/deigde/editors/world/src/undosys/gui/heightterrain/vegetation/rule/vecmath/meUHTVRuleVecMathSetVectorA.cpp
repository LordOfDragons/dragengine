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

#include "meUHTVRuleVecMathSetVectorA.h"
#include "../../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../../world/heightterrain/rules/meHTVRuleVectorMath.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVRuleVecMathSetVectorA
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVRuleVecMathSetVectorA::meUHTVRuleVecMathSetVectorA(meHTVegetationLayer *vlayer, meHTVRuleVectorMath *rule, const decVector &nvalue){
	if(!vlayer || !rule) DETHROW(deeInvalidParam);
	
	pVLayer = NULL;
	pRule = NULL;
	
	SetShortInfo("Vegetation Layer Rule Math Set Vector A");
	SetMemoryConsumption(sizeof(meUHTVRuleVecMathSetVectorA));
	
	pOldVector = rule->GetVectorA();
	pNewVector = nvalue;
	
	pVLayer = vlayer;
	pRule = rule;
}

meUHTVRuleVecMathSetVectorA::~meUHTVRuleVecMathSetVectorA(){
	if(pRule) pRule->FreeReference();
	if(pVLayer) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVRuleVecMathSetVectorA::Undo(){
	pRule->SetVectorA(pOldVector);
	pVLayer->NotifyRuleChanged(pRule);
}

void meUHTVRuleVecMathSetVectorA::Redo(){
	pRule->SetVectorA(pNewVector);
	pVLayer->NotifyRuleChanged(pRule);
}
