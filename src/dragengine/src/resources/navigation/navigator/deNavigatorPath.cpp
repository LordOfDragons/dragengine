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

#include "deNavigatorPath.h"
#include "../../../common/exceptions.h"



// Class deNavigatorPath
//////////////////////////

// Constructor, destructor
////////////////////////////

deNavigatorPath::deNavigatorPath() :
pPoints( NULL ),
pCount( 0 ),
pSize( 0 ){
}

deNavigatorPath::deNavigatorPath( const deNavigatorPath &path ) :
pPoints( NULL ),
pCount( 0 ),
pSize( 0 )
{
	if( path.pCount == 0 ){
		return;
	}
	
	pPoints = new decDVector[ path.pCount ];
	memcpy( pPoints, path.pPoints, sizeof( decDVector ) * path.pCount );
	pCount = path.pCount;
	pSize = path.pCount;
}

deNavigatorPath::~deNavigatorPath(){
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

const decDVector &deNavigatorPath::GetAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return pPoints[ index ];
}

void deNavigatorPath::SetAt( int index, const decDVector &position ){
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	pPoints[ index ] = position;
}

void deNavigatorPath::Add( const decDVector &point ){
	if( pCount == pSize ){
		const int newSize = pCount + 10;
		decDVector * const newArray = new decDVector[ newSize ];
		if( pPoints ){
			memcpy( newArray, pPoints, sizeof( decDVector ) * pCount );
			delete [] pPoints;
		}
		pPoints = newArray;
		pSize = newSize;
	}
	
	pPoints[ pCount++ ] = point;
}

void deNavigatorPath::AddPath( const deNavigatorPath &path ){
	int i;
	for( i=0; i<path.pCount; i++ ){
		Add( path.pPoints[ i ] );
	}
}

void deNavigatorPath::RemoveFrom( int index ){
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=index+1; i<pCount; i++ ){
		pPoints[ i - 1 ] = pPoints[ i ];
	}
	
	pCount--;
}

void deNavigatorPath::RemoveAll(){
	pCount = 0;
}



// Operators
//////////////

deNavigatorPath &deNavigatorPath::operator=( const deNavigatorPath &path ){
	RemoveAll();
	AddPath( path );
	return *this;
}
