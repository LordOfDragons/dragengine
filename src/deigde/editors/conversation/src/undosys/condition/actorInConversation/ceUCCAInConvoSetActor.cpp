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

#include "ceUCCAInConvoSetActor.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionActorInConversation.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCAInConvoSetActor
//////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCAInConvoSetActor::ceUCCAInConvoSetActor( ceConversationTopic *topic,
ceConversationAction *action, ceCConditionActorInConversation *condition, const char *newID ) :
pTopic( NULL ),
pAction( NULL ),
pCondition( NULL )
{
	if( ! topic || ! action || ! condition || ! newID ){
		DETHROW( deeInvalidParam );
	}
	
	pOldID = condition->GetActor();
	pNewID = newID;
	
	SetShortInfo( "Actor in conversation set actor id" );
	
	pTopic = topic;
	topic->AddReference();
	
	pAction = action;
	action->AddReference();
	
	pCondition = condition;
	condition->AddReference();
}

ceUCCAInConvoSetActor::~ceUCCAInConvoSetActor(){
	if( pCondition ){
		pCondition->FreeReference();
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

void ceUCCAInConvoSetActor::Undo(){
	pCondition->SetActor( pOldID.GetString() );
	pTopic->NotifyActionChanged( pAction );
}

void ceUCCAInConvoSetActor::Redo(){
	pCondition->SetActor( pNewID.GetString() );
	pTopic->NotifyActionChanged( pAction );
}
