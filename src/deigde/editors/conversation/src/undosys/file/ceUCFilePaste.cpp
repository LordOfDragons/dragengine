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

#include "ceUCFilePaste.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/file/ceConversationFile.h"

#include <dragengine/common/exceptions.h>



// Class ceUCFilePaste
////////////////////////

// Constructor, destructor
////////////////////////////

ceUCFilePaste::ceUCFilePaste( ceConversation *conversation, const ceConversationFileList &files ) :
pConversation( NULL ){
	if( ! conversation || files.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = files.GetCount();
	ceConversationFile *newFile = NULL;
	int i;
	
	if( count == 1 ){
		SetShortInfo( "Paste file" );
		
	}else{
		SetShortInfo( "Paste files" );
	}
	
	try{
		for( i=0; i<count; i++ ){
			newFile = new ceConversationFile( *files.GetAt( i ) );
			decString fileID( newFile->GetID() );
			int newNameIndex = 1;
			
			while( conversation->GetFileList().HasWithID( fileID ) ){
				newNameIndex++;
				fileID.Format( "%s_%i", newFile->GetID().GetString(), newNameIndex );
			}
			
			newFile->SetID( fileID );
			
			pFiles.Add( newFile );
			newFile->FreeReference();
			newFile = NULL;
		}
		
	}catch( const deException & ){
		if( newFile ){
			newFile->FreeReference();
		}
		throw;
	}
	
	pConversation = conversation;
	conversation->AddReference();
}

ceUCFilePaste::~ceUCFilePaste(){
	pFiles.RemoveAll();
	if( pConversation ){
		pConversation->FreeReference();
	}
}



// Management
///////////////

void ceUCFilePaste::Undo(){
	const int count = pFiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pConversation->RemoveFile( pFiles.GetAt( i ) );
	}
}

void ceUCFilePaste::Redo(){
	const int count = pFiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pConversation->AddFile( pFiles.GetAt( i ) );
	}
}
