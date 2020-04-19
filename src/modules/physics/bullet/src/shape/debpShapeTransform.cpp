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

#include "debpShapeTransform.h"
#include "debpShapeSphere.h"
#include "debpShapeBox.h"
#include "debpShapeCylinder.h"
#include "debpShapeCapsule.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeTransform
/////////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeTransform::debpShapeTransform(){
}

debpShapeTransform::~debpShapeTransform(){
}



// Management
///////////////

const debpDCollisionVolume &debpShapeTransform::TransformBase( const debpShape *shape, const decDMatrix &transformation ){
	switch( ( debpShape::eShapeTypes )shape->GetType() ){
	case debpShape::estSphere:
		return TransformShapeSphere( *( ( ( debpShapeSphere* )shape )->GetShapeSphere() ), transformation );
		
	case debpShape::estBox:
		return TransformShapeBox( *( ( ( debpShapeBox* )shape )->GetShapeBox() ), transformation );
		
	case debpShape::estCylinder:
		return TransformShapeCylinder( *( ( ( debpShapeCylinder* )shape )->GetShapeCylinder() ), transformation );
		
	case debpShape::estCapsule:
		return TransformShapeCapsule( *( ( ( debpShapeCapsule* )shape )->GetShapeCapsule() ), transformation );
	}
	
	DETHROW( deeInvalidParam );
}

const debpDCollisionVolume& debpShapeTransform::TransformCurrent( const debpShape *shape, const decDMatrix &transformation ){
	switch( ( debpShape::eShapeTypes )shape->GetType() ){
	case debpShape::estSphere:
		return TransformCollisionSphere( ( ( debpShapeSphere* )shape )->GetCollisionSphere(), transformation );
		
	case debpShape::estBox:
		return TransformCollisionBox( ( ( debpShapeBox* )shape )->GetCollisionBox(), transformation );
		
	case debpShape::estCylinder:
		return TransformCollisionCylinder( ( ( debpShapeCylinder* )shape )->GetCollisionCylinder(), transformation );
		
	case debpShape::estCapsule:
		return TransformCollisionCapsule( ( ( debpShapeCapsule* )shape )->GetCollisionCapsule(), transformation );
	}
	
	DETHROW( deeInvalidParam );
}



const debpDCollisionSphere &debpShapeTransform::TransformShapeSphere( const decShapeSphere &sphere, const decDMatrix &transformation ){
	pSphere.SetCenter( transformation * sphere.GetPosition() );
	pSphere.SetRadius( sphere.GetRadius() );
	return pSphere;
}

const debpDCollisionBox &debpShapeTransform::TransformShapeBox( const decShapeBox &box, const decDMatrix &transformation ){
	pBox.SetCenter( transformation * box.GetPosition() );
	pBox.SetOrientation( box.GetOrientation() * transformation.ToQuaternion() );
	pBox.SetHalfSize( box.GetHalfExtends() );
	return pBox;
}

const debpDCollisionCylinder &debpShapeTransform::TransformShapeCylinder( const decShapeCylinder &cylinder, const decDMatrix &transformation ){
	pCylinder.SetPosition( transformation * cylinder.GetPosition() );
	pCylinder.SetOrientation( cylinder.GetOrientation() * transformation.ToQuaternion() );
	pCylinder.SetHalfHeight( cylinder.GetHalfHeight() );
	pCylinder.SetTopRadius( cylinder.GetTopRadius() );
	pCylinder.SetBottomRadius( cylinder.GetBottomRadius() );
	return pCylinder;
}

const debpDCollisionCapsule &debpShapeTransform::TransformShapeCapsule( const decShapeCapsule &capsule, const decDMatrix &transformation ){
	pCapsule.SetPosition( transformation * capsule.GetPosition() );
	pCapsule.SetOrientation( capsule.GetOrientation() * transformation.ToQuaternion() );
	pCapsule.SetHalfHeight( capsule.GetHalfHeight() );
	pCapsule.SetTopRadius( capsule.GetTopRadius() );
	pCapsule.SetBottomRadius( capsule.GetBottomRadius() );
	return pCapsule;
}



const debpDCollisionSphere &debpShapeTransform::TransformCollisionSphere( const debpDCollisionSphere &sphere, const decDMatrix &transformation ){
	pSphere.SetCenter( transformation * sphere.GetCenter() );
	pSphere.SetRadius( sphere.GetRadius() );
	return pSphere;
}

const debpDCollisionBox &debpShapeTransform::TransformCollisionBox( const debpDCollisionBox &box, const decDMatrix &transformation ){
	pBox.SetCenter( transformation * box.GetCenter() );
	pBox.SetOrientation( box.GetOrientation() * transformation.ToQuaternion() );
	pBox.SetHalfSize( box.GetHalfSize() );
	return pBox;
}

const debpDCollisionCylinder &debpShapeTransform::TransformCollisionCylinder( const debpDCollisionCylinder &cylinder, const decDMatrix &transformation ){
	pCylinder.SetPosition( transformation * cylinder.GetPosition() );
	pCylinder.SetOrientation( cylinder.GetOrientation() * transformation.ToQuaternion() );
	pCylinder.SetHalfHeight( cylinder.GetHalfHeight() );
	pCylinder.SetTopRadius( cylinder.GetTopRadius() );
	pCylinder.SetBottomRadius( cylinder.GetBottomRadius() );
	return pCylinder;
}

const debpDCollisionCapsule &debpShapeTransform::TransformCollisionCapsule( const debpDCollisionCapsule &capsule, const decDMatrix &transformation ){
	pCapsule.SetPosition( transformation * capsule.GetPosition() );
	pCapsule.SetOrientation( capsule.GetOrientation() * transformation.ToQuaternion() );
	pCapsule.SetHalfHeight( capsule.GetHalfHeight() );
	pCapsule.SetTopRadius( capsule.GetTopRadius() );
	pCapsule.SetBottomRadius( capsule.GetBottomRadius() );
	return pCapsule;
}
