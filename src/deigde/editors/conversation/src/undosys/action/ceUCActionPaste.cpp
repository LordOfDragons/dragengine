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
	pRemoveActions( pTopic->GetActionList() );
	pTopic->NotifyActionStructureChanged( NULL );
}

void ceUCActionPaste::Redo(){
	pInsertActions( pTopic->GetActionList() );
	pTopic->NotifyActionStructureChanged( NULL );
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
