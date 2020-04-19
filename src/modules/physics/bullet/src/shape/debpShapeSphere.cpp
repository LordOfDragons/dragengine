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

#include "debpShapeSphere.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeSphere
//////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeSphere::debpShapeSphere( decShapeSphere *shape ) : debpShape( estSphere, shape ){
	pSSphere = shape;
	
	pCSphere.SetRadius( shape->GetRadius() );
	
	SetCollisionVolume( &pCSphere );
}

debpShapeSphere::~debpShapeSphere(){
}



// Management
///////////////

void debpShapeSphere::UpdateWithMatrix( const decDMatrix &transformation ){
	pCSphere.SetCenter( transformation * pSSphere->GetPosition() );
}

void debpShapeSphere::PrintDebug( dePhysicsBullet &module ){
	const decDVector &c = pCSphere.GetCenter();
	double r = pCSphere.GetRadius();
	
	module.LogInfoFormat( "sphere: c=(%.4g,%.4g,%.4g) r=%.4g", c.x, c.y, c.z, r );
}
