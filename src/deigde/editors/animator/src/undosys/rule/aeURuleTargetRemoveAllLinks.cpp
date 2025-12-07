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

#include "aeURuleTargetRemoveAllLinks.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/link/aeLink.h"
#include "../../animator/controller/aeControllerTarget.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleTargetRemoveAllLinks
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleTargetRemoveAllLinks::aeURuleTargetRemoveAllLinks(aeRule *rule,
aeControllerTarget *target, const aeLinkList &links) :
pRule(NULL),
pTarget(NULL),
pLinks(links)
{
	if(!rule || !target || links.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Rule Target Remove All Links");
	
	pRule = rule;
	pRule->AddReference();
	
	pTarget = target;
}

aeURuleTargetRemoveAllLinks::~aeURuleTargetRemoveAllLinks(){
	if(pRule){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeURuleTargetRemoveAllLinks::Undo(){
	const int count = pLinks.GetCount();
	int i;
	for(i=0; i<count; i++){
		pTarget->AddLink(pLinks.GetAt(i));
	}
	pRule->NotifyRuleChanged();
}

void aeURuleTargetRemoveAllLinks::Redo(){
	const int count = pLinks.GetCount();
	int i;
	for(i=0; i<count; i++){
		pTarget->RemoveLink(pLinks.GetAt(i));
	}
	pRule->NotifyRuleChanged();
}
