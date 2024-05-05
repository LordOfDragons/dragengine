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
