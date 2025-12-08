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

#include "ceUCAWaitPaste.h"
#include "../../../conversation/action/ceCAWait.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAWaitPaste
/////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAWaitPaste::ceUCAWaitPaste(ceConversationTopic *topic, ceCAWait::Ref wait,
const ceConversationActionList &actions, int index) :
ceUCActionPaste(topic, actions, index)
{
	if(!wait || actions.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	
	if(index < 0 || index > wait->GetActions().GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	pWait = NULL;
	
	if(actions.GetCount() == 1){
		SetShortInfo("Action Wait Paset Action");
		
	}else{
		SetShortInfo("Action Wait Paste Actions");
	}
	
	pWait = wait;
}

ceUCAWaitPaste::~ceUCAWaitPaste(){
}



// Management
///////////////

void ceUCAWaitPaste::Undo(){
	ceConversationAction * const activateAction = ActivateActionAfterRemove(pWait->GetActions());
	
	pRemoveActions(pWait->GetActions());
	GetTopic().NotifyActionStructureChanged(pWait);
	
	GetTopic().SetActive(activateAction ? activateAction : pWait, NULL);
}

void ceUCAWaitPaste::Redo(){
	pInsertActions(pWait->GetActions());
	GetTopic().NotifyActionStructureChanged(pWait);
	
	pSelectInserted();
}
