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

#include "ceUCAPChoiceActionRemoveAll.h"
#include "../ceUActionHelpers.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionRemoveAll
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionRemoveAll::ceUCAPChoiceActionRemoveAll(ceConversationTopic *topic,
ceCAPlayerChoice *playerChoice, ceCAPlayerChoiceOption *option){
	if(! topic || ! playerChoice){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	
	if(option){
		pActionList = option->GetActions();
		
	}else{
		pActionList = playerChoice->GetActions();
	}
	
	SetShortInfo("Player Choice Remove All Actions");
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	if(option){
		pOption = option;
		option->AddReference();
	}
}

ceUCAPChoiceActionRemoveAll::~ceUCAPChoiceActionRemoveAll(){
	if(pOption){
		pOption->FreeReference();
	}
	if(pPlayerChoice){
		pPlayerChoice->FreeReference();
	}
	if(pTopic){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAPChoiceActionRemoveAll::Undo(){
	if(pOption){
		pOption->GetActions() = pActionList;
		
	}else{
		pPlayerChoice->GetActions() = pActionList;
	}
	
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
	
	if(pActionList.GetCount() > 0){
		pTopic->SetActive(pActionList.GetAt(0), NULL);
	}
}

void ceUCAPChoiceActionRemoveAll::Redo(){
	if(pOption){
		pOption->GetActions().RemoveAll();
		
	}else{
		pPlayerChoice->GetActions().RemoveAll();
	}
	
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
	
	pTopic->SetActive(pPlayerChoice, NULL);
}
