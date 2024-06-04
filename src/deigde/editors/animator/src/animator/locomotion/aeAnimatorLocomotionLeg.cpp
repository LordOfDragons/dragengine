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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeAnimatorLocomotion.h"
#include "aeAnimatorLocomotionLeg.h"
#include "../aeAnimator.h"
#include "../controller/aeController.h"
#include "../../visitors/aeCLClosestHit.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/sensor/deTouchSensorManager.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/exceptions.h>



// Class aeAnimatorLocomotionLeg
//////////////////////////////////

// Constructor, destructor
////////////////////////////

aeAnimatorLocomotionLeg::aeAnimatorLocomotionLeg( aeAnimatorLocomotion *locomotion ){
	if( ! locomotion ) DETHROW( deeInvalidParam );
	
	deDebugDrawer *debugDrawer = locomotion->GetDebugDrawer();
	
	pLocomotion = locomotion;
	
	pLiftOffTime = 0.0f;
	pPutDownTime = 0.5f;
	
	pPositionLocked = false;
	
	pIKInfluence = 0.0f;
	
	pDDSLocked.SetParentDebugDrawer( debugDrawer );
	pDDSLocked.SetVisible( false );
	pDDSLocked.SetEdgeColor( decColor( 0.0f, 0.5f, 0.75f, 0.25f ) );
	pDDSLocked.SetFillColor( decColor( 0.0f, 0.5f, 0.75f, 0.1f ) );
	
	pDDSLockedCF.SetParentDebugDrawer( debugDrawer );
	pDDSLockedCF.SetVisible( false );
	
	pDDSPredict.SetParentDebugDrawer( debugDrawer );
	pDDSPredict.SetVisible( false );
	pDDSPredict.SetEdgeColor( decColor( 0.0f, 0.75f, 0.75f, 0.25f ) );
	pDDSPredict.SetFillColor( decColor( 0.0f, 0.75f, 0.75f, 0.1f ) );
	
	UpdateShapes();
}

aeAnimatorLocomotionLeg::~aeAnimatorLocomotionLeg(){
	pDDSPredict.SetParentDebugDrawer( NULL );
	pDDSLockedCF.SetParentDebugDrawer( NULL );
	pDDSLocked.SetParentDebugDrawer( NULL );
}



// Management
///////////////

void aeAnimatorLocomotionLeg::SetName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	pName = name;
}

void aeAnimatorLocomotionLeg::SetPutDownPositionStand( const decVector &position ){
	if( position.IsEqualTo( pPutDownPosStand ) ){
		return;
	}
	
	pPutDownPosStand = position;
	
	pLocomotion->GetAnimator()->NotifyLocomotionChanged();
	pLocomotion->GetAnimator()->SetChanged( true );
}

void aeAnimatorLocomotionLeg::SetPutDownPositionWalk( const decVector &position ){
	if( position.IsEqualTo( pPutDownPosWalk ) ){
		return;
	}
	
	pPutDownPosWalk = position;
	
	pLocomotion->GetAnimator()->NotifyLocomotionChanged();
	pLocomotion->GetAnimator()->SetChanged( true );
}

void aeAnimatorLocomotionLeg::SetPutDownPositionRun( const decVector &position ){
	if( position.IsEqualTo( pPutDownPosRun ) ){
		return;
	}
	
	pPutDownPosRun = position;
	
	pLocomotion->GetAnimator()->NotifyLocomotionChanged();
	pLocomotion->GetAnimator()->SetChanged( true );
}

void aeAnimatorLocomotionLeg::SetLiftOffTime( float time ){
	if( fabsf( time - pLiftOffTime ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pLiftOffTime = time;
	
	pLocomotion->GetAnimator()->NotifyLocomotionChanged();
	pLocomotion->GetAnimator()->SetChanged( true );
}

void aeAnimatorLocomotionLeg::SetPutDownTime( float time ){
	if( fabsf( time - pPutDownTime ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pPutDownTime = time;
	
	pLocomotion->GetAnimator()->NotifyLocomotionChanged();
	pLocomotion->GetAnimator()->SetChanged( true );
}

void aeAnimatorLocomotionLeg::SetVisBoneName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	pVisBoneName = name;
}



void aeAnimatorLocomotionLeg::Reset(){
	const decQuaternion &locoOrientation = pLocomotion->GetOrientationQuaternion();
	const decDVector &locoPosition = pLocomotion->GetPosition();
	decDMatrix locoMatrix( decDMatrix::CreateFromQuaternion( locoOrientation ) * decDMatrix::CreateTranslation( locoPosition ) );
	decDMatrix locoInvMatrix( locoMatrix.Invert() );
	deTouchSensor &touchSensor = *pLocomotion->GetTouchSensor();
	float movedist = 1.0f; //0.5f;
	
	aeCLClosestHit closestHit;
	decDVector castOrigin;
	float distance;
	
	pPredictPosition = locoMatrix * decDVector( pPutDownPosStand );
	
	// project the predict position down to get the ik location
	castOrigin = pPredictPosition + decDVector( 0.0, movedist, 0.0 );
	
	closestHit.Reset();
	touchSensor.RayHits( castOrigin, decVector( 0.0f, -1.0f, 0.0f ), &closestHit );
	
	if( closestHit.GetHasHit() ){
		distance = closestHit.GetHitDistance();
		pPredictPosition += decDVector( 0.0, ( double )( movedist - distance ), 0.0 );
		// this should be blended to avoid flicking motions. the blend factor should
		// be determined using the distance between the new predicted position and
		// the old one. if the distance is small the switch should be done immediatly
		// while with growing distance it should be down to 0.5 or for example
		
		// y is not figured out properly yet. find something better
		pPredictNormal = closestHit.GetHitNormal();
		pPredictRotation.Set( -asinf( pPredictNormal.z ), pLocomotion->GetOrientation().GetValue() * DEG2RAD, asinf( pPredictNormal.x ) );
	}
	
	pLockedPosition = pPredictPosition;
	pLockedRotation = pPredictRotation;
	pLockedNormal = pPredictNormal;
	pPositionLocked = true;
	
	// determine ground plane ( for controllers )
	pGroundPosition = ( locoInvMatrix * pLockedPosition ).ToVector();
	pGroundNormal = locoInvMatrix.TransformNormal( decDVector( pLockedNormal ) ).ToVector();
	
	// determine ik position, rotation and influence ( for controllers )
	pIKInfluence = pPositionLocked ? 1.0f : 0.0f; // maybe blending?
	pIKPosition = ( locoInvMatrix * pLockedPosition ).ToVector();
	pIKOrientation = ( decDMatrix::CreateRotation( pLockedRotation ) * locoInvMatrix ).GetEulerAngles().ToVector();
	
	// update the debug drawer shapes
	RepositionShapes();
}

void aeAnimatorLocomotionLeg::Update( float elapsed ){
	const decQuaternion &locoOrientation = pLocomotion->GetOrientationQuaternion();
	const decDVector &locoPosition = pLocomotion->GetPosition();
	decDMatrix locoMatrix( decDMatrix::CreateFromQuaternion( locoOrientation ) * decDMatrix::CreateTranslation( locoPosition ) );
	decDMatrix locoInvMatrix( locoMatrix.Invert() );
	deTouchSensor &touchSensor = *pLocomotion->GetTouchSensor();
	aeAnimator &animator = *pLocomotion->GetAnimator();
	float movedist = 1.0f; //0.5f;
	
	//deColliderVolume *castCollider = pLocomotion->
	float velocity = pLocomotion->GetMovingSpeed();
	float speedWalk = pLocomotion->GetWalkSpeed();
	float speedRun = pLocomotion->GetRunSpeed();
	aeCLClosestHit closestHit;
	float blendFactor = 0.0f;
	decDVector planeVector;
	decDVector castOrigin;
	bool lockPosition;
	float distance;
	
	float putDownTime = 0.0f;
	float liftOffTime = 0.0f;
	float motionTime = 0.0f;
	
	// determine the current put down position according to speed as well as motion time parameters
	if( velocity < speedWalk ){
		if( speedWalk > 0.001f ){
			blendFactor = velocity / speedWalk;
		}
		
		pPredictPosition = locoMatrix * decDVector( pPutDownPosStand * ( 1.0f - blendFactor ) + pPutDownPosWalk * blendFactor );
		
		putDownTime = pPutDownTime;
		liftOffTime = pLiftOffTime;
		if( animator.GetControllers().GetCount() >= 2 ){
			const aeController &controller = *animator.GetControllers().GetAt( 1 ); // hack
			motionTime = controller.GetCurrentValue();
		}
		
	}else if( velocity < speedRun ){
		if( fabsf( speedRun - speedWalk ) > 0.001f ){
			blendFactor = ( velocity - speedWalk ) / ( speedRun - speedWalk );
		}
		
		pPredictPosition = locoMatrix * decDVector( pPutDownPosWalk * ( 1.0f - blendFactor ) + pPutDownPosRun * blendFactor );
		
		putDownTime = pPutDownTime;
		liftOffTime = pLiftOffTime;
		if( animator.GetControllers().GetCount() >= 2 ){
			const aeController &controller = *animator.GetControllers().GetAt( 1 ); // hack
			motionTime = controller.GetCurrentValue();
		}
		
	}else{
		pPredictPosition.Set( pPutDownPosRun );
		
		putDownTime = pPutDownTime;
		liftOffTime = pLiftOffTime;
		if( animator.GetControllers().GetCount() >= 2 ){
			const aeController &controller = *animator.GetControllers().GetAt( 1 ); // hack
			motionTime = controller.GetCurrentValue();
		}
	}
	
	// project the predict position down to get the ik location
	castOrigin = pPredictPosition + decDVector( 0.0, movedist, 0.0 );
	
	closestHit.Reset();
	touchSensor.RayHits( castOrigin, decVector( 0.0f, -1.0f, 0.0f ), &closestHit );
	
	if( closestHit.GetHasHit() ){
		distance = closestHit.GetHitDistance();
		pPredictPosition += decDVector( 0.0, ( double )( movedist - distance ), 0.0 );
		// this should be blended to avoid flicking motions. the blend factor should
		// be determined using the distance between the new predicted position and
		// the old one. if the distance is small the switch should be done immediatly
		// while with growing distance it should be down to 0.5 or for example
		
		// y is not figured out properly yet. find something better
		pPredictNormal = closestHit.GetHitNormal();
		decVector localNormal( locoInvMatrix.TransformNormal( decDVector( pPredictNormal ) ).ToVector() );
		pPredictRotation.Set( -asinf( localNormal.z ), pLocomotion->GetOrientation().GetValue() * DEG2RAD, asinf( localNormal.x ) );
		//pPredictRotation.Set( -asinf( pPredictNormal.z ), pLocomotion->GetOrientation() * DEG2RAD, asinf( pPredictNormal.x ) );
	}
	
	// depending on the time lock or unlock the locked ik position and orientation
	if( velocity > 0.001f ){
		if( liftOffTime < putDownTime ){
			lockPosition = ( motionTime <= liftOffTime || motionTime >= putDownTime );
			
		}else{
			lockPosition = ( motionTime >= putDownTime && motionTime <= liftOffTime );
		}
		
	}else{
		lockPosition = true;
	}
	
	if( lockPosition && ! pPositionLocked ){
		pLockedPosition = pPredictPosition;
		pLockedRotation = pPredictRotation;
		pLockedNormal = pPredictNormal;
	}
	pPositionLocked = lockPosition;
	
	// determine ground plane ( for controllers )
	if( pPositionLocked ){
		pGroundPosition = ( locoInvMatrix * pLockedPosition ).ToVector();
		pGroundNormal = locoInvMatrix.TransformNormal( decDVector( pLockedNormal ) ).ToVector();
		
	}else{
		planeVector = pPredictPosition - pLockedPosition;
		
		pGroundPosition = ( locoInvMatrix * pPredictPosition ).ToVector();
		
		if( planeVector.Length() > 1e-5 ){
			planeVector.Normalize();
			pGroundNormal = locoInvMatrix.TransformNormal( planeVector % ( decDVector( 0.0, 1.0, 0.0 ) % planeVector ) ).ToVector();
			
		}else{
			pGroundNormal.Set( 0.0f, 1.0f, 0.0f );
		}
	}
	
	// determine ik position, rotation and influence ( for controllers )
	pIKInfluence = pPositionLocked ? 1.0f : 0.0f; // maybe blending?
	pIKPosition = ( locoInvMatrix * pLockedPosition ).ToVector();
	pIKOrientation = ( decDMatrix::CreateRotation( pLockedRotation ) * locoInvMatrix ).GetEulerAngles().ToVector();
	
	// update the debug drawer shapes
	RepositionShapes();
	
	/*
	int legnum;
	for( legnum=0; legnum<4 && pLocomotion->GetLegAt( legnum ) != this; legnum++ );
	if( legnum == 0 )
	printf( "leg %i: gp=(%.3f,%.3f,%.3f) gn=(%.3f,%.3f,%.3f)\n", legnum, pGroundPosition.x, pGroundPosition.y, pGroundPosition.z, pGroundNormal.x, pGroundNormal.y, pGroundNormal.z );
	*/
}

void aeAnimatorLocomotionLeg::PostUpdate(){
#if 0
	const decQuaternion &locoOrientation = pLocomotion->GetOrientationQuaternion();
	const decDVector &locoPosition = pLocomotion->GetPosition();
	decDMatrix locoMatrix( decDMatrix::CreateFromQuaternion( locoOrientation ) * decDMatrix::CreateTranslation( locoPosition ) );
	deComponent *engComponent = pLocomotion->GetAnimator()->GetEngineComponent();
	deTouchSensor &touchSensor = *pLocomotion->GetTouchSensor();
	decDMatrix locoInvMatrix( locoMatrix.Invert() );
	decQuaternion ikBoneOrientation;
	aeCLClosestHit closestHit;
	decDVector ikBonePosition;
	decVector ikBoneRotation;
	decDVector castOrigin;
	decDMatrix boneMatrix;
	deRig *engRig = NULL;
	int boneIndex;
	
	// if there is no rig or component disable the ik
	if( engComponent ){
		engRig = engComponent->GetRig();
	}
	
	if( ! engRig ){
		pIKInfluence = 0.0f;
		return;
	}
	
	// locate the ik bone. if there is none the ik is disabled
	boneIndex = engRig->IndexOfBoneNamed( pVisBoneName.GetString() );
	if( boneIndex == -1 ){
		pIKInfluence = 0.0f;
		return;
	}
	
	// prepare the bones so we have the correct ik bone positions to start with
	engComponent->PrepareBones();
	
	boneMatrix = decDMatrix( engComponent->GetBoneAt( boneIndex ).GetMatrix() ) * locoMatrix;
	ikBonePosition = boneMatrix.GetPosition();
	ikBoneOrientation = boneMatrix.ToQuaternion();
	
	// project the bone downwards to touch the ground. this determines the ik
	// position and normal for the post animator stage
	castOrigin = ikBonePosition + decDVector( 0.0, 0.5, 0.0 );
	
	closestHit.Reset();
	touchSensor.RayHits( aeAnimatorLocomotion::etsaGround, castOrigin, decVector( 0.0f, -1.0f, 0.0f ), &closestHit );
	
	if( ! closestHit.GetHasHit() ){
		pIKInfluence = 0.0f;
		return;
	}
	
	decVector localNormal( locoInvMatrix.TransformNormal( decDVector( closestHit.GetHitNormal() ) ).ToVector() );
	boneMatrix.y -= ( double )closestHit.GetHitDistance() - 0.5;
	ikBoneRotation.Set( boneMatrix.GetEulerAngles().ToVector() );
	
	ikBoneRotation.Set( -asinf( localNormal.z ), pLocomotion->GetBodyOrientation() * DEG2RAD, asinf( localNormal.x ) );
	boneMatrix = decDMatrix::CreateRotation( ikBoneRotation ) * decDMatrix::CreateTranslation( ikBonePosition ) * locoMatrix;
	
	pIKInfluence = 1.0f;
	pIKPosition = ( locoInvMatrix * ikBonePosition ).ToVector();
	pIKOrientation = ( decDMatrix::CreateRotation( ikBoneRotation ) * locoInvMatrix ).GetEulerAngles().ToVector();
	
	// update the debug drawe shapes with the ik parameters
	pDDSLocked.SetPosition( ikBonePosition );
	pDDSLocked.SetOrientation( ikBoneOrientation );
	pDDSLockedCF.SetPosition( ikBonePosition );
	pDDSLockedCF.SetOrientation( ikBoneOrientation );
#endif
}

void aeAnimatorLocomotionLeg::UpdateDebugDrawers(){
	deComponent *engComponent = pLocomotion->GetAnimator()->GetEngineComponent();
	decDMatrix boneMatrix;
	deRig *engRig = NULL;
	int boneIndex;
	
	if( engComponent ){
		engRig = engComponent->GetRig();
	}
	
	if( engRig ){
		engComponent->PrepareBones();
		
		boneIndex = engRig->IndexOfBoneNamed( pVisBoneName.GetString() );
		if( boneIndex != -1 ){
			boneMatrix = decDMatrix( engComponent->GetBoneAt( boneIndex ).GetMatrix() );
			
			pDDSLocked.SetPosition( boneMatrix.GetPosition() );
			pDDSLocked.SetOrientation( boneMatrix.ToQuaternion() );
			pDDSLockedCF.SetPosition( pDDSLocked.GetPosition() );
			pDDSLockedCF.SetOrientation( pDDSLocked.GetOrientation() );
		}
	}
}



void aeAnimatorLocomotionLeg::UpdateShapes(){
	decShape *shape = NULL;
	
	pDDSLocked.RemoveAllShapes();
	pDDSPredict.RemoveAllShapes();
	
	try{
		shape = new decShapeBox( decVector( 0.06f, 0.05f, 0.15f ), decVector( 0.0f, 0.05f, 0.0f ) );
		//shape = new decShapeSphere( 0.05 );
		if( ! shape ) DETHROW( deeOutOfMemory );
		pDDSLocked.AddShape( shape );
		shape = NULL;
		
		shape = new decShapeSphere( 0.05f );
		if( ! shape ) DETHROW( deeOutOfMemory );
		pDDSPredict.AddShape( shape );
		shape = NULL;
		
	}catch( const deException & ){
		if( shape ) delete shape;
		throw;
	}
}

void aeAnimatorLocomotionLeg::RepositionShapes(){
	const decQuaternion &locoOrientation = pLocomotion->GetOrientationQuaternion();
	const decDVector &locoPosition = pLocomotion->GetPosition();
	decDMatrix locoMatrix = decDMatrix::CreateFromQuaternion( locoOrientation ) * decDMatrix::CreateTranslation( locoPosition );
	const decDMatrix invLocoMatrix = locoMatrix.Invert();
	
	pDDSPredict.SetPosition( invLocoMatrix * pPredictPosition );
	
	pDDSLocked.SetPosition( invLocoMatrix * pLockedPosition );
	pDDSLockedCF.SetPosition( invLocoMatrix * pLockedPosition );
	pDDSLockedCF.SetOrientation( ( decDMatrix::CreateRotation( pLockedRotation ) * invLocoMatrix ).ToQuaternion() );
	
	if( pPositionLocked ){
		pDDSLocked.SetEdgeColor( decColor( 0.5f, 0.5f, 0.75f, 0.25f ) );
		pDDSLocked.SetFillColor( decColor( 0.5f, 0.5f, 0.75f, 0.1f ) );
		
	}else{
		pDDSLocked.SetEdgeColor( decColor( 0.0f, 0.5f, 0.75f, 0.25f ) );
		pDDSLocked.SetFillColor( decColor( 0.0f, 0.5f, 0.75f, 0.1f ) );
	}
}



void aeAnimatorLocomotionLeg::SetShapesVisible( bool visible ){
	pDDSLocked.SetVisible( visible );
	pDDSLockedCF.SetVisible( visible );
	//pDDSPredict.SetVisible( visible );
}
