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

#include "ceUCActionRemove.h"
#include "ceUActionHelpers.h"
#include "../../conversation/action/ceConversationAction.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCActionRemove
///////////////////////////

// Constructor, destructor
////////////////////////////

ceUCActionRemove::ceUCActionRemove(ceConversationTopic::Ref topic, ceConversationAction::Ref action){
	if(!topic || !action) DETHROW(deeInvalidParam);
	
	pTopic = NULL;
	pAction = NULL;
	pIndex = topic->GetActionList().IndexOf(action);
	if(pIndex == -1) DETHROW(deeInvalidParam);
	
	SetShortInfo("Remove Action");
	
	pTopic = topic;
	pAction = action;
}

ceUCActionRemove::~ceUCActionRemove(){
}



// Management
///////////////

void ceUCActionRemove::Undo(){
	pTopic->GetActionList().InsertAt(pAction, pIndex);
	pTopic->NotifyActionStructureChanged(NULL);
	
	pTopic->SetActive(pAction, NULL);
}

void ceUCActionRemove::Redo(){
	ceConversationAction * const activateAction =
		ceUActionHelpers::ActivateActionAfterRemove(pTopic->GetActionList(), pAction);
	
	pTopic->GetActionList().Remove(pAction);
	pTopic->NotifyActionStructureChanged(NULL);
	
	if(activateAction){
		pTopic->SetActive(activateAction, NULL);
	}
}
