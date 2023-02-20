/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <dragengine/dragengine_configuration.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef OS_UNIX
#include <unistd.h>
#endif

#include "delLoggerWritePipe.h"
#include "../engine/delEngineProcess.h"

#include <dragengine/common/exceptions.h>



// Class delLoggerWritePipe
/////////////////////////////

// Constructor, destructor
////////////////////////////

#ifdef OS_W32
delLoggerWritePipe::delLoggerWritePipe( HANDLE pipe ) :
pPipe( pipe )
#else
delLoggerWritePipe::delLoggerWritePipe( int pipe ) :
pPipe( pipe )
#endif
{
}

delLoggerWritePipe::~delLoggerWritePipe(){
}



// Management
///////////////

void delLoggerWritePipe::LogInfo( const char *source, const char *message ){
	if( ! source ){
		DETHROW_INFO( deeNullPointer, "source" );
	}
	if( ! message ){
		DETHROW_INFO( deeNullPointer, "message" );
	}
	
	LogToPipe( source, message, delEngineProcess::eltInfo );
}

void delLoggerWritePipe::LogWarn( const char *source, const char *message ){
	if( ! source ){
		DETHROW_INFO( deeNullPointer, "source" );
	}
	if( ! message ){
		DETHROW_INFO( deeNullPointer, "message" );
	}
	
	LogToPipe( source, message, delEngineProcess::eltWarn );
}

void delLoggerWritePipe::LogError( const char *source, const char *message ){
	if( ! source ){
		DETHROW_INFO( deeNullPointer, "source" );
	}
	if( ! message ){
		DETHROW_INFO( deeNullPointer, "message" );
	}
	
	LogToPipe( source, message, delEngineProcess::eltError );
}



// Protected Functions
////////////////////////

void delLoggerWritePipe::LogToPipe( const char *source, const char *message, int type ){
	unsigned short lenMessage = ( unsigned short )strlen( message );
	unsigned short lenSource = ( unsigned short )strlen( source );
	unsigned char wtype = ( unsigned char )type;
	char *data = NULL;
	int dataLen = 0;
	char *dataPtr;
	
	// calculate the length of the required data buffer
	dataLen += sizeof( wtype );
	dataLen += sizeof( lenSource );
	dataLen += lenSource;
	dataLen += sizeof( lenMessage );
	dataLen += lenMessage;
	
	try{
		// create data buffer with the appropriate data
		data = new char[ dataLen ];
		dataPtr = data;
		
		memcpy( dataPtr, &wtype, sizeof( wtype ) );
		dataPtr += sizeof( wtype );
		
		memcpy( dataPtr, &lenSource, sizeof( lenSource ) );
		dataPtr += sizeof( lenSource );
		memcpy( dataPtr, source, lenSource );
		dataPtr += lenSource;
		
		memcpy( dataPtr, &lenMessage, sizeof( lenMessage ) );
		dataPtr += sizeof( lenMessage );
		memcpy( dataPtr, message, lenMessage );
		dataPtr += lenMessage;
		
		// send data to pipe
		WriteToPipe( data, dataLen );
		
		// clean up
		delete [] data;
		
	}catch( const deException & ){
		if( data ){
			delete [] data;
		}
		throw;
	}
}

void delLoggerWritePipe::WriteToPipe( const void *data, int length ){
#ifdef OS_W32
	DWORD bytesWritten = 0;
	
	if( ! WriteFile( pPipe, data, length, &bytesWritten, NULL ) ){
		DETHROW( deeInvalidAction );
	}
	if( ( int )bytesWritten < length ){
		DETHROW( deeInvalidAction );
	}
	
#else
	if( write( pPipe, data, length ) < length ){
		DETHROW( deeInvalidAction );
	}
#endif
}
