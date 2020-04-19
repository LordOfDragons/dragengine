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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ceClipboardDataFile.h"
#include "../conversation/file/ceConversationFile.h"

#include <dragengine/common/exceptions.h>



// Class ceClipboardDataFile
//////////////////////////////

const char * const ceClipboardDataFile::TYPE_NAME = "file";

// Constructor, destructor
////////////////////////////

ceClipboardDataFile::ceClipboardDataFile( const ceConversationFileList &files ) :
igdeClipboardData( TYPE_NAME )
{
	const int count = files.GetCount();
	ceConversationFile *file = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			file = new ceConversationFile( *files.GetAt( i ) );
			pFiles.Add( file );
			file->FreeReference();
			file = NULL;
		}
		
	}catch( const deException & ){
		if( file ){
			file->FreeReference();
		}
		throw;
	}
}

ceClipboardDataFile::~ceClipboardDataFile(){
	pFiles.RemoveAll();
}
