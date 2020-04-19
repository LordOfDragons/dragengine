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

#include "ceUCAWaitMove.h"
#include "../../../conversation/action/ceCAWait.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAWaitMove
////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAWaitMove::ceUCAWaitMove( ceConversationTopic *topic, ceCAWait *wait, ceConversationAction *action, int newIndex ){
	if( ! topic || ! wait || ! action ) DETHROW( deeInvalidParam );
	
	int count = 0;
	
	pTopic = NULL;
	pWait = NULL;
	pAction = NULL;
	pNewIndex = newIndex;
	pOldIndex = -1;
	
	pOldIndex = wait->GetActions().IndexOf( action );
	count = wait->GetActions().GetCount();
	
	if( pOldIndex == -1 ) DETHROW( deeInvalidParam );
	if( pNewIndex < 0 || pNewIndex >= count ) DETHROW( deeInvalidParam );
	if( pNewIndex == pOldIndex ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Action Wait Move Action" );
	
	pTopic = topic;
	topic->AddReference();
	
	pWait = wait;
	wait->AddReference();
	
	pAction = action;
	action->AddReference();
}

ceUCAWaitMove::~ceUCAWaitMove(){
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

void ceUCAWaitMove::Undo(){
	pWait->GetActions().MoveTo( pAction, pOldIndex );
	pTopic->NotifyActionStructureChanged( pWait );
}

void ceUCAWaitMove::Redo(){
	pWait->GetActions().MoveTo( pAction, pNewIndex );
	pTopic->NotifyActionStructureChanged( pWait );
}
