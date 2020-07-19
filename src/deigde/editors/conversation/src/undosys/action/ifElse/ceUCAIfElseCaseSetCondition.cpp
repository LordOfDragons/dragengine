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

#include "ceUCAIfElseCaseSetCondition.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/condition/ceConversationCondition.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseCaseSetCondition
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseCaseSetCondition::ceUCAIfElseCaseSetCondition( ceConversationTopic *topic, ceCAIfElse *ifElse,
ceCAIfElseCase *ifcase, ceConversationCondition *newCondition ){
	if( ! topic || ! ifElse || ! ifcase ){
		DETHROW( deeInvalidParam );
	}
	
	pTopic = NULL;
	pIfElse = NULL;
	pCase = NULL;
	pOldCondition = NULL;
	pNewCondition = NULL;
	
	SetShortInfo( "If-else set case condition" );
	
	pTopic = topic;
	topic->AddReference();
	
	pIfElse = ifElse;
	ifElse->AddReference();
	
	pCase = ifcase;
	ifcase->AddReference();
	
	pOldCondition = ifcase->GetCondition();
	if( pOldCondition ){
		pOldCondition->AddReference();
	}
	
	pNewCondition = newCondition;
	if( newCondition ){
		newCondition->AddReference();
	}
}

ceUCAIfElseCaseSetCondition::~ceUCAIfElseCaseSetCondition(){
	if( pNewCondition ){
		pNewCondition->FreeReference();
	}
	if( pOldCondition ){
		pOldCondition->FreeReference();
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

void ceUCAIfElseCaseSetCondition::Undo(){
	pCase->SetCondition( pOldCondition );
	pTopic->NotifyActionStructureChanged( pIfElse );
	
	pTopic->SetActive( pIfElse, pOldCondition );
}

void ceUCAIfElseCaseSetCondition::Redo(){
	pCase->SetCondition( pNewCondition );
	pTopic->NotifyActionStructureChanged( pIfElse );
	
	pTopic->SetActive( pIfElse, pNewCondition );
}
