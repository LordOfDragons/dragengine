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

#include "ceUCAPChoiceActionAdd.h"
#include "../ceUActionHelpers.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionAdd
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionAdd::ceUCAPChoiceActionAdd(ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
ceCAPlayerChoiceOption *option, ceConversationAction *action, int index){
	if(!topic || !playerChoice || !action){
		DETHROW(deeInvalidParam);
	}
	
	if(option){
		if(index < 0 || index > option->GetActions().GetCount()){
			DETHROW(deeInvalidParam);
		}
		
	}else{
		if(index < 0 || index > playerChoice->GetActions().GetCount()){
			DETHROW(deeInvalidParam);
		}
	}
	
	pTopic = nullptr;
	pPlayerChoice = nullptr;
	pOption = nullptr;
	pAction = nullptr;
	pIndex = index;
	
	SetShortInfo("Player Choice Add Action");
	
	pTopic = topic;
	pPlayerChoice = playerChoice;
	if(option){
		pOption = option;
	}
	
	pAction = action;
}

ceUCAPChoiceActionAdd::~ceUCAPChoiceActionAdd(){
}



// Management
///////////////

void ceUCAPChoiceActionAdd::Undo(){
	ceConversationAction *activateAction = nullptr;
	
	if(pOption){
		activateAction = ceUActionHelpers::ActivateActionAfterRemove(pOption->GetActions(), pAction);
		pOption->GetActions().Remove(pAction);
		
	}else{
		activateAction = ceUActionHelpers::ActivateActionAfterRemove(pPlayerChoice->GetActions(), pAction);
		pPlayerChoice->GetActions().Remove(pAction);
	}
	
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
	
	pTopic->SetActive(activateAction ? activateAction : pPlayerChoice, nullptr);
}

void ceUCAPChoiceActionAdd::Redo(){
	if(pOption){
		pOption->GetActions().Insert(pAction, pIndex);
		
	}else{
		pPlayerChoice->GetActions().Insert(pAction, pIndex);
	}
	
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
	pTopic->SetActive(pAction, nullptr);
}
