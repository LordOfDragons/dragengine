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
#include <stdlib.h>
#include <string.h>

#include "ceConversationAction.h"
#include "ceCAIfElseCase.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>



// Class ceCAIfElseCase
/////////////////////////

// Constructor, destructor
////////////////////////////

ceCAIfElseCase::ceCAIfElseCase() :
pCondition( NULL ),
pTIMExpanded( true ),
pTIMConditionExpanded( true ),
pTIMActionsExpanded( true ){
}

ceCAIfElseCase::ceCAIfElseCase( const ceCAIfElseCase &ifcase ) :
pCondition( NULL ),
pTIMExpanded( ifcase.pTIMExpanded ),
pTIMConditionExpanded( ifcase.pTIMConditionExpanded ),
pTIMActionsExpanded( ifcase.pTIMActionsExpanded )
{
	const ceConversationActionList &actions = ifcase.GetActions();
	ceConversationAction *newAction = NULL;
	int i, count;
	
	try{
		if( ifcase.GetCondition() ){
			SetCondition( ifcase.GetCondition()->CreateCopy() );
		}
		
		count = actions.GetCount();
		for( i=0; i<count; i++ ){
			newAction = actions.GetAt( i )->CreateCopy();
			pActions.Add( newAction );
			newAction->FreeReference();
			newAction = NULL;
		}
		
	}catch( const deException & ){
		if( newAction ){
			newAction->FreeReference();
		}
		pActions.RemoveAll();
		throw;
	}
}

ceCAIfElseCase::~ceCAIfElseCase(){
	pActions.RemoveAll();
	SetCondition( NULL );
}



// Management
///////////////

void ceCAIfElseCase::SetCondition( ceConversationCondition *condition ){
	if( condition == pCondition ){
		return;
	}
	
	if( pCondition ){
		pCondition->FreeReference();
	}
	
	pCondition = condition;
	
	if( condition ){
		condition->AddReference();
	}
}



// UI
///////

void ceCAIfElseCase::SetTIMExpanded( bool expanded ){
	pTIMExpanded = expanded;
}

void ceCAIfElseCase::SetTIMConditionExpanded( bool expanded ){
	pTIMConditionExpanded = expanded;
}

void ceCAIfElseCase::SetTIMActionsExpanded( bool expanded ){
	pTIMActionsExpanded = expanded;
}
