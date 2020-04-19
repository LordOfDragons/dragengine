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

#include "ceUCAIfElseCaseRemove.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseCaseRemove
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseCaseRemove::ceUCAIfElseCaseRemove( ceConversationTopic *topic, ceCAIfElse *ifelse, ceCAIfElseCase *ifcase ){
	if( ! topic || ! ifelse || ! ifcase ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pIfElse = NULL;
	pCase = NULL;
	pIndex = ifelse->GetCases().IndexOf( ifcase );
	
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "If-else remove case" );
	
	pTopic = topic;
	topic->AddReference();
	
	pIfElse = ifelse;
	ifelse->AddReference();
	
	pCase = ifcase;
	ifcase->AddReference();
}

ceUCAIfElseCaseRemove::~ceUCAIfElseCaseRemove(){
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

void ceUCAIfElseCaseRemove::Undo(){
	pIfElse->GetCases().InsertAt( pCase, pIndex );
	pTopic->NotifyActionStructureChanged( pIfElse );
}

void ceUCAIfElseCaseRemove::Redo(){
	pIfElse->GetCases().Remove( pCase );
	pTopic->NotifyActionStructureChanged( pIfElse );
}
