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
#include <sys/stat.h>

#include "decDiskFileReader.h"
#include "../exceptions.h"



// Class decDiskFileReader
///////////////////////////

// Constructor, Destructor
////////////////////////////

decDiskFileReader::decDiskFileReader( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	struct stat st;
	
	pFile = NULL;
	
	try{
		pFilename = filename;
		
		// Android silently kills an application if a file does not exist.
		// for this reason the file is first stat-ed before opened. if the
		// file does not exist this prevents a strange crash. for all other
		// platforms this order of action is fine too
		if( stat( filename, &st ) ){
			DETHROW( deeFileNotFound );
		}
		
		pFile = fopen( filename, "rb" );
		if( ! pFile ){
			DETHROW( deeFileNotFound );
		}
		
		pLength = ( int )st.st_size;
		pModificationTime = ( TIME_SYSTEM )st.st_mtime;
		
	}catch( const deException & ){
		if( pFile ){
			fclose( pFile );
		}
		throw;
	}
}

decDiskFileReader::~decDiskFileReader(){
	if( pFile ){
		fclose( pFile );
	}
}



// Management
///////////////

const char *decDiskFileReader::GetFilename(){
	return pFilename;
}

int decDiskFileReader::GetLength(){
	return pLength;
}

TIME_SYSTEM decDiskFileReader::GetModificationTime(){
	return pModificationTime;
}



// Seeking
////////////

int decDiskFileReader::GetPosition(){
	return ( int )ftell( pFile );
}

void decDiskFileReader::SetPosition( int position ){
	if( fseek( pFile, position, SEEK_SET ) ){
		DETHROW( deeReadFile );
	}
}

void decDiskFileReader::MovePosition( int offset ){
	if( fseek( pFile, offset, SEEK_CUR ) ){
		DETHROW( deeReadFile );
	}
}

void decDiskFileReader::SetPositionEnd( int position ){
	if( fseek( pFile, position, SEEK_END ) ){
		DETHROW( deeReadFile );
	}
}



// Reading
////////////

void decDiskFileReader::Read( void *buffer, int size ){
	const int readBytes = ( int )fread( buffer, 1, size, pFile );
	
	if( readBytes == size ){
		return;
	}
	
	const bool endOfFile = ( feof( pFile ) != 0 );
	clearerr( pFile );  // required to support growing files
	
	if( ! endOfFile ){
		DETHROW( deeReadFile );
	}
}
