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
	int i;
	
	if( oldIndex == -1 || index < 0 || index > pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < oldIndex ){
		void * const tempPointer = pPointers[ oldIndex ];
		for( i=oldIndex; i>index; i-- ){
			pPointers[ i ] = pPointers[ i - 1 ];
		}
		pPointers[ index ] = tempPointer;
		
	}else if( index > oldIndex ){
		void * const tempPointer = pPointers[ oldIndex ];
		for( i=oldIndex; i<index; i++ ){
			pPointers[ i ] = pPointers[ i + 1 ];
		}
		
		if( index == pPointerCount ){
			pPointers[ pPointerCount - 1 ] = tempPointer;
			
		}else{
			pPointers[ index ] = tempPointer;
		}
	}
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
	}
	
	return *this;
}
