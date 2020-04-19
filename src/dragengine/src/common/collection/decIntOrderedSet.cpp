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

#include "decIntOrderedSet.h"
#include "../exceptions.h"



// Class decIntOrderedSet
///////////////////////////

// Constructor, destructor
////////////////////////////

decIntOrderedSet::decIntOrderedSet(){
	pValues = NULL;
	pValueCount = 0;
	pValueSize = 0;
}

decIntOrderedSet::decIntOrderedSet( int capacity ){
	if( capacity < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pValues = NULL;
	pValueCount = 0;
	pValueSize = 0;
	
	if( capacity > 0 ){
		pValues = new int[ capacity ];
		pValueSize = capacity;
	}
}

decIntOrderedSet::decIntOrderedSet( const decIntOrderedSet &set ){
	int count = set.GetCount();
	
	pValues = NULL;
	pValueCount = 0;
	pValueSize = 0;
	
	if( count > 0 ){
		pValues = new int[ count ];
		pValueSize = count;
		
		for( pValueCount=0; pValueCount<count; pValueCount++ ){
			pValues[ pValueCount ] = set.pValues[ pValueCount ];
		}
	}
}

decIntOrderedSet::~decIntOrderedSet(){
	RemoveAll();
	
	if( pValues ){
		delete [] pValues;
	}
}



// Management
///////////////

int decIntOrderedSet::GetAt( int index ) const{
	if( index < 0 || index >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pValues[ index ];
}

int decIntOrderedSet::IndexOf( int value ) const{
	int p;
	
	for( p=0; p<pValueCount; p++ ){
		if( value == pValues[ p ] ){
			return p;
		}
	}
	
	return -1;
}

bool decIntOrderedSet::Has( int value ) const{
	int p;
	
	for( p=0; p<pValueCount; p++ ){
		if( value == pValues[ p ] ){
			return true;
		}
	}
	
	return false;
}

void decIntOrderedSet::Add( int value ){
	if( Has( value ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pValueCount == pValueSize ){
		int newSize = pValueSize * 3 / 2 + 1;
		int *newArray = new int[ newSize ];
		if( pValues ){
			memcpy( newArray, pValues, sizeof( int ) * pValueSize );
			delete [] pValues;
		}
		pValues = newArray;
		pValueSize = newSize;
	}
	
	pValues[ pValueCount ] = value;
	pValueCount++;
}

void decIntOrderedSet::AddIfAbsent( int value ){
	if( Has( value ) ){
		return;
	}
	
	if( pValueCount == pValueSize ){
		int newSize = pValueSize * 3 / 2 + 1;
		int *newArray = new int[ newSize ];
		if( pValues ){
			memcpy( newArray, pValues, sizeof( int ) * pValueSize );
			delete [] pValues;
		}
		pValues = newArray;
		pValueSize = newSize;
	}
	
	pValues[ pValueCount ] = value;
	pValueCount++;
}

void decIntOrderedSet::Insert( int value, int index ){
	if( Has( value ) || index < 0 || index > pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	if( pValueCount == pValueSize ){
		int newSize = pValueSize * 3 / 2 + 1;
		int *newArray = new int[ newSize ];
		if( pValues ){
			memcpy( newArray, pValues, sizeof( int ) * pValueSize );
			delete [] pValues;
		}
		pValues = newArray;
		pValueSize = newSize;
	}
	
	for( i=pValueCount; i>index; i-- ){
		pValues[ i ] = pValues[ i - 1 ];
	}
	pValues[ index ] = value;
	pValueCount++;
}

void decIntOrderedSet::Move( int value, int to ){
	const int from = IndexOf( value );
	if( from == -1 || to < 0 || to >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	const int tempValue = pValues[ from ];
	int i;
	
	if( to < from ){
		for( i=from; i>to; i-- ){
			pValues[ i ] = pValues[ i - 1 ];
		}
		
	}else if( to > from ){
		for( i=from; i<to; i++ ){
			pValues[ i ] = pValues[ i + 1 ];
		}
	}
	
	pValues[ to ] = tempValue;
}

void decIntOrderedSet::Remove( int value ){
	int p, position = IndexOf( value );
	
	if( position == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	for( p=position+1; p<pValueCount; p++ ){
		pValues[ p - 1 ] = pValues[ p ];
	}
	pValueCount--;
}

void decIntOrderedSet::RemoveIfPresent( int value ){
	int p, position = IndexOf( value );
	
	if( position == -1 ){
		return;
	}
	
	for( p=position+1; p<pValueCount; p++ ){
		pValues[ p - 1 ] = pValues[ p ];
	}
	pValueCount--;
}

void decIntOrderedSet::RemoveAll(){
	pValueCount = 0;
}



bool decIntOrderedSet::Equals( const decIntOrderedSet &set ) const{
	int p;
	
	if( set.pValueCount != pValueCount ){
		return false;
	}
	
	for( p=0; p<pValueCount; p++ ){
		if( ! set.Has( pValues[ p ] ) ){
			return false;
		}
	}
	
	return true;
}

decIntOrderedSet decIntOrderedSet::GetHead( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pValueCount ){
		count = pValueCount;
	}
	if( count == 0 ){
		return decIntOrderedSet();
	}
	
	decIntOrderedSet list( count );
	
	for( list.pValueCount=0; list.pValueCount<count; list.pValueCount++ ){
		list.pValues[ list.pValueCount ] = pValues[ list.pValueCount ];
	}
	
	return list;
}

void decIntOrderedSet::GetHead( decIntOrderedSet &list, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pValueCount ){
		count = pValueCount;
	}
	
	if( count > list.pValueSize ){
		int *newArray = new int[ count ];
		if( list.pValues ){
			delete [] list.pValues;
		}
		list.pValues = newArray;
		list.pValueSize = count;
	}
	
	for( list.pValueCount=0; list.pValueCount<count; list.pValueCount++ ){
		list.pValues[ list.pValueCount ] = pValues[ list.pValueCount ];
	}
}

decIntOrderedSet decIntOrderedSet::GetTail( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pValueCount ){
		count = pValueCount;
	}
	if( count == 0 ){
		return decIntOrderedSet();
	}
	
	decIntOrderedSet list( count );
	int from = pValueCount - count;
	
	for( list.pValueCount=0; list.pValueCount<count; list.pValueCount++ ){
		list.pValues[ list.pValueCount ] = pValues[ from + list.pValueCount ];
	}
	
	return list;
}

void decIntOrderedSet::GetTail( decIntOrderedSet &list, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pValueCount ){
		count = pValueCount;
	}
	
	if( count > list.pValueSize ){
		int *newArray = new int[ count ];
		if( list.pValues ){
			delete [] list.pValues;
		}
		list.pValues = newArray;
		list.pValueSize = count;
	}
	
	int from = pValueCount - count;
	
	for( list.pValueCount=0; list.pValueCount<count; list.pValueCount++ ){
		list.pValues[ list.pValueCount ] = pValues[ from + list.pValueCount ];
	}
}

decIntOrderedSet decIntOrderedSet::GetMiddle( int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pValueCount ){
		count = pValueCount - from;
	}
	if( count == 0 ){
		return decIntOrderedSet();
	}
	
	decIntOrderedSet list( count );
	
	for( list.pValueCount=0; list.pValueCount<count; list.pValueCount++ ){
		list.pValues[ list.pValueCount ] = pValues[ from + list.pValueCount ];
	}
	
	return list;
	
}

void decIntOrderedSet::GetMiddle( decIntOrderedSet &list, int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pValueCount ){
		count = pValueCount - from;
	}
	
	if( count > list.pValueSize ){
		int *newArray = new int[ count ];
		if( list.pValues ){
			delete [] list.pValues;
		}
		list.pValues = newArray;
		list.pValueSize = count;
	}
	
	for( list.pValueCount=0; list.pValueCount<count; list.pValueCount++ ){
		list.pValues[ list.pValueCount ] = pValues[ from + list.pValueCount ];
	}
}

decIntOrderedSet decIntOrderedSet::GetSliced( int from, int to, int step ) const{
	// TODO Implementation
	return GetMiddle( from, to );
}

void decIntOrderedSet::GetSliced( decIntOrderedSet &list, int from, int to, int step ) const{
	// TODO Implementation
	GetMiddle( list, from, to );
}



// Operators
//////////////

bool decIntOrderedSet::operator==( const decIntOrderedSet &set ) const{
	return Equals( set );
}

decIntOrderedSet decIntOrderedSet::operator+( const decIntOrderedSet &set ) const{
	decIntOrderedSet nset( pValueCount + set.pValueCount );
	int i;
	
	for( i=0; i<pValueCount; i++ ){
		nset.pValues[ i ] = pValues[ i ];
	}
	
	for( i=0; i<set.pValueCount; i++ ){
		nset.AddIfAbsent( set.pValues[ i ] );
	}
	
	return nset;
}

int decIntOrderedSet::operator[]( int index ) const{
	return GetAt( index );
}



decIntOrderedSet &decIntOrderedSet::operator=( const decIntOrderedSet &set ){
	if( set.pValueCount > pValueSize ){
		int *newArray = new int[ set.pValueCount ];
		if( pValues ){
			delete [] pValues;
		}
		pValues = newArray;
		pValueSize = set.pValueCount;
	}
	
	for( pValueCount=0; pValueCount<set.pValueCount; pValueCount++ ){
		pValues[ pValueCount ] = set.pValues[ pValueCount ];
	}
	
	return *this;
}

decIntOrderedSet &decIntOrderedSet::operator+=( const decIntOrderedSet &set ){
	if( set.pValueCount > 0 ){
		int i, count = pValueCount + set.pValueCount;
		
		if( count > pValueSize ){
			int *newArray = new int[ count ];
			if( pValues ){
				memcpy( newArray, pValues, sizeof( int ) * pValueSize );
				delete [] pValues;
			}
			pValues = newArray;
			pValueSize = count;
		}
		
		for( i=0; i<count; i++ ){
			AddIfAbsent( set.pValues[ i ] );
		}
	}
	
	return *this;
}
