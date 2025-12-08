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

#include "ceUCAIfElsePaste.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElsePaste
///////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElsePaste::ceUCAIfElsePaste(ceConversationTopic *topic, ceCAIfElse *ifElse,
ceCAIfElseCase *ifcase, const ceConversationActionList &actions, int index) :
ceUCActionPaste(topic, actions, index)
{
	if(!ifElse || actions.GetCount() == 0){
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
	
	pIfElse = NULL;
	pCase = NULL;
	
	if(actions.GetCount() == 1){
		SetShortInfo("IfElse Action");
		
	}else{
		SetShortInfo("IfElse Actions");
	}
	
	pIfElse = ifElse;
	ifElse->AddReference();
	
	if(ifcase){
		pCase = ifcase;
		ifcase->AddReference();
	}
}

ceUCAIfElsePaste::~ceUCAIfElsePaste(){
}



// Management
///////////////

void ceUCAIfElsePaste::Undo(){
	ceConversationAction *activateAction = NULL;
	
	if(pCase){
		activateAction = ActivateActionAfterRemove(pCase->GetActions());
		pRemoveActions(pCase->GetActions());
		
	}else{
		activateAction = ActivateActionAfterRemove(pIfElse->GetElseActions());
		pRemoveActions(pIfElse->GetElseActions());
	}
	GetTopic().NotifyActionStructureChanged(pIfElse);
	
	GetTopic().SetActive(activateAction ? activateAction : pIfElse, NULL);
}

void ceUCAIfElsePaste::Redo(){
	if(pCase){
		pInsertActions(pCase->GetActions());
		
	}else{
		pInsertActions(pIfElse->GetElseActions());
	}
	GetTopic().NotifyActionStructureChanged(pIfElse);
	
	pSelectInserted();
}
