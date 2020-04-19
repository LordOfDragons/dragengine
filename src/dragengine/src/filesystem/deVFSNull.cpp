/* 
 * Drag[en]gine Game Engine
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

#include <stdlib.h>
#include <string.h>

#include "deVFSNull.h"
#include "dePathList.h"
#include "../common/file/decPath.h"
#include "../common/file/decNullFileWriter.h"
#include "../common/exceptions.h"



// Class deVFSNull
////////////////////

// Constructor, destructor
////////////////////////////

deVFSNull::deVFSNull( const decPath &rootPath ) :
deVFSContainer( rootPath ){
}

deVFSNull::~deVFSNull(){
}



// Management
///////////////

bool deVFSNull::ExistsFile( const decPath & ){
	return false;
}

bool deVFSNull::CanReadFile( const decPath & ){
	return false;
}

bool deVFSNull::CanWriteFile( const decPath & ){
	return true;
}

bool deVFSNull::CanDeleteFile( const decPath & ){
	return true;
}

decBaseFileReader *deVFSNull::OpenFileForReading( const decPath &path ){
	DETHROW( deeInvalidAction );
}

decBaseFileWriter *deVFSNull::OpenFileForWriting( const decPath &path ){
	return new decNullFileWriter( path.GetPathUnix() );
}

void deVFSNull::DeleteFile( const decPath & ){
}

void deVFSNull::TouchFile( const decPath & ){
}

void deVFSNull::SearchFiles( const decPath &, deContainerFileSearch& ){
}

deVFSContainer::eFileTypes deVFSNull::GetFileType( const decPath & ){
	DETHROW( deeInvalidAction );
}

uint64_t deVFSNull::GetFileSize( const decPath & ){
	DETHROW( deeInvalidAction );
}

TIME_SYSTEM deVFSNull::GetFileModificationTime( const decPath & ){
	DETHROW( deeInvalidAction );
}
