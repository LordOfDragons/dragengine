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

#include "ceUCAPChoiceActionMove.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionMove
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionMove::ceUCAPChoiceActionMove( ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
ceCAPlayerChoiceOption *option, ceConversationAction *action, int newIndex ){
	if( ! topic || ! playerChoice || ! action ){
		DETHROW( deeInvalidParam );
	}
	
	int count = 0;
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	pAction = NULL;
	pNewIndex = newIndex;
	pOldIndex = -1;
	
	if( option ){
		pOldIndex = option->GetActions().IndexOf( action );
		count = option->GetActions().GetCount();
		
	}else{
		pOldIndex = playerChoice->GetActions().IndexOf( action );
		count = playerChoice->GetActions().GetCount();
	}
	
	if( pOldIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	if( pNewIndex < 0 || pNewIndex >= count ){
		DETHROW( deeInvalidParam );
	}
	if( pNewIndex == pOldIndex ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Player Choice Move Action" );
	
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

ceUCAPChoiceActionMove::~ceUCAPChoiceActionMove(){
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

void ceUCAPChoiceActionMove::Undo(){
	if( pOption ){
		pOption->GetActions().MoveTo( pAction, pOldIndex );
		
	}else{
		pPlayerChoice->GetActions().MoveTo( pAction, pOldIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
}

void ceUCAPChoiceActionMove::Redo(){
	if( pOption ){
		pOption->GetActions().MoveTo( pAction, pNewIndex );
		
	}else{
		pPlayerChoice->GetActions().MoveTo( pAction, pNewIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
}
