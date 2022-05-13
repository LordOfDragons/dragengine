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

#include "decMemoryFile.h"
#include "decMemoryFileWriter.h"
#include "../exceptions.h"



// Class decMemoryFileWriter
//////////////////////////////

// Constructor, Destructor
////////////////////////////

decMemoryFileWriter::decMemoryFileWriter( decMemoryFile *memoryFile, bool append ) :
pFile( NULL ),
pPosition( 0 )
{
	if( ! memoryFile ){
		DETHROW( deeInvalidParam );
	}
	
	pFile = memoryFile;
	
	if( append ){
		pPosition = memoryFile->GetLength();
		
	}else{
		memoryFile->Resize( 0, false );
	}
	
	memoryFile->Touch();
	memoryFile->AddReference();
}

decMemoryFileWriter::~decMemoryFileWriter(){
	if( pFile ){
		pFile->Touch();
		pFile->FreeReference();
	}
}



// Seeking
////////////

int decMemoryFileWriter::GetPosition(){
	return pPosition;
}

void decMemoryFileWriter::SetPosition( int position ){
	if( position < 0 || position > pFile->GetLength() ){
		DETHROW( deeOutOfBoundary );
	}
	pPosition = position;
}

void decMemoryFileWriter::MovePosition( int offset ){
	const int newPos = pPosition + offset;
	if( newPos < 0 || newPos > pFile->GetLength() ){
		DETHROW( deeOutOfBoundary );
	}
	pPosition = newPos;
}

void decMemoryFileWriter::SetPositionEnd( int position ){
	if( position < 0 || position > pFile->GetLength() ){
		DETHROW( deeOutOfBoundary );
	}
	pPosition = pFile->GetLength() - position;
}



// Management
///////////////

const char *decMemoryFileWriter::GetFilename(){
	return pFile->GetFilename();
}

void decMemoryFileWriter::Write( const void *buffer, int size ){
	if( size == 0 ){
		return;
	}
	
	if( ! buffer || size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPosition + size > pFile->GetLength() ){
		pFile->Resize( pPosition + size, false );
	}
	memcpy( pFile->GetPointer() + pPosition, buffer, size );
	pPosition += size;
}
