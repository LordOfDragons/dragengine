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

#include "ceUCCVarSetVariable.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionVariable.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCVarSetVariable
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCVarSetVariable::ceUCCVarSetVariable( ceConversationTopic *topic, ceConversationAction *action,
ceCConditionVariable *condition, const char *newVariable ){
	if( ! topic || ! action || ! condition ) DETHROW( deeInvalidParam );
	
	pTopic = NULL;
	pAction = NULL;
	pCondition = NULL;
	pOldVariable = condition->GetVariable();
	pNewVariable = newVariable;
	
	SetShortInfo( "Condition Variable: Set Variable" );
	
	pTopic = topic;
	topic->AddReference();
	
	pAction = action;
	action->AddReference();
	
	pCondition = condition;
	condition->AddReference();
}

ceUCCVarSetVariable::~ceUCCVarSetVariable(){
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

void ceUCCVarSetVariable::Undo(){
	pCondition->SetVariable( pOldVariable );
	pTopic->NotifyActionChanged( pAction );
}

void ceUCCVarSetVariable::Redo(){
	pCondition->SetVariable( pNewVariable );
	pTopic->NotifyActionChanged( pAction );
}
