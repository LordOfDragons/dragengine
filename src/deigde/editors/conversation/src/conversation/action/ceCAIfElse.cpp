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

#include "ceCAIfElse.h"
#include "ceCAIfElseCase.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>



// Class ceCAIfElse
/////////////////////

// Constructor, destructor
////////////////////////////

ceCAIfElse::ceCAIfElse() :
ceConversationAction( eatIfElse ),
pTIMExpanded( true ),
pTIMElseExpanded( true ){
}

ceCAIfElse::ceCAIfElse( const ceCAIfElse &action ) :
ceConversationAction( action ),
pTIMExpanded( action.pTIMExpanded ),
pTIMElseExpanded( action.pTIMElseExpanded )
{
	const ceCAIfElseCaseList &cases = action.GetCases();
	const ceConversationActionList &elseActions = action.GetElseActions();
	ceConversationAction *newAction = NULL;
	ceCAIfElseCase *newCase = NULL;
	int i, count;
	
	try{
		count = cases.GetCount();
		for( i=0; i<count; i++ ){
			newCase = new ceCAIfElseCase( *cases.GetAt( i ) );
			pCases.Add( newCase );
			newCase->FreeReference();
			newCase = NULL;
		}
		
		count = elseActions.GetCount();
		for( i=0; i<count; i++ ){
			newAction = elseActions.GetAt( i )->CreateCopy();
			pElseActions.Add( newAction );
			newAction->FreeReference();
			newAction = NULL;
		}
		
	}catch( const deException & ){
		if( newAction ){
			newAction->FreeReference();
		}
		if( newCase ){
			newCase->FreeReference();
		}
		pCases.RemoveAll();
		pElseActions.RemoveAll();
		throw;
	}
}

ceCAIfElse::~ceCAIfElse(){
	pCases.RemoveAll();
	pElseActions.RemoveAll();
}



// Management
///////////////

ceConversationAction *ceCAIfElse::CreateCopy() const{
	return new ceCAIfElse( *this );
}



// UI
///////

void ceCAIfElse::SetTIMExpanded( bool expanded ){
	pTIMExpanded = expanded;
}

void ceCAIfElse::SetTIMElseExpanded( bool expanded ){
	pTIMElseExpanded = expanded;
}
