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
#include "ceCAPlayerChoiceOption.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>



// Class ceCAPlayerChoiceOption
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceCAPlayerChoiceOption::ceCAPlayerChoiceOption() :
pCondition( NULL ),
pTIMExpanded( true ),
pTIMConditionExpanded( true ),
pTIMActionsExpanded( true ){
}

ceCAPlayerChoiceOption::ceCAPlayerChoiceOption( const ceCAPlayerChoiceOption &option ) :
pText( option.pText ),
pCondition( NULL ),
pTIMExpanded( option.pTIMExpanded ),
pTIMConditionExpanded( option.pTIMConditionExpanded ),
pTIMActionsExpanded( option.pTIMActionsExpanded )
{
	const ceConversationActionList &actions = option.GetActions();
	ceConversationAction *action = NULL;
	int i;
	
	try{
		if( option.pCondition ){
			SetCondition( option.pCondition->CreateCopy() );
		}
		
		const int count = actions.GetCount();
		for( i=0; i<count; i++ ){
			action = actions.GetAt( i )->CreateCopy();
			pActions.Add( action );
			action->FreeReference();
			action = NULL;
		}
		
	}catch( const deException & ){
		if( action ){
			action->FreeReference();
		}
		pActions.RemoveAll();
		throw;
	}
}

ceCAPlayerChoiceOption::~ceCAPlayerChoiceOption(){
	pActions.RemoveAll();
	SetCondition( NULL );
}



// Management
///////////////

void ceCAPlayerChoiceOption::SetText( const decUnicodeString &text ){
	pText = text;
}

void ceCAPlayerChoiceOption::SetCondition( ceConversationCondition *condition ){
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

void ceCAPlayerChoiceOption::SetTIMExpanded( bool expanded ){
	pTIMExpanded = expanded;
}

void ceCAPlayerChoiceOption::SetTIMConditionExpanded( bool expanded ){
	pTIMConditionExpanded = expanded;
}

void ceCAPlayerChoiceOption::SetTIMActionsExpanded( bool expanded ){
	pTIMActionsExpanded = expanded;
}
