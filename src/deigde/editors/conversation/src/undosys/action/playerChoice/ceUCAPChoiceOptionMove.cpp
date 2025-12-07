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

#include "ceUCAPChoiceOptionMove.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceOptionMove
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceOptionMove::ceUCAPChoiceOptionMove(ceConversationTopic *topic, ceCAPlayerChoice *playerChoice, ceCAPlayerChoiceOption *option, int newIndex){
	if(!topic || !playerChoice || !option) DETHROW(deeInvalidParam);
	
	int count = playerChoice->GetOptions().GetCount();
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	pNewIndex = newIndex;
	pOldIndex = playerChoice->GetOptions().IndexOf(option);
	
	if(pOldIndex == -1){
		DETHROW(deeInvalidParam);
	}
	if(pNewIndex < 0 || pNewIndex >= count){
		DETHROW(deeInvalidParam);
	}
	if(pNewIndex == pOldIndex){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Player Choice Move Option");
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	pOption = option;
	option->AddReference();
}

ceUCAPChoiceOptionMove::~ceUCAPChoiceOptionMove(){
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

void ceUCAPChoiceOptionMove::Undo(){
	pPlayerChoice->GetOptions().MoveTo(pOption, pOldIndex);
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
}

void ceUCAPChoiceOptionMove::Redo(){
	pPlayerChoice->GetOptions().MoveTo(pOption, pNewIndex);
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
}
