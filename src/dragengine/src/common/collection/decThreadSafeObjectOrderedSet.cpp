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

#include "decThreadSafeObjectOrderedSet.h"
#include "../exceptions.h"
#include "../../threading/deThreadSafeObject.h"



// Class decThreadSafeObjectOrderedSet
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

decThreadSafeObjectOrderedSet::decThreadSafeObjectOrderedSet(){
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
}

decThreadSafeObjectOrderedSet::decThreadSafeObjectOrderedSet( int capacity ){
	if( capacity < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	
	if( capacity > 0 ){
		pObjects = new deThreadSafeObject*[ capacity ];
		pObjectSize = capacity;
	}
}

decThreadSafeObjectOrderedSet::decThreadSafeObjectOrderedSet( const decThreadSafeObjectOrderedSet &set ){
	int count = set.GetCount();
	
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	
	if( count > 0 ){
		deThreadSafeObject *object;
		
		pObjects = new deThreadSafeObject*[ count ];
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

decThreadSafeObjectOrderedSet::~decThreadSafeObjectOrderedSet(){
	RemoveAll();
	
	if( pObjects ){
		delete [] pObjects;
	}
}



// Management
///////////////

deThreadSafeObject *decThreadSafeObjectOrderedSet::GetAt( int index ) const{
	if( index < 0 || index >= pObjectCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pObjects[ index ];
}

int decThreadSafeObjectOrderedSet::IndexOf( deThreadSafeObject *object ) const{
	int p;
	
	for( p=0; p<pObjectCount; p++ ){
		if( object == pObjects[ p ] ){
			return p;
		}
	}
	
	return -1;
}

bool decThreadSafeObjectOrderedSet::Has( deThreadSafeObject *object ) const{
	int p;
	
	for( p=0; p<pObjectCount; p++ ){
		if( object == pObjects[ p ] ){
			return true;
		}
	}
	
	return false;
}

void decThreadSafeObjectOrderedSet::SetAt( int index, deThreadSafeObject *object ){
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

void decThreadSafeObjectOrderedSet::Add( deThreadSafeObject *object ){
	if( Has( object ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pObjectCount == pObjectSize ){
		int newSize = pObjectSize * 3 / 2 + 1;
		deThreadSafeObject **newArray = new deThreadSafeObject*[ newSize ];
		if( pObjects ){
			memcpy( newArray, pObjects, sizeof( deThreadSafeObject* ) * pObjectSize );
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

void decThreadSafeObjectOrderedSet::AddIfAbsent( deThreadSafeObject *object ){
	if( Has( object ) ){
		return;
	}
	
	if( pObjectCount == pObjectSize ){
		int newSize = pObjectSize * 3 / 2 + 1;
		deThreadSafeObject **newArray = new deThreadSafeObject*[ newSize ];
		if( pObjects ){
			memcpy( newArray, pObjects, sizeof( deThreadSafeObject* ) * pObjectSize );
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

void decThreadSafeObjectOrderedSet::Insert( deThreadSafeObject *object, int index ){
	if( Has( object ) || index < 0 || index > pObjectCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	if( pObjectCount == pObjectSize ){
		int newSize = pObjectSize * 3 / 2 + 1;
		deThreadSafeObject **newArray = new deThreadSafeObject*[ newSize ];
		if( pObjects ){
			memcpy( newArray, pObjects, sizeof( deThreadSafeObject* ) * pObjectSize );
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

void decThreadSafeObjectOrderedSet::Move( deThreadSafeObject *object, int to ){
	const int from = IndexOf( object );
	if( from == -1 || to < 0 || to >= pObjectCount ){
		DETHROW( deeInvalidParam );
	}
	
	deThreadSafeObject * const tempObject = pObjects[ from ];
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

void decThreadSafeObjectOrderedSet::Remove( deThreadSafeObject *object ){
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

void decThreadSafeObjectOrderedSet::RemoveIfPresent( deThreadSafeObject *object ){
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

void decThreadSafeObjectOrderedSet::RemoveFrom( int index ){
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

void decThreadSafeObjectOrderedSet::RemoveAll(){
	while( pObjectCount > 0 ){
		pObjectCount--;
		if( pObjects[ pObjectCount ] ){
			pObjects[ pObjectCount ]->FreeReference();
		}
	}
}



bool decThreadSafeObjectOrderedSet::Equals( const decThreadSafeObjectOrderedSet &set ) const{
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

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::GetHead( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pObjectCount ){
		count = pObjectCount;
	}
	if( count == 0 ){
		return decThreadSafeObjectOrderedSet();
	}
	
	decThreadSafeObjectOrderedSet set( count );
	deThreadSafeObject *object;
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
	
	return set;
}

void decThreadSafeObjectOrderedSet::GetHead( decThreadSafeObjectOrderedSet &set, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pObjectCount ){
		count = pObjectCount;
	}
	
	deThreadSafeObject *object;
	
	if( count > set.pObjectSize ){
		deThreadSafeObject **newArray = new deThreadSafeObject*[ count ];
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

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::GetTail( int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pObjectCount ){
		count = pObjectCount;
	}
	if( count == 0 ){
		return decThreadSafeObjectOrderedSet();
	}
	
	decThreadSafeObjectOrderedSet set( count );
	int from = pObjectCount - count;
	deThreadSafeObject *object;
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ from + set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
	
	return set;
}

void decThreadSafeObjectOrderedSet::GetTail( decThreadSafeObjectOrderedSet &set, int count ) const{
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count > pObjectCount ){
		count = pObjectCount;
	}
	
	if( count > set.pObjectSize ){
		deThreadSafeObject **newArray = new deThreadSafeObject*[ count ];
		if( set.pObjects ){
			delete [] set.pObjects;
		}
		set.pObjects = newArray;
		set.pObjectSize = count;
	}
	
	int from = pObjectCount - count;
	deThreadSafeObject *object;
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ from + set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
}

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::GetMiddle( int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pObjectCount ){
		count = pObjectCount - from;
	}
	if( count == 0 ){
		return decThreadSafeObjectOrderedSet();
	}
	
	decThreadSafeObjectOrderedSet set( count );
	deThreadSafeObject *object;
	
	for( set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++ ){
		object = pObjects[ from + set.pObjectCount ];
		set.pObjects[ set.pObjectCount ] = object;
		if( object ){
			object->AddReference();
		}
	}
	
	return set;
	
}

void decThreadSafeObjectOrderedSet::GetMiddle( decThreadSafeObjectOrderedSet &set, int from, int to ) const{
	if( from < 0 || to < from ){
		DETHROW( deeInvalidParam );
	}
	
	int count = to - from + 1;
	if( count > pObjectCount ){
		count = pObjectCount - from;
	}
	deThreadSafeObject *object;
	
	if( count > set.pObjectSize ){
		deThreadSafeObject **newArray = new deThreadSafeObject*[ count ];
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

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::GetSliced( int from, int to, int step ) const{
	// temporary hack: same as middle
	return GetMiddle( from, to );
}

void decThreadSafeObjectOrderedSet::GetSliced( decThreadSafeObjectOrderedSet &set, int from, int to, int step ) const{
	// temporary hack: same as middle
	GetMiddle( set, from, to );
}



// Operators
//////////////

bool decThreadSafeObjectOrderedSet::operator==( const decThreadSafeObjectOrderedSet &set ) const{
	return Equals( set );
}

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::operator+( const decThreadSafeObjectOrderedSet &set ) const{
	decThreadSafeObjectOrderedSet nset( pObjectCount + set.pObjectCount );
	deThreadSafeObject *object;
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

deThreadSafeObject *decThreadSafeObjectOrderedSet::operator[]( int index ) const{
	return GetAt( index );
}



decThreadSafeObjectOrderedSet &decThreadSafeObjectOrderedSet::operator=( const decThreadSafeObjectOrderedSet &set ){
	deThreadSafeObject *object;
	
	RemoveAll();
	
	if( set.pObjectCount > pObjectSize ){
		deThreadSafeObject **newArray = new deThreadSafeObject*[ set.pObjectCount ];
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

decThreadSafeObjectOrderedSet &decThreadSafeObjectOrderedSet::operator+=( const decThreadSafeObjectOrderedSet &set ){
	if( set.pObjectCount > 0 ){
		int i, count = pObjectCount + set.pObjectCount;
		
		if( count > pObjectSize ){
			deThreadSafeObject **newArray = new deThreadSafeObject*[ count ];
			if( pObjects ){
				memcpy( newArray, pObjects, sizeof( deThreadSafeObject* ) * pObjectSize );
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
