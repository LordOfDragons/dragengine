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

#include "meUHTVRuleMathSetOp.h"
#include "../../../../../../world/heightterrain/meHTVegetationLayer.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVRuleMathSetOp
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVRuleMathSetOp::meUHTVRuleMathSetOp(meHTVegetationLayer::Ref vlayer, meHTVRuleMath::Ref rule, meHTVRuleMath::eOperators nop){
	if(!vlayer || !rule) DETHROW(deeInvalidParam);
	
	pVLayer = NULL;
	pRule = NULL;
	
	SetShortInfo("Vegetation Layer Rule Math Set Operator");
	SetMemoryConsumption(sizeof(meUHTVRuleMathSetOp));
	
	pOldOp = rule->GetOperator();
	pNewOp = nop;
	
	pVLayer = vlayer;
	pRule = rule;
}

meUHTVRuleMathSetOp::~meUHTVRuleMathSetOp(){
	if(pRule) pRule->FreeReference();
	if(pVLayer) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVRuleMathSetOp::Undo(){
	pRule->SetOperator(pOldOp);
	pVLayer->NotifyRuleChanged(pRule);
}

void meUHTVRuleMathSetOp::Redo(){
	pRule->SetOperator(pNewOp);
	pVLayer->NotifyRuleChanged(pRule);
}
