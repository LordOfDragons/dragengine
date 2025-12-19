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

#include "ceUCATriggerSetName.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCATrigger.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCATriggerSetName
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCATriggerSetName::ceUCATriggerSetName(ceConversationTopic *topic, ceCATrigger *action, const char *newName){
	if(!topic || !action || !newName){
		DETHROW(deeInvalidParam);
	}
	
	pTopic = nullptr;
	pAction = nullptr;
	pOldName = action->GetName();
	pNewName = newName;
	
	SetShortInfo("Action trigger set name");
	
	pTopic = topic;
	pAction = action;
}

ceUCATriggerSetName::~ceUCATriggerSetName(){
}



// Management
///////////////

void ceUCATriggerSetName::Undo(){
	pAction->SetName(pOldName.GetString());
	pTopic->NotifyActionChanged(pAction);
}

void ceUCATriggerSetName::Redo(){
	pAction->SetName(pNewName.GetString());
	pTopic->NotifyActionChanged(pAction);
}
