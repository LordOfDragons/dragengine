/* 
 * Drag[en]gine IGDE Rig Editor
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
#include <stdio.h>
#include "reRigShape.h"
#include "reRigShapeList.h"
#include "dragengine/common/exceptions.h"



// Class reRigShapeList
/////////////////////////

// Constructor, destructor
////////////////////////////

reRigShapeList::reRigShapeList(){
	pShapes = NULL;
	pShapeCount = 0;
	pShapeSize = 0;
}

reRigShapeList::~reRigShapeList(){
	RemoveAllShapes();
	if( pShapes ) delete [] pShapes;
}



// Management
///////////////

reRigShape *reRigShapeList::GetShapeAt( int index ) const{
	if( index < 0 || index >= pShapeCount ) DETHROW( deeInvalidParam );
	
	return pShapes[ index ];
}

int reRigShapeList::IndexOfShape( reRigShape *shape ) const{
	if( ! shape ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pShapeCount; i++ ){
		if( shape == pShapes[ i ] ) return i;
	}
	
	return -1;
}

bool reRigShapeList::HasShape( reRigShape *shape ) const{
	if( ! shape ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pShapeCount; i++ ){
		if( shape == pShapes[ i ] ) return true;
	}
	
	return false;
}

void reRigShapeList::AddShape( reRigShape *shape ){
	if( HasShape( shape ) ) DETHROW( deeInvalidParam );
	
	if( pShapeCount == pShapeSize ){
		int newSize = pShapeSize * 3 / 2 + 1;
		reRigShape **newArray = new reRigShape*[ newSize ];
		if( pShapes ){
			memcpy( newArray, pShapes, sizeof( reRigShape* ) * pShapeSize );
			delete [] pShapes;
		}
		pShapes = newArray;
		pShapeSize = newSize;
	}
	
	pShapes[ pShapeCount ] = shape;
	pShapeCount++;
	
	shape->AddReference();
}

void reRigShapeList::RemoveShape( reRigShape *shape ){
	int i, index = IndexOfShape( shape );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pShapeCount; i++ ){
		pShapes[ i - 1 ] = pShapes[ i ];
	}
	pShapeCount--;
	
	shape->FreeReference();
}

void reRigShapeList::RemoveAllShapes(){
	while( pShapeCount > 0 ){
		pShapeCount--;
		pShapes[ pShapeCount ]->FreeReference();
	}
}
