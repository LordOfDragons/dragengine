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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "delEngineProcess.h"
#include "delEngineInstance.h"
#include "delEngineInstanceReadLog.h"

#include <dragengine/common/exceptions.h>



// Class delEngineInstanceReadLog
////////////////////////////////////

// Constructor, destructor
////////////////////////////

delEngineInstanceReadLog::delEngineInstanceReadLog( delEngineInstance &engineInstance, deLogger *logger ) :
pEngineInstance( engineInstance ),
pLogger( logger ),
pBufferSource( nullptr ),
pBufferSourceLen( 0 ),
pBufferMessage( nullptr ),
pBufferMessageLen( 0 )
{
	if( ! logger ){
		DETHROW_INFO( deeNullPointer, "logger" );
	}
}

delEngineInstanceReadLog::~delEngineInstanceReadLog(){
	if( pBufferMessage ){
		free( pBufferMessage );
	}
	if( pBufferSource ){
		free( pBufferSource );
	}
}



// Management
///////////////

void delEngineInstanceReadLog::Run(){
	unsigned short length;
	unsigned char type;
	
	while( ReadFromPipe( &type, sizeof( type ) ) ){
		ReadFromPipe( &length, sizeof( length ) );
		PrepareBufferSource( length );
		ReadFromPipe( pBufferSource, length );
		pBufferSource[ length ] = '\0';
		
		ReadFromPipe( &length, sizeof( length ) );
		PrepareBufferMessage( length );
		ReadFromPipe( pBufferMessage, length );
		pBufferMessage[ length ] = '\0';
		
		if( type == delEngineProcess::eltInfo ){
			pLogger->LogInfo( pBufferSource, pBufferMessage );
			
		}else if( type == delEngineProcess::eltWarn ){
			pLogger->LogWarn( pBufferSource, pBufferMessage );
			
		}else if( type == delEngineProcess::eltError ){
			pLogger->LogError( pBufferSource, pBufferMessage );
		}
	}
}



bool delEngineInstanceReadLog::ReadFromPipe( void *data, int length ){
	/*
#ifdef OS_W32
	DWORD bytesRead = 0;
	
	return ReadFile( pEngineInstance->GetPipeLog(), data, length, &bytesRead, NULL ) && ( int )bytesRead == length;
	
#else
	return read( pEngineInstance->GetPipeLog(), data, length ) == length;
#endif
	*/
	return false;
}



void delEngineInstanceReadLog::PrepareBufferSource( int requiredLength ){
	if( requiredLength <= pBufferSourceLen ){
		return;
	}
	
	pBufferSource = ( char* )realloc( pBufferSource, requiredLength + 1 );
	if( ! pBufferSource ){
		DETHROW( deeOutOfMemory );
	}
	
	pBufferSourceLen = requiredLength;
}

void delEngineInstanceReadLog::PrepareBufferMessage( int requiredLength ){
	if( requiredLength <= pBufferMessageLen ){
		return;
	}
	
	pBufferMessage = ( char* )realloc( pBufferMessage, requiredLength + 1 );
	if( ! pBufferMessage ){
		DETHROW( deeOutOfMemory );
	}
	
	pBufferMessageLen = requiredLength;
}
