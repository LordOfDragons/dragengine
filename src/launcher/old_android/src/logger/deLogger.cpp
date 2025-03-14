/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
