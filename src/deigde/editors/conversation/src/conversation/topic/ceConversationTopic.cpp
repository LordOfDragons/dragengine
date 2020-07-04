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

#include "ceConversationTopic.h"
#include "../ceConversation.h"
#include "../action/ceConversationAction.h"
#include "../file/ceConversationFile.h"

#include <dragengine/common/exceptions.h>



// Class ceConversationTopic
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationTopic::ceConversationTopic( const char *id ) :
pFile( NULL ),
pID( id ),
pActiveCondition( NULL ){
}

ceConversationTopic::ceConversationTopic( const ceConversationTopic &topic ) :
pFile( NULL ),
pID( topic.pID ),
pActiveCondition( NULL )
{
	const int count = topic.pActions.GetCount();
	ceConversationActionReference action;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			action.TakeOver( topic.pActions.GetAt( i )->CreateCopy() );
			pActions.Add( action );
		}
		
	}catch( const deException & ){
		pActions.RemoveAll();
		throw;
	}
}

ceConversationTopic::~ceConversationTopic(){
	SetActive( NULL, NULL );
	pActions.RemoveAll();
}



// Management
///////////////

void ceConversationTopic::SetFile( ceConversationFile *file ){
	pFile = file;
}

void ceConversationTopic::SetID( const char *id ){
	if( pID == id ){
		return;
	}
	
	if( pFile && pFile->GetTopicList().HasWithID( id ) ){
		DETHROW( deeInvalidParam );
	}
	
	pID = id;
	
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyTopicChanged( pFile, this );
	}
}

void ceConversationTopic::NotifyActionStructureChanged( ceConversationAction *action ){
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyActionStructureChanged( pFile, this, action );
	}
}

void ceConversationTopic::NotifyActionChanged( ceConversationAction *action ){
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyActionChanged( pFile, this, action );
	}
}

void ceConversationTopic::SetActive( ceConversationAction *action, ceConversationCondition *condition ){
	if( pActiveAction == action && pActiveCondition == condition ){
		return;
	}
	
	pActiveAction = action;
	pActiveCondition = condition;
	
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyActiveChanged( pFile, this );
	}
}

void ceConversationTopic::NotifyConditionChanged( ceConversationAction *action,
ceConversationCondition *condition ){
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyConditionChanged( pFile, this, action, condition );
	}
}

void ceConversationTopic::NotifyConditionStructureChanged( ceConversationAction *action ){
	if( pFile && pFile->GetConversation() ){
		pFile->GetConversation()->NotifyConditionStructureChanged( pFile, this, action );
	}
}
