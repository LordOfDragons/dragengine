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

#include "deLogger.h"
#include "../common/exceptions.h"
#include "../common/string/decString.h"
#include "../common/string/decStringList.h"



// Class deLogger
///////////////////

// Constructor, destructor
////////////////////////////

deLogger::deLogger(){
}

deLogger::~deLogger(){
}



// Management
///////////////

void deLogger::LogInfo( const char *source, const char *message ){
}

void deLogger::LogInfoFormat( const char *source, const char *message, ... ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	decString string;
	va_list list;
	
	va_start( list, message );
	string.FormatUsing( message, list );
	va_end( list );
	
	LogInfo( source, string.GetString() );
}

void deLogger::LogInfoFormatUsing( const char *source, const char *message, va_list args ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	decString string;
	string.FormatUsing( message, args );
	
	LogInfo( source, string.GetString() );
}



void deLogger::LogWarn( const char *source, const char *message ){
}

void deLogger::LogWarnFormat( const char *source, const char *message, ... ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	decString string;
	va_list list;
	
	va_start( list, message );
	string.FormatUsing( message, list );
	va_end( list );
	
	LogWarn( source, string.GetString() );
}

void deLogger::LogWarnFormatUsing( const char *source, const char *message, va_list args ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	decString string;
	string.FormatUsing( message, args );
	
	LogWarn( source, string.GetString() );
}



void deLogger::LogError( const char *source, const char *message ){
}

void deLogger::LogErrorFormat( const char *source, const char *message, ... ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	decString string;
	va_list list;
	
	va_start( list, message );
	string.FormatUsing( message, list );
	va_end( list );
	
	LogError( source, string.GetString() );
}

void deLogger::LogErrorFormatUsing( const char *source, const char *message, va_list args ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	decString string;
	string.FormatUsing( message, args );
	
	LogError( source, string.GetString() );
}

void deLogger::LogException( const char *source, const deException &exception ){
	const decStringList output = exception.FormatOutput();
	const int count = output.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		LogError( source, output.GetAt( i ) );
	}
}
