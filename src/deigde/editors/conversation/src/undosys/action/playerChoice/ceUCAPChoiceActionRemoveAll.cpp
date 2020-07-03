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

#include "ceUCAPChoiceActionRemoveAll.h"
#include "../ceUndoHelpers.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionRemoveAll
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionRemoveAll::ceUCAPChoiceActionRemoveAll( ceConversationTopic *topic,
ceCAPlayerChoice *playerChoice, ceCAPlayerChoiceOption *option ){
	if( ! topic || ! playerChoice ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	
	if( option ){
		pActionList = option->GetActions();
		
	}else{
		pActionList = playerChoice->GetActions();
	}
	
	SetShortInfo( "Player Choice Remove All Actions" );
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	if( option ){
		pOption = option;
		option->AddReference();
	}
}

ceUCAPChoiceActionRemoveAll::~ceUCAPChoiceActionRemoveAll(){
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

void ceUCAPChoiceActionRemoveAll::Undo(){
	if( pOption ){
		pOption->GetActions() = pActionList;
		
	}else{
		pPlayerChoice->GetActions() = pActionList;
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
	
	if( pActionList.GetCount() > 0 ){
		pTopic->SetActiveAction( pActionList.GetAt( 0 ) );
	}
}

void ceUCAPChoiceActionRemoveAll::Redo(){
	if( pOption ){
		pOption->GetActions().RemoveAll();
		
	}else{
		pPlayerChoice->GetActions().RemoveAll();
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
	
	pTopic->SetActiveAction( pPlayerChoice );
}
