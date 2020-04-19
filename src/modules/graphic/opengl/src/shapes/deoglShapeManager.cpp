/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <stdlib.h>

#include "deoglShape.h"
#include "deoglShapeManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglShapeManager
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShapeManager::deoglShapeManager(){
	pShapes = NULL;
	pShapeCount = 0;
	pShapeSize = 0;
}

deoglShapeManager::~deoglShapeManager(){
	pCleanUp();
}



// Management
///////////////

deoglShape *deoglShapeManager::GetShapeAt( int index ) const{
	if( index < 0 || index >= pShapeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pShapes[ index ];
}

void deoglShapeManager::AddShape( deoglShape *shape ){
	if( ! shape ){
		DETHROW( deeInvalidParam );
	}
	
	if( pShapeCount == pShapeSize ){
		int i, newSize = pShapeSize * 3 / 2 + 1;
		deoglShape **newArray = new deoglShape*[ newSize ];
		if( pShapes ){
			for( i=0; i<pShapeSize; i++ ) newArray[ i ] = pShapes[ i ];
			delete [] pShapes;
		}
		pShapes = newArray;
		pShapeSize = newSize;
	}
	pShapes[ pShapeCount ] = shape;
	pShapeCount++;
}

void deoglShapeManager::RemoveAllShapes(){
	while( pShapeCount > 0 ){
		delete pShapes[ pShapeCount - 1 ];
		pShapeCount--;
	}
}



// Private Functions
//////////////////////

void deoglShapeManager::pCleanUp(){
	RemoveAllShapes();
	if( pShapes ){
		delete [] pShapes;
	}
}
