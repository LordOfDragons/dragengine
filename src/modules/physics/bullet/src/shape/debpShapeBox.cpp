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

#include "debpShapeBox.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeBox
///////////////////////

// Constructor, destructor
////////////////////////////

debpShapeBox::debpShapeBox( decShapeBox *shape ) : debpShape( estBox, shape ){
	pSBox = shape;
	
	pCBox.SetHalfSize( shape->GetHalfExtends() );
	
	SetCollisionVolume( &pCBox );
}

debpShapeBox::~debpShapeBox(){
}



// Management
///////////////

void debpShapeBox::UpdateWithMatrix( const decDMatrix &transformation ){
	pCBox.SetCenter( transformation * pSBox->GetPosition() );
	pCBox.SetOrientation( pSBox->GetOrientation() * transformation.ToQuaternion() );
}

void debpShapeBox::PrintDebug( dePhysicsBullet &module ){
	const decDVector &c = pCBox.GetCenter();
	const decDVector &h = pCBox.GetHalfSize();
	const decQuaternion &o = pCBox.GetOrientation();
	
	module.LogInfoFormat( "box: c=(%.4g,%.4g,%.4g) h=(%.4g,%.4g,%.4g) o=(%.4g,%.4g,%.4g,%.4g)", c.x, c.y, c.z, h.x, h.y, h.z, o.x, o.y, o.z, o.w );
}
