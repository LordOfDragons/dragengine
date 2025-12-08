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

#include "aeURuleTargetRemoveLink.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/link/aeLink.h"
#include "../../animator/controller/aeControllerTarget.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleTargetRemoveLink
//////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleTargetRemoveLink::aeURuleTargetRemoveLink(aeRule *rule, aeControllerTarget *target, aeLink *link){
	if(!rule || !target || !link) DETHROW(deeInvalidParam);
	
	pRule = NULL;
	pTarget = NULL;
	pLink = NULL;
	
	SetShortInfo("Rule Target Remove Link");
	
	pRule = rule;
	pTarget = target;
	
	pLink = link;
}

aeURuleTargetRemoveLink::~aeURuleTargetRemoveLink(){
	if(pLink) pLink->FreeReference();
	if(pRule) pRule->FreeReference();
}



// Management
///////////////

void aeURuleTargetRemoveLink::Undo(){
	pTarget->AddLink(pLink);
	pRule->NotifyRuleChanged();
}

void aeURuleTargetRemoveLink::Redo(){
	pTarget->RemoveLink(pLink);
	pRule->NotifyRuleChanged();
}
