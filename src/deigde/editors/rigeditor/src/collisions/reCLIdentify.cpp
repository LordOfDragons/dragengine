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

#include <stdlib.h>

#include "reCLIdentify.h"
#include "../rig/reRig.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/shape/reRigShape.h"
#include "../rig/shape/reSelectionShapes.h"
#include "../rigeditor.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Class reCLIdentify
///////////////////////

// Constructor, destructor
////////////////////////////

reCLIdentify::reCLIdentify( reRig *rig ){
	if( ! rig ) DETHROW( deeInvalidParam );
	
	pRig = rig;
	pBone = NULL;
	pShape = NULL;
}

reCLIdentify::~reCLIdentify(){
}



// Management
///////////////

void reCLIdentify::Reset(){
	pBone = NULL;
	pShape = NULL;
}

bool reCLIdentify::HasBone() const{
	return pBone != NULL;
}

bool reCLIdentify::HasShape() const{
	return pShape != NULL;
}



// Notifications
//////////////////

void reCLIdentify::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( info->IsCollider() ){
		deCollider *collider = info->GetCollider();
		deColliderVisitorIdentify identify;
		
		collider->Visit( identify );
		if( identify.IsVolume() ){
			pBone = NULL;
			pShape = pGetShapeFromCollider( &identify.CastToVolume() );
		}
	}
}

bool reCLIdentify::CanHitCollider( deCollider *owner, deCollider *collider ){
	/*
	meObject *object = pGetObjectForCollider( collider );
	if( object ) return object->GetVisible();
	return false;
	*/
	return true;
}

void reCLIdentify::ColliderChanged( deCollider *owner ){
}



// Private Functions
//////////////////////

reRigBone *reCLIdentify::pGetBoneFromCollider( deCollider *collider ) const{
	return NULL; // TODO
}

reRigShape *reCLIdentify::pGetShapeFromCollider( deColliderVolume *collider ) const{
	int b, boneCount = pRig->GetBoneCount();
	reRigShape *shape = NULL;
	
	// check if a rig shape belongs to this collider
	shape = pRig->GetShapeWith( collider );
	if( shape ) return shape;
	
	// check if a bone shape belongs to this collider
	for( b=0; b<boneCount; b++ ){
		shape = pRig->GetBoneAt( b )->GetShapeWith( collider );
		if( shape ) return shape;
	}
	
	// otherwise no shape belongs to this collider
	return NULL;
}
