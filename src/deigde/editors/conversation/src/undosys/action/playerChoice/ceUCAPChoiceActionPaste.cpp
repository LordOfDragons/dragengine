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

#include "ceUCAPChoiceActionPaste.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionPaste
//////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionPaste::ceUCAPChoiceActionPaste(ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
ceCAPlayerChoiceOption *option, const ceConversationActionList &actions, int index) :
ceUCActionPaste(topic, actions, index)
{
	if(!playerChoice || actions.GetCount() == 0){
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
	
	pPlayerChoice = NULL;
	pOption = NULL;
	
	if(actions.GetCount() == 1){
		SetShortInfo("Player Choice Paste Action");
		
	}else{
		SetShortInfo("Player Choice Paste Actions");
	}
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	if(option){
		pOption = option;
		option->AddReference();
	}
}

ceUCAPChoiceActionPaste::~ceUCAPChoiceActionPaste(){
}



// Management
///////////////

void ceUCAPChoiceActionPaste::Undo(){
	ceConversationAction *activateAction = NULL;
	
	if(pOption){
		activateAction = ActivateActionAfterRemove(pOption->GetActions());
		pRemoveActions(pOption->GetActions());
		
	}else{
		activateAction = ActivateActionAfterRemove(pPlayerChoice->GetActions());
		pRemoveActions(pPlayerChoice->GetActions());
	}
	GetTopic().NotifyActionStructureChanged(pPlayerChoice);
	
	GetTopic().SetActive(activateAction ? activateAction : pPlayerChoice, NULL);
}

void ceUCAPChoiceActionPaste::Redo(){
	if(pOption){
		pInsertActions(pOption->GetActions());
		
	}else{
		pInsertActions(pPlayerChoice->GetActions());
	}
	GetTopic().NotifyActionStructureChanged(pPlayerChoice);
	pSelectInserted();
}
