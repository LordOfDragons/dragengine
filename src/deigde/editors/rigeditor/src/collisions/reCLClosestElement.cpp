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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reCLClosestElement.h"
#include "../rig/reRig.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/constraint/reRigConstraint.h"
#include "../rig/shape/reRigShape.h"
#include "../rig/push/reRigPush.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Class reCLClosestElement
/////////////////////////////

// Constructor, destructor
////////////////////////////

reCLClosestElement::reCLClosestElement( reRig &rig ) :
pRig( rig ),

pTestSimRig( false ),
pTestBones( false ),
pTestShapes( false ),
pTestConstraints( false ),
pTestPushes( false ),

pHitDistance( 0.0f ),
pHitSimRig( false ),
pHitSimBone( NULL ),
pHitBone( NULL ),
pHitShape( NULL ),
pHitConstraint( NULL ),
pHitPush( NULL ),
pHasHit( false ){
}

reCLClosestElement::~reCLClosestElement(){
}



// Management
///////////////

void reCLClosestElement::SetTestSimRig( bool test ){
	pTestSimRig = test;
}

void reCLClosestElement::SetTestBones( bool test ){
	pTestBones = test;
}

void reCLClosestElement::SetTestShapes( bool test ){
	pTestShapes = test;
}

void reCLClosestElement::SetTestConstraints( bool test ){
	pTestConstraints = test;
}

void reCLClosestElement::SetTestPushes( bool test ){
	pTestPushes = test;
}



void reCLClosestElement::Reset(){
	pHitDistance = 0.0f;
	pHitNormal.SetZero();
	
	pHitSimRig = false;
	pHitSimBone = NULL;
	pHitBone = NULL;
	pHitShape = NULL;
	pHitConstraint = NULL;
	pHitPush = NULL;
	
	pHasHit = false;
}



// Notifications
//////////////////

void reCLClosestElement::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( ! info->IsCollider() ){
		return;
	}
	if( pHasHit && info->GetDistance() >= pHitDistance ){
		return;
	}
	
	deCollider * const collider = info->GetCollider();
	
	if( pTestSimRig ){
		if( collider == ( deCollider* )pRig.GetEngineSimulationCollider() ){
			const int boneCount = pRig.GetBoneCount();
			const int bone = info->GetBone();
			
			pInitResult( info );
			
			if( bone >= 0 && bone < boneCount ){
				pHitSimBone = pRig.GetBoneAt( bone );
				
			}else{
				pHitSimRig = true;
			}
			
			return;
		}
	}
	
	if( pTestBones ){
		const int boneCount = pRig.GetBoneCount();
		int i;
		
		for( i=0; i<boneCount; i++ ){
			reRigBone * const bone = pRig.GetBoneAt( i );
			if( collider != ( deCollider* )bone->GetCollider() ){
				continue;
			}
			
			pInitResult( info );
			pHitBone = bone;
			return;
		}
	}
	
	if( pTestShapes ){
		const int rigShapeCount = pRig.GetShapeCount();
		const int boneCount = pRig.GetBoneCount();
		int i, j;
		
		for( i=0; i< rigShapeCount; i++ ){
			reRigShape * const shape = pRig.GetShapeAt( i );
			if( collider != ( deCollider* )shape->GetCollider() ){
				continue;
			}
			
			pInitResult( info );
			pHitShape = shape;
			return;
		}
		
		for( i=0; i<boneCount; i++ ){
			reRigBone * const bone = pRig.GetBoneAt( i );
			const int boneShapeCount = bone->GetShapeCount();
			
			for( j=0; j<boneShapeCount; j++ ){
				reRigShape * const shape = bone->GetShapeAt( j );
				if( collider != ( deCollider* )shape->GetCollider() ){
					continue;
				}
				
				pInitResult( info );
				pHitShape = shape;
				return;
			}
		}
	}
	
	if( pTestConstraints ){
		const int rigConstraintCount = pRig.GetConstraintCount();
		const int boneCount = pRig.GetBoneCount();
		int i, j;
		
		for( i=0; i<rigConstraintCount; i++ ){
			reRigConstraint * const constraint = pRig.GetConstraintAt( i );
			if( collider != ( deCollider* )constraint->GetCollider() ){
				continue;
			}
			
			pInitResult( info );
			pHitConstraint = constraint;
			return;
		}
		
		for( i=0; i<boneCount; i++ ){
			reRigBone * const bone = pRig.GetBoneAt( i );
			const int boneConstraintCount = bone->GetConstraintCount();
			
			for( j=0; j<boneConstraintCount; j++ ){
				reRigConstraint * const constraint = bone->GetConstraintAt( j );
				if( collider != ( deCollider* )constraint->GetCollider() ){
					continue;
				}
				
				pInitResult( info );
				pHitConstraint = constraint;
				return;
			}
		}
	}
	
	if( pTestPushes ){
		const int pushCount = pRig.GetPushCount();
		int i;
		
		for( i=0; i<pushCount; i++ ){
			reRigPush * const push = pRig.GetPushAt( i );
			if( collider != ( deCollider* )push->GetCollider() ){
				continue;
			}
			
			pInitResult( info );
			pHitPush = push;
			return;
		}
	}
}

bool reCLClosestElement::CanHitCollider( deCollider *owner, deCollider *collider ){
	return true;
}

void reCLClosestElement::ColliderChanged( deCollider *owner ){
}



// Private Functions
//////////////////////

void reCLClosestElement::pInitResult( deCollisionInfo *info ){
	Reset();
	
	pHitDistance = info->GetDistance();
	pHitNormal = info->GetNormal();
	
	pHasHit = true;
}
