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
#include <string.h>
#include <stdlib.h>

#include "aeURuleGroupMoveRuleUp.h"
#include "../../../animator/rule/aeRuleGroup.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleGroupMoveRuleUp
/////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleGroupMoveRuleUp::aeURuleGroupMoveRuleUp(aeRuleGroup *group, aeRule *rule){
	if(! group || ! rule){
		DETHROW(deeInvalidParam);
	}
	
	pGroup = NULL;
	pRule = NULL;
	
	pIndex = group->GetRules().IndexOf(rule);
	if(pIndex < 1){
		DETHROW(deeInvalidParam);
	}
	
	pGroup = group;
	group->AddReference();
	
	pRule = rule;
	rule->AddReference();
}

aeURuleGroupMoveRuleUp::~aeURuleGroupMoveRuleUp(){
	pCleanUp();
}



// Management
///////////////

void aeURuleGroupMoveRuleUp::Undo(){
	pGroup->MoveRuleTo(pRule, pIndex);
}

void aeURuleGroupMoveRuleUp::Redo(){
	pGroup->MoveRuleTo(pRule, pIndex - 1);
}



// Private Functions
//////////////////////

void aeURuleGroupMoveRuleUp::pCleanUp(){
	if(pRule){
		pRule->FreeReference();
	}
	if(pGroup){
		pGroup->FreeReference();
	}
}
