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

#include "ceUCAIfElseCaseMove.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseCaseMove
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseCaseMove::ceUCAIfElseCaseMove( ceConversationTopic *topic, ceCAIfElse *ifelse, ceCAIfElseCase *ifcase, int newIndex ){
	if( ! topic || ! ifelse || ! ifcase ){
		DETHROW( deeInvalidParam );
	}
	
	int count = ifelse->GetCases().GetCount();
	
	pTopic = NULL;
	pIfElse = NULL;
	pCase = NULL;
	pNewIndex = newIndex;
	pOldIndex = ifelse->GetCases().IndexOf( ifcase );
	
	if( pOldIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	if( pNewIndex < 0 || pNewIndex >= count ){
		DETHROW( deeInvalidParam );
	}
	if( pNewIndex == pOldIndex ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "If-else move case" );
	
	pTopic = topic;
	topic->AddReference();
	
	pIfElse = ifelse;
	ifelse->AddReference();
	
	pCase = ifcase;
	ifcase->AddReference();
}

ceUCAIfElseCaseMove::~ceUCAIfElseCaseMove(){
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

void ceUCAIfElseCaseMove::Undo(){
	pIfElse->GetCases().MoveTo( pCase, pOldIndex );
	pTopic->NotifyActionStructureChanged( pIfElse );
}

void ceUCAIfElseCaseMove::Redo(){
	pIfElse->GetCases().MoveTo( pCase, pNewIndex );
	pTopic->NotifyActionStructureChanged( pIfElse );
}
