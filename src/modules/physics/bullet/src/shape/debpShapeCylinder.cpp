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

#include "debpShapeCylinder.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeCylinder
////////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeCylinder::debpShapeCylinder( decShapeCylinder *shape ) : debpShape( estCylinder, shape ){
	pSCylinder = shape;
	
	SetCollisionVolume( &pCCylinder );
}

debpShapeCylinder::~debpShapeCylinder(){
}



// Management
///////////////

void debpShapeCylinder::UpdateWithMatrix( const decDMatrix &transformation, const decDVector &scale ){
	pCCylinder.SetPosition( transformation * pSCylinder->GetPosition() );
	pCCylinder.SetOrientation( pSCylinder->GetOrientation() * transformation.ToQuaternion() );
	
	const float scaleRadius = ( float )( ( scale.x + scale.z ) * 0.5 );
	pCCylinder.SetHalfHeight( pSCylinder->GetHalfHeight() * ( float )scale.y );
	pCCylinder.SetTopRadius( pSCylinder->GetTopRadius() * scaleRadius );
	pCCylinder.SetBottomRadius( pSCylinder->GetBottomRadius() * scaleRadius );
}

void debpShapeCylinder::PrintDebug( dePhysicsBullet &module ){
	const decDVector &p = pCCylinder.GetPosition();
	double h = pCCylinder.GetHalfHeight();
	double t = pCCylinder.GetTopRadius();
	double b = pCCylinder.GetBottomRadius();
	const decQuaternion &o = pCCylinder.GetOrientation();
	
	module.LogInfoFormat( "cylinder: p=(%.4g,%.4g,%.4g) h=%.4g t=%.4g b=%.4g o=(%.4g,%.4g,%.4g,%.4g)", p.x, p.y, p.z, h, t, b, o.x, o.y, o.z, o.w );
}
