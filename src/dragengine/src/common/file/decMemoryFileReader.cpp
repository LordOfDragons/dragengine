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
#include "decMemoryFileReader.h"
#include "../exceptions.h"



// Class decMemoryFileReader
//////////////////////////////

// Constructor, Destructor
////////////////////////////

decMemoryFileReader::decMemoryFileReader( decMemoryFile *memoryFile ) :
pFile( NULL ),
pPosition( 0 )
{
	if( ! memoryFile ){
		DETHROW( deeInvalidParam );
	}
	
	pFile = memoryFile;
	memoryFile->AddReference();
}

decMemoryFileReader::decMemoryFileReader( const decMemoryFileReader &reader ) :
pFile( reader.pFile ),
pPosition( reader.pPosition )
{
	pFile->AddReference();
}

decMemoryFileReader::~decMemoryFileReader(){
	if( pFile ){
		pFile->FreeReference();
	}
}



// Management
///////////////

const char *decMemoryFileReader::GetFilename(){
	return pFile->GetFilename();
}

int decMemoryFileReader::GetLength(){
	return pFile->GetLength();
}

TIME_SYSTEM decMemoryFileReader::GetModificationTime(){
	return pFile->GetModificationTime();
}



// Seeking
////////////

int decMemoryFileReader::GetPosition(){
	return pPosition;
}

void decMemoryFileReader::SetPosition( int position ){
	if( position < 0 || position > pFile->GetLength() ){
		DETHROW( deeOutOfBoundary );
	}
	pPosition = position;
}

void decMemoryFileReader::MovePosition( int offset ){
	const int newPos = pPosition + offset;
	if( newPos < 0 || newPos > pFile->GetLength() ){
		DETHROW( deeOutOfBoundary );
	}
	pPosition = newPos;
}

void decMemoryFileReader::SetPositionEnd( int position ){
	if( position < 0 || position > pFile->GetLength() ){
		DETHROW( deeOutOfBoundary );
	}
	pPosition = pFile->GetLength() - position;
}



// Reading
////////////

void decMemoryFileReader::Read( void *buffer, int size ){
	if( ! buffer ){
		DETHROW( deeInvalidParam );
	}
	if( pPosition + size > pFile->GetLength() ){
		DETHROW( deeInvalidParam );
	}
	
	memcpy( buffer, pFile->GetPointer() + pPosition, size );
	pPosition += size;
}

decBaseFileReader::Ref decMemoryFileReader::Duplicate(){
	return decBaseFileReader::Ref::New( new decMemoryFileReader( *this ) );
}
