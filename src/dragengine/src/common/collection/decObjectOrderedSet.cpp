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

#include "decObjectOrderedSet.h"

#include "../../deObject.h"
#include "../exceptions.h"



// Class decObjectOrderedSet
//////////////////////////////

// Constructor, destructor
////////////////////////////

decObjectOrderedSet::decObjectOrderedSet(){
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
}

decObjectOrderedSet::decObjectOrderedSet( int capacity ){
	if( capacity < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	
	if( capacity > 0 ){
		pObjects = new deObject*[ capacity ];
		pObjectSize = capacity;
	}
}

decObjectOrderedSet::decObjectOrderedSet( const decObjectOrderedSet &set ){
	int count = set.GetCount();
	
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	
	if( count > 0 ){
		deObject *object;
		
		pObjects = new deObject*[ count ];
		pObjectSize = count;
		
		for( pObjectCount=0; pObjectCount<count; pObjectCount++ ){
			object = set.pObjects[ pObjectCount ];
			pObjects[ pObjectCount ] = object;
			if( object ){
				object->AddReference();
			}
		}
	}
}

decObjectOrderedSet::~decObjectOrderedSet(){
	RemoveAll();
	
	if( pObjects ){
		delete [] pObjects;
	}
}



// Management
///////////////

deObject *decObjectOrderedSet::GetAt( int index ) const{
	if( index < 0 || index >= pObjectCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pObjects[ index ];
}

int decObjectOrderedSet::IndexOf( deObject *object ) const{
	int p;
	
	for( p=0; p<pObjectCount; p++ ){
		if( object == pObjects[ p ] ){
			return p;
		}
	}
	
	return -1;
}

bool decObjectOrderedSet::Has( deObject *object ) const{
	int p;
	
	for( p=0; p<pObjectCount; p++ ){
		if( object == pObjects[ p ] ){
			return true;
		}
	}
	
	return false;
}

void decObjectOrderedSet::SetAt( int index, deObject *object ){
	if( index < 0 || index >= pObjectCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( object == pObjects[ index ] ){
		return;
	}
	
	if( Has( object ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pObjects[ index ] ){
		pObjects[ index ]->FreeReference();
	}
	
	pObjects[ index ] = object;
	
	if( object ){
		object->AddReference();
	}
}

void decObjectOrderedSet::Add( deObject *object ){
	if( Has( object ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pObjectCount == pObjectSize ){
		int newSize = pObjectSize * 3 / 2 + 1;
		deObject **newArray = new deObject*[ newSize ];
		if( pObjects ){
			memcpy( newArray, pObjects, sizeof( deObject* ) * pObjectSize );
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = newSize;
	}
	
	pObjects[ pObjectCount ] = object;
	if( object ){
		object->AddReference();
	}
	pObjectCount++;
}

void decObjectOrderedSet::AddIfAbsent( deObject *object ){
	if( Has( object ) ){
		return;
	}
	
	if( pObjectCount == pObjectSize ){
		int newSize = pObjectSize * 3 / 2 + 1;
		deObject **newArray = new deObject*[ newSize ];
		if( pObjects ){
			memcpy( newArray, pObjects, sizeof( deObject* ) * pObjectSize );
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = newSize;
	}
	
	pObjects[ pObjectCount ] = object;
	if( object ){
		object->AddReference();
	}
	pObjectCount++;
}

void decObjectOrderedSet::Insert( deObject *object, int index ){
	if( Has( object ) || index < 0 || index > pObjectCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	if( pObjectCount == pObjectSize ){
		int newSize = pObjectSize * 3 / 2 + 1;
		deObject **newArray = new deObject*[ newSize ];
		if( pObjects ){
			memcpy( newArray, pObjects, sizeof( deObject* ) * pObjectSize );
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = newSize;
	}
	
	for( i=pObjectCount; i>index; i-- ){
		pObjects[ i ] = pObjects[ i - 1 ];
	}
	pObjects[ index ] = object;
	if( object ){
		object->AddReference();
	}
	pObjectCount++;
}

void decObjectOrderedSet::Move( deObject *object, int to ){
	const int from = IndexOf( object );
	if( from == -1 || to < 0 || to >= pObjectCount ){
		DETHROW( deeInvalidParam );
	}
	
	deObject * const tempObject = pObjects[ from ];
	int i;
	
	if( to < from ){
		for( i=from; i>to; i-- ){
			pObjects[ i ] = pObjects[ i - 1 ];
		}
		
	}else if( to > from ){
		for( i=from; i<to; i++ ){
			pObjects[ i ] = pObjects[ i + 1 ];
		}
	}
	
	pObjects[ to ] = tempObject;
}

void decObjectOrderedSet::Remove( deObject *object ){
	int p, position = IndexOf( object );
	
	if( position == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pObjects[ position ] ){
		pObjects[ position ]->FreeReference();
	}
	for( p=position+1; p<pObjectCount; p++ ){
		pObjects[ p - 1 ] = pObjects[ p ];
	}
	pObjectCount--;
}

void decObjectOrderedSet::RemoveIfPresent( deObject *object ){
	int p, position = IndexOf( object );
	
	if( position == -1 ){
		return;
	}
	
	if( pObjects[ position ] ){
		pObjects[ position ]->FreeReference();
	}
	for( p=position+1; p<pObjectCount; p++ ){
		pObjects[ p - 1 ] = pObjects[ p ];
	}
	pObjectCount--;
}

void decObjectOrderedSet::RemoveFrom( int index ){
	if( index < 0 || index >= pObjectCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pObjects[ index ] ){
		pObjects[ index ]->FreeReference();
	}
	
	int i;
	for( i=index+1; i<pObjectCount; i++ ){
		pObjects[ i - 1 ] = pObjects[ i ];
	}
	pObjectCount--;
}

void decObjectOrderedSet::RemoveAll(){
	while( pObjectCount > 0 ){
		pObjectCount--;
		if( pObjects[ pObjectCount ] ){
			pObjects[ pObjectCount ]->FreeReference();
		}
	}
}



bool decObjectOrderedSet::Equals( const decObjectOrderedSet &set ) const{
	int p;
	
	if( set.pObjectCount != pObjectCount ){
		return false;
	}
	
	for( p=0; p<pObjectCount; p++ ){
		if( ! set.Has( pObjects[ p ] ) ){
			return false;
		}
	}
	
	return true;
}

decObjectOrderedSet decObjectOrderedSet::GetHead( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pObjectCount ){
		count = pObjectCount;
	}
	if( count == 0 ){
		return decObjectOrderedSet();
	}
	
	decObjectOrderedSet set( count );
	deObject *object;
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
	
	return set;
}

void decObjectOrderedSet::GetHead( decObjectOrderedSet &set, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pObjectCount ){
		count = pObjectCount;
	}
	
	deObject *object;
	
	if( count > set.pObjectSize ){
		deObject **newArray = new deObject*[ count ];
		if( set.pObjects ){
			delete [] set.pObjects;
		}
		set.pObjects = newArray;
		set.pObjectSize = count;
	}
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
}

decObjectOrderedSet decObjectOrderedSet::GetTail( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pObjectCount ){
		count = pObjectCount;
	}
	if( count == 0 ){
		return decObjectOrderedSet();
	}
	
	decObjectOrderedSet set( count );
	int from = pObjectCount - count;
	deObject *object;
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ from + set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
	
	return set;
}

void decObjectOrderedSet::GetTail( decObjectOrderedSet &set, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pObjectCount ){
		count = pObjectCount;
	}
	
	if( count > set.pObjectSize ){
		deObject **newArray = new deObject*[ count ];
		if( set.pObjects ){
			delete [] set.pObjects;
		}
		set.pObjects = newArray;
		set.pObjectSize = count;
	}
	
	int from = pObjectCount - count;
	deObject *object;
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ from + set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
}

decObjectOrderedSet decObjectOrderedSet::GetMiddle( int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pObjectCount ){
		count = pObjectCount - from;
	}
	if( count == 0 ){
		return decObjectOrderedSet();
	}
	
	decObjectOrderedSet set( count );
	deObject *object;
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ from + set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
	
	return set;
	
}

void decObjectOrderedSet::GetMiddle( decObjectOrderedSet &set, int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pObjectCount ){
		count = pObjectCount - from;
	}
	deObject *object;
	
	if( count > set.pObjectSize ){
		deObject **newArray = new deObject*[ count ];
		if( set.pObjects ){
			delete [] set.pObjects;
		}
		set.pObjects = newArray;
		set.pObjectSize = count;
	}
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ from + set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
}

decObjectOrderedSet decObjectOrderedSet::GetSliced( int from, int to, int step ) const{
	// temporary hack: same as middle
	return GetMiddle( from, to );
}

void decObjectOrderedSet::GetSliced( decObjectOrderedSet &set, int from, int to, int step ) const{
	// temporary hack: same as middle
	GetMiddle( set, from, to );
}



// Operators
//////////////

bool decObjectOrderedSet::operator==( const decObjectOrderedSet &set ) const{
	return Equals( set );
}

decObjectOrderedSet decObjectOrderedSet::operator+( const decObjectOrderedSet &set ) const{
	decObjectOrderedSet nset( pObjectCount + set.pObjectCount );
	deObject *object;
	int i;
	
	for( i=0; i<pObjectCount; i++ ){
		object = pObjects[ i ];
		nset.pObjects[ i ] = object;
		if( object ){
			object->AddReference();
		}
	}
	
	for( i=0; i<set.pObjectCount; i++ ){
		nset.AddIfAbsent( set.pObjects[ i ] );
	}
	
	return nset;
}

deObject *decObjectOrderedSet::operator[]( int index ) const{
	return GetAt( index );
}



decObjectOrderedSet &decObjectOrderedSet::operator=( const decObjectOrderedSet &set ){
	if( &set == this ){
		return *this;
	}
	
	deObject *object;
	
	RemoveAll();
	
	if( set.pObjectCount > pObjectSize ){
		deObject **newArray = new deObject*[ set.pObjectCount ];
		if( pObjects ){
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = set.pObjectCount;
	}
	
	for( pObjectCount=0; pObjectCount<set.pObjectCount; pObjectCount++ ){
		object = set.pObjects[ pObjectCount ];
		pObjects[ pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
	
	return *this;
}

decObjectOrderedSet &decObjectOrderedSet::operator+=( const decObjectOrderedSet &set ){
	if( set.pObjectCount > 0 ){
		int i, count = pObjectCount + set.pObjectCount;
		
		if( count > pObjectSize ){
			deObject **newArray = new deObject*[ count ];
			if( pObjects ){
				memcpy( newArray, pObjects, sizeof( deObject* ) * pObjectSize );
				delete [] pObjects;
			}
			pObjects = newArray;
			pObjectSize = count;
		}
		
		for( i=0; i<set.pObjectCount; i++ ){
			AddIfAbsent( set.pObjects[ i ] );
		}
	}
	
	return *this;
}
