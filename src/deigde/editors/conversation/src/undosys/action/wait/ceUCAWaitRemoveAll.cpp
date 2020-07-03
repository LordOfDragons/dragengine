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

#include "ceUCAWaitRemoveAll.h"
#include "../ceUndoHelpers.h"
#include "../../../conversation/action/ceCAWait.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAWaitRemoveAll
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAWaitRemoveAll::ceUCAWaitRemoveAll( ceConversationTopic *topic, ceCAWait *wait ){
	if( ! topic || ! wait ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pWait = NULL;
	pActionList = wait->GetActions();
	
	SetShortInfo( "Action Wait Remove All Actions" );
	
	pTopic = topic;
	topic->AddReference();
	
	pWait = wait;
	wait->AddReference();
}

ceUCAWaitRemoveAll::~ceUCAWaitRemoveAll(){
	if( pWait ){
		pWait->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAWaitRemoveAll::Undo(){
	pWait->GetActions() = pActionList;
	pTopic->NotifyActionStructureChanged( pWait );
	
	if( pActionList.GetCount() > 0 ){
		pTopic->SetActiveAction( pActionList.GetAt( 0 ) );
	}
}

void ceUCAWaitRemoveAll::Redo(){
	pWait->GetActions().RemoveAll();
	pTopic->NotifyActionStructureChanged( pWait );
	
	pTopic->SetActiveAction( pWait );
}
