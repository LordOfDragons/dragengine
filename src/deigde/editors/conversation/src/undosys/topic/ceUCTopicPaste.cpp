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

#include "ceUCTopicPaste.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCTopicPaste
/////////////////////////

// Constructor, destructor
////////////////////////////

ceUCTopicPaste::ceUCTopicPaste( ceConversationFile *file, const ceConversationTopicList &topics ) :
pFile( NULL ){
	if( ! file || topics.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = topics.GetCount();
	ceConversationTopic *newTopic = NULL;
	int i;
	
	if( count == 1 ){
		SetShortInfo( "Paste topic" );
		
	}else{
		SetShortInfo( "Paste topics" );
	}
	
	try{
		for( i=0; i<count; i++ ){
			newTopic = new ceConversationTopic( *topics.GetAt( i ) );
			decString topicID( newTopic->GetID() );
			int newNameIndex = 1;
			
			while( file->GetTopicList().HasWithID( topicID ) ){
				newNameIndex++;
				topicID.Format( "%s_%i", newTopic->GetID().GetString(), newNameIndex );
			}
			
			newTopic->SetID( topicID );
			
			pTopics.Add( newTopic );
			newTopic->FreeReference();
			newTopic = NULL;
		}
		
	}catch( const deException & ){
		if( newTopic ){
			newTopic->FreeReference();
		}
		throw;
	}
	
	pFile = file;
	file->AddReference();
}

ceUCTopicPaste::~ceUCTopicPaste(){
	pTopics.RemoveAll();
	if( pFile ){
		pFile->FreeReference();
	}
}



// Management
///////////////

void ceUCTopicPaste::Undo(){
	const int count = pTopics.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pFile->RemoveTopic( pTopics.GetAt( i ) );
	}
}

void ceUCTopicPaste::Redo(){
	const int count = pTopics.GetCount();
	if( count == 0 ){
		return;
	}
	
	int i;
	for( i=0; i<count; i++ ){
		pFile->AddTopic( pTopics.GetAt( i ) );
	}
	pFile->SetActiveTopic( pTopics.GetAt( count - 1 ) );
}
