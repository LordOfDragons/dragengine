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

#include "ceUCAIfElseAdd.h"
#include "../ceUndoHelpers.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElseAdd
/////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElseAdd::ceUCAIfElseAdd( ceConversationTopic *topic, ceCAIfElse *ifElse,
ceCAIfElseCase *ifcase, ceConversationAction *action, int index ){
	if( ! topic || ! ifElse || ! action ){
		DETHROW( deeInvalidParam );
	}
	
	if( ifcase ){
		if( index < 0 || index > ifcase->GetActions().GetCount() ){
			DETHROW( deeInvalidParam );
		}
		
	}else{
		if( index < 0 || index > ifElse->GetElseActions().GetCount() ){
			DETHROW( deeInvalidParam );
		}
	}
	
	pTopic = NULL;
	pIfElse = NULL;
	pAction = NULL;
	pCase = NULL;
	pIndex = index;
	
	SetShortInfo( "IfElse Add Action" );
	
	pTopic = topic;
	topic->AddReference();
	
	pIfElse = ifElse;
	ifElse->AddReference();
	
	pAction = action;
	action->AddReference();
	
	if( ifcase ){
		pCase = ifcase;
		ifcase->AddReference();
	}
}

ceUCAIfElseAdd::~ceUCAIfElseAdd(){
	if( pCase ){
		pCase->FreeReference();
	}
	if( pAction ){
		pAction->FreeReference();
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

void ceUCAIfElseAdd::Undo(){
	ceConversationAction *activateAction = NULL;
	
	if( pCase ){
		activateAction = ceUndoHelpers::ActivateActionAfterRemove( pCase->GetActions(), pAction );
		pCase->GetActions().Remove( pAction );
		
	}else{
		activateAction = ceUndoHelpers::ActivateActionAfterRemove( pIfElse->GetElseActions(), pAction );
		pIfElse->GetElseActions().Remove( pAction );
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
	
	pTopic->SetActiveAction( activateAction ? activateAction : pIfElse );
}

void ceUCAIfElseAdd::Redo(){
	if( pCase ){
		pCase->GetActions().InsertAt( pAction, pIndex );
		
	}else{
		pIfElse->GetElseActions().InsertAt( pAction, pIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pIfElse );
	
	pTopic->SetActiveAction( pAction );
}
