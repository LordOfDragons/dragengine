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

#include <stdlib.h>
#include <string.h>
#include "dePatternList.h"
#include "../common/exceptions.h"



// Class dePatternList
////////////////////////

// Constructor, destructor
////////////////////////////

dePatternList::dePatternList(){
}

dePatternList::~dePatternList(){
}




// Management
///////////////

int dePatternList::GetPatternCount() const {
	return pPatterns.GetCount();
}

const decString &dePatternList::GetPatternAt( int index ) const{
	return pPatterns.GetAt( index );
}

int dePatternList::IndexOfPattern( const char *pattern ) const{
	return pPatterns.IndexOf( pattern );
}

bool dePatternList::HasPattern( const char *pattern ) const{
	return pPatterns.Has( pattern );
}

void dePatternList::AddPattern( const char *pattern ){
	if( pPatterns.Has( pattern ) ){
		DETHROW( deeInvalidParam );
	}
	pPatterns.Add( pattern );
}

void dePatternList::RemovePattern( const char *pattern ){
	const int index = pPatterns.IndexOf( pattern );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	pPatterns.RemoveFrom( index );
}

void dePatternList::RemoveAllPatterns(){
	pPatterns.RemoveAll();
}



void dePatternList::SortPatternByLength(){
	const int count = pPatterns.GetCount();
	int p = 1;
	
	while( p < count ){
		const int length1 = pPatterns[ p - 1 ].GetLength();
		const int length2 = pPatterns[ p ].GetLength();
		
		if( length2 > length1 ){
			pPatterns.Move( p - 1, p );
			
			if( p > 1 ){
				p--;
				continue;
			}
		}
		
		p++;
	}
}

bool dePatternList::PathMatches( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}

	const int pathLength = ( int )strlen( path );
	const int count = pPatterns.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		const int patternLength = pPatterns[ p ].GetLength();
		
		if( patternLength < pathLength ){
			if( strcmp( path + ( pathLength - patternLength ), pPatterns[ p ].GetString() ) == 0 ){
				return true;
			}
		}
	}
	
	return false;
}

int dePatternList::IndexOfPatternMatchingPath( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}

	const int pathLength = ( int )strlen( path );
	const int count = pPatterns.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		const int patternLength = pPatterns[ p ].GetLength();
		
		if( patternLength < pathLength ){
			if( strcmp( path + ( pathLength - patternLength ), pPatterns[ p ] ) == 0 ){
				return p;
			}
		}
	}
	
	return -1;
}
