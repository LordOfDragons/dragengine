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

#include "ceUCAPChoiceActionRemove.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionRemove
///////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionRemove::ceUCAPChoiceActionRemove( ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
ceCAPlayerChoiceOption *option, ceConversationAction *action ){
	if( ! topic || ! playerChoice || ! action ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	pAction = NULL;
	
	if( option ){
		pIndex = option->GetActions().IndexOf( action );
		
	}else{
		pIndex = playerChoice->GetActions().IndexOf( action );
	}
	
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Player Choice Remove Action" );
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	if( option ){
		pOption = option;
		option->AddReference();
	}
	
	pAction = action;
	action->AddReference();
}

ceUCAPChoiceActionRemove::~ceUCAPChoiceActionRemove(){
	if( pAction ){
		pAction->FreeReference();
	}
	if( pOption ){
		pOption->FreeReference();
	}
	if( pPlayerChoice ){
		pPlayerChoice->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAPChoiceActionRemove::Undo(){
	if( pOption ){
		pOption->GetActions().InsertAt( pAction, pIndex );
		
	}else{
		pPlayerChoice->GetActions().InsertAt( pAction, pIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
}

void ceUCAPChoiceActionRemove::Redo(){
	if( pOption ){
		pOption->GetActions().Remove( pAction );
		
	}else{
		pPlayerChoice->GetActions().Remove( pAction );
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
}
