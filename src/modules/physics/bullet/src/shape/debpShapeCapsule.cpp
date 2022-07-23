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

#include "debpShapeCapsule.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeCapsule
///////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeCapsule::debpShapeCapsule( decShapeCapsule *shape ) : debpShape( estCapsule, shape ){
	pSCapsule = shape;
	SetCollisionVolume( &pCCapsule );
}

debpShapeCapsule::~debpShapeCapsule(){
}



// Management
///////////////

void debpShapeCapsule::UpdateWithMatrix( const decDMatrix &transformation, const decDVector &scale ){
	pCCapsule.SetPosition( transformation * pSCapsule->GetPosition() );
	pCCapsule.SetOrientation( pSCapsule->GetOrientation() * transformation.ToQuaternion() );
	
	const float scaleRadius = ( float )( ( scale.x + scale.z ) * 0.5 );
	pCCapsule.SetHalfHeight( pSCapsule->GetHalfHeight() * ( float )scale.y );
	pCCapsule.SetTopRadius( pSCapsule->GetTopRadius() * scaleRadius );
	pCCapsule.SetBottomRadius( pSCapsule->GetBottomRadius() * scaleRadius );
}

void debpShapeCapsule::PrintDebug( dePhysicsBullet &module ){
	const decDVector &p = pCCapsule.GetPosition();
	double h = pCCapsule.GetHalfHeight();
	double t = pCCapsule.GetTopRadius();
	double b = pCCapsule.GetBottomRadius();
	const decQuaternion &o = pCCapsule.GetOrientation();
	
	module.LogInfoFormat( "capsule: p=(%.4g,%.4g,%.4g) h=%.4g t=%.4g b=%.4g o=(%.4g,%.4g,%.4g,%.4g)", p.x, p.y, p.z, h, t, b, o.x, o.y, o.z, o.w );
}
