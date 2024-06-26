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

#include "decIntList.h"
#include "../exceptions.h"



// Class decIntList
/////////////////////

// Constructor, destructor
////////////////////////////

decIntList::decIntList(){
	pValues = NULL;
	pValueCount = 0;
	pValueSize = 0;
}

decIntList::decIntList( int capacity ){
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

decIntList::decIntList( const decIntList &list ){
	int count = list.GetCount();
	
	pValues = NULL;
	pValueCount = 0;
	pValueSize = 0;
	
	if( count > 0 ){
		pValues = new int[ count ];
		pValueSize = count;
		
		for( pValueCount=0; pValueCount<count; pValueCount++ ){
			pValues[ pValueCount ] = list.pValues[ pValueCount ];
		}
	}
}

decIntList::~decIntList(){
	if( pValues ){
		delete [] pValues;
	}
}



// Management
///////////////

int decIntList::GetAt( int index ) const{
	if( index < 0 || index >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pValues[ index ];
}

void decIntList::SetAt( int index, int value ){
	if( index < 0 || index >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	pValues[ index ] = value;
}

int decIntList::IndexOf( int value ) const{
	int p;
	
	for( p=0; p<pValueCount; p++ ){
		if( value == pValues[ p ] ){
			return p;
		}
	}
	
	return -1;
}

int decIntList::IndexOf( int value, int start ) const{
	if( start < 0 || start >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	int p;
	
	for( p=start; p<pValueCount; p++ ){
		if( value == pValues[ p ] ){
			return p;
		}
	}
	
	return -1;
}

bool decIntList::Has( int value ) const{
	int p;
	
	for( p=0; p<pValueCount; p++ ){
		if( value == pValues[ p ] ){
			return true;
		}
	}
	
	return false;
}

int decIntList::CountOccurance( int value ) const{
	int p, occuranceCount = 0;
	
	for( p=0; p<pValueCount; p++ ){
		if( value == pValues[ p ] ){
			occuranceCount++;
		}
	}
	
	return occuranceCount;
}

void decIntList::Add( int value ){
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

void decIntList::Insert( int value, int index ){
	if( index < 0 || index > pValueCount ){
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

void decIntList::Move( int from, int to ){
	if( from < 0 || from >= pValueCount || to < 0 || to >= pValueCount ){
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

void decIntList::RemoveFrom( int index ){
	if( index < 0 || index >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	int p;
	
	for( p=index+1; p<pValueCount; p++ ){
		pValues[ p - 1 ] = pValues[ p ];
	}
	pValueCount--;
}

void decIntList::RemoveAll(){
	pValueCount = 0;
}



bool decIntList::Equals( const decIntList &list ) const{
	int p;
	
	if( list.pValueCount != pValueCount ){
		return false;
	}
	
	for( p=0; p<pValueCount; p++ ){
		if( list.pValues[ p ] != pValues[ p ] ){
			return false;
		}
	}
	
	return true;
}

decIntList decIntList::GetHead( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pValueCount ){
		count = pValueCount;
	}
	if( count == 0 ){
		return decIntList();
	}
	
	decIntList list( count );
	
	for( list.pValueCount=0; list.pValueCount<count; list.pValueCount++ ){
		list.pValues[ list.pValueCount ] = pValues[ list.pValueCount ];
	}
	
	return list;
}

void decIntList::GetHead( decIntList &list, int count ) const{
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

decIntList decIntList::GetTail( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pValueCount ){
		count = pValueCount;
	}
	if( count == 0 ){
		return decIntList();
	}
	
	decIntList list( count );
	int from = pValueCount - count;
	
	for( list.pValueCount=0; list.pValueCount<count; list.pValueCount++ ){
		list.pValues[ list.pValueCount ] = pValues[ from + list.pValueCount ];
	}
	
	return list;
}

void decIntList::GetTail( decIntList &list, int count ) const{
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

decIntList decIntList::GetMiddle( int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pValueCount ){
		count = pValueCount - from;
	}
	if( count == 0 ){
		return decIntList();
	}
	
	decIntList list( count );
	
	for( list.pValueCount=0; list.pValueCount<count; list.pValueCount++ ){
		list.pValues[ list.pValueCount ] = pValues[ from + list.pValueCount ];
	}
	
	return list;
	
}

void decIntList::GetMiddle( decIntList &list, int from, int to ) const{
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

decIntList decIntList::GetSliced( int from, int to, int step ) const{
	// TODO Implementation
	return GetMiddle( from, to );
}

void decIntList::GetSliced( decIntList &list, int from, int to, int step ) const{
	// TODO Implementation
	GetMiddle( list, from, to );
}



// Operators
//////////////

bool decIntList::operator==( const decIntList &list ) const{
	return Equals( list );
}

decIntList decIntList::operator+( const decIntList &list ) const{
	decIntList nlist( pValueCount + list.pValueCount );
	int r;
	
	for( r=0; r<pValueCount; r++ ){
		nlist.pValues[ r ] = pValues[ r ];
	}
	
	for( r=0; r<list.pValueCount; r++ ){
		nlist.pValues[ pValueCount + r ] = list.pValues[ r ];
	}
	
	return nlist;
}

int decIntList::operator[]( int index ) const{
	return GetAt( index );
}



decIntList &decIntList::operator=( const decIntList &list ){
	if( &list == this ){
		return *this;
	}
	
	if( list.pValueCount > pValueSize ){
		int *newArray = new int[ list.pValueCount ];
		if( pValues ){
			delete [] pValues;
		}
		pValues = newArray;
		pValueSize = list.pValueCount;
	}
	
	for( pValueCount=0; pValueCount<list.pValueCount; pValueCount++ ){
		pValues[ pValueCount ] = list.pValues[ pValueCount ];
	}
	
	return *this;
}

decIntList &decIntList::operator+=( const decIntList &list ){
	if( list.pValueCount > 0 ){
		int r, count = pValueCount + list.pValueCount;
		
		if( count > pValueSize ){
			int *newArray = new int[ count ];
			if( pValues ){
				memcpy( newArray, pValues, sizeof( int ) * pValueSize );
				delete [] pValues;
			}
			pValues = newArray;
			pValueSize = count;
		}
		
		for( r=0; r<count; r++ ){
			pValues[ pValueCount++ ] = list.pValues[ r ];
		}
	}
	
	return *this;
}
