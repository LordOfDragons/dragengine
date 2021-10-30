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

#include "debpColliderRig.h"
#include "debpColliderAttachment.h"
#include "debpColliderBone.h"
#include "debpColliderBones.h"
#include "debpColliderConstraint.h"
#include "../debpBulletShape.h"
#include "../dePhysicsBullet.h"
#include "../debpRig.h"
#include "../debpMotionState.h"
#include "../debpPhysicsBody.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../shape/debpShape.h"
#include "../shape/debpCreateShape.h"
#include "../visitors/debpCreateBulletShape.h"
#include "../world/debpCollisionWorld.h"
#include "../world/debpWorld.h"

#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderRig.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigConstraint.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"



// Class debpColliderRig
//////////////////////////

// Constructor, destructor
////////////////////////////

debpColliderRig::debpColliderRig( dePhysicsBullet *bullet, deColliderRig &collider ) :
debpCollider( bullet, collider, ectRig ),
pColliderRig( collider )
{
	pHasGravity = false;
	pHasLinVelo = false;
	pHasAngVelo = false;
	pPreventUpdate = false;
	pPreventAttNotify = false;
	pDirtyBones = false;
	pDirtyAttachments = true;
	pDirtyShapes = true;
	
	pSimplePhyBody = NULL;
	pBones = NULL;
	pTestMode = etmNone;
	pRig = NULL;
	
	RigChanged();
	PositionChanged();
	OrientationChanged();
	ScaleChanged();
	LinearVelocityChanged();
	AngularVelocityChanged();
	GravityChanged();
	EnabledChanged();
	PropertiesChanged();
	ResponseTypeChanged();
}

debpColliderRig::~debpColliderRig(){
	pCleanUp();
}



// Management
///////////////

void debpColliderRig::DirtyBones(){
	pDirtyBones = true;
	
	RegisterColDetPrepare();
	RegisterColDetFinish();
}

void debpColliderRig::DirtyAttachments(){
	pDirtyAttachments = true;
	
	RegisterColDetPrepare();
	RegisterColDetFinish();
}



void debpColliderRig::SetParentWorld( debpWorld *parentWorld ){
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

void debpColliderRig::CreateBody(){
	debpCollisionWorld * const dynWorld = GetDynamicsWorld();
	if( ! dynWorld ){
		return;
	}
	
	int c, constraintCount = GetConstraintCount();
	
	for( c=0; c<constraintCount; c++ ){
		GetConstraintAt( c )->SetDynamicsWorld( dynWorld );
	}
	
	if( pSimplePhyBody ){
		pSimplePhyBody->SetDynamicsWorld( dynWorld );
		
	}else if( pBones ){
		pBones->SetDynamicWorld( dynWorld );
	}
	
	GravityChanged();
	}

void debpColliderRig::DestroyBody(){
	int c, constraintCount = GetConstraintCount();
	
	for( c=0; c<constraintCount; c++ ){
		GetConstraintAt( c )->SetDynamicsWorld( NULL );
	}
	
	if( pSimplePhyBody ){
		pSimplePhyBody->SetDynamicsWorld( NULL );
		
	}else if( pBones ){
		pBones->SetDynamicWorld( NULL );
	}
}

void debpColliderRig::UpdateFromBody(){
	// if a simple body just set the collider from the physics body
	if( pSimplePhyBody ){
		if( pSimplePhyBody->UpdateFromBody() ){
			// prevent all updates during this time
			pPreventUpdate = true;
			
			// update
			pColliderRig.SetPosition( pSimplePhyBody->GetPosition() );
			pColliderRig.SetOrientation( pSimplePhyBody->GetOrientation() );
			pColliderRig.SetLinearVelocity( pSimplePhyBody->GetLinearVelocity() );
			pColliderRig.SetAngularVelocity( pSimplePhyBody->GetAngularVelocity() );
			
			// update attachments if required
			pUpdateAttachments( false );
			
			// finished so we can allow updates again.
			pPreventUpdate = false;
		}
		
	// otherwise update bonewise
	}else if( pBones ){
		pPreventUpdate = true;
		if( pBones->UpdateFromBody() ){
			RequiresUpdate();
		}
		//pUpdateAttachments( false ); // not done in component
		pPreventUpdate = false;
	}
}

void debpColliderRig::UpdateExtends(){
	decDVector minExtend, maxExtend;
	
	if( pRig ){
		int b, s, shapeCount = pRigShapes.GetShapeCount();
		decDVector distance( pPredictDisp * 0.5 );
		decDVector minExtendShape, maxExtendShape;
		debpDCollisionBox colBox;
		bool first = true;
		
		// update the collision volumes of the shapes
		UpdateShapes();
		
		// determine the bounding box of all collision volumes
		for( s=0; s<shapeCount; s++ ){
			pRigShapes.GetShapeAt( s )->GetCollisionVolume()->GetEnclosingBox( &colBox );
			
			if( first ){
				minExtend = colBox.GetCenter() - colBox.GetHalfSize();
				maxExtend = colBox.GetCenter() + colBox.GetHalfSize();
				first = false;
				
			}else{
				minExtendShape = colBox.GetCenter() - colBox.GetHalfSize();
				maxExtendShape = colBox.GetCenter() + colBox.GetHalfSize();
				
				if( minExtendShape.x < minExtend.x ) minExtend.x = minExtendShape.x;
				if( minExtendShape.y < minExtend.y ) minExtend.y = minExtendShape.y;
				if( minExtendShape.z < minExtend.z ) minExtend.z = minExtendShape.z;
				
				if( maxExtendShape.x > maxExtend.x ) maxExtend.x = maxExtendShape.x;
				if( maxExtendShape.y > maxExtend.y ) maxExtend.y = maxExtendShape.y;
				if( maxExtendShape.z > maxExtend.z ) maxExtend.z = maxExtendShape.z;
			}
		}
		
		if( pBones ){
			const int bpcount = pBones->GetBonePhysicsCount();
			
			for( b=0; b<bpcount; b++ ){
				const debpShapeList &shapeList = pBones->GetBonePhysicsAt( b ).GetShapes();
				
				shapeCount = shapeList.GetShapeCount();
				
				for( s=0; s<shapeCount; s++ ){
					shapeList.GetShapeAt( s )->GetCollisionVolume()->GetEnclosingBox( &colBox );
					
					if( first ){
						minExtend = colBox.GetCenter() - colBox.GetHalfSize();
						maxExtend = colBox.GetCenter() + colBox.GetHalfSize();
						first = false;
						
					}else{
						minExtendShape = colBox.GetCenter() - colBox.GetHalfSize();
						maxExtendShape = colBox.GetCenter() + colBox.GetHalfSize();
						
						if( minExtendShape.x < minExtend.x ) minExtend.x = minExtendShape.x;
						if( minExtendShape.y < minExtend.y ) minExtend.y = minExtendShape.y;
						if( minExtendShape.z < minExtend.z ) minExtend.z = minExtendShape.z;
						
						if( maxExtendShape.x > maxExtend.x ) maxExtend.x = maxExtendShape.x;
						if( maxExtendShape.y > maxExtend.y ) maxExtend.y = maxExtendShape.y;
						if( maxExtendShape.z > maxExtend.z ) maxExtend.z = maxExtendShape.z;
					}
				}
			}
		}
		
		// enlarge the bounding box at the correct sides by the predicted displacement
		if( distance.x < 0.0 ){
			minExtend.x += distance.x;
			
		}else{
			maxExtend.x += distance.x;
		}
		
		if( distance.y < 0.0 ){
			minExtend.y += distance.y;
			
		}else{
			maxExtend.y += distance.y;
		}
		
		if( distance.z < 0.0 ){
			minExtend.z += distance.z;
			
		}else{
			maxExtend.z += distance.z;
		}
	}
	
	SetExtends( minExtend, maxExtend );
}

void debpColliderRig::PrepareForStep(){
	debpCollider::PrepareForStep();
	
	pPreventAttNotify = true;
	RegisterColDetFinish(); // to disable pPreventAttNotify
	
	if( pBones ){
		pBones->ActivateDirtyPhysicsBodies();
		pDirtyBones = false;
	}
	
	pUpdateAttachments( false );
	PrepareRigConstraintsForStep();
}



void debpColliderRig::PrepareDetection( float elapsed ){
	debpCollider::PrepareDetection( elapsed );
	
	// update the kinematic bones if using bone test mode
	if( pBones && pDirtyBones ){
		// bullet uses interpolation of the previous transformation and velocities with the current
		// values to calculate CCD in particular. if the user sets the collider bone parameter himself
		// it is assumed the object teleported or has been reset. in this case the interpolation has
		// to be reset to prevent wrong results. to achieve this the pResetKinematicInterpolation is
		// used to determine if Bone*Changed calls do reset the interpolation or not. during this
		// preparation code Bone*Changed is not allowed to reset the interpolation or the CCD breaks
// 		pResetKinematicInterpolation = false;
		pBones->PrepareForDetection( elapsed );
// 		pResetKinematicInterpolation = true;
	}
}

void debpColliderRig::FinishDetection(){
	if( GetIsPrepared() ){
		CheckRigConstraintsBroke();
	}
	
	debpCollider::FinishDetection();
	
	pPreventAttNotify = false;
}

bool debpColliderRig::CalcAutoColDetPrepare(){
	if( debpCollider::CalcAutoColDetPrepare() ){
		return true;
	}
	
	if( pBones && pBones->GetBonePhysicsCount() > 0 ){
		return pColliderRig.GetResponseType() == deCollider::ertDynamic || pBones->HasConstraints();
	}
	
	return false;
}

bool debpColliderRig::CalcAutoColDetFinish(){
	if( debpCollider::CalcAutoColDetPrepare() ){
		return true;
	}
	
	if( pBones && pBones->GetBonePhysicsCount() > 0 ){
		return pColliderRig.GetResponseType() == deCollider::ertDynamic || pBones->HasConstraints();
	}
	
	return false;
}



void debpColliderRig::PrepareRigConstraintsForStep(){
	if( pBones ){
		pBones->PrepareConstraintsForStep();
	}
}

void debpColliderRig::CheckRigConstraintsBroke(){
	if( pBones ){
		pBones->CheckConstraintsBroke();
	}
}



void debpColliderRig::UpdateCollisionObjectAABBs(){
	if( pSimplePhyBody ){
		pSimplePhyBody->UpdateAABB();
		
	}else if( pBones ){
		pBones->UpdatePhyBodyAABBs();
	}
}



void debpColliderRig::UpdateShapes(){
	if( ! pDirtyShapes ){
		return;
	}
	
	const decDMatrix &matrix = GetMatrix();
	
	pRigShapes.UpdateWithMatrix( matrix );
	if( pBones ){
		pBones->UpdateShapes();
	}
	
	pUpdateShapeExtends();
	
	pDirtyShapes = false;
}

void debpColliderRig::UpdateShapesWithMatrix( const decDMatrix &transformation ){
	pRigShapes.UpdateWithMatrix( transformation );
	if( pBones ){
		pBones->UpdateShapes( transformation );
	}
	
	pUpdateShapeExtends();
}

void debpColliderRig::pUpdateShapeExtends(){
	decDVector minExtend, maxExtend;
	
	if( pRig ){
		int b, s, shapeCount = pRigShapes.GetShapeCount();
		decDVector distance( pPredictDisp * 0.5 );
		decDVector minExtendShape, maxExtendShape;
		debpDCollisionBox colBox;
		bool first = true;
		
		for( s=0; s<shapeCount; s++ ){
			pRigShapes.GetShapeAt( s )->GetCollisionVolume()->GetEnclosingBox( &colBox );
			
			if( first ){
				minExtend = colBox.GetCenter() - colBox.GetHalfSize();
				maxExtend = colBox.GetCenter() + colBox.GetHalfSize();
				first = false;
				
			}else{
				minExtendShape = colBox.GetCenter() - colBox.GetHalfSize();
				maxExtendShape = colBox.GetCenter() + colBox.GetHalfSize();
				
				if( minExtendShape.x < minExtend.x ) minExtend.x = minExtendShape.x;
				if( minExtendShape.y < minExtend.y ) minExtend.y = minExtendShape.y;
				if( minExtendShape.z < minExtend.z ) minExtend.z = minExtendShape.z;
				
				if( maxExtendShape.x > maxExtend.x ) maxExtend.x = maxExtendShape.x;
				if( maxExtendShape.y > maxExtend.y ) maxExtend.y = maxExtendShape.y;
				if( maxExtendShape.z > maxExtend.z ) maxExtend.z = maxExtendShape.z;
			}
		}
		
		if( pBones ){
			const int bpcount = pBones->GetBonePhysicsCount();
			
			for( b=0; b<bpcount; b++ ){
				const debpShapeList &shapeList = pBones->GetBonePhysicsAt( b ).GetShapes();
				
				shapeCount = shapeList.GetShapeCount();
				
				for( s=0; s<shapeCount; s++ ){
					shapeList.GetShapeAt( s )->GetCollisionVolume()->GetEnclosingBox( &colBox );
					
					if( first ){
						minExtend = colBox.GetCenter() - colBox.GetHalfSize();
						maxExtend = colBox.GetCenter() + colBox.GetHalfSize();
						first = false;
						
					}else{
						minExtendShape = colBox.GetCenter() - colBox.GetHalfSize();
						maxExtendShape = colBox.GetCenter() + colBox.GetHalfSize();
						
						if( minExtendShape.x < minExtend.x ) minExtend.x = minExtendShape.x;
						if( minExtendShape.y < minExtend.y ) minExtend.y = minExtendShape.y;
						if( minExtendShape.z < minExtend.z ) minExtend.z = minExtendShape.z;
						
						if( maxExtendShape.x > maxExtend.x ) maxExtend.x = maxExtendShape.x;
						if( maxExtendShape.y > maxExtend.y ) maxExtend.y = maxExtendShape.y;
						if( maxExtendShape.z > maxExtend.z ) maxExtend.z = maxExtendShape.z;
					}
				}
			}
		}
	}
	
	SetExtends( minExtend, maxExtend );
}



bool debpColliderRig::IsSimpleShape() const{
	return pSimplePhyBody != NULL;
}



// Debugging
//////////////

void debpColliderRig::UpdateDebugDrawer(){
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

void debpColliderRig::UpdateDDSShape(){
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
		ddshape->GetShapeList() = pRig->GetRig().GetShapes();
		break;
		
	default:
		ddshape->GetShapeList().RemoveAll();
		break;
	}
	
	GetDebugDrawer()->NotifyShapeContentChanged();
}



// Actions
////////////

void debpColliderRig::EnableBoneConstraint( int bone, int constraint, bool enable ){
	GetBullet()->LogWarn( "EnableBoneConstraint not implemented in debpColliderRig" );
}

void debpColliderRig::ReplaceBoneConstraint( int bone, int constraint, const deRigConstraint &replacement ){
	GetBullet()->LogWarn( "ReplaceBoneConstraint not implemented in debpColliderRig" );
}



// forces
///////////

void debpColliderRig::ApplyForce( const decVector &force ){
	if( pSimplePhyBody ){
		pSimplePhyBody->ApplyForce( force );
		
	}else if( pBones ){
		pBones->ApplyForce( force );
	}
}

void debpColliderRig::ApplyForceAt( const decVector &force, const decVector &point ){
	if( pSimplePhyBody ){
		pSimplePhyBody->ApplyForceAt( force, point );
		
	}else if( pBones ){
		pBones->ApplyForceAt( force, point );
	}
}

void debpColliderRig::ApplyTorque( const decVector &torque ){
	if( pSimplePhyBody ){
		pSimplePhyBody->ApplyTorque( torque );
		
	}else if( pBones ){
		pBones->ApplyTorque( torque );
	}
}



void debpColliderRig::ApplyBoneForce( int bone, const decVector &force ){
	debpColliderBone * const cbone = pBones ? pBones->GetBoneAt( bone ) : NULL;
	if( cbone ){
		cbone->GetPhysicsBody()->ApplyForce( force );
	}
}

void debpColliderRig::ApplyBoneForceAt( int bone, const decVector &force, const decVector &point ){
	debpColliderBone * const cbone = pBones ? pBones->GetBoneAt( bone ) : NULL;
	if( cbone ){
		cbone->GetPhysicsBody()->ApplyForceAt( force, point );
	}
}

void debpColliderRig::ApplyBoneTorque( int bone, const decVector &torque ){
	debpColliderBone * const cbone = pBones ? pBones->GetBoneAt( bone ) : NULL;
	if( cbone ){
		cbone->GetPhysicsBody()->ApplyTorque( torque );
	}
}



// Notifications
//////////////////

void debpColliderRig::PositionChanged(){
	const decDVector &position = pColliderRig.GetPosition();
	if( pPosition.IsEqualTo( position ) ){
		return;
	}
	
	debpCollider::PositionChanged();
	
	pPosition = position;
	
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		if( pSimplePhyBody ){
			pSimplePhyBody->SetPosition( position );
			
		}else{
			DirtyBones();
		}
	}
	
	pDirtyShapes = true;
	
	RequiresUpdate();
	
	if( pColliderRig.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderRig::OrientationChanged(){
	const decQuaternion &orientation = pColliderRig.GetOrientation();
	if( pOrientation.IsEqualTo( orientation ) ){
		return;
	}
	
	debpCollider::OrientationChanged();
	
	pOrientation = orientation;
	
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		if( pSimplePhyBody ){
			pSimplePhyBody->SetOrientation( orientation );
			
		}else{
			DirtyBones();
		}
	}
	
	pDirtyShapes = true;
	
	RequiresUpdate();
	
	if( pColliderRig.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderRig::ScaleChanged(){
	const decVector &scale = pColliderRig.GetScale();
	
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
	pUpdateBones();
	
	RequiresUpdate();
	
	if( pColliderRig.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderRig::GeometryChanged(){
	const decDVector &position = pColliderRig.GetPosition();
	const decQuaternion &orientation = pColliderRig.GetOrientation();
	if( pPosition.IsEqualTo( position ) && pOrientation.IsEqualTo( orientation ) ){
		return;
	}
	
	debpCollider::GeometryChanged();
	
	pPosition = position;
	pOrientation = orientation;
	
	MarkMatrixDirty();
	MarkDirtyOctree();
	
	if( ! pPreventUpdate ){
		if( pSimplePhyBody ){
			pSimplePhyBody->SetPosition( position );
			pSimplePhyBody->SetOrientation( orientation );
			
		}else{
			DirtyBones();
		}
	}
	
	pDirtyShapes = true;
	
	RequiresUpdate();
	
	if( pColliderRig.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}

void debpColliderRig::LinearVelocityChanged(){
	const decVector &linVelo = pColliderRig.GetLinearVelocity();
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

void debpColliderRig::AngularVelocityChanged(){
	const decVector &angVelo = pColliderRig.GetAngularVelocity();
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

void debpColliderRig::GravityChanged(){
	if( pColliderRig.GetUseLocalGravity() ){
		pGravity = pColliderRig.GetGravity();
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

void debpColliderRig::EnabledChanged(){
	const bool enabled = pColliderRig.GetEnabled();
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

void debpColliderRig::PropertiesChanged(){
	if( pSimplePhyBody ){
		pSimplePhyBody->SetMass( pColliderRig.GetMass() );
	}
	
	GravityChanged();
}

void debpColliderRig::ResponseTypeChanged(){
	const deCollider::eResponseType responseType = pColliderRig.GetResponseType();
	
	if( pSimplePhyBody ){
		if( responseType == deCollider::ertStatic ){
			pSimplePhyBody->SetResponseType( debpPhysicsBody::ertStatic );
			SetUseKinematicSimulation( false );
			
		}else if( responseType == deCollider::ertKinematic ){
			pSimplePhyBody->SetResponseType( debpPhysicsBody::ertKinematic );
			SetUseKinematicSimulation( true );
			
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

void debpColliderRig::CollisionFilterChanged(){
	if( pSimplePhyBody ){
		pSimplePhyBody->CollisionFilteringChanged();
		
	}else if( pBones ){
		pBones->CollisionFilteringChanged();
	}
}

void debpColliderRig::IgnoreCollidersChanged(){
	CollisionFilterChanged();
}

void debpColliderRig::RigChanged(){
	deRig *engRig = pColliderRig.GetRig();
	
	if( engRig ){
		pRig = ( debpRig* )engRig->GetPeerPhysics();
		
	}else{
		pRig = NULL;
	}
	
	pUpdateBones();
	MarkDirtyOctree();
	
	SetAddToOctree( pRig != NULL );
}



void debpColliderRig::AttachmentAdded( int index, deColliderAttachment *attachment ){
	debpCollider::AttachmentAdded( index, attachment );
	DirtyAttachments();
}

void debpColliderRig::AttachmentChanged( int index, deColliderAttachment *attachment ){
	debpCollider::AttachmentChanged( index, attachment );
	DirtyAttachments();
}

void debpColliderRig::AttachmentRemoved( int index, deColliderAttachment *attachment ){
	debpCollider::AttachmentRemoved( index, attachment );
	DirtyAttachments();
}

void debpColliderRig::AllAttachmentsRemoved(){
	debpCollider::AllAttachmentsRemoved();
	DirtyAttachments();
}

void debpColliderRig::AttachmentsForceUpdate(){
	// prepare bones?
	if( pColliderRig.GetAttachmentCount() > 0 ){
		pUpdateAttachments( true );
	}
}



void debpColliderRig::ConstraintAdded( int index, deColliderConstraint *constraint ){
	debpCollider::ConstraintAdded( index, constraint );
	
	debpColliderConstraint &bpConstraint = *GetConstraintAt( index );
	bpConstraint.SetDynamicsWorld( GetDynamicsWorld() );
	bpConstraint.SetEnabled( pColliderRig.GetEnabled() );
	
	ConstraintChanged( index, constraint );
}

void debpColliderRig::ConstraintChanged( int index, deColliderConstraint *constraint ){
	debpCollider::ConstraintChanged( index, constraint );
	
	debpColliderConstraint &bpconstr = *GetConstraintAt( index );
	
	debpPhysicsBody *oldbody = bpconstr.GetFirstBody();
	int boneIndex = constraint->GetBone();
	debpPhysicsBody *newbody = NULL;
	decDVector offset;
	
	if( pSimplePhyBody ){
		newbody = pSimplePhyBody;
		
	}else if( pBones ){
		debpColliderBone *cbone = boneIndex >= 0 && boneIndex < pBones->GetBoneCount()
			? pBones->GetBoneAt( boneIndex ) : pBones->GetRootBone();
		if( cbone ){
			newbody = cbone->GetPhysicsBody();
			offset = cbone->GetOffset();
		}
	}
	
	if( newbody != oldbody ){
		bpconstr.SetFirstBody( newbody );
		bpconstr.SetFirstOffset( offset.ToVector() );
	}
	
	bpconstr.ConstraintChanged();
}



void debpColliderRig::BonePositionChanged( int index ){
	if( ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( ! cbone ){
		return;
	}
	
	if( ! pPreventUpdate ){
		cbone->GetPhysicsBody()->SetPosition( pColliderRig.GetBoneAt( index ).GetPosition() );
	}
	
	if( pColliderRig.GetAttachmentCount() > 0 ){
		DirtyAttachments();
	}
	
	pDirtyShapes = true;
	MarkDirtyOctree();
}

void debpColliderRig::BoneOrientationChanged( int index ){
	if( ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( ! cbone ){
		return;
	}
	
	if( ! pPreventUpdate ){
		cbone->GetPhysicsBody()->SetOrientation( pColliderRig.GetBoneAt( index ).GetOrientation() );
	}
	
	if( pColliderRig.GetAttachmentCount() > 0 ){
		DirtyAttachments();
	}
	
	pDirtyShapes = true;
	MarkDirtyOctree();
}

void debpColliderRig::BoneLinearVelocityChanged( int index ){
	if( pPreventUpdate ){
		return;
	}
	if( ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( ! cbone ){
		return;
	}
	
	cbone->GetPhysicsBody()->SetLinearVelocity( pColliderRig.GetBoneAt( index ).GetLinearVelocity() );
}

void debpColliderRig::BoneAngularVelocityChanged( int index ){
	if( pPreventUpdate ){
		return;
	}
	if( ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( ! cbone ){
		return;
	}
	
	cbone->GetPhysicsBody()->SetAngularVelocity( pColliderRig.GetBoneAt( index ).GetAngularVelocity() );
}

void debpColliderRig::BonePropertiesChanged( int index ){
	if( ! pBones ){
		return;
	}
	
	debpColliderBone * const cbone = pBones->GetBoneAt( index );
	if( cbone ){
		cbone->GetPhysicsBody()->SetMass( pColliderRig.GetBoneAt( index ).GetMass() );
	}
}

void debpColliderRig::BoneDynamicChanged( int index ){
	if( pBones ){
		pBones->UpdatePhysicsType( pColliderRig.GetResponseType(), index );
	}
}



// Private Functions
//////////////////////

void debpColliderRig::pCleanUp(){
	if( pBones ){
		delete pBones;
	}
	if( pSimplePhyBody ){
		delete pSimplePhyBody;
	}
}

void debpColliderRig::pUpdateBones(){
	int responseType = pColliderRig.GetResponseType();
	bool enabled = pColliderRig.GetEnabled();
	int c, constraintCount = GetConstraintCount();
	debpColliderConstraint *bpConstraint = NULL;
	debpCollisionWorld *dynWorld = GetDynamicsWorld();
	debpCreateShape createShape;
	int boneCount = 0;
	const deRig *engRig = NULL;
	decVector locPos;
	decQuaternion locOrient;
	int s, shapeCount = 0;
	decMatrix bcMatrix, bcRotMatrix;
	
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
	
	// check if there is a rig
	if( pRig ){
		engRig = &pRig->GetRig();
		boneCount = engRig->GetBoneCount();
		shapeCount = engRig->GetShapes().GetCount();
	}
	
	pTestMode = etmNone;
	
	// create bones
	if( shapeCount == 0 && boneCount > 0 ){
		pBones = new debpColliderBones( *this, &pColliderRig );
		
		// update constraints. (what's this doing here? that makes no sense)
		/*if( pRootBone != -1 ){
			constraintCount = GetConstraintCount();
			
			for( c=0; c<constraintCount; c++ ){
				bpConstraint = GetConstraintAt( c );
				
				bpConstraint->SetFirstBody( pBones[ pRootBone ]->GetPhysicsBody() );
				bpConstraint->SetFirstOffset( pBones[ pRootBone ]->GetOffset().ToVector() );
			}
		}*/
		
	// if there are no bones but we have a rig then this is a simple one
	}else if( shapeCount > 0 ){
		// create the physics body which is the same no matter what shape we have
		pSimplePhyBody = new debpPhysicsBody;
		
		pSimplePhyBody->SetOwnerCollider( this, -1 );
		pSimplePhyBody->SetDynamicsWorld( dynWorld );
		
		pSimplePhyBody->SetLinearVelocity( pLinVelo );
		pSimplePhyBody->SetAngularVelocity( pAngVelo );
		pSimplePhyBody->SetGravity( pGravity );
		
		pSimplePhyBody->SetEnabled( enabled );
		pSimplePhyBody->SetMass( pColliderRig.GetMass() );
		
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
		
		// use rig shapes as shape for the physics body
		debpCreateBulletShape createBulletShape;
		
		const decVector &scale = pColliderRig.GetScale();
		createBulletShape.SetScale( scale );
		
		decVector cmp( -engRig->GetCentralMassPoint() );
		cmp.x *= scale.x;
		cmp.y *= scale.y;
		cmp.z *= scale.z;
		createBulletShape.SetOffset( cmp );
		
		for( s=0; s<shapeCount; s++ ){
			createBulletShape.SetShapeIndex( s );
			engRig->GetShapes().GetAt( s )->Visit( createBulletShape );
		}
		pSimplePhyBody->SetShape( createBulletShape.GetBulletShape() );
		
		//pSimplePhyBody->SetCcdParameters( createBulletShape.GetCcdThreshold(), createBulletShape.GetCcdRadius() );
		
		pTestMode = etmRigShape;
		
		// create the collision shapes
		createShape.Reset();
		for( s=0; s<shapeCount; s++ ){
			engRig->GetShapes().GetAt( s )->Visit( createShape );
			
			if( createShape.GetCreatedShape() ){
				pRigShapes.AddShape( createShape.GetCreatedShape() );
				createShape.SetCreatedShape( NULL );
			}
		}
		
		// update constraints
		constraintCount = GetConstraintCount();
		for( c=0; c<constraintCount; c++ ){
			bpConstraint = GetConstraintAt( c );
			
			bpConstraint->SetFirstBody( pSimplePhyBody );
			bpConstraint->SetFirstOffset( decVector() );
		}
	}
	
	DirtyBones();
	DirtyAttachments();
	
	UpdateDDSShape(); // debug if enabled
	
	// update auto re-register
	SetAutoColDetPrepare( CalcAutoColDetPrepare() );
	SetAutoColDetFinish( CalcAutoColDetFinish() );
}

void debpColliderRig::pUpdateAttachments( bool force ){
	if( ! pDirtyAttachments && ! force ){
		return;
	}
	
	int i, j, count = pColliderRig.GetAttachmentCount();
	const decDMatrix &posMatrix = GetMatrix();
	deColliderVisitorIdentify visitor;
	const deRig *engRig = NULL;
	bool changed = false;
	float weightFactor;
	int boneIndex;
	
	if( pRig ){
		engRig = &pRig->GetRig();
	}
	
	for( i=0; i<count; i++ ){
		debpColliderAttachment &bpAttachment = *GetAttachmentAt( i );
		const deColliderAttachment &attachment = *bpAttachment.GetAttachment();
		deResource * const attachedResource = attachment.GetResource();
		if( attachedResource->GetResourceManager()->GetResourceType() != deResourceManager::ertCollider ){
			continue;
		}
		deCollider &attachedCollider = *( ( deCollider* )attachedResource );
		const int attachType = attachment.GetAttachType();
		
		attachedCollider.Visit( visitor );
		
		changed = false;
		
		if( attachType == deColliderAttachment::eatStatic ){
			bpAttachment.Reposition( posMatrix, ! pPreventAttNotify );
			
		}else if( attachType == deColliderAttachment::eatRig ){
			if( ! engRig ){
				continue;
			}
			
			// this is going to be a bit more complex since. most simple solution would be to set
			// the collider bones from this collider bones and then letting the other collider
			// deal with the rest. not important right now as for the first test attaching
			// colliders is not a requirement
			/*
			if( visitor.IsComponent() ){
				const deColliderComponent &attachedColComp = visitor.CastToComponent();
				deComponent * const attachedComponent = attachedColComp.GetComponent();
				
				if( attachedComponent ){
					deRig * const attachedRig = attachedComponent->GetRig();
					
					if( attachedRig ){
						boneCount = attachedComponent->GetBoneCount();
						
						if( boneCount > 0 ){
							if( bpAttachment.GetDirtyMappings() ){
								bpAttachment.SetBoneMappingCount( boneCount );
								
								for( j=0; j<boneCount; j++ ){
									bpAttachment.SetBoneMappingAt( j, engRig->IndexOfBoneNamed( attachedRig->GetBoneAt( j )->GetName() ) );
								}
								
								bpAttachment->SetDirtyMappings( false );
							}
							
							attachedCollider->SetPosition( pColliderRig.GetPosition() );
							attachedCollider->SetOrientation( pColliderRig.GetOrientation() );
							
							for( j=0; j<boneCount; j++ ){
								boneIndex = bpAttachment.GetBoneMappingAt( j );
								
								if( boneIndex != -1 ){
									const deComponentBone &compBone = *component->GetBone( boneIndex );
									deComponentBone &attachedCompBone = attachedComponent->GetBone( j );
									
									attachedCompBone->SetPosition( compBone->GetPosition() );
									attachedCompBone->SetRotation( compBone->GetRotation() );
								}
							}
							
							attachedComponent->InvalidateBones();
							changed = true;
						}
					}
				}
			}
			*/
			
		}else if( attachType == deColliderAttachment::eatBone ){
			if( engRig ){
				if( bpAttachment.GetDirtyMappings() ){
					bpAttachment.SetTrackBone( engRig->IndexOfBoneNamed( attachment.GetTrackBone() ) );
					bpAttachment.SetDirtyMappings( false );
				}
				
				boneIndex = bpAttachment.GetTrackBone();
				
				if( boneIndex == -1 ){
					bpAttachment.Reposition( posMatrix, ! pPreventAttNotify );
					
				}else{
					const deColliderBone &colBone = pColliderRig.GetBoneAt( boneIndex );
					bpAttachment.Reposition( colBone.GetMatrix(), ! pPreventAttNotify );
				}
			}
			
		}else if( attachType == deColliderAttachment::eatWeight ){
			if( engRig ){
				const deColliderAttachment::sWeight * const weights = attachment.GetWeights();
				const int weightCount = attachment.GetWeightCount();
				
				if( bpAttachment.GetDirtyMappings() ){
					bpAttachment.SetBoneMappingCount( weightCount );
					
					for( j=0; j<weightCount; j++ ){
						bpAttachment.SetBoneMappingAt( j, engRig->IndexOfBoneNamed( weights[ j ].bone ) );
					}
					
					bpAttachment.SetDirtyMappings( false );
				}
				
				if( weightCount > 0 ){
					decDMatrix transform;
					
					for( j=0; j<weightCount; j++ ){
						boneIndex = bpAttachment.GetBoneMappingAt( j );
						weightFactor = weights[ j ].weight;
						
						if( boneIndex != -1 ){
							transform.QuickAddTo( pColliderRig.GetBoneAt( boneIndex ).GetMatrix().QuickMultiply( weightFactor ) );
						}
					}
					
					bpAttachment.Reposition( transform, ! pPreventAttNotify );
				}
			}
			
		}else if( attachType == deColliderAttachment::eatRelativeMovement ){
			// TODO
		}
		
		// notify attachments if not prevented
		if( changed ){
			debpCollider &bpCollider = *( ( debpCollider* )attachedCollider.GetPeerPhysics() );
			
			if( pPreventAttNotify ){
				// notification is prevented. register collider for finish detection.
				// at this time the notification can be resolved
				bpCollider.RegisterColDetFinish();
				
			}else{
				attachedCollider.GetPeerScripting()->ColliderChanged( &attachedCollider );
				bpCollider.ClearRequiresUpdate();
			}
		}
	}
	
	pDirtyShapes = true;
	
	pDirtyAttachments = false;
}

void debpColliderRig::pUpdateIsMoving(){
	SetIsMoving( pColliderRig.GetResponseType() != deCollider::ertStatic
		&& ( pHasAngVelo || pHasLinVelo || pHasGravity ) );
}
