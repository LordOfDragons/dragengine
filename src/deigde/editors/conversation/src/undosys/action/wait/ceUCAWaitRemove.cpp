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

#include "ceUCAWaitRemove.h"
#include "../ceUActionHelpers.h"
#include "../../../conversation/action/ceCAWait.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAWaitRemove
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAWaitRemove::ceUCAWaitRemove( ceConversationTopic *topic, ceCAWait *wait, ceConversationAction *action ){
	if( ! topic || ! wait || ! action ) DETHROW( deeInvalidParam );
	
	pTopic = NULL;
	pWait = NULL;
	pAction = NULL;
	pIndex = -1;
	
	pIndex = wait->GetActions().IndexOf( action );
	
	if( pIndex == -1 ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Action Wait Remove Action" );
	
	pTopic = topic;
	topic->AddReference();
	
	pWait = wait;
	wait->AddReference();
	
	pAction = action;
	action->AddReference();
}

ceUCAWaitRemove::~ceUCAWaitRemove(){
	if( pAction ){
		pAction->FreeReference();
	}
	if( pWait ){
		pWait->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAWaitRemove::Undo(){
	pWait->GetActions().InsertAt( pAction, pIndex );
	pTopic->NotifyActionStructureChanged( pWait );
	
	pTopic->SetActive( pAction, NULL );
}

void ceUCAWaitRemove::Redo(){
	ceConversationAction * const activateAction =
		ceUActionHelpers::ActivateActionAfterRemove( pWait->GetActions(), pAction );
	
	pWait->GetActions().Remove( pAction );
	pTopic->NotifyActionStructureChanged( pWait );
	
	pTopic->SetActive( activateAction ? activateAction : pWait, NULL );
}
