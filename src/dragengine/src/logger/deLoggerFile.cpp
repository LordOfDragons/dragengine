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
#include <stdarg.h>

#include "deLoggerFile.h"
#include "../common/exceptions.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/string/decString.h"
#include "../common/utils/decDateTime.h"
#include "../threading/deMutexGuard.h"



// Class deLoggerFile
///////////////////////

// Constructor, destructor
////////////////////////////

deLoggerFile::deLoggerFile( decBaseFileWriter *writer ) :
pWriter( NULL )
{
	if( ! writer ){
		DETHROW( deeInvalidParam );
	}
	
	pWriter = writer;
	writer->AddReference();
}

deLoggerFile::~deLoggerFile(){
	if( pWriter ){
		pWriter->FreeReference();
	}
}



// Management
///////////////

void deLoggerFile::LogInfo( const char *source, const char *message ){
	LogPrefix( source, message, "II " );
}

void deLoggerFile::LogWarn( const char *source, const char *message ){
	LogPrefix( source, message, "WW " );
}

void deLoggerFile::LogError( const char *source, const char *message ){
	LogPrefix( source, message, "EE " );
}

void deLoggerFile::LogPrefix( const char *source, const char *message, const char *prefix ){
	if( ! source || ! message || ! prefix ){
		DETHROW( deeInvalidParam );
	}
	
	const int len =( int )strlen( message );
	const decDateTime timestamp;
	decString string;
	
	string.Format( "%s[%s] [%4d-%02d-%02d %02d:%02d:%02d] %s%s", prefix, source,
		timestamp.GetYear(), timestamp.GetMonth() + 1, timestamp.GetDay() + 1,
		timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond(),
		message, ( len == 0 || message[ len - 1 ] != '\n' ) ? "\n" : "" );
	
	const deMutexGuard lock( pMutex );
	
	pWriter->Write( string.GetString(), string.GetLength() );
	fflush( NULL );
}
