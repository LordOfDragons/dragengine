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

#include "ceUCCAParamSetTestVariable.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionActorParameter.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCAParamSetTestVariable
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCAParamSetTestVariable::ceUCCAParamSetTestVariable( ceConversationTopic *topic,
ceConversationAction *action, ceCConditionActorParameter *condition, const char *newVariable ) :
pTopic( NULL ),
pAction( NULL ),
pCondition( NULL ),
pNewVariable( newVariable )
{
	if( ! topic || ! action || ! condition ){
		DETHROW( deeInvalidParam );
	}
	
	pOldVariable = condition->GetTestVariable();
	
	SetShortInfo( "ActorParameter: Set Test Variable" );
	
	pTopic = topic;
	topic->AddReference();
	
	pAction = action;
	action->AddReference();
	
	pCondition = condition;
	condition->AddReference();
}

ceUCCAParamSetTestVariable::~ceUCCAParamSetTestVariable(){
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

void ceUCCAParamSetTestVariable::Undo(){
	pCondition->SetTestVariable( pOldVariable );
	pTopic->NotifyConditionChanged( pAction, pCondition );
}

void ceUCCAParamSetTestVariable::Redo(){
	pCondition->SetTestVariable( pNewVariable );
	pTopic->NotifyConditionChanged( pAction, pCondition );
}
