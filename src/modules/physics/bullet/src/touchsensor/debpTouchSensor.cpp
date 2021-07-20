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
#include <string.h>

#include "debpTouchSensor.h"
#include "debpTSContactResultCallback.h"
#include "debpTSConvexResultCallback.h"
#include "../dePhysicsBullet.h"
#include "../debpGhostObject.h"
#include "../debpPhysicsBody.h"
#include "../coldet/debpCollisionDetection.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionDetection.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpDCollisionTriangle.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderBone.h"
#include "../collider/debpColliderBones.h"
#include "../collider/debpColliderComponent.h"
#include "../collider/debpColliderRig.h"
#include "../collider/debpColliderVolume.h"
#include "../debug/debpDebugDrawerColors.h"
#include "../shape/debpShape.h"
#include "../shape/debpCreateShape.h"
#include "../terrain/heightmap/debpHeightTerrain.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../visitors/debpShapeGroupExtends.h"
#include "../visitors/debpCreateBulletShape.h"
#include "../world/debpWorld.h"
#include "../world/debpDelayedOperation.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/sensor/deTouchSensorReference.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Debug
// #define DO_TIMING
// #define DO_TIMING2

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	static decTimer timerTotal;
	static decTimer timer;

	#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER(what)			pBullet->LogInfoFormat( "TouchSensor Timer: %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )
	#define DEBUG_PRINT_TIMER_TOTAL()		pBullet->LogInfoFormat( "TouchSensor Timer-Total = %iys", ( int )( timerTotal.GetElapsedTime() * 1000000.0 ) )
	
	#ifdef DO_TIMING2
		int timerTSPositionChanged = 0;
		int timerTSPositionChangedCount = 0;
		int timerTSOrientationChanged = 0;
		int timerTSOrientationChangedCount = 0;
		int timerTSRayHits = 0;
		int timerTSRayHitsCount = 0;
		int timerTSColliderHits = 0;
		int timerTSColliderHitsCount = 0;
		int timerTSColliderMoveHits = 0;
		int timerTSColliderMoveHitsCount = 0;
		int timerTSColliderRotateHits = 0;
		int timerTSColliderRotateHitsCount = 0;
		int timerTSColliderMoveRotateHits = 0;
		int timerTSColliderMoveRotateHitsCount = 0;
	#endif
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
	#define DEBUG_PRINT_TIMER_TOTAL()
#endif



// Class debpTouchSensor
//////////////////////////

// Constructor, destructor
////////////////////////////

debpTouchSensor::debpTouchSensor( dePhysicsBullet &bullet, deTouchSensor &touchSensor ) :
pBullet( bullet ),
pTouchSensor( touchSensor ),
pParentWorld( NULL ),
pDirtyMatrix( true ),
pDirtyExtends( true ),
pGhostObject( NULL ),
pDebugDrawer( NULL ),
pDDSShape( NULL )
{
	try{
		pGhostObject = new debpGhostObject;
		pGhostObject->SetOwnerTouchSensor( this );
		pGhostObject->SetPosition( touchSensor.GetPosition() );
		pGhostObject->SetOrientation( touchSensor.GetOrientation() );
		pGhostObject->SetEnabled( touchSensor.GetEnabled() );
		
		ShapeChanged();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debpTouchSensor::~debpTouchSensor(){
	pCleanUp();
}



// Management
///////////////

void debpTouchSensor::SetParentWorld( debpWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	pClearTracking();
	
	if( pParentWorld ){
		if( pDebugDrawer ){
			pParentWorld->GetWorld().RemoveDebugDrawer( pDebugDrawer );
		}
	}
	
	pParentWorld = parentWorld;
	
	if( parentWorld ){
		if( pDebugDrawer ){
			parentWorld->GetWorld().AddDebugDrawer( pDebugDrawer );
		}
	}
	
	// if tracking enter-leave all remaining changes have to be applied now
	/*
	if( pTouchSensor->GetTrackEnterLeave() ){
		int i;
		for( i=0; i<pShapeCount; i++ ){
			pShapes[ i ]->ApplyChanges();
		}
	}
	*/
	
	// update dynamic world in ghost objects
	debpCollisionWorld *bpdynworld = NULL;
	
	if( parentWorld ){
		bpdynworld = parentWorld->GetDynamicsWorld();
	}
	
	pGhostObject->SetDynamicsWorld( bpdynworld );
}



const decDVector &debpTouchSensor::GetMinimumExtend(){
	if( pDirtyExtends ){
		pUpdateExtends();
		pDirtyExtends = false;
	}
	
	return pMinExtend;
}

const decDVector &debpTouchSensor::GetMaximumExtend(){
	if( pDirtyExtends ){
		pUpdateExtends();
		pDirtyExtends = false;
	}
	
	return pMaxExtend;
}



const decDMatrix &debpTouchSensor::GetMatrix(){
	if( pDirtyMatrix ){
		pUpdateMatrix();
		pDirtyMatrix = false;
	}
	
	return pMatrix;
}

const decDMatrix &debpTouchSensor::GetInverseMatrix(){
	if( pDirtyMatrix ){
		pUpdateMatrix();
		pDirtyMatrix = false;
	}
	
	return pInvMatrix;
}



void debpTouchSensor::ApplyChanges(){
	if( ! pTouchSensor.GetTrackEnterLeave() ){
		const debpDeveloperMode &devmode = pBullet.GetDeveloperMode();
		
		if( devmode.GetEnabled()
		&& devmode.GetShowCategory().IsNotEmpty()
		&& devmode.GetShowCategory().Matches( pTouchSensor.GetCollisionFilter().GetCategory() ) ){
			UpdateDebugDrawer();
		}
		
		return;
	}
	
	// NOTE it is possible the user removes touch sensors during tracking changes. to avoid problems
	//      touch sensors are keeping a fake reference while applying changes to not vanish underneath
	const deTouchSensorReference guard( &pTouchSensor );
	
	// this function is only called if tracking enter-leave is enabled
	if( pGhostObject->GetGhostObject() ){
		// check all collision pairs in the ghost collider for touching the sensor
		const btGhostObject &ghostObject = *pGhostObject->GetGhostObject();
		const int count = ghostObject.getNumOverlappingObjects();
		int i;
		
		for( i=0; i<count; i++ ){
			const btCollisionObject * const btColObj = ghostObject.getOverlappingObject( i );
			const debpCollisionObject &colObj = *( ( debpCollisionObject* )btColObj->getUserPointer() );
			
			if( colObj.IsOwnerCollider() ){
				debpCollider * const collider = colObj.GetOwnerCollider();
				
				// temporary hack. if the collider is dynamic UpdateShapes is not called and collision fails
// 				if( collider->GetCollider().GetResponseType() == deCollider::ertDynamic ){
// 					collider->UpdateShapes();
// 				}
				
				if( TestCollider( collider ) ){
					if( ! pTouchingColliders.Has( collider ) ){
						if( pLeavingColliders.Has( collider ) ){
							pLeavingColliders.Remove( collider );
							pTouchingColliders.Add( collider );
							
						}else{
							pTouchingColliders.Add( collider );
							collider->GetTrackingTouchSensors().Add( this );
						}
						
						pTouchSensor.NotifyColliderEntered( &collider->GetCollider() );
						// at this point we might have been removed from the parent world
						// by the user. in this case pParentWorld is NULL and all further
						// actions are invalid. we need to quit the loop
						if( ! pParentWorld ){
							break;
						}
					}
					
				}else{
					if( pTouchingColliders.Has( collider ) ){
						pTouchingColliders.Remove( collider );
						pLeavingColliders.Add( collider );
					}
				}
				
			//}else if( colObj.IsOwnerHTSector() ){
			//	// TODO
			}
		}
		
		// send a notification for all leaving colliders. if we have been removed from the
		// parent world in the mean time pLeavingColliders is empty so no need to check
		// if pParentWorld is NULL
		while( pLeavingColliders.GetCount() > 0 ){
			const int index = pLeavingColliders.GetCount() - 1;
			debpCollider * const collider = ( debpCollider* )pLeavingColliders.GetAt( index );
			pLeavingColliders.RemoveFrom( index );
			collider->GetTrackingTouchSensors().Remove( this );
			
			pTouchSensor.NotifyColliderLeft( &collider->GetCollider() );
		}
		
	}else{
		// all colliders left the shape
		while( pTouchingColliders.GetCount() > 0 ){
			const int index = pTouchingColliders.GetCount() - 1;
			debpCollider * const collider = ( debpCollider* )pTouchingColliders.GetAt( index );
			pTouchingColliders.RemoveFrom( index );
			collider->GetTrackingTouchSensors().Remove( this );
			
			pTouchSensor.NotifyColliderLeft( &collider->GetCollider() );
		}
	}
	
	UpdateDebugDrawer();
}



void debpTouchSensor::MarkGhostObjectDirty(){
	/*
	btGhostObject * const ghostObject = pGhostObject->GetGhostObject();
	
	if( ghostObject ){
		// force an update. basically this is m_updateRevision++; but the parameter is
		// protected and out of reach. setting the collision shape updates the revision
		// number while doing little work (assigning two pointers). it's the cheapest
		// way to do the update
		ghostObject->setCollisionShape( ghostObject->getCollisionShape() );
	}
	*/
}

bool debpTouchSensor::Collides( const debpCollider &collider ) const{
	deCollider &engCollider = collider.GetCollider();
	return pTouchSensor.GetCollisionFilter().Collides( engCollider.GetCollisionFilter() )
		&& ! pTouchSensor.HasIgnoreCollider( &engCollider );
}

bool debpTouchSensor::CollidesNot( const debpCollider &collider ) const{
	deCollider &engCollider = collider.GetCollider();
	return pTouchSensor.GetCollisionFilter().CollidesNot( engCollider.GetCollisionFilter() )
		|| pTouchSensor.HasIgnoreCollider( &engCollider );
}

bool debpTouchSensor::TestCollider( debpCollider *collider ){
	const decDMatrix &transformation = GetMatrix();
	debpDCollisionBox approxColliderBox;
	
	// this is a little bit hacky here. The ShapeHitsCollider call is a precise call and does a
	// conclusive test. for all practical situations this is okay but if a collider component
	// has no rig a speed problem happens. in this situation a slow test between the shape and
	// each triangle is done. this case can happen if a user places an unoptimized model in a
	// game or in editors. especially for the later it is better to avoid performance loss by
	// using an approximation. the collision detection routine does not know we allow an
	// approximated test here so we have to do this manually.
	bool preciseTesting = true;
	
	if( collider->IsComponent() ){
		switch( ( ( debpColliderComponent* )collider )->GetTestMode() ){
		case debpColliderComponent::etmModelStatic:
			// if the face count is small we can do a precise test. but right now better not
			preciseTesting = false;
			break;
			
		case debpColliderComponent::etmModelDynamic:
			// no precise testing for dynamic models right now
			preciseTesting = false;
			break;
			
		default:
			break;
		}
		
		if( ! preciseTesting ){
			// Get{Min,Max}imumExtend is deprecated. use btCollisionShape AABB instead
			approxColliderBox.SetFromExtends( collider->GetMinimumExtend(), collider->GetMaximumExtend() );
		}
	}
	
	if( preciseTesting ){
		/*
		// TODO we've got a huge problem here. the collision detection provided by the game engine
		//      (which is going to be deprecated) does not contain Capsule-Box or Capsule-Sphere
		//      collision tests and thus never finds a collision. this is a problem for actors
		//      using a capsule. we need to delegate the collision test to bullet
		const int count = pShape.GetShapeCount();
		debpCollisionDetection &coldet = pBullet.GetCollisionDetection();
		debpCollisionResult result;
		int i;
		
		for( i=0; i<count; i++ ){
			debpShape * const shape = *pShape.GetShapeAt( i );
			shape->UpdateWithMatrix( transformation );
			if( coldet.ShapeHitsCollider( shape, collider, result ) ){
				return true;
			}
		}
		*/
		
		debpCollisionWorld &cw = *pGhostObject->GetDynamicsWorld();
		btGhostObject * const go = pGhostObject->GetGhostObject();
		
		if( collider->IsVolume() ){
			const debpColliderVolume &volume = *collider->CastToVolume();
			
			return volume.GetPhysicsBody()
				&& volume.GetPhysicsBody()->GetRigidBody()
				&& cw.safeContactPairTest( go, volume.GetPhysicsBody()->GetRigidBody() );
			
		}else if( collider->IsComponent() ){
			const debpColliderComponent &component = *collider->CastToComponent();
			
			if( component.GetSimplePhysicsBody() ){
				return component.GetSimplePhysicsBody()->GetRigidBody()
					&& cw.safeContactPairTest( go, component.GetSimplePhysicsBody()->GetRigidBody() );
				
			}else if( component.GetBones() ){
				const debpColliderBones &bones = *component.GetBones();
				const int boneCount = bones.GetBonePhysicsCount();
				int i;
				
				for( i=0; i<boneCount; i++ ){
					const debpPhysicsBody * const phyBody = bones.GetBonePhysicsAt( i ).GetPhysicsBody();
					if( phyBody && phyBody->GetRigidBody() && cw.safeContactPairTest( go, phyBody->GetRigidBody() ) ){
						return true;
					}
				}
			}
			
		}else if( collider->IsRigged() ){
			const debpColliderRig &rig = *collider->CastToRigged();
			
			if( rig.GetSimplePhysicsBody() ){
				return rig.GetSimplePhysicsBody()->GetRigidBody()
					&& cw.safeContactPairTest( go, rig.GetSimplePhysicsBody()->GetRigidBody() );
				
			}else if( rig.GetBones() ){
				debpColliderBones &bones = *rig.GetBones();
				const int boneCount = bones.GetBonePhysicsCount();
				int i;
				
				for( i=0; i<boneCount; i++ ){
					const debpPhysicsBody * const phyBody = bones.GetBonePhysicsAt( i ).GetPhysicsBody();
					if( phyBody && phyBody->GetRigidBody() && cw.safeContactPairTest( go, phyBody->GetRigidBody() ) ){
						return true;
					}
				}
			}
			
		}else{
			DETHROW( deeInvalidParam );
		}
		
	}else{
		const int count = pShape.GetShapeCount();
		int i;
		
		for( i=0; i<count; i++ ){
			debpShape &shape = *pShape.GetShapeAt( i );
			shape.UpdateWithMatrix( transformation );
			if( shape.GetCollisionVolume()->BoxHitsVolume( &approxColliderBox ) ){
				return true;
			}
		}
	}
	
	return false;
}



// Notifications
//////////////////

void debpTouchSensor::PositionChanged(){
	#ifdef DO_TIMING2
	timer.Reset();
	#endif
	pDirtyMatrix = true;
	pDirtyExtends = true;
	
	const decDVector &position = pTouchSensor.GetPosition();
	pGhostObject->SetPosition( position );
	if( pDebugDrawer ){
		pDebugDrawer->SetPosition( position );
	}
	#ifdef DO_TIMING2
	timerTSPositionChanged += ( int )( timer.GetElapsedTime() * 1e6f );
	timerTSPositionChangedCount++;
	#endif
}

void debpTouchSensor::OrientationChanged(){
	#ifdef DO_TIMING2
	timer.Reset();
	#endif
	pDirtyMatrix = true;
	pDirtyExtends = true;
	
	const decQuaternion &orientation = pTouchSensor.GetOrientation();
	pGhostObject->SetOrientation( orientation );
	if( pDebugDrawer ){
		pDebugDrawer->SetOrientation( orientation );
	}
	#ifdef DO_TIMING2
	timerTSOrientationChanged += ( int )( timer.GetElapsedTime() * 1e6f );
	timerTSOrientationChangedCount++;
	#endif
}



void debpTouchSensor::CollisionFilterChanged(){
	MarkGhostObjectDirty();
}

void debpTouchSensor::IgnoreCollidersChanged(){
	MarkGhostObjectDirty();
}

void debpTouchSensor::EnabledChanged(){
	pGhostObject->SetEnabled( pTouchSensor.GetEnabled() );
}

void debpTouchSensor::TrackEnterLeaveChanged(){
}



bool debpTouchSensor::IsEmpty(){
	return pTouchingColliders.GetCount() == 0;
}

int debpTouchSensor::GetColliderCount(){
	return pTouchingColliders.GetCount();
}

deCollider *debpTouchSensor::GetColliderAt( int collider ){
	return &( ( debpCollider* )pTouchingColliders.GetAt( collider ) )->GetCollider();
}



void debpTouchSensor::ShapeChanged(){
	const decShapeList &shapeList = pTouchSensor.GetShape();
	const int count = shapeList.GetCount();
	
	pShape.RemoveAllShapes();
	if( count == 0 ){
		return;
	}
	
	debpCreateShape createShape;
	int i;
	for( i=0; i<count; i++ ){
		shapeList.GetAt( i )->Visit( createShape );
		if( ! createShape.GetCreatedShape() ){
			DETHROW( deeInvalidParam );
		}
		pShape.AddShape( createShape.GetCreatedShape() );
	}
	
	// update ghost object shape
	debpCreateBulletShape createBulletShape;
	for( i=0; i<count; i++ ){
		shapeList.GetAt( i )->Visit( createBulletShape );
	}
	pGhostObject->SetShape( createBulletShape.GetBulletShape() );
	
	pCalculateBasicExtends();
}



// Collision Detection
////////////////////////

bool debpTouchSensor::PointInside( const decDVector &point ){
	return pBullet.GetCollisionDetection().GetBulletShapeCollision().IsPointInside(
		*pGhostObject->GetGhostObject(), btVector3( ( btScalar )point.x,
			( btScalar )point.y, ( btScalar )point.z ) );
}

void debpTouchSensor::AllHits( deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pGhostObject->GetGhostObject() ){
		return;
	}
	
	// TODO here we only need to lock the ghost object not the entire world. for the
	//      time being the world is locked to get it working quickly
	pGhostObject->GetDynamicsWorld()->GetDelayedOperation().Lock();
	
	try{
		debpCollisionDetection &coldet = pBullet.GetCollisionDetection();
		deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
		
		const btAlignedObjectArray<btCollisionObject*> &colobjs =
			pGhostObject->GetGhostObject()->getOverlappingPairs();
		const int colobjCount = colobjs.size();
		int i;
		
		for( i=0; i<colobjCount; i++ ){
			const debpCollisionObject &colObj =
				*( ( debpCollisionObject* )colobjs.at( i )->getUserPointer() );
			
			if( colObj.IsOwnerCollider() ){
				// huge problem. colliders composing of multiple rigid bodies (bones in general)
				// results in the collider being visited multiple times which is not what the
				// function is supposed to do
				if( TestCollider( colObj.GetOwnerCollider() ) ){
					colinfo->SetCollider( &colObj.GetOwnerCollider()->GetCollider(), -1, -1, -1 );
					listener->CollisionResponse( NULL, colinfo );
				}
				
			}else if( colObj.IsOwnerHTSector() ){
				// how can we test if this really overlaps?
				const debpHTSector &hts = *colObj.GetOwnerHTSector();
				colinfo->SetHTSector( hts.GetHeightTerrain()->GetHeightTerrain(), hts.GetSector() );
				listener->CollisionResponse( NULL, colinfo );
			}
		}
		
		pGhostObject->GetDynamicsWorld()->GetDelayedOperation().Unlock();
		
	}catch( const deException & ){
		pGhostObject->GetDynamicsWorld()->GetDelayedOperation().Unlock();
		throw;
	}
#ifdef DO_TIMING2
timerTSRayHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerTSRayHitsCount++;
#endif
}

void debpTouchSensor::RayHits( const decDVector &rayOrigin, const decVector &rayDirection,
deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
// 	printf( "touchSensor=%p shape=%p rayHits rayOrigin=(%f,%f,%f) rayDirection=(%f,%f,%f)\n", pTouchSensor, this,
// 		rayOrigin.x, rayOrigin.y, rayOrigin.z, rayDirection.x, rayDirection.y, rayDirection.z );
#define BULLET_RAY_CAST_UNSTABLE 1
	
	if( ! pGhostObject->GetGhostObject() ){
		return;
	}
	
	// TODO here we only need to lock the ghost object not the entire world. for the
	//      time being the world is locked to get it working quickly
	pGhostObject->GetDynamicsWorld()->GetDelayedOperation().Lock();
	
	try{
		debpCollisionDetection &coldet = pBullet.GetCollisionDetection();
		deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
		
		const btVector3 btRayFrom( ( btScalar )rayOrigin.x,
			( btScalar )rayOrigin.y, ( btScalar )rayOrigin.z );
		const decDVector rayTo( rayOrigin + rayDirection );
		const btVector3 btRayTo( ( btScalar )rayTo.x, ( btScalar )rayTo.y, ( btScalar )rayTo.z );
		const btQuaternion btOrientation( BT_ZERO, BT_ZERO, BT_ZERO, BT_ONE );
		
		const btTransform rayFromTrans( btOrientation, btRayFrom );
		const btTransform rayToTrans( btOrientation, btRayTo );
		
		#ifdef BULLET_RAY_CAST_UNSTABLE
			// bullet has a broken ray-box test implementation using Gjk which has a tendency
			// to miss collisions half of the time. as a quick fix a sweep test is done with
			// a tiny sphere which yields a comparable result but is not prone to the problem
			debpTSConvexResultCallback result( colinfo, *this );
			result.SetTestRay( NULL, listener );
			colinfo->SetStopTesting( false );
			coldet.GetRayHackShape().SweepTest( *pGhostObject, rayFromTrans, rayToTrans, result );
			
		#else
			const btAlignedObjectArray<btCollisionObject*> &colobjs =
				pGhostObject->GetGhostObject()->getOverlappingPairs();
			const int colobjCount = colobjs.size();
			int i;
			
			// update abbbs?
			
			debpTSRayResultCallback result( colinfo, *pTouchSensor );
			result.SetTestRay( rayOrigin, rayDirection, NULL, 0, listener );
			colinfo->SetStopTesting( false );
			
			for( i=0; i<colobjCount; i++ ){
				btCollisionObject * const colobj = colobjs.at( i );
				
				if( result.needsCollision( colobj->getBroadphaseHandle() ) ){
					debpCollisionWorld::rayTestSingle( rayFromTrans, rayToTrans, colobj,
						colobj->getCollisionShape(), colobj->getWorldTransform(), result );
				}
			}
		#endif
		
		pGhostObject->GetDynamicsWorld()->GetDelayedOperation().Unlock();
		
	}catch( const deException & ){
		pGhostObject->GetDynamicsWorld()->GetDelayedOperation().Unlock();
		throw;
	}
#ifdef DO_TIMING2
timerTSRayHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerTSRayHitsCount++;
#endif
}

static btManifoldPoint vDummyManifoldPoint; // Dummy manifold point to be used where bullet requires one but dragengine not

void debpTouchSensor::ColliderHits( deCollider *collider, deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! collider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
// 	printf( "touchSensor=%p shape=%p colliderHits pos=(%f,%f,%f)\n", pTouchSensor, this,
// 		    engCollider->GetPosition().x, engCollider->GetPosition().y, engCollider->GetPosition().z );
	
	if( ! pGhostObject->GetGhostObject() ){
		return;
	}
	
	// TODO here we only need to lock the ghost object not the entire world. for the
	//      time being the world is locked to get it working quickly
	pGhostObject->GetDynamicsWorld()->GetDelayedOperation().Lock();
	
	try{
		// update abbbs?
		
		debpCollisionDetection &coldet = pBullet.GetCollisionDetection();
		debpCollider * const bpCollider = ( debpCollider* )collider->GetPeerPhysics();
		deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
		debpCollisionResult coldetResult;
		
		const btAlignedObjectArray<btCollisionObject*> &colobjs =
			pGhostObject->GetGhostObject()->getOverlappingPairs();
		const int colobjCount = colobjs.size();
		int i;
		
		debpTSContactResultCallback result( colinfo, *this );
		
		if( bpCollider->IsVolume() ){
			debpColliderVolume &colliderVolume = *bpCollider->CastToVolume();
			btGhostObject * const staticCollisionTest = colliderVolume.GetStaticCollisionTest();
			const btCollisionObjectWrapper obj0Wrap( NULL, staticCollisionTest->getCollisionShape(),
				staticCollisionTest, staticCollisionTest->getWorldTransform(), -1, -1 );
			
			const decDVector &position = colliderVolume.GetPosition();
			const btVector3 btposition( ( btScalar )position.x,
				( btScalar )position.y, ( btScalar )position.z );
			
			const decQuaternion &orientation = colliderVolume.GetOrientation();
			const btQuaternion btorientation( ( btScalar )orientation.x, ( btScalar )orientation.y,
				( btScalar )orientation.z, ( btScalar )orientation.w );
			
			staticCollisionTest->setWorldTransform( btTransform( btorientation, btposition ) );
			
			result.SetTestCollider( staticCollisionTest, bpCollider, listener );
			colinfo->SetStopTesting( false );
			
			bpCollider->UpdateShapes();
			
			for( i=0; i<colobjCount; i++ ){
				btCollisionObject * const bpcolobj = colobjs.at( i );
				
				btVector3 aabbMin, aabbMax;
				bpcolobj->getCollisionShape()->getAabb( bpcolobj->getWorldTransform(), aabbMin, aabbMax );
				
				if( result.needsCollision( bpcolobj->getBroadphaseHandle() ) ){
					const debpCollisionObject &colobj = *( ( debpCollisionObject* )bpcolobj->getUserPointer() );
					
					// test against collider
					if( colobj.IsOwnerCollider() ){
						if( coldet.ColliderHitsCollider( bpCollider, colobj.GetOwnerCollider(), coldetResult ) ){
							const btCollisionObjectWrapper obj1Wrap( NULL, bpcolobj->getCollisionShape(), bpcolobj, bpcolobj->getWorldTransform(), -1, -1 );
							result.addSingleResult( vDummyManifoldPoint, &obj0Wrap, 0, -1, &obj1Wrap, 0, coldetResult.face );
							//if( result.addSingleResult( vDummyManifoldPoint, &obj0Wrap, 0, -1, &obj1Wrap, 0, coldetResult.face ) < ( btScalar )0.5 ){
							//	break;
							//}
						}
						
					// test against height terrain
					}else if( colobj.IsOwnerHTSector() ){
						if( coldet.ColliderHitsHeightTerrain( bpCollider, colobj.GetOwnerHTSector(), coldetResult ) ){
							const btCollisionObjectWrapper obj1Wrap( NULL, bpcolobj->getCollisionShape(), bpcolobj, bpcolobj->getWorldTransform(), -1, -1 );
							result.addSingleResult( vDummyManifoldPoint, &obj0Wrap, 0, -1, &obj1Wrap, 0, coldetResult.face );
							//if( result.addSingleResult( vDummyManifoldPoint, &obj0Wrap, 0, -1, &obj1Wrap, 0, coldetResult.face ) < ( btScalar )0.5 ){
							//	break;
							//}
						}
					}
				}
			}
		}
		
		pGhostObject->GetDynamicsWorld()->GetDelayedOperation().Unlock();
		
	}catch( const deException & ){
		pGhostObject->GetDynamicsWorld()->GetDelayedOperation().Unlock();
		throw;
	}
#ifdef DO_TIMING2
timerTSColliderHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerTSColliderHitsCount++;
#endif
}

void debpTouchSensor::ColliderMoveHits( deCollider *collider, const decVector &displacement,
deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! collider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
// 	printf( "touchSensor=%p shape=%p colliderMoveHits pos=(%f,%f,%f) disp(%f,%f,%f)\n", pTouchSensor, this,
// 		   engCollider->GetPosition().x, engCollider->GetPosition().y, engCollider->GetPosition().z,
// 		   displacement.x, displacement.y, displacement.z );
	
	if( ! pGhostObject->GetGhostObject() ){
		return;
	}
	
	debpCollisionDetection &coldet = pBullet.GetCollisionDetection();
	deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
	debpCollider *bpCollider = ( debpCollider* )collider->GetPeerPhysics();
	
	debpTSConvexResultCallback result( colinfo, *this );
	result.SetTestCollider( bpCollider, listener );
	colinfo->SetStopTesting( false );
	
	if( bpCollider->IsVolume() ){
		debpColliderVolume &colliderVolume = *bpCollider->CastToVolume();
		debpSweepCollisionTest &sweepCollisionTest = *colliderVolume.GetSweepCollisionTest();
		
		if( sweepCollisionTest.GetShapeList().GetCount() == 0 ){
			return;
		}
		
		const decDVector &from = colliderVolume.GetPosition();
		const btVector3 btfrom( ( btScalar )from.x, ( btScalar )from.y, ( btScalar )from.z );
		
		const decDVector to = from + displacement;
		const btVector3 btto( ( btScalar )to.x, ( btScalar )to.y, ( btScalar )to.z );
		
		const decQuaternion &orientation = colliderVolume.GetOrientation();
		const btQuaternion btorientation( ( btScalar )orientation.x, ( btScalar )orientation.y,
			( btScalar )orientation.z, ( btScalar )orientation.w );
		
		const btTransform transformFrom( btorientation, btfrom );
		const btTransform transformTo( btorientation, btto );
		
		sweepCollisionTest.SweepTest( *pGhostObject, transformFrom, transformTo, result );
	}
#ifdef DO_TIMING2
timerTSColliderMoveHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerTSColliderMoveHitsCount++;
#endif
}

void debpTouchSensor::ColliderRotateHits( deCollider *collider, const decVector &rotation,
deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! collider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pGhostObject->GetGhostObject() ){
		return;
	}
	
	debpCollisionDetection &coldet = pBullet.GetCollisionDetection();
	deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
	debpCollider *bpCollider = ( debpCollider* )collider->GetPeerPhysics();
	
	debpTSConvexResultCallback result( colinfo, *this );
	result.SetTestCollider( bpCollider, listener );
	colinfo->SetStopTesting( false );
	
	if( bpCollider->IsVolume() ){
		debpColliderVolume &colliderVolume = *bpCollider->CastToVolume();
		debpSweepCollisionTest &sweepCollisionTest = *colliderVolume.GetSweepCollisionTest();
		
		if( sweepCollisionTest.GetShapeList().GetCount() > 0 ){
			const decDVector &position = colliderVolume.GetPosition();
			const btVector3 btposition( ( btScalar )position.x,
				( btScalar )position.y, ( btScalar )position.z );
			
			const decQuaternion &orientationFrom = colliderVolume.GetOrientation();
			const btQuaternion btRotFrom( ( btScalar )orientationFrom.x,
				( btScalar )orientationFrom.y, ( btScalar )orientationFrom.z,
				( btScalar )orientationFrom.w );
			
			const decQuaternion orientationTo( orientationFrom
				* decQuaternion::CreateFromEuler( rotation ) );
			const btQuaternion btRotTo( ( btScalar )orientationTo.x, ( btScalar )orientationTo.y,
				( btScalar )orientationTo.z, ( btScalar )orientationTo.w );
			
			const btTransform transformFrom( btRotFrom, btposition );
			const btTransform transformTo( btRotTo, btposition );
			
			sweepCollisionTest.SweepTest( *pGhostObject, transformFrom, transformTo, result );
		}
	}
#ifdef DO_TIMING2
timerTSColliderRotateHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerTSColliderRotateHitsCount++;
#endif
}

void debpTouchSensor::ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
const decVector &rotation, deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! collider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
// 	printf( "touchSensor=%p shape=%p colliderMoveRotateHits pos=(%f,%f,%f) disp(%f,%f,%f) rot(%f,%f,%f)\n", pTouchSensor, this,
// 		   engCollider->GetPosition().x, engCollider->GetPosition().y, engCollider->GetPosition().z,
// 		   displacement.x, displacement.y, displacement.z, rotation.x / DEG2RAD, rotation.y / DEG2RAD, rotation.z / DEG2RAD );
	
	if( ! pGhostObject->GetGhostObject() ){
		return;
	}
	
	debpCollisionDetection &coldet = pBullet.GetCollisionDetection();
	deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
	debpCollider *bpCollider = ( debpCollider* )collider->GetPeerPhysics();
	
	debpTSConvexResultCallback result( colinfo, *this );
	result.SetTestCollider( bpCollider, listener );
	colinfo->SetStopTesting( false );
	
	if( bpCollider->IsVolume() ){
		debpColliderVolume &colliderVolume = *bpCollider->CastToVolume();
		debpSweepCollisionTest &sweepCollisionTest = *colliderVolume.GetSweepCollisionTest();
		
		if( sweepCollisionTest.GetShapeList().GetCount() > 0 ){
			const decDVector &positionFrom = colliderVolume.GetPosition();
			const btVector3 btPosFrom( ( btScalar )positionFrom.x,
				( btScalar )positionFrom.y, ( btScalar )positionFrom.z );
			
			const decDVector positionTo( positionFrom + decDVector( displacement ) );
			const btVector3 btPosTo( ( btScalar )positionTo.x,
				( btScalar )positionTo.y, ( btScalar )positionTo.z );
			
			const decQuaternion &orientationFrom = colliderVolume.GetOrientation();
			const btQuaternion btRotFrom( ( btScalar )orientationFrom.x,
				( btScalar )orientationFrom.y, ( btScalar )orientationFrom.z,
				( btScalar )orientationFrom.w );
			
			const decQuaternion orientationTo( orientationFrom
				* decQuaternion::CreateFromEuler( rotation ) );
			const btQuaternion btRotTo( ( btScalar )orientationTo.x, ( btScalar )orientationTo.y,
				( btScalar )orientationTo.z, ( btScalar )orientationTo.w );
			
			const btTransform transformFrom( btRotFrom, btPosFrom );
			const btTransform transformTo( btRotTo, btPosTo );
			
			sweepCollisionTest.SweepTest( *pGhostObject, transformFrom, transformTo, result );
		}
	}
#ifdef DO_TIMING2
timerTSColliderMoveRotateHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerTSColliderMoveRotateHitsCount++;
#endif
}



// Debugging
//////////////

void debpTouchSensor::UpdateDebugDrawer(){
	const debpDeveloperMode &devmode = pBullet.GetDeveloperMode();
	
	if( devmode.GetEnabled()
	&& devmode.GetShowCategory().IsNotEmpty()
	&& devmode.GetShowCategory().Matches( pTouchSensor.GetCollisionFilter().GetCategory() ) ){
		// ensure the debug drawer exists
		if( ! pDebugDrawer ){
			pDebugDrawer = pBullet.GetGameEngine()->GetDebugDrawerManager()->CreateDebugDrawer();
			pDebugDrawer->SetXRay( true );
			pDebugDrawer->SetPosition( pTouchSensor.GetPosition() );
			pDebugDrawer->SetOrientation( pTouchSensor.GetOrientation() );
			
			if( pParentWorld ){
				pParentWorld->GetWorld().AddDebugDrawer( pDebugDrawer );
			}
		}
		
		// show shapes if layer mask matches
		if( ! pDDSShape ){
			pDDSShape = new deDebugDrawerShape;
			pDDSShape->SetFillColor( debpDebugDrawerColors::touchSensorFill );
			pDDSShape->SetEdgeColor( debpDebugDrawerColors::touchSensorEdge );
			pDebugDrawer->AddShape( pDDSShape );
			UpdateDDSShape();
		}
		
	}else{
		// if the debug drawer exists remove it
		if( pDebugDrawer ){
			if( pParentWorld ){
				pParentWorld->GetWorld().RemoveDebugDrawer( pDebugDrawer );
			}
			
			pDDSShape = NULL;
			
			pDebugDrawer->FreeReference();
			pDebugDrawer = NULL;
		}
	}
}

void debpTouchSensor::UpdateDDSShape(){
	if( ! pDDSShape ){
		return;
	}
	
	pDDSShape->GetShapeList() = pTouchSensor.GetShape();
	GetDebugDrawer()->NotifyShapeContentChanged();
}



// Private functions
//////////////////////

void debpTouchSensor::pCleanUp(){
	pClearTracking();
	
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
	if( pGhostObject ){
		delete pGhostObject;
	}
}

void debpTouchSensor::pUpdateMatrix(){
	pMatrix.SetWorld( pTouchSensor.GetPosition(), pTouchSensor.GetOrientation() );
	pInvMatrix = pMatrix.Invert();
}

void debpTouchSensor::pUpdateExtends(){
	const decQuaternion &orientation = pTouchSensor.GetOrientation();
	const decDVector &position = pTouchSensor.GetPosition();
	debpDCollisionBox box, extendsBox;
	
	box.SetFromExtends( decDVector( pBasicMinExtend ), decDVector( pBasicMaxExtend ) );
	box.SetCenter( position + decDMatrix::CreateFromQuaternion( orientation ).TransformNormal( box.GetCenter() ) );
	box.SetOrientation( orientation );
	
	box.GetEnclosingBox( &extendsBox );
	
	pMinExtend = extendsBox.GetCenter() - extendsBox.GetHalfSize();
	pMaxExtend = extendsBox.GetCenter() + extendsBox.GetHalfSize();
	
//	pBullet->LogInfoFormat( "TouchSensor %p: extends (%g,%g,%g) -> (%g,%g,%g)", this,
//		pMinExtend.x, pMinExtend.y, pMinExtend.z, pMaxExtend.x, pMaxExtend.y, pMaxExtend.z );
	
	pDirtyExtends = false;
}

void debpTouchSensor::pCalculateBasicExtends(){
	const decShapeList &list = pTouchSensor.GetShape();
	debpShapeGroupExtends shapeGroupExtends;
	const int count = list.GetCount();
	int i;
	
	// determine the extend of the shapes
	for( i=0; i<count; i++ ){
		list.GetAt( i )->Visit( shapeGroupExtends );
	}
	
	// store the found extends
	pBasicMinExtend = shapeGroupExtends.GetMinimumExtend();
	pBasicMaxExtend = shapeGroupExtends.GetMaximumExtend();
	
//	pBullet->LogInfoFormat( "TouchSensor %p ( %i shapes ): basic extends (%g,%g,%g) -> (%g,%g,%g)", this, shapeCount,
//		pBasicMinExtend.x, pBasicMinExtend.y, pBasicMinExtend.z,
//		pBasicMaxExtend.x, pBasicMaxExtend.y, pBasicMaxExtend.z );
	
	// real extends are now dirty
	pDirtyExtends = true;
}

void debpTouchSensor::pClearTracking(){
	// remove touching colliders
	while( pTouchingColliders.GetCount() > 0 ){
		const int index = pTouchingColliders.GetCount() - 1;
		debpCollider * const collider = ( debpCollider* )pTouchingColliders.GetAt( index );
		pTouchingColliders.RemoveFrom( index );
		collider->GetTrackingTouchSensors().Remove( this );
		
		pTouchSensor.NotifyColliderLeft( &collider->GetCollider() );
	}
	
	// remove leaving colliders
	while( pLeavingColliders.GetCount() > 0 ){
		const int index = pLeavingColliders.GetCount() - 1;
		debpCollider * const collider = ( debpCollider* )pLeavingColliders.GetAt( index );
		pLeavingColliders.RemoveFrom( index );
		collider->GetTrackingTouchSensors().Remove( this );
		
		pTouchSensor.NotifyColliderLeft( &collider->GetCollider() );
	}
	
	// in the above both can not happen at the same time so GetTrackingTouchSensors() can not
	// be altered twice by the same collider (which would result in an exception)
}
