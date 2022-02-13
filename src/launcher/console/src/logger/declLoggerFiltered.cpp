/* 
 * Drag[en]gine Console Launcher
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

#include "declLoggerFiltered.h"

#include <dragengine/common/exceptions.h>



// Class declLoggerFiltered
/////////////////////////////

// Constructor, destructor
////////////////////////////

declLoggerFiltered::declLoggerFiltered(){
}

declLoggerFiltered::~declLoggerFiltered(){
}



// Management
///////////////

void declLoggerFiltered::SetLoggerInfo( deLogger *logger ){
	pLoggerInfo = logger;
}

void declLoggerFiltered::SetLoggerWarning( deLogger *logger ){
	pLoggerWarn = logger;
}

void declLoggerFiltered::SetLoggerError( deLogger *logger ){
	pLoggerError = logger;
}



void declLoggerFiltered::LogInfo( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLoggerInfo ){
		pLoggerInfo->LogInfo( source, message );
	}
}

void declLoggerFiltered::LogInfoFormat( const char *source, const char *message, ... ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLoggerInfo ){
		va_list list;
		va_start( list, message );
		pLoggerInfo->LogInfoFormatUsing( source, message, list );
		va_end( list );
	}
}

void declLoggerFiltered::LogInfoFormatUsing( const char *source, const char *message, va_list args ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLoggerInfo ){
		pLoggerInfo->LogInfoFormatUsing( source, message, args );
	}
}



void declLoggerFiltered::LogWarn( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLoggerWarn ){
		pLoggerWarn->LogWarn( source, message );
	}
}

void declLoggerFiltered::LogWarnFormat( const char *source, const char *message, ... ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLoggerWarn ){
		va_list list;
		va_start( list, message );
		pLoggerWarn->LogWarnFormatUsing( source, message, list );
		va_end( list );
	}
}

void declLoggerFiltered::LogWarnFormatUsing( const char *source, const char *message, va_list args ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLoggerWarn ){
		pLoggerWarn->LogWarnFormatUsing( source, message, args );
	}
}



void declLoggerFiltered::LogError( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLoggerError ){
		pLoggerError->LogError( source, message );
	}
}

void declLoggerFiltered::LogErrorFormat( const char *source, const char *message, ... ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLoggerError ){
		va_list list;
		va_start( list, message );
		pLoggerError->LogErrorFormatUsing( source, message, list );
		va_end( list );
	}
}

void declLoggerFiltered::LogErrorFormatUsing( const char *source, const char *message, va_list args ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLoggerError ){
		pLoggerError->LogErrorFormatUsing( source, message, args );
	}
}

void declLoggerFiltered::LogException( const char *source, const deException &exception ){
	const decStringList output( exception.FormatOutput() );
	const int count = output.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		LogError( source, output.GetAt( i ) );
	}
}
