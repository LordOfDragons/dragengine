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

#include "ceUCAPChoiceOptionAdd.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceOptionAdd
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceOptionAdd::ceUCAPChoiceOptionAdd( ceConversationTopic *topic, ceCAPlayerChoice *playerChoice, ceCAPlayerChoiceOption *option, int index ){
	if( ! topic || ! playerChoice || ! option ){
		DETHROW( deeInvalidParam );
	}
	if( index < 0 || index > playerChoice->GetOptions().GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	pIndex = index;
	
	SetShortInfo( "Player Choice Add Option" );
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	pOption = option;
	option->AddReference();
}

ceUCAPChoiceOptionAdd::~ceUCAPChoiceOptionAdd(){
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

void ceUCAPChoiceOptionAdd::Undo(){
	pPlayerChoice->GetOptions().Remove( pOption );
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
}

void ceUCAPChoiceOptionAdd::Redo(){
	pPlayerChoice->GetOptions().InsertAt( pOption, pIndex );
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
}
