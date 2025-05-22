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

#include "debpColliderComponent.h"
#include "debpColliderAttachment.h"
#include "debpColliderBone.h"
#include "debpColliderConstraint.h"
#include "debpColliderBones.h"
#include "../dePhysicsBullet.h"
#include "../debpBulletShape.h"
#include "../debpBulletCompoundShape.h"
#include "../debpMotionState.h"
#include "../debpPhysicsBody.h"
#include "../debpRig.h"
#include "../coldet/debpSweepCollisionTest.h"
#include "../coldet/debpCollisionDetection.h"
#include "../coldet/debpConvexResultCallback.h"
#include "../coldet/debpContactResultCallback.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../component/debpModel.h"
#include "../component/debpComponent.h"
#include "../component/debpBulletShapeModel.h"
#include "../forcefield/debpForceField.h"
#include "../shape/debpShape.h"
#include "../shape/debpCreateShape.h"
#include "../terrain/heightmap/debpHeightTerrain.h"
#include "../visitors/debpCreateBulletShape.h"
#include "../visitors/debpClosestConvexResultCallback.h"
#include "../visitors/debpShapeSurface.h"
#include "../world/debpWorld.h"
#include "../world/debpCollisionWorld.h"

#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/resources/collider/deColliderReference.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigConstraint.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "debpColliderVolume.h"
#include "debpColliderRig.h"



// #define DO_TIMING
// #define DO_TIMING2

#if defined DO_TIMING || defined DO_TIMING2
	#include <dragengine/common/utils/decTimer.h>
#endif

#ifdef DO_TIMING
	static decTimer timerTotal;
	static decTimer timer;

	#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER(what)			GetBullet()->LogInfoFormat( "Collider Component Timer: %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )
	#define DEBUG_PRINT_TIMER_TOTAL(what)	GetBullet()->LogInfoFormat( "Collider Component Timer-Total: %s = %iys", what, ( int )( timerTotal.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
	#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif

#ifdef DO_TIMING2
	static decTimer timer2;
	
	#define DEBUG_RESET_TIMER2				timer2.Reset();
	#define DEBUG_PRINT_TIMER2(what)		GetBullet()->LogInfoFormat( "Collider Component Timer: %s = %iys", what, ( int )( timer2.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMER2
	#define DEBUG_PRINT_TIMER2(what)
#endif



// Class debpColliderComponent
////////////////////////////////

// Constructor, destructor
////////////////////////////

debpColliderComponent::debpColliderComponent( dePhysicsBullet *bullet, deColliderComponent &collider ) :
debpCollider( bullet, collider, ectComponent ),
pColliderComponent( collider )
{
	pHasGravity = false;
	pHasLinVelo = false;
	pHasAngVelo = false;
	pPreventUpdate = false;
	pPreventAttNotify = false;
	pDirtyBones = false;
	pDirtyAttachments = true;
	pDirtyShapes = true;
	pDirtyRelMoveMatrix = false;
	pDirtySweepTest = true;
	pDirtyStaticTest = true;
	pResetKinematicInterpolation = true;
	
	pLinkedComponent = NULL;
	
	pBones = NULL;
	pSimplePhyBody = NULL;
	pHasRigOffset = false;
	pTestMode = etmNone;
	
	pSweepCollisionTest = NULL;
	
	pStaticCollisionTest = NULL;
	pStaticCollisionTestShape = NULL;
	pStaticCollisionTestObject.SetOwnerCollider( this, -1 );
	
	ComponentChanged();
	PositionChanged();
	OrientationChanged();
	ScaleChanged();
	LinearVelocityChanged();
	AngularVelocityChanged();
	GravityChanged();
	EnabledChanged();
	PropertiesChanged();
	ResponseTypeChanged();
	
	pDirtyDebugChecks = true;
}

debpColliderComponent::~debpColliderComponent(){
	pCleanUp();
}



// Management
///////////////

void debpColliderComponent::SetLinkedComponent( debpComponent *component ){
	pLinkedComponent = component;
}



void debpColliderComponent::DirtyBones(){
	pDirtyBones = true;
	
	RegisterColDetPrepare();
	RegisterColDetFinish();
}

void debpColliderComponent::DirtyAttachments(){
	pDirtyAttachments = true;
	
	RegisterColDetPrepare();
	RegisterColDetFinish();
}



void debpColliderComponent::SetParentWorld( debpWorld *parentWorld ){
	if( parentWorld == GetParentWorld() ){
		return;
	}
	
	if( pBones ){
		pBones->DestroyDebugDrawers();
	}
	
	debpCollider::SetParentWorld( parentWorld );
	
	if( pBones && GetDebugDrawer() ){
		pBones->CreateDebugDrawers();
	}
}

void debpColliderComponent::CreateBody(){
	debpCollisionWorld * const dynWorld = GetDynamicsWorld();
	if( ! dynWorld ){
		return;
	}
	
	int c, constraintCount = GetConstraintCount();
	
	for( c=0; c<constraintCount; c++ ){
		GetConstraintAt( c )->SetDynamicsWorld( dynWorld );
	}
	
	/*
	GetBullet()->LogInfoFormat( "component(model='%s',rig='%s')",
		( pColliderComponent.GetComponent() && pColliderComponent.GetComponent()->GetModel() )
	? pColliderComponent.GetComponent()->GetModel()->GetFilename() : "",
	( pColliderComponent.GetComponent() && pColliderComponent.GetComponent()->GetRig() )
	? pColliderComponent.GetComponent()->GetRig()->GetFilename() : "" );
	*/
	
	if( pSimplePhyBody ){
		pSimplePhyBody->SetDynamicsWorld( dynWorld );
		
	}else if( pBones ){
		pBones->SetDynamicWorld( dynWorld );
	}
	
	GravityChanged();
}

void debpColliderComponent::DestroyBody(){
	int c, constraintCount = GetConstraintCount();
	
	for( c=0; c<constraintCount; c++ ){
		GetConstraintAt( c )->SetDynamicsWorld( NULL );
	}
	
	if( pSimplePhyBody ){
		pSimplePhyBody->SetDynamicsWorld( NULL );
		
	}else if( pBones ){
		pBones->SetDynamicWorld( NULL );
	}
	
	GravityChanged();
}

void debpColliderComponent::UpdateFromBody(){
	// if a simple body just set the collider from the physics body
	if( pSimplePhyBody ){
		if( pSimplePhyBody->UpdateFromBody() ){
			// prevent all updates during this time
			pPreventUpdate = true;
			
			// update
			//pRelMoveDisplacement += ( pSimplePhyBody->GetPosition() - pColliderComponent.GetPosition() ).ToVector();
			//pRelMoveRotation *= pSimplePhyBody->GetOrientation() * pColliderComponent.GetOrientation().Conjugate();
			
			if( pHasRigOffset ){
				pColliderComponent.SetPosition( pSimplePhyBody->GetMatrix() * -pRigOffset );
				
			}else{
				pColliderComponent.SetPosition( pSimplePhyBody->GetPosition() );
			}
			
			pColliderComponent.SetOrientation( pSimplePhyBody->GetOrientation() );
			pColliderComponent.SetLinearVelocity( pSimplePhyBody->GetLinearVelocity() );
			pColliderComponent.SetAngularVelocity( pSimplePhyBody->GetAngularVelocity() );
			
			// finished so we can allow updates again.
			pPreventUpdate = false;
		}
		
	// otherwise update bonewise
	}else if( pBones ){
		pPreventUpdate = true;
		if( pBones->UpdateFromBody() ){
			RequiresUpdate();
		}
		pPreventUpdate = false;
	}
	
	// update component if existing. has to be done always since component is implicitely
	// attached to a collider component. a possible optimization would be to track if a
	// change happened but SetPosition and SetOrientation do already nothing if position
	// or orientation are equal to the current value
// 	if( component ){
// 		component->SetPosition( pColliderComponent.GetPosition() );
// 		component->SetOrientation( pColliderComponent.GetOrientation() );
// 	}
	
	// update attachments if required. this is independent of the existence of bullet physics
	// bodies for bones since attachments are calculated against the component bones not the
	// collider component bones
	// 
	// unfortunatey in the case of kinematic animation we do not know the bones have changed
	// unless we calculate collider bone information and compare them. even then kinematic
	// bones would be missed since they are not calculated if dynamic. thus the only solution
	// is to always force attachments updates if there are attachments on this collider
	if( GetAttachmentCount() > 0 ){
		pUpdateAttachments( true ); // false if the problem would not exist
	}
}

void debpColliderComponent::UpdateExtends(){
	decDVector minExtend( GetPosition() );
	decDVector maxExtend( GetPosition() );
	
	switch( pTestMode ){
	case etmBoneShape:
		if( pBones ){
			UpdateShapes();
			//pBones->CalcShapeExtends( minExtend, maxExtend );
			minExtend = GetShapeMinimumExtend();
			maxExtend = GetShapeMaximumExtend();
		}
		break;
		
	case etmModelDynamic:
	case etmModelStatic:{
		if( pLinkedComponent ){
			pLinkedComponent->PrepareExtends();
			minExtend = pLinkedComponent->GetMinimumExtend();
			maxExtend = pLinkedComponent->GetMaximumExtend();
		}
		}break;
		
	case etmRigShape:{
		const int shapeCount = pRigShapes.GetShapeCount();
		debpDCollisionBox colBox;
		bool hasExtend = false;
		int i;
		
		for( i=0; i<shapeCount; i++ ){
			pRigShapes.GetShapeAt( i )->GetCollisionVolume()->GetEnclosingBox( &colBox );
			
			if( hasExtend ){
				minExtend.SetSmallest( colBox.GetCenter() - colBox.GetHalfSize() );
				maxExtend.SetLargest( colBox.GetCenter() + colBox.GetHalfSize() );
				
			}else{
				minExtend = colBox.GetCenter() - colBox.GetHalfSize();
				maxExtend = colBox.GetCenter() + colBox.GetHalfSize();
				hasExtend = true;
			}
		}
		}break;
		
	default:
		break;
	}
	
	// apply predicted displacement
	if( pPredictDisp.x > 0.0f ){
		maxExtend.x += pPredictDisp.x;
		
	}else{
		minExtend.x += pPredictDisp.x;
	}
	
	if( pPredictDisp.y > 0.0f ){
		maxExtend.y += pPredictDisp.y;
		
	}else{
		minExtend.y += pPredictDisp.y;
	}
	
	if( pPredictDisp.z > 0.0f ){
		maxExtend.z += pPredictDisp.z;
		
	}else{
		minExtend.z += pPredictDisp.z;
	}
	
	SetExtends( GetPosition() + minExtend, GetPosition() + maxExtend );
	
	/*
	deComponent *component = pColliderComponent.GetComponent();
	
	if( component ){
		component->PrepareExtends();
		decDVector minExtend( component->GetMinimumExtend() );
		decDVector maxExtend( component->GetMaximumExtend() );
		
		if( pPredictDisp.x > 0.0f ){
			maxExtend.x += pPredictDisp.x;
		}else{
			minExtend.x += pPredictDisp.x;
		}
		
		if( pPredictDisp.y > 0.0f ){
			maxExtend.y += pPredictDisp.y;
		}else{
			minExtend.y += pPredictDisp.y;
		}
		
		if( pPredictDisp.z > 0.0f ){
			maxExtend.z += pPredictDisp.z;
		}else{
			minExtend.z += pPredictDisp.z;
		}
		
		SetExtends( component->GetPosition() + minExtend, component->GetPosition() + maxExtend );
		
	}else{
		SetExtends( decDVector(), decDVector() );
	}
	*/
}

void debpColliderComponent::PrepareForStep(){
	debpCollider::PrepareForStep();
	
DEBUG_RESET_TIMERS;
	pPreventAttNotify = true;
	RegisterColDetFinish(); // to disable pPreventAttNotify
	
	if( pBones ){
// 		if( pColliderComponent.GetComponent() ){
// 			pColliderComponent.GetComponent()->PrepareBones();  // TODO IS THIS REQUIRED?!
// 		}
		
		// this is dirty but i've got no better idea right now
		if( GetConstraintCount() > 0 ){
			pBones->SetAllBonesDirty();
		}
		
		pBones->ActivateDirtyPhysicsBodies();
		pDirtyBones = false;
DEBUG_PRINT_TIMER( "Update bone phy bodies" );
	}
	
	//pUpdateAttachments();
	pCheckDebugChecks();
	PrepareRigConstraintsForStep();
DEBUG_PRINT_TIMER_TOTAL( "PrepareForStep" );
}

void debpColliderComponent::DetectCustomCollision( float elapsed ){
	if( ! GetUseKinematicSimulation() || ! GetIsMoving() ){
		return;
	}
	if( pTestMode != etmRigShape ){
		return;
	}
	
	ApplyGravity( elapsed );
	
	if( ! pColliderComponent.GetEnabled() || pColliderComponent.GetCollisionFilter().CanNotCollide() ){
		// no collision possible so just move the collider
		pColliderComponent.SetPosition( pPosition + pLinVelo );
		pColliderComponent.SetLinearVelocity( pLinVelo );
		pPredictDisp.SetZero();
		return;
	}
	
	debpWorld &world = *GetParentWorld();
	debpCollisionWorld &dynamicsWorld = *world.GetDynamicsWorld();
	debpClosestConvexResultCallback colliderMoveHits;
	deCollisionInfo *colinfo = world.GetCollisionInfo();
	int stopIfCloseToHitPlane = 0;
	float localElapsed = elapsed;
	decVector unstuckVelocity;
	debpDCollisionBox colBox;
	
	int cspmax = 20;
	int cheapStuckPrevention = 0;
	BP_DEBUG_IF( float csphist[ cspmax + 1 ] )
	
	// hack, apply rotation before moving. has to be done correctly later on
	PredictRotation( elapsed );
	ApplyRotation();
	// end hack
	
	//pUpdateBPShape();
	pUpdateSweepCollisionTest();
	
	while( localElapsed > 1e-6f ){
		//PredictRotation( localElapsed ); // does not call expensive updates
		PredictDisplacement( localElapsed ); // calls expensive updates
		
		world.UpdateOctrees();
		
		colliderMoveHits.Reset();
		colliderMoveHits.SetTestCollider( this );
		
		if( ! colliderMoveHits.MoveDirIsNotZero() ){
			break;
		}
		
		world.UpdateDynWorldAABBs();
		
		const btQuaternion btorientation( ( btScalar )pOrientation.x, ( btScalar )pOrientation.y,
			( btScalar )pOrientation.z, ( btScalar )pOrientation.w );
		const btTransform transformFrom( btorientation, colliderMoveHits.m_convexFromWorld );
		const btTransform transformTo( btorientation, colliderMoveHits.m_convexToWorld );
		
		pSweepCollisionTest->SweepTest( dynamicsWorld, transformFrom, transformTo, colliderMoveHits );
		
		if( ! colliderMoveHits.HasLocalCollision() ){
			//ApplyRotation(); // does not do expensive updates
			ApplyDisplacement(); // does expensive updates
			break;
		}
		
		colliderMoveHits.GetResult( *colinfo );
		//const float displacementDistance = pPredictDisp.Length();
		
		
		// check if the hit plane distance is small. if a collision is found the linear velocity
		// is updated by the game script. a second collision can happen which alters the linear
		// velocity again. if the two collision planes are in a bad position it happens that the
		// third collision will be close to the second. this can lead to the collider oscillating
		// between the collision points and never converge. the third collision though has the
		// last collider position close to the hit plane surface. if this distance to the hit
		// plane is less than 0.1mm then stop further testing
		if( ( colinfo->GetNormal() * pPredictDisp ) * colinfo->GetDistance() > -1.5e-4f ){
			if( ++stopIfCloseToHitPlane == 3 ){
				break;
			}
			
		}else{
			stopIfCloseToHitPlane = 0;
		}
		
		// otherwise continue with collision handling
		//InterpolateRotation( colinfo->GetDistance() ); // does not do expensive updates
		InterpolatePosition( colinfo->GetDistance() ); // does expensive updates
		const decDVector positionAtHit( pPosition );
		
		/*
		if( GetUseFakeDynamics() ){
			if( pColInfo->GetDistance() < 0.001 ) break;
			bpColliderVolume->ApplyFakeDynamicResponse( *pColInfo );
			
		}else{
			collider->GetPeerScripting()->CollisionResponse( collider, pColInfo );
		}
		*/
#if 0
		if( colinfo->IsCollider() ){
			deCollider *c = colinfo->GetCollider();
			debpCollider *col = ( debpCollider* )c->GetPeerPhysics();
			if( col->IsVolume() ){
				bullet.LogInfoFormat( "hit collider volume at (%f,%f,%f)", c->GetPosition().x, c->GetPosition().y, c->GetPosition().z );
			}else if( col->IsComponent() ){
				bullet.LogInfoFormat( "hit collider component at (%f,%f,%f) rig=%s attached=%i", c->GetPosition().x, c->GetPosition().y, c->GetPosition().z,
					col->CastToComponent()->GetColliderComponent().GetComponent()->GetRig() ?
					col->CastToComponent()->GetColliderComponent().GetComponent()->GetRig()->GetFilename().GetString() : "-",
					pColliderComponent.HasAttachmentWith( &col->GetCollider() ) );
			}else{
				bullet.LogInfoFormat( "hit collider rigged at (%f,%f,%f) rig=%s", c->GetPosition().x, c->GetPosition().y, c->GetPosition().z,
					col->CastToRigged()->GetRig()->GetRig().GetFilename().GetString() );
			}
		}else if( colinfo->IsHTSector() ){
			bullet.LogInfo( "hit height terrain" );
		}
#endif
		
		deColliderReference guard( &pColliderComponent ); // avoid collider being removed while in use
		
		pColliderComponent.GetPeerScripting()->CollisionResponse( &pColliderComponent, colinfo );
		
		// if position is at collision point apply a small shift to avoid re-collision
		if( positionAtHit.IsEqualTo( pPosition, 1e-5 ) ){
// 				bullet.LogWarnFormat("shift normal=(%f,%f,%f)", colinfo->GetNormal().x, colinfo->GetNormal().y, colinfo->GetNormal().z );
			pColliderComponent.SetPosition( pPosition + decDVector( colinfo->GetNormal() ) * 1e-5 );
		}
		
		// check if the collision response is potentially leading to a collision again.
		// this is the case if the linear velocity is not zero and the angle between
		// linear velocity and hit normal is larger than 90 degrees minus a small threshold.
		// what we need to avoid is the case of the velocity running perpendicular to the
		// hit normal or worse slightly towards it. if this is the case clamp the linear
		// velocity to an angle slightly less than 90 degrees. this ensures the collider
		// is moving away from the surface in which case no collision will happen
		const decVector &respLinVelo = pColliderComponent.GetLinearVelocity();
		
		if( respLinVelo.IsZero( 0.001f ) ){
			// too small velocity. at 60Hz step size this is 0.01mm which is small.
			// we could even break at 0.01f which is 0.16mm at 60Hz
			break;
		}
		
		const decVector &hitNormal = colinfo->GetNormal();
		const float norRespLinVelo = respLinVelo * hitNormal;
		
		if( norRespLinVelo < 1e-4f ){
			// the idea behind unstuckVelocity is this. if the collider is touching both a
			// ground collider and a perpendicular wall collider it is possible the collision
			// alternates between the two at 0 hit distance because the correction velocity
			// of both is perpendicular to the other and can trigger the excat same problem
			// we try to avoid here. by accumulating the unstuck velocities we can ensure
			// we get away also from such a situation
			unstuckVelocity += hitNormal * ( 0.001f - norRespLinVelo );
			pColliderComponent.SetLinearVelocity( respLinVelo + unstuckVelocity );
		}
		
		localElapsed -= localElapsed * colinfo->GetDistance();
		
		BP_DEBUG_IF( csphist[ cheapStuckPrevention ] = colinfo->GetDistance() )
		cheapStuckPrevention++;
		
		if( cheapStuckPrevention == cspmax ){
			#ifdef WITH_DEBUG
			dePhysicsBullet &bullet = *GetBullet();
			const decDVector &position = pColliderComponent.GetPosition();
			const decVector rotation( decMatrix::CreateFromQuaternion(
				pColliderComponent.GetOrientation() ).GetEulerAngles() / DEG2RAD );
			const decVector &lvelo = pColliderComponent.GetLinearVelocity();
			const decVector avelo( pColliderComponent.GetAngularVelocity() / DEG2RAD );
			int i;
			
			bullet.LogWarnFormat( "STUCK! collider=%p responseType=%i",
				&pColliderComponent, pColliderComponent.GetResponseType() );
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
			#endif
			
			pColliderComponent.SetLinearVelocity( decVector() );
			pColliderComponent.SetAngularVelocity( decVector() );
			break;
		}
	}
}

bool debpColliderComponent::GetRigidBodyDeactivated() const{
	if( pSimplePhyBody && pSimplePhyBody->GetRigidBody() ){
		return ! pSimplePhyBody->GetRigidBody()->isActive();
	}
	if( pBones && pBones->GetBoneCount() > 0 ){
		const int count = pBones->GetBoneCount();
		int i;
		for( i=0; i<count; i++ ){
			const debpColliderBone * const bone = pBones->GetBoneAt( i );
			if( bone ){
				debpPhysicsBody * const phyBody = bone->GetPhysicsBody();
				if( phyBody && phyBody->GetRigidBody() && phyBody->GetRigidBody()->isActive() ){
					return false;
				}
			}
		}
	}
	return true;
}

void debpColliderComponent::ApplyGravity( float elapsed ){
	pLinVelo += pGravity * elapsed;
}

void debpColliderComponent::ApplyForceFields( float elapsed ){
	if( GetUseKinematicSimulation() || pColliderComponent.GetResponseType() != deCollider::ertDynamic ){
		return;
	}
	
	const deWorld &world = GetParentWorld()->GetWorld();
	
	// bullet does not by itself know force fields but it knows gravity which is a kind of force
	// field. for this reason the force fields are simulated by altering the gravity. to avoid
	// speed loss we check first if there are any force fields affecting the component at all.
	// this is not the best solution and requires an active tracking of force fields entering
	// and leaving colliders for optimal performance. for this time being this is though enough
	bool hasForceFields = false;
	deForceField *forceField = world.GetRootForceField();
	while( forceField ){
		if( forceField->GetCollisionFilter().CollidesNot( pColliderComponent.GetCollisionFilter() ) ){
			forceField = forceField->GetLLWorldNext();
			continue;
		}
		
		//const debpForceField &bpForceField = *( ( debpForceField* )forceField->GetPeerPhysics() );
		
		// TODO check if the force field overlaps the component
		
		hasForceFields = true;
		break;
		//forceField = forceField->GetLLWorldNext();
	}
	
	if( ! hasForceFields ){
		return;
	}
	
	// update fluctuation simulation
	const float flucAngle = DEG2RAD * 180.0f;
	
	pForceFieldFluctuation.Update( elapsed );
	const float fluctStrength = pForceFieldFluctuation.GetStrength();
	const float fluctDirection = pForceFieldFluctuation.GetDirection();
	float forceFactor;
	float distance;
	
	// force fields modify the gravity. for this to work we have to first reset the gravity then
	// add the contribution of each force field
	/*
	if( pSimplePhyBody ){
		pSimplePhyBody->SetGravity( pGravity );
		
	}else{
		for( i=0; i<pBonePhysicsCount; i++ ){
			pBonesPhysics[ i ]->GetPhysicsBody()->SetGravity( pGravity );
		}
	}
	*/
	
	forceField = world.GetRootForceField();
	while( forceField ){
		if( forceField->GetCollisionFilter().CollidesNot( pColliderComponent.GetCollisionFilter() ) ){
			forceField = forceField->GetLLWorldNext();
			continue;
		}
		
		const debpForceField &bpForceField = *( ( debpForceField* )forceField->GetPeerPhysics() );
		// TODO check if the force field overlaps the component
		
		// apply acceleration to physics bodies
		const deForceField::eApplicationTypes ffApplyType = forceField->GetApplicationType();
		const deForceField::eFieldTypes ffFieldType = forceField->GetFieldType();
		const decDVector &ffpos = forceField->GetPosition();
		const decVector &ffDir = bpForceField.GetDirection();
		const float ffFluctuationDirection = forceField->GetFluctuationDirection();
		const float ffFluctuationForce = forceField->GetFluctuationForce();
		const float ffRadius = forceField->GetRadius();
		const float ffRadiusSquared = ffRadius * ffRadius;
		const float ffExponent = forceField->GetExponent();
		const float ffForce = forceField->GetForce();
		
		if( pSimplePhyBody ){
			switch( ffApplyType ){
			case deForceField::eatDirect:
				forceFactor = pColliderComponent.GetForceFieldDirect();
				break;
				
			case deForceField::eatSurface:
				// use half the surface area as estimation. this is a good enough approximation
				// since for convex objects half their surface faces a given direction
				forceFactor = pColliderComponent.GetForceFieldSurface() * pSimplePhyBody->GetShapeSurface() * 0.5f;
				break;
				
			case deForceField::eatMass:
				forceFactor = pColliderComponent.GetForceFieldMass() * pSimplePhyBody->GetMass();
				break;
				
			case deForceField::eatSpeed:
				forceFactor = pColliderComponent.GetForceFieldSpeed() * pSimplePhyBody->GetLinearVelocity().Length();
				break;
				
			default:
				DETHROW( deeInvalidParam );
			}
			
			decVector direction( pSimplePhyBody->GetPosition() - ffpos );
			const float distanceSquared = direction.LengthSquared();
			if( distanceSquared >= ffRadiusSquared ){
				forceField = forceField->GetLLWorldNext();
				continue;
			}
			
			if( ffFieldType == deForceField::eftLinear ){
				distance = sqrtf( distanceSquared );
				direction = ffDir;
				
			}else{
				if( distanceSquared < FLOAT_SAFE_EPSILON ){
					forceField = forceField->GetLLWorldNext();
					continue;
				}
				distance = sqrtf( distanceSquared );
				direction *= 1.0f - distance / ffRadius;
			}
			
			const float addForce = ( ffForce + ffFluctuationForce * fluctStrength )
				* powf( 1.0f - distance / ffRadius, ffExponent ) * forceFactor;
			
			const decMatrix flucmat( decMatrix::CreateRotationY( ffFluctuationDirection * fluctDirection * flucAngle ) );
			
			pSimplePhyBody->ApplyForce( flucmat.TransformNormal( direction ) * addForce );
			//pSimplePhyBody->ApplyTorque( addtorque ); // += direction * addtorque;
			
		}else if( pBones ){
			pBones->ApplyForceField( bpForceField, fluctStrength, fluctDirection );
		}
		
		forceField = forceField->GetLLWorldNext();
	}
}

void debpColliderComponent::PredictDisplacement( float elapsed ){
	pPredictDisp = pLinVelo * elapsed;
	MarkDirtyOctree();
}

void debpColliderComponent::InterpolatePosition( float percentage ){
	const decVector &displacement = pPredictDisp * percentage;
	
	//pRelMoveDisplacement += displacement;
	////pRelMoveRotation *= pSimplePhyBody->GetOrientation() * pColliderComponent.GetOrientation().Conjugate();
	
	pColliderComponent.SetPosition( pPosition + decDVector( displacement ) );
	pColliderComponent.SetLinearVelocity( pLinVelo );
	
	pPredictDisp.SetZero();
}

void debpColliderComponent::ApplyDisplacement(){
	//pRelMoveDisplacement += pPredictDisp;
	////pRelMoveRotation *= pSimplePhyBody->GetOrientation() * pColliderComponent.GetOrientation().Conjugate();
	
	pColliderComponent.SetPosition( pPosition + decDVector( pPredictDisp ) );
	pColliderComponent.SetLinearVelocity( pLinVelo );
	
	pPredictDisp.SetZero();
}

void debpColliderComponent::PredictRotation( float elapsed ){
	pPredictRotation = pAngVelo * elapsed;
	//if( pHasAngVelo ) printf( "predict rotation (%f,%f,%f)\n", pPredictRotation.x / DEG2RAD, pPredictRotation.y / DEG2RAD, pPredictRotation.z / DEG2RAD );
}

void debpColliderComponent::InterpolateRotation( float percentage ){
	if( pHasAngVelo ){
		const decVector &rotation = pPredictRotation * percentage;
		
		pColliderComponent.SetOrientation( decQuaternion::CreateFromEuler( rotation ) * pOrientation );
		pPredictRotation.SetZero();
	}
}

void debpColliderComponent::ApplyRotation(){
	if( pHasAngVelo ){
		/*
		const decQuaternion qo = pOrientation;
		const decQuaternion qn = qo * decMatrix::CreateRotation( pPredictRotation ).ToQuaternion();
		const decVector eo = decMatrix::CreateFromQuaternion( qo ).GetEulerAngles() / DEG2RAD;
		const decVector en = decMatrix::CreateFromQuaternion( qn ).GetEulerAngles() / DEG2RAD;
		printf( "apply rotation (%f,%f,%f,%f) => (%f,%f,%f,%f)\n", qo.x, qo.y, qo.z, qo.w, qn.x, qn.y, qn.z, qn.w );
		printf( "  euler (%f,%f,%f) => (%f,%f,%f)\n", eo.x, eo.y, eo.z, en.x, en.y, en.z );
		*/
		pColliderComponent.SetOrientation( decQuaternion::CreateFromEuler( pPredictRotation ) * pOrientation );
		pPredictDisp.SetZero();
	}
}



void debpColliderComponent::PrepareDetection( float elapsed ){
DEBUG_RESET_TIMER2;
	debpCollider::PrepareDetection( elapsed );
	
	// apply force fields for dynamic simulation
	ApplyForceFields( elapsed );
	
	// update the kinematic bones if using bone test mode
	/*GetBullet()->LogInfoFormat( "PrepareDetection: spb=%p db=%d rig=%s", pSimplePhyBody, pDirtyBones,
		pColliderComponent.GetComponent() && pColliderComponent.GetComponent()->GetRig()
			? pColliderComponent.GetComponent()->GetRig()->GetFilename().GetString() : "?" );*/
	if( pBones && pDirtyBones ){
		// bullet uses interpolation of the previous transformation and velocities with the current
		// values to calculate CCD in particular. if the user sets the collider bone parameter himself
		// it is assumed the object teleported or has been reset. in this case the interpolation has
		// to be reset to prevent wrong results. to achieve this the pResetKinematicInterpolation is
		// used to determine if Bone*Changed calls do reset the interpolation or not. during this
		// preparation code Bone*Changed is not allowed to reset the interpolation or the CCD breaks
		pResetKinematicInterpolation = false;
		pBones->PrepareForDetection( elapsed );
		pResetKinematicInterpolation = true;
	}
	DEBUG_PRINT_TIMER2( "ColliderComponent PrepareDetection" );
}

void debpColliderComponent::FinishDetection(){
	if( GetIsPrepared() ){
		CheckRigConstraintsBroke();
	}
	
	debpCollider::FinishDetection();
	
	pPreventAttNotify = false;
}

bool debpColliderComponent::CalcAutoColDetPrepare(){
	if( debpCollider::CalcAutoColDetPrepare() ){
		return true;
	}
	
	if( pBones && pBones->GetBonePhysicsCount() > 0 ){
		return pColliderComponent.GetResponseType() == deCollider::ertDynamic || pBones->HasConstraints();
	}
	
	return false;
}

bool debpColliderComponent::CalcAutoColDetFinish(){
	if( debpCollider::CalcAutoColDetFinish() ){
		return true;
	}
	
	if( pBones && pBones->GetBonePhysicsCount() > 0 ){
		return pColliderComponent.GetResponseType() == deCollider::ertDynamic || pBones->HasConstraints();
	}
	
	return false;
}



void debpColliderComponent::PrepareRigConstraintsForStep(){
	if( pBones ){
		pBones->PrepareConstraintsForStep();
	}
}

void debpColliderComponent::CheckRigConstraintsBroke(){
	if( pBones ){
		pBones->CheckConstraintsBroke();
	}
}



void debpColliderComponent::UpdateCollisionObjectAABBs(){
	if( pSimplePhyBody ){
		pSimplePhyBody->UpdateAABB();
		
	}else if( pBones ){
		pBones->UpdatePhyBodyAABBs();
	}
}



void debpColliderComponent::UpdateOctreePosition(){
	debpCollider::UpdateOctreePosition();
}

void debpColliderComponent::UpdateShapes(){
	if( ! pDirtyShapes ){
		return;
	}
	
	const decDMatrix &matrix = GetMatrix();
	
	if( pHasRigOffset ){
		pRigShapes.UpdateWithMatrix( decDMatrix::CreateTranslation( -pRigOffset ).QuickMultiply( matrix ) );
		
	}else{
		pRigShapes.UpdateWithMatrix( matrix );
	}
	
	if( pBones ){
		pBones->UpdateShapes();
	}
	
	UpdateShapeExtends();
	pDirtyShapes = false;
}

void debpColliderComponent::UpdateShapesWithMatrix( const decDMatrix &transformation ){
	pDirtyShapes = true;
	
	if( pRigShapes.GetShapeCount() > 0 ){
		if( pHasRigOffset ){
			pRigShapes.UpdateWithMatrix( decDMatrix::CreateTranslation( -pRigOffset )
				.QuickMultiply( GetInverseMatrix() ).QuickMultiply( transformation ) );
			
		}else{
			pRigShapes.UpdateWithMatrix( transformation );
		}
	}
	
	if( pBones ){
		pBones->UpdateShapes( transformation );
	}
	
	UpdateShapeExtends();
}

void debpColliderComponent::UpdateShapeExtends(){
	decDVector minExtend( GetPosition() );
	decDVector maxExtend( GetPosition() );
	
	switch( pTestMode ){
	case etmBoneShape:
		if( pBones ){
			pBones->CalcShapeExtends( minExtend, maxExtend );
		}
		break;
		
	case etmModelDynamic:
	case etmModelStatic:{
		if( pLinkedComponent ){
			pLinkedComponent->PrepareExtends();
			minExtend = pLinkedComponent->GetMinimumExtend();
			maxExtend = pLinkedComponent->GetMaximumExtend();
		}
		}break;
		
	case etmRigShape:{
		const int shapeCount = pRigShapes.GetShapeCount();
		debpDCollisionBox colBox;
		bool hasExtend = false;
		int i;
		
		for( i=0; i<shapeCount; i++ ){
			pRigShapes.GetShapeAt( i )->GetCollisionVolume()->GetEnclosingBox( &colBox );
			
			if( hasExtend ){
				minExtend.SetSmallest( colBox.GetCenter() - colBox.GetHalfSize() );
				maxExtend.SetLargest( colBox.GetCenter() + colBox.GetHalfSize() );
				
			}else{
				minExtend = colBox.GetCenter() - colBox.GetHalfSize();
				maxExtend = colBox.GetCenter() + colBox.GetHalfSize();
				hasExtend = true;
			}
		}
		}break;
		
	default:
		break;
	}
	
	SetShapeExtends( minExtend, maxExtend );
}



debpSweepCollisionTest *debpColliderComponent::GetSweepCollisionTest(){
	pUpdateSweepCollisionTest();
	return pSweepCollisionTest;
}

btGhostObject *debpColliderComponent::GetStaticCollisionTest(){
	pUpdateStaticCollisionTest();
	return pStaticCollisionTest->getCollisionShape() ? pStaticCollisionTest : nullptr;
}

btGhostObject *debpColliderComponent::GetStaticCollisionTestPrepare(){
	btGhostObject * const go = GetStaticCollisionTest();
	if(go){
		go->setWorldTransform(btTransform(
			{(btScalar)pOrientation.x, (btScalar)pOrientation.y,
				(btScalar)pOrientation.z, (btScalar)pOrientation.w},
			{(btScalar)pPosition.x, (btScalar)pPosition.y, (btScalar)pPosition.z}));
	}
	return go;
}

bool debpColliderComponent::PrepareStaticCollisionTest(){
	return GetStaticCollisionTestPrepare() != nullptr;
}



// Debugging
//////////////

void debpColliderComponent::UpdateDebugDrawer(){
	debpCollider::UpdateDebugDrawer();
	
	if( pBones ){
		if( GetDebugDrawer() ){
			pBones->CreateDebugDrawers();
			pBones->UpdateDebugDrawers();
			
		}else{
			pBones->DestroyDebugDrawers();
		}
	}
}

void debpColliderComponent::UpdateDDSShape(){
	deDebugDrawerShape * const ddshape = GetDDSShape();
	if( ! ddshape ){
		return;
	}
	
	switch( pTestMode ){
	case etmBoneShape:
		if( pBones ){
			pBones->UpdateDebugDrawersShape();
		}
		break;
		
	case etmRigShape:
		ddshape->GetShapeList() = pColliderComponent.GetComponent()->GetRig()->GetShapes();
		break;
		
	default:
		ddshape->GetShapeList().RemoveAll();
		break;
	}
	
	GetDebugDrawer()->NotifyShapeContentChanged();
}



// Actions
////////////

void debpColliderComponent::EnableBoneConstraint( int bone, int constraint, bool enable ){
	if( pBones ){
		pBones->EnableConstraint( bone, constraint, enable );
	}
}

void debpColliderComponent::ReplaceBoneConstraint( int bone, int constraint, const deRigConstraint &replacement ){
	GetBullet()->LogWarn( "ReplaceBoneConstraint not implemented in debpColliderRig" );
}



// forces
///////////

void debpColliderComponent::ApplyImpuls( const decVector &impuls ){
	if( pSimplePhyBody ){
		pSimplePhyBody->ApplyImpuls( impuls );
		
	}else if( pBones ){
		pBones->ApplyImpuls( impuls );
	}
}

void debpColliderComponent::ApplyImpulsAt( const decVector &impuls, const decVector &position ){
	if( pSimplePhyBody ){
		pSimplePhyBody->ApplyImpulsAt( impuls, position );
		
	}else if( pBones ){
		pBones->ApplyImpulsAt( impuls, position );
	}
}

void debpColliderComponent::ApplyTorqueImpuls( const decVector &torqueImpuls ){
	if( pSimplePhyBody ){
		pSimplePhyBody->ApplyTorqueImpuls( torqueImpuls );
		
	}else if( pBones ){
		pBones->ApplyTorqueImpuls( torqueImpuls );
	}
}

void debpColliderComponent::ApplyForce( const decVector &force ){
	if( pSimplePhyBody ){
		pSimplePhyBody->ApplyForce( force );
		
	}else if( pBones ){
		pBones->ApplyForce( force );
	}
}

void debpColliderComponent::ApplyForceAt( const decVector &force, const decVector &point ){
	if( pSimplePhyBody ){
		pSimplePhyBody->ApplyForceAt( force, point );
		
	}else if( pBones ){
		pBones->ApplyForceAt( force, point );
	}
}

void debpColliderComponent::ApplyTorque( const decVector &torque ){
	if( pSimplePhyBody ){
		pSimplePhyBody->ApplyTorque( torque );
		
	}else if( pBones ){
		pBones->ApplyTorque( torque );
	}
}



void debpColliderComponent::ApplyBoneImpuls( int bone, const decVector &impuls ){
	debpColliderBone * const cbone = pBones ? pBones->GetBoneAt( bone ) : NULL;
	if( cbone ){
		cbone->GetPhysicsBody()->ApplyImpuls( impuls );
	}
}

void debpColliderComponent::ApplyBoneImpulsAt( int bone, const decVector &impuls, const decVector &position ){
	debpColliderBone * const cbone = pBones ? pBones->GetBoneAt( bone ) : NULL;
	if( cbone ){
		cbone->GetPhysicsBody()->ApplyImpulsAt( impuls, position );
	}
}

void debpColliderComponent::ApplyBoneTorqueImpuls( int bone, const decVector &torqueImpuls ){
	debpColliderBone * const cbone = pBones ? pBones->GetBoneAt( bone ) : NULL;
	if( cbone ){
		cbone->GetPhysicsBody()->ApplyTorqueImpuls( torqueImpuls );
	}
}

void debpColliderComponent::ApplyBoneForce( int bone, const decVector &force ){
	debpColliderBone * const cbone = pBones ? pBones->GetBoneAt( bone ) : NULL;
	if( cbone ){
		cbone->GetPhysicsBody()->ApplyForce( force );
	}
}

void debpColliderComponent::ApplyBoneForceAt( int bone, const decVector &force, const decVector &point ){
	debpColliderBone * const cbone = pBones ? pBones->GetBoneAt( bone ) : NULL;
	if( cbone ){
		cbone->GetPhysicsBody()->ApplyForceAt( force, point );
	}
}

void debpColliderComponent::ApplyBoneTorque( int bone, const decVector &torque ){
	debpColliderBone * const cbone = pBones ? pBones->GetBoneAt( bone ) : NULL;
	if( cbone ){
		cbone->GetPhysicsBody()->ApplyTorque( torque );
	}
}



// notifications
//////////////////

void debpColliderComponent::PositionChanged(){
	const decDVector &position = pColliderComponent.GetPosition();
	if( pPosition.IsEqualTo( position ) ){
		return;
	}
	
	debpCollider::PositionChanged();
	
	pRelMoveDisplacement += ( position - pPosition ).ToVector();
	pDirtyRelMoveMatrix = true;
	
	pPosition = position;
	
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		if( pSimplePhyBody ){
			if( pHasRigOffset ){
				pSimplePhyBody->SetPosition( GetMatrix() * pRigOffset );
				
			}else{
				pSimplePhyBody->SetPosition( position );
			}
			
		}else{
			if( pBones ){
				pBones->UpdateFromKinematic( pResetKinematicInterpolation );
			}
			DirtyBones();
		}
	}
	
	deComponent * const component = pColliderComponent.GetComponent();
	if( component ){
		// update component if existing since component is implicitely attached
		component->SetPosition( position );
	}
	
	pDirtyShapes = true;
	
	RequiresUpdate();
	
	if( pColliderComponent.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderComponent::OrientationChanged(){
	const decQuaternion &orientation = pColliderComponent.GetOrientation();
	if( pOrientation.IsEqualTo( orientation ) ){
		return;
	}
	
	debpCollider::OrientationChanged();
	
	pRelMoveRotation *= orientation * pOrientation.Conjugate();
	pDirtyRelMoveMatrix = true;
	
	pOrientation = orientation;
	
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		if( pSimplePhyBody ){
			pSimplePhyBody->SetOrientation( orientation );
			
		}else{
			if( pBones ){
				pBones->UpdateFromKinematic( pResetKinematicInterpolation );
			}
			DirtyBones();
		}
	}
	
	deComponent * const component = pColliderComponent.GetComponent();
	if( component ){
		// update component if existing since component is implicitely attached
		component->SetOrientation( orientation );
	}
	
	pDirtyShapes = true;
	
	RequiresUpdate();
	
	if( pColliderComponent.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderComponent::ScaleChanged(){
	const decVector &scale = pColliderComponent.GetScale();
	
	if( pScale.IsEqualTo( scale ) ){
		return;
	}
	
	pScale = scale;
	
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		if( ! pSimplePhyBody ){
			DirtyBones();
		}
	}
	
	pDirtyShapes = true;
	pDirtySweepTest = true;
	pDirtyStaticTest = true;
	
	pUpdateBones();
	
	RequiresUpdate();
	
	if( pColliderComponent.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderComponent::GeometryChanged(){
	const decDVector &position = pColliderComponent.GetPosition();
	const decQuaternion &orientation = pColliderComponent.GetOrientation();
	const decVector &scale = pColliderComponent.GetScale();
	const bool sameScale = pScale.IsEqualTo( scale );
	if( pPosition.IsEqualTo( position ) && pOrientation.IsEqualTo( orientation ) && sameScale ){
		return;
	}
	
	debpCollider::GeometryChanged();
	
	pRelMoveDisplacement += ( position - pPosition ).ToVector();
	pRelMoveRotation *= orientation * pOrientation.Conjugate();
	pDirtyRelMoveMatrix = true;
	
	pPosition = position;
	pOrientation = orientation;
	pScale = scale;
	
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		if( pSimplePhyBody ){
			if( pHasRigOffset ){
				pSimplePhyBody->SetPosition( GetMatrix() * pRigOffset );
				
			}else{
				pSimplePhyBody->SetPosition( position );
			}
			
			pSimplePhyBody->SetOrientation( orientation );
			
		}else{
			if( pBones ){
				pBones->UpdateFromKinematic( pResetKinematicInterpolation );
			}
			DirtyBones();
		}
	}
	
	deComponent * const component = pColliderComponent.GetComponent();
	if( component ){
		// update component if existing since component is implicitely attached
		component->SetPosition( position );
		component->SetOrientation( orientation );
		component->SetScaling( scale );
	}
	
	pDirtyShapes = true;
	
	if( ! sameScale ){
		pDirtySweepTest = true;
		pDirtyStaticTest = true;
		
		pUpdateBones();
	}
	
	RequiresUpdate();
	
	if( pColliderComponent.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderComponent::LinearVelocityChanged(){
	const decVector &linVelo = pColliderComponent.GetLinearVelocity();
	if( pLinVelo.IsEqualTo( linVelo ) ){
		return;
	}
	
	if( ! pPreventUpdate ){
		if( pSimplePhyBody ){
			pSimplePhyBody->SetLinearVelocity( linVelo );
			
		}else if( pBones ){
			pBones->LinearVelocityChanged( linVelo );
		}
	}
	
	pLinVelo = linVelo;
	pHasLinVelo = linVelo * linVelo > 1e-6f; //1e-4f;
	pUpdateIsMoving();
	
	RequiresUpdate();
	
	MarkDirtyOctree();
}

void debpColliderComponent::AngularVelocityChanged(){
	const decVector &angVelo = pColliderComponent.GetAngularVelocity();
	if( pAngVelo.IsEqualTo( angVelo ) ){
		return;
	}
	
	if( ! pPreventUpdate ){
		if( pSimplePhyBody ){
			pSimplePhyBody->SetAngularVelocity( angVelo );
			
		}else if( pBones ){
			pBones->AngularVelocityChanged( angVelo );
		}
	}
	
	pAngVelo = angVelo;
	pHasAngVelo = angVelo * angVelo > 0.0003046174f; //3.046174e-06f; //0.03046174f;
	pUpdateIsMoving();
	
	RequiresUpdate();
}

void debpColliderComponent::GravityChanged(){
	if( pColliderComponent.GetUseLocalGravity() ){
		pGravity = pColliderComponent.GetGravity();
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
	
	if( pSimplePhyBody ){
		pSimplePhyBody->SetGravity( pGravity );
		
	}else if( pBones ){
		pBones->SetGravity( pGravity );
	}
}

void debpColliderComponent::EnabledChanged(){
	bool enabled = pColliderComponent.GetEnabled();
	int c, constraintCount = GetConstraintCount();
	
	for( c=0; c<constraintCount; c++ ){ // is going to change
		GetConstraintAt( c )->SetEnabled( enabled );
	}
	
	if( pSimplePhyBody ){
		pSimplePhyBody->SetEnabled( enabled );
		
	}else if( pBones ){
		pBones->EnablePhysicsBodies( enabled );
	}
}

void debpColliderComponent::PropertiesChanged(){
	if( pSimplePhyBody ){
		pSimplePhyBody->SetMass( pColliderComponent.GetMass() );
	}
	
	GravityChanged();
}

void debpColliderComponent::ResponseTypeChanged(){
	const deCollider::eResponseType responseType = pColliderComponent.GetResponseType();
	
	if( pSimplePhyBody ){
		if( responseType == deCollider::ertStatic ){
			pSimplePhyBody->SetResponseType( debpPhysicsBody::ertStatic );
			SetUseKinematicSimulation( false );
			
		}else if( responseType == deCollider::ertKinematic ){
			if( pTestMode == etmModelStatic || pTestMode == etmModelDynamic ){ // tri mesh supports no kinematic!
				pSimplePhyBody->SetResponseType( debpPhysicsBody::ertStatic );
				SetUseKinematicSimulation( false );
				
			}else{
				pSimplePhyBody->SetResponseType( debpPhysicsBody::ertKinematic );
				SetUseKinematicSimulation( true );
			}
			
		}else{
			pSimplePhyBody->SetResponseType( debpPhysicsBody::ertDynamic );
			SetUseKinematicSimulation( false );
		}
		
	}else if( pBones ){
		pBones->UpdatePhysicsType( responseType );
		SetUseKinematicSimulation( false );
	}
	
	SetAutoColDetPrepare( CalcAutoColDetPrepare() );
	SetAutoColDetFinish( CalcAutoColDetFinish() );
}

void debpColliderComponent::CollisionFilterChanged(){
	if( pSimplePhyBody ){
		pSimplePhyBody->CollisionFilteringChanged();
		
	}else if( pBones ){
		pBones->CollisionFilteringChanged();
	}
}

void debpColliderComponent::IgnoreCollidersChanged(){
	CollisionFilterChanged();
}

void debpColliderComponent::RigChanged(){
	pUpdateBones();
	ComponentShapeChanged();
}

void debpColliderComponent::ComponentChanged(){
	// NOTE if deColliderComponent.SetComponent() is called these actions happen:
	// - Peer.ComponentChanged()
	// - deColliderComponent.SetRig( component.GetRig() )
	// - if rig differs:
	//   - Peer.RigChanged()
	// 
	// this means RigChanged() has to call pUpdateBones() but if the new component has the same
	// rig ComponentChanged() has to call pUpdateBones since RigChanged() will not be called.
	deComponent *component = pColliderComponent.GetComponent();
	
	if( pLinkedComponent ){
		pLinkedComponent->SetLinkedCollider( NULL );
		pLinkedComponent = NULL;
	}
	
	deRig *rig = NULL;
	if( component ){
		rig = component->GetRig();
	}
	
	if( rig == pColliderComponent.GetRig() ){
		// if the component changes and the component rig is the same as the collider component
		// rig no RigChanged will be send. fix this
		pUpdateBones();
	}
	//MarkDirtyOctree();
// 	ComponentRigChanged();
	
	if( component ){
		pLinkedComponent = ( debpComponent* )component->GetPeerPhysics();
		pLinkedComponent->SetLinkedCollider( this );
	}
	
	SetAddToOctree( component != NULL );
	
	// invalidate attachments
	const int attachmentCount = GetAttachmentCount();
	if( attachmentCount > 0 ){
		DirtyAttachments();
		
		int i;
		for( i=0; i<attachmentCount; i++ ){
			GetAttachmentAt( i )->AttachmentChanged();
		}
	}
	
	// align component if present
	if( component ){
		component->SetPosition( pColliderComponent.GetPosition() );
		component->SetOrientation( pColliderComponent.GetOrientation() );
		component->SetScaling( pColliderComponent.GetScale() );
	}
}

void debpColliderComponent::AttachmentAdded( int index, deColliderAttachment *attachment ){
	debpCollider::AttachmentAdded( index, attachment );
	DirtyAttachments();
	pApplyAccumRelMoveMatrices();
	
	// the pApplyAccumRelMoveMatrices affects also the added attachment which is wrong.
	// we can though not call pApplyAccumRelMoveMatrices before super calling since then
	// the wrong attachment count is used. we can do this even if the type does not match
	// since resetting the matrix is not wrong in these situations
	GetAttachmentAt( index )->SetAccumRelMoveMatrix( decDMatrix() );
}

void debpColliderComponent::AttachmentChanged( int index, deColliderAttachment *attachment ){
	debpCollider::AttachmentChanged( index, attachment );
	DirtyAttachments();
}

void debpColliderComponent::AttachmentRemoved( int index, deColliderAttachment *attachment ){
	debpCollider::AttachmentRemoved( index, attachment );
	DirtyAttachments();
}

void debpColliderComponent::AllAttachmentsRemoved(){
	debpCollider::AllAttachmentsRemoved();
	DirtyAttachments();
}

void debpColliderComponent::AttachmentsForceUpdate(){
	if( pColliderComponent.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderComponent::InitWeightAttachment( deColliderAttachment &attachment, int face ){
	if( ! pLinkedComponent || ! pColliderComponent.GetComponent()
	|| ! pColliderComponent.GetComponent()->GetModel()
	|| ! pColliderComponent.GetComponent()->GetRig() ){
		attachment.SetAttachType( deColliderAttachment::eatStatic );
		return;
	}
	
	const deModel &model = *pColliderComponent.GetComponent()->GetModel();
	const deModelLOD &lod = *model.GetLODAt( 0 );
	if( face < 0 || face >= lod.GetFaceCount() ){
		attachment.SetAttachType( deColliderAttachment::eatStatic );
		return;
	}
	
	const deModelFace &modelFace = lod.GetFaceAt( face );
	int i, j, k, entryCount = 0;
	float weightFactors[ 10 ];
	int weightBones[ 10 ];
	
	const deModelVertex * const vertices[ 3 ] = {
		lod.GetVertices() + modelFace.GetVertex1(),
		lod.GetVertices() + modelFace.GetVertex2(),
		lod.GetVertices() + modelFace.GetVertex3() };
	
	pLinkedComponent->PrepareWeights();
	
	for( i=0; i<3; i++ ){
		if( vertices[ i ]->GetWeightSet() == -1 ){
			continue;
		}
		
		const debpModel::sWeightSet &weightSet =
			pLinkedComponent->GetModel()->GetWeightSets()[ vertices[ i ]->GetWeightSet() ];
		for( j=0; j<weightSet.count; j++ ){
			const int curWeightBone = weightSet.first[ j ].GetBone();
			for( k=0; k<entryCount; k++ ){
				if( weightBones[ k ] == curWeightBone ){
					weightFactors[ k ] += weightSet.first[ j ].GetWeight();
					break;
				}
			}
			if( k == entryCount && entryCount < 9 ){
				weightBones[ entryCount ] = curWeightBone;
				weightFactors[ entryCount ] = weightSet.first[ j ].GetWeight();
				entryCount++;
			}
		}
	}
	
	if( entryCount == 0 ){
		attachment.SetAttachType( deColliderAttachment::eatStatic );
		return;
	}
	
	float factorsTotal = 0.0f;
	decMatrix matrix;
	
	for( i=0; i<entryCount; i++ ){
		weightBones[ i ] = pLinkedComponent->GetModelRigMapping( weightBones[ i ] );
		factorsTotal += weightFactors[ i ];
	}
	for( i=0; i<entryCount; i++ ){
		weightFactors[ i ] /= factorsTotal;
		if( i == 0 ){
			matrix = pLinkedComponent->GetBoneAt( weightBones[ i ] )
				.weightMatrix.QuickMultiply( weightFactors[ i ] );
			
		}else{
			matrix.QuickAddTo( pLinkedComponent->GetBoneAt( weightBones[ i ] )
				.weightMatrix.QuickMultiply( weightFactors[ i ] ) );
		}
	}
	
	matrix = decMatrix::CreateWorld( attachment.GetPosition(), attachment.GetOrientation() )
		.QuickMultiply( matrix.QuickInvert() );
	
	attachment.SetPosition( matrix.GetPosition() );
	attachment.SetOrientation( matrix.ToQuaternion() );
	attachment.SetAttachType( deColliderAttachment::eatWeight );
	attachment.SetWeightCount( entryCount );
	
	deColliderAttachment::sWeight * const attWeights = attachment.GetWeights();
	const deRig &rig = *pColliderComponent.GetComponent()->GetRig();
	
	for( i=0; i<entryCount; i++ ){
		//LogInfoFormat( "Collider: add weight %s => %f", rig->GetBoneAt( weightBones[ i ] )->GetName(), weightFactors[ i ] );
		attWeights[ i ].bone = rig.GetBoneAt( weightBones[ i ] ).GetName();
		attWeights[ i ].weight = weightFactors[ i ];
	}
}



void debpColliderComponent::ConstraintAdded( int index, deColliderConstraint *constraint ){
	debpCollider::ConstraintAdded( index, constraint );
	
	debpColliderConstraint *bpConstraint = GetConstraintAt( index );
	debpCollisionWorld *dynWorld = GetDynamicsWorld();
	
	bpConstraint->SetDynamicsWorld( dynWorld );
	bpConstraint->SetEnabled( pColliderComponent.GetEnabled() );
	
	ConstraintChanged( index, constraint );
}

void debpColliderComponent::ConstraintChanged( int index, deColliderConstraint *constraint ){
	debpCollider::ConstraintChanged( index, constraint );
	
	debpColliderConstraint &bpconstr = *GetConstraintAt( index );
	
	const deCollider * const engTargetCollider = constraint->GetTargetCollider();
	debpPhysicsBody * const oldBody1 = bpconstr.GetFirstBody();
	debpPhysicsBody * const oldBody2 = bpconstr.GetSecondBody();
	const int bone1 = constraint->GetBone();
	debpPhysicsBody *newBody1 = NULL;
	debpPhysicsBody *newBody2 = NULL;
	decVector offset1;
	decVector offset2;
	
	if( pSimplePhyBody ){
		newBody1 = pSimplePhyBody;
		offset1 = pRigOffset.ToVector();
		
	}else if( pBones ){
		debpColliderBone *cbone = bone1 >= 0 && bone1 < pBones->GetBoneCount()
			? pBones->GetBoneAt( bone1 ) : pBones->GetRootBone();
		if( cbone ){
			newBody1 = cbone->GetPhysicsBody();
			offset1 = cbone->GetOffset().ToVector();
		}
	}
	
	if( engTargetCollider ){
		const debpCollider &targetCollider = *( ( const debpCollider * )engTargetCollider->GetPeerPhysics() );
		
		if( targetCollider.IsVolume() ){
			newBody2 = targetCollider.CastToVolume()->GetPhysicsBody();
			
		}else if( targetCollider.IsComponent() ){
			const debpColliderComponent &tcComponent = *targetCollider.CastToComponent();
			
			if( tcComponent.pSimplePhyBody ){
				newBody2 = tcComponent.pSimplePhyBody;
				offset2 = tcComponent.pRigOffset.ToVector();
				
			}else if( tcComponent.pBones ){
				debpColliderBone *cbone = NULL;
				
				const deRig * const rig = tcComponent.pBones->GetRig();
				if( rig ){
					const int index = rig->IndexOfBoneNamed( constraint->GetTargetBone() );
					if( index != -1 ){
						cbone = tcComponent.pBones->GetBoneAt( index );
					}
				}
				
				if( ! cbone ){
					cbone = tcComponent.pBones->GetRootBone();
				}
				
				if( cbone ){
					newBody2 = cbone->GetPhysicsBody();
					offset2 = cbone->GetOffset().ToVector();
				}
			}
			
		}else{ // rigged
		}
	}
	
	if( newBody1 != oldBody1 ){
		bpconstr.SetFirstBody( newBody1 );
		bpconstr.SetFirstOffset( offset1 );
	}
	if( newBody2 != oldBody2 ){
		bpconstr.SetSecondBody( newBody2 );
		bpconstr.SetSecondOffset( offset2 );
	}
	
	bpconstr.ConstraintChanged();
}



// bone notifications
///////////////////////

void debpColliderComponent::BonePositionChanged( int index ){
	if( ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( ! cbone ){
		return;
	}
	
	if( ! pPreventUpdate ){
		cbone->GetPhysicsBody()->SetPosition( pColliderComponent.GetBoneAt( index ).GetPosition() );
		if( pResetKinematicInterpolation ){
			cbone->GetPhysicsBody()->ResetKinematicInterpolation();
		}
	}
	
	if( pColliderComponent.GetAttachmentCount() > 0 ){
		DirtyAttachments();
	}
	
	pDirtyShapes = true;
	MarkDirtyOctree();
}

void debpColliderComponent::BoneOrientationChanged( int index ){
	if( ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( ! cbone ){
		return;
	}
	
	if( ! pPreventUpdate ){
		cbone->GetPhysicsBody()->SetOrientation( pColliderComponent.GetBoneAt( index ).GetOrientation() );
		if( pResetKinematicInterpolation ){
			cbone->GetPhysicsBody()->ResetKinematicInterpolation();
		}
	}
	
	if( pColliderComponent.GetAttachmentCount() > 0 ){
		DirtyAttachments();
	}
	
	pDirtyShapes = true;
	MarkDirtyOctree();
}

void debpColliderComponent::BoneLinearVelocityChanged( int index ){
	if( pPreventUpdate || ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( ! cbone ){
		return;
	}
	
	cbone->GetPhysicsBody()->SetLinearVelocity( pColliderComponent.GetBoneAt( index ).GetLinearVelocity() );
	if( pResetKinematicInterpolation ){
		cbone->GetPhysicsBody()->ResetKinematicInterpolation();
	}
}

void debpColliderComponent::BoneAngularVelocityChanged( int index ){
	if( pPreventUpdate || ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( ! cbone ){
		return;
	}
	
	cbone->GetPhysicsBody()->SetAngularVelocity( pColliderComponent.GetBoneAt( index ).GetAngularVelocity() );
	if( pResetKinematicInterpolation ){
		cbone->GetPhysicsBody()->ResetKinematicInterpolation();
	}
}

void debpColliderComponent::BonePropertiesChanged( int index ){
	if( ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( cbone ){
		cbone->GetPhysicsBody()->SetMass( pColliderComponent.GetBoneAt( index ).GetMass() );
	}
}

void debpColliderComponent::BoneDynamicChanged( int index ){
	if( pBones ){
		pBones->UpdatePhysicsType( pColliderComponent.GetResponseType(), index );
	}
}



// Collision Detection
////////////////////////

bool debpColliderComponent::PointInside( const decDVector &point ){
	switch( pTestMode ){
	case etmRigShape:{
		if( ! pSimplePhyBody ){
			return false;
		}
		
		UpdateShapes();
		
		const int shapeCount = pRigShapes.GetShapeCount();
		int i;
		
		for( i=0; i<shapeCount; i++ ){
			const debpShape &shape = *pRigShapes.GetShapeAt( i );
			if( shape.GetCollisionVolume()->IsPointInside( point ) ){
				return true;
			}
		}
		
		return false;
		} break;
		
	case etmBoneShape:
		if( pBones ){
			UpdateShapes();
			return pBones->PointInside( point );
		}
		return false;
		
	case etmModelStatic:
	case etmModelDynamic:
		// TODO
		return false;
		
	default: // etmNone
		return false;
	}
}

void debpColliderComponent::RayHits( const decDVector &rayOrigin,
const decVector &rayDirection, deBaseScriptingCollider *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	if(!pPrepreStaticCollisionTestPos()){
		return;
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
	
	// do collision test
	#ifdef BULLET_RAY_CAST_UNSTABLE
		// bullet has a broken ray-box test implementation using Gjk which has a tendency
		// to miss collisions half of the time. as a quick fix a sweep test is done with
		// a tiny sphere which yields a comparable result but is not prone to the problem
		debpConvexResultCallback result( colinfo );
		result.SetTestRay( NULL, listener );
		colinfo->SetStopTesting( false );
		coldet.GetRayHackShape().SweepTest( *this, rayFromTrans, rayToTrans, result );
	
	#else
		// update abbbs?
		debpRayResultCallback result( colinfo );
		result.SetTestRay( rayOrigin, rayDirection, NULL, 0, listener );
		colinfo->SetStopTesting( false );
		// TODO requires per bone staticCollisionTest
		debpCollisionWorld::rayTestSingle( rayFromTrans, rayToTrans, staticCollisionTest,
			staticCollisionTest->getCollisionShape(), staticCollisionTest->getWorldTransform(), result );
		//debpCollisionWorld::rayTestSingle( rayFromTrans, rayToTrans, staticCollisionTest,
		//	staticCollisionTest->getCollisionShape(), staticCollisionTest->getWorldTransform(), result );
	#endif
}

static btManifoldPoint vDummyManifoldPoint; // Dummy manifold point to be used where bullet requires one but dragengine not

void debpColliderComponent::ColliderHits( deCollider *engCollider, deBaseScriptingCollider *listener ){
	DEASSERT_NOTNULL(engCollider)
	DEASSERT_NOTNULL(listener)
	
	debpCollider &collider = *(debpCollider*)engCollider->GetPeerPhysics();
	if(!PrepareStaticCollisionTest() || !collider.PrepareStaticCollisionTest()){
		return;
	}
	
	debpCollisionDetection &coldet = GetBullet()->GetCollisionDetection();
	debpContactResultCallback result(coldet.GetCollisionInfo());
	result.SetTestCollider(&collider, listener);
	coldet.GetCollisionInfo()->SetStopTesting(false);
	coldet.ColliderHitsCollider(*this, collider, result);
	
	#if 0
	if(!pPrepreStaticCollisionTestPos()){
		return;
	}
	
	debpCollisionDetection &coldet = GetBullet()->GetCollisionDetection();
	debpCollider * const collider = (debpCollider*)engCollider->GetPeerPhysics();
	deCollisionInfo * const colinfo = coldet.GetCollisionInfo();
	debpCollisionResult coldetResult;
	
	debpContactResultCallback result(colinfo);
	
	if(collider->IsVolume()){
		debpColliderVolume &colliderVolume = *collider->CastToVolume();
		btGhostObject * const staticCollisionTest2 = colliderVolume.GetStaticCollisionTestPrepare();
		if(!staticCollisionTest2){
			return;
		}
		const btCollisionObjectWrapper obj0Wrap( NULL, staticCollisionTest2->getCollisionShape(),
			staticCollisionTest2, staticCollisionTest2->getWorldTransform(), -1, -1 );
		
		result.SetTestCollider( staticCollisionTest2, collider, listener );
		colinfo->SetStopTesting( false );
		
		btVector3 aabbMin, aabbMax;
		staticCollisionTest2->getCollisionShape()->getAabb( staticCollisionTest2->getWorldTransform(), aabbMin, aabbMax );
		
		if( coldet.ColliderHitsCollider( collider, this, coldetResult ) ){
			const btCollisionObjectWrapper obj1Wrap( NULL, staticCollisionTest2->getCollisionShape(),
				staticCollisionTest2, staticCollisionTest2->getWorldTransform(), -1, -1 );
			result.addSingleResult( vDummyManifoldPoint, &obj0Wrap, 0, -1, &obj1Wrap, 0, coldetResult.face );
		}
	}
	#endif
}

void debpColliderComponent::ColliderMoveHits( deCollider *engCollider,
const decVector &displacement, deBaseScriptingCollider *listener ){
	if( ! engCollider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	if(!pPrepreStaticCollisionTestPos()){
		return;
	}
	
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

void debpColliderComponent::ColliderRotateHits( deCollider *engCollider,
const decVector &rotation, deBaseScriptingCollider *listener ){
	if( ! engCollider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	if(!pPrepreStaticCollisionTestPos()){
		return;
	}
	
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

void debpColliderComponent::ColliderMoveRotateHits( deCollider *engCollider,
const decVector &displacement, const decVector &rotation, deBaseScriptingCollider *listener ){
	if( ! engCollider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	if(!pPrepreStaticCollisionTestPos()){
		return;
	}
	
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



// internal notifications
///////////////////////////

void debpColliderComponent::ComponentRigChanged(){
	// update rig in superclass
	if( pColliderComponent.GetComponent() ){
		pColliderComponent.SetRig( pColliderComponent.GetComponent()->GetRig() );
		
	}else{
		pColliderComponent.SetRig( NULL );
	}
	
	// now the bones can be safely updated without array index problems
	pUpdateBones();
	ComponentShapeChanged();
}

void debpColliderComponent::ComponentShapeChanged(){
	if( pBones /*&& ! pPreventUpdate*/ ){
		DirtyBones();
	}
	
	// TODO in model dynamic test mode the triangle mesh changes causing the collision object
	//      to change. this can be done using a component hosted triangle collision shape
	//      which changes. requires no bone update.
	// NOTE for the time being model dynamic is not supported so this entire code is disbaled
	/*if( pTestMode == etmModelDynamic ){
		pUpdateBones();
	}*/
	
	pDirtyShapes = true;
	pDirtySweepTest = true;
	pDirtyStaticTest = true;
	
	MarkDirtyOctree();
}

void debpColliderComponent::ComponentMeshDirty(){
	if( pBones ){
		DirtyBones();
	}
	
	if( GetAttachmentCount() > 0 ){
		// component mesh dirty is send if component has bones changed by animation.
		// attachments have to be updated also if animation only is used
		DirtyAttachments();
	}
	
	// TODO in model dynamic test mode the triangle mesh changes causing the collision object
	//      to change. this can be done using a component hosted triangle collision shape
	//      which changes. requires no bone update.
	// NOTE for the time being model dynamic is not supported so this entire code is disbaled
	/*if( pTestMode == etmModelDynamic ){
		pUpdateBones();
	}*/
	
	pDirtyShapes = true;
	pDirtySweepTest = true;
	pDirtyStaticTest = true;
	
	MarkDirtyOctree();
}



bool debpColliderComponent::IsSimpleShape() const{
	return pSimplePhyBody != NULL;
}



// private functions
//////////////////////

void debpColliderComponent::pCleanUp(){
	if( pLinkedComponent ){
		pLinkedComponent->SetLinkedCollider( NULL );
	}
	
	if( pBones ){
		delete pBones;
	}
	
	if( pSimplePhyBody ){
		delete pSimplePhyBody;
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

void debpColliderComponent::pUpdateBones(){
	deComponent *component = pColliderComponent.GetComponent();
	int responseType = pColliderComponent.GetResponseType();
	bool enabled = pColliderComponent.GetEnabled();
	int c, constraintCount = GetConstraintCount();
	debpCollisionWorld *dynWorld = GetDynamicsWorld();
	debpCreateShape createShape;
	decVector locPos;
	decQuaternion locOrient;
	decMatrix bcMatrix, bcRotMatrix;
	int b, s;
	
	// invalidate constraints
	for( c=0; c<constraintCount; c++ ){
		GetConstraintAt( c )->SetFirstBody( NULL );
	}
	
	// free bones if existing
	if( pBones ){
		delete pBones;
		pBones = NULL;
	}
	
	// free simple physics body if existing
	if( pSimplePhyBody ){
		delete pSimplePhyBody;
		pSimplePhyBody = NULL;
	}
	
	// remove shapes
	pRigShapes.RemoveAllShapes();
	pHasRigOffset = false;
	pRigOffset.SetZero();
	
	// check if there is a component and a rig
	bool modelCollision = false;
	debpModel *model = NULL;
	deRig *rig = NULL;
	int shapeCount = 0;
	int boneCount = 0;
	
	if( component ){
		deModel * const engModel = component->GetModel();
		if( engModel ){
			model = ( debpModel* )engModel->GetPeerPhysics();
		}
		
		rig = component->GetRig();
		if( rig ){
			boneCount = rig->GetBoneCount();
			shapeCount = rig->GetShapes().GetCount();
			modelCollision = rig->GetModelCollision();
		}
	}
	
	// if there are no shapes but bones check if there is at least one bone with a shape
	bool hasBonesWithShape = false;
	if( shapeCount == 0 ){
		for( b=0; b<boneCount; b++ ){
			if( rig->GetBoneAt( b ).GetShapes().GetCount() > 0 ){
				hasBonesWithShape = true;
				break;
			}
		}
	}
	
	// set test mode to static or dynamic model depending if the model can deform,
	// a component is assigned and a rig is assigned to the component
	pTestMode = etmNone;
	
	if( modelCollision ){
		if( model && model->GetCanDeform() && rig ){
			pTestMode = etmModelDynamic;
			
		}else{
			pTestMode = etmModelStatic;
		}
	}
	
	// Bullet becomes incredibly slow if a dynamic triangle mesh is used. for the time being
	// this mode is disabled by forcing bones without shapes
	if( pTestMode == etmModelDynamic && shapeCount == 0 && boneCount == 0 ){
		modelCollision  = true; // this forces bones to be created although without a shape
		//pTestMode = etmModelStatic; // this is not working. as soon as the model moves bullet dies
	}
	
	// create bones. this is done only if there are no rig shapes and there is at least
	// one bone with a shape. if there are no bones with shapes the model would be
	// impossible to collide with. in this case the model is considered having no rig
	// shapes and no bones
	if( ! modelCollision && shapeCount == 0 && boneCount > 0 && hasBonesWithShape ){
		pBones = new debpColliderBones( *this, &pColliderComponent );
		pTestMode = etmBoneShape;
		
	// if there are no bones with shapes but we have a component then this is a simple one.
	// this can be either a rig with only rig shapes or no rig at all
	}else if( component ){
		// scaling
// 		decVector scale( component->GetScaling().Multiply( pColliderComponent.GetScale() ) );
		const decVector scale( pColliderComponent.GetScale() );
		
		// create the physics body which is the same no matter what shape we have
		pSimplePhyBody = new debpPhysicsBody;
		
		pSimplePhyBody->SetOwnerCollider( this, -1 );
		pSimplePhyBody->SetDynamicsWorld( dynWorld );
		
		pSimplePhyBody->SetLinearVelocity( pLinVelo );
		pSimplePhyBody->SetAngularVelocity( pAngVelo );
		pSimplePhyBody->SetGravity( pGravity );
		
		pSimplePhyBody->SetEnabled( enabled );
		pSimplePhyBody->SetMass( pColliderComponent.GetMass() );
		
		if( responseType == deCollider::ertStatic ){
			pSimplePhyBody->SetResponseType( debpPhysicsBody::ertStatic );
			
		}else if( responseType == deCollider::ertKinematic ){
			if( shapeCount > 0 ){
				pSimplePhyBody->SetResponseType( debpPhysicsBody::ertKinematic );
				
			}else{
				pSimplePhyBody->SetResponseType( debpPhysicsBody::ertStatic ); // tri mesh supports no kinematic!
			}
			
		}else{
			pSimplePhyBody->SetResponseType( debpPhysicsBody::ertDynamic );
		}
		
		pSimplePhyBody->SetPosition( pPosition );
		pSimplePhyBody->SetOrientation( pOrientation );
		
		// use model if model collision is requested. currently doesn't work for dynamics
		if( modelCollision ){
			model->PrepareShape();
			
			if( scale.IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ) ) ){
				pSimplePhyBody->SetShape( model->GetShape() );
				pSimplePhyBody->SetShapeSurface( 0.0f ); // TODO just triangle surface is enough
				
			}else{
				// btScaledBvhTriangleMeshShape would contain working scaling but compound scaling is not working
				
				debpBulletCompoundShape *bulletShape = NULL;
				btCompoundShape *compoundShape = NULL;
				btTransform transform;
				transform.setIdentity(); // required, constructor does not initialize anything
				
				try{
					compoundShape = new btCompoundShape( true );
					compoundShape->setUserPointer( ( void* )( intptr_t )0 );
					compoundShape->addChildShape( transform, model->GetShape()->GetShape() ); // not released on destructor
					
					compoundShape->setLocalScaling( btVector3( ( btScalar )scale.x, ( btScalar )scale.y, ( btScalar )scale.z ) );
						// setLocalScaling has to come last or scaling does not propagate
					
					bulletShape = new debpBulletCompoundShape( compoundShape );
					bulletShape->AddChildShape( model->GetShape() );
					
					pSimplePhyBody->SetShape( bulletShape );
					bulletShape->FreeReference();
					
					pSimplePhyBody->SetShapeSurface( 0.0f );
					
				}catch( const deException & ){
					if( bulletShape ){
						bulletShape->FreeReference();
					}
					throw;
				}
			}
			
		// otherwise use rig shapes if present
		}else if( shapeCount > 0 ){
//	GetBullet()->LogInfo( "pSetRigShape()" );
			debpCreateBulletShape createBulletShape;
			debpShapeSurface shapeSurface;
			
			pRigOffset = rig->GetCentralMassPoint();
			pHasRigOffset = ! pRigOffset.IsZero();
			
			if( pHasRigOffset ){
				pSimplePhyBody->SetPosition( GetMatrix() * pRigOffset );
			}
			
			decVector cmp( -pRigOffset );
			cmp.x *= scale.x;
			cmp.y *= scale.y;
			cmp.z *= scale.z;
			createBulletShape.SetOffset( cmp );
			createBulletShape.SetScale( scale );
			
			for( s=0; s<shapeCount; s++ ){
				createBulletShape.SetShapeIndex( s );
				decShape * const shape = rig->GetShapes().GetAt( s );
				shape->Visit( createBulletShape );
				shape->Visit( shapeSurface );
			}
			createBulletShape.Finish();
			pSimplePhyBody->SetShape( createBulletShape.GetBulletShape() );
			pSimplePhyBody->SetShapeSurface( shapeSurface.GetSurface() );
			
			//pSimplePhyBody->SetCcdParameters( createBulletShape.GetCcdThreshold(), createBulletShape.GetCcdRadius() );
			
			pTestMode = etmRigShape;
			
			// create the collision shapes
			createShape.Reset();
			for( s=0; s<shapeCount; s++ ){
				rig->GetShapes().GetAt( s )->Visit( createShape );
				if( createShape.GetCreatedShape() ){
					pRigShapes.AddShape( createShape.GetCreatedShape() );
					createShape.SetCreatedShape( NULL );
				}
			}
			
		// nothing. revert to no collision
		}else{
			pTestMode = etmNone;
		}
	}
	
	// prepare model octree if required
	if( pTestMode == etmModelStatic || pTestMode == etmModelDynamic ){
		if( model ){
			model->PrepareOctree();
		}
	}
	
	// update constraints
	constraintCount = GetConstraintCount();
	for( c=0; c<constraintCount; c++ ){
		ConstraintChanged( c, &GetConstraintAt( c )->GetConstraint() );
	}
	
	// update
	DirtyBones();
	DirtyAttachments();
	
	// response mode depends on pTestMode
	ResponseTypeChanged();
	
	// mark debug checks dirty so they are checked again the next time the collider is prepared
	pDirtyDebugChecks = true;
	
	UpdateDDSShape(); // debug if enabled
	
	// update auto re-register
	SetAutoColDetPrepare( CalcAutoColDetPrepare() );
	SetAutoColDetFinish( CalcAutoColDetFinish() );
}

// #include <dragengine/common/utils/decTimer.h>
// static decString debugDepth;
void debpColliderComponent::pUpdateAttachments( bool force ){
	if( ! pDirtyAttachments && ! force ){
		return;
	}
	
// 	debugDepth.Append( "  " );
// 	decTimer timer;
	
	deComponent * const component = pColliderComponent.GetComponent();
	const int count = pColliderComponent.GetAttachmentCount();
	
	if( count > 0 ){
		const decDMatrix &posMatrix = GetMatrix();
		const deRig * const rig = component ? component->GetRig() : nullptr;
		const deModel * const model = component ? component->GetModel() : nullptr;
		int i, j;
		
		// adjust all attachments
		for( i=0; i<count; i++ ){
			debpColliderAttachment &bpAttachment = *GetAttachmentAt( i );
			const deColliderAttachment &attachment = *bpAttachment.GetAttachment();
			
// 			decTimer timer2;
			
			switch( attachment.GetAttachType() ){
			case deColliderAttachment::eatStatic:
				bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify );
				break;
				
			case deColliderAttachment::eatRig:{
				if( ! rig || ! model ){
					bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify ); // fall back to static
					break;
				}
				
				deResource &attachedResource = *attachment.GetResource();
				const deResourceManager::eResourceType attResType = ( deResourceManager::eResourceType )
					attachedResource.GetResourceManager()->GetResourceType();
				const bool isAttCollider = attResType == deResourceManager::ertCollider;
				const bool isAttComponent = attResType == deResourceManager::ertComponent;
				
				if( ! isAttCollider && ! isAttComponent ){
					bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify ); // fall back to static
					break;
				}
				
				deCollider * const attachedCollider = isAttCollider ? ( deCollider* )&attachedResource : NULL;
				deColliderVisitorIdentify visitor;
				bool changed = false;
				
				if( attachedCollider ){
					attachedCollider->Visit( visitor );
				}
				
				if( isAttComponent || visitor.IsComponent() ){
					deComponent * const attachedComponent = isAttCollider
						? visitor.CastToComponent().GetComponent() : ( deComponent* )&attachedResource;
					const deModel * const attachedModel = attachedComponent
						? attachedComponent->GetModel() : nullptr;
					
					if( ! attachedComponent || ! attachedModel ){
						bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify ); // fall back to static
						break;
					}
					
					deRig * const attachedRig = attachedComponent->GetRig();
					if( ! attachedRig ){
						bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify ); // fall back to static
						break;
					}
					
					const int boneCount = attachedComponent->GetBoneCount();
					const int vpsCount = attachedComponent->GetVertexPositionSetCount();
					if( boneCount == 0 ){
						bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify ); // fall back to static
						break;
					}
					
					if( bpAttachment.GetDirtyMappings() ){
						bpAttachment.SetBoneMappingCount( boneCount );
						for( j=0; j<boneCount; j++ ){
							bpAttachment.SetBoneMappingAt( j, rig->IndexOfBoneNamed(
								attachedRig->GetBoneAt( j ).GetName() ) );
						}
						
						bpAttachment.SetVpsMappingCount( vpsCount );
						for( j=0; j<vpsCount; j++ ){
							bpAttachment.SetVpsMappingAt( j, model->IndexOfVertexPositionSetNamed(
								attachedModel->GetVertexPositionSetAt( j )->GetName() ) );
						}
						
						bpAttachment.SetDirtyMappings( false );
					}
					
					if( isAttCollider ){
						attachedCollider->SetGeometry( pColliderComponent.GetPosition(),
							pColliderComponent.GetOrientation() );
						
					}else{
						attachedComponent->SetPosition( pColliderComponent.GetPosition() );
						attachedComponent->SetOrientation( pColliderComponent.GetOrientation() );
					}
					
					for( j=0; j<boneCount; j++ ){
						const int boneIndex = bpAttachment.GetBoneMappingAt( j );
						if( boneIndex == -1 ){
							continue;
						}
						
						const deComponentBone &compBone = component->GetBoneAt( boneIndex );
						deComponentBone &attachedCompBone = attachedComponent->GetBoneAt( j );
						
						if( attachedCompBone.GetParentBone() == -1 ){
							// root bone requires component space copy
							( ( debpComponent* )component->GetPeerPhysics() )->PrepareBone( boneIndex );
							const decMatrix boneMatrix( compBone.GetMatrix()
								.QuickMultiply( attachedCompBone.GetInverseOriginalMatrix() ) );
							
							attachedCompBone.SetPosition( boneMatrix.GetPosition() );
							attachedCompBone.SetRotation( boneMatrix.ToQuaternion() );
							
						}else{
							// child bones require only bone local copy
							attachedCompBone.SetPosition( compBone.GetPosition() );
							attachedCompBone.SetRotation( compBone.GetRotation() );
						}
					}
					
					for( j=0; j<vpsCount; j++ ){
						const int vpsIndex = bpAttachment.GetVpsMappingAt( j );
						if( vpsIndex != -1 ){
							attachedComponent->SetVertexPositionSetWeightAt( j,
								component->GetVertexPositionSetWeightAt( vpsIndex ) );
						}
					}
					
					attachedComponent->InvalidateBones();
					
					changed = true; // TODO: can this be optimized to be false in some cases?
					
				}else{
					bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify ); // fall back to static
					break;
				}
				
				// notify attachments if not prevented
				if( changed && isAttCollider ){
					debpCollider &bpCollider = *( ( debpCollider* )attachedCollider->GetPeerPhysics() );
					
					if( pPreventAttNotify ){
						// notification is prevented. register collider for finish detection.
						// at this time the notification can be resolved
						bpCollider.RegisterColDetFinish();
						
					}else{
						attachedCollider->GetPeerScripting()->ColliderChanged( attachedCollider );
						bpCollider.ClearRequiresUpdate();
					}
				}
				}break;
				
			case deColliderAttachment::eatBone:{
				if( ! rig ){
					bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify ); // fall back to static
					break;
				}
				
				if( bpAttachment.GetDirtyMappings() ){
					bpAttachment.SetTrackBone( rig->IndexOfBoneNamed( attachment.GetTrackBone() ) );
					bpAttachment.SetDirtyMappings( false );
				}
				
				const int boneIndex = bpAttachment.GetTrackBone();
				if( boneIndex == -1 ){
					bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify ); // fall back to static
					break;
				}
				
				//deColliderBone *colBone = pColliderComponent.GetBoneAt( boneIndex );
				( ( debpComponent* )component->GetPeerPhysics() )->PrepareBone( boneIndex );
				
				/*if( colBone->GetDynamic() && pBones[ boneIndex ] ){
					bpAttachment.Reposition( pBones[ boneIndex ]->GetRealMatrix(), ! pPreventAttNotify );
					
				}else{*/
					const deComponentBone &cbone = component->GetBoneAt( boneIndex );
					bpAttachment.Reposition( decDMatrix( cbone.GetMatrix() ).QuickMultiply( posMatrix ),
						pLinVelo, ! pPreventAttNotify );
				//}
				
				}break;
				
			case deColliderAttachment::eatWeight:{
				if( ! rig || ! pLinkedComponent ){
					bpAttachment.Reposition( posMatrix, pLinVelo, ! pPreventAttNotify ); // fall back to static
					break;
				}
				
				//debpComponent &bpComponent = *( ( debpComponent* )component->GetPeerPhysics() );
				const deColliderAttachment::sWeight * const weights = attachment.GetWeights();
				const int weightCount = attachment.GetWeightCount();
				
				pLinkedComponent->PrepareBoneWeights(); // TODO optimize this. requires new code
				
				if( bpAttachment.GetDirtyMappings() ){
					bpAttachment.SetBoneMappingCount( weightCount );
					for( j=0; j<weightCount; j++ ){
						bpAttachment.SetBoneMappingAt( j, rig->IndexOfBoneNamed( weights[ j ].bone ) );
					}
					bpAttachment.SetDirtyMappings( false );
				}
				
				if( weightCount > 0 ){
					decDMatrix transform;
					bool first = true;
					
					for( j=0; j<weightCount; j++ ){
						const int boneIndex = bpAttachment.GetBoneMappingAt( j );
						if( boneIndex == -1 ){
							continue;
						}
						
						const float weightFactor = weights[ j ].weight;
						
						if( first ){
							transform = pLinkedComponent->GetBoneAt( boneIndex )
								.weightMatrix.QuickMultiply( weightFactor );
							first = false;
							
						}else{
							transform.QuickAddTo( pLinkedComponent->GetBoneAt( boneIndex )
								.weightMatrix.QuickMultiply( weightFactor ) );
						}
					}
					
					bpAttachment.Reposition( transform.QuickMultiply( posMatrix ), pLinVelo, ! pPreventAttNotify );
				}
				}break;
				
			case deColliderAttachment::eatRelativeMovement:{
				if( pDirtyRelMoveMatrix ){
					pRelMoveMatrix = decDMatrix::CreateTranslation( decDVector( pRelMoveDisplacement ) - pPosition )
						.QuickMultiply( decDMatrix::CreateWorld( pPosition, pRelMoveRotation ) );
					pRelMoveMatrixRot = pRelMoveMatrix.ToQuaternion();
					pDirtyRelMoveMatrix = false;
				}
				
				bpAttachment.Transform( bpAttachment.GetAccumRelMoveMatrix()
					.QuickMultiply( pRelMoveMatrix ), pLinVelo, ! pPreventAttNotify );
				bpAttachment.SetAccumRelMoveMatrix( decDMatrix() );
				}break;
			}
			
// 			const int consumed = ( int )( timer2.GetElapsedTime() * 1e6f );
// 			if( consumed > 1 ) printf( "%sUpdateAttachments2 (%p[%d] %d: %d) %dys\n", debugDepth.GetString(), this,
// 				attachment.GetAttachType(), i, attachment.GetResource()->GetResourceManager()->GetResourceType(), consumed );
		}
	}
	
	pDirtyShapes = true;
	
	pRelMoveDisplacement.SetZero();
	pRelMoveRotation.SetZero();
	pDirtyRelMoveMatrix = true;
	
	// no more dirty
	pDirtyAttachments = false;
	
// 	const int consumed = ( int )( timer.GetElapsedTime() * 1e6f );
// 	debugDepth = debugDepth.GetMiddle( 0, -2 );
// 	if( consumed > 10000 ) printf( "%sUpdateAttachments(%p: %d) %dys\n", debugDepth.GetString(), this, GetAttachmentCount(), consumed );
}

void debpColliderComponent::pApplyAccumRelMoveMatrices(){
	if( ! pDirtyAttachments || ! pDirtyRelMoveMatrix ){
		return;
	}
	
	const int count = pColliderComponent.GetAttachmentCount();
	int i;
	for( i=0; i<count; i++ ){
		debpColliderAttachment &bpAttachment = *GetAttachmentAt( i );
		const deColliderAttachment &attachment = *bpAttachment.GetAttachment();
		
		if( attachment.GetAttachType() == deColliderAttachment::eatRelativeMovement ){
			bpAttachment.Transform( bpAttachment.GetAccumRelMoveMatrix()
				.QuickMultiply( decDMatrix::CreateTranslation( decDVector( pRelMoveDisplacement ) - pPosition ) )
				.QuickMultiply( decDMatrix::CreateWorld( pPosition, pRelMoveRotation ) ),
				pLinVelo, ! pPreventAttNotify );
			bpAttachment.SetAccumRelMoveMatrix( decDMatrix() );
		}
	}
	
	pRelMoveDisplacement.SetZero();
	pRelMoveRotation.SetZero();
	pDirtyRelMoveMatrix = true;
}

void debpColliderComponent::pUpdateIsMoving(){
	SetIsMoving( pColliderComponent.GetResponseType() != deCollider::ertStatic
		&& ( pHasAngVelo || pHasLinVelo || pHasGravity ) );
}



void debpColliderComponent::pUpdateSweepCollisionTest(){
	if( ! pSweepCollisionTest ){
		pSweepCollisionTest = new debpSweepCollisionTest( GetBullet()->GetCollisionDetection() );
		pDirtySweepTest = true;
	}
	
	if( pDirtySweepTest ){
		const decVector &scale = pColliderComponent.GetScale();
		deRig *rig = NULL;
		int count = 0;
		int i;
		
		if( pColliderComponent.GetComponent() ){
			rig = pColliderComponent.GetComponent()->GetRig();;
			if( rig ){
				count = rig->GetShapes().GetCount();
			}
		}
		
		pSweepCollisionTest->RemoveAllShapes();
		
		for( i=0; i<count; i++ ){
			pSweepCollisionTest->AddShape( *rig->GetShapes().GetAt( i ), scale );
		}
		
		pDirtySweepTest = false;
	}
}

void debpColliderComponent::pUpdateStaticCollisionTest(){
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
		pStaticCollisionTestShape = pCreateBPShape();
		
		if( pStaticCollisionTestShape ){
			pStaticCollisionTestShape->AddReference();
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

void debpColliderComponent::pCheckDebugChecks(){
	if( ! pDirtyDebugChecks ){
		return;
	}
	
	// warn if a model is used without a rig unless it can not collide
	/*
	if( pColliderComponent.GetEnabled() && pColliderComponent.GetCollisionFilter().CanCollide() ){
		if( pTestMode == etmModelStatic || pTestMode == etmModelDynamic ){
			const deComponent * const component = pColliderComponent.GetComponent();
			const deModel *model = NULL;
			const deRig *rig = NULL;
			
			if( component ){
				model = component->GetModel();
				rig = component->GetRig();
			}
			
			if( model && ! rig ){
				const decDVector &position = pColliderComponent.GetPosition();
				GetBullet()->LogWarnFormat( "ColliderComponent at (%f,%f,%f) with model '%s' has no rig."
					" Triangle mesh collision will be used. This can be a performance problem.",
					position.x, position.y, position.z, model->GetFilename() );
			}
		}
	}
	*/
	
	pDirtyDebugChecks = false;
}

debpBulletShape *debpColliderComponent::pCreateBPShape(){
	deRig *rig = NULL;
	int count = 0;
	
	if( pColliderComponent.GetComponent() ){
		rig = pColliderComponent.GetComponent()->GetRig();;
		if( rig ){
			count = rig->GetShapes().GetCount();
		}
	}
	
	if( count == 0 ){
		return NULL;
	}
	
	debpCreateBulletShape createBulletShape;
	int i;
	
	createBulletShape.SetScale( pColliderComponent.GetScale() );
	
	for( i=0; i<count; i++ ){
		createBulletShape.SetShapeIndex( i );
		rig->GetShapes().GetAt( i )->Visit( createBulletShape );
	}
	createBulletShape.Finish();
	
	debpBulletShape * const bulletShape = createBulletShape.GetBulletShape();
	if( bulletShape ){
		bulletShape->AddReference(); // otherwise visitor destructor frees created shape
	}
	return bulletShape;
}



bool debpColliderComponent::pPrepreStaticCollisionTestPos(){
	// early exit
	switch(pTestMode){
	case etmRigShape:
		if(!pSimplePhyBody){
			return false;
		}
		break;
		
	case etmBoneShape:
		if(!pBones){
			return false;
		}
		break;
		
	default:
		return false;
	}
	
	// prepare static collision test to be in the right spot
	switch(pTestMode){
	case etmRigShape:
		if(!GetStaticCollisionTestPrepare()){
			return false;
		}
		break;
		
	case etmBoneShape:
		if(pBones && !pBones->UpdateStaticCollisionTests()){
			return false;
		}
		break;
		
	default:
		return false;
	}
	
	return true;
}
