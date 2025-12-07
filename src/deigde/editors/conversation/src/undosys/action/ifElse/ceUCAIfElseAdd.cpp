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

#include "ceUCAIfElseAdd.h"
#include "../ceUActionHelpers.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseAdd
/////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseAdd::ceUCAIfElseAdd(ceConversationTopic *topic, ceCAIfElse *ifElse,
ceCAIfElseCase *ifcase, ceConversationAction *action, int index){
	if(!topic || !ifElse || !action){
		DETHROW(deeInvalidParam);
	}
	
	if(ifcase){
		if(index < 0 || index > ifcase->GetActions().GetCount()){
			DETHROW(deeInvalidParam);
		}
		
	}else{
		if(index < 0 || index > ifElse->GetElseActions().GetCount()){
			DETHROW(deeInvalidParam);
		}
	}
	
	pTopic = NULL;
	pIfElse = NULL;
	pAction = NULL;
	pCase = NULL;
	pIndex = index;
	
	SetShortInfo("IfElse Add Action");
	
	pTopic = topic;
	topic->AddReference();
	
	pIfElse = ifElse;
	ifElse->AddReference();
	
	pAction = action;
	action->AddReference();
	
	if(ifcase){
		pCase = ifcase;
		ifcase->AddReference();
	}
}

ceUCAIfElseAdd::~ceUCAIfElseAdd(){
}



// Management
///////////////

void ceUCAIfElseAdd::Undo(){
	ceConversationAction *activateAction = NULL;
	
	if(pCase){
		activateAction = ceUActionHelpers::ActivateActionAfterRemove(pCase->GetActions(), pAction);
		pCase->GetActions().Remove(pAction);
		
	}else{
		activateAction = ceUActionHelpers::ActivateActionAfterRemove(pIfElse->GetElseActions(), pAction);
		pIfElse->GetElseActions().Remove(pAction);
	}
	
	pTopic->NotifyActionStructureChanged(pIfElse);
	
	pTopic->SetActive(activateAction ? activateAction : pIfElse, NULL);
}

void ceUCAIfElseAdd::Redo(){
	if(pCase){
		pCase->GetActions().InsertAt(pAction, pIndex);
		
	}else{
		pIfElse->GetElseActions().InsertAt(pAction, pIndex);
	}
	
	pTopic->NotifyActionStructureChanged(pIfElse);
	
	pTopic->SetActive(pAction, NULL);
}
