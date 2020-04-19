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

#include "ceUCCVarSetTestValue.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionVariable.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCVarSetTestValue
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCVarSetTestValue::ceUCCVarSetTestValue( ceConversationTopic *topic, ceConversationAction *action,
ceCConditionVariable *condition, int newValue ){
	if( ! topic || ! action || ! condition ) DETHROW( deeInvalidParam );
	
	pTopic = NULL;
	pAction = NULL;
	pCondition = NULL;
	pOldValue = condition->GetTestValue();
	pNewValue = newValue;
	
	SetShortInfo( "Condition Variable: Set Test Value" );
	
	pTopic = topic;
	topic->AddReference();
	
	pAction = action;
	action->AddReference();
	
	pCondition = condition;
	condition->AddReference();
}

ceUCCVarSetTestValue::~ceUCCVarSetTestValue(){
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

void ceUCCVarSetTestValue::Undo(){
	pCondition->SetTestValue( pOldValue );
	pTopic->NotifyActionChanged( pAction );
}

void ceUCCVarSetTestValue::Redo(){
	pCondition->SetTestValue( pNewValue );
	pTopic->NotifyActionChanged( pAction );
}
