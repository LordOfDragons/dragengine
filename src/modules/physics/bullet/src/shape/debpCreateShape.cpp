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

#include "debpCreateShape.h"
#include "debpShapeSphere.h"
#include "debpShapeBox.h"
#include "debpShapeCylinder.h"
#include "debpShapeCapsule.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/exceptions.h>



// Class debpCreateShape
//////////////////////////

// Constructor, destructor
////////////////////////////

debpCreateShape::debpCreateShape(){
}

debpCreateShape::~debpCreateShape(){
}



// Management
///////////////

void debpCreateShape::SetCreatedShape( debpShape *shape ){
	pShape = shape;
}

void debpCreateShape::Reset(){
	pShape = NULL;
}



// Visiting
/////////////

void debpCreateShape::VisitShape( decShape& ){
	pShape = NULL;
}

void debpCreateShape::VisitShapeSphere( decShapeSphere &sphere ){
	pShape.TakeOver( new debpShapeSphere( &sphere ) );
}

void debpCreateShape::VisitShapeBox( decShapeBox &box ){
	pShape.TakeOver( new debpShapeBox( &box ) );
}

#include "debpShapeHackBox.h"
void debpCreateShape::VisitShapeCylinder( decShapeCylinder &cylinder ){
	// hack required because of debpCollisionWorld::contactTest with USE_BULLET_COLLISION disabled
	printf( "[TEMPORARY HACK] debpCreateShape::VisitShapeCylinder\n" );
	const float radius = cylinder.GetTopRadius() / 1.414213f;
	pShape.TakeOver( new debpShapeHackBox( new decShapeBox(
		decVector( radius, cylinder.GetHalfHeight(), radius ),
		cylinder.GetPosition(), cylinder.GetOrientation() ) ) );
	
	//pShape.TakeOver( new debpShapeCylinder( &cylinder ) );
}

void debpCreateShape::VisitShapeCapsule( decShapeCapsule &capsule ){
	pShape.TakeOver( new debpShapeCapsule( &capsule ) );
}

void debpCreateShape::VisitShapeHull( decShapeHull &hull ){
	// not supported
	VisitShape( hull );
}
