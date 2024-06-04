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
