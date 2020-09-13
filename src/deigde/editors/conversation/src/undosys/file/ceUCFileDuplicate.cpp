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

#include "ceUCFileDuplicate.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/file/ceConversationFile.h"

#include <dragengine/common/exceptions.h>



// Class ceUCFileDuplicate
////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCFileDuplicate::ceUCFileDuplicate( ceConversation *conversation, const ceConversationFile &file, const char *newID ) :
pConversation( NULL ),
pFile( NULL )
{
	if( ! conversation ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Duplicate file" );
	
	try{
		pFile = new ceConversationFile( file );
		pFile->SetID( newID );
		
	}catch( const deException & ){
		if( pFile ){
			pFile->FreeReference();
		}
		throw;
	}
	
	pConversation = conversation;
	conversation->AddReference();
}

ceUCFileDuplicate::~ceUCFileDuplicate(){
	if( pFile ){
		pFile->FreeReference();
	}
	if( pConversation ){
		pConversation->FreeReference();
	}
}



// Management
///////////////

void ceUCFileDuplicate::Undo(){
	pConversation->RemoveFile( pFile );
}

void ceUCFileDuplicate::Redo(){
	pConversation->AddFile( pFile );
	pConversation->SetActiveFile( pFile );
}
