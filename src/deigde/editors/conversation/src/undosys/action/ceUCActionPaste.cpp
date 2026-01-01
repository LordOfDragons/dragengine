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

#include "ceUCActionPaste.h"
#include "../../conversation/action/ceConversationAction.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCActionPaste
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCActionPaste::ceUCActionPaste(ceConversationTopic *topic,
const ceConversationAction::List &actions, int index)
{
	if(!topic || actions.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = nullptr;
	pIndex = index;
	
	if(actions.GetCount() == 1){
		SetShortInfo("Paste Action");
		
	}else{
		SetShortInfo("Paste Actions");
	}
	
	pTopic = topic;
	
	actions.Visit([&](const ceConversationAction &a){
		pActions.Add(a.CreateCopy());
	});
}

ceUCActionPaste::~ceUCActionPaste(){
}



// Management
///////////////

void ceUCActionPaste::Undo(){
	ceConversationAction * const activateAction = ActivateActionAfterRemove(pTopic->GetActions());
	
	pRemoveActions(pTopic->GetActions());
	pTopic->NotifyActionStructureChanged(nullptr);
	
	if(activateAction){
		pTopic->SetActive(activateAction, nullptr);
	}
}

void ceUCActionPaste::Redo(){
	pInsertActions(pTopic->GetActions());
	pTopic->NotifyActionStructureChanged(nullptr);
	
	pSelectInserted();
}



// Protected Functions
////////////////////////

void ceUCActionPaste::pInsertActions(ceConversationAction::List &list){
	const int count = pActions.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		list.Insert(pActions.GetAt(i), pIndex + i);
	}
}

void ceUCActionPaste::pRemoveActions(ceConversationAction::List &list){
	pActions.Visit([&](ceConversationAction *a){
		list.Remove(a);
	});
}

void ceUCActionPaste::pSelectInserted(){
	if(pActions.IsNotEmpty()){
		pTopic->SetActive(pActions.First(), nullptr);
	}
}

ceConversationAction *ceUCActionPaste::ActivateActionAfterRemove(
const ceConversationAction::List &list) const{
	if(pActions.IsEmpty()){
		return nullptr;
	}
	
	int index = list.IndexOf(pActions.Last());
	DEASSERT_TRUE(index != -1)
	
	if(index < list.GetCount() - 1){
		return list.GetAt(index + 1);
	}
	
	index = list.IndexOf(pActions.First());
	DEASSERT_TRUE(index != -1)
	
	return index > 0 ? list.GetAt(index - 1) : nullptr;
}
