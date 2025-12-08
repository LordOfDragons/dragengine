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
ceCConditionLogic *logic, const ceConversationConditionList &conditions){
	if(!topic || !action || !logic || conditions.GetCount() == 0) DETHROW(deeInvalidParam);
	
	const int count = conditions.GetCount();
	ceConversationCondition::Ref newCondition = NULL;
	int i;
	
	pTopic = NULL;
	pAction = NULL;
	pLogic = NULL;
	
	if(count == 1){
		SetShortInfo("Logic Paste Condition");
		
	}else{
		SetShortInfo("Logic Paste Conditions");
	}
	
	pTopic = topic;
	pAction = action;
	pLogic = logic;
	try{
		for(i=0; i<count; i++){
			newCondition = conditions.GetAt(i)->CreateCopy();
			pConditions.Add(newCondition);
			newCondition = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
}

ceUCCLogicPaste::~ceUCCLogicPaste(){
	pConditions.RemoveAll();
}



// Management
///////////////

void ceUCCLogicPaste::Undo(){
	const int count = pConditions.GetCount();
	if(count == 0){
		return;
	}
	
	ceConversationCondition * const activateCondition = ActivateConditionAfterRemove();
	
	int i;
	for(i=0; i<count; i++){
		pLogic->GetConditions().Remove(pConditions.GetAt(i));
	}
	
	pTopic->NotifyConditionStructureChanged(pAction);
	
	pTopic->SetActive(pAction, activateCondition ? activateCondition : pLogic);
}

void ceUCCLogicPaste::Redo(){
	const int count = pConditions.GetCount();
	if(count == 0){
		return;
	}
	
	int i;
	for(i=0; i<count; i++){
		pLogic->GetConditions().Add(pConditions.GetAt(i));
	}
	
	pTopic->NotifyConditionStructureChanged(pAction);
	
	pTopic->SetActive(pAction, pConditions.GetAt(0));
}

ceConversationCondition *ceUCCLogicPaste::ActivateConditionAfterRemove() const{
	if(pConditions.GetCount() == 0){
		return NULL;
	}
	
	int index = pLogic->GetConditions().IndexOf(pConditions.GetAt(pConditions.GetCount() - 1));
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	if(index < pLogic->GetConditions().GetCount() - 1){
		return pLogic->GetConditions().GetAt(index + 1);
	}
	
	index = pLogic->GetConditions().IndexOf(pConditions.GetAt(0));
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(index > 0){
		return pLogic->GetConditions().GetAt(index - 1);
		
	}else{
		return NULL;
	}
}
