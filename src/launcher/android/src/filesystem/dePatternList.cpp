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
	pPatterns = NULL;
	pPatternCount = 0;
	pPatternSize = 0;
}

dePatternList::~dePatternList(){
	RemoveAllPatterns();
	if( pPatterns ) delete [] pPatterns;
}




// Management
///////////////

const char *dePatternList::GetPatternAt( int index ) const{
	if( index < 0 || index >= pPatternCount ) DETHROW( deeOutOfBoundary );
	
	return ( const char * )pPatterns[ index ];
}

int dePatternList::IndexOfPattern( const char *pattern ) const{
	if( ! pattern ) DETHROW( deeInvalidParam );
	int p;
	
	for( p=0; p<pPatternCount; p++ ){
		if( strcmp( pattern, pPatterns[ p ] ) == 0 ){
			return p;
		}
	}
	
	return -1;
}

bool dePatternList::HasPattern( const char *pattern ) const{
	if( ! pattern ) DETHROW( deeInvalidParam );
	int p;
	
	for( p=0; p<pPatternCount; p++ ){
		if( strcmp( pattern, pPatterns[ p ] ) == 0 ){
			return true;
		}
	}
	
	return false;
}

void dePatternList::AddPattern( const char *pattern ){
	if( HasPattern( pattern ) ) DETHROW( deeInvalidParam );
	
	if( pPatternCount == pPatternSize ){
		int newSize = pPatternSize * 3 / 2 + 1;
		char **newArray = new char*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pPatterns ){
			memcpy( newArray, pPatterns, sizeof( char* ) * pPatternSize );
			delete [] pPatterns;
		}
		pPatterns = newArray;
		pPatternSize = newSize;
	}
	
	pPatterns[ pPatternCount ] = new char[ strlen( pattern ) + 1 ];
	if( ! pPatterns[ pPatternCount ] ) DETHROW( deeOutOfMemory );
	strcpy( pPatterns[ pPatternCount ], pattern );
	
	pPatternCount++;
}

void dePatternList::RemovePattern( const char *pattern ){
	int i, index = IndexOfPattern( pattern );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	delete [] pPatterns[ index ];
	
	for( i=index+1; i<pPatternCount; i++ ){
		pPatterns[ i - 1 ] = pPatterns[ i ];
	}
	pPatternCount--;
}

void dePatternList::RemoveAllPatterns(){
	while( pPatternCount > 0 ){
		pPatternCount--;
		delete [] pPatterns[ pPatternCount ];
	}
}



void dePatternList::SortPatternByLength(){
	int p = 1, length1, length2;
	char *exchange;
	
	while( p < pPatternCount ){
		length1 = strlen( pPatterns[ p - 1 ] );
		length2 = strlen( pPatterns[ p ] );
		
		if( length2 > length1 ){
			exchange = pPatterns[ p - 1 ];
			pPatterns[ p - 1 ] = pPatterns[ p ];
			pPatterns[ p ] = exchange;
			
			if( p > 1 ){
				p--;
				continue;
			}
		}
		
		p++;
	}
}

bool dePatternList::PathMatches( const char *path ) const{
	if( ! path ) DETHROW( deeInvalidParam );
	int pathLength = strlen( path );
	int p, patternLength;
	
	for( p=0; p<pPatternCount; p++ ){
		patternLength = strlen( pPatterns[ p ] );
		
		if( patternLength < pathLength ){
			if( strcmp( path + ( pathLength - patternLength ), pPatterns[ p ] ) == 0 ){
				return true;
			}
		}
	}
	
	return false;
}

int dePatternList::IndexOfPatternMatchingPath( const char *path ) const{
	if( ! path ) DETHROW( deeInvalidParam );
	int pathLength = strlen( path );
	int p, patternLength;
	
	for( p=0; p<pPatternCount; p++ ){
		patternLength = strlen( pPatterns[ p ] );
		
		if( patternLength < pathLength ){
			if( strcmp( path + ( pathLength - patternLength ), pPatterns[ p ] ) == 0 ){
				return p;
			}
		}
	}
	
	return -1;
}
