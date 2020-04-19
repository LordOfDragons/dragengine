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

#include "ceCAWait.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>



// Class ceCAWait
///////////////////

// Constructor, destructor
////////////////////////////

ceCAWait::ceCAWait() :
ceConversationAction( eatWait ),
pCondition( NULL ),
pInterval( 0.2f ),
pTIMExpanded( true ),
pTIMConditionExpanded( true ),
pTIMActionsExpanded( true ){
}

ceCAWait::ceCAWait( const ceCAWait &action ) :
ceConversationAction( action ),
pCondition( NULL ),
pInterval( action.GetInterval() ),
pTIMExpanded( action.pTIMExpanded ),
pTIMConditionExpanded( action.pTIMConditionExpanded ),
pTIMActionsExpanded( action.pTIMActionsExpanded )
{
	const ceConversationActionList &actions = action.GetActions();
	ceConversationAction *newAction = NULL;
	int i, count;
	
	try{
		if( action.GetCondition() ){
			pCondition = action.GetCondition()->CreateCopy();
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
		SetCondition( NULL );
		pActions.RemoveAll();
		throw;
	}
}

ceCAWait::~ceCAWait(){
	SetCondition( NULL );
	pActions.RemoveAll();
}



// Management
///////////////

void ceCAWait::SetCondition( ceConversationCondition *condition ){
	if( condition != pCondition ){
		if( pCondition ){
			pCondition->FreeReference();
		}
		
		pCondition = condition;
		
		if( condition ){
			condition->AddReference();
		}
	}
}

void ceCAWait::SetInterval( float interval ){
	if( interval < 0.0f ){
		pInterval = 0.0f;
		
	}else{
		pInterval = interval;
	}
}



ceConversationAction *ceCAWait::CreateCopy() const{
	return new ceCAWait( *this );
}



// UI
///////

void ceCAWait::SetTIMExpanded( bool expanded ){
	pTIMExpanded = expanded;
}

void ceCAWait::SetTIMConditionExpanded( bool expanded ){
	pTIMConditionExpanded = expanded;
}

void ceCAWait::SetTIMActionsExpanded( bool expanded ){
	pTIMActionsExpanded = expanded;
}
