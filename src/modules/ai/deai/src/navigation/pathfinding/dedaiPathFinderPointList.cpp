/* 
 * Drag[en]gine AI Module
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
#include <string.h>

#include "dedaiPathFinderPointList.h"

#include <dragengine/common/exceptions.h>



// Class dedaiPathFinderPointList
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiPathFinderPointList::dedaiPathFinderPointList(){
	pPoints = NULL;
	pPointCount = 0;
	pPointSize = 0;
}

dedaiPathFinderPointList::~dedaiPathFinderPointList(){
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

const decVector &dedaiPathFinderPointList::GetFirst() const{
	if( pPointCount == 0 ){
		DETHROW( deeInvalidParam );
	}
	return pPoints[ 0 ];
}

const decVector &dedaiPathFinderPointList::GetAt( int index ) const{
	if( index < 0 || index >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	return pPoints[ index ];
}

void dedaiPathFinderPointList::Add( const decVector &point ){
	if( pPointCount == pPointSize ){
		const int newSize = pPointCount + 10;
		decVector *newArray = new decVector[ newSize ];
		if( pPoints ){
			memcpy( newArray, pPoints, sizeof( decVector ) * pPointCount );
			delete [] pPoints;
		}
		pPoints = newArray;
		pPointSize = newSize;
	}
	
	pPoints[ pPointCount++ ] = point;
}

void dedaiPathFinderPointList::RemoveFirst(){
	if( pPointCount == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=1; i<pPointCount; i++ ){
		pPoints[ i - 1 ] = pPoints[ i ];
	}
	pPointCount--;
}

void dedaiPathFinderPointList::RemoveFirst( int count ){
	if( count > 0 ){
		if( count > pPointCount ){
			DETHROW( deeInvalidParam );
		}
		
		int i;
		
		for( i=count; i<pPointCount; i++ ){
			pPoints[ i - count ] = pPoints[ i ];
		}
		pPointCount -= count;
	}
}

void dedaiPathFinderPointList::RemoveFrom( int index ){
	if( index < 0 || index >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=index+1; i<pPointCount; i++ ){
		pPoints[ i - 1 ] = pPoints[ i ];
	}
	pPointCount--;
}

void dedaiPathFinderPointList::RemoveAll(){
	pPointCount = 0;
}



void dedaiPathFinderPointList::Transform( const decMatrix &matrix ){
	int i;
	
	for( i=0; i<pPointCount; i++ ){
		pPoints[ i ] = matrix * pPoints[ i ];
	}
}
