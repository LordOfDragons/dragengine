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

#include "ceUCCLogicRemoveAll.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionLogic.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCLogicRemoveAll
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCLogicRemoveAll::ceUCCLogicRemoveAll( ceConversationTopic *topic,
ceConversationAction *action, ceCConditionLogic *logic ) :
pTopic( NULL ),
pAction( NULL ),
pLogic( NULL )
{
	if( ! topic || ! action || ! logic ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Logic remove all conditions" );
	
	pConditions = logic->GetConditions();
	
	pTopic = topic;
	topic->AddReference();
	
	pAction = action;
	action->AddReference();
	
	pLogic = logic;
	logic->AddReference();
}

ceUCCLogicRemoveAll::~ceUCCLogicRemoveAll(){
	if( pLogic ){
		pLogic->FreeReference();
	}
	if( pAction ){
		pAction->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCCLogicRemoveAll::Undo(){
	pLogic->GetConditions() = pConditions;
	pTopic->NotifyConditionStructureChanged( pAction );
	
	pTopic->SetActive( pAction, pConditions.GetCount() > 0 ? pConditions.GetAt( 0 ) : NULL );
}

void ceUCCLogicRemoveAll::Redo(){
	pLogic->GetConditions().RemoveAll();
	pTopic->NotifyConditionStructureChanged( pAction );
}
