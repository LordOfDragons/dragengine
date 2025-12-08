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

#include "meUHTVRuleCPSetClass.h"
#include "../../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../../world/heightterrain/rules/meHTVRuleClosestProp.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVRuleCPSetClass
///////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVRuleCPSetClass::meUHTVRuleCPSetClass(meHTVegetationLayer *vlayer, meHTVRuleClosestProp *rule, const char *nclass){
	if(!vlayer || !rule || !nclass) DETHROW(deeInvalidParam);
	
	pVLayer = NULL;
	pRule = NULL;
	
	SetShortInfo("Vegetation Layer Rule Closest Prop Set Class");
	SetMemoryConsumption(sizeof(meUHTVRuleCPSetClass));
	
	pOldPropClass = rule->GetPropClass();
	pNewPropClass = nclass;
	
	pVLayer = vlayer;
	vlayer->AddReference();
	pRule = rule;
	rule->AddReference();
}

meUHTVRuleCPSetClass::~meUHTVRuleCPSetClass(){
	if(pRule) pRule->FreeReference();
	if(pVLayer) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVRuleCPSetClass::Undo(){
	pRule->SetPropClass(pOldPropClass.GetString());
	pVLayer->NotifyRuleChanged(pRule);
}

void meUHTVRuleCPSetClass::Redo(){
	pRule->SetPropClass(pNewPropClass.GetString());
	pVLayer->NotifyRuleChanged(pRule);
}
