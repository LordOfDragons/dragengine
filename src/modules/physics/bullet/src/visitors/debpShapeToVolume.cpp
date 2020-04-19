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

#include "debpShapeToVolume.h"
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



// Class debpShapeToVolume
////////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeToVolume::debpShapeToVolume(){
	pSphere = NULL;
	pBox = NULL;
	pCylinder = NULL;
	pCapsule = NULL;
	pVolume = NULL;
	
	try{
		pSphere = new debpDCollisionSphere;
		pBox = new debpDCollisionBox;
		pCylinder = new debpDCollisionCylinder;
		pCapsule = new debpDCollisionCapsule;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debpShapeToVolume::~debpShapeToVolume(){
	pCleanUp();
}



// Management
///////////////

void debpShapeToVolume::SetMatrix( const decDMatrix &matrix ){
	pMatrix = matrix;
}



// Convenience Function
/////////////////////////

debpDCollisionVolume *debpShapeToVolume::GetVolumeFor( decShape *shape ){
	if( ! shape ){
		DETHROW( deeInvalidParam );
	}
	
	shape->Visit( *this );
	
	return pVolume;
}



// Visiting
/////////////

void debpShapeToVolume::VisitShape( decShape &shape ){
	pSphere->SetCenter( pMatrix.GetPosition() );
	pSphere->SetRadius( 0.1 );
	
	pVolume = pSphere;
}

void debpShapeToVolume::VisitShapeSphere( decShapeSphere &sphere ){
	pSphere->SetCenter( pMatrix * sphere.GetPosition() );
	pSphere->SetRadius( sphere.GetRadius() );
	
	pVolume = pSphere;
}

void debpShapeToVolume::VisitShapeBox( decShapeBox &box ){
	pBox->SetCenter( pMatrix * box.GetPosition() );
	pBox->SetHalfSize( box.GetHalfExtends() );
	pBox->SetOrientation( ( decDMatrix::CreateFromQuaternion( box.GetOrientation() ) * pMatrix ).ToQuaternion() );
	
	pVolume = pBox;
}

void debpShapeToVolume::VisitShapeCylinder( decShapeCylinder &cylinder ){
	pCylinder->SetPosition( pMatrix * cylinder.GetPosition() );
	pCylinder->SetOrientation( ( decDMatrix::CreateFromQuaternion( cylinder.GetOrientation() ) * pMatrix ).ToQuaternion() );
	pCylinder->SetHalfHeight( cylinder.GetHalfHeight() );
	pCylinder->SetTopRadius( cylinder.GetTopRadius() );
	pCylinder->SetBottomRadius( cylinder.GetBottomRadius() );
	
	pVolume = pCylinder;
}

void debpShapeToVolume::VisitShapeCapsule( decShapeCapsule &capsule ){
	pCapsule->SetPosition( pMatrix * capsule.GetPosition() );
	pCapsule->SetOrientation( ( decDMatrix::CreateFromQuaternion( capsule.GetOrientation() ) * pMatrix ).ToQuaternion() );
	pCapsule->SetHalfHeight( capsule.GetHalfHeight() );
	pCapsule->SetTopRadius( capsule.GetTopRadius() );
	pCapsule->SetBottomRadius( capsule.GetBottomRadius() );
	
	pVolume = pCapsule;
}

void debpShapeToVolume::VisitShapeHull( decShapeHull &hull ) {
	VisitShape( hull );
}



// Private Functions
//////////////////////

void debpShapeToVolume::pCleanUp(){
	if( pCapsule ){
		delete pCapsule;
	}
	if( pCylinder ){
		delete pCylinder;
	}
	if( pBox ){
		delete pBox;
	}
	if( pSphere ){
		delete pSphere;
	}
}
