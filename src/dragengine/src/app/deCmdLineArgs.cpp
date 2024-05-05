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

#include "deCmdLineArgs.h"
#include "../common/exceptions.h"


// Class deCmdLineArgs
////////////////////////

// Constructor, destructor
////////////////////////////

deCmdLineArgs::deCmdLineArgs(){
}

deCmdLineArgs::deCmdLineArgs( const deCmdLineArgs &copy ) :
pArguments( copy.pArguments ){
}

deCmdLineArgs::~deCmdLineArgs(){
}



// Management
///////////////

int deCmdLineArgs::GetCount() const{
	return pArguments.GetCount();
}

const decString &deCmdLineArgs::GetArgument( int index ) const{
	return pArguments.GetAt( index );
}

int deCmdLineArgs::IndexOfArgument( const char *argument ) const{
	return pArguments.IndexOf( argument );
}

bool deCmdLineArgs::HasArgument( const char *argument ) const{
	return pArguments.Has( argument );
}

void deCmdLineArgs::AddArgument( const char *argument ){
	pArguments.Add( argument );
}

void deCmdLineArgs::RemoveArgument( int index ){
	pArguments.RemoveFrom( index );
}

void deCmdLineArgs::RemoveAllArguments(){
	pArguments.RemoveAll();
}



void deCmdLineArgs::AddArgsSplit( const char *commandLine ){
	const decString scmdline( commandLine );
	const int len = scmdline.GetLength();
	int cur = 0;
	int start = 0;
	int next;
	
	while( start < len ){
		// skip spaces
		while( start < len && isspace( scmdline.GetAt( start ) ) ){
			start++;
		}
		
		// if we are at the end the rest of the argument line is just space
		if( start == len ){
			break;
		}
		
		// parse string
		if( scmdline.GetAt( start ) == '"' ){
			start++;
			cur = start;
			while( cur < len && scmdline.GetAt( cur ) != '"' ){
				cur++;
			}
			next = cur + 1;
			
		// parse non-string
		}else{
			cur = start;
			while( cur < len && ! isspace( scmdline.GetAt( cur ) ) ){
				cur++;
			}
			next = cur;
		}
		
		// add argument if not empty
		if( cur - start > 0 ){
			pArguments.Add( scmdline.GetMiddle( start, cur ) );
		}
		
		// next round
		start = next;
	}
}
