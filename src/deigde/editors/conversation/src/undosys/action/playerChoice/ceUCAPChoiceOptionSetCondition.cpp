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

#include "ceUCAPChoiceOptionSetCondition.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/condition/ceConversationCondition.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceOptionSetCondition
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceOptionSetCondition::ceUCAPChoiceOptionSetCondition( ceConversationTopic *topic,
ceCAPlayerChoice *ifElse, ceCAPlayerChoiceOption *ifcase, ceConversationCondition *newCondition ) :
pTopic( NULL ),
pPlayerChoice( NULL ),
pOption( NULL ),
pOldCondition( NULL ),
pNewCondition( NULL )
{
	if( ! topic || ! ifElse || ! ifcase ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Player Choice Option set case condition" );
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = ifElse;
	ifElse->AddReference();
	
	pOption = ifcase;
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

ceUCAPChoiceOptionSetCondition::~ceUCAPChoiceOptionSetCondition(){
	if( pNewCondition ){
		pNewCondition->FreeReference();
	}
	if( pOldCondition ){
		pOldCondition->FreeReference();
	}
	if( pOption ){
		pOption->FreeReference();
	}
	if( pPlayerChoice ){
		pPlayerChoice->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAPChoiceOptionSetCondition::Undo(){
	pOption->SetCondition( pOldCondition );
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
	
	pTopic->SetActive( pPlayerChoice, pOldCondition );
}

void ceUCAPChoiceOptionSetCondition::Redo(){
	pOption->SetCondition( pNewCondition );
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
	
	pTopic->SetActive( pPlayerChoice, pNewCondition );
}
