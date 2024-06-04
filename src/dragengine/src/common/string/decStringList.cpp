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
#include "decStringList.h"
#include "../exceptions.h"
#include "../../dragengine_configuration.h"



// Class decStringList
////////////////////////

// Constructor, destructor
////////////////////////////

decStringList::decStringList(){
	pStrings = NULL;
	pStringCount = 0;
	pStringSize = 0;
}

decStringList::decStringList( const decStringList &list ){
	const int count = list.GetCount();
	
	pStrings = NULL;
	pStringCount = 0;
	pStringSize = 0;
	
	if( count > 0 ){
		pStrings = new decString*[ count ];
		pStringSize = count;
		
		while( pStringCount < count ){
			pStrings[ pStringCount ] = new decString( list.GetAt( pStringCount ) );
			pStringCount++;
		}
	}
}

decStringList::~decStringList(){
	RemoveAll();
	if( pStrings ){
		delete [] pStrings;
	}
}



// Management
///////////////

const decString &decStringList::GetAt( int index ) const{
	if( index < 0 || index >= pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	return *pStrings[ index ];
}

void decStringList::SetAt( int index, const decString &string ){
	if( index < 0 || index >= pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	*pStrings[ index ] = string;
}

void decStringList::SetAt( int index, const char *string ){
	if( index < 0 || index >= pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	*pStrings[ index ] = string;
}

int decStringList::IndexOf( const decString &string ) const{
	int s;
	
	for( s=0; s<pStringCount; s++ ){
		if( pStrings[ s ]->Equals( string ) ){
			return s;
		}
	}
	
	return -1;
}

int decStringList::IndexOf( const char *string ) const{
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

bool decStringList::Has( const decString &string ) const{
	return IndexOf( string ) != -1;
}

bool decStringList::Has( const char *string ) const{
	return IndexOf( string ) != -1;
}

void decStringList::Add( const decString &string ){
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

void decStringList::Add( const char *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
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

void decStringList::InsertAt( const decString &string, int index ){
	if( index < 0 || index > pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
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
	
	for( i=pStringCount; i>index; i-- ){
		pStrings[ i ] = pStrings[ i - 1 ];
	}
	pStrings[ index ] = new decString( string );
	pStringCount++;
}

void decStringList::InsertAt( const char *string, int index ){
	if( index < 0 || index > pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
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
	
	for( i=pStringCount; i>index; i-- ){
		pStrings[ i ] = pStrings[ i - 1 ];
	}
	pStrings[ index ] = new decString( string );
	pStringCount++;
}

void decStringList::Move( int from, int to ){
	if( from < 0 || from >= pStringCount || to < 0 || to >= pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	decString * const tempString = pStrings[ from ];
	int i;
	
	if( to < from ){
		for( i=from; i>to; i-- ){
			pStrings[ i ] = pStrings[ i - 1 ];
		}
		
	}else if( to > from ){
		for( i=from; i<to; i++ ){
			pStrings[ i ] = pStrings[ i + 1 ];
		}
	}
	
	pStrings[ to ] = tempString;
}

void decStringList::RemoveFrom( int index ){
	if( index < 0 || index >= pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	int s;
	
	delete pStrings[ index ];
	
	for( s=index+1; s<pStringCount; s++ ){
		pStrings[ s - 1 ] = pStrings[ s ];
	}
	pStringCount--;
}

void decStringList::RemoveAll(){
	while( pStringCount > 0 ){
		pStringCount--;
		delete pStrings[ pStringCount ];
	}
}

void decStringList::SortAscending(){
	if( pStringCount > 1 ){
		pSortAscending( 0, pStringCount - 1 );
	}
}

void decStringList::SortDescending(){
	if( pStringCount > 1 ){
		pSortDescending( 0, pStringCount - 1 );
	}
}



decStringList decStringList::Splice( int first ) const{
	return Splice( first, pStringCount );
}

decStringList decStringList::Splice( int first, int last ) const{
	if( first < 0 ){
		first = pStringCount + first;
	}
	if( first < 0 || first >= pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( last < 0 ){
		last = pStringCount + last;
	}
	if( last < 0 || last > pStringCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( last <= first ){
		return decStringList();
	}
	
	decStringList list;
	while( first < last ){
		list.Add( *pStrings[ first++ ] );
	}
	return list;
}

decString decStringList::Join( const decString &separator ) const{
	return Join( separator.GetString() );
}

decString decStringList::Join( const char *separator ) const{
	if( ! separator ){
		DETHROW( deeInvalidParam );
	}
	
	const int separatorLength = ( int )strlen( separator );
	decString joined;
	int i;
	
	int requiredLength = 0;
	for( i=0; i<pStringCount; i++ ){
		requiredLength += pStrings[ i ]->GetLength();
	}
	if( pStringCount > 1 ){
		requiredLength += separatorLength * ( pStringCount - 1 );
	}
	
	joined.Set( ' ', requiredLength );
	
	char *next = ( char* )joined.GetString();
	for( i=0; i<pStringCount; i++ ){
		if( i > 0 && separatorLength > 0 ){
			#ifdef OS_W32_VS
				strcpy_s( next, separatorLength + 1, separator );
			#else
				strcpy( next, separator );
			#endif
			next += separatorLength;
		}
		
		const int stringLength = pStrings[ i ]->GetLength();
		
		if( stringLength > 0 ){
			#ifdef OS_W32_VS
				strcpy_s( next, stringLength + 1, pStrings[ i ]->GetString() );
			#else
				strcpy( next, pStrings[ i ]->GetString() );
			#endif
			next += stringLength;
		}
	}
	
	*next = '\0';
	
	return joined;
}



// Operators
//////////////

const decString &decStringList::operator[]( int index ) const{
	return GetAt( index );
}

decStringList &decStringList::operator=( const decStringList &list ){
	const int count = list.GetCount();
	int s;
	
	RemoveAll();
	for( s=0; s<count; s++ ){
		Add( list.GetAt( s ) );
	}
	
	return *this;
}

bool decStringList::operator==( const decStringList &list ) const {
	int i;
	
	if( list.GetCount() != pStringCount ){
		return false;
	}
	
	for( i=0; i<pStringCount; i++ ){
		if( *pStrings[ i ] != list.GetAt( i ) ){
			return false;
		}
	}
	
	return true;
}

bool decStringList::operator!=( const decStringList &list ) const {
	return ! ( *this == list );
}

decStringList decStringList::operator+( const decStringList &list ) const{
	const int count = list.GetCount();
	decStringList newList( *this );
	int s;
	
	for( s=0; s<count; s++ ){
		newList.Add( list.GetAt( s ) );
	}
	
	return newList;
}

decStringList &decStringList::operator+=( const decStringList &list ){
	const int count = list.GetCount();
	int s;
	
	for( s=0; s<count; s++ ){
		Add( list.GetAt( s ) );
	}
	
	return *this;
}

decStringList decStringList::operator+( const decString &string ) const{
	decStringList newList( *this );
	
	newList.Add( string );
	
	return newList;
}

decStringList decStringList::operator+( const char *string ) const{
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	decStringList newList( *this );
	
	newList.Add( string );
	
	return newList;
}

decStringList &decStringList::operator+=( const decString &string ){
	Add( string );
	return *this;
}

decStringList &decStringList::operator+=( const char *string ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	Add( string );
	return *this;
}



// Private Functions
//////////////////////

void decStringList::pSortAscending( int left, int right ){
	decString * const pivot = pStrings[ left ];
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( left < right && *pStrings[ right ] >= *pivot ){
			right--;
		}
		if( left != right ){
			pStrings[ left ] = pStrings[ right ];
			left++;
		}
		while( left < right && *pStrings[ left ] <= *pivot ){
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

void decStringList::pSortDescending( int left, int right ){
	decString * const pivot = pStrings[ left ];
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( left < right && *pStrings[ right ] <= *pivot ){
			right--;
		}
		if( left != right ){
			pStrings[ left ] = pStrings[ right ];
			left++;
		}
		while( left < right && *pStrings[ left ] >= *pivot ){
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
