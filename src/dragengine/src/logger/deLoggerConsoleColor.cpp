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

#include "../dragengine_configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "deLoggerConsoleColor.h"
#include "../common/exceptions.h"
#include "../common/utils/decDateTime.h"
#include "../threading/deMutexGuard.h"

#ifdef ANDROID
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Dragengine", __VA_ARGS__);
#endif

#ifdef OS_W32
// some toolchains do not define this as they should
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif



// Defines
////////////

/*
#define ATTR_RESET		0
#define ATTR_BRIGHT		1
#define ATTR_DIM		2
#define ATTR_UNDERLINE 	3
#define ATTR_BLINK		4
#define ATTR_REVERSE	7
#define ATTR_HIDDEN		8

#define FG_BLACK 		30
#define FG_RED			31
#define FG_GREEN		32
#define FG_YELLOW		33
#define FG_BLUE			34
#define FG_MAGENTA		35
#define FG_CYAN			36
#define	FG_WHITE		37

#define BG_BLACK 		40
#define BG_RED			41
#define BG_GREEN		42
#define BG_YELLOW		43
#define BG_BLUE			44
#define BG_MAGENTA		45
#define BG_CYAN			46
#define	BG_WHITE		47
*/

static const int vColorTableForeground[] = {
	0,  // etcDefault
	30, // etcBlack
	31, // etcRed
	32, // etcGreen
	33, // etcYellow
	34, // etcBlue
	35, // etcMagenta
	36, // etcCyan
	37  // etcWhite
};

static const int vColorTableBackground[] = {
	0,  // etcDefault
	40, // etcBlack
	41, // etcRed
	42, // etcGreen
	43, // etcYellow
	44, // etcBlue
	45, // etcMagenta
	46, // etcCyan
	47  // etcWhite
};



// Class deLoggerConsoleColor
///////////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerConsoleColor::deLoggerConsoleColor() :
pInfoAttribute( etaDefault ),
pInfoColorForeground( etcDefault ), //( etcWhite ),
pInfoColorBackground( etcDefault ), //( etcBlack ),

pWarnAttribute( etaBright ),
pWarnColorForeground( etcYellow ),
pWarnColorBackground( etcBlack ),

pErrorAttribute( etaBright ),
pErrorColorForeground( etcRed ),
pErrorColorBackground( etcBlack )
{
#ifdef OS_W32
	// set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if( hOut != INVALID_HANDLE_VALUE ){
		DWORD dwMode = 0;
		if( GetConsoleMode( hOut, &dwMode ) ){
			SetConsoleMode( hOut, dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING );
		}
	}
#endif
}

deLoggerConsoleColor::~deLoggerConsoleColor(){
}



// Management
///////////////

deLoggerConsoleColor::eTextAttributes deLoggerConsoleColor::GetInfoAttribute(){
	const deMutexGuard lock( pMutex );
	return pInfoAttribute;
}

void deLoggerConsoleColor::SetInfoAttribute( eTextAttributes attribute ){
	if( attribute < etaDefault || attribute > etaBlink ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard lock( pMutex );
	pInfoAttribute = attribute;
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetInfoColorForeground(){
	const deMutexGuard lock( pMutex );
	return pInfoColorForeground;
}

void deLoggerConsoleColor::SetInfoColorForeground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard lock( pMutex );
	pInfoColorForeground = color;
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetInfoColorBackground(){
	const deMutexGuard lock( pMutex );
	return pInfoColorBackground;
}

void deLoggerConsoleColor::SetInfoColorBackground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard lock( pMutex );
	pInfoColorBackground = color;
}



deLoggerConsoleColor::eTextAttributes deLoggerConsoleColor::GetWarnAttribute(){
	const deMutexGuard lock( pMutex );
	return pWarnAttribute;
}

void deLoggerConsoleColor::SetWarnAttribute( eTextAttributes attribute ){
	if( attribute < etaDefault || attribute > etaBlink ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard lock( pMutex );
	pWarnAttribute = attribute;
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetWarnColorForeground(){
	const deMutexGuard lock( pMutex );
	return pWarnColorForeground;
}

void deLoggerConsoleColor::SetWarnColorForeground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard lock( pMutex );
	pWarnColorForeground = color;
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetWarnColorBackground(){
	const deMutexGuard lock( pMutex );
	return pWarnColorBackground;
}

void deLoggerConsoleColor::SetWarnColorBackground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard lock( pMutex );
	pWarnColorBackground = color;
}



deLoggerConsoleColor::eTextAttributes deLoggerConsoleColor::GetErrorAttribute(){
	const deMutexGuard lock( pMutex );
	return pErrorAttribute;
}

void deLoggerConsoleColor::SetErrorAttribute( eTextAttributes attribute ){
	if( attribute < etaDefault || attribute > etaBlink ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard lock( pMutex );
	pErrorAttribute = attribute;
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetErrorColorForeground(){
	const deMutexGuard lock( pMutex );
	return pErrorColorForeground;
}

void deLoggerConsoleColor::SetErrorColorForeground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard lock( pMutex );
	pErrorColorForeground = color;
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetErrorColorBackground(){
	const deMutexGuard lock( pMutex );
	return pErrorColorBackground;
}

void deLoggerConsoleColor::SetErrorColorBackground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard lock( pMutex );
	pErrorColorBackground = color;
}



void deLoggerConsoleColor::LogInfo( const char *source, const char *message ){
	LogMessage( source, message, pInfoAttribute, pInfoColorForeground, pInfoColorBackground );
}

void deLoggerConsoleColor::LogWarn( const char *source, const char *message ){
	LogMessage( source, message, pWarnAttribute, pWarnColorForeground, pWarnColorBackground );
}

void deLoggerConsoleColor::LogError( const char *source, const char *message ){
	LogMessage( source, message, pErrorAttribute, pErrorColorForeground, pErrorColorBackground );
}



// Protected Functions
////////////////////////

void deLoggerConsoleColor::pPrintMessage( const char *source, const char *message,
eTextAttributes attribute, eTextColors colorForeground, eTextColors colorBackground,
char lineEnd ) const {
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	const decDateTime timestamp;
	
	//if( attribute == etaDefault && colorForeground == etcWhite && colorBackground == etcBlack ){
	if( attribute == etaDefault && colorForeground == etcDefault && colorBackground == etcDefault ){
		printf( "[%s] [%4d-%02d-%02d %02d:%02d:%02d] %s%c", source,
			timestamp.GetYear(), timestamp.GetMonth() + 1, timestamp.GetDay() + 1,
			timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond(), message, lineEnd );
		
	}else{
		printf( "\033[%d;%d;%dm[%s] [%4d-%02d-%02d %02d:%02d:%02d] %s\33[0;0;0m%c", attribute,
			vColorTableForeground[ colorForeground ], vColorTableBackground[ colorBackground ],
			source, timestamp.GetYear(), timestamp.GetMonth() + 1, timestamp.GetDay() + 1,
			timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond(), message, lineEnd );
	}
}

void deLoggerConsoleColor::LogMessage( const char *source, const char *message,
eTextAttributes attribute, eTextColors colorForeground, eTextColors colorBackground ){
	const int len = ( int )strlen( message );
	const deMutexGuard lock( pMutex );
	
	pPrintMessage( source, message, attribute, colorForeground, colorBackground,
		( len == 0 || message[ len - 1 ] != '\n' ) ? '\n' : 0 );
}
