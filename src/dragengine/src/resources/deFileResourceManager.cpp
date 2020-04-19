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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deFileResourceManager.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/file/decPath.h"
#include "../filesystem/deVirtualFileSystem.h"
#include "../systems/deInputSystem.h"
#include "../systems/modules/input/deBaseInputModule.h"



// Class deFileResourceManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deFileResourceManager::deFileResourceManager( deEngine *engine, eResourceType type ) :
deResourceManager( engine, type ){
}

deFileResourceManager::~deFileResourceManager(){
}



// Management
///////////////

bool deFileResourceManager::FindFileForReading( decPath &path, const deVirtualFileSystem &vfs,
const char *filename, const char *basePath ) const{
	if( ! decPath::IsUnixPathAbsolute( basePath ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( decPath::IsUnixPathAbsolute( filename ) ){
		path.SetFromUnix( filename );
		if( vfs.ExistsFile( path ) ){
			return true;
		}
		
	}else{
		path.SetFromUnix( basePath );
		path.AddUnixPath( filename );
		if( vfs.ExistsFile( path ) ){
			return true;
		}
	}
	
	return false;
}

decBaseFileReader *deFileResourceManager::OpenFileForReading(
const deVirtualFileSystem &vfs, const char *filename ) const{
	decPath path;
	path.SetFromUnix( filename );
	return vfs.OpenFileForReading( path );
}

decBaseFileWriter *deFileResourceManager::OpenFileForWriting(
const deVirtualFileSystem &vfs, const char *filename ) const{
	decPath path;
	path.SetFromUnix( filename );
	return vfs.OpenFileForWriting( path );
}
