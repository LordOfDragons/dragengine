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
#include <ctype.h>

#include "decUnicodeArgumentList.h"
#include "decUnicodeString.h"
#include "../../exceptions.h"



// class decUnicodeArgumentList
/////////////////////////////////

// constructor, destructor
////////////////////////////

decUnicodeArgumentList::decUnicodeArgumentList(){
	pArguments = NULL;
	pArgumentCount = 0;
	pArgumentSize = 0;
}

decUnicodeArgumentList::~decUnicodeArgumentList(){
	RemoveAllArguments();
	if( pArguments ) delete [] pArguments;
}



// management
///////////////

const decUnicodeString *decUnicodeArgumentList::GetArgumentAt( int index ) const{
	if( index < 0 || index >= pArgumentCount ) DETHROW( deeOutOfBoundary );
	return ( const decUnicodeString * )pArguments[ index ];
}

bool decUnicodeArgumentList::MatchesArgumentAt( int index, const char *string ) const{
	if( index < 0 || index >= pArgumentCount ) DETHROW( deeOutOfBoundary );
	if( ! string ) DETHROW( deeInvalidParam );
	decUnicodeString ustring;
	ustring.SetFromUTF8( string );
	return pArguments[ index ]->Compare( ustring ) == 0;
}

void decUnicodeArgumentList::AddArgument( const decUnicodeString &argument ){
	if( pArgumentCount == pArgumentSize ){
		int i, newSize = pArgumentSize * 3 / 2 + 1;
		decUnicodeString **newArray = new decUnicodeString*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pArguments ){
			for( i=0; i<pArgumentSize; i++ ) newArray[ i ] = pArguments[ i ];
			delete [] pArguments;
		}
		pArguments = newArray;
		pArgumentSize = newSize;
	}
	pArguments[ pArgumentCount ] = new decUnicodeString( argument );
	if( ! pArguments[ pArgumentCount ] ) DETHROW( deeOutOfMemory );
	pArgumentCount++;
}

void decUnicodeArgumentList::RemoveAllArguments(){
	while( pArgumentCount > 0 ){
		delete pArguments[ pArgumentCount - 1 ];
		pArgumentCount--;
	}
}

void decUnicodeArgumentList::ParseCommand( const decUnicodeString &command ){
	int cur = 0, start = 0, next, len = command.GetLength();
	decUnicodeString buffer;
	while( start < len ){
		// skip spaces
		while( start < len && isspace( command[ start ] ) ) start++;
		if( start == len ) break;
		// parse string
		if( command[ start ] == '"' ){
			start++;
			cur = start;
			while( cur < len && command[ cur ] != '"' ) cur++;
			next = cur + 1;
		// parse non-string
		}else{
			cur = start;
			while( cur < len && ! isspace( command[ cur ] ) ) cur++;
			next = cur;
		}
		// add argument if not empty
		if( cur - start > 0 ){
			buffer = command.GetMiddle( start, cur );
			AddArgument( buffer );
		}
		// next round
		start = next;
	}
}
