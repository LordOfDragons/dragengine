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

#include "decPointerOrderedSet.h"

#include "../exceptions.h"



// Class decPointerOrderedSet
///////////////////////////////

// Constructor, destructor
////////////////////////////

decPointerOrderedSet::decPointerOrderedSet() :
pPointers( NULL ),
pPointerCount( 0 ),
pPointerSize( 0 ){
}

decPointerOrderedSet::decPointerOrderedSet( int capacity ) :
pPointers( NULL ),
pPointerCount( 0 ),
pPointerSize( 0 )
{
	if( capacity < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( capacity == 0 ){
		return;
	}
	
	pPointers = new void*[ capacity ];
	pPointerSize = capacity;
}

decPointerOrderedSet::decPointerOrderedSet( const decPointerOrderedSet &set ) :
pPointers( NULL ),
pPointerCount( 0 ),
pPointerSize( 0 )
{
	const int count = set.GetCount();
	if( count == 0 ){
		return;
	}
	
	pPointers = new void*[ count ];
	pPointerSize = count;
	
	for( pPointerCount=0; pPointerCount<count; pPointerCount++ ){
		pPointers[ pPointerCount ] = set.pPointers[ pPointerCount ];
	}
}

decPointerOrderedSet::~decPointerOrderedSet(){
	RemoveAll();
	
	if( pPointers ){
		delete [] pPointers;
	}
}



// Management
///////////////

void *decPointerOrderedSet::GetAt( int index ) const{
	if( index < 0 || index >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPointers[ index ];
}

int decPointerOrderedSet::IndexOf( void *pointer ) const{
	int p;
	
	for( p=0; p<pPointerCount; p++ ){
		if( pointer == pPointers[ p ] ){
			return p;
		}
	}
	
	return -1;
}

bool decPointerOrderedSet::Has( void *pointer ) const{
	int p;
	
	for( p=0; p<pPointerCount; p++ ){
		if( pointer == pPointers[ p ] ){
			return true;
		}
	}
	
	return false;
}

void decPointerOrderedSet::Add( void *pointer ){
	if( Has( pointer ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPointerCount == pPointerSize ){
		int newSize = pPointerSize * 3 / 2 + 1;
		void **newArray = new void*[ newSize ];
		if( pPointers ){
			memcpy( newArray, pPointers, sizeof( void* ) * pPointerSize );
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = newSize;
	}
	
	pPointers[ pPointerCount ] = pointer;
	pPointerCount++;
}

void decPointerOrderedSet::AddIfAbsent( void *pointer ){
	if( Has( pointer ) ){
		return;
	}
	
	if( pPointerCount == pPointerSize ){
		int newSize = pPointerSize * 3 / 2 + 1;
		void **newArray = new void*[ newSize ];
		if( pPointers ){
			memcpy( newArray, pPointers, sizeof( void* ) * pPointerSize );
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = newSize;
	}
	
	pPointers[ pPointerCount ] = pointer;
	pPointerCount++;
}

void decPointerOrderedSet::Insert( void *pointer, int index ){
	if( Has( pointer ) || index < 0 || index > pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	if( pPointerCount == pPointerSize ){
		int newSize = pPointerSize * 3 / 2 + 1;
		void **newArray = new void*[ newSize ];
		if( pPointers ){
			memcpy( newArray, pPointers, sizeof( void* ) * pPointerSize );
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = newSize;
	}
	
	for( i=pPointerCount; i>index; i-- ){
		pPointers[ i ] = pPointers[ i - 1 ];
	}
	pPointers[ index ] = pointer;
	pPointerCount++;
}

void decPointerOrderedSet::Move( void *pointer, int to ){
	const int from = IndexOf( pointer );
	if( from == -1 || to < 0 || to >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	void * const tempPointer = pPointers[ from ];
	int i;
	
	if( to < from ){
		for( i=from; i>to; i-- ){
			pPointers[ i ] = pPointers[ i - 1 ];
		}
		
	}else if( to > from ){
		for( i=from; i<to; i++ ){
			pPointers[ i ] = pPointers[ i + 1 ];
		}
	}
	
	pPointers[ to ] = tempPointer;
}

void decPointerOrderedSet::Remove( void *pointer ){
	int p, position = IndexOf( pointer );
	
	if( position == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	for( p=position+1; p<pPointerCount; p++ ){
		pPointers[ p - 1 ] = pPointers[ p ];
	}
	pPointerCount--;
}

void decPointerOrderedSet::RemoveIfPresent( void *pointer ){
	int p, position = IndexOf( pointer );
	
	if( position == -1 ){
		return;
	}
	
	for( p=position+1; p<pPointerCount; p++ ){
		pPointers[ p - 1 ] = pPointers[ p ];
	}
	pPointerCount--;
}

void decPointerOrderedSet::RemoveFrom( int index ){
	if( index < 0 || index >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=index+1; i<pPointerCount; i++ ){
		pPointers[ i - 1 ] = pPointers[ i ];
	}
	pPointerCount--;
}

void decPointerOrderedSet::RemoveAll(){
	pPointerCount = 0;
}



bool decPointerOrderedSet::Equals( const decPointerOrderedSet &set ) const{
	int p;
	
	if( set.pPointerCount != pPointerCount ){
		return false;
	}
	
	for( p=0; p<pPointerCount; p++ ){
		if( ! set.Has( pPointers[ p ] ) ){
			return false;
		}
	}
	
	return true;
}

decPointerOrderedSet decPointerOrderedSet::GetHead( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pPointerCount ){
		count = pPointerCount;
	}
	if( count == 0 ){
		return decPointerOrderedSet();
	}
	
	decPointerOrderedSet set( count );
	for( set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++ ){
		set.pPointers[ set.pPointerCount ] = pPointers[ set.pPointerCount ];
	}
	
	return set;
}

void decPointerOrderedSet::GetHead( decPointerOrderedSet &set, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pPointerCount ){
		count = pPointerCount;
	}
	
	if( count > set.pPointerSize ){
		void **newArray = new void*[ count ];
		if( set.pPointers ){
			delete [] set.pPointers;
		}
		set.pPointers = newArray;
		set.pPointerSize = count;
	}
	
	for( set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++ ){
		set.pPointers[ set.pPointerCount ] = pPointers[ set.pPointerCount ];
	}
}

decPointerOrderedSet decPointerOrderedSet::GetTail( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pPointerCount ){
		count = pPointerCount;
	}
	if( count == 0 ){
		return decPointerOrderedSet();
	}
	
	decPointerOrderedSet set( count );
	int from = pPointerCount - count;
	
	for( set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++ ){
		set.pPointers[ set.pPointerCount ] = pPointers[ from + set.pPointerCount ];
	}
	
	return set;
}

void decPointerOrderedSet::GetTail( decPointerOrderedSet &set, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pPointerCount ){
		count = pPointerCount;
	}
	
	if( count > set.pPointerSize ){
		void **newArray = new void*[ count ];
		if( set.pPointers ){
			delete [] set.pPointers;
		}
		set.pPointers = newArray;
		set.pPointerSize = count;
	}
	
	int from = pPointerCount - count;
	
	for( set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++ ){
		set.pPointers[ set.pPointerCount ] = pPointers[ from + set.pPointerCount ];
	}
}

decPointerOrderedSet decPointerOrderedSet::GetMiddle( int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pPointerCount ){
		count = pPointerCount - from;
	}
	if( count == 0 ){
		return decPointerOrderedSet();
	}
	
	decPointerOrderedSet set( count );
	
	for( set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++ ){
		set.pPointers[ set.pPointerCount ] = pPointers[ from + set.pPointerCount ];
	}
	
	return set;
	
}

void decPointerOrderedSet::GetMiddle( decPointerOrderedSet &set, int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pPointerCount ){
		count = pPointerCount - from;
	}
	
	if( count > set.pPointerSize ){
		void **newArray = new void*[ count ];
		if( set.pPointers ){
			delete [] set.pPointers;
		}
		set.pPointers = newArray;
		set.pPointerSize = count;
	}
	
	for( set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++ ){
		set.pPointers[ set.pPointerCount ] = pPointers[ from + set.pPointerCount ];
	}
}

decPointerOrderedSet decPointerOrderedSet::GetSliced( int from, int to, int step ) const{
	// temporary hack: same as middle
	return GetMiddle( from, to );
}

void decPointerOrderedSet::GetSliced( decPointerOrderedSet &set, int from, int to, int step ) const{
	// temporary hack: same as middle
	GetMiddle( set, from, to );
}



// Operators
//////////////

bool decPointerOrderedSet::operator==( const decPointerOrderedSet &set ) const{
	return Equals( set );
}

decPointerOrderedSet decPointerOrderedSet::operator+( const decPointerOrderedSet &set ) const{
	decPointerOrderedSet nset( pPointerCount + set.pPointerCount );
	int i;
	
	for( i=0; i<pPointerCount; i++ ){
		nset.pPointers[ i ] = pPointers[ i ];
	}
	
	for( i=0; i<set.pPointerCount; i++ ){
		nset.AddIfAbsent( set.pPointers[ i ] );
	}
	
	return nset;
}

void *decPointerOrderedSet::operator[]( int index ) const{
	return GetAt( index );
}



decPointerOrderedSet &decPointerOrderedSet::operator=( const decPointerOrderedSet &set ){
	RemoveAll();
	
	if( set.pPointerCount > pPointerSize ){
		void **newArray = new void*[ set.pPointerCount ];
		if( pPointers ){
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = set.pPointerCount;
	}
	
	for( pPointerCount=0; pPointerCount<set.pPointerCount; pPointerCount++ ){
		pPointers[ pPointerCount ] = set.pPointers[ pPointerCount ];
	}
	
	return *this;
}

decPointerOrderedSet &decPointerOrderedSet::operator+=( const decPointerOrderedSet &set ){
	if( set.pPointerCount > 0 ){
		int i, count = pPointerCount + set.pPointerCount;
		
		if( count > pPointerSize ){
			void **newArray = new void*[ count ];
			if( pPointers ){
				memcpy( newArray, pPointers, sizeof( void* ) * pPointerSize );
				delete [] pPointers;
			}
			pPointers = newArray;
			pPointerSize = count;
		}
		
		for( i=0; i<set.pPointerCount; i++ ){
			AddIfAbsent( set.pPointers[ i ] );
		}
	}
	
	return *this;
}
