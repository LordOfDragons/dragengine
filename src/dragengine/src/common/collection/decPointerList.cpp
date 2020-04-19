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

#include "decPointerList.h"

#include "../exceptions.h"



// Class decPointerList
/////////////////////////

// Constructor, destructor
////////////////////////////

decPointerList::decPointerList(){
	pPointers = NULL;
	pPointerCount = 0;
	pPointerSize = 0;
}

decPointerList::decPointerList( int capacity ){
	if( capacity < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pPointers = NULL;
	pPointerCount = 0;
	pPointerSize = 0;
	
	if( capacity > 0 ){
		pPointers = new void*[ capacity ];
		pPointerSize = capacity;
	}
}

decPointerList::decPointerList( const decPointerList &list ){
	const int count = list.GetCount();
	
	pPointers = NULL;
	pPointerCount = 0;
	pPointerSize = 0;
	
	if( count > 0 ){
		pPointers = new void*[ count ];
		pPointerSize = count;
		
		memcpy( pPointers, list.pPointers, sizeof( void* ) * count );
		pPointerCount = count;
	}
}

decPointerList::~decPointerList(){
	if( pPointers ){
		delete [] pPointers;
	}
}



// Management
///////////////

void *decPointerList::GetAt( int position ) const{
	if( position < 0 || position >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	return pPointers[ position ];
}

void decPointerList::SetAt( int position, void *pointer ){
	if( position < 0 || position >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	pPointers[ position ] = pointer;
}

int decPointerList::IndexOf( void *pointer ) const{
	int p;
	
	for( p=0; p<pPointerCount; p++ ){
		if( pointer == pPointers[ p ] ){
			return p;
		}
	}
	
	return -1;
}

int decPointerList::IndexOf( void *pointer, int start ) const{
	if( start < 0 || start >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	int p;
	
	for( p=start; p<pPointerCount; p++ ){
		if( pointer == pPointers[ p ] ){
			return p;
		}
	}
	
	return -1;
}

bool decPointerList::Has( void *pointer ) const{
	int p;
	
	for( p=0; p<pPointerCount; p++ ){
		if( pointer == pPointers[ p ] ){
			return true;
		}
	}
	
	return false;
}

int decPointerList::CountOccurance( void *pointer ) const{
	int p, occuranceCount = 0;
	
	for( p=0; p<pPointerCount; p++ ){
		if( pointer == pPointers[ p ] ){
			occuranceCount++;
		}
	}
	
	return occuranceCount;
}

void decPointerList::Add( void *pointer ){
	if( pPointerCount == pPointerSize ){
		const int newSize = pPointerSize * 3 / 2 + 1;
		void ** const newArray = new void*[ newSize ];
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

void decPointerList::Insert( void *pointer, int index ){
	if( Has( pointer ) || index < 0 || index > pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	if( pPointerCount == pPointerSize ){
		const int newSize = pPointerSize * 3 / 2 + 1;
		void ** const newArray = new void*[ newSize ];
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

void decPointerList::Move( void *pointer, int index ){
	const int oldIndex = IndexOf( pointer );
	if( oldIndex == -1 || index < 0 || index >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	void * const tempPointer = pPointers[ oldIndex ];
	int i;
	
	if( index < oldIndex ){
		for( i=oldIndex; i>index; i-- ){
			pPointers[ i ] = pPointers[ i - 1 ];
		}
		
	}else if( index > oldIndex ){
		for( i=oldIndex; i<index; i++ ){
			pPointers[ i ] = pPointers[ i + 1 ];
		}
	}
	
	pPointers[ index ] = tempPointer;
}

void decPointerList::RemoveFrom( int position ){
	if( position < 0 || position >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	int p;
	
	for( p=position+1; p<pPointerCount; p++ ){
		pPointers[ p - 1 ] = pPointers[ p ];
	}
	
	pPointerCount--;
}

void decPointerList::RemoveAll(){
	pPointerCount = 0;
}



bool decPointerList::Equals( const decPointerList &list ) const{
	int p;
	
	if( list.pPointerCount != pPointerCount ){
		return false;
	}
	
	for( p=0; p<pPointerCount; p++ ){
		if( list.pPointers[ p ] != pPointers[ p ] ){
			return false;
		}
	}
	
	return true;
}

decPointerList decPointerList::GetHead( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pPointerCount ){
		count = pPointerCount;
	}
	if( count == 0 ){
		return decPointerList();
	}
	
	decPointerList list( count );
	memcpy( list.pPointers, pPointers, sizeof( void* ) * count );
	list.pPointerCount = count;
	
	return list;
}

void decPointerList::GetHead( decPointerList &list, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pPointerCount ){
		count = pPointerCount;
	}
	list.pPointerCount = 0;
	
	if( count > list.pPointerSize ){
		void ** const newArray = new void*[ count ];
		if( list.pPointers ){
			delete [] list.pPointers;
		}
		list.pPointers = newArray;
		list.pPointerSize = count;
	}
	
	if( count > 0 ){
		memcpy( list.pPointers, pPointers, sizeof( void* ) * count );
		list.pPointerCount = count;
	}
}

decPointerList decPointerList::GetTail( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pPointerCount ){
		count = pPointerCount;
	}
	if( count == 0 ){
		return decPointerList();
	}
	
	decPointerList list( count );
	memcpy( list.pPointers, pPointers + ( pPointerCount - count ), sizeof( void* ) * count );
	list.pPointerCount = count;
	
	return list;
}

void decPointerList::GetTail( decPointerList &list, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pPointerCount ){
		count = pPointerCount;
	}
	list.pPointerCount = 0;
	
	if( count > list.pPointerSize ){
		void ** const newArray = new void*[ count ];
		if( list.pPointers ){
			delete [] list.pPointers;
		}
		list.pPointers = newArray;
		list.pPointerSize = count;
	}
	
	if( count > 0 ){
		memcpy( list.pPointers, pPointers + ( pPointerCount - count ), sizeof( void* ) * count );
		list.pPointerCount = count;
	}
}

decPointerList decPointerList::GetMiddle( int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pPointerCount ){
		count = pPointerCount - from;
	}
	if( count == 0 ){
		return decPointerList();
	}
	
	decPointerList list( count );
	memcpy( list.pPointers, pPointers + from, sizeof( void* ) * count );
	list.pPointerCount = count;
	
	return list;
	
}

void decPointerList::GetMiddle( decPointerList &list, int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pPointerCount ){
		count = pPointerCount - from;
	}
	list.pPointerCount = 0;
	
	if( count > list.pPointerSize ){
		void ** const newArray = new void*[ count ];
		if( list.pPointers ){
			delete [] list.pPointers;
		}
		list.pPointers = newArray;
		list.pPointerSize = count;
	}
	
	if( count > 0 ){
		memcpy( list.pPointers, pPointers + from, sizeof( void* ) * count );
		list.pPointerCount = count;
	}
}

decPointerList decPointerList::GetSliced( int from, int to, int step ) const{
	// temporary hack: same as middle
	return GetMiddle( from, to );
}

void decPointerList::GetSliced( decPointerList &list, int from, int to, int step ) const{
	// temporary hack: same as middle
	GetMiddle( list, from, to );
}



// Operators
//////////////

bool decPointerList::operator==( const decPointerList &list ) const{
	return Equals( list );
}

decPointerList decPointerList::operator+( const decPointerList &list ) const{
	decPointerList nlist( pPointerCount + list.pPointerCount );
	
	memcpy( nlist.pPointers, pPointers, sizeof( void* ) * pPointerCount );
	memcpy( nlist.pPointers + pPointerCount, list.pPointers, sizeof( void* ) * list.pPointerCount );
	
	return nlist;
}

void *decPointerList::operator[]( int position ) const{
	if( position < 0 || position >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	return pPointers[ position ];
}

void *&decPointerList::operator[]( int position ){
	if( position < 0 || position >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	return pPointers[ position ];
}



decPointerList &decPointerList::operator=( const decPointerList &list ){
	pPointerCount = 0;
	
	if( list.pPointerCount > pPointerSize ){
		void ** const newArray = new void*[ list.pPointerCount ];
		if( pPointers ){
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = list.pPointerCount;
	}
	
	if( list.pPointerCount > 0 ){
		memcpy( pPointers, list.pPointers, sizeof( void* ) * list.pPointerCount );
		pPointerCount = list.pPointerCount;
	}
	
	return *this;
}

decPointerList &decPointerList::operator+=( const decPointerList &list ){
	if( list.pPointerCount > 0 ){
		if( pPointerCount + list.pPointerCount > pPointerSize ){
			const int newSize = pPointerCount + list.pPointerCount;
			void ** const newArray = new void*[ newSize ];
			if( pPointers ){
				memcpy( newArray, pPointers, sizeof( void* ) * pPointerSize );
				delete [] pPointers;
			}
			pPointers = newArray;
			pPointerSize = newSize;
		}
		
		memcpy( pPointers + pPointerCount, list.pPointers, sizeof( void* ) * list.pPointerCount );
		pPointerCount += list.pPointerCount;
	}
	
	return *this;
}
