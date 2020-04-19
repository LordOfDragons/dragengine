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

#include "ceConversationFile.h"
#include "../ceConversation.h"
#include "../topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceConversationFile
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationFile::ceConversationFile( const char *id ) :
pConversation( NULL ),
pID( id ),
pActiveTopic( NULL ){
}

ceConversationFile::ceConversationFile( const ceConversationFile &file ) :
pConversation( NULL ),
pID( file.pID ),
pActiveTopic( NULL )
{
	const int count = file.pTopics.GetCount();
	ceConversationTopic *topic = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			topic = new ceConversationTopic( *file.pTopics.GetAt( i ) );
			AddTopic( topic );
			topic->FreeReference();
			topic = NULL;
		}
		
	}catch( const deException & ){
		if( topic ){
			topic->FreeReference();
		}
		RemoveAllTopics();
		throw;
	}
}

ceConversationFile::~ceConversationFile(){
	RemoveAllTopics();
}



// Management
///////////////

void ceConversationFile::SetConversation( ceConversation *conversation ){
	pConversation = conversation;
}

void ceConversationFile::SetID( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	if( pID.Equals( id ) ){
		return;
	}
	
	if( pConversation && pConversation->GetFileList().HasWithID( id ) ){
		DETHROW( deeInvalidParam );
	}
	
	pID = id;
	
	if( pConversation ){
		pConversation->NotifyFileChanged( this );
	}
}



// Topics
///////////

void ceConversationFile::AddTopic( ceConversationTopic *topic ){
	if( ! topic || pTopics.HasWithID( topic->GetID().GetString() ) || topic->GetFile() ){
		DETHROW( deeInvalidParam );
	}
	
	pTopics.Add( topic );
	topic->SetFile( this );
	
	if( pConversation ){
		pConversation->NotifyTopicStructureChanged( this );
	}
	
	if( ! pActiveTopic ){
		SetActiveTopic( topic );
	}
}

void ceConversationFile::RemoveTopic( ceConversationTopic *topic ){
	if( ! topic || ! pTopics.Has( topic ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( topic == pActiveTopic ){
		if( pTopics.GetCount() == 1 ){
			SetActiveTopic( NULL );
			
		}else{
			if( pTopics.GetAt( 0 ) == topic ){
				SetActiveTopic( pTopics.GetAt( 1 ) );
				
			}else{
				SetActiveTopic( pTopics.GetAt( 0 ) );
			}
		}
	}
	
	topic->SetFile( NULL );
	pTopics.Remove( topic );
	
	if( pConversation ){
		pConversation->NotifyTopicStructureChanged( this );
	}
}

void ceConversationFile::RemoveAllTopics(){
	const int count = pTopics.GetCount();
	int i;
	
	SetActiveTopic( NULL );
	
	for( i=0; i<count; i++ ){
		pTopics.GetAt( i )->SetFile( NULL );
	}
	pTopics.RemoveAll();
	
	if( pConversation ){
		pConversation->NotifyTopicStructureChanged( this );
	}
}

void ceConversationFile::SetActiveTopic( ceConversationTopic *topic ){
	if( topic == pActiveTopic ){
		return;
	}
	
	if( pActiveTopic ){
		pActiveTopic->FreeReference();
	}
	
	pActiveTopic = topic;
	
	if( topic ){
		topic->AddReference();
	}
	
	if( pConversation ){
		pConversation->NotifyActiveTopicChanged( this );
	}
}
