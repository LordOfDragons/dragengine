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

#include "ceUCAPChoiceOptionSetCondition.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/condition/ceConversationCondition.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceOptionSetCondition
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceOptionSetCondition::ceUCAPChoiceOptionSetCondition(ceConversationTopic *topic,
ceCAPlayerChoice *ifElse, ceCAPlayerChoiceOption *ifcase, ceConversationCondition *newCondition) :
pTopic(NULL),
pPlayerChoice(NULL),
pOption(NULL),
pOldCondition(NULL),
pNewCondition(NULL)
{
	if(!topic || !ifElse || !ifcase){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Player Choice Option set case condition");
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = ifElse;
	ifElse->AddReference();
	
	pOption = ifcase;
	ifcase->AddReference();
	
	pOldCondition = ifcase->GetCondition();
	if(pOldCondition){
	}
	
	pNewCondition = newCondition;
}

ceUCAPChoiceOptionSetCondition::~ceUCAPChoiceOptionSetCondition(){
}



// Management
///////////////

void ceUCAPChoiceOptionSetCondition::Undo(){
	pOption->SetCondition(pOldCondition);
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
	
	pTopic->SetActive(pPlayerChoice, pOldCondition);
}

void ceUCAPChoiceOptionSetCondition::Redo(){
	pOption->SetCondition(pNewCondition);
	pTopic->NotifyActionStructureChanged(pPlayerChoice);
	
	pTopic->SetActive(pPlayerChoice, pNewCondition);
}
