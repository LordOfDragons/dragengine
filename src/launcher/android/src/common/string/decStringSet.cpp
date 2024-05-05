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

#include "decStringSet.h"

#include "../exceptions.h"



// Class decStringSet
///////////////////////

// Constructor, destructor
////////////////////////////

decStringSet::decStringSet(){
	pStrings = NULL;
	pStringCount = 0;
	pStringSize = 0;
}

decStringSet::decStringSet( const decStringSet &set ){
	const int count = set.GetCount();
	
	pStrings = NULL;
	pStringCount = 0;
	pStringSize = 0;
	
	if( count > 0 ){
		pStrings = new decString*[ count ];
		pStringSize = count;
		
		while( pStringCount < count ){
			pStrings[ pStringCount ] = new decString( set.GetAt( pStringCount ) );
			pStringCount++;
		}
	}
}

decStringSet::~decStringSet(){
	RemoveAll();
	if( pStrings ){
		delete [] pStrings;
	}
}



// Management
///////////////

const decString &decStringSet::GetAt( int index ) const{
	if( index < 0 || index >= pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	return *pStrings[ index ];
}

int decStringSet::IndexOf( const decString &string ) const{
	int s;
	
	for( s=0; s<pStringCount; s++ ){
		if( pStrings[ s ]->Equals( string ) ){
			return s;
		}
	}
	
	return -1;
}

int decStringSet::IndexOf( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	int s;
	
	for( s=0; s<pStringCount; s++ ){
		if( pStrings[ s ]->Equals( string ) ){
			return s;
		}
	}
	
	return -1;
}

bool decStringSet::Has( const decString &string ) const{
	return IndexOf( string ) != -1;
}

bool decStringSet::Has( const char *string ) const{
	return IndexOf( string ) != -1;
}

void decStringSet::Add( const decString &string ){
	if( Has( string ) ){
		return;
	}
	
	if( pStringCount == pStringSize ){
		const int newSize = pStringSize * 3 / 2 + 1;
		decString ** const newArray = new decString*[ newSize ];
		if( pStrings ){
			memcpy( newArray, pStrings, sizeof( decString* ) * pStringSize );
			delete [] pStrings;
		}
		pStrings = newArray;
		pStringSize = newSize;
	}
	
	pStrings[ pStringCount ] = new decString( string );
	pStringCount++;
}

void decStringSet::Add( const char *string ){
	if( Has( string ) ){
		return;
	}
	
	if( pStringCount == pStringSize ){
		const int newSize = pStringSize * 3 / 2 + 1;
		decString ** const newArray = new decString*[ newSize ];
		if( pStrings ){
			memcpy( newArray, pStrings, sizeof( decString* ) * pStringSize );
			delete [] pStrings;
		}
		pStrings = newArray;
		pStringSize = newSize;
	}
	
	pStrings[ pStringCount ] = new decString( string );
	pStringCount++;
}

void decStringSet::Remove( const decString &string ){
	const int index = IndexOf( string );
	int s;
	
	if( index != -1 ){
		delete pStrings[ index ];
		
		for( s=index+1; s<pStringCount; s++ ){
			pStrings[ s - 1 ] = pStrings[ s ];
		}
		pStringCount--;
	}
}

void decStringSet::Remove( const char *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	const int index = IndexOf( string );
	int s;
	
	if( index != -1 ){
		delete pStrings[ index ];
		
		for( s=index+1; s<pStringCount; s++ ){
			pStrings[ s - 1 ] = pStrings[ s ];
		}
		pStringCount--;
	}
}

void decStringSet::RemoveAll(){
	while( pStringCount > 0 ){
		pStringCount--;
		delete pStrings[ pStringCount ];
	}
}

void decStringSet::SortAscending(){
	if( pStringCount > 1 ){
		pSortAscending( 0, pStringCount - 1 );
	}
}

void decStringSet::SortDescending(){
	if( pStringCount > 1 ){
		pSortDescending( 0, pStringCount - 1 );
	}
}



// Operators
//////////////

const decString &decStringSet::operator[]( int index ) const{
	return GetAt( index );
}

decStringSet &decStringSet::operator=( const decStringSet &set ){
	const int count = set.GetCount();
	int s;
	
	RemoveAll();
	for( s=0; s<count; s++ ){
		Add( set.GetAt( s ) ); // hard coding this call would remove the HasString checks
	}
	
	return *this;
}

bool decStringSet::operator==( const decStringSet &set ) const{
	int i;
	
	if( set.GetCount() != pStringCount ){
		return false;
	}
	
	for( i=0; i<pStringCount; i++ ){
		if( ! set.Has( *pStrings[ i ] ) ){
			return false;
		}
	}
	
	return true;
}

bool decStringSet::operator!=( const decStringSet &set ) const{
	return ! ( *this == set );
}

decStringSet decStringSet::operator+( const decStringSet &set ) const{
	const int count = set.GetCount();
	decStringSet newSet( *this );
	int s;
	
	for( s=0; s<count; s++ ){
		newSet.Add( set.GetAt( s ) );
	}
	
	return newSet;
}

decStringSet &decStringSet::operator+=( const decStringSet &set ){
	const int count = set.GetCount();
	int s;
	
	for( s=0; s<count; s++ ){
		Add( set.GetAt( s ) );
	}
	
	return *this;
}

decStringSet decStringSet::operator+( const decString &string ) const{
	decStringSet newList( *this );
	
	newList.Add( string );
	
	return newList;
}

decStringSet decStringSet::operator+( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	decStringSet newList( *this );
	
	newList.Add( string );
	
	return newList;
}

decStringSet &decStringSet::operator+=( const decString &string ){
	Add( string );
	
	return *this;
}

decStringSet &decStringSet::operator+=( const char *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	Add( string );
	
	return *this;
}



// Private Functions
//////////////////////

void decStringSet::pSortAscending( int left, int right ){
	decString * const pivot = pStrings[ left ];
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( ( *pStrings[ right ] >= *pivot ) && ( left < right ) ){
			right--;
		}
		if( left != right ){
			pStrings[ left ] = pStrings[ right ];
			left++;
		}
		while( ( *pStrings[ left ] <= *pivot ) && ( left < right ) ){
			left++;
		}
		if( left != right ){
			pStrings[ right ] = pStrings[ left ];
			right--;
		}
	}
	
	pStrings[ left ] = pivot;
	if( l_hold < left ){
		pSortAscending( l_hold, left - 1 );
	}
	if( r_hold > left ){
		pSortAscending( left + 1, r_hold );
	}
}

void decStringSet::pSortDescending( int left, int right ){
	decString * const pivot = pStrings[ left ];
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( ( *pStrings[ right ] <= *pivot ) && ( left < right ) ){
			right--;
		}
		if( left != right ){
			pStrings[ left ] = pStrings[ right ];
			left++;
		}
		while( ( *pStrings[ left ] >= *pivot ) && ( left < right ) ){
			left++;
		}
		if( left != right ){
			pStrings[ right ] = pStrings[ left ];
			right--;
		}
	}
	
	pStrings[ left ] = pivot;
	if( l_hold < left ){
		pSortDescending( l_hold, left - 1 );
	}
	if( r_hold > left ){
		pSortDescending( left + 1, r_hold );
	}
}
