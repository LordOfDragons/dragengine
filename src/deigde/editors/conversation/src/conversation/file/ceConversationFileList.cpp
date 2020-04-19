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
#include "ceConversationFileList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversationFile
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationFileList::ceConversationFileList(){
}

ceConversationFileList::~ceConversationFileList(){
}



// Management
///////////////

int ceConversationFileList::GetCount() const{
	return pFiles.GetCount();
}

ceConversationFile *ceConversationFileList::GetAt( int index ) const{
	return ( ceConversationFile* )pFiles.GetAt( index );
}

ceConversationFile *ceConversationFileList::GetWithID( const char *id ) const{
	const int count = pFiles.GetCount();
	ceConversationFile *file;
	int i;
	
	for( i=0; i<count; i++ ){
		file = ( ceConversationFile* )pFiles.GetAt( i );
		
		if( file->GetID().Equals( id ) ){
			return file;
		}
	}
	
	return NULL;
}

int ceConversationFileList::IndexOf( ceConversationFile *file ) const{
	return pFiles.IndexOf( file );
}

int ceConversationFileList::IndexOfWithID( const char *id ) const{
	const int count = pFiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceConversationFile* )pFiles.GetAt( i ) )->GetID().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceConversationFileList::Has( ceConversationFile *file ) const{
	return pFiles.Has( file );
}

bool ceConversationFileList::HasWithID( const char *id ) const{
	const int count = pFiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceConversationFile* )pFiles.GetAt( i ) )->GetID().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

void ceConversationFileList::Add( ceConversationFile *file ){
	if( ! file || HasWithID( file->GetID().GetString() ) ) DETHROW( deeInvalidParam );
	
	pFiles.Add( file );
}

void ceConversationFileList::Remove( ceConversationFile *file ){
	pFiles.Remove( file );
}

void ceConversationFileList::RemoveAll(){
	pFiles.RemoveAll();
}



ceConversationFileList &ceConversationFileList::operator=( const ceConversationFileList &list ){
	pFiles = list.pFiles;
	return *this;
}
