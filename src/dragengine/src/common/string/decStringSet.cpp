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
