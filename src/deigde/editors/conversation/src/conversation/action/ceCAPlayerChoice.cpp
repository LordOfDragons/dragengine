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

#include "ceCAPlayerChoice.h"
#include "ceCAPlayerChoiceOption.h"

#include <dragengine/common/exceptions.h>



// Class ceCAPlayerChoice
///////////////////////////

// Constructor, destructor
////////////////////////////

ceCAPlayerChoice::ceCAPlayerChoice() :
ceConversationAction( eatPlayerChoice ),
pTIMExpanded( true ),
pTIMActionsExpanded( true ){
}

ceCAPlayerChoice::ceCAPlayerChoice( const ceCAPlayerChoice &action ) :
ceConversationAction( action ),
pTIMExpanded( action.pTIMExpanded ),
pTIMActionsExpanded( action.pTIMActionsExpanded )
{
	const ceCAPlayerChoiceOptionList &options = action.GetOptions();
	const ceConversationActionList &actions = action.GetActions();
	ceCAPlayerChoiceOption *newOption = NULL;
	ceConversationAction *newAction = NULL;
	int i, count;
	
	pVariableName = action.GetVariableName();
	
	try{
		count = options.GetCount();
		for( i=0; i<count; i++ ){
			newOption = new ceCAPlayerChoiceOption( *options.GetAt( i ) );
			pOptions.Add( newOption );
			newOption->FreeReference();
			newOption = NULL;
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
		if( newOption ){
			newOption->FreeReference();
		}
		pActions.RemoveAll();
		pOptions.RemoveAll();
		throw;
	}
}

ceCAPlayerChoice::~ceCAPlayerChoice(){
	pOptions.RemoveAll();
	pActions.RemoveAll();
}



// Management
///////////////

ceConversationAction *ceCAPlayerChoice::CreateCopy() const{
	return new ceCAPlayerChoice( *this );
}

void ceCAPlayerChoice::SetVariableName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	pVariableName = name;
}



// UI
///////

void ceCAPlayerChoice::SetTIMExpanded( bool expanded ){
	pTIMExpanded = expanded;
}

void ceCAPlayerChoice::SetTIMActionsExpanded( bool expanded ){
	pTIMActionsExpanded = expanded;
}
