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

#include <stdlib.h>
#include <string.h>

#include "dePathList.h"
#include "../common/file/decPath.h"
#include "../common/exceptions.h"



// Class dePathList
/////////////////////

// Constructor, destructor
////////////////////////////

dePathList::dePathList() :
pPath( NULL ),
pPathCount( 0 ),
pPathSize( 0 ){
}

dePathList::dePathList ( const dePathList &list ):
pPath( NULL ),
pPathCount( 0 ),
pPathSize( 0 )
{
	*this = list;
}

dePathList::~dePathList(){
	RemoveAll();
	if( pPath ){
		delete [] pPath;
	}
}




// Management
///////////////

const decPath &dePathList::GetAt( int index ) const{
	if( index < 0 || index >= pPathCount ){
		DETHROW( deeOutOfBoundary );
	}
	
	return *pPath[ index ];
}

int dePathList::IndexOf( const decPath &path ) const{
	int p;
	
	for( p=0; p<pPathCount; p++ ){
		if( path == *pPath[ p ] ){
			return p;
		}
	}
	
	return -1;
}

bool dePathList::Has( const decPath &path ) const{
	int p;
	
	for( p=0; p<pPathCount; p++ ){
		if( path == *pPath[ p ] ){
			return true;
		}
	}
	
	return false;
}

void dePathList::Add( const decPath &path ){
	if( Has( path ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPathCount == pPathSize ){
		const int newSize = pPathSize * 3 / 2 + 1;
		decPath ** const newArray = new decPath*[ newSize ];
		if( pPath ){
			memcpy( newArray, pPath, sizeof( decPath* ) * pPathSize );
			delete [] pPath;
		}
		pPath = newArray;
		pPathSize = newSize;
	}
	
	pPath[ pPathCount ] = new decPath( path );
	
	pPathCount++;
}

void dePathList::AddIfAbsent( const decPath &path ){
	if( Has( path ) ){
		return;
	}
	
	if( pPathCount == pPathSize ){
		const int newSize = pPathSize * 3 / 2 + 1;
		decPath ** const newArray = new decPath*[ newSize ];
		if( pPath ){
			memcpy( newArray, pPath, sizeof( decPath* ) * pPathSize );
			delete [] pPath;
		}
		pPath = newArray;
		pPathSize = newSize;
	}
	
	pPath[ pPathCount ] = new decPath( path );
	
	pPathCount++;
}

void dePathList::Remove( const decPath &path ){
	int i, index = IndexOf( path );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	delete pPath[ index ];
	
	for( i=index+1; i<pPathCount; i++ ){
		pPath[ i - 1 ] = pPath[ i ];
	}
	pPathCount--;
}

void dePathList::RemoveIfPresent( const decPath &path ){
	int i, index = IndexOf( path );
	if( index == -1 ){
		return;
	}
	
	delete pPath[ index ];
	
	for( i=index+1; i<pPathCount; i++ ){
		pPath[ i - 1 ] = pPath[ i ];
	}
	pPathCount--;
}

void dePathList::RemoveAll(){
	while( pPathCount > 0 ){
		pPathCount--;
		delete pPath[ pPathCount ];
	}
}



// Operators
//////////////

dePathList &dePathList::operator=( const dePathList &list ){
	pPathCount = 0;
	
	int i;
	for( i=0; i<list.pPathCount; i++ ){
		Add( *list.pPath[ i ] );
	}
	
	return *this;
}
