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

#include "ceUCTopicDuplicate.h"
#include "../../conversation/topic/ceConversationTopic.h"
#include "../../conversation/file/ceConversationFile.h"

#include <dragengine/common/exceptions.h>



// Class ceUCTopicDuplicate
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCTopicDuplicate::ceUCTopicDuplicate( ceConversationFile *file, const ceConversationTopic &topic, const char *newID ) :
pFile( NULL ),
pTopic( NULL )
{
	if( ! file ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Duplicate topic" );
	
	try{
		pTopic = new ceConversationTopic( topic );
		pTopic->SetID( newID );
		
	}catch( const deException & ){
		if( pTopic ){
			pTopic->FreeReference();
		}
		throw;
	}
	
	pFile = file;
	file->AddReference();
}

ceUCTopicDuplicate::~ceUCTopicDuplicate(){
	if( pTopic ){
		pTopic->FreeReference();
	}
	if( pFile ){
		pFile->FreeReference();
	}
}



// Management
///////////////

void ceUCTopicDuplicate::Undo(){
	pFile->RemoveTopic( pTopic );
}

void ceUCTopicDuplicate::Redo(){
	pFile->AddTopic( pTopic );
	pFile->SetActiveTopic( pTopic );
}
