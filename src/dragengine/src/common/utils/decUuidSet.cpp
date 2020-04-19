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

#include "decUuid.h"
#include "decUuidSet.h"

#include "../exceptions.h"



// Class decUuidSet
/////////////////////

// Constructor, destructor
////////////////////////////

decUuidSet::decUuidSet(){
	pUuids = NULL;
	pUuidCount = 0;
	pUuidSize = 0;
}

decUuidSet::decUuidSet( const decUuidSet &set ){
	const int count = set.GetCount();
	
	pUuids = NULL;
	pUuidCount = 0;
	pUuidSize = 0;
	
	if( count > 0 ){
		pUuids = new decUuid[ count ];
		pUuidSize = count;
		
		while( pUuidCount < count ){
			pUuids[ pUuidCount ] = set.GetAt( pUuidCount );
			pUuidCount++;
		}
	}
}

decUuidSet::~decUuidSet(){
	RemoveAll();
	if( pUuids ){
		delete [] pUuids;
	}
}



// Management
///////////////

const decUuid &decUuidSet::GetAt( int index ) const{
	if( index < 0 || index >= pUuidCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pUuids[ index ];
}

int decUuidSet::IndexOf( const decUuid &uuid ) const{
	int i;
	
	for( i=0; i<pUuidCount; i++ ){
		if( pUuids[ i ] == uuid ){
			return i;
		}
	}
	
	return -1;
}

bool decUuidSet::Has( const decUuid &uuid ) const{
	return IndexOf( uuid ) != -1;
}

void decUuidSet::Add( const decUuid &uuid ){
	if( Has( uuid ) ){
		return;
	}
	
	if( pUuidCount == pUuidSize ){
		const int newSize = pUuidSize * 3 / 2 + 1;
		decUuid * const newArray = new decUuid[ newSize ];
		if( pUuids ){
			memcpy( newArray, pUuids, sizeof( decUuid ) * pUuidSize );
			delete [] pUuids;
		}
		pUuids = newArray;
		pUuidSize = newSize;
	}
	
	pUuids[ pUuidCount ] = uuid;
	pUuidCount++;
}

void decUuidSet::Remove( const decUuid &uuid ){
	const int index = IndexOf( uuid );
	int i;
	
	if( index != -1 ){
		for( i=index+1; i<pUuidCount; i++ ){
			pUuids[ i - 1 ] = pUuids[ i ];
		}
		pUuidCount--;
	}
}

void decUuidSet::RemoveAll(){
	pUuidCount = 0;
}



// Operators
//////////////

const decUuid &decUuidSet::operator[]( int index ) const{
	return GetAt( index );
}

decUuidSet &decUuidSet::operator=( const decUuidSet &set ){
	const int count = set.GetCount();
	int i;
	
	RemoveAll();
	for( i=0; i<count; i++ ){
		Add( set.GetAt( i ) ); // hard coding this call would remove the HasUuid checks
	}
	
	return *this;
}

bool decUuidSet::operator==( const decUuidSet &set ) const{
	int i;
	
	if( set.GetCount() != pUuidCount ){
		return false;
	}
	
	for( i=0; i<pUuidCount; i++ ){
		if( ! set.Has( pUuids[ i ] ) ){
			return false;
		}
	}
	
	return true;
}

bool decUuidSet::operator!=( const decUuidSet &set ) const{
	return ! ( *this == set );
}

decUuidSet decUuidSet::operator+( const decUuidSet &set ) const{
	const int count = set.GetCount();
	decUuidSet newSet( *this );
	int i;
	
	for( i=0; i<count; i++ ){
		newSet.Add( set.GetAt( i ) );
	}
	
	return newSet;
}

decUuidSet &decUuidSet::operator+=( const decUuidSet &set ){
	const int count = set.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		Add( set.GetAt( i ) );
	}
	
	return *this;
}

decUuidSet decUuidSet::operator+( const decUuid &uuid ) const{
	decUuidSet newList( *this );
	
	newList.Add( uuid );
	
	return newList;
}

decUuidSet &decUuidSet::operator+=( const decUuid &uuid ){
	Add( uuid );
	
	return *this;
}
