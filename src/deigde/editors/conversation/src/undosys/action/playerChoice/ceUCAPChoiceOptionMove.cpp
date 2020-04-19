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

#include "ceUCAPChoiceOptionMove.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceOptionMove
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceOptionMove::ceUCAPChoiceOptionMove( ceConversationTopic *topic, ceCAPlayerChoice *playerChoice, ceCAPlayerChoiceOption *option, int newIndex ){
	if( ! topic || ! playerChoice || ! option ) DETHROW( deeInvalidParam );
	
	int count = playerChoice->GetOptions().GetCount();
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	pNewIndex = newIndex;
	pOldIndex = playerChoice->GetOptions().IndexOf( option );
	
	if( pOldIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	if( pNewIndex < 0 || pNewIndex >= count ){
		DETHROW( deeInvalidParam );
	}
	if( pNewIndex == pOldIndex ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Player Choice Move Option" );
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	pOption = option;
	option->AddReference();
}

ceUCAPChoiceOptionMove::~ceUCAPChoiceOptionMove(){
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

void ceUCAPChoiceOptionMove::Undo(){
	pPlayerChoice->GetOptions().MoveTo( pOption, pOldIndex );
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
}

void ceUCAPChoiceOptionMove::Redo(){
	pPlayerChoice->GetOptions().MoveTo( pOption, pNewIndex );
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
}
