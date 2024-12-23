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

#include "decDiskFileWriter.h"
#include "../exceptions.h"



// Class decDiskFileWriter
///////////////////////////

// Constructor, destructor
////////////////////////////

decDiskFileWriter::decDiskFileWriter( const char *filename, bool append ) :
pFilename( filename ),
pFile( NULL )
{
	pFile = fopen( filename, append ? "ab" : "wb" );
	if( ! pFile ){
		DETHROW( deeFileNotFound );
	}
}

decDiskFileWriter::~decDiskFileWriter(){
	if( pFile ){
		fclose( pFile );
	}
}



// Management
///////////////

const char *decDiskFileWriter::GetFilename(){
	return pFilename.GetString();
}



// Seeking
////////////

int decDiskFileWriter::GetPosition(){
	return ( int )ftell( pFile );
}

void decDiskFileWriter::SetPosition( int position ){
	if( fseek( pFile, position, SEEK_SET ) ){
		DETHROW( deeReadFile );
	}
}

void decDiskFileWriter::MovePosition( int offset ){
	if( fseek( pFile, offset, SEEK_CUR ) ){
		DETHROW( deeReadFile );
	}
}

void decDiskFileWriter::SetPositionEnd( int position ){
	if( fseek( pFile, position, SEEK_END ) ){
		DETHROW( deeReadFile );
	}
}



// Reading
////////////

void decDiskFileWriter::Write( const void *buffer, int size ){
	if( size == 0 ){
		return;
	}
	
	if( fwrite( buffer, size, 1, pFile ) != 1 ){
		DETHROW( deeWriteFile );
	}
}
