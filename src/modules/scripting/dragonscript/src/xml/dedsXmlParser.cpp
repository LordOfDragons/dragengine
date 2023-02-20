/* 
 * Drag[en]gine DragonScript Script Module
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

// includes
#include <libdscript/libdscript.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dedsXmlParser.h"
#include <dragengine/logger/deLogger.h>
#include <libdscript/exceptions.h>



// class dedsXmlParser
////////////////////////

// constructor, destructor
////////////////////////////

dedsXmlParser::dedsXmlParser( deLogger *logger ) : decXmlParser( logger ){
	pLog = new char[ 1 ];
	pLog[ 0 ] = '\0';
	pLogLen = 0;
}

dedsXmlParser::~dedsXmlParser(){
	if( pLog ){
		delete [] pLog;
	}
}



// management
///////////////

void dedsXmlParser::UnexpectedEOF( int line, int pos ){
	const int newline = pLogLen == 0 ? 0 : 1;
	char * const newLog = new char[ pLogLen + newline + 29 ];

	if( pLogLen > 0 ){
		#ifdef OS_W32_VS
			strncpy_s( newLog, pLogLen + 1, pLog, pLogLen );
		#else
			strncpy( newLog, pLog, pLogLen + 1 );
		#endif
		newLog[ pLogLen ] = '\n';
	}

	const char * const message = "Unexpected end-of-file found";

	#ifdef OS_W32_VS
		strcpy_s( newLog + pLogLen + newline, strlen( message ) + 1, message );
	#else
		strcpy( newLog + pLogLen + newline, message );
	#endif

	delete [] pLog;
	pLog = newLog;
	pLogLen += newline + 28;
}

void dedsXmlParser::UnexpectedToken( int line, int pos, const char *token ){
	const int newline = pLogLen == 0 ? 0 : 1;
	const int newLen = 50 + ( int )strlen( token );
	char * const newLog = new char[ pLogLen + newline + newLen ];

	if( pLogLen > 0 ){
		#ifdef OS_W32_VS
			strncpy_s( newLog, pLogLen + 1, pLog, pLogLen );
		#else
			strncpy( newLog, pLog, pLogLen + 1 );
		#endif
		newLog[ pLogLen ] = '\n';
	}
	snprintf( newLog + pLogLen + newline, newLen, "Unexpected token '%s' found at %i:%i", token, line, pos );
	delete [] pLog;
	pLog = newLog;
	pLogLen += newline + newLen;
}
