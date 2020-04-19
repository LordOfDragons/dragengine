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

#include "decPointerSet.h"

#include "../exceptions.h"



// Class decPointerSet
////////////////////////

// Constructor, destructor
////////////////////////////

decPointerSet::decPointerSet(){
	pPointers = NULL;
	pPointerCount = 0;
	pPointerSize = 0;
}

decPointerSet::decPointerSet( int capacity ){
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

decPointerSet::decPointerSet( const decPointerSet &set ){
	const int count = set.GetCount();
	
	pPointers = NULL;
	pPointerCount = 0;
	pPointerSize = 0;
	
	if( count > 0 ){
		pPointers = new void*[ count ];
		pPointerSize = count;
		
		memcpy( pPointers, set.pPointers, sizeof( void* ) * count );
		pPointerCount = count;
	}
}

decPointerSet::~decPointerSet(){
	if( pPointers ){
		delete [] pPointers;
	}
}



// Management
///////////////

void *decPointerSet::GetAt( int position ) const{
	if( position < 0 || position >= pPointerCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPointers[ position ];
}

bool decPointerSet::Has( void *pointer ) const{
	int p;
	
	for( p=0; p<pPointerCount; p++ ){
		if( pointer == pPointers[ p ] ){
			return true;
		}
	}
	
	return false;
}

void decPointerSet::Add( void *pointer ){
	if( Has( pointer ) ){
		DETHROW( deeInvalidParam );
	}
	
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

void decPointerSet::AddIfAbsent( void *pointer ){
	if( Has( pointer ) ){
		return;
	}
	
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

void decPointerSet::Remove( void *pointer ){
	const int position = pIndexOf( pointer );
	
	if( position == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pPointerCount--;
	if( position < pPointerCount ){
		pPointers[ position ] = pPointers[ pPointerCount ];
	}
}

void decPointerSet::RemoveIfPresent( void *pointer ){
	const int position = pIndexOf( pointer );
	
	if( position == -1 ){
		return;
	}
	
	pPointerCount--;
	if( position < pPointerCount ){
		pPointers[ position ] = pPointers[ pPointerCount ];
	}
}

void decPointerSet::RemoveAll(){
	pPointerCount = 0;
}



bool decPointerSet::Equals( const decPointerSet &set ) const{
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



// Operators
//////////////

bool decPointerSet::operator==( const decPointerSet &set ) const{
	return Equals( set );
}

decPointerSet decPointerSet::operator+( const decPointerSet &set ) const{
	decPointerSet nset( pPointerCount + set.pPointerCount );
	int p;
	
	memcpy( nset.pPointers, pPointers, sizeof( void* ) * pPointerCount );
	
	for( p=0; p<set.pPointerCount; p++ ){
		nset.AddIfAbsent( set.GetAt( p ) );
	}
	
	return nset;
}

void *decPointerSet::operator[]( int position ) const{
	return GetAt( position );
}



decPointerSet &decPointerSet::operator=( const decPointerSet &set ){
	pPointerCount = 0;
	
	if( set.pPointerCount > pPointerSize ){
		void ** const newArray = new void*[ set.pPointerCount ];
		if( pPointers ){
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = set.pPointerCount;
	}
	
	if( set.pPointerCount > 0 ){
		memcpy( pPointers, set.pPointers, sizeof( void* ) * set.pPointerCount );
		pPointerCount = set.pPointerCount;
	}
	
	return *this;
}

decPointerSet &decPointerSet::operator+=( const decPointerSet &set ){
	if( set.pPointerCount > 0 ){
		int p;
		
		if( pPointerCount + set.pPointerCount > pPointerSize ){
			const int newSize = pPointerCount + set.pPointerCount;
			void ** const newArray = new void*[ newSize ];
			if( pPointers ){
				memcpy( newArray, pPointers, sizeof( void* ) * pPointerSize );
				delete [] pPointers;
			}
			pPointers = newArray;
			pPointerSize = newSize;
		}
		
		for( p=0; p<set.pPointerCount; p++ ){
			AddIfAbsent( set.GetAt( p ) );
		}
	}
	
	return *this;
}



// Private Functions
//////////////////////

int decPointerSet::pIndexOf( void *pointer ) const{
	int p;
	
	for( p=0; p<pPointerCount; p++ ){
		if( pointer == pPointers[ p ] ){
			return p;
		}
	}
	
	return -1;
}
