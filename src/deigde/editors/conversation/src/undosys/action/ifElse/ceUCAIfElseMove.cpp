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

#include "ceUCAIfElseMove.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseMove
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseMove::ceUCAIfElseMove( ceConversationTopic *topic, ceCAIfElse *ifElse,
ceCAIfElseCase *ifcase, ceConversationAction *action, int newIndex ){
	if( ! topic || ! ifElse || ! action ){
		DETHROW( deeInvalidParam );
	}
	
	int count = 0;
	
	pTopic = NULL;
	pIfElse = NULL;
	pAction = NULL;
	pCase = NULL;
	pNewIndex = newIndex;
	pOldIndex = -1;
	
	if( ifcase ){
		pOldIndex = ifcase->GetActions().IndexOf( action );
		count = ifcase->GetActions().GetCount();
		
	}else{
		pOldIndex = ifElse->GetElseActions().IndexOf( action );
		count = ifElse->GetElseActions().GetCount();
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
	
	SetShortInfo( "IfElse Move Action" );
	
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

ceUCAIfElseMove::~ceUCAIfElseMove(){
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

void ceUCAIfElseMove::Undo(){
	if( pCase ){
		pCase->GetActions().MoveTo( pAction, pOldIndex );
		
	}else{
		pIfElse->GetElseActions().MoveTo( pAction, pOldIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
}

void ceUCAIfElseMove::Redo(){
	if( pCase ){
		pCase->GetActions().MoveTo( pAction, pNewIndex );
		
	}else{
		pIfElse->GetElseActions().MoveTo( pAction, pNewIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
}
