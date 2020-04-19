/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalATLogger.h"
#include "deoalAudioThread.h"
#include "../deAudioOpenAL.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGGING_SOURCE "OpenAL-AT"



// Class deoalATLogger
////////////////////////

// Constructor, destructor
////////////////////////////

deoalATLogger::deoalATLogger( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ){
}

deoalATLogger::~deoalATLogger(){
}



// Management
///////////////

deLogger &deoalATLogger::GetLogger(){
	return *pAudioThread.GetOal().GetGameEngine()->GetLogger();
}

void deoalATLogger::Synchronize(){
}



void deoalATLogger::LogInfo( const char *message ){
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogInfo( LOGGING_SOURCE, message );
		
	}else{
		pAudioThread.GetOal().LogInfo( message );
	}
}

void deoalATLogger::LogInfoFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogInfoFormatUsing( LOGGING_SOURCE, message, list );
		
	}else{
		pAudioThread.GetOal().LogInfoFormatUsing( message, list );
	}
	
	va_end( list );
}

void deoalATLogger::LogInfoFormatUsing( const char *message, va_list args ){
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogInfoFormatUsing( LOGGING_SOURCE, message, args );
		
	}else{
		pAudioThread.GetOal().LogInfoFormatUsing( message, args );
	}
}

void deoalATLogger::LogWarn( const char *message ){
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogWarn( LOGGING_SOURCE, message );
		
	}else{
		pAudioThread.GetOal().LogWarn( message );
	}
}

void deoalATLogger::LogWarnFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogWarnFormatUsing( LOGGING_SOURCE, message, list );
		
	}else{
		pAudioThread.GetOal().LogWarnFormatUsing( message, list );
	}
	
	va_end( list );
}

void deoalATLogger::LogWarnFormatUsing( const char *message, va_list args ){
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogWarnFormatUsing( LOGGING_SOURCE, message, args );
		
	}else{
		pAudioThread.GetOal().LogWarnFormatUsing( message, args );
	}
}

void deoalATLogger::LogError( const char *message ){
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogError( LOGGING_SOURCE, message );
		
	}else{
		pAudioThread.GetOal().LogError( message );
	}
}

void deoalATLogger::LogErrorFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogErrorFormatUsing( LOGGING_SOURCE, message, list );
		
	}else{
		pAudioThread.GetOal().LogErrorFormatUsing( message, list );
	}
	
	va_end( list );
}

void deoalATLogger::LogErrorFormatUsing( const char *message, va_list args ){
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogErrorFormatUsing( LOGGING_SOURCE, message, args );
		
	}else{
		pAudioThread.GetOal().LogErrorFormatUsing( message, args );
	}
}

void deoalATLogger::LogException( const deException &exception ){
	if( pAudioThread.GetAsyncAudio() ){
		GetLogger().LogException( LOGGING_SOURCE, exception );
		
	}else{
		pAudioThread.GetOal().LogException( exception );
	}
}
