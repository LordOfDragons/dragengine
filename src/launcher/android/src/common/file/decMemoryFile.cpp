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
