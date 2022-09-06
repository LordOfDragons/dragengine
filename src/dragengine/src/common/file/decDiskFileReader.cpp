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
#include "../../dragengine_configuration.h"

#ifdef OS_W32
#include "../../app/deOSWindows.h"
#endif



// Class decDiskFileReader
///////////////////////////

// Constructor, Destructor
////////////////////////////

decDiskFileReader::decDiskFileReader( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	pFile = NULL;
	
	
	
	try{
		pFilename = filename;
		
#ifdef OS_W32
		wchar_t widePath[ MAX_PATH ];
		deOSWindows::Utf8ToWide( filename, widePath, MAX_PATH );
		
		WIN32_FILE_ATTRIBUTE_DATA fa;
		if( ! GetFileAttributesExW( widePath, GetFileExInfoStandard, &fa ) ){
			DETHROW_INFO( deeFileNotFound, filename );
		}
		
		#ifdef OS_W32_VS
		if( _wfopen_s( &pFile, widePath, L"rb" ) ){
		#else
		pFile = _wfopen( widePath, L"rb" );
		if( ! pFile ){
		#endif
			DETHROW_INFO( deeFileNotFound, filename );
		}
		
		pLength = ( int )( ( ( uint64_t )fa.nFileSizeHigh << 32 ) + ( uint64_t )fa.nFileSizeLow );
		
		SYSTEMTIME stime;
		if( ! FileTimeToSystemTime( &fa.ftLastWriteTime, &stime ) ){
			DETHROW( deeInvalidParam );
		}
		
		decDateTime modTime;
		modTime.SetYear( stime.wYear );
		modTime.SetMonth( stime.wMonth - 1 );
		modTime.SetDay( stime.wDay - 1 );
		modTime.SetHour( stime.wHour );
		modTime.SetMinute( stime.wMinute );
		modTime.SetSecond( stime.wSecond );
		
		pModificationTime = modTime.ToSystemTime();
		
#else
		// Android silently kills an application if a file does not exist.
		// for this reason the file is first stat-ed before opened. if the
		// file does not exist this prevents a strange crash. for all other
		// platforms this order of action is fine too
		struct stat st;
		
		if( stat( filename, &st ) ){
			DETHROW_INFO( deeFileNotFound, filename );
		}
		
		pFile = fopen( filename, "rb" );
		if( ! pFile ){
			DETHROW_INFO( deeFileNotFound, filename );
		}
		
		pLength = ( int )st.st_size;
		pModificationTime = ( TIME_SYSTEM )st.st_mtime;
#endif
		
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
		DETHROW_INFO( deeReadFile, pFilename );
	}
}

void decDiskFileReader::MovePosition( int offset ){
	if( fseek( pFile, offset, SEEK_CUR ) ){
		DETHROW_INFO( deeReadFile, pFilename );
	}
}

void decDiskFileReader::SetPositionEnd( int position ){
	if( fseek( pFile, position, SEEK_END ) ){
		DETHROW_INFO( deeReadFile, pFilename );
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
		DETHROW_INFO( deeReadFile, pFilename );
	}
}

decBaseFileReader::Ref decDiskFileReader::Duplicate(){
	return decBaseFileReader::Ref::New( new decDiskFileReader( pFilename ) );
}
