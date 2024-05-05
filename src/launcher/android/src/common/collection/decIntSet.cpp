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

#include "decIntSet.h"
#include "../exceptions.h"



// Class decIntSet
////////////////////

// Constructor, destructor
////////////////////////////

decIntSet::decIntSet(){
	pValues = NULL;
	pValueCount = 0;
	pValueSize = 0;
}

decIntSet::decIntSet( int capacity ){
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

decIntSet::decIntSet( const decIntSet &set ){
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

decIntSet::~decIntSet(){
	if( pValues ){
		delete [] pValues;
	}
}



// Management
///////////////

int decIntSet::GetAt( int index ) const{
	if( index < 0 || index >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pValues[ index ];
}

bool decIntSet::Has( int value ) const{
	int p;
	
	for( p=0; p<pValueCount; p++ ){
		if( value == pValues[ p ] ){
			return true;
		}
	}
	
	return false;
}

void decIntSet::Add( int value ){
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

void decIntSet::AddIfAbsent( int value ){
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

void decIntSet::Remove( int value ){
	int p, position = pIndexOf( value );
	
	if( position == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	for( p=position+1; p<pValueCount; p++ ){
		pValues[ p - 1 ] = pValues[ p ];
	}
	pValueCount--;
}

void decIntSet::RemoveIfPresent( int value ){
	int p, position = pIndexOf( value );
	
	if( position == -1 ){
		return;
	}
	
	for( p=position+1; p<pValueCount; p++ ){
		pValues[ p - 1 ] = pValues[ p ];
	}
	pValueCount--;
}

void decIntSet::RemoveAll(){
	pValueCount = 0;
}



bool decIntSet::Equals( const decIntSet &set ) const{
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



// Operators
//////////////

bool decIntSet::operator==( const decIntSet &set ) const{
	return Equals( set );
}

decIntSet decIntSet::operator+( const decIntSet &set ) const{
	decIntSet nset( pValueCount + set.pValueCount );
	int i;
	
	for( i=0; i<pValueCount; i++ ){
		nset.pValues[ i ] = pValues[ i ];
	}
	
	for( i=0; i<set.pValueCount; i++ ){
		nset.AddIfAbsent( set.pValues[ i ] );
	}
	
	return nset;
}

int decIntSet::operator[]( int position ) const{
	return GetAt( position );
}



decIntSet &decIntSet::operator=( const decIntSet &set ){
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

decIntSet &decIntSet::operator+=( const decIntSet &set ){
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



// Private Functions
//////////////////////

int decIntSet::pIndexOf( int value ) const{
	int p;
	
	for( p=0; p<pValueCount; p++ ){
		if( value == pValues[ p ] ){
			return p;
		}
	}
	
	return -1;
}
