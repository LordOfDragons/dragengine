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

#include "ceUCCLogicPaste.h"
#include "../ceUConditionHelpers.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceCConditionLogic.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCLogicPaste
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCLogicPaste::ceUCCLogicPaste( ceConversationTopic *topic, ceConversationAction *action,
ceCConditionLogic *logic, const ceConversationConditionList &conditions ){
	if( ! topic || ! action || ! logic || conditions.GetCount() == 0 ) DETHROW( deeInvalidParam );
	
	const int count = conditions.GetCount();
	ceConversationCondition *newCondition = NULL;
	int i;
	
	pTopic = NULL;
	pAction = NULL;
	pLogic = NULL;
	
	if( count == 1 ){
		SetShortInfo( "Logic Paste Condition" );
		
	}else{
		SetShortInfo( "Logic Paste Conditions" );
	}
	
	pTopic = topic;
	topic->AddReference();
	
	pAction = action;
	action->AddReference();
	
	pLogic = logic;
	logic->AddReference();
	
	try{
		for( i=0; i<count; i++ ){
			newCondition = conditions.GetAt( i )->CreateCopy();
			pConditions.Add( newCondition );
			newCondition->FreeReference();
			newCondition = NULL;
		}
		
	}catch( const deException & ){
		if( newCondition ){
			newCondition->FreeReference();
		}
		throw;
	}
}

ceUCCLogicPaste::~ceUCCLogicPaste(){
	pConditions.RemoveAll();
	if( pLogic ){
		pLogic->FreeReference();
	}
	if( pAction ){
		pAction->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCCLogicPaste::Undo(){
	const int count = pConditions.GetCount();
	if( count == 0 ){
		return;
	}
	
	ceConversationCondition * const activateCondition = ActivateConditionAfterRemove();
	
	int i;
	for( i=0; i<count; i++ ){
		pLogic->GetConditions().Remove( pConditions.GetAt( i ) );
	}
	
	pTopic->NotifyConditionStructureChanged( pAction );
	
	pTopic->SetActive( pAction, activateCondition ? activateCondition : pLogic );
}

void ceUCCLogicPaste::Redo(){
	const int count = pConditions.GetCount();
	if( count == 0 ){
		return;
	}
	
	int i;
	for( i=0; i<count; i++ ){
		pLogic->GetConditions().Add( pConditions.GetAt( i ) );
	}
	
	pTopic->NotifyConditionStructureChanged( pAction );
	
	pTopic->SetActive( pAction, pConditions.GetAt( 0 ) );
}

ceConversationCondition *ceUCCLogicPaste::ActivateConditionAfterRemove() const{
	if( pConditions.GetCount() == 0 ){
		return NULL;
	}
	
	int index = pLogic->GetConditions().IndexOf( pConditions.GetAt( pConditions.GetCount() - 1 ) );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	if( index < pLogic->GetConditions().GetCount() - 1 ){
		return pLogic->GetConditions().GetAt( index + 1 );
	}
	
	index = pLogic->GetConditions().IndexOf( pConditions.GetAt( 0 ) );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( index > 0 ){
		return pLogic->GetConditions().GetAt( index - 1 );
		
	}else{
		return NULL;
	}
}
