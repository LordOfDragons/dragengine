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

#include "reUShapeHullAddPoint.h"
#include "../../../../rig/shape/reRigShapeHull.h"

#include <dragengine/common/exceptions.h>



// Class reUShapeHullAddPoint
///////////////////////////////

// Constructor, destructor
////////////////////////////

reUShapeHullAddPoint::reUShapeHullAddPoint( reRigShapeHull *shape, const decVector &point, int index ) :
pShape( shape ),
pPoint( point ),
pIndex( index )
{
	if( ! shape || index < 0 || index > shape->GetPointCount() ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Shape hull add point" );
}

reUShapeHullAddPoint::~reUShapeHullAddPoint(){
}



// Management
///////////////

void reUShapeHullAddPoint::Undo(){
	( ( reRigShapeHull& )( deObject& )pShape ).RemovePoint( pIndex );
}

void reUShapeHullAddPoint::Redo(){
	( ( reRigShapeHull& )( deObject& )pShape ).InsertPoint( pPoint, pIndex );
}
