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

#include "debpShapeGroupExtends.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionCylinder.h"
#include "../coldet/collision/debpDCollisionCapsule.h"

#include "dragengine/common/shape/decShapeBox.h"
#include "dragengine/common/shape/decShapeSphere.h"
#include "dragengine/common/shape/decShapeCylinder.h"
#include "dragengine/common/shape/decShapeCapsule.h"
#include "dragengine/common/shape/decShapeHull.h"
#include "dragengine/common/exceptions.h"



// Class debpShapeGroupExtends
////////////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeGroupExtends::debpShapeGroupExtends(){
	pExtendsNotSetYet = true;
}

debpShapeGroupExtends::~debpShapeGroupExtends(){
}



// Management
///////////////

void debpShapeGroupExtends::Reset(){
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	pExtendsNotSetYet = true;
}

void debpShapeGroupExtends::SetRotation( const decQuaternion &rotation ){
	pMatrix.SetFromQuaternion( rotation );
}

void debpShapeGroupExtends::SetReferencePoint( const decDVector &referencePoint ){
	pMinExtend = referencePoint;
	pMaxExtend = referencePoint;
	pExtendsNotSetYet = false;
}



// Visiting
/////////////

void debpShapeGroupExtends::VisitShape( decShape &shape ){
}

void debpShapeGroupExtends::VisitShapeSphere( decShapeSphere &sphere ){
	const decDVector &position = sphere.GetPosition();
	float radius = sphere.GetRadius();
	decDVector minExtend, maxExtend;
	
	minExtend.x = position.x - radius;
	minExtend.y = position.y - radius;
	minExtend.z = position.z - radius;
	
	maxExtend.x = position.x + radius;
	maxExtend.y = position.y + radius;
	maxExtend.z = position.z + radius;
	
	pAddExtends( minExtend, maxExtend );
}

void debpShapeGroupExtends::VisitShapeBox( decShapeBox &box ){
	debpDCollisionBox colBox( box.GetPosition(), box.GetHalfExtends(),
		( decDMatrix::CreateFromQuaternion( box.GetOrientation() ) * pMatrix ).ToQuaternion() );
	
	pAddExtendsFrom( colBox );
}

void debpShapeGroupExtends::VisitShapeCylinder( decShapeCylinder &cylinder ){
	debpDCollisionCylinder colCylinder( cylinder.GetPosition(), cylinder.GetHalfHeight(),
		cylinder.GetTopRadius(), cylinder.GetBottomRadius(),
		( decDMatrix::CreateFromQuaternion( cylinder.GetOrientation() ) * pMatrix ).ToQuaternion() );
	
	pAddExtendsFrom( colCylinder );
}

void debpShapeGroupExtends::VisitShapeCapsule( decShapeCapsule &capsule ){
	debpDCollisionCapsule colCapsule( capsule.GetPosition(), capsule.GetHalfHeight(),
		capsule.GetTopRadius(), capsule.GetBottomRadius(),
		( decDMatrix::CreateFromQuaternion( capsule.GetOrientation() ) * pMatrix ).ToQuaternion() );
	
	pAddExtendsFrom( colCapsule );
}

void debpShapeGroupExtends::VisitShapeHull( decShapeHull &hull ){
	const int count = hull.GetPointCount();
	int i;
	for( i=0; i<count; i++ ){
		const decVector &point = hull.GetPointAt( i );
		pAddExtends( point, point );
	}
}



// Private Functions
//////////////////////

void debpShapeGroupExtends::pAddExtends( const decDVector &minExtend, const decDVector &maxExtend ){
	if( pExtendsNotSetYet ){
		pMinExtend = minExtend;
		pMaxExtend = maxExtend;
		pExtendsNotSetYet = false;
		
	}else{
		if( minExtend.x < pMinExtend.x ){
			pMinExtend.x = minExtend.x;
		}
		if( maxExtend.x > pMaxExtend.x ){
			pMaxExtend.x = maxExtend.x;
		}
		
		if( minExtend.y < pMinExtend.y ){
			pMinExtend.y = minExtend.y;
		}
		if( maxExtend.y > pMaxExtend.y ){
			pMaxExtend.y = maxExtend.y;
		}
		
		if( minExtend.z < pMinExtend.z ){
			pMinExtend.z = minExtend.z;
		}
		if( maxExtend.z > pMaxExtend.z ){
			pMaxExtend.z = maxExtend.z;
		}
	}
}

void debpShapeGroupExtends::pAddExtendsFrom( debpDCollisionVolume &volume ){
	debpDCollisionBox extendsBox;
	
	volume.GetEnclosingBox( &extendsBox );
	
	pAddExtends( extendsBox.GetCenter() - extendsBox.GetHalfSize(), extendsBox.GetCenter() + extendsBox.GetHalfSize() );
}
