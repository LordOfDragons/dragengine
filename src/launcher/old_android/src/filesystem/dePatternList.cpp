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
