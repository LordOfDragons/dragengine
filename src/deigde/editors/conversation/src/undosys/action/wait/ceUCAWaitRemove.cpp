/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
