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

#include "ceUCAPChoiceActionMove.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionMove
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionMove::ceUCAPChoiceActionMove(ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
ceCAPlayerChoiceOption *option, ceConversationAction *action, int newIndex){
	if(!topic || !playerChoice || !action){
		DETHROW(deeInvalidParam);
	}
	
	int count = 0;
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	pAction = NULL;
	pNewIndex = newIndex;
	pOldIndex = -1;
	
	if(option){
		pOldIndex = option->GetActions().IndexOf(action);
		count = option->GetActions().GetCount();
		
	}else{
		pOldIndex = playerChoice->GetActions().IndexOf(action);
		count = playerChoice->GetActions().GetCount();
	}
	
	if(pOldIndex == -1){
		DETHROW(deeInvalidParam);
	}
	if(pNewIndex < 0 || pNewIndex >= count){
		DETHROW(deeInvalidParam);
	}
	if(pNewIndex == pOldIndex){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Player Choice Move Action");
	
	pTopic = topic;
	pPlayerChoice = playerChoice;
	if(option){
		pOption = option;
	}
	
	pAction = action;
}

ceUCAPChoiceActionMove::~ceUCAPChoiceActionMove(){
}



// Management
///////////////

void ceUCAPChoiceActionMove::Undo(){
	if(pOption){
		pOption->GetActions().MoveTo(pAction, pOldIndex);
		
	}else{
		pPlayerChoice->GetActions().MoveTo(pAction, pOldIndex);
	}
	
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
}

void ceUCAPChoiceActionMove::Redo(){
	if(pOption){
		pOption->GetActions().MoveTo(pAction, pNewIndex);
		
	}else{
		pPlayerChoice->GetActions().MoveTo(pAction, pNewIndex);
	}
	
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
}
