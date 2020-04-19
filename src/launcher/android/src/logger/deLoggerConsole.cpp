/* 
 * Drag[en]gine Android Launcher
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

#include "../dragengine_configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "deLoggerConsole.h"
#include "../common/exceptions.h"

#ifdef ANDROID
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Dragengine", __VA_ARGS__);
#endif



// Class deLoggerConsole
//////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerConsole::deLoggerConsole(){
}

deLoggerConsole::~deLoggerConsole(){
}



// Management
///////////////

void deLoggerConsole::LogInfo( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen( message );
		
		if( len > 0 && message[ len - 1 ] == '\n' ){
			printf( "II [%s] %s", source, message );
			
		}else{
			printf( "II [%s] %s\n", source, message );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerConsole::LogWarn( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen( message );
		
		if( len > 0 && message[ len - 1 ] == '\n' ){
			printf( "WW [%s] %s", source, message );
			
		}else{
			printf( "WW [%s] %s\n", source, message );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerConsole::LogError( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen( message );
		
		if( len > 0 && message[ len - 1 ] == '\n' ){
			printf( "EE [%s] %s", source, message );
			
		}else{
			printf( "EE [%s] %s\n", source, message );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}
