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
#include <string.h>
#include <stdlib.h>

#include "ceUCActionPaste.h"
#include "../../conversation/action/ceConversationAction.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCActionPaste
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCActionPaste::ceUCActionPaste( ceConversationTopic *topic, const ceConversationActionList &actions, int index ){
	if( ! topic || actions.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = actions.GetCount();
	ceConversationAction *newAction = NULL;
	int i;
	
	pTopic = NULL;
	pIndex = index;
	
	if( count == 1 ){
		SetShortInfo( "Paste Action" );
		
	}else{
		SetShortInfo( "Paste Actions" );
	}
	
	pTopic = topic;
	topic->AddReference();
	
	try{
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
		throw;
	}
}

ceUCActionPaste::~ceUCActionPaste(){
	pActions.RemoveAll();
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCActionPaste::Undo(){
	ceConversationAction * const activateAction = ActivateActionAfterRemove( pTopic->GetActionList() );
	
	pRemoveActions( pTopic->GetActionList() );
	pTopic->NotifyActionStructureChanged( NULL );
	
	if( activateAction ){
		pTopic->SetActive( activateAction, NULL );
	}
}

void ceUCActionPaste::Redo(){
	pInsertActions( pTopic->GetActionList() );
	pTopic->NotifyActionStructureChanged( NULL );
	
	pSelectInserted();
}



// Protected Functions
////////////////////////

void ceUCActionPaste::pInsertActions( ceConversationActionList &list ){
	const int count = pActions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		list.InsertAt( pActions.GetAt( i ), pIndex + i );
	}
}

void ceUCActionPaste::pRemoveActions( ceConversationActionList &list ){
	const int count = pActions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		list.Remove( pActions.GetAt( i ) );
	}
}

void ceUCActionPaste::pSelectInserted(){
	if( pActions.GetCount() > 0 ){
		pTopic->SetActive( pActions.GetAt( 0 ), NULL );
	}
}

ceConversationAction *ceUCActionPaste::ActivateActionAfterRemove(
const ceConversationActionList &list ) const{
	if( pActions.GetCount() == 0 ){
		return NULL;
	}
	
	int index = list.IndexOf( pActions.GetAt( pActions.GetCount() - 1 ) );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	if( index < list.GetCount() - 1 ){
		return list.GetAt( index + 1 );
	}
	
	index = list.IndexOf( pActions.GetAt( 0 ) );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( index > 0 ){
		return list.GetAt( index - 1 );
		
	}else{
		return NULL;
	}
}
