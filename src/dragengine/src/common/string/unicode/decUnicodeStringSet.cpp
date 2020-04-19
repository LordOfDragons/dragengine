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

#include "decUnicodeStringSet.h"

#include "../../exceptions.h"



// Class decUnicodeStringSet
//////////////////////////////

// Constructor, destructor
////////////////////////////

decUnicodeStringSet::decUnicodeStringSet(){
	pStrings = NULL;
	pStringCount = 0;
	pStringSize = 0;
}

decUnicodeStringSet::decUnicodeStringSet( const decUnicodeStringSet &set ){
	const int count = set.GetCount();
	
	pStrings = NULL;
	pStringCount = 0;
	pStringSize = 0;
	
	if( count > 0 ){
		pStrings = new decUnicodeString*[ count ];
		pStringSize = count;
		
		while( pStringCount < count ){
			pStrings[ pStringCount ] = new decUnicodeString( set.GetAt( pStringCount ) );
			pStringCount++;
		}
	}
}

decUnicodeStringSet::~decUnicodeStringSet(){
	RemoveAll();
	if( pStrings ){
		delete [] pStrings;
	}
}



// Management
///////////////

const decUnicodeString &decUnicodeStringSet::GetAt( int index ) const{
	if( index < 0 || index >= pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	return *pStrings[ index ];
}

int decUnicodeStringSet::IndexOf( const decUnicodeString &string ) const{
	int s;
	
	for( s=0; s<pStringCount; s++ ){
		if( pStrings[ s ]->Equals( string ) ){
			return s;
		}
	}
	
	return -1;
}

bool decUnicodeStringSet::Has( const decUnicodeString &string ) const{
	return IndexOf( string ) != -1;
}

void decUnicodeStringSet::Add( const decUnicodeString &string ){
	if( Has( string ) ){
		return;
	}
	
	if( pStringCount == pStringSize ){
		const int newSize = pStringSize * 3 / 2 + 1;
		decUnicodeString ** const newArray = new decUnicodeString*[ newSize ];
		if( pStrings ){
			memcpy( newArray, pStrings, sizeof( decUnicodeString* ) * pStringSize );
			delete [] pStrings;
		}
		pStrings = newArray;
		pStringSize = newSize;
	}
	
	pStrings[ pStringCount ] = new decUnicodeString( string );
	pStringCount++;
}

void decUnicodeStringSet::Remove( const decUnicodeString &string ){
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

void decUnicodeStringSet::RemoveAll(){
	while( pStringCount > 0 ){
		pStringCount--;
		delete pStrings[ pStringCount ];
	}
}

void decUnicodeStringSet::SortAscending(){
	pSortAscending( 0, pStringCount - 1 );
}

void decUnicodeStringSet::SortDescending(){
	pSortDescending( 0, pStringCount - 1 );
}



// Operators
//////////////

const decUnicodeString &decUnicodeStringSet::operator[]( int index ) const{
	return GetAt( index );
}

decUnicodeStringSet &decUnicodeStringSet::operator=( const decUnicodeStringSet &set ){
	const int count = set.GetCount();
	int s;
	
	RemoveAll();
	for( s=0; s<count; s++ ){
		Add( set.GetAt( s ) ); // hard coding this call would remove the HasString checks
	}
	
	return *this;
}

bool decUnicodeStringSet::operator==( const decUnicodeStringSet &set ){
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

bool decUnicodeStringSet::operator!=( const decUnicodeStringSet &set ){
	return ! ( *this == set );
}

decUnicodeStringSet decUnicodeStringSet::operator+( const decUnicodeStringSet &set ) const{
	const int count = set.GetCount();
	decUnicodeStringSet newSet( *this );
	int s;
	
	for( s=0; s<count; s++ ){
		newSet.Add( set.GetAt( s ) );
	}
	
	return newSet;
}

decUnicodeStringSet &decUnicodeStringSet::operator+=( const decUnicodeStringSet &set ){
	const int count = set.GetCount();
	int s;
	
	for( s=0; s<count; s++ ){
		Add( set.GetAt( s ) );
	}
	
	return *this;
}

decUnicodeStringSet decUnicodeStringSet::operator+( const decUnicodeString &string ) const{
	decUnicodeStringSet newList( *this );
	
	newList.Add( string );
	
	return newList;
}

decUnicodeStringSet &decUnicodeStringSet::operator+=( const decUnicodeString &string ){
	Add( string );
	
	return *this;
}



// Private Functions
//////////////////////

void decUnicodeStringSet::pSortAscending( int left, int right ){
	decUnicodeString * const pivot = pStrings[ left ];
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

void decUnicodeStringSet::pSortDescending( int left, int right ){
	decUnicodeString * const pivot = pStrings[ left ];
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
