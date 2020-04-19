/* 
 * Drag[en]gine Android Launcher
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
#include <unistd.h>

#include "dealFDFileReader.h"
#include "common/exceptions.h"
#include "common/utils/decDateTime.h"



// Class dealFDFileReader
///////////////////////////

// Constructor, Destructor
////////////////////////////

dealFDFileReader::dealFDFileReader( const char *filename, int fileDescriptor, long offset, long length ) :
pFilename( filename ),
pFileDescriptor( fileDescriptor ),
pOffset( offset ),
pLength( length ),
pModificationTime( decDateTime::GetSystemTime() )
{
	if( ! filename || fileDescriptor == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( lseek( fileDescriptor, offset, SEEK_SET ) == -1 ){
		DETHROW( deeReadFile );
	}
}

dealFDFileReader::~dealFDFileReader(){
}



// Management
///////////////

const char *dealFDFileReader::GetFilename(){
	return pFilename;
}

int dealFDFileReader::GetLength(){
	return ( int )pLength;
}

TIME_SYSTEM dealFDFileReader::GetModificationTime(){
	return pModificationTime;
}

int dealFDFileReader::GetPosition(){
	const long position = lseek( pFileDescriptor, 0, SEEK_CUR );
	if( position < pOffset || position > pOffset + pLength ){
		DETHROW( deeReadFile );
	}
	return ( int )( position - pOffset );
}



// Seeking
////////////

void dealFDFileReader::SetPosition( int position ){
	if( position < 0 || position > ( int )pLength ){
		DETHROW( deeReadFile );
	}
	if( lseek( pFileDescriptor, pOffset + ( long )position, SEEK_SET ) == -1 ){
		DETHROW( deeReadFile );
	}
}

void dealFDFileReader::MovePosition( int offset ){
	long position = lseek( pFileDescriptor, 0, SEEK_CUR );
	if( position == -1 ){
		DETHROW( deeReadFile );
	}
	
	position += ( long )offset;
	if( position < pOffset || position > pOffset + pLength ){
		DETHROW( deeReadFile );
	}
	
	if( lseek( pFileDescriptor, position, SEEK_SET ) == -1 ){
		DETHROW( deeReadFile );
	}
}

void dealFDFileReader::SetPositionEnd( int position ){
	if( position > 0 || position < -( int )pLength ){
		DETHROW( deeReadFile );
	}
	if( lseek( pFileDescriptor, pOffset + pLength + ( long )position, SEEK_SET ) == -1 ){
		DETHROW( deeReadFile );
	}
}



// Reading
////////////

void dealFDFileReader::Read( void *buffer, int size ){
	const long position = lseek( pFileDescriptor, 0, SEEK_CUR );
	const long eof = pOffset + pLength;
	if( position < pOffset || position > eof ){
		DETHROW( deeReadFile );
	}
	
	const int readBytes = ( int )read( pFileDescriptor, buffer, decMath::min( size, ( int )( eof - position ) ) );
	if( readBytes == -1 ){
		DETHROW( deeReadFile );
	}
	if( readBytes < size ){ // eof or something else preventing full read
		DETHROW( deeReadFile );
	}
}
