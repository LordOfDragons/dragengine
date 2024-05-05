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

#include "decMemoryFile.h"
#include "../exceptions.h"



// Class decMemoryFile
////////////////////////

// Constructor, Destructor
////////////////////////////

decMemoryFile::decMemoryFile( const char *filename ) :
pFilename( filename ),
pData( NULL ),
pSize( 0 ),
pCapacity( 0 ),
pModificationTime( decDateTime().ToSystemTime() ){
}

decMemoryFile::~decMemoryFile(){
	if( pData ){
		free( pData );
	}
}



// Management
///////////////

void decMemoryFile::SetModificationTime( TIME_SYSTEM time ){
	pModificationTime = time;
}

void decMemoryFile::Resize( int size, bool compact ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( size == pSize ){
		return;
	}
	
	if( size == 0 ){
		if( compact ){
			free( pData );
			pData = NULL;
			pCapacity = 0;
		}
		pSize = 0;
		return;
	}
	
	if( size > pCapacity ){
		const int newCapacity = compact ? size : size * 5 / 4; // 25% larger to avoid re-allocating quickly
		char * const newData = ( char* )realloc( pData, newCapacity );
		if( ! newData ){
			DETHROW( deeOutOfMemory );
		}
		pData = newData;
		pCapacity = newCapacity;
	}
	
	pSize = size;
}



void decMemoryFile::Touch(){
	pModificationTime = decDateTime::GetSystemTime();
}
