/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
