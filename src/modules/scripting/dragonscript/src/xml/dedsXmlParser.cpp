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
	if( ! pLog ) DSTHROW( dueOutOfMemory );
	pLog[ 0 ] = '\0';
	pLogLen = 0;
}

dedsXmlParser::~dedsXmlParser(){
	if( pLog ) delete [] pLog;
}



// management
///////////////

void dedsXmlParser::UnexpectedEOF( int line, int pos ){
	int newline = pLogLen == 0 ? 0 : 1;
	char *newLog = NULL;
	newLog = new char[ pLogLen + newline + 28 ];
	if( ! newLog ) DSTHROW( dueOutOfMemory );
	if( pLogLen > 0 ){
		strcpy( newLog, pLog );
		newLog[ pLogLen ] = '\n';
	}
	strcpy( newLog + pLogLen + newline, "Unexpected end-of-file found" );
	delete [] pLog;
	pLog = newLog;
	pLogLen += newline + 28;
}

void dedsXmlParser::UnexpectedToken( int line, int pos, const char *token ){
	int newline = pLogLen == 0 ? 0 : 1;
	int newLen = 50 + strlen( token );
	char *newLog = NULL;
	newLog = new char[ pLogLen + newline + newLen ];
	if( ! newLog ) DSTHROW( dueOutOfMemory );
	if( pLogLen > 0 ){
		strcpy( newLog, pLog );
		newLog[ pLogLen ] = '\n';
	}
	sprintf( newLog + pLogLen + newline, "Unexpected token '%s' found at %i:%i", token, line, pos );
	delete [] pLog;
	pLog = newLog;
	pLogLen += newline + newLen;
}
