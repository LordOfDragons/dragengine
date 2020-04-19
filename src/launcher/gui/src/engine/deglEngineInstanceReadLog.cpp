/* 
 * Drag[en]gine GUI Launcher
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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "deglEngineProcess.h"
#include "deglEngineInstance.h"
#include "deglEngineInstanceReadLog.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Class deglEngineInstanceReadLog
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deglEngineInstanceReadLog::deglEngineInstanceReadLog( deglEngineInstance *engineInstance, deLogger *logger ){
	if( ! engineInstance || ! logger ) DETHROW( deeInvalidParam );
	
	pEngineInstance = engineInstance;
	
	pBufferSource = NULL;
	pBufferSourceLen = 0;
	pBufferMessage = NULL;
	pBufferMessageLen = 0;
	
	pLogger = logger;
	logger->AddReference();
}

deglEngineInstanceReadLog::~deglEngineInstanceReadLog(){
	if( pBufferMessage ) free( pBufferMessage );
	if( pBufferSource ) free( pBufferSource );
	
	if( pLogger ) pLogger->FreeReference();
}



// Management
///////////////

void deglEngineInstanceReadLog::Run(){
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
		
		if( type == deglEngineProcess::eltInfo ){
			pLogger->LogInfo( pBufferSource, pBufferMessage );
			
		}else if( type == deglEngineProcess::eltWarn ){
			pLogger->LogWarn( pBufferSource, pBufferMessage );
			
		}else if( type == deglEngineProcess::eltError ){
			pLogger->LogError( pBufferSource, pBufferMessage );
		}
	}
}



bool deglEngineInstanceReadLog::ReadFromPipe( void *data, int length ){
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



void deglEngineInstanceReadLog::PrepareBufferSource( int requiredLength ){
	if( requiredLength > pBufferSourceLen ){
		pBufferSource = ( char* )realloc( pBufferSource, requiredLength + 1 );
		if( ! pBufferSource ) DETHROW( deeOutOfMemory );
		
		pBufferSourceLen = requiredLength;
	}
}

void deglEngineInstanceReadLog::PrepareBufferMessage( int requiredLength ){
	if( requiredLength > pBufferMessageLen ){
		pBufferMessage = ( char* )realloc( pBufferMessage, requiredLength + 1 );
		if( ! pBufferMessage ) DETHROW( deeOutOfMemory );
		
		pBufferMessageLen = requiredLength;
	}
}
