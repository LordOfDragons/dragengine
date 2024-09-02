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
