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

#include "debpColliderAttachment.h"
#include "debpColliderConstraint.h"
#include "debpColliderVolume.h"
#include "../coldet/unstuck/debpUnstuckCollider.h"
#include "../coldet/debpSweepCollisionTest.h"
#include "../coldet/debpCollisionDetection.h"
#include "../coldet/debpConvexResultCallback.h"
#include "../coldet/debpRayResultCallback.h"
#include "../coldet/debpContactResultCallback.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../debpMotionState.h"
#include "../debpPhysicsBody.h"
#include "../dePhysicsBullet.h"
#include "../shape/debpCreateShape.h"
#include "../shape/debpShapeBox.h"
#include "../shape/debpShape.h"
#include "../shape/debpShapeSphere.h"
#include "../terrain/heightmap/debpHeightTerrain.h"
#include "../visitors/debpCreateBulletShape.h"
#include "../visitors/debpClosestConvexResultCallback.h"
#include "../world/debpCollisionWorld.h"
#include "../world/debpWorld.h"
#include "../debpBulletShape.h"
#include "../debpGhostObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderReference.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>



// #define DO_TIMING
// #define DO_TIMING2

#if defined DO_TIMING || defined DO_TIMING2
	#include <dragengine/common/utils/decTimer.h>
#endif

#ifdef DO_TIMING
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER(what)			GetBullet()->LogInfoFormat( "Collider Volume Timer: %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )
	#define DEBUG_PRINT_TIMER_TOTAL(what)	GetBullet()->LogInfoFormat( "Collider Volume Timer-Total: %s = %iys", what, ( int )( timerTotal.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
	#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif

#ifdef DO_TIMING2
	static decTimer timer2;
	
	#define DEBUG_RESET_TIMER2				timer2.Reset();
	#define DEBUG_PRINT_TIMER2(what)		GetBullet()->LogInfoFormat( "Collider Volume Timer: %s = %iys", what, ( int )( timer2.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMER2
	#define DEBUG_PRINT_TIMER2(what)
#endif



// Class debpColliderVolume
/////////////////////////////

// Constructor, destructor
////////////////////////////

debpColliderVolume::debpColliderVolume( dePhysicsBullet *bullet, deColliderVolume &collider ) :
debpCollider( bullet, collider, ectVolume ),
pColliderVolume( collider )
{
	pHasGravity = false;
	pHasLinVelo = false;
	pHasAngVelo = false;
	pPreventUpdate = false;
	pPreventAttNotify = false;
	pDirtyBPShape = true;
	pDirtyShapes = true;
	pDirtySweepTest = true;
	pDirtyStaticTest = true;
	pDirtyAttachments = true;
	pUseFakeDynamics = false;
	pSweepCollisionTest = NULL;
	
	pPhyBody = new debpPhysicsBody;
	pPhyBody->SetOwnerCollider( this, -1 );
	
	pStaticCollisionTest = NULL;
	pStaticCollisionTestShape = NULL;
	pStaticCollisionTestObject.SetOwnerCollider( this, -1 );
	
	pGhostKinematicMovement = NULL;
// 	pGhostKinematicMovement = new debpGhostObject;
// 	pGhostKinematicMovement->SetOwnerCollider( this, -1 );
// 	pGhostKinematicMovement->SetEnabled( false );
// 	pGhostKinematicMovement->SetShape( new btBoxShape( btVector3( ( btScalar )1.0, ( btScalar )0.5, ( btScalar )1.0f ) ) );
	
	PositionChanged();
	OrientationChanged();
	ScaleChanged();
	LinearVelocityChanged();
	AngularVelocityChanged();
	GravityChanged();
	EnabledChanged();
	PropertiesChanged();
	ResponseTypeChanged();
	CollisionVolumeChanged();
}

debpColliderVolume::~debpColliderVolume(){
	pCleanUp();
}



// physics management
///////////////////////

void debpColliderVolume::CreateBody(){
	debpCollisionWorld *dynWorld = GetDynamicsWorld();
	
	if( dynWorld ){
		const int constraintCount = GetConstraintCount();
		int i;
		for( i=0; i<constraintCount; i++ ){
			GetConstraintAt( i )->SetDynamicsWorld( dynWorld );
		}
		
		GravityChanged();
		
		pPhyBody->SetDynamicsWorld( dynWorld );
		
// 		if( pGhostKinematicMovement ){
// 			pGhostKinematicMovement->SetDynamicsWorld( dynWorld );
// 		}
	}
}

void debpColliderVolume::DestroyBody(){
	const int constraintCount = GetConstraintCount();
	int i;
	for( i=0; i<constraintCount; i++ ){
		GetConstraintAt( i )->SetDynamicsWorld( NULL );
	}
	
// 	if( pGhostKinematicMovement ){
// 		pGhostKinematicMovement->SetDynamicsWorld( NULL );
// 	}
	pPhyBody->SetDynamicsWorld( NULL );
}

/*
void debpColliderVolume::ApplyConstantForce(){
	if( ! GetUseInSimulation() || ! pPhyCtrl || ! GetHasConstantForce() ) return;
	const decVector &force = pColliderVolume.GetConstantForce();
	pPhyCtrl->ApplyForce( force.x, force.y, force.z, false );
}
*/

void debpColliderVolume::UpdateFromBody(){
	if( ! pPhyBody->UpdateFromBody() ){
		return;
	}
	
	pPreventUpdate = true;
	
	pColliderVolume.SetPosition( pPhyBody->GetPosition() );
	pColliderVolume.SetOrientation( pPhyBody->GetOrientation() );
	pColliderVolume.SetLinearVelocity( pPhyBody->GetLinearVelocity() );
	pColliderVolume.SetAngularVelocity( pPhyBody->GetAngularVelocity() );
	
	pPreventUpdate = false;
	
	pUpdateAttachments( false );
}

void debpColliderVolume::UpdateExtends(){
	if( pShapes.GetShapeCount() == 0 ){
		SetExtends( decDVector(), decDVector() );
		return;
	}
	
	int s, shapeCount = pShapes.GetShapeCount();
	decDVector distance( pPredictDisp * 0.5 );
	decDVector minExtendShape, maxExtendShape;
	decDVector minExtendAll, maxExtendAll;
	debpDCollisionBox colBox;
	
	// update the collision volumes of the shapes
	UpdateShapes();
	
	// determine the bounding box of all collision volumes
	pShapes.GetShapeAt( 0 )->GetCollisionVolume()->GetEnclosingBox( &colBox );
	minExtendAll = colBox.GetCenter() - colBox.GetHalfSize();
	maxExtendAll = colBox.GetCenter() + colBox.GetHalfSize();
	
	for( s=1; s<shapeCount; s++ ){
		pShapes.GetShapeAt( s )->GetCollisionVolume()->GetEnclosingBox( &colBox );
		
		minExtendAll.SetSmallest( colBox.GetCenter() - colBox.GetHalfSize() );
		maxExtendAll.SetLargest( colBox.GetCenter() + colBox.GetHalfSize() );
	}
	
	// enlarge the bounding box at the correct sides by the predicted displacement
	if( distance.x < 0.0 ){
		minExtendAll.x += distance.x;
		
	}else{
		maxExtendAll.x += distance.x;
	}
	
	if( distance.y < 0.0 ){
		minExtendAll.y += distance.y;
		
	}else{
		maxExtendAll.y += distance.y;
	}
	
	if( distance.z < 0.0 ){
		minExtendAll.z += distance.z;
		
	}else{
		maxExtendAll.z += distance.z;
	}
	
	// set the extends we found
	SetExtends( minExtendAll, maxExtendAll );
}

void debpColliderVolume::PrepareForStep(){
	debpCollider::PrepareForStep();
DEBUG_RESET_TIMERS;
	pPreventAttNotify = true;
	RegisterColDetFinish(); // to disable pPreventAttNotify
	
	pUpdateBPShape();
DEBUG_PRINT_TIMER( "Update BP Shape" );
	pUpdateAttachments( false );
DEBUG_PRINT_TIMER( "Update Attachments" );
DEBUG_PRINT_TIMER_TOTAL( "PrepareForStep" );
}



void debpColliderVolume::DirtyBPShape(){
	pDirtyBPShape = true;
	
	RegisterColDetPrepare();
	RegisterColDetFinish();
}

void debpColliderVolume::DirtyAttachments(){
	pDirtyAttachments = true;
	
	RegisterColDetPrepare();
	RegisterColDetFinish();
}



void debpColliderVolume::ApplyGravity( float elapsed ){
	pLinVelo += pGravity * elapsed;
}

void debpColliderVolume::PredictDisplacement( float elapsed ){
	pPredictDisp = pLinVelo * elapsed;
	pPredictRot = pAngVelo * elapsed;
	MarkDirtyOctree();
}

void debpColliderVolume::InterpolatePosition( float percentage ){
	pColliderVolume.SetPosition( pPosition + pPredictDisp * percentage );
	pColliderVolume.SetLinearVelocity( pLinVelo );
	pPredictDisp.SetZero();
	
	if( pHasAngVelo ){
		pColliderVolume.SetOrientation( pOrientation * decQuaternion::CreateFromEuler( pPredictRot * percentage ) );
		pColliderVolume.SetAngularVelocity( pAngVelo );
		pPredictRot.SetZero();
	}
	
	pUpdateAttachments( false );
}

void debpColliderVolume::ApplyDisplacement(){
	pColliderVolume.SetPosition( pPosition + pPredictDisp );
	pColliderVolume.SetLinearVelocity( pLinVelo );
	pPredictDisp.SetZero();
	
	if( pHasAngVelo ){
		pColliderVolume.SetOrientation( pOrientation * decQuaternion::CreateFromEuler( pPredictRot ) );
		pColliderVolume.SetAngularVelocity( pAngVelo );
		pPredictRot.SetZero();
	}
	
	pUpdateAttachments( false );
}

void debpColliderVolume::ApplyFakeDynamicResponse( deCollisionInfo &colinfo ){
	const decVector &normal = colinfo.GetNormal();
	float impactVelocity = -( normal * pLinVelo );
	
	//if( impactVelocity > 0.1f ){ // more than 1 dm/s
	//	impactVelocity *= 1.2f; // bounce off with 20% of impact velocity
	//}
	
	pLinVelo += normal * impactVelocity;
	
	pLinVelo *= 0.95f; // some damping
}


#include "debpColliderComponent.h"
void debpColliderVolume::DetectCustomCollision( float elapsed ){
	DEBUG_RESET_TIMER2;
	if( ! GetUseKinematicSimulation() || ! GetIsMoving() ){
		return;
	}
	
	ApplyGravity( elapsed );
// 	if( pGhostKinematicMovement ){
// 		pGhostKinematicMovement->SetEnabled( GetUseKinematicSimulation() && GetIsMoving() );
// 	}
	
	if( ! pColliderVolume.GetEnabled() || pColliderVolume.GetCollisionFilter().CanNotCollide() ){
		// no collision possible so just move the collider
		pColliderVolume.SetPosition( pPosition + pLinVelo * elapsed );
		pColliderVolume.SetLinearVelocity( pLinVelo );
		pPredictDisp.SetZero();
		pPredictLinVelo.SetZero();
		
		pColliderVolume.SetAngularVelocity( pAngVelo );
		pPredictRot.SetZero();
		
		return;
	}
	
	dePhysicsBullet &bullet = *GetBullet();
	debpWorld &world = *GetParentWorld();
	deCollisionInfo *colinfo = world.GetCollisionInfo();
	debpCollisionWorld &dynamicsWorld = *world.GetDynamicsWorld();
	debpClosestConvexResultCallback colliderMoveHits;
	int stopIfCloseToHitPlane = 0;
	decVector unstuckVelocity;
	debpDCollisionBox colBox;
	
	int cspmax = 20;
	int cheapStuckPrevention = 0;
	float csphist[ cspmax + 1 ];
	float localElapsed = elapsed;
	
	//pUpdateBPShape();
	pUpdateSweepCollisionTest();
	
	while( localElapsed > 1e-6f ){
		PredictDisplacement( localElapsed );
		
		colliderMoveHits.Reset();
		colliderMoveHits.SetTestCollider( this );
		
		if( colliderMoveHits.MoveDirIsZero() && colliderMoveHits.RotationZero() ){
			break;
		}
		
// 		bullet.LogWarnFormat("CHECK %02d: collider=%p lv=(%f,%f,%f)", cheapStuckPrevention, &pColliderVolume,
// 			pColliderVolume.GetLinearVelocity().x, pColliderVolume.GetLinearVelocity().y,
// 			pColliderVolume.GetLinearVelocity().z );
		
		const btQuaternion btorientation( ( btScalar )pOrientation.x, ( btScalar )pOrientation.y,
			( btScalar )pOrientation.z, ( btScalar )pOrientation.w );
		const btTransform transformFrom( btorientation, colliderMoveHits.m_convexFromWorld );
		btTransform transformTo( btorientation, colliderMoveHits.m_convexToWorld );
		if( pHasAngVelo ){
			const decQuaternion toOrientation( pOrientation * decQuaternion::CreateFromEuler( pPredictRot ) );
			transformTo.setRotation( btQuaternion( ( btScalar )toOrientation.x, ( btScalar )toOrientation.y,
				( btScalar )toOrientation.z, ( btScalar )toOrientation.w ) );
		}
		
// 		GetBullet()->LogInfoFormat("DetectCustomCollision: Begin %g,%g,%g", colliderMoveHits.GetMoveDirection().x,
// 			colliderMoveHits.GetMoveDirection().y, colliderMoveHits.GetMoveDirection().z);
		if( false ){
		pSweepCollisionTest->SweepTest( *pGhostKinematicMovement, transformFrom, transformTo, colliderMoveHits );
		}else{
		pSweepCollisionTest->SweepTest( dynamicsWorld, transformFrom, transformTo, colliderMoveHits );
		}
// 		GetBullet()->LogInfoFormat("DetectCustomCollision: Result %g %d", colliderMoveHits.GetHitDistance(), colliderMoveHits.HasLocalCollision());
		
		if( ! colliderMoveHits.HasLocalCollision() ){
			ApplyDisplacement();
			break;
		}
		
		colliderMoveHits.GetResult( *colinfo );
#if 0
		if(colinfo->GetCollider()){
			if(colliderMoveHits.GetHitDistance() == 0.0f){
				const decVector &n = colinfo->GetNormal();
				const decVector &pd = pPredictDisp;
				const float a = acosf(n.Normalized() * pd.Normalized()) / DEG2RAD;
				
				GetBullet()->LogInfoFormat("DetectCustomCollision: Collision n(%g,%g,%g) pd(%g,%g,%g) a=%g",
					n.x, n.y, n.z, pd.x, pd.y, pd.z, a );
				
				const debpCollider &c = *((debpCollider*)colinfo->GetCollider()->GetPeerPhysics());
				if(c.IsComponent() && colinfo->GetShape() != -1 && colinfo->GetBone() == -1){
					debpColliderComponent &cc = *c.CastToComponent();
					const decDVector &ccp = cc.GetPosition();
					GetBullet()->LogInfoFormat("  Collider(%g,%g,%g) s=%d", ccp.x, ccp.y, ccp.z, colinfo->GetShape());
					const debpShape &s = *cc.GetRigShapes().GetShapeAt(colinfo->GetShape());
					if(s.GetType() == debpShape::estBox){
						const debpShapeBox &sb = (debpShapeBox&)s;
// 						const decDMatrix m(decDMatrix::CreateWorld(sb.GetShapeBox()->GetPosition(), sb.GetShapeBox()->GetOrientation()) * cc.GetMatrix());
						const decDVector sp(sb.GetShapeBox()->GetPosition()/*m.GetPosition()*/);
						const decDVector sr(sb.GetShapeBox()->GetOrientation().GetEulerAngles()/*m.GetEulerAngles()*/ / DEG2RAD);
						
						GetBullet()->LogInfoFormat("  Collider(%g,%g,%g) s=%d p(%g,%g,%g) o(%g,%g,%g)",
							ccp.x, ccp.y, ccp.z, colinfo->GetShape(), sp.x, sp.y, sp.z, sr.x, sr.y, sr.z);
					}
				}
			}
		}
#endif
		
		// check if the hit plane distance is small. if a collision is found the linear velocity
		// is updated by the game script. a second collision can happen which alters the linear
		// velocity again. if the two collision planes are in a bad position it happens that the
		// third collision will be close to the second. this can lead to the collider oscillating
		// between the collision points and never converge. the third collision though has the
		// last collider position close to the hit plane surface. if this distance to the hit
		// plane is less than 0.1mm then stop further testing
		if( ( colinfo->GetNormal() * pPredictDisp ) * colinfo->GetDistance() > -1.5e-4f ){
			if( ++stopIfCloseToHitPlane == 3 ){
				// this causes problems with situations where walls or floors have multiple
				// shapes interpenetrating to cover all spaces. if the shapes are not precisely
				// oriented this can cause a bunch of collisions at 0 or near 0 distance causing
				// this code to fire and preventing the player from moving at all. we assume the
				// game scripts do not reposition the collider by adding collision gaps. in this
				// situation skipping the check should be okay. otherwise find a better solution
// 				break;
			}
			
		}else{
			stopIfCloseToHitPlane = 0;
		}
		
		// otherwise continue with collision handling
		InterpolatePosition( colliderMoveHits.GetHitDistance() );
		const decDVector positionAtHit( pPosition );
		
		if( GetUseFakeDynamics() ){
			if( colliderMoveHits.GetHitDistance() < 0.001f ){
				break;
			}
			ApplyFakeDynamicResponse( *colinfo );
			
		}else{
			deColliderReference guard( &pColliderVolume ); // avoid collider being removed while in use
			
			colinfo->SetDistance( localElapsed * ( 1.0f - colliderMoveHits.GetHitDistance() ) );
			pColliderVolume.GetPeerScripting()->CollisionResponse( &pColliderVolume, colinfo ); // can potentially remove collider
			
			// if position is at collision point apply a small shift to avoid re-collision
			if( positionAtHit.IsEqualTo( pPosition, 1e-5 ) ){
// 				bullet.LogWarnFormat("shift normal=(%f,%f,%f)", colinfo->GetNormal().x, colinfo->GetNormal().y, colinfo->GetNormal().z );
				pColliderVolume.SetPosition( pPosition + decDVector( colinfo->GetNormal() ) * 1e-5 );
			}
			
			// update aabbs of both collision objects if still existing
			if( colinfo->GetCollider() ){
				( ( debpCollider* )colinfo->GetCollider()->GetPeerPhysics() )->UpdateCollisionObjectAABBs();
			}
			
			UpdateCollisionObjectAABBs();
			
			// if the reference counts drops to 1 the collider has been removed while in use
			// in this case release the collider and stop testing it
			if( pColliderVolume.GetRefCount() == 1 ){
				pColliderVolume.FreeReference();
				return;
			}
		}
		
		// check if the collision response is potentially leading to a collision again.
		// this is the case if the linear velocity is not zero and the angle between
		// linear velocity and hit normal is larger than 90 degrees minus a small threshold.
		// what we need to avoid is the case of the velocity running perpendicular to the
		// hit normal or worse slightly towards it. if this is the case clamp the linear
		// velocity to an angle slightly less than 90 degrees. this ensures the collider
		// is moving away from the surface in which case no collision will happen
		const decVector &respLinVelo = pColliderVolume.GetLinearVelocity();
		if( respLinVelo.LengthSquared() < 1e-6f ){
			// too small velocity. at 60Hz step size this is 0.01mm which is small.
			// we could even break at 0.01f which is 0.16mm at 60Hz
			break;
		}
		
		const decVector &hitNormal = colinfo->GetNormal();
		const float norRespLinVelo = respLinVelo * hitNormal;
// 		bullet.LogWarnFormat("ht=(%f,%f,%f) rlv=(%f,%f,%f) dist=%f dot=%f", hitNormal.x, hitNormal.y,
// 			hitNormal.z, respLinVelo.x, respLinVelo.y, respLinVelo.z, colliderMoveHits.GetHitDistance(), norRespLinVelo );
		
		if( norRespLinVelo < 1e-4f ){
			// the idea behind unstuckVelocity is this. if the collider is touching both a
			// ground collider and a perpendicular wall collider it is possible the collision
			// alternates between the two at 0 hit distance because the correction velocity
			// of both is perpendicular to the other and can trigger the exact same problem
			// we try to avoid here. by accumulating the unstuck velocities we can ensure
			// we get away also from such a situation
			unstuckVelocity += hitNormal * ( 0.001f - norRespLinVelo );
// 			bullet.LogWarnFormat("unstuckVelocity (%f,%f,%f)", unstuckVelocity.x, unstuckVelocity.y, unstuckVelocity.z );
			pColliderVolume.SetLinearVelocity( respLinVelo + unstuckVelocity );
		}
		
		localElapsed -= localElapsed * colliderMoveHits.GetHitDistance();
		
		csphist[ cheapStuckPrevention ] = colliderMoveHits.GetHitDistance();
		cheapStuckPrevention++;
		
		if( cheapStuckPrevention == cspmax ){
			const decDVector &position = pColliderVolume.GetPosition();
			const decVector rotation( decMatrix::CreateFromQuaternion(
				pColliderVolume.GetOrientation() ).GetEulerAngles() / DEG2RAD );
			const decVector &lvelo = pColliderVolume.GetLinearVelocity();
			const decVector avelo( pColliderVolume.GetAngularVelocity() / DEG2RAD );
			int i;
			
			pColliderVolume.SetLinearVelocity( decVector() );
			pColliderVolume.SetAngularVelocity( decVector() );
			
			bullet.LogWarnFormat( "STUCK! collider=%p responseType=%i",
				&pColliderVolume, pColliderVolume.GetResponseType() );
			bullet.LogWarnFormat( "   position=(%f,%f,%f) orientation=(%f,%f,%f)",
				position.x, position.y, position.z, rotation.x, rotation.y, rotation.z );
			bullet.LogWarnFormat( "   linearVelocity=(%f,%f,%f) angularVelocity=(%f,%f,%f)",
				lvelo.x, lvelo.y, lvelo.z, avelo.x, avelo.y, avelo.z );
			bullet.LogWarnFormat( "   elapsed=%f localElapsed=%f", elapsed, localElapsed );
				
			decString text( "   history=[" );
			for( i=0; i<cspmax; i++ ){
				text.AppendFormat( "%s%f", i==0?"":",", csphist[i] );
			}
			text.Append( "]" );
			bullet.LogWarn( text );
			break;
		}
	}
	DEBUG_PRINT_TIMER2( "ColliderVolume DetectCustomCollision" );
}

void debpColliderVolume::PrepareDetection( float elapsed ){
	debpCollider::PrepareDetection( elapsed );
	//float linearDamping = 0.1f;
	//pLinVelo *= powf( 1.0f - linearDamping, elapsed );
	/*
	if( pColliderVolume.GetResponseType() == deCollider::ertDynamic ){
		GetBullet()->LogInfoFormat( "prepdet col=%p ufd=%i uks=%i im=%i", pColliderVolume, GetUseFakeDynamics()?1:0, GetUseKinematicSimulation()?1:0, GetIsMoving()?1:0 );
	}
	*/
	/*if( fabs(pColliderVolume.GetPosition().x-0.672262)<0.001 && fabs(pColliderVolume.GetPosition().z-14.855800)<0.001 ){
		GetBullet()->LogInfoFormat( "prepdet ufd=%i uks=%i im=%i", GetUseFakeDynamics()?1:0, GetUseKinematicSimulation()?1:0, GetIsMoving()?1:0 );
	}*/
}

void debpColliderVolume::FinishDetection(){
	debpCollider::FinishDetection();
	pPreventAttNotify = false;
}

void debpColliderVolume::UpdateCollisionObjectAABBs(){
	pPhyBody->UpdateAABB();
	
// 	if( pGhostKinematicMovement ){
// 		pGhostKinematicMovement->UpdateAABB();
// 	}
}



void debpColliderVolume::UpdateShapes(){
	if( ! pDirtyShapes ){
		return;
	}
	
	pShapes.UpdateWithMatrix( GetMatrix() );
	UpdateShapeExtends();
	
	pDirtyShapes = false;
}

void debpColliderVolume::UpdateShapesWithMatrix( const decDMatrix &transformation ){
	pDirtyShapes = true;
	
	pShapes.UpdateWithMatrix( transformation );
	UpdateShapeExtends();
}

void debpColliderVolume::UpdateShapeExtends(){
	if( pShapes.GetShapeCount() == 0 ){
		SetShapeExtends( decDVector(), decDVector() );
		return;
	}
	
	int s, shapeCount = pShapes.GetShapeCount();
	decDVector distance( pPredictDisp * 0.5 );
	decDVector minExtendShape, maxExtendShape;
	decDVector minExtendAll, maxExtendAll;
	debpDCollisionBox colBox;
	
	// determine the bounding box of all collision volumes
	pShapes.GetShapeAt( 0 )->GetCollisionVolume()->GetEnclosingBox( &colBox );
	minExtendAll = colBox.GetCenter() - colBox.GetHalfSize();
	maxExtendAll = colBox.GetCenter() + colBox.GetHalfSize();
	
	for( s=1; s<shapeCount; s++ ){
		pShapes.GetShapeAt( s )->GetCollisionVolume()->GetEnclosingBox( &colBox );
		
		minExtendAll.SetSmallest( colBox.GetCenter() - colBox.GetHalfSize() );
		maxExtendAll.SetLargest( colBox.GetCenter() + colBox.GetHalfSize() );
	}
	
	// set the extends we found
	SetShapeExtends( minExtendAll, maxExtendAll );
}



debpSweepCollisionTest *debpColliderVolume::GetSweepCollisionTest(){
	pUpdateSweepCollisionTest();
	return pSweepCollisionTest;
}

btGhostObject* debpColliderVolume::GetStaticCollisionTest(){
	pUpdateStaticCollisionTest();
	return pStaticCollisionTest;
}



// Debugging
//////////////

void debpColliderVolume::UpdateDDSShape(){
	deDebugDrawerShape * const ddshape = GetDDSShape();
	if( ! ddshape ){
		return;
	}
	
	ddshape->GetShapeList() = pColliderVolume.GetShapes();
	GetDebugDrawer()->NotifyShapeContentChanged();
}



// forces
///////////

void debpColliderVolume::ApplyImpuls( const decVector &impuls ){
	pPhyBody->ApplyImpuls( impuls );
}

void debpColliderVolume::ApplyImpulsAt( const decVector &impuls, const decVector &position ){
	pPhyBody->ApplyImpulsAt( impuls, position );
}

void debpColliderVolume::ApplyTorqueImpuls( const decVector &torqueImpuls ){
	pPhyBody->ApplyTorqueImpuls( torqueImpuls );
}

void debpColliderVolume::ApplyForce( const decVector &force ){
	pPhyBody->ApplyForce( force );
}

void debpColliderVolume::ApplyForceAt( const decVector &force, const decVector &position ){
	pPhyBody->ApplyForceAt( force, position );
}

void debpColliderVolume::ApplyTorque( const decVector &torque ){
	pPhyBody->ApplyTorque( torque );
}



// notifications
//////////////////

void debpColliderVolume::PositionChanged(){
	const decDVector &position = pColliderVolume.GetPosition();
	if( pPosition.IsEqualTo( position ) ){
		return;
	}
	
	debpCollider::PositionChanged();
	
	pPosition = position;
	
	pDirtyShapes = true;
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		RequiresUpdate();
		
		pPhyBody->SetPosition( position );
// 		if( pGhostKinematicMovement ){
// 			pGhostKinematicMovement->SetPosition( position );
// 		}
	}
	
	if( pColliderVolume.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderVolume::OrientationChanged(){
	const decQuaternion &orientation = pColliderVolume.GetOrientation();
	if( pOrientation.IsEqualTo( orientation ) ){
		return;
	}
	
	debpCollider::OrientationChanged();
	
	pOrientation = orientation;
	
	pDirtyShapes = true;
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		RequiresUpdate();
		
		pPhyBody->SetOrientation( orientation );
	}
	
	if( pColliderVolume.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderVolume::ScaleChanged(){
	const decVector &scale = pColliderVolume.GetScale();
	
	if( pScale.IsEqualTo( scale ) ){
		return;
	}
	
	pScale = scale;
	
	pDirtyShapes = true;
	pDirtyBPShape = true;
	pDirtySweepTest = true;
	pDirtyStaticTest = true;
	
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		RequiresUpdate();
	}
	
	if( pColliderVolume.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderVolume::GeometryChanged(){
	const decDVector &position = pColliderVolume.GetPosition();
	const decQuaternion &orientation = pColliderVolume.GetOrientation();
	const decVector &scale = pColliderVolume.GetScale();
	const bool sameScale = pScale.IsEqualTo( scale );
	if( pPosition.IsEqualTo( position ) && pOrientation.IsEqualTo( orientation ) && sameScale ){
		return;
	}
	
	debpCollider::GeometryChanged();
	
	pPosition = position;
	pOrientation = orientation;
	pScale = scale;
	
	pDirtyShapes = true;
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! sameScale ){
		pDirtyBPShape = true;
		pDirtySweepTest = true;
		pDirtyStaticTest = true;
	}
	
	if( ! pPreventUpdate ){
		RequiresUpdate();
		
		pPhyBody->SetPosition( position );
		pPhyBody->SetOrientation( orientation );
	}
	
	if( pColliderVolume.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderVolume::LinearVelocityChanged(){
	const decVector &linVelo = pColliderVolume.GetLinearVelocity();
	if( pLinVelo.IsEqualTo( linVelo ) ){
		return;
	}
	
	if( ! pPreventUpdate ){
		RequiresUpdate();
		
		pPhyBody->SetLinearVelocity( linVelo );
	}
	
	pLinVelo = linVelo;
	pHasLinVelo = linVelo * linVelo > 1e-6f; //1e-4f;
	pUpdateIsMoving();
	
	MarkDirtyOctree();
}

void debpColliderVolume::AngularVelocityChanged(){
	const decVector &angVelo = pColliderVolume.GetAngularVelocity();
	if( pAngVelo.IsEqualTo( angVelo ) ){
		return;
	}
	
	if( ! pPreventUpdate ){
		RequiresUpdate();
		
		pPhyBody->SetAngularVelocity( angVelo );
	}
	
	pAngVelo = angVelo;
	pHasAngVelo = angVelo * angVelo > 0.0003046174f; //3.046174e-06f; //0.03046174f;
	pUpdateIsMoving();
	
	MarkDirtyOctree();
}

void debpColliderVolume::GravityChanged(){
	if( pColliderVolume.GetUseLocalGravity() ){
		pGravity = pColliderVolume.GetGravity();
		pHasGravity = pGravity * pGravity > 1e-6f; //1e-4f;
		
	}else{
		debpWorld *bpWorld = GetParentWorld();
		if( bpWorld ){
			pGravity = bpWorld->GetWorld().GetGravity();
			pHasGravity = pGravity * pGravity > 1e-6f; //1e-4f;
			
		}else{
			pGravity.SetZero();
			pHasGravity = false;
		}
	}
	pUpdateIsMoving();
	
	pPhyBody->SetGravity( pGravity );
//	GetBullet()->LogInfoFormat( "gravity col=%p gravity=(%f,%f,%f) pos=(%f,%f,%f)", pColliderVolume, pGravity.x, pGravity.y, pGravity.z,
//		pColliderVolume.GetPosition().x, pColliderVolume.GetPosition().y, pColliderVolume.GetPosition().z );
}

void debpColliderVolume::EnabledChanged(){
	pPhyBody->SetEnabled( pColliderVolume.GetEnabled() );
//	GetBullet()->LogInfoFormat( "enabled col=%p ena=%i pos=(%f,%f,%f)", pColliderVolume, pColliderVolume.GetEnabled()?1:0,
//		pColliderVolume.GetPosition().x, pColliderVolume.GetPosition().y, pColliderVolume.GetPosition().z );
}

void debpColliderVolume::PropertiesChanged(){
	pPhyBody->SetMass( pColliderVolume.GetMass() );
//	GetBullet()->LogInfoFormat( "properties col=%p pos=(%f,%f,%f)", pColliderVolume, pColliderVolume.GetPosition().x,
//		pColliderVolume.GetPosition().y, pColliderVolume.GetPosition().z );
}

void debpColliderVolume::ResponseTypeChanged(){
	int responseType = pColliderVolume.GetResponseType();
	
	if( responseType == deCollider::ertStatic ){
		pPhyBody->SetResponseType( debpPhysicsBody::ertStatic );
		SetUseKinematicSimulation( false );
		
	}else if( responseType == deCollider::ertKinematic || pUseFakeDynamics ){
		pPhyBody->SetResponseType( debpPhysicsBody::ertKinematic );
		SetUseKinematicSimulation( true );
		
	}else{
		pPhyBody->SetResponseType( debpPhysicsBody::ertDynamic );
		SetUseKinematicSimulation( false );
	}
	
// 	if( pGhostKinematicMovement ){
// 		pGhostKinematicMovement->SetEnabled( GetUseKinematicSimulation() && GetIsMoving() );
// 	}
}

void debpColliderVolume::CollisionFilterChanged(){
	if( pPhyBody ){
		pPhyBody->CollisionFilteringChanged();
	}
}

void debpColliderVolume::IgnoreCollidersChanged(){
	CollisionFilterChanged();
}

void debpColliderVolume::CollisionVolumeChanged(){
	DirtyBPShape();
	pDirtySweepTest = true;
	pDirtyStaticTest = true;
	pRebuildShapeList();
	
	MarkDirtyOctree();
	SetAddToOctree( pColliderVolume.GetShapes().GetCount() > 0 );
	
	UpdateDDSShape(); // debugging if enabled
	
//	GetBullet()->LogInfoFormat( "colvol col=%p pos=(%f,%f,%f)", pColliderVolume, pColliderVolume.GetPosition().x,
//		pColliderVolume.GetPosition().y, pColliderVolume.GetPosition().z );
}

void debpColliderVolume::AttachmentAdded( int index, deColliderAttachment *attachment ){
	debpCollider::AttachmentAdded( index, attachment );
	DirtyAttachments();
}

void debpColliderVolume::AttachmentChanged( int index, deColliderAttachment *attachment ){
	debpCollider::AttachmentChanged( index, attachment );
	DirtyAttachments();
}

void debpColliderVolume::AttachmentRemoved( int index, deColliderAttachment *attachment ){
	debpCollider::AttachmentRemoved( index, attachment );
	DirtyAttachments();
}

void debpColliderVolume::AllAttachmentsRemoved(){
	debpCollider::AllAttachmentsRemoved();
	DirtyAttachments();
}

void debpColliderVolume::AttachmentsForceUpdate(){
	pUpdateAttachments( true );
}



void debpColliderVolume::ConstraintAdded( int index, deColliderConstraint* constraint )
{
	debpCollider::ConstraintAdded( index, constraint );
	
	debpColliderConstraint &bpConstraint = *GetConstraintAt( index );
	bpConstraint.SetDynamicsWorld( GetDynamicsWorld() );
	bpConstraint.SetEnabled( pColliderVolume.GetEnabled() );
	
	ConstraintChanged( index, constraint );
}

void debpColliderVolume::ConstraintChanged( int index, deColliderConstraint* constraint )
{
	debpCollider::ConstraintChanged( index, constraint );
	
	debpColliderConstraint &bpConstraint = *GetConstraintAt( index );
	if( pPhyBody != bpConstraint.GetFirstBody() ){
		bpConstraint.SetFirstBody( pPhyBody );
	}
	
	bpConstraint.ConstraintChanged();
}



// Collision Detection
////////////////////////

bool debpColliderVolume::PointInside( const decDVector &point ){
	UpdateShapes();
	
	const int shapeCount = pShapes.GetShapeCount();
	int i;
	
	for( i=0; i<shapeCount; i++ ){
		const debpShape &shape = *pShapes.GetShapeAt( i );
		if( shape.GetCollisionVolume()->IsPointInside( point ) ){
			return true;
		}
	}
	
	return false;
}

void debpColliderVolume::RayHits( const decDVector &rayOrigin, const decVector &rayDirection, deBaseScriptingCollider *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
#define BULLET_RAY_CAST_UNSTABLE 1
	
	debpCollisionDetection &coldet = GetBullet()->GetCollisionDetection();
	deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
	
	const btVector3 btRayFrom( ( btScalar )rayOrigin.x, ( btScalar )rayOrigin.y, ( btScalar )rayOrigin.z );
	const decDVector rayTo( rayOrigin + rayDirection );
	const btVector3 btRayTo( ( btScalar )rayTo.x, ( btScalar )rayTo.y, ( btScalar )rayTo.z );
	const btQuaternion btOrientation( ( btScalar )0, ( btScalar )0, ( btScalar )0, ( btScalar )1 );
	
	const btTransform rayFromTrans( btOrientation, btRayFrom );
	const btTransform rayToTrans( btOrientation, btRayTo );
	
	// prepre static collision test to be in the right spot
	btGhostObject * const staticCollisionTest = GetStaticCollisionTest();
	const btVector3 btColPos( ( btScalar )pPosition.x, ( btScalar )pPosition.y, ( btScalar )pPosition.z );
	const btQuaternion btColOrien( ( btScalar )pOrientation.x, ( btScalar )pOrientation.y,
		( btScalar )pOrientation.z, ( btScalar )pOrientation.w );
	staticCollisionTest->setWorldTransform( btTransform( btColOrien, btColPos ) );
	
	// do collision test
	#ifdef BULLET_RAY_CAST_UNSTABLE
		// bullet has a broken ray-box test implementation using Gjk which has a tendency
		// to miss collisions half of the time. as a quick fix a sweep test is done with
		// a tiny sphere which yields a comparable result but is not prone to the problem
		debpConvexResultCallback result( colinfo );
		result.SetTestRay( NULL, listener );
		colinfo->SetStopTesting( false );
		coldet.GetRayHackShape().SweepTest( *this, rayFromTrans, rayToTrans, result );
			// BUG! SweepTest uses collider.GetDynamicsWorld() on *this which is null-derefence
			//      if used by Navigator.pathCollideRay on collider not added to world (for
			//      example test if path passes through a collider)
		
	#else
		// update abbbs?
		debpRayResultCallback result( colinfo );
		result.SetTestRay( rayOrigin, rayDirection, NULL, 0, listener );
		colinfo->SetStopTesting( false );
		
		debpCollisionWorld::rayTestSingle( rayFromTrans, rayToTrans, staticCollisionTest,
			staticCollisionTest->getCollisionShape(), staticCollisionTest->getWorldTransform(), result );
	#endif
}

static btManifoldPoint vDummyManifoldPoint; // Dummy manifold point to be used where bullet requires one but dragengine not

void debpColliderVolume::ColliderHits( deCollider *engCollider, deBaseScriptingCollider *listener ){
	if( ! engCollider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	// prepre static collision test to be in the right spot
	btGhostObject * const staticCollisionTest = GetStaticCollisionTest();
	const btVector3 btColPos( ( btScalar )pPosition.x, ( btScalar )pPosition.y, ( btScalar )pPosition.z );
	const btQuaternion btColOrien( ( btScalar )pOrientation.x, ( btScalar )pOrientation.y,
		( btScalar )pOrientation.z, ( btScalar )pOrientation.w );
	staticCollisionTest->setWorldTransform( btTransform( btColOrien, btColPos ) );
	
	// do collision test
	
	// update abbbs?
	
	debpCollisionDetection &coldet = GetBullet()->GetCollisionDetection();
	debpCollider * const collider = ( debpCollider* )engCollider->GetPeerPhysics();
	deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
	debpCollisionResult coldetResult;
	
	debpContactResultCallback result( colinfo );
	
	if( collider->IsVolume() ){
		debpColliderVolume &colliderVolume = *collider->CastToVolume();
		btGhostObject * const staticCollisionTest = colliderVolume.GetStaticCollisionTest();
		const btCollisionObjectWrapper obj0Wrap( NULL, staticCollisionTest->getCollisionShape(),
			staticCollisionTest, staticCollisionTest->getWorldTransform(), -1, -1 );
		
		const decDVector &position = colliderVolume.GetPosition();
		const btVector3 btposition( ( btScalar )position.x, ( btScalar )position.y, ( btScalar )position.z );
		
		const decQuaternion &orientation = colliderVolume.GetOrientation();
		const btQuaternion btorientation( ( btScalar )orientation.x, ( btScalar )orientation.y,
			( btScalar )orientation.z, ( btScalar )orientation.w );
		
		staticCollisionTest->setWorldTransform( btTransform( btorientation, btposition ) );
		
		result.SetTestCollider( staticCollisionTest, collider, listener );
		colinfo->SetStopTesting( false );
		
		collider->UpdateShapes(); // the collider shapes can be messed up. we restore them back to the world relative state
		
		btVector3 aabbMin, aabbMax;
		staticCollisionTest->getCollisionShape()->getAabb( staticCollisionTest->getWorldTransform(), aabbMin, aabbMax );
		
		if( coldet.ColliderHitsCollider( collider, this, coldetResult ) ){
			const btCollisionObjectWrapper obj1Wrap( NULL, staticCollisionTest->getCollisionShape(),
				staticCollisionTest, staticCollisionTest->getWorldTransform(), -1, -1 );
			result.addSingleResult( vDummyManifoldPoint, &obj0Wrap, 0, -1, &obj1Wrap, 0, coldetResult.face );
		}
	}
}

void debpColliderVolume::ColliderMoveHits( deCollider *engCollider, const decVector &displacement, deBaseScriptingCollider *listener ){
	if( ! engCollider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	// prepre static collision test to be in the right spot
	btGhostObject * const staticCollisionTest = GetStaticCollisionTest();
	const btVector3 btColPos( ( btScalar )pPosition.x, ( btScalar )pPosition.y, ( btScalar )pPosition.z );
	const btQuaternion btColOrien( ( btScalar )pOrientation.x, ( btScalar )pOrientation.y,
		( btScalar )pOrientation.z, ( btScalar )pOrientation.w );
	staticCollisionTest->setWorldTransform( btTransform( btColOrien, btColPos ) );
	
	// do collision test
	debpCollisionDetection &coldet = GetBullet()->GetCollisionDetection();
	deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
	debpCollider *collider = ( debpCollider* )engCollider->GetPeerPhysics();
	
	debpConvexResultCallback result( colinfo );
	result.SetTestCollider( collider, listener );
	colinfo->SetStopTesting( false );
	
	if( collider->IsVolume() ){
		debpColliderVolume &colliderVolume = *collider->CastToVolume();
		debpSweepCollisionTest &sweepCollisionTest = *colliderVolume.GetSweepCollisionTest();
		
		if( sweepCollisionTest.GetShapeList().GetCount() > 0 ){
			const decDVector &from = colliderVolume.GetPosition();
			const btVector3 btfrom( ( btScalar )from.x, ( btScalar )from.y, ( btScalar )from.z );
			
			const decDVector to( from + displacement );
			const btVector3 btto( ( btScalar )to.x, ( btScalar )to.y, ( btScalar )to.z );
			
			const decQuaternion &orientation = colliderVolume.GetOrientation();
			const btQuaternion btorientation( ( btScalar )orientation.x, ( btScalar )orientation.y,
				( btScalar )orientation.z, ( btScalar )orientation.w );
			
			const btTransform transformFrom( btorientation, btfrom );
			const btTransform transformTo( btorientation, btto );
			
			sweepCollisionTest.SweepTest( *this, transformFrom, transformTo, result );
		}
	}
}

void debpColliderVolume::ColliderRotateHits( deCollider *engCollider, const decVector &rotation, deBaseScriptingCollider *listener ){
	if( ! engCollider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	// prepre static collision test to be in the right spot
	btGhostObject * const staticCollisionTest = GetStaticCollisionTest();
	const btVector3 btColPos( ( btScalar )pPosition.x, ( btScalar )pPosition.y, ( btScalar )pPosition.z );
	const btQuaternion btColOrien( ( btScalar )pOrientation.x, ( btScalar )pOrientation.y,
		( btScalar )pOrientation.z, ( btScalar )pOrientation.w );
	staticCollisionTest->setWorldTransform( btTransform( btColOrien, btColPos ) );
	
	// do collision test
	debpCollisionDetection &coldet = GetBullet()->GetCollisionDetection();
	deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
	debpCollider *collider = ( debpCollider* )engCollider->GetPeerPhysics();
	
	debpConvexResultCallback result( colinfo );
	result.SetTestCollider( collider, listener );
	colinfo->SetStopTesting( false );
	
	if( collider->IsVolume() ){
		debpColliderVolume &colliderVolume = *collider->CastToVolume();
		debpSweepCollisionTest &sweepCollisionTest = *colliderVolume.GetSweepCollisionTest();
		
		if( sweepCollisionTest.GetShapeList().GetCount() > 0 ){
			const decDVector &position = colliderVolume.GetPosition();
			const btVector3 btposition( ( btScalar )position.x, ( btScalar )position.y, ( btScalar )position.z );
			
			const decQuaternion &orientationFrom = colliderVolume.GetOrientation();
			const btQuaternion btRotFrom( ( btScalar )orientationFrom.x, ( btScalar )orientationFrom.y,
				( btScalar )orientationFrom.z, ( btScalar )orientationFrom.w );
			
			const decQuaternion orientationTo = orientationFrom * decQuaternion::CreateFromEuler( rotation );
			const btQuaternion btRotTo( ( btScalar )orientationTo.x, ( btScalar )orientationTo.y,
				( btScalar )orientationTo.z, ( btScalar )orientationTo.w );
			
			const btTransform transformFrom( btRotFrom, btposition );
			const btTransform transformTo( btRotTo, btposition );
			
			sweepCollisionTest.SweepTest( *this, transformFrom, transformTo, result );
		}
	}
}

void debpColliderVolume::ColliderMoveRotateHits( deCollider *engCollider, const decVector &displacement,
const decVector &rotation, deBaseScriptingCollider *listener ){
	if( ! engCollider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	// prepre static collision test to be in the right spot
	btGhostObject * const staticCollisionTest = GetStaticCollisionTest();
	const btVector3 btColPos( ( btScalar )pPosition.x, ( btScalar )pPosition.y, ( btScalar )pPosition.z );
	const btQuaternion btColOrien( ( btScalar )pOrientation.x, ( btScalar )pOrientation.y,
		( btScalar )pOrientation.z, ( btScalar )pOrientation.w );
	staticCollisionTest->setWorldTransform( btTransform( btColOrien, btColPos ) );
	
	// do collision test
	debpCollisionDetection &coldet = GetBullet()->GetCollisionDetection();
	deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
	debpCollider *collider = ( debpCollider* )engCollider->GetPeerPhysics();
	
	debpConvexResultCallback result( colinfo );
	result.SetTestCollider( collider, listener );
	colinfo->SetStopTesting( false );
	
	if( collider->IsVolume() ){
		debpColliderVolume &colliderVolume = *collider->CastToVolume();
		debpSweepCollisionTest &sweepCollisionTest = *colliderVolume.GetSweepCollisionTest();
		
		if( sweepCollisionTest.GetShapeList().GetCount() > 0 ){
			const decDVector &positionFrom = colliderVolume.GetPosition();
			const btVector3 btPosFrom( ( btScalar )positionFrom.x, ( btScalar )positionFrom.y, ( btScalar )positionFrom.z );
			
			const decDVector positionTo( positionFrom + decDVector( displacement ) );
			const btVector3 btPosTo( ( btScalar )positionTo.x, ( btScalar )positionTo.y, ( btScalar )positionTo.z );
			
			const decQuaternion &orientationFrom = colliderVolume.GetOrientation();
			const btQuaternion btRotFrom( ( btScalar )orientationFrom.x, ( btScalar )orientationFrom.y,
				( btScalar )orientationFrom.z, ( btScalar )orientationFrom.w );
			
			const decQuaternion orientationTo = orientationFrom * decQuaternion::CreateFromEuler( rotation );
			const btQuaternion btRotTo( ( btScalar )orientationTo.x, ( btScalar )orientationTo.y,
				( btScalar )orientationTo.z, ( btScalar )orientationTo.w );
			
			const btTransform transformFrom( btRotFrom, btPosFrom );
			const btTransform transformTo( btRotTo, btPosTo );
			
			sweepCollisionTest.SweepTest( *this, transformFrom, transformTo, result );
		}
	}
}



// private functions
//////////////////////

void debpColliderVolume::pCleanUp(){
	DestroyBody();
	if( pGhostKinematicMovement ){
		delete pGhostKinematicMovement;
	}
	if( pPhyBody ){
		delete pPhyBody;
	}
	
	if( pStaticCollisionTest ){
		delete pStaticCollisionTest;
	}
	if( pSweepCollisionTest ){
		delete pSweepCollisionTest;
	}
	if( pStaticCollisionTestShape ){
		pStaticCollisionTestShape->FreeReference();
	}
}



void debpColliderVolume::pRebuildShapeList(){
	const decShapeList &shapes = pColliderVolume.GetShapes();
	int s, shapeCount = shapes.GetCount();
	debpCreateShape createShape;
	
	pShapes.RemoveAllShapes();
	
	for( s=0; s<shapeCount; s++ ){
		shapes.GetAt( s )->Visit( createShape );
		if( createShape.GetCreatedShape() ){
			pShapes.AddShape( createShape.GetCreatedShape() );
			createShape.SetCreatedShape( NULL );
		}
	}
	
	pUpdateUseFakeDynamics();
	
	pDirtyShapes = true;
}

void debpColliderVolume::pUpdateSweepCollisionTest(){
	if( ! pSweepCollisionTest ){
		pSweepCollisionTest = new debpSweepCollisionTest( GetBullet()->GetCollisionDetection() );
		pDirtySweepTest = true;
	}
	
	if( pDirtySweepTest ){
		const decShapeList &shapes = pColliderVolume.GetShapes();
		const decVector &scale = pColliderVolume.GetScale();
		const int count = shapes.GetCount();
		int i;
		
		pSweepCollisionTest->RemoveAllShapes();
		
		for( i=0; i<count; i++ ){
			pSweepCollisionTest->AddShape( *shapes.GetAt( i ), scale );
		}
		
		pDirtySweepTest = false;
	}
}

void debpColliderVolume::pUpdateStaticCollisionTest(){
	if( ! pStaticCollisionTest ){
		pStaticCollisionTest = new btGhostObject;
		pStaticCollisionTest->setUserPointer( &pStaticCollisionTestObject );
		pDirtyStaticTest = true;
	}
	
	if( ! pDirtyStaticTest ){
		return;
	}
	
	if( pStaticCollisionTestShape ){
		pStaticCollisionTestShape->FreeReference();
		pStaticCollisionTestShape = NULL;
	}
	
	try{
		if( pColliderVolume.GetShapes().GetCount() > 0 ){
			pStaticCollisionTestShape = pCreateBPShape(); // take over reference
		}
		
		if( pStaticCollisionTestShape->GetShape() ){
			pStaticCollisionTest->setCollisionShape( pStaticCollisionTestShape->GetShape() );
			
		}else{
			pStaticCollisionTest->setCollisionShape( NULL );
		}
		
	}catch( const deException & ){
		if( pStaticCollisionTestShape ){
			pStaticCollisionTestShape->FreeReference();
			pStaticCollisionTestShape = NULL;
		}
		throw;
	}
	
	pDirtyStaticTest = false;
}

void debpColliderVolume::pUpdateUseFakeDynamics(){
	if( pShapes.GetShapeCount() > 0 ){
		bool useFakeDynamics = false;
		debpDCollisionBox boundingBox;
		
		// this is now a big time hack here. bullet should properly handle dynamic objects
		// but fails to do so in certain situations since CCD is not reactivated yet. to
		// deal with this situation fake kinematic simulation is used. for this we have to
		// check if the bounding box is not too small in any of the directions. the shapes
		// used here though give the transformed collision shapes but we need the original
		// one here. to avoid extra code which later on will be removed once bullet works
		// as expected a hack is used. we calculate the bounding box using a fake collision
		// volume but not using the one in the shape to avoid lots of calculations. for this
		// though we have to check which shape type we have. hard coded stuff like this is
		// bad but since it's just a temporary hack it will be tolerated for the time being.
		debpShape *shape = pShapes.GetShapeAt( 0 );
		
		if( shape->GetType() == debpShape::estSphere ){
			const decShapeSphere &sphere = *( ( debpShapeSphere* )shape )->GetShapeSphere();
			debpDCollisionSphere colvol( decDVector( sphere.GetPosition() ), ( double )sphere.GetRadius() );
			colvol.GetEnclosingBox( &boundingBox );
			
			const decDVector &halfSize = boundingBox.GetHalfSize();
			useFakeDynamics = ( halfSize.x < 0.05 ) || ( halfSize.y < 0.05 ) || ( halfSize.z < 0.05 );
			
		}else if( shape->GetType() == debpShape::estBox ){
			const decShapeBox &box = *( ( debpShapeBox* )shape )->GetShapeBox();
			debpDCollisionBox colvol( decDVector( box.GetPosition() ), decDVector( box.GetHalfExtends() ), box.GetOrientation() );
			colvol.GetEnclosingBox( &boundingBox );
			
			const decDVector &halfSize = boundingBox.GetHalfSize();
			useFakeDynamics = ( halfSize.x < 0.05 ) || ( halfSize.y < 0.05 ) || ( halfSize.z < 0.05 );
			
		} // cylinders and capsules are anyways not supported yet
		
		if( useFakeDynamics != pUseFakeDynamics ){
			pUseFakeDynamics = useFakeDynamics;
			ResponseTypeChanged();
		}
		/*
		GetBullet()->LogInfoFormat( "uufd collider=%p c=(%f,%f,%f) hs=(%f,%f,%f) ufd=%i", pColliderVolume, boundingBox.GetCenter().x,
			boundingBox.GetCenter().y, boundingBox.GetCenter().z, halfSize.x, halfSize.y, halfSize.z, pUseFakeDynamics?1:0 );
		*/
		/*if( fabs(pColliderVolume.GetPosition().x-0.672262)<0.001 && fabs(pColliderVolume.GetPosition().z-14.855800)<0.001 ){
			GetBullet()->LogInfoFormat( "box c=(%f,%f,%f) hs=(%f,%f,%f) ufd=%i", box.GetCenter().x, box.GetCenter().y, box.GetCenter().z, halfSize.x, halfSize.y, halfSize.z, useFakeDyn?1:0 );
		}*/
	}
}



void debpColliderVolume::pUpdateBPShape(){
	if( ! pDirtyBPShape ){
		return;
	}
	
	debpBulletShape *shape = NULL;
	try{
		if( pColliderVolume.GetShapes().GetCount() > 0 ){
			shape = pCreateBPShape(); // take over reference
		}
		pPhyBody->SetShape( shape );
		if( shape ){
			shape->FreeReference();
		}
		
	}catch( const deException & ){
		if( shape ){
			shape->FreeReference();
		}
		throw;
	}
	
	pDirtyBPShape = false;
}

debpBulletShape *debpColliderVolume::pCreateBPShape(){
	const decShapeList &shapes = pColliderVolume.GetShapes();
	const int count = shapes.GetCount();

	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	debpCreateBulletShape createBulletShape;
	int i;
	
	createBulletShape.SetScale( pColliderVolume.GetScale() );
	
//	GetBullet()->LogInfo( "volule create shape" );
	
	for( i=0; i<count; i++ ){
		createBulletShape.SetShapeIndex( i );
		shapes.GetAt( i )->Visit( createBulletShape );
	}
	
	//pPhyBody->SetCcdParameters( visCreateBody.GetCcdThreshold(), visCreateBody.GetCcdRadius() );
	
	debpBulletShape * const bulletShape = createBulletShape.GetBulletShape();
	if( bulletShape ){
		bulletShape->AddReference(); // otherwise visitor destructor frees created shape
	}
	return bulletShape;
}

void debpColliderVolume::pUpdateAttachments( bool force ){
	if( ! pDirtyAttachments && ! force ){
		return;
	}
	
	const int count = pColliderVolume.GetAttachmentCount();
	const decDMatrix &posMatrix = GetMatrix();
	int i;
	
	// adjust all attachments
	for( i=0; i<count; i++ ){
		debpColliderAttachment &bpAttachment = *GetAttachmentAt( i );
		const deColliderAttachment &attachment = *bpAttachment.GetAttachment();
		
		if( attachment.GetAttachType() == deColliderAttachment::eatRelativeMovement ){
			// TODO similar to component collider
			
		}else{
			bpAttachment.Reposition( posMatrix, ! pPreventAttNotify );
		}
	}
	
	// no more dirty
	pDirtyAttachments = false;
}

void debpColliderVolume::pUpdateIsMoving(){
	SetIsMoving( pColliderVolume.GetResponseType() != deCollider::ertStatic
		&& ( pHasAngVelo || pHasLinVelo || pHasGravity ) );
}
