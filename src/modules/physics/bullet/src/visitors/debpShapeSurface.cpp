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

#include "debpShapeSurface.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionCylinder.h"
#include "../coldet/collision/debpDCollisionCapsule.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeSurface
///////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeSurface::debpShapeSurface() :
pUseDirection( false ),
pSurface( 0.0f ){
}

debpShapeSurface::~debpShapeSurface(){
}



// Management
///////////////

void debpShapeSurface::SetDirection( const decVector &direction ){
	pDirection = direction;
	pUseDirection = direction.IsZero();
}

void debpShapeSurface::Reset(){
	pSurface = 0.0f;
}



// Visiting
/////////////

void debpShapeSurface::VisitShape( decShape &shape ){
}

void debpShapeSurface::VisitShapeSphere( decShapeSphere &sphere ){
	pSurface += 4.0f * PI * sphere.GetRadius() * sphere.GetRadius();
}

void debpShapeSurface::VisitShapeBox( decShapeBox &box ){
	pSurface += ( box.GetHalfExtends().x * box.GetHalfExtends().y
		+ box.GetHalfExtends().x * box.GetHalfExtends().z
		+ box.GetHalfExtends().y * box.GetHalfExtends().z ) * 2.0f;
}

void debpShapeSurface::VisitShapeCylinder( decShapeCylinder &cylinder ){
	pSurface += 2.0f * PI * cylinder.GetTopRadius() * ( cylinder.GetHalfHeight() * 2.0f + cylinder.GetTopRadius() );
	// TODO tapered support
}

void debpShapeSurface::VisitShapeCapsule( decShapeCapsule &capsule ){
	// surface += surfaceCylinderMantle + surfaceHalfSphere * 2
	// 
	// thus
	// surface += surfaceCylinderMantle + surfaceSphere
	pSurface += 4.0f * PI * capsule.GetTopRadius() * ( capsule.GetHalfHeight() + capsule.GetTopRadius() );
}

void debpShapeSurface::VisitShapeHull( decShapeHull &hull ) {
	// TODO calculate volume
	// use for the time being the surface of a sphere containing all points
	const int count = hull.GetPointCount();
	if( count == 0 ){
		pSurface = 0.0f;
		return;
	}
	
	decVector center;
	int i;
	
	for( i=0; i<count; i++ ){
		center += hull.GetPointAt( i );
	}
	center /= ( float )count;
	
	float radiusSquared = 0.0f;
	for( i=0; i<count; i++ ){
		radiusSquared = decMath::max( radiusSquared, ( hull.GetPointAt( i ) - center ).LengthSquared() );
	}
	
	pSurface += 4.0f * PI * radiusSquared;
}
