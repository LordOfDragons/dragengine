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

#include "ceUCAWaitSetCondition.h"
#include "../../../conversation/action/ceCAWait.h"
#include "../../../conversation/condition/ceConversationCondition.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAWaitSetCondition
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAWaitSetCondition::ceUCAWaitSetCondition( ceConversationTopic *topic, ceCAWait *wait, ceConversationCondition *newCondition ){
	if( ! topic || ! wait ) DETHROW( deeInvalidParam );
	
	pTopic = NULL;
	pWait = NULL;
	pOldCondition = NULL;
	pNewCondition = NULL;
	
	SetShortInfo( "Action Wait Set Condition" );
	
	pTopic = topic;
	topic->AddReference();
	
	pWait = wait;
	wait->AddReference();
	
	pOldCondition = wait->GetCondition();
	if( pOldCondition ){
		pOldCondition->AddReference();
	}
	
	pNewCondition = newCondition;
	if( newCondition ){
		newCondition->AddReference();
	}
}

ceUCAWaitSetCondition::~ceUCAWaitSetCondition(){
	if( pNewCondition ){
		pNewCondition->FreeReference();
	}
	if( pOldCondition ){
		pOldCondition->FreeReference();
	}
	if( pWait ){
		pWait->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAWaitSetCondition::Undo(){
	pWait->SetCondition( pOldCondition );
	pTopic->NotifyActionStructureChanged( pWait );
}

void ceUCAWaitSetCondition::Redo(){
	pWait->SetCondition( pNewCondition );
	pTopic->NotifyActionStructureChanged( pWait );
}
