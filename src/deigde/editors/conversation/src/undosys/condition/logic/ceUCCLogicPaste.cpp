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

#include "ceUCCLogicPaste.h"
#include "../ceUConditionHelpers.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionLogic.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCLogicPaste
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCLogicPaste::ceUCCLogicPaste(ceConversationTopic *topic, ceConversationAction *action,
ceCConditionLogic *logic, const ceConversationCondition::List &conditions){
	DEASSERT_NOTNULL(topic)
	DEASSERT_NOTNULL(action)
	DEASSERT_NOTNULL(logic)
	DEASSERT_TRUE(conditions.IsNotEmpty())
	
	if(conditions.GetCount() == 1){
		SetShortInfo("Logic Paste Condition");
		
	}else{
		SetShortInfo("Logic Paste Conditions");
	}
	
	pTopic = topic;
	pAction = action;
	pLogic = logic;
	
	conditions.Visit([&](const ceConversationCondition &c){
		pConditions.Add(c.CreateCopy());
	});
}

ceUCCLogicPaste::~ceUCCLogicPaste(){
}



// Management
///////////////

void ceUCCLogicPaste::Undo(){
	if(pConditions.IsEmpty()){
		return;
	}
	
	ceConversationCondition * const activateCondition = ActivateConditionAfterRemove();
	pConditions.Visit([&](ceConversationCondition *c){
		pLogic->GetConditions().Remove(c);
	});
	
	pTopic->NotifyConditionStructureChanged(pAction);
	
	pTopic->SetActive(pAction, activateCondition ? activateCondition : pLogic);
}

void ceUCCLogicPaste::Redo(){
	if(pConditions.IsEmpty()){
		return;
	}
	
	pConditions.Visit([&](ceConversationCondition *c){
		pLogic->GetConditions().Add(c);
	});
	
	pTopic->NotifyConditionStructureChanged(pAction);
	
	pTopic->SetActive(pAction, pConditions.GetAt(0));
}

ceConversationCondition *ceUCCLogicPaste::ActivateConditionAfterRemove() const{
	if(pConditions.IsEmpty()){
		return nullptr;
	}
	
	int index = pLogic->GetConditions().IndexOf(pConditions.Last());
	DEASSERT_TRUE(index != -1)
	
	if(index < pLogic->GetConditions().GetCount() - 1){
		return pLogic->GetConditions().GetAt(index + 1);
	}
	
	index = pLogic->GetConditions().IndexOf(pConditions.First());
	DEASSERT_TRUE(index != -1)
	
	return index > 0 ? pLogic->GetConditions().GetAt(index - 1) : nullptr;
}
