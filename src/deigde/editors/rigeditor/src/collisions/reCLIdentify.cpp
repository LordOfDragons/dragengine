/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
