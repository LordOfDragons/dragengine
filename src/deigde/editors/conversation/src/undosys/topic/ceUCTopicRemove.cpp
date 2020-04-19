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

#include "ceUCTopicRemove.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCTopicRemove
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCTopicRemove::ceUCTopicRemove( ceConversationTopic *topic ){
	if( ! topic ) DETHROW( deeInvalidParam );
	
	ceConversationFile *file = topic->GetFile();
	if( ! file ) DETHROW( deeInvalidParam );
	
	pFile = NULL;
	pTopic = NULL;
	
	SetShortInfo( "Remove Topic" );
	
	pFile = file;
	file->AddReference();
	
	pTopic = topic;
	topic->AddReference();
}

ceUCTopicRemove::~ceUCTopicRemove(){
	if( pTopic ){
		pTopic->FreeReference();
	}
	if( pFile ){
		pFile->FreeReference();
	}
}



// Management
///////////////

void ceUCTopicRemove::Undo(){
	pFile->AddTopic( pTopic );
}

void ceUCTopicRemove::Redo(){
	pFile->RemoveTopic( pTopic );
}
