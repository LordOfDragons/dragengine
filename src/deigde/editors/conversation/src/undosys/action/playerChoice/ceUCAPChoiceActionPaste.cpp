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

#include "ceUCAPChoiceActionPaste.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionPaste
//////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionPaste::ceUCAPChoiceActionPaste( ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
ceCAPlayerChoiceOption *option, const ceConversationActionList &actions, int index ) :
ceUCActionPaste( topic, actions, index )
{
	if( ! playerChoice || actions.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( option ){
		if( index < 0 || index > option->GetActions().GetCount() ){
			DETHROW( deeInvalidParam );
		}
		
	}else{
		if( index < 0 || index > playerChoice->GetActions().GetCount() ){
			DETHROW( deeInvalidParam );
		}
	}
	
	pPlayerChoice = NULL;
	pOption = NULL;
	
	if( actions.GetCount() == 1 ){
		SetShortInfo( "Player Choice Paste Action" );
		
	}else{
		SetShortInfo( "Player Choice Paste Actions" );
	}
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	if( option ){
		pOption = option;
		option->AddReference();
	}
}

ceUCAPChoiceActionPaste::~ceUCAPChoiceActionPaste(){
	if( pOption ){
		pOption->FreeReference();
	}
	if( pPlayerChoice ){
		pPlayerChoice->FreeReference();
	}
}



// Management
///////////////

void ceUCAPChoiceActionPaste::Undo(){
	if( pOption ){
		pRemoveActions( pOption->GetActions() );
		
	}else{
		pRemoveActions( pPlayerChoice->GetActions() );
	}
	GetTopic().NotifyActionStructureChanged( pPlayerChoice );
}

void ceUCAPChoiceActionPaste::Redo(){
	if( pOption ){
		pInsertActions( pOption->GetActions() );
		
	}else{
		pInsertActions( pPlayerChoice->GetActions() );
	}
	GetTopic().NotifyActionStructureChanged( pPlayerChoice );
}
