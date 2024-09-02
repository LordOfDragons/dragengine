/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
