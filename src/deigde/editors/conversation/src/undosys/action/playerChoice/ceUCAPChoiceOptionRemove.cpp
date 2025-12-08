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

#include "ceUCAPChoiceOptionRemove.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceOptionRemove
///////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceOptionRemove::ceUCAPChoiceOptionRemove(ceConversationTopic *topic, ceCAPlayerChoice *playerChoice, ceCAPlayerChoiceOption *option){
	if(!topic || !playerChoice || !option) DETHROW(deeInvalidParam);
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	pIndex = playerChoice->GetOptions().IndexOf(option);
	
	if(pIndex == -1){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Player Choice Remove Option");
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	pOption = option;
	option->AddReference();
}

ceUCAPChoiceOptionRemove::~ceUCAPChoiceOptionRemove(){
}



// Management
///////////////

void ceUCAPChoiceOptionRemove::Undo(){
	pPlayerChoice->GetOptions().InsertAt(pOption, pIndex);
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
}

void ceUCAPChoiceOptionRemove::Redo(){
	pPlayerChoice->GetOptions().Remove(pOption);
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
}
