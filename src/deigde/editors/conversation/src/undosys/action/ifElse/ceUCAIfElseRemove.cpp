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

#include "ceUCAIfElseRemove.h"
#include "../ceUActionHelpers.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseRemove
////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseRemove::ceUCAIfElseRemove( ceConversationTopic *topic, ceCAIfElse *ifElse,
ceCAIfElseCase *ifcase, ceConversationAction *action ){
	if( ! topic || ! ifElse || ! action ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pIfElse = NULL;
	pAction = NULL;
	pCase = NULL;
	pIndex = -1;
	
	if( ifcase ){
		pIndex = ifcase->GetActions().IndexOf( action );
		
	}else{
		pIndex = ifElse->GetElseActions().IndexOf( action );
	}
	
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "IfElse Remove Action" );
	
	pTopic = topic;
	topic->AddReference();
	
	pIfElse = ifElse;
	ifElse->AddReference();
	
	if( ifcase ){
		pCase = ifcase;
		ifcase->AddReference();
	}
	
	pAction = action;
	action->AddReference();
}

ceUCAIfElseRemove::~ceUCAIfElseRemove(){
	if( pAction ){
		pAction->FreeReference();
	}
	if( pCase ){
		pCase->FreeReference();
	}
	if( pIfElse ){
		pIfElse->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAIfElseRemove::Undo(){
	if( pCase ){
		pCase->GetActions().InsertAt( pAction, pIndex );
		
	}else{
		pIfElse->GetElseActions().InsertAt( pAction, pIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
	pTopic->SetActive( pAction, NULL );
}

void ceUCAIfElseRemove::Redo(){
	ceConversationAction *activateAction = NULL;
	
	if( pCase ){
		activateAction = ceUActionHelpers::ActivateActionAfterRemove( pCase->GetActions(), pAction );
		pCase->GetActions().Remove( pAction );
		
	}else{
		activateAction = ceUActionHelpers::ActivateActionAfterRemove( pIfElse->GetElseActions(), pAction );
		pIfElse->GetElseActions().Remove( pAction );
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
	
	pTopic->SetActive( activateAction ? activateAction : pIfElse, NULL );
}
