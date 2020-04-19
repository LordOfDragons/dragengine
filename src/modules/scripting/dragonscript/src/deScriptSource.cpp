/* 
 * Drag[en]gine DragonScript Script Module
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
#include "deScriptSource.h"
#include "dragengine/filesystem/deVirtualFileSystem.h"
#include "dragengine/common/file/decBaseFileReader.h"
#include <libdscript/exceptions.h>



// Class deScriptSource
/////////////////////////

// Constructor, destructor
////////////////////////////

deScriptSource::deScriptSource( const deVirtualFileSystem &vfs, const decPath &path ) :
pVFS( vfs ),
pFileReader( NULL ),
pPath( path ),
pPathString( path.GetPathUnix() ),
pPosition( 0 ),
pLength( 0 ){
}

deScriptSource::~deScriptSource(){
	if( pFileReader ){
		pFileReader->FreeReference();
	}
}



// Management
///////////////

const char *deScriptSource::GetName(){
	return pPathString.GetString();
}

void deScriptSource::Open(){
	if( ! pFileReader ){
		pFileReader = pVFS.OpenFileForReading( pPath );
		if( ! pFileReader ) DSTHROW( dueOutOfMemory );
		
	}else{
		pFileReader->SetPosition( 0 );
	}
	
	pLength = pFileReader->GetLength();
	pPosition = 0;
}

int deScriptSource::ReadData( char *buffer, int size ){
	if( ! buffer || ! pFileReader ) DSTHROW( dueInvalidParam );
	
	if( pPosition + size > pLength ){
		size = pLength - pPosition;
	}
	
	if( size > 0 ) pFileReader->Read( buffer, size );
	
	pPosition += size;
	
	return size;
}

void deScriptSource::Close(){
	if( pFileReader ){
		pFileReader->FreeReference();
		pFileReader = NULL;
	}
}
