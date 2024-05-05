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

#include "debpClosestConvexResultCallback.h"
#include "../debpCollisionObject.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../terrain/heightmap/debpHeightTerrain.h"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Class debpClosestConvexResultCallback
///////////////////////////////////////////

// Constructors
/////////////////

debpClosestConvexResultCallback::debpClosestConvexResultCallback() :
ClosestConvexResultCallback( btVector3( BT_ZERO, BT_ZERO, BT_ZERO ), btVector3( BT_ZERO, BT_ZERO, BT_ZERO ) ),
pCollider( NULL ),
pColliderVolume( NULL ),
pColliderComponent( NULL ),
pHitHTSector( NULL ),
pHitCollider( NULL ),
pHitBone( -1 ),
pHitShape( -1 ),
pHitFace( -1 ){
}



// Management
///////////////

void debpClosestConvexResultCallback::SetTestCollider( debpCollider *bpCollider ){
	if( ! bpCollider ){
		DETHROW( deeInvalidParam );
	}
	
	// clear the last collider
	pCollider = NULL;
	pColliderVolume = NULL;
	pColliderComponent = NULL;
	
	// determine the type of the new collider and initalize the working parameters
	if( bpCollider->IsVolume() ){
		pCollider = bpCollider;
		pColliderVolume = ( debpColliderVolume* )bpCollider;
		pMoveDirection = pColliderVolume->GetPredictedDisplacement();
		pRotation = pColliderVolume->GetPredictedRotation();
		
		const decDVector &position = pColliderVolume->GetPosition();
		
		m_convexFromWorld.setX( ( btScalar )position.x );
		m_convexFromWorld.setY( ( btScalar )position.y );
		m_convexFromWorld.setZ( ( btScalar )position.z );
		
		m_convexToWorld.setX( ( btScalar )( position.x + pMoveDirection.x ) );
		m_convexToWorld.setY( ( btScalar )( position.y + pMoveDirection.y ) );
		m_convexToWorld.setZ( ( btScalar )( position.z + pMoveDirection.z ) );
		
	}else if( bpCollider->IsComponent() ){
		pCollider = bpCollider;
		pColliderComponent = ( debpColliderComponent* )bpCollider;
		pMoveDirection = pColliderComponent->GetPredictedDisplacement();
		
		const decDVector &position = pColliderComponent->GetPosition();
		
		m_convexFromWorld.setX( ( btScalar )position.x );
		m_convexFromWorld.setY( ( btScalar )position.y );
		m_convexFromWorld.setZ( ( btScalar )position.z );
		
		m_convexToWorld.setX( ( btScalar )( position.x + pMoveDirection.x ) );
		m_convexToWorld.setY( ( btScalar )( position.y + pMoveDirection.y ) );
		m_convexToWorld.setZ( ( btScalar )( position.z + pMoveDirection.z ) );
	}
	
	// reset local collisions flag
	m_closestHitFraction = ( btScalar )1.0;
}

void debpClosestConvexResultCallback::SetTestCollider( debpCollider *bpCollider, const decDVector &displacement ){
	if( ! bpCollider ){
		DETHROW( deeInvalidParam );
	}
	
	// clear the last collider
	pCollider = NULL;
	pColliderVolume = NULL;
	pColliderComponent = NULL;
	
	// common parameters
	pMoveDirection = displacement;
	
	// determine the type of the new collider and initalize the working parameters
	if( bpCollider->IsVolume() ){
		pCollider = bpCollider;
		pColliderVolume = ( debpColliderVolume* )bpCollider;
		
		const decDVector &position = pColliderVolume->GetPosition();
		
		m_convexFromWorld.setX( ( btScalar )position.x );
		m_convexFromWorld.setY( ( btScalar )position.y );
		m_convexFromWorld.setZ( ( btScalar )position.z );
		
		m_convexToWorld.setX( ( btScalar )( position.x + displacement.x ) );
		m_convexToWorld.setY( ( btScalar )( position.y + displacement.y ) );
		m_convexToWorld.setZ( ( btScalar )( position.z + displacement.z ) );
		
	}else if( bpCollider->IsComponent() ){
		pCollider = bpCollider;
		pColliderComponent = ( debpColliderComponent* )bpCollider;
		
		const decDVector &position = pColliderComponent->GetPosition();
		
		m_convexFromWorld.setX( ( btScalar )position.x );
		m_convexFromWorld.setY( ( btScalar )position.y );
		m_convexFromWorld.setZ( ( btScalar )position.z );
		
		m_convexToWorld.setX( ( btScalar )( position.x + displacement.x ) );
		m_convexToWorld.setY( ( btScalar )( position.y + displacement.y ) );
		m_convexToWorld.setZ( ( btScalar )( position.z + displacement.z ) );
	}
	
	// reset local collisions flag
	m_closestHitFraction = ( btScalar )1.0;
}



bool debpClosestConvexResultCallback::MoveDirIsNotZero() const{
	return pMoveDirection.LengthSquared() > 1e-12;
}

bool debpClosestConvexResultCallback::MoveDirIsZero() const{
	return pMoveDirection.LengthSquared() <= 1e-12;
}

bool debpClosestConvexResultCallback::RotationNotZero() const{
	return pRotation.LengthSquared() > 1e-12f;
}

bool debpClosestConvexResultCallback::RotationZero() const{
	return pRotation.LengthSquared() <= 1e-12f;
}



void debpClosestConvexResultCallback::Reset(){
	pHitHTSector = NULL;
	pHitCollider = NULL;
	m_closestHitFraction = ( btScalar )1.0;
	pHitBone = -1;
	pHitShape = -1;
	pHitFace = -1;
}

void debpClosestConvexResultCallback::GetResult( deCollisionInfo &collisionInfo ) const{
	if( hasHit() ){
		if( pHitHTSector ){
			collisionInfo.SetHTSector( pHitHTSector->GetHeightTerrain()->GetHeightTerrain(), pHitHTSector->GetSector() );
			
		}else{
			collisionInfo.SetCollider( pHitCollider, pHitBone, pHitShape, pHitFace );
		}
		
		collisionInfo.SetDistance( ( float )m_closestHitFraction );
		collisionInfo.SetNormal( decVector( m_hitNormalWorld.x(), m_hitNormalWorld.y(), m_hitNormalWorld.z() ) );
		
	}else{
		collisionInfo.Clear();
	}
}



// Bullet
///////////

bool debpClosestConvexResultCallback::needsCollision( btBroadphaseProxy *proxy0 ) const{
	// basic bullet filtering
	if( ! ConvexResultCallback::needsCollision( proxy0 ) ){
		return false;
	}
	
	// determine the collision partner using the custom pointer
	const btCollisionObject &collisionObject = *( ( btCollisionObject* )proxy0->m_clientObject );
	const debpCollisionObject &colObj = *( ( debpCollisionObject* )collisionObject.getUserPointer() );
	deCollider * const engOrgCollider = &pCollider->GetCollider();
	
	// test against a collider
	if( colObj.IsOwnerCollider() ){
		debpCollider * const collider = colObj.GetOwnerCollider();
		
		// no self collision. this happens because for testing a collision a separate
		// collision shape so individual shapes can be tested and transforms respected
		if( collider == pCollider ){
			return false;
		}
		
		// ask if a collision between those two colliders is possible
		deCollider * const engCollider = &collider->GetCollider();
		
		if( pCollider->CollidesNot( *collider ) ){
			return false;
		}
		
		// check if a collision between the two colliders is possible according to the moving collider listener
		if( engOrgCollider->GetPeerScripting()
		&& ! engOrgCollider->GetPeerScripting()->CanHitCollider( engOrgCollider, engCollider ) ){
			return false;
		}
// 		if( pColliderComponent ) printf( "collider %s needsCollision %s\n",
// 			( pColliderComponent->GetColliderComponent()->GetComponent() &&
// 			pColliderComponent->GetColliderComponent()->GetComponent()->GetRig() ) ?
// 			pColliderComponent->GetColliderComponent()->GetComponent()->GetRig()->GetFilename() : "-",
// 			( collider->IsComponent() &&
// 			collider->CastToComponent()->GetColliderComponent()->GetComponent() &&
// 			collider->CastToComponent()->GetColliderComponent()->GetComponent()->GetRig() ) ?
// 			collider->CastToComponent()->GetColliderComponent()->GetComponent()->GetRig()->GetFilename() : "-" );
		
		return true;
		
	// test against a height terrain sector
	}else if( colObj.GetOwnerHTSector() ){
		return engOrgCollider->GetCollisionFilter().Collides( colObj.GetOwnerHTSector()->
			GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter() );
	}
	
	// all other combinations score no collision
	return false;
}

btScalar debpClosestConvexResultCallback::addSingleResult( btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace ){
	const debpCollisionObject &colObj = *( ( debpCollisionObject* )convexResult.m_hitCollisionObject->getUserPointer() );
	
	if( colObj.IsOwnerCollider() ){
		pHitHTSector = NULL;
		pHitCollider = &colObj.GetOwnerCollider()->GetCollider();
		pHitBone = colObj.GetOwnerBone();
		pHitShape = ( int )( intptr_t )convexResult.m_hitCollisionShape->getUserPointer() - 1;
		pHitFace = -1;
// 		if( pColliderComponent ) printf( "collider %s addSingleResult (%g,%g,%g) %s\n",
// 			( pColliderComponent->GetColliderComponent()->GetComponent() &&
// 			pColliderComponent->GetColliderComponent()->GetComponent()->GetRig() ) ?
// 			pColliderComponent->GetColliderComponent()->GetComponent()->GetRig()->GetFilename() : "-",
// 			colObj.GetOwnerCollider()->GetCollider()->GetPosition().x,
// 			colObj.GetOwnerCollider()->GetCollider()->GetPosition().y,
// 			colObj.GetOwnerCollider()->GetCollider()->GetPosition().z,
// 			( colObj.GetOwnerCollider()->IsComponent() &&
// 			colObj.GetOwnerCollider()->CastToComponent()->GetColliderComponent()->GetComponent() &&
// 			colObj.GetOwnerCollider()->CastToComponent()->GetColliderComponent()->GetComponent()->GetRig() ) ?
// 			colObj.GetOwnerCollider()->CastToComponent()->GetColliderComponent()->GetComponent()->GetRig()->GetFilename() : "-" );
		
	}else if( colObj.IsOwnerHTSector() ){
		pHitHTSector = colObj.GetOwnerHTSector();
		pHitCollider = NULL;
		pHitBone = -1;
		pHitShape = -1;
		
		if( convexResult.m_localShapeInfo ){
			//pHitFace = convexResult.m_localShapeInfo->m_triangleIndex; // problem... bullet index not our index
			pHitFace = -1;
			
		}else{
			pHitFace = -1;
		}
	}
	
	return ClosestConvexResultCallback::addSingleResult( convexResult, normalInWorldSpace );
}
