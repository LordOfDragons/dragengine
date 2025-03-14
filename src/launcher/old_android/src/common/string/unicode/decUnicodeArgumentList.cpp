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
