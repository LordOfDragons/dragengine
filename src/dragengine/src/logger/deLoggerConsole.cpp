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

#include "../dragengine_configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "deLoggerConsole.h"
#include "../common/exceptions.h"
#include "../common/utils/decDateTime.h"
#include "../threading/deMutexGuard.h"

#ifdef OS_ANDROID
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
	LogPrefix( source, message, "II " );
}

void deLoggerConsole::LogWarn( const char *source, const char *message ){
	LogPrefix( source, message, "WW " );
}

void deLoggerConsole::LogError( const char *source, const char *message ){
	LogPrefix( source, message, "EE " );
}

void deLoggerConsole::LogPrefix( const char *source, const char *message, const char *prefix ){
	if( ! source || ! message || ! prefix ){
		DETHROW( deeInvalidParam );
	}
	
	const int len = ( int )strlen( message );
	const decDateTime timestamp;
	
	const deMutexGuard lock( pMutex );
	
	printf( "%s[%s] [%4d-%02d-%02d %02d:%02d:%02d] %s%s", prefix, source,
		timestamp.GetYear(), timestamp.GetMonth() + 1, timestamp.GetDay() + 1,
		timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond(),
		message, ( len == 0 || message[ len - 1 ] != '\n' ) ? "\n" : "" );
}
