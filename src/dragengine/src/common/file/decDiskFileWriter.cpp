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

#include "decDiskFileWriter.h"
#include "../exceptions.h"

#ifdef OS_W32
#include "../../app/deOSWindows.h"
#endif



// Class decDiskFileWriter
///////////////////////////

// Constructor, destructor
////////////////////////////

decDiskFileWriter::decDiskFileWriter( const char *filename, bool append ) :
pFilename( filename ),
pFile( NULL )
{
#ifdef OS_W32
	wchar_t widePath[ MAX_PATH ];
	deOSWindows::Utf8ToWide( filename, widePath, MAX_PATH );
	pFile = _wfopen( widePath, append ? L"ab" : L"wb" );
	
#else
	pFile = fopen( filename, append ? "ab" : "wb" );
#endif
	if( ! pFile ){
		DETHROW_INFO( deeFileNotFound, filename );
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
		DETHROW_INFO( deeReadFile, pFilename );
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
		DETHROW_INFO( deeWriteFile, pFilename );
	}
}
