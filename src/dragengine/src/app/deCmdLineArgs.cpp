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
