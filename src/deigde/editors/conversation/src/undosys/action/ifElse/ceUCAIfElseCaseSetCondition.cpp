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

#include "ceUCAIfElseCaseSetCondition.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/condition/ceConversationCondition.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseCaseSetCondition
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseCaseSetCondition::ceUCAIfElseCaseSetCondition(ceConversationTopic *topic, ceCAIfElse *ifElse,
ceCAIfElseCase *ifcase, ceConversationCondition *newCondition){
	if(!topic || !ifElse || !ifcase){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = NULL;
	pIfElse = NULL;
	pCase = NULL;
	pOldCondition = NULL;
	pNewCondition = NULL;
	
	SetShortInfo("If-else set case condition");
	
	pTopic = topic;
	pIfElse = ifElse;
	pCase = ifcase;
	pOldCondition = ifcase->GetCondition();
	pNewCondition = newCondition;
}

ceUCAIfElseCaseSetCondition::~ceUCAIfElseCaseSetCondition(){
}



// Management
///////////////

void ceUCAIfElseCaseSetCondition::Undo(){
	pCase->SetCondition(pOldCondition);
	pTopic->NotifyActionStructureChanged(pIfElse);
	
	pTopic->SetActive(pIfElse, pOldCondition);
}

void ceUCAIfElseCaseSetCondition::Redo(){
	pCase->SetCondition(pNewCondition);
	pTopic->NotifyActionStructureChanged(pIfElse);
	
	pTopic->SetActive(pIfElse, pNewCondition);
}
