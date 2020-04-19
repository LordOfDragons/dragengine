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

#include "deLoggerConsoleColor.h"
#include "../common/exceptions.h"

#ifdef ANDROID
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Dragengine", __VA_ARGS__);
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
pErrorColorBackground( etcBlack ){
}

deLoggerConsoleColor::~deLoggerConsoleColor(){
}



// Management
///////////////

deLoggerConsoleColor::eTextAttributes deLoggerConsoleColor::GetInfoAttribute(){
	eTextAttributes attribute;
	pMutex.Lock();
	attribute = pInfoAttribute;
	pMutex.Unlock();
	return attribute;
}

void deLoggerConsoleColor::SetInfoAttribute( eTextAttributes attribute ){
	if( attribute < etaDefault || attribute > etaBlink ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	pInfoAttribute = attribute;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetInfoColorForeground(){
	eTextColors color;
	pMutex.Lock();
	color = pInfoColorForeground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetInfoColorForeground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	pInfoColorForeground = color;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetInfoColorBackground(){
	eTextColors color;
	pMutex.Lock();
	color = pInfoColorBackground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetInfoColorBackground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	pInfoColorBackground = color;
	pMutex.Unlock();
}



deLoggerConsoleColor::eTextAttributes deLoggerConsoleColor::GetWarnAttribute(){
	eTextAttributes attribute;
	pMutex.Lock();
	attribute = pWarnAttribute;
	pMutex.Unlock();
	return attribute;
}

void deLoggerConsoleColor::SetWarnAttribute( eTextAttributes attribute ){
	if( attribute < etaDefault || attribute > etaBlink ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	pWarnAttribute = attribute;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetWarnColorForeground(){
	eTextColors color;
	pMutex.Lock();
	color = pWarnColorForeground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetWarnColorForeground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	pWarnColorForeground = color;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetWarnColorBackground(){
	eTextColors color;
	pMutex.Lock();
	color = pWarnColorBackground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetWarnColorBackground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	pWarnColorBackground = color;
	pMutex.Unlock();
}



deLoggerConsoleColor::eTextAttributes deLoggerConsoleColor::GetErrorAttribute(){
	eTextAttributes attribute;
	pMutex.Lock();
	attribute = pErrorAttribute;
	pMutex.Unlock();
	return attribute;
}

void deLoggerConsoleColor::SetErrorAttribute( eTextAttributes attribute ){
	if( attribute < etaDefault || attribute > etaBlink ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	pErrorAttribute = attribute;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetErrorColorForeground(){
	eTextColors color;
	pMutex.Lock();
	color = pErrorColorForeground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetErrorColorForeground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	pErrorColorForeground = color;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetErrorColorBackground(){
	eTextColors color;
	pMutex.Lock();
	color = pErrorColorBackground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetErrorColorBackground( eTextColors color ){
	if( color < etcDefault || color > etcWhite ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	pErrorColorBackground = color;
	pMutex.Unlock();
}



void deLoggerConsoleColor::LogInfo( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen( message );
		
		if( len > 0 && message[ len - 1 ] == '\n' ){
			pPrintMessage( source, message, pInfoAttribute, pInfoColorForeground, pInfoColorBackground, 0 );
			
		}else{
			pPrintMessage( source, message, pInfoAttribute, pInfoColorForeground, pInfoColorBackground, '\n' );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerConsoleColor::LogWarn( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen( message );
		
		if( len > 0 && message[ len - 1 ] == '\n' ){
			pPrintMessage( source, message, pWarnAttribute, pWarnColorForeground, pWarnColorBackground, 0 );
			
		}else{
			pPrintMessage( source, message, pWarnAttribute, pWarnColorForeground, pWarnColorBackground, '\n' );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerConsoleColor::LogError( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen( message );
		
		if( len > 0 && message[ len - 1 ] == '\n' ){
			pPrintMessage( source, message, pErrorAttribute, pErrorColorForeground, pErrorColorBackground, 0 );
			
		}else{
			pPrintMessage( source, message, pErrorAttribute, pErrorColorForeground, pErrorColorBackground, '\n' );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}



// Protected Functions
////////////////////////

void deLoggerConsoleColor::pPrintMessage( const char *source, const char *message,
eTextAttributes attribute, eTextColors colorForeground, eTextColors colorBackground,
char lineEnd ) const {
	//if( attribute == etaDefault && colorForeground == etcWhite && colorBackground == etcBlack ){
	if( attribute == etaDefault && colorForeground == etcDefault && colorBackground == etcDefault ){
		printf( "[%s] %s%c", source, message, lineEnd );
		
	}else{
		printf( "\033[%d;%d;%dm[%s] %s\33[0;0;0m%c", attribute,
			vColorTableForeground[ colorForeground ],
			vColorTableBackground[ colorBackground ],
			source, message, lineEnd );
	}
}
