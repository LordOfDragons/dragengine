/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglDelayedFileWrite.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class deoglDelayedFileWrite
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDelayedFileWrite::deoglDelayedFileWrite( const decPath &path ) :
pMemoryFile( NULL ),
pPath( path ){
	try{
		pMemoryFile = new decMemoryFile( path.GetPathUnix() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDelayedFileWrite::~deoglDelayedFileWrite(){
	pCleanUp();
}



// Management
///////////////

decBaseFileWriter *deoglDelayedFileWrite::GetFileWriter() const{
	return new decMemoryFileWriter( pMemoryFile, false );
}

void deoglDelayedFileWrite::SaveFile( deVirtualFileSystem &vfs ){
	decBaseFileWriter *fileWriter = NULL;
	decPath noConstPath( pPath );
	
	try{
		fileWriter = vfs.OpenFileForWriting( noConstPath );
		fileWriter->Write( pMemoryFile->GetPointer(), pMemoryFile->GetLength() );
		fileWriter->FreeReference();
		
	}catch( const deException & ){
		if( fileWriter ){
			fileWriter->FreeReference();
		}
		throw;
	}
}



// Private Functions
//////////////////////

void deoglDelayedFileWrite::pCleanUp(){
	if( pMemoryFile ){
		pMemoryFile->FreeReference();
	}
}
