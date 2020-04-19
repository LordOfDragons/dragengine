/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpShape.h"
#include "debpShapeList.h"

#include <dragengine/common/exceptions.h>



// Class debpShapeList
////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeList::debpShapeList(){
}

debpShapeList::~debpShapeList(){
}



// Management
///////////////

int debpShapeList::GetShapeCount() const{
	return pShapes.GetCount();
}

debpShape *debpShapeList::GetShapeAt( int index ) const{
	return ( debpShape* )pShapes.GetAt( index );
}

void debpShapeList::AddShape( debpShape *shape ){
	if( ! shape ){
		DETHROW( deeInvalidParam );
	}
	pShapes.Add( shape );
}

void debpShapeList::RemoveAllShapes(){
	pShapes.RemoveAll();
}



void debpShapeList::UpdateWithMatrix( const decDMatrix &transformation ){
	const int count = pShapes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( debpShape* )pShapes.GetAt( i ) )->UpdateWithMatrix( transformation );
	}
}
