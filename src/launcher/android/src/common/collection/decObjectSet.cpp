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

#include "decObjectSet.h"

#include "../../deObject.h"
#include "../exceptions.h"



// Class decObjectSet
///////////////////////

// Constructor, destructor
////////////////////////////

decObjectSet::decObjectSet(){
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
}

decObjectSet::decObjectSet( int capacity ){
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

decObjectSet::decObjectSet( const decObjectSet &set ){
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

decObjectSet::~decObjectSet(){
	RemoveAll();
	
	if( pObjects ){
		delete [] pObjects;
	}
}



// Management
///////////////

deObject *decObjectSet::GetAt( int index ) const{
	if( index < 0 || index >= pObjectCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pObjects[ index ];
}

bool decObjectSet::Has( deObject *object ) const{
	int p;
	
	for( p=0; p<pObjectCount; p++ ){
		if( object == pObjects[ p ] ){
			return true;
		}
	}
	
	return false;
}

void decObjectSet::Add( deObject *object ){
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

void decObjectSet::AddIfAbsent( deObject *object ){
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

void decObjectSet::Remove( deObject *object ){
	int p, position = pIndexOf( object );
	
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

void decObjectSet::RemoveIfPresent( deObject *object ){
	int p, position = pIndexOf( object );
	
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

void decObjectSet::RemoveAll(){
	while( pObjectCount > 0 ){
		pObjectCount--;
		if( pObjects[ pObjectCount ] ){
			pObjects[ pObjectCount ]->FreeReference();
		}
	}
}



bool decObjectSet::Equals( const decObjectSet &set ) const{
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



// Operators
//////////////

bool decObjectSet::operator==( const decObjectSet &set ) const{
	return Equals( set );
}

decObjectSet decObjectSet::operator+( const decObjectSet &set ) const{
	decObjectSet nset( pObjectCount + set.pObjectCount );
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

deObject *decObjectSet::operator[]( int position ) const{
	return GetAt( position );
}



decObjectSet &decObjectSet::operator=( const decObjectSet &set ){
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

decObjectSet &decObjectSet::operator+=( const decObjectSet &set ){
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
		
		for( i=0; i<count; i++ ){
			AddIfAbsent( set.pObjects[ i ] );
		}
	}
	
	return *this;
}



// Private Functions
//////////////////////

int decObjectSet::pIndexOf( deObject *object ) const{
	int p;
	
	for( p=0; p<pObjectCount; p++ ){
		if( object == pObjects[ p ] ){
			return p;
		}
	}
	
	return -1;
}
