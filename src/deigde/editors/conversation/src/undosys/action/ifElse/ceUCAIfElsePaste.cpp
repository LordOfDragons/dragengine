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

#include "ceUCAIfElsePaste.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAIfElsePaste
///////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAIfElsePaste::ceUCAIfElsePaste( ceConversationTopic *topic, ceCAIfElse *ifElse,
ceCAIfElseCase *ifcase, const ceConversationActionList &actions, int index ) :
ceUCActionPaste( topic, actions, index )
{
	if( ! ifElse || actions.GetCount() == 0 ){
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
	
	pIfElse = NULL;
	pCase = NULL;
	
	if( actions.GetCount() == 1 ){
		SetShortInfo( "IfElse Action" );
		
	}else{
		SetShortInfo( "IfElse Actions" );
	}
	
	pIfElse = ifElse;
	ifElse->AddReference();
	
	if( ifcase ){
		pCase = ifcase;
		ifcase->AddReference();
	}
}

ceUCAIfElsePaste::~ceUCAIfElsePaste(){
	if( pCase ){
		pCase->FreeReference();
	}
	if( pIfElse ){
		pIfElse->FreeReference();
	}
}



// Management
///////////////

void ceUCAIfElsePaste::Undo(){
	ceConversationAction *activateAction = NULL;
	
	if( pCase ){
		activateAction = ActivateActionAfterRemove( pCase->GetActions() );
		pRemoveActions( pCase->GetActions() );
		
	}else{
		activateAction = ActivateActionAfterRemove( pIfElse->GetElseActions() );
		pRemoveActions( pIfElse->GetElseActions() );
	}
	GetTopic().NotifyActionStructureChanged( pIfElse );
	
	GetTopic().SetActive( activateAction ? activateAction : pIfElse, NULL );
}

void ceUCAIfElsePaste::Redo(){
	if( pCase ){
		pInsertActions( pCase->GetActions() );
		
	}else{
		pInsertActions( pIfElse->GetElseActions() );
	}
	GetTopic().NotifyActionStructureChanged( pIfElse );
	
	pSelectInserted();
}
