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
#include <string.h>
#include <stdio.h>

#include "reRigConstraint.h"
#include "reRigConstraintDof.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <deigde/gui/igdeShapeBuilder.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWAngleRange.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigConstraint.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class reRigConstraint
//////////////////////////

// Constructor, destructor
////////////////////////////

reRigConstraint::reRigConstraint( deEngine *engine ){
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	igdeShapeBuilder shapeBuilder;
	
	pEngine = engine;
	pRig = NULL;
	pRigBone = NULL;
	
	pDebugDrawer = NULL;
	pDDSConstraint = NULL;
	pDDSJointError = NULL;
	pDDSCoordSys = NULL;
	pDDSOffset = NULL;
	pDDSRangeLinear = NULL;
	pDDSRangeAngularX = NULL;
	pDDSRangeAngularY = NULL;
	pDDSRangeAngularZ = NULL;
	pCollider = NULL;
	pEngConstraint = NULL;
	
	pConstraintBone = NULL;
	
	pDof[ deColliderConstraint::edofLinearX ] = NULL;
	pDof[ deColliderConstraint::edofLinearY ] = NULL;
	pDof[ deColliderConstraint::edofLinearZ ] = NULL;
	pDof[ deColliderConstraint::edofAngularX ] = NULL;
	pDof[ deColliderConstraint::edofAngularY ] = NULL;
	pDof[ deColliderConstraint::edofAngularZ ] = NULL;
	
	pDampingLinear = 1.0f;
	pDampingAngular = 1.0f;
	pDampingSpring = 1.0f;
	
	pIsRope = false;
	
	pBreakingThreshold = 0.0f;
	
	pSelected = false;
	pActive = false;
	
	pShowJointError = false;
	
	pDirtyPositions = true;
	
	try{
		pDof[ deColliderConstraint::edofLinearX ] = new reRigConstraintDof( *this, deColliderConstraint::edofLinearX );
		pDof[ deColliderConstraint::edofLinearY ] = new reRigConstraintDof( *this, deColliderConstraint::edofLinearY );
		pDof[ deColliderConstraint::edofLinearZ ] = new reRigConstraintDof( *this, deColliderConstraint::edofLinearZ );
		pDof[ deColliderConstraint::edofAngularX ] = new reRigConstraintDof( *this, deColliderConstraint::edofAngularX );
		pDof[ deColliderConstraint::edofAngularY ] = new reRigConstraintDof( *this, deColliderConstraint::edofAngularY );
		pDof[ deColliderConstraint::edofAngularZ ] = new reRigConstraintDof( *this, deColliderConstraint::edofAngularZ );
		
		pCollider = engine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled( true );
		pCollider->SetResponseType( deCollider::ertKinematic );
		pCollider->SetUseLocalGravity( true );
		
		decLayerMask layerMask;
		layerMask.SetBit( reRig::eclmConstraints );
		
		pCollider->SetCollisionFilter( decCollisionFilter( layerMask ) );
		
		// create debug drawer and shapes
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay( true );
		
		pDDSConstraint = new igdeWDebugDrawerShape;
		pDDSConstraint->SetVisible( false );
		pDDSConstraint->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSCoordSys = new igdeWCoordSysArrows;
		pDDSCoordSys->SetVisible( false );
		pDDSCoordSys->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSJointError = new igdeWDebugDrawerShape;
		pDDSJointError->SetVisible( false );
		pDDSJointError->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSOffset = new igdeWDebugDrawerShape;
		pDDSOffset->SetVisible( false );
		pDDSOffset->SetEdgeColor( decColor( 1.0f, 0.5f, 0.0f, 1.0f ) );
		pDDSOffset->SetFillColor( decColor( 1.0f, 0.5f, 0.0f, 0.1f ) );
		shapeBuilder.CreateSphere( *pDDSOffset, decVector(), 0.01f );
		pDDSOffset->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSRangeLinear = new igdeWDebugDrawerShape;
		pDDSRangeLinear->SetVisible( false );
		pDDSRangeLinear->SetEdgeColor( decColor( 0.5f, 0.5f, 0.5f, 0.25f ) );
		pDDSRangeLinear->SetFillColor( decColor( 0.5f, 0.5f, 0.5f, 0.05f ) );
		pDDSRangeLinear->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSRangeAngularX = new igdeWAngleRange;
		pDDSRangeAngularX->SetVisible( false );
		pDDSRangeAngularX->SetDiskLineColor( decColor( 1.0f, 0.0f, 0.0f, 1.0f ) );
		pDDSRangeAngularX->SetDiskAreaColor( decColor( 1.0f, 0.0f, 0.0f, 0.2f ) );
		pDDSRangeAngularX->SetRadius( 0.2f );
		pDDSRangeAngularX->SetOrientation( decQuaternion::CreateFromEuler( 0.0f, -90.0f * DEG2RAD, -90.0f * DEG2RAD ) );
		pDDSRangeAngularX->SetParentDebugDrawer( pDebugDrawer  );
		
		pDDSRangeAngularY = new igdeWAngleRange;
		pDDSRangeAngularY->SetVisible( false );
		pDDSRangeAngularY->SetDiskLineColor( decColor( 0.0f, 0.5f, 0.0f, 1.0f ) );
		pDDSRangeAngularY->SetDiskAreaColor( decColor( 0.0f, 0.5f, 0.0f, 0.2f ) );
		pDDSRangeAngularY->SetRadius( 0.18f );
		pDDSRangeAngularY->SetOrientation( decQuaternion::CreateFromEuler( 90.0f * DEG2RAD, 0.0f, -180.0f * DEG2RAD ) );
		pDDSRangeAngularY->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSRangeAngularZ = new igdeWAngleRange;
		pDDSRangeAngularZ->SetVisible( false );
		pDDSRangeAngularZ->SetDiskLineColor( decColor( 0.0f, 0.0f, 1.0f, 1.0f ) );
		pDDSRangeAngularZ->SetDiskAreaColor( decColor( 0.0f, 0.0f, 1.0f, 0.2f ) );
		pDDSRangeAngularZ->SetRadius( 0.16f );
		pDDSRangeAngularZ->SetParentDebugDrawer( pDebugDrawer );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reRigConstraint::~reRigConstraint(){
	pCleanUp();
}



// Management
///////////////

void reRigConstraint::SetRig( reRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	if( pRig ){
		pRig->GetEngineWorld()->RemoveCollider( pCollider );
		pRig->GetEngineWorld()->RemoveDebugDrawer( pDebugDrawer );
	}
	
	pRig = rig;
	
	if( rig ){
		rig->GetEngineWorld()->AddDebugDrawer( pDebugDrawer );
		rig->GetEngineWorld()->AddCollider( pCollider );
	}
	
	InvalidatePositions();
}

void reRigConstraint::SetRigBone( reRigBone *rigBone ){
	if( rigBone == pRigBone ){
		return;
	}
	
	pRigBone = rigBone;
	InvalidatePositions();
}

void reRigConstraint::SetEngineConstraint( deColliderConstraint *constraint ){
	if( constraint == pEngConstraint ){
		return;
	}
	
	pEngConstraint = constraint;
	
	if( constraint ){
		pUpdateConstraintPosition( *constraint );
		
		pDof[ deColliderConstraint::edofLinearX ]->UpdateEngineDof( pEngConstraint->GetDofLinearX() );
		pDof[ deColliderConstraint::edofLinearY ]->UpdateEngineDof( pEngConstraint->GetDofLinearY() );
		pDof[ deColliderConstraint::edofLinearZ ]->UpdateEngineDof( pEngConstraint->GetDofLinearZ() );
		pDof[ deColliderConstraint::edofAngularX ]->UpdateEngineDof( pEngConstraint->GetDofAngularX() );
		pDof[ deColliderConstraint::edofAngularY ]->UpdateEngineDof( pEngConstraint->GetDofAngularY() );
		pDof[ deColliderConstraint::edofAngularZ ]->UpdateEngineDof( pEngConstraint->GetDofAngularZ() );
		
		constraint->SetLinearDamping( pDampingLinear );
		constraint->SetAngularDamping( pDampingAngular );
		constraint->SetSpringDamping( pDampingSpring );
		
		constraint->SetIsRope( pIsRope );
		
		constraint->SetBreakingThreshold( pBreakingThreshold );
		
		if( pConstraintBone && pRig ){
			pEngConstraint->SetBone( pRig->IndexOfBone( pConstraintBone ) );
			
		}else{
			pEngConstraint->SetBone( -1 );
		}
	}
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyEngineConstraintChanged();
}

deRigConstraint *reRigConstraint::BuildEngineRigConstraint(){
	deRigConstraint *engConstraint = NULL;
	
	try{
		engConstraint = new deRigConstraint;
		
		engConstraint->SetReferencePosition( pPosition );
		engConstraint->SetReferenceOrientation( decMatrix::CreateRotation( pOrientation * DEG2RAD ).ToQuaternion() );
		engConstraint->SetBoneOffset( pOffset );
		
		pDof[ deColliderConstraint::edofLinearX ]->UpdateEngineDof( engConstraint->GetDofLinearX() );
		pDof[ deColliderConstraint::edofLinearY ]->UpdateEngineDof( engConstraint->GetDofLinearY() );
		pDof[ deColliderConstraint::edofLinearZ ]->UpdateEngineDof( engConstraint->GetDofLinearZ() );
		pDof[ deColliderConstraint::edofAngularX ]->UpdateEngineDof( engConstraint->GetDofAngularX() );
		pDof[ deColliderConstraint::edofAngularY ]->UpdateEngineDof( engConstraint->GetDofAngularY() );
		pDof[ deColliderConstraint::edofAngularZ ]->UpdateEngineDof( engConstraint->GetDofAngularZ() );
		
		engConstraint->SetLinearDamping( pDampingLinear );
		engConstraint->SetAngularDamping( pDampingAngular );
		engConstraint->SetSpringDamping( pDampingSpring );
		
		engConstraint->SetIsRope( pIsRope );
		
		engConstraint->SetBreakingThreshold( pBreakingThreshold );
		
		if( pRig && pRigBone && pConstraintBone ){
			engConstraint->SetParentBone( pConstraintBone->GetOrder() );
			
		}else{
			engConstraint->SetParentBone( -1 );
		}
		
	}catch( const deException & ){
		if( engConstraint ){
			delete engConstraint;
		}
		throw;
	}
	
	return engConstraint;
}

deColliderConstraint *reRigConstraint::BuildEngineColliderConstraint(){
	deColliderConstraint *engConstraint = NULL;
	
	try{
		engConstraint = new deColliderConstraint;
		
		pUpdateConstraintPosition( *engConstraint );
		
		pDof[ deColliderConstraint::edofLinearX ]->UpdateEngineDof( engConstraint->GetDofLinearX() );
		pDof[ deColliderConstraint::edofLinearY ]->UpdateEngineDof( engConstraint->GetDofLinearY() );
		pDof[ deColliderConstraint::edofLinearZ ]->UpdateEngineDof( engConstraint->GetDofLinearZ() );
		pDof[ deColliderConstraint::edofAngularX ]->UpdateEngineDof( engConstraint->GetDofAngularX() );
		pDof[ deColliderConstraint::edofAngularY ]->UpdateEngineDof( engConstraint->GetDofAngularY() );
		pDof[ deColliderConstraint::edofAngularZ ]->UpdateEngineDof( engConstraint->GetDofAngularZ() );
		
		engConstraint->SetLinearDamping( pDampingLinear );
		engConstraint->SetAngularDamping( pDampingAngular );
		engConstraint->SetSpringDamping( pDampingSpring );
		
		engConstraint->SetIsRope( pIsRope );
		
		engConstraint->SetBreakingThreshold( pBreakingThreshold );
		
	}catch( const deException & ){
		if( engConstraint ){
			delete engConstraint;
		}
		throw;
	}
	
	return engConstraint;
}



void reRigConstraint::SetPosition( const decVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	if( pEngConstraint ){
		pUpdateConstraintPosition( *pEngConstraint );
		NotifyEngineConstraintChanged();
	}
	
	InvalidatePositions();
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyAllConstraintChanged();
}

void reRigConstraint::SetOrientation( const decVector &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	
	if( pEngConstraint ){
		pUpdateConstraintPosition( *pEngConstraint );
		NotifyEngineConstraintChanged();
	}
	
	InvalidatePositions();
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyAllConstraintChanged();
}

void reRigConstraint::SetOffset( const decVector &offset ){
	if( offset.IsEqualTo( pOffset ) ){
		return;
	}
	
	pOffset = offset;
	
	if( pEngConstraint ){
		pUpdateConstraintPosition( *pEngConstraint );
		NotifyEngineConstraintChanged();
	}
	
	pDDSOffset->SetPosition( pOffset );
	InvalidatePositions();
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyAllConstraintChanged();
}



reRigConstraintDof &reRigConstraint::GetDof( deColliderConstraint::eDegreesOfFreedom dof ) const{
	return *pDof[ dof ];
}



void reRigConstraint::SetLinearDamping( float damping ){
	if( fabsf( damping - pDampingLinear ) < 1e-5f ){
		return;
	}
	
	pDampingLinear = damping;
	
	if( pEngConstraint ){
		pEngConstraint->SetLinearDamping( pDampingLinear );
		NotifyEngineConstraintChanged();
	}
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyAllConstraintChanged();
}

void reRigConstraint::SetAngularDamping( float damping ){
	if( fabsf( damping - pDampingAngular ) < 1e-5f ){
		return;
	}
	
	pDampingAngular = damping;
	
	if( pEngConstraint ){
		pEngConstraint->SetAngularDamping( pDampingAngular );
		NotifyEngineConstraintChanged();
	}
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyAllConstraintChanged();
}

void reRigConstraint::SetSpringDamping( float damping ){
	if( fabsf( damping - pDampingSpring ) < 1e-5f ){
		return;
	}
	
	pDampingSpring = damping;
	
	if( pEngConstraint ){
		pEngConstraint->SetSpringDamping( pDampingSpring );
		NotifyEngineConstraintChanged();
	}
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyAllConstraintChanged();
}



void reRigConstraint::SetIsRope( bool isRope ){
	if( isRope == pIsRope ){
		return;
	}
	
	pIsRope = isRope;
	
	if( pEngConstraint ){
		pEngConstraint->SetIsRope( pIsRope );
		NotifyEngineConstraintChanged();
	}
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyAllConstraintChanged();
}

void reRigConstraint::SetBreakingThreshold( float impulseThreshold ){
	if( fabsf( impulseThreshold - pBreakingThreshold ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBreakingThreshold = impulseThreshold;
	
	if( pEngConstraint ){
		pEngConstraint->SetBreakingThreshold( pBreakingThreshold );
		NotifyEngineConstraintChanged();
	}
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyAllConstraintChanged();
}



void reRigConstraint::SetConstraintBone( reRigBone *bone ){
	if( bone == pConstraintBone ){
		return;
	}
	
	if( pConstraintBone ){
		pConstraintBone->FreeReference();
	}
	
	pConstraintBone = bone;
	
	if( bone ){
		bone->AddReference();
	}
	
	if( pEngConstraint ){
		if( pConstraintBone && pRig ){
			pEngConstraint->SetBone( pRig->IndexOfBone( pConstraintBone ) );
			
		}else{
			pEngConstraint->SetBone( -1 );
		}
		pUpdateConstraintPosition( *pEngConstraint );
		NotifyEngineConstraintChanged();
	}
	
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
	NotifyAllConstraintChanged();
	
	InvalidatePositions();
}



void reRigConstraint::SetShowJointError( bool showJointError ){
	if( showJointError == pShowJointError ){
		return;
	}
	
	pShowJointError = showJointError;
	
	InvalidatePositions(); //pUpdateDDConstraint();
}



void reRigConstraint::SetSelected( bool selected ){
	if( selected == pSelected ){
		return;
	}
	
	pSelected = selected;
	
	InvalidatePositions(); //pUpdateDDConstraint();
}

void reRigConstraint::SetActive( bool active ){
	if( active == pActive ){
		return;
	}
	
	pActive = active;
	
	InvalidatePositions(); //pUpdateDDConstraint();
}



void reRigConstraint::ShowStateChanged(){
	InvalidatePositions(); //pUpdateDDConstraint();
	
	pCollider->SetEnabled( IsVisible() );
}



void reRigConstraint::Update(){
	if( pDirtyPositions ){
		pDirtyPositions = false;
		
		pPoseMatrix1.SetRT( pOrientation * DEG2RAD, pPosition );
		pPoseMatrix2 = pPoseMatrix1;
		
		if( pRigBone ){
			if( pConstraintBone ){
				pPoseMatrix1 *= decDMatrix( pRigBone->GetMatrix() )
					* decDMatrix( pConstraintBone->GetInverseMatrix() )
					* pConstraintBone->GetPoseMatrix();
				
			}else{
				pPoseMatrix1 *= pRigBone->GetPoseMatrix();
			}
			
			pPoseMatrix2 *= pRigBone->GetPoseMatrix();
		}
		
		pDebugDrawer->SetPosition( pPoseMatrix1.GetPosition() );
		pDebugDrawer->SetOrientation( pPoseMatrix1.ToQuaternion() );
		
		pUpdateDDConstraint();
		pUpdateDDJointError();
		pUpdateColliderShape();
	}
}

void reRigConstraint::InvalidatePositions(){
	pDirtyPositions = true;
}



bool reRigConstraint::IsVisible() const{
	if( pRig ){
		if( pRigBone ){
			return pRig->GetShowAllBoneConstraints() || pRigBone->GetSelected();
			
		}else{
			return pRig->GetShowRigConstraints();
		}
		
	}else{
		return false;
	}
}

reRigConstraint *reRigConstraint::Duplicate() const{
	reRigConstraint *constraint = NULL;
	
	try{
		constraint = new reRigConstraint( pEngine );
		
		constraint->SetPosition( pPosition );
		constraint->SetOrientation( pOrientation );
		constraint->SetOffset( pOffset );
		
		constraint->GetDofLinearX().SetParametersFrom( *pDof[ deColliderConstraint::edofLinearX ] );
		constraint->GetDofLinearY().SetParametersFrom( *pDof[ deColliderConstraint::edofLinearY ] );
		constraint->GetDofLinearZ().SetParametersFrom( *pDof[ deColliderConstraint::edofLinearZ ] );
		constraint->GetDofAngularX().SetParametersFrom( *pDof[ deColliderConstraint::edofAngularX ] );
		constraint->GetDofAngularY().SetParametersFrom( *pDof[ deColliderConstraint::edofAngularY ] );
		constraint->GetDofAngularZ().SetParametersFrom( *pDof[ deColliderConstraint::edofAngularZ ] );
		
		constraint->SetLinearDamping( pDampingLinear );
		constraint->SetAngularDamping( pDampingAngular );
		constraint->SetSpringDamping( pDampingSpring );
		
		constraint->SetIsRope( pIsRope );
		constraint->SetBreakingThreshold( pBreakingThreshold );
		
		constraint->SetConstraintBone( pConstraintBone );
		
	}catch( const deException & ){
		if( constraint ){
			constraint->FreeReference();
		}
		throw;
	}
	
	return constraint;
}

void reRigConstraint::Scale( float scale ){
	SetPosition( pPosition * scale );
	SetOffset( pOffset * scale );
	
	int i;
	for( i=0; i<3; i++ ){
		pDof[ i ]->SetLowerLimit( pDof[ i ]->GetLowerLimit() * scale );
		pDof[ i ]->SetUpperLimit( pDof[ i ]->GetUpperLimit() * scale );
	}
}



void reRigConstraint::NotifyAllConstraintChanged(){
	if( pRig ){
		pRig->NotifyAllConstraintChanged( this );
	}
}

void reRigConstraint::NotifyEngineConstraintChanged(){
	if( pRig && pEngConstraint ){
		deColliderComponent &engSimCollider = *pRig->GetEngineSimulationCollider();
		engSimCollider.NotifyConstraintChanged( engSimCollider.IndexOfConstraint( pEngConstraint ) );
	}
}



// Private Functions
//////////////////////

void reRigConstraint::pCleanUp(){
	SetRigBone( NULL );
	SetRig( NULL );
	
	if( pConstraintBone ){
		pConstraintBone->FreeReference();
	}
	
	if( pCollider ){
		pCollider->FreeReference();
	}
	
	if( pDof[ deColliderConstraint::edofLinearX ] ){
		delete pDof[ deColliderConstraint::edofLinearX ];
	}
	if( pDof[ deColliderConstraint::edofLinearY ] ){
		delete pDof[ deColliderConstraint::edofLinearY ];
	}
	if( pDof[ deColliderConstraint::edofLinearZ ] ){
		delete pDof[ deColliderConstraint::edofLinearZ ];
	}
	if( pDof[ deColliderConstraint::edofAngularX ] ){
		delete pDof[ deColliderConstraint::edofAngularX ];
	}
	if( pDof[ deColliderConstraint::edofAngularY ] ){
		delete pDof[ deColliderConstraint::edofAngularY ];
	}
	if( pDof[ deColliderConstraint::edofAngularZ ] ){
		delete pDof[ deColliderConstraint::edofAngularZ ];
	}
	
	if( pDDSRangeAngularZ ){
		delete pDDSRangeAngularZ;
	}
	if( pDDSRangeAngularY ){
		delete pDDSRangeAngularY;
	}
	if( pDDSRangeAngularX ){
		delete pDDSRangeAngularX;
	}
	if( pDDSRangeLinear ){
		delete pDDSRangeLinear;
	}
	if( pDDSOffset ){
		delete pDDSOffset;
	}
	if( pDDSJointError ){
		delete pDDSJointError;
	}
	if( pDDSCoordSys ){
		delete pDDSCoordSys;
	}
	if( pDDSConstraint ){
		delete pDDSConstraint;
	}
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
}



void reRigConstraint::pUpdateDDConstraint(){
	bool selvis = IsVisible() && pSelected;
	
	pDDSConstraint->SetVisible( IsVisible() );
	pDDSCoordSys->SetVisible( selvis );
	pDDSOffset->SetVisible( selvis );
	pDDSRangeLinear->SetVisible( selvis );
	pDDSRangeAngularX->SetVisible( selvis );
	pDDSRangeAngularY->SetVisible( selvis );
	pDDSRangeAngularZ->SetVisible( selvis );
	
	pUpdateDDConstraintGeometry();
	
	if( pActive ){
		pDDSConstraint->SetEdgeColor( decColor( 1.0f, 0.5f, 1.0f, 1.0f ) );
		pDDSConstraint->SetFillColor( decColor( 1.0f, 0.5f, 1.0f, 0.1f ) );
		
	}else if( pSelected ){
		pDDSConstraint->SetEdgeColor( decColor( 1.0f, 0.0f, 1.0f, 1.0f ) );
		pDDSConstraint->SetFillColor( decColor( 1.0f, 0.0f, 1.0f, 0.1f ) );
		
	}else{
		pDDSConstraint->SetEdgeColor( decColor( 0.5f, 0.0f, 0.5f, 1.0f ) );
		pDDSConstraint->SetFillColor( decColor( 0.5f, 0.0f, 0.5f, 0.1f ) );
	}
}

void reRigConstraint::pUpdateDDConstraintGeometry(){
	decVector arrowStart, arrowEnd;
	igdeShapeBuilder builder;
	
	// constraint
	if( pDDSConstraint->GetVisible() ){
		if( pRigBone ){
			arrowEnd = pRigBone->GetPoseMatrix().ToMatrix() * pRigBone->GetCentralMassPoint();
			
			if( pConstraintBone ){
				arrowStart = ( pRigBone->GetMatrix() * pConstraintBone->GetInverseMatrix()
					* pConstraintBone->GetPoseMatrix().ToMatrix() ) * pPosition;
				
			}else{
				arrowStart = pPosition;
			}
			
		}else{
			arrowStart = pPosition;
			
			if( pConstraintBone ){
				arrowEnd = pConstraintBone->GetPoseMatrix().ToMatrix() * pConstraintBone->GetCentralMassPoint();
				
			}else{
				arrowEnd = pRig->GetPoseMatrix().ToMatrix().GetPosition();
			}
		}
		
		const decMatrix invPoseMatrix = pPoseMatrix1.Invert().ToMatrix();
		arrowStart = invPoseMatrix * arrowStart;
		arrowEnd = invPoseMatrix * arrowEnd;
		
		if( ( arrowEnd - arrowStart ).Length() > 0.01f ){
			builder.CreateArrow( *pDDSConstraint, arrowStart, arrowEnd, 0.025f, 0.035f, 0.001f );
			
		}else{
			builder.CreateSphere( *pDDSConstraint, arrowStart, 0.025f );
		}
	}
	
	// linear range
	if( pDDSRangeLinear->GetVisible() ){
		decVector lower;
		decVector upper;
		
		lower.x = pDof[ deColliderConstraint::edofLinearX ]->GetLowerLimit();
		upper.x = pDof[ deColliderConstraint::edofLinearX ]->GetUpperLimit();
		if( upper.x - lower.x < 0.001f ){
			upper.x = lower.x + 0.001f;
		}
		
		lower.y = pDof[ deColliderConstraint::edofLinearY ]->GetLowerLimit();
		upper.y = pDof[ deColliderConstraint::edofLinearY ]->GetUpperLimit();
		if( upper.y - lower.y < 0.001f ){
			upper.y = lower.y + 0.001f;
		}
		
		lower.z = pDof[ deColliderConstraint::edofLinearZ ]->GetLowerLimit();
		upper.z = pDof[ deColliderConstraint::edofLinearZ ]->GetUpperLimit();
		if( upper.z - lower.z < 0.001f ){
			upper.z = lower.z + 0.001f;
		}
		
		builder.CreateBoxFromExtends( *pDDSRangeLinear, lower, upper );
	}
	
	// angular range
	if( pDDSRangeAngularX->GetVisible() ){
		const reRigConstraintDof &dof = *pDof[ deColliderConstraint::edofAngularX ];
		
		if( dof.GetUpperLimit() - dof.GetLowerLimit() < -FLOAT_SAFE_EPSILON ){
			pDDSRangeAngularX->SetRange( 0.0f, 360.0f );
			
		}else{
			pDDSRangeAngularX->SetRange( dof.GetLowerLimit(), dof.GetUpperLimit() );
		}
	}
	
	if( pDDSRangeAngularY->GetVisible() ){
		const reRigConstraintDof &dof = *pDof[ deColliderConstraint::edofAngularY ];
		
		if( dof.GetUpperLimit() - dof.GetLowerLimit() < -FLOAT_SAFE_EPSILON ){
			pDDSRangeAngularY->SetRange( 0.0f, 360.0f );
			
		}else{
			pDDSRangeAngularY->SetRange( dof.GetLowerLimit(), dof.GetUpperLimit() );
		}
	}
	
	if( pDDSRangeAngularZ->GetVisible() ){
		const reRigConstraintDof &dof = *pDof[ deColliderConstraint::edofAngularZ ];
		
		if( dof.GetUpperLimit() - dof.GetLowerLimit() < -FLOAT_SAFE_EPSILON ){
			pDDSRangeAngularZ->SetRange( 0.0f, 360.0f );
			
		}else{
			pDDSRangeAngularZ->SetRange( dof.GetLowerLimit(), dof.GetUpperLimit() );
		}
	}
}

void reRigConstraint::pUpdateDDJointError(){
	pDDSJointError->SetVisible( pShowJointError && pRig && pRig->GetSimulationRunning() );
	
	pDDSJointError->SetEdgeColor( decColor( 1.0f, 0.0f, 0.0f, 1.0 ) );
	pDDSJointError->SetFillColor( decColor( 1.0f, 0.0f, 0.0f, 0.5 ) );
	
	if( pDDSJointError->GetVisible() ){
		decVector currentLocation, correctLocation;
		decVector up( 0.0, 1.0f, 0.0f );
		decQuaternion orientation;
		decShapeBox *box = NULL;
		
		if( pRigBone ){
			currentLocation = pRigBone->GetPoseMatrix().ToMatrix() * pPosition;
			
			if( pConstraintBone ){
				correctLocation = ( pRigBone->GetMatrix() * pConstraintBone->GetInverseMatrix() * pConstraintBone->GetPoseMatrix().ToMatrix() ) * pPosition;
				
			}else{
				correctLocation = pPosition;
			}
			
		}else{
			correctLocation = pPosition;
			
			if( pConstraintBone ){
				currentLocation = pConstraintBone->GetPoseMatrix().ToMatrix() * pPosition;
				
			}else{
				currentLocation = pRig->GetPoseMatrix().ToMatrix() * pPosition;
			}
		}
		
		const decMatrix invPoseMatrix = pPoseMatrix1.Invert().ToMatrix();
		currentLocation = invPoseMatrix * currentLocation;
		correctLocation = invPoseMatrix * correctLocation;
		
		decVector view = currentLocation - correctLocation;
		
		if( view.Length() > 1e-6f ){
			view.Normalize();
			
			if( fabs( view * up ) < 0.99999 ){
				orientation = decMatrix::CreateVU( view, view % ( up % view )  ).ToQuaternion();
				
			}else{
				orientation = decMatrix::CreateRotationX( PI * 0.5f ).ToQuaternion();
			}
		}
		
		pDDSJointError->RemoveAllShapes();
		
		try{
			box = new decShapeBox( decVector( 0.001f, 0.001f, ( correctLocation - currentLocation ).Length() * 0.5f ),
				( currentLocation + correctLocation ) * 0.5f, orientation );
			
			pDDSJointError->AddShape( box );
			
		}catch( const deException & ){
			if( box ){
				delete box;
			}
		}
	}
}

void reRigConstraint::pUpdateColliderShape(){
	if( pCollider && pRig ){
		decVector halfExtends( 0.05f, 0.05f, 0.1f );
		decVector up( 0.0, 1.0f, 0.0f );
		decVector arrowStart, arrowEnd;
		decQuaternion orientation;
		decShape *shape = NULL;
		float arrowLen;
		
		if( pRigBone ){
			arrowEnd = pRigBone->GetPoseMatrix().ToMatrix() * pRigBone->GetCentralMassPoint();
			if( pConstraintBone ){
				arrowStart = ( pRigBone->GetMatrix() * pConstraintBone->GetInverseMatrix() * pConstraintBone->GetPoseMatrix().ToMatrix() ) * pPosition;
				
			}else{
				arrowStart = pPosition;
			}
			
		}else{
			arrowStart = pPosition;
			if( pConstraintBone ){
				arrowEnd = pConstraintBone->GetPoseMatrix().ToMatrix() * pConstraintBone->GetCentralMassPoint();
				
			}else{
				arrowEnd = pRig->GetPoseMatrix().ToMatrix().GetPosition();
			}
		}
		
		decVector view = arrowEnd - arrowStart;
		
		if( view.Length() > 1e-6f ){
			view.Normalize();
			
			if( fabs( view * up ) < 0.99999 ){
				orientation = decMatrix::CreateVU( view, view % ( up % view )  ).ToQuaternion();
				
			}else{
				orientation = decMatrix::CreateRotationX( PI * 0.5f ).ToQuaternion();
			}
		}
		
		pCollider->SetPosition( ( arrowStart + arrowEnd ) * 0.5f );
		pCollider->SetOrientation( orientation );
		pCollider->SetEnabled( IsVisible() );
		
		decShapeList shapeList;
		
		arrowLen = ( arrowEnd - arrowStart ).Length();
		
		try{
			if( arrowLen > 0.001f ){
				shape = new decShapeBox( decVector( 0.025f, 0.025f, arrowLen * 0.5f + 0.025f ) );
			
			}else{
				shape = new decShapeSphere( 0.04f );
			}
			
			shapeList.Add( shape );
			
		}catch( const deException & ){
			if( shape ){
				delete shape;
			}
		}
		
		pCollider->SetShapes( shapeList );
	}
}

void reRigConstraint::pUpdateConstraintPosition( deColliderConstraint &engConstraint ){
	if( pConstraintBone ){
		pConstraintBone->UpdateMatrices();
		
		const decMatrix bcRotMatrix = decMatrix::CreateRotation( pOrientation * DEG2RAD );
		engConstraint.SetPosition2( pPosition + bcRotMatrix * pOffset );
		engConstraint.SetOrientation2( decQuaternion() );
		
		const decMatrix &bcMatrix = pConstraintBone->GetInverseMatrix();
		engConstraint.SetPosition1( bcMatrix * pPosition );
		engConstraint.SetOrientation1( ( bcRotMatrix * bcMatrix ).ToQuaternion() );
		
	}else{
		const decQuaternion orientation = decMatrix::CreateRotation( pOrientation * DEG2RAD ).ToQuaternion();
		engConstraint.SetOrientation1( orientation );
		engConstraint.SetOrientation2( orientation );
	}
}
