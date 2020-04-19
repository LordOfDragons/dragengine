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

#include "ceUCASetVarSetValueVariable.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCASetVariable.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCASetVarSetValueVariable
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCASetVarSetValueVariable::ceUCASetVarSetValueVariable(
ceConversationTopic *topic, ceCASetVariable *action, const char *newName ) :
pTopic( NULL ),
pAction( NULL ),
pNewName( newName )
{
	if( ! topic || ! action ){
		DETHROW( deeInvalidParam );
	}
	
	pOldName = action->GetValueVariable();
	
	SetShortInfo( "SetVariable Value Variable" );
	
	pTopic = topic;
	topic->AddReference();
	
	pAction = action;
	action->AddReference();
}

ceUCASetVarSetValueVariable::~ceUCASetVarSetValueVariable(){
	if( pAction ){
		pAction->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCASetVarSetValueVariable::Undo(){
	pAction->SetValueVariable( pOldName );
	pTopic->NotifyActionChanged( pAction );
}

void ceUCASetVarSetValueVariable::Redo(){
	pAction->SetValueVariable( pNewName );
	pTopic->NotifyActionChanged( pAction );
}
