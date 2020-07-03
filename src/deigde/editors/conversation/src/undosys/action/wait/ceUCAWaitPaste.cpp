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

#include "ceUCAWaitPaste.h"
#include "../../../conversation/action/ceCAWait.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAWaitPaste
/////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAWaitPaste::ceUCAWaitPaste( ceConversationTopic *topic, ceCAWait *wait,
const ceConversationActionList &actions, int index ) :
ceUCActionPaste( topic, actions, index )
{
	if( ! wait || actions.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < 0 || index > wait->GetActions().GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pWait = NULL;
	
	if( actions.GetCount() == 1 ){
		SetShortInfo( "Action Wait Paset Action" );
		
	}else{
		SetShortInfo( "Action Wait Paste Actions" );
	}
	
	pWait = wait;
	wait->AddReference();
}

ceUCAWaitPaste::~ceUCAWaitPaste(){
	if( pWait ){
		pWait->FreeReference();
	}
}



// Management
///////////////

void ceUCAWaitPaste::Undo(){
	ceConversationAction * const activateAction = ActivateActionAfterRemove( pWait->GetActions() );
	
	pRemoveActions( pWait->GetActions() );
	GetTopic().NotifyActionStructureChanged( pWait );
	
	GetTopic().SetActiveAction( activateAction ? activateAction : pWait );
}

void ceUCAWaitPaste::Redo(){
	pInsertActions( pWait->GetActions() );
	GetTopic().NotifyActionStructureChanged( pWait );
	
	pSelectInserted();
}
