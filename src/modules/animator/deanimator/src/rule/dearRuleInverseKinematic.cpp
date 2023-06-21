/* 
 * Drag[en]gine Animator Module
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
#include <string.h>

#include "dearRuleInverseKinematic.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearIKWorkState.h"
#include "../deDEAnimator.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleInverseKinematic.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRigBone.h>


// Class dearRuleInverseKinematic
///////////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat( "Rule Inverse Kinematic = %iys", ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleInverseKinematic::dearRuleInverseKinematic( dearAnimatorInstance &instance,
const dearAnimator &animator, int firstLink, const deAnimatorRuleInverseKinematic &rule ) :
dearRule( instance, animator, firstLink, rule ),

pInverseKinematic( rule ),

pChain( nullptr ),
pChainCount( 0 ),
pChainLength( 0.0f ),

pTargetGoalPosition( rule.GetTargetGoalPosition(), firstLink ),
pTargetGoalOrientation( rule.GetTargetGoalOrientation(), firstLink ),
pTargetLocalPosition( rule.GetTargetLocalPosition(), firstLink ),
pTargetLocalOrientation( rule.GetTargetLocalOrientation(), firstLink ),
pTargetReachRange( rule.GetTargetReachRange(), firstLink ),
pTargetReachCenter( rule.GetTargetReachCenter(), firstLink ),

pSolverBone( -1 ),
pReachBone( -1 ),

pAdjustPosition( rule.GetAdjustPosition() ),
pAdjustOrientation( rule.GetAdjustOrientation() ),
pUseSolverBone( rule.GetUseSolverBone() ),

pGoalPosition( rule.GetGoalPosition() ),
pGoalOrientation( rule.GetGoalOrientation() ),
pLocalPosition( rule.GetLocalPosition() ),
pLocalOrientation( rule.GetLocalOrientation() ),

pReachRange( rule.GetReachRange() ),
pReachCenter( rule.GetReachCenter() )
{
	RuleChanged();
}

dearRuleInverseKinematic::~dearRuleInverseKinematic(){
	if( pChain ){
		delete [] pChain;
	}
}



// Management
///////////////

void dearRuleInverseKinematic::Apply( dearBoneStateList &stalist, dearVPSStateList& ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() ){
		return;
	}
	
	sParameters params;
	params.blendFactor = GetBlendFactor();
	if( params.blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	const dearAnimatorInstance &instance = GetInstance();
	
	params.goalPosition = pGoalPosition;
	pTargetGoalPosition.GetVector( instance, params.goalPosition );
	
	params.goalOrientation = pGoalOrientation;
	pTargetGoalOrientation.GetQuaternion( instance, params.goalOrientation );
	
	params.localPosition = pLocalPosition;
	pTargetLocalPosition.GetVector( instance, params.localPosition );
	
	params.localOrientation = pLocalOrientation;
	pTargetLocalOrientation.GetQuaternion( instance, params.localOrientation );
	
	params.hasSolverBone = pUseSolverBone && pSolverBone != -1;
	if( params.hasSolverBone ){
		dearBoneState &boneState = *stalist.GetStateAt( pSolverBone );
		boneState.UpdateMatrices();
		
		params.goalPosition = boneState.GetGlobalMatrix() * params.goalPosition;
		params.goalOrientation *= boneState.GetGlobalMatrix().ToQuaternion();
	}
	
	if( pChainCount > 1 ){
		params.reachRange = pReachRange * decMath::clamp(
			pTargetReachRange.GetValue( instance, 1.0f ), 0.0f, 1.0f );
		
		params.reachCenter = pReachCenter;
		pTargetReachCenter.GetVector( instance, params.reachCenter );
		
		pLimitReach( stalist, params.goalPosition, params.reachRange, params.reachCenter );
		
		params.hasIKLimits = false;
		int i;
		for( i=0; i<pChainCount; i++ ){
			params.hasIKLimits |= pChain[ i ].GetHasLimits() || pChain[ i ].GetHasDampening();
		}
		
		// less and things can explode
		params.rotationCrossThreshold = 0.001f;
		
		// 1mm close to target is good enough
		params.goalReachedThreshold = 0.001f;
		
		// if distance to goal is not changing by this threshold the algorithm either
		// reached a singularity or the goal is outside the reach of the bone chain
		params.improvedThreshold = 1e-5f;
		
		// pSolveCCD( stalist, params );
		pSolveFabrik( stalist, params );
		
		pUpdateBonesFromWorkingState( stalist, params );
		
	}else if( pChainCount == 1 ){
		pSolveSingleBone( stalist, params );
	}
DEBUG_PRINT_TIMER;
}

void dearRuleInverseKinematic::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateChain();
	pUpdateChainWeights();
	pUpdateReachBone();
}



// Private functions
//////////////////////

void dearRuleInverseKinematic::pUpdateChain(){
	const dearBoneStateList &stalist = GetInstance().GetBoneStateList();
	const int boneCount = pInverseKinematic.GetListBones().GetCount();
	
	// a valid chain needs 2 bone
	if( boneCount == 2 ){
		const int bone1 = GetBoneMappingFor( 0 );
		const int bone2 = GetBoneMappingFor( 1 );
		
		// a valid chain requires two valid bones which are not identic
		if( bone1 != -1 && bone2 != -1 && bone1 != bone2 ){
			const dearBoneState * const boneState1 = stalist.GetStateAt( bone1 );
			const dearBoneState * const boneState2 = stalist.GetStateAt( bone2 );
			
			// check if a valid chain can be build with bone1 as root
			const dearBoneState *parent = boneState2->GetParentState();
			
			int chainCount = 2;
			while( parent && parent != boneState1 ){
				parent = parent->GetParentState();
				chainCount++;
			}
			
			if( parent == boneState1 ){
				// build chain with bone1 as root
				pSetChainCount( chainCount );
				int chainPosition = chainCount - 1;
				parent = boneState2;
				while( parent != boneState1 ){
					pChain[ chainPosition-- ].SetBoneStateIndex( parent->GetIndex() );
					parent = parent->GetParentState();
				}
				pChain[ 0 ].SetBoneStateIndex( bone1 );
				
			}else{
				// check if a valid chain can be build with bone2 as root
				parent = boneState1->GetParentState();
				chainCount = 2;
				while( parent && parent != boneState2 ){
					parent = parent->GetParentState();
					chainCount++;
				}
				
				// build chain with bone2 as root
				if( parent == boneState2 ){
					pSetChainCount( chainCount );
					int chainPosition = chainCount - 1;
					parent = boneState1;
					while( parent != boneState2 ){
						pChain[ chainPosition-- ].SetBoneStateIndex( parent->GetIndex() );
						parent = parent->GetParentState();
					}
					pChain[ 0 ].SetBoneStateIndex( bone2 );
					
				}else{
					pSetChainCount( 0 );
				}
			}
			
		}else{
			pSetChainCount( 0 );
		}
		
	// a singular chain works with one bone
	}else if( boneCount == 1 ){
		const int bone1 = GetBoneMappingFor( 0 );
		
		if( bone1 != -1 ){
			pSetChainCount( 1 );
			pChain[ 0 ].SetBoneStateIndex( bone1 );
		}
		
	// otherwise we have no valid chain
	}else{
		pSetChainCount( 0 );
	}
	
	// update ik limits for the entire chain
	pInitIKLimits();
	
	// debug
	/*
	module.LOGINFOFORMAT( "IK: Inverse Kinematic Chain:" );
	for( bone1=0; bone1<pChainCount; bone1++ ){
		module.LOGINFOFORMAT( "- %s", pChain[ bone1 ].GetBoneState()->GetRigBoneName() );
	}
	*/
	
	// solver bone
	pSolverBone = GetInstance().GetBoneStateList().IndexOfStateNamed( pInverseKinematic.GetSolverBone() );
}

void dearRuleInverseKinematic::pSetChainCount( int count ){
	if( count == pChainCount ){
		return;
	}
	
	dearIKWorkState *newChain = NULL;
	if( count > 0 ){
		newChain = new dearIKWorkState[ count ];
	}
	
	if( pChain ){
		delete [] pChain;
	}
	
	pChain = newChain;
	pChainCount = count;
}

void dearRuleInverseKinematic::pInitIKLimits(){
	const dearBoneStateList &stateList = GetInstance().GetBoneStateList();
	int i;
	
	for( i=0; i<pChainCount; i++ ){
		const deRigBone * const rigBone = stateList.GetStateAt( pChain[ i ].GetBoneStateIndex() )->GetRigBone();
		
		if( rigBone ){
			const decVector &lowerLimits = rigBone->GetIKLimitsLower();
			const decVector &upperLimits = rigBone->GetIKLimitsUpper();
			const decVector &resistance = rigBone->GetIKResistance();
			decVector dampening( 1.0f, 1.0f, 1.0f );
			bool hasLimits = false;
			
			if( rigBone->GetIKLockedX() ){
				pChain[ i ].SetAxisTypeX( dearIKWorkState::eatLocked );
				hasLimits = true;
				
			}else if( lowerLimits.x < upperLimits.x ){
				pChain[ i ].SetAxisTypeX( dearIKWorkState::eatLimited );
				dampening.x = 1.0f - resistance.x;
				hasLimits = true;
				
			}else{
				pChain[ i ].SetAxisTypeX( dearIKWorkState::eatFree );
				dampening.x = 1.0f - resistance.x;
			}
			
			if( rigBone->GetIKLockedY() ){
				pChain[ i ].SetAxisTypeY( dearIKWorkState::eatLocked );
				hasLimits = true;
				
			}else if( lowerLimits.y < upperLimits.y ){
				pChain[ i ].SetAxisTypeY( dearIKWorkState::eatLimited );
				dampening.y = 1.0f - resistance.y;
				hasLimits = true;
				
			}else{
				pChain[ i ].SetAxisTypeY( dearIKWorkState::eatFree );
				dampening.y = 1.0f - resistance.y;
			}
			
			if( rigBone->GetIKLockedZ() ){
				pChain[ i ].SetAxisTypeZ( dearIKWorkState::eatLocked );
				hasLimits = true;
				
			}else if( lowerLimits.z < upperLimits.z ){
				pChain[ i ].SetAxisTypeZ( dearIKWorkState::eatLimited );
				dampening.z = 1.0f - resistance.z;
				hasLimits = true;
				
			}else{
				pChain[ i ].SetAxisTypeZ( dearIKWorkState::eatFree );
				dampening.z = 1.0f - resistance.z;
			}
			
			if( hasLimits ){
				pChain[ i ].UpdateLimits( *rigBone );
			}
			
			pChain[ i ].SetDampening( dampening );
			pChain[ i ].SetHasLimits( hasLimits );
			
		}else{
			pChain[ i ].SetAxisTypeX( dearIKWorkState::eatFree );
			pChain[ i ].SetAxisTypeY( dearIKWorkState::eatFree );
			pChain[ i ].SetAxisTypeZ( dearIKWorkState::eatFree );
			pChain[ i ].SetHasLimits( false );
			pChain[ i ].SetDampening( decVector( 1.0f, 1.0f, 1.0f ) );
		}
	}
}

void dearRuleInverseKinematic::pUpdateChainWeights(){
	pChainLength = 0.0f;
	
	if( pChainCount == 0 ){
		return;
	}
	
	if( pChainCount == 1 ){
		pChain[ 0 ].SetWeight( 1.0f );
		return;
	}
	
	// calculate chain length. this is the sum of all bone segments from the base to the tip
	decDVector position( pChain[ 0 ].GetGlobalMatrix().GetPosition() );
	int i;
	
	for( i=1; i<pChainCount; i++ ){
		const decDVector nextPosition( pChain[ i ].GetGlobalMatrix().GetPosition() );
		pChainLength += ( nextPosition - position ).Length();
		position = nextPosition;
	}
	
	// calculate weights. weight is calculated for the bone distance along the chain length.
	// distance is the sum of all bone segments up to the bone position
	position = pChain[ 0 ].GetGlobalMatrix().GetPosition();
	float weight, accum = 0.0f;
	
	pChain[ 0 ].SetWeight( 1.0f );
	
	for( i=1; i<pChainCount; i++ ){
		const decDVector nextPosition( pChain[ i ].GetGlobalMatrix().GetPosition() );
		accum += ( nextPosition - position ).Length();
		position = nextPosition;
		
		// weight = decMath::linearStep( ( float )i, 0.0f, ( float )( pChainCount - 1 ), 1.0f, 0.1f );
		weight = decMath::linearStep( accum, 0.0f, pChainLength, 1.0f, 0.1f );
		
		pChain[ i ].SetWeight( weight );
	}
	
	/*
	const float scaleWeight = 1.0f;
	for( i=0; i<pChainCount; i++ ){
		pChain[ i ].SetWeight( pChain[ i ].GetWeight() * scaleWeight );
	}
	*/
}

void dearRuleInverseKinematic::pUpdateReachBone(){
	pReachBone = GetInstance().GetBoneStateList().IndexOfStateNamed( pInverseKinematic.GetReachBone() );
}

void dearRuleInverseKinematic::pLimitReach( dearBoneStateList &stalist, decVector &goalPosition,
float range, const decVector &center ){
	if( range < 1e-4f ){
		return;
	}
	
	decVector reachCenter( center );
	
	if( pReachBone != -1 ){
		dearBoneState &boneState = *stalist.GetStateAt( pReachBone );
		boneState.UpdateMatrices();
		reachCenter = boneState.GetGlobalMatrix() * reachCenter;
	}
	
	const decVector diff( goalPosition - reachCenter );
	const float diffLen = diff.Length();
	
	if( diffLen > range ){
		goalPosition = reachCenter + diff * ( range / diffLen );
	}
}

void dearRuleInverseKinematic::pApplyAdjustOrientation( decMatrix &goalMatrix,
decVector &tipPosition, const decVector &localPosition ){
	if( ! pAdjustOrientation ){
		return;
	}
	
	const decVector position( pChain[ pChainCount - 1 ].GetGlobalMatrix().GetPosition() );
	
	goalMatrix.a14 = position.x;
	goalMatrix.a24 = position.y;
	goalMatrix.a34 = position.z;
	
	pChain[ pChainCount - 1 ].SetGlobalMatrix( goalMatrix );
	
	tipPosition = pChain[ pChainCount - 1 ].GetGlobalMatrix() * localPosition;
}

void dearRuleInverseKinematic::pInitWorkingStates( dearBoneStateList &stalist, const sParameters &params ){
	int i;
	
	for( i=0; i<pChainCount; i++ ){
		dearBoneState &boneState = *stalist.GetStateAt( pChain[ i ].GetBoneStateIndex() );
		boneState.UpdateMatrices();
		pChain[ i ].SetGlobalMatrix( boneState.GetGlobalMatrix() );
		pChain[ i ].SetInverseGlobalMatrix( boneState.GetInverseGlobalMatrix() );
	}
	
	if( params.hasIKLimits ){
		for( i=0; i<pChainCount; i++ ){
			dearBoneState &boneState = *stalist.GetStateAt( pChain[ i ].GetBoneStateIndex() );
			// boneState.UpdateFromGlobalMatrix();
			
			pChain[ i ].SetRigLocalRotation( boneState.GetRigLocalMatrix().ToQuaternion() );
			pChain[ i ].SetLockedRotation( ( boneState.GetOrientation()
				* pChain[ i ].GetLimitZeroQuat() ).GetEulerAngles() );
		}
	}
	
	pChain[ pChainCount - 1 ].SetEndPosition( params.localPosition );
}

void dearRuleInverseKinematic::pUpdateInverseGlobal(){
	int i;
	for( i=0; i<pChainCount; i++ ){
		pChain[ i ].SetInverseGlobalMatrix( pChain[ i ].GetGlobalMatrix().QuickInvert() );
	}
}

void dearRuleInverseKinematic::pInitLength( const decVector &localPosition ){
	int i;
	for( i=0; i<pChainCount-1; i++ ){
		pChain[ i ].SetEndPosition( pChain[ i ].GetInverseGlobalMatrix()
			* pChain[ i + 1 ].GetGlobalMatrix().GetPosition() );
	}
	
	pChain[ pChainCount - 1 ].SetEndPosition( localPosition );
}

void dearRuleInverseKinematic::pSolveSingleBone( dearBoneStateList &stalist, const sParameters &params ){
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	
	// set position and orientation to the bone from the ik settings.
	// the position is only set if there is no parent bone.
	dearBoneState &boneState = *stalist.GetStateAt( pChain[ 0 ].GetBoneStateIndex() );
	decMatrix goalMatrix;
	
	boneState.UpdateMatrices();
	
	if( pAdjustOrientation ){
		goalMatrix.SetWorld( params.goalPosition, params.localOrientation * params.goalOrientation );
		
	}else{
		goalMatrix = boneState.GetGlobalMatrix();
		goalMatrix.a14 = params.goalPosition.x;
		goalMatrix.a24 = params.goalPosition.y;
		goalMatrix.a34 = params.goalPosition.z;
	}
	
	if( boneState.GetParentState() ){
		goalMatrix = goalMatrix.QuickMultiply( boneState.GetParentState()->GetInverseGlobalMatrix() );
	}
	goalMatrix = goalMatrix.QuickMultiply( boneState.GetInverseRigLocalMatrix() );
	
	if( pAdjustOrientation ){
		boneState.BlendWith( goalMatrix * -params.localPosition,
			goalMatrix.ToQuaternion(), blendMode, params.blendFactor, pAdjustPosition, true );
		
	}else{
		boneState.BlendWith( goalMatrix.GetPosition() - params.localPosition,
			decQuaternion(), blendMode, params.blendFactor, pAdjustPosition, false );
	}
}

void dearRuleInverseKinematic::pSolveCCD( dearBoneStateList &stalist, const sParameters &params ){
	// according to paper rotation should not go beyond PI (180 degrees) or problems can happen.
	// clamping here though to a lower value to improve stability with ik limits
	// const float maxRot = DEG2RAD * 60.0f;
	const float maxRot = DEG2RAD * 1.0f;
	
	const int maxStepCount = 500;
	
	decMatrix baseRotMatrix, baseInverseRotMat;
	decMatrix rotationMatrix, matrix;
	decMatrix boneLocalRot, boneLocalRotOrg;
	decVector tipPosition, bonePosition;
	decVector vector, rotationAxis;
	float rotationAngle;
	float tipDistSquared, goalDistSquared;
	int i, j, s;
	
	pInitWorkingStates( stalist, params );
	
	decMatrix goalMatrix( decMatrix::CreateFromQuaternion(
		params.localOrientation * params.goalOrientation ) );
	
	// determine the base matrices
	if( params.hasIKLimits ){
		const dearBoneState &boneState = *stalist.GetStateAt( pChain[ 0 ].GetBoneStateIndex() );
		
		if( boneState.GetParentState() ){
			baseRotMatrix = boneState.GetRigLocalMatrix()
				.QuickMultiplyRotation( boneState.GetParentState()->GetGlobalMatrix() );
			baseInverseRotMat = boneState.GetParentState()->GetInverseGlobalMatrix()
				.QuickMultiplyRotation( boneState.GetInverseRigLocalMatrix() );
			
		}else{
			baseRotMatrix = boneState.GetRigLocalMatrix().GetRotationMatrix();
			baseInverseRotMat = boneState.GetInverseRigLocalMatrix().GetRotationMatrix();
		}
	}
	
	pApplyAdjustOrientation( goalMatrix, tipPosition, params.localPosition );
	
	// determine the squared distance of the goal to the base of the chain. used later on to
	// check for singularity situations
	vector = params.goalPosition - pChain[ 0 ].GetGlobalMatrix().GetPosition();
	goalDistSquared = vector.LengthSquared();
	
	// adjust bones until the we get close enough to the target
	tipPosition = pChain[ pChainCount - 1 ].GetGlobalEnd();
	decVector lastTipPosition( tipPosition );
	
	// NOTE according to papers best is to start first with the bone closest to the target
	// then working the way down to the base. during each rotation constraints can be
	// taken into consideration (clamping for example). in the case of a non-reachable
	// state (singularity) some random rotation can be used to try to get out of the
	// locked situation. such a singularity happens if the end point can not be moved any
	// closer to the desired point during one chain loop and the point is not yet at the
	// end point. this solution is prone to broken limbs due to ik flipping.
	// -> reducing angle after MAX_ROT claming to 10% reduces broken limbs
	// => for( i=pChainCount-1; i>=0; i-- )
	// 
	// using the opposite direction though (from base to tip) reduces the flipping since
	// the chain is preferred to be moved by the base first to reach the goal. this though
	// causes limits to explode the calculation.
	// -> reducing MAX_ROT to 10% is not helping
	// -> reducing angle after MAX_ROT claming to 10% is helping
	// => for( i=0; i<pChainCount; i++ )
	
	for( s=0; s<maxStepCount; s++ ){
		if( params.hasIKLimits ){
			pUpdateInverseGlobal();
		}
		
		// adjust all bones in the chain to approach closer to the target
		// for( i=0; i<pChainCount; i++ ){
		for( i=pChainCount-1; i>=0; i-- ){
			bonePosition = pChain[ i ].GetGlobalMatrix().GetPosition();
			
			if( pAdjustOrientation && i == pChainCount - 1 ){
				pApplyAdjustOrientation( goalMatrix, tipPosition, params.localPosition );
				continue;
			}
			
			if( ! pCalcRotation( params, params.goalPosition, tipPosition, bonePosition,
			rotationAxis, rotationAngle ) ){
				continue;
			}
			
			rotationAngle = decMath::clamp( rotationAngle, -maxRot, maxRot );
			
			rotationAngle *= 0.9f; // damping to improve stability and smooth result
			
			rotationAngle *= pChain[ i ].GetWeight();
			
			rotationMatrix.SetRotationAxis( rotationAxis, -rotationAngle ); // why negation needed?
			
			// retrieve the current rotation and apply limits if the rig bone has any
			if( pChain[ i ].GetHasLimits() || pChain[ i ].GetHasDampening() ){
				// determine the rotation relative to the rig local state
				const decMatrix &globMat = pChain[ i ].GetGlobalMatrix();
				
				if( i == 0 ){
					boneLocalRot = globMat.QuickMultiplyRotation( rotationMatrix )
						.QuickMultiplyRotation( baseInverseRotMat );
					boneLocalRotOrg = globMat.QuickMultiplyRotation( baseInverseRotMat );
					
				}else{
					const decMatrix tempRotMat( pChain[ i - 1 ].GetInverseGlobalMatrix()
						.QuickMultiplyRotation( stalist.GetStateAt(
							pChain[ i ].GetBoneStateIndex() )->GetInverseRigLocalMatrix() ) );
					
					boneLocalRot = globMat.QuickMultiplyRotation( rotationMatrix )
						.QuickMultiplyRotation( tempRotMat );
					boneLocalRotOrg = globMat.QuickMultiplyRotation( tempRotMat );
				}
				
				decQuaternion quatRotated( boneLocalRot.Normalized().ToQuaternion()
					* pChain[ i ].GetLimitZeroQuat() );
				
				if( pChain[ i ].GetHasDampening() ){
					const decQuaternion quatCurrent( boneLocalRotOrg.Normalized().ToQuaternion()
						* pChain[ i ].GetLimitZeroQuat() );
					decVector eulerDiff( ( quatRotated * quatCurrent.Conjugate() ).GetEulerAngles() );
					
					const decVector &dampening = pChain[ i ].GetDampening();
					eulerDiff.x *= dampening.x;
					eulerDiff.y *= dampening.y;
					eulerDiff.z *= dampening.z;
					quatRotated = decQuaternion::CreateFromEuler( eulerDiff ) * quatCurrent;
				}
				
				decVector rotation( quatRotated.GetEulerAngles() );
				
				// apply limits on the rotation where required. includes dampening
				const decVector &lockedRotation = pChain[ i ].GetLockedRotation();
				const decVector &limitsLower = pChain[ i ].GetLimitLower();
				const decVector &limitsUpper = pChain[ i ].GetLimitUpper();
				
				if( pChain[ i ].GetAxisTypeX() == dearIKWorkState::eatLocked ){
					rotation.x = lockedRotation.x;
					
				}else if( pChain[ i ].GetAxisTypeX() == dearIKWorkState::eatLimited ){
					if( rotation.x < limitsLower.x ){
						rotation.x = limitsLower.x;
						
					}else if( rotation.x > limitsUpper.x ){
						rotation.x = limitsUpper.x;
					}
				}
				
				if( pChain[ i ].GetAxisTypeY() == dearIKWorkState::eatLocked ){
					rotation.y = lockedRotation.y;
					
				}else if( pChain[ i ].GetAxisTypeY() == dearIKWorkState::eatLimited ){
					if( rotation.y < limitsLower.y ){
						rotation.y = limitsLower.y;
						
					}else if( rotation.y > limitsUpper.y ){
						rotation.y = limitsUpper.y;
					}
				}
				
				if( pChain[ i ].GetAxisTypeZ() == dearIKWorkState::eatLocked ){
					rotation.z = lockedRotation.z;
					
				}else if( pChain[ i ].GetAxisTypeZ() == dearIKWorkState::eatLimited ){
					if( rotation.z < limitsLower.z ){
						rotation.z = limitsLower.z;
						
					}else if( rotation.z > limitsUpper.z ){
						rotation.z = limitsUpper.z;
					}
				}
				
				quatRotated = decQuaternion::CreateFromEuler( rotation )
					* pChain[ i ].GetLimitZeroQuatInv();
				
				// create new rotation matrix
				if( i == 0 ){
					rotationMatrix = pChain[ i ].GetInverseGlobalMatrix()
						.QuickMultiplyRotation( decMatrix::CreateFromQuaternion( quatRotated ) )
						.QuickMultiplyRotation( baseRotMatrix );
					
				}else{
					rotationMatrix = pChain[ i ].GetInverseGlobalMatrix()
						.QuickMultiplyRotation( decMatrix::CreateFromQuaternion( quatRotated ) )
						.QuickMultiplyRotation( stalist.GetStateAt(
							pChain[ i ].GetBoneStateIndex() )->GetRigLocalMatrix() )
						.QuickMultiplyRotation( pChain[ i - 1 ].GetGlobalMatrix() );
				}
				
				rotationMatrix.Normalize();
			}
			
			// rotate chains from the current link to the tip
			rotationMatrix = decMatrix::CreateTranslation( -bonePosition )
				.QuickMultiply( rotationMatrix )
				.QuickMultiply( decMatrix::CreateTranslation( bonePosition ) );
			
			for( j=i; j<pChainCount; j++ ){
				pChain[ j ].SetGlobalMatrix( pChain[ j ].GetGlobalMatrix().QuickMultiply( rotationMatrix ) );
			}
			
			// calculate the new tip position for the next bone
			tipPosition = pChain[ pChainCount - 1 ].GetGlobalEnd();
		}
		
		pApplyAdjustOrientation( goalMatrix, tipPosition, params.localPosition );
		
		// check if another round is required
		
		// if the target is close enough to the goal a solution has been found
		if( ( tipPosition - params.goalPosition ).Length() < params.goalReachedThreshold ){
			break;
		}
		
		// if the change in distance is too small two situations can happen. either the target is outside of the
		// reach of the chain or the chain ended up in a singularity
		if( ( tipPosition - lastTipPosition ).Length() < params.improvedThreshold ){
			vector = tipPosition - pChain[ 0 ].GetGlobalMatrix().GetPosition();
			tipDistSquared = vector.LengthSquared();
			
			// if the goal is further away from the base of the chain than the tip is then the goal is out of
			// reach and a better solution can not be found. this result is most of the time a fully stretched
			// chain but can be possible less optimal
			if( tipDistSquared < goalDistSquared ){
				break;
			}
			
			// otherwise the chain ended up in a singularity. to break this singularity every bone in the chain
			// is knocked sidewards a little bit. this way the chain should be able to to move again to a useful
			// solution
			break;
		}
		
		lastTipPosition = tipPosition;
	}
}

// #define FABRIK_MODIFIED
// #define FABRIK_MODIFIED_FIX
#define FABRIK_MODIFIED_2

void dearRuleInverseKinematic::pSolveFabrik( dearBoneStateList &stalist, const sParameters &params ){
	const int maxStepCount = 500; //50;
	
	pInitWorkingStates( stalist, params );
	pInitLength( params.localPosition );
	
	decQuaternion baseRot, baseInverseRot;
	if( params.hasIKLimits ){
		const dearBoneState &boneState = *stalist.GetStateAt( pChain[ 0 ].GetBoneStateIndex() );
		
		baseRot = pChain[ 0 ].GetRigLocalRotation();
		if( boneState.GetParentState() ){
			baseRot *= boneState.GetParentState()->GetGlobalMatrix().ToQuaternion();
		}
		baseInverseRot = baseRot.Conjugate();
	}
	
	const decQuaternion goalOrientation( params.localOrientation * params.goalOrientation );
	
	dearIKWorkState &ikwTip = pChain[ pChainCount - 1 ];
	
	const decVector basePosition( pChain[ 0 ].GetGlobalMatrix().GetPosition() );
	decVector lastEnd( ikwTip.GetGlobalEnd() );
	
	decVector targetPosition, targetInBonePosition, rotationAxis, rotationPivot;
	decMatrix boneMatrix, matrixRotation;
	/*const*/ float scale = 1.0f;
	float rotationAngle;
	int i, j, k;
	
	for( i=0; i<maxStepCount; i++ ){
		// if( params.hasIKLimits && i > 0 ){
			// pUpdateInverseGlobal();
		// }
		
		// // scale = decMath::linearStep( ( float )i, 0.0f, ( float )( maxStepCount / 2 - 1 ), 0.25f, 1.0f );
		scale = 1.0f;
		
		// forward stage
		targetPosition = params.goalPosition;
		
		for( j=pChainCount-1; j>=0; j-- ){
			dearIKWorkState &iws = pChain[ j ];
			
			if( pAdjustOrientation && j == pChainCount - 1 ){
				boneMatrix.SetFromQuaternion( goalOrientation );
				
			}else{
				boneMatrix = iws.GetGlobalMatrix();
				decQuaternion rotation;
				
				const bool hasRotation = pCalcRotation( params, targetPosition, iws.GetGlobalEnd(),
					boneMatrix.GetPosition(), rotationAxis, rotationAngle );
				
#ifdef FABRIK_MODIFIED
				if( params.hasIKLimits ){
					iws.SetLastGlobalOrientation( boneMatrix.ToQuaternion() );
				}
				
				if( j < pChainCount - 1 ){
					if( hasRotation ){
						rotation.SetFromAxis( rotationAxis, rotationAngle * scale );
						boneMatrix.SetFromQuaternion( boneMatrix.ToQuaternion() * rotation );
					}
					
					dearIKWorkState &iws2 = pChain[ j + 1 ];
					if( iws2.GetHasDampening() || iws2.GetHasLimits() ){
							// GetModule().LogInfoFormat("lockedRot: %d (%f,%f,%f)", j,
							// 	iws2.GetLockedRotation().x * RAD2DEG, iws2.GetLockedRotation().y * RAD2DEG, iws2.GetLockedRotation().z * RAD2DEG);
						// update the global matrices of this bone. this is required for the
						// calculations below requring the parent bone to be used
						iws.SetGlobalMatrix( boneMatrix );
						iws.SetInverseGlobalMatrix( boneMatrix.QuickInvert() );
						
						// calculate the linked orientation of the child bone
						const decQuaternion globalOrientation( iws2.GetGlobalMatrix().ToQuaternion() );
							// GetModule().LogInfoFormat("globalOrientation: %d (%f,%f,%f)", j,
							// 	globalOrientation.GetEulerAngles().x * RAD2DEG, globalOrientation.GetEulerAngles().y * RAD2DEG, globalOrientation.GetEulerAngles().z * RAD2DEG);
						
						decQuaternion orientation( pGlobalOrientationToBoneOrientation(
							j + 1, globalOrientation, decQuaternion() ) );
								// GetModule().LogInfoFormat("pGlobalOrientationToBoneOrientation: %d (%f,%f,%f)", j,
								// 	orientation.GetEulerAngles().x * RAD2DEG, orientation.GetEulerAngles().y * RAD2DEG, orientation.GetEulerAngles().z * RAD2DEG);
						if( iws2.GetHasDampening() ){
							orientation = pApplyIKResistance( j + 1,
								iws2.GetLastGlobalOrientation(), baseInverseRot, orientation );
						}
						if( iws2.GetHasLimits() ){
							orientation = pApplyIKLimits( j + 1, orientation );
						}
								// GetModule().LogInfoFormat("pApplyIKLimits: %d (%f,%f,%f)", j,
								// 	orientation.GetEulerAngles().x * RAD2DEG, orientation.GetEulerAngles().y * RAD2DEG, orientation.GetEulerAngles().z * RAD2DEG);
						orientation = pBoneOrientationToGlobalOrientation(
							j + 1, decQuaternion(), orientation );
								// GetModule().LogInfoFormat("pBoneOrientationToGlobalOrientation: %d (%f,%f,%f)", j,
								// 	orientation.GetEulerAngles().x * RAD2DEG, orientation.GetEulerAngles().y * RAD2DEG, orientation.GetEulerAngles().z * RAD2DEG);
						
						// the child bone has to stay where it is so apply the rotation to the
						// current bone instead. at this point in time "orientation" is the
						// orientation the child bone would have compared to the current bone
						// orientation and globalOrientation is the orientation the child bone
						// has now. the global rotation in this situation would rotate the child
						// bone to the limited orientation:
						//   rotation = globalOrientation.conjugate * orientation
						// 
						// since this bone has to stay this bone has to rotate instead by the
						// opposite direction which is:
						//   rotation' = rotation.conjugate
						//             = (globalOrientation.conjugate * orientation).conjugate
						//             = orientation.conjugate * globalOrientation
						rotation = orientation.Conjugate() * globalOrientation;
						
						boneMatrix.SetFromQuaternion( boneMatrix.ToQuaternion() * rotation );
							// GetModule().LogInfoFormat("boneMatrixRot: %d (%f,%f,%f)", j,
							// 	boneMatrix.GetEulerAngles().x * RAD2DEG, boneMatrix.GetEulerAngles().y * RAD2DEG, boneMatrix.GetEulerAngles().z * RAD2DEG);
							// GetModule().LogInfoFormat("CHECK: %d [%d,%d] (%f,%f,%f)", j, iws2.GetHasLimits(), iws2.GetHasDampening(),
							// 	rotation.x * RAD2DEG, rotation.y * RAD2DEG, rotation.z * RAD2DEG);
					}
					
				}else if( hasRotation ){
					rotation.SetFromAxis( rotationAxis, rotationAngle * scale );
					boneMatrix.SetFromQuaternion( boneMatrix.ToQuaternion() * rotation );
				}
				
#elif defined FABRIK_MODIFIED_2
				if( params.hasIKLimits ){
					iws.SetLastGlobalOrientation( boneMatrix.ToQuaternion() );
				}
				
				if( j < pChainCount - 1 ){
					if( hasRotation ){
						rotation.SetFromAxis( rotationAxis, rotationAngle * scale );
						boneMatrix.SetFromQuaternion( boneMatrix.ToQuaternion() * rotation );
					}
					
					dearIKWorkState &iws2 = pChain[ j + 1 ];
					if( iws2.GetHasDampening() || iws2.GetHasLimits() ){
						// update the global matrices of this bone. this is required for the
						// calculations below requring the parent bone to be used
						iws.SetGlobalMatrix( boneMatrix );
						iws.SetInverseGlobalMatrix( boneMatrix.QuickInvert() );
						
						// calculate the linked orientation of the child bone
						const decQuaternion globalOrientation( iws2.GetGlobalMatrix().ToQuaternion() );
						
						decQuaternion orientation( pGlobalOrientationToBoneOrientation(
							j + 1, globalOrientation, decQuaternion() ) );
						if( iws2.GetHasDampening() ){
							orientation = pApplyIKResistance( j + 1,
								iws2.GetLastGlobalOrientation(), baseInverseRot, orientation );
						}
						if( iws2.GetHasLimits() ){
							orientation = pApplyIKLimits( j + 1, orientation );
						}
						orientation = pBoneOrientationToGlobalOrientation(
							j + 1, decQuaternion(), orientation );
						
						// according to the original fabrik algorithm the child bone has to stay
						// at the same position and orientation and the current bone has to be
						// rotated and moved. doing this though causes constrainted bones
						// (especially using locks) to explode. the root cause is that adjusting
						// the current bone to satisfy the constraints causes a huge rotation
						// of the entire bone chain up to the base.
						// 
						// the source of the problem is the child bone having turned in an
						// unfavorable way violating the constraint. to solve this issue the child
						// bone has to be rotated to satisfy the constraint.
						matrixRotation = pGlobalRotationMatrix(
							iws2.GetGlobalMatrix().GetPosition(),
							globalOrientation.Conjugate() * orientation ); 
						
						for( k=j+1; k<pChainCount; k++ ){
							pChain[ k ].TransformGlobalMatrix( matrixRotation, false );
						}
						
						// this though will cause the chain end position to move away from the
						// target position it has been pulled towards. to solve this problem the
						// bone chain starting with this bone towards the tip end has to be rotated
						// backwards to make the chain end position to line up again with the
						// target position. the chain end position will point along the vector
						// from the current bone position to the target but at a different
						// distance. this is fine enough since upcoming steps can now improve
						// the result without violating the limits that hard.
						rotationPivot = iws.GetGlobalMatrix().GetPosition();
						
						if( pCalcRotation( params, params.goalPosition, ikwTip.GetGlobalEnd(),
						rotationPivot, rotationAxis, rotationAngle ) ){
							matrixRotation = pGlobalRotationMatrix(
								rotationPivot, rotationAxis, -rotationAngle ); 
							
							for( k=j; k<pChainCount; k++ ){
								pChain[ k ].TransformGlobalMatrix( matrixRotation, true );
							}
						}
						
						// in short this correction enforces the joint restrictions in a way
						// the pose does not rotate away from the good solution.
						// 
						// this is more expensive than regular fabrik but avoids explosion and
						// requires less steps since pose is kept close to good solutions
					}
					
				}else if( hasRotation ){
					rotation.SetFromAxis( rotationAxis, rotationAngle * scale );
					boneMatrix.SetFromQuaternion( boneMatrix.ToQuaternion() * rotation );
				}
				
#else
				if( hasRotation ){
					rotation.SetFromAxis( rotationAxis, rotationAngle );
					if( iws.GetHasDampening() || iws.GetHasLimits() ){
						const decQuaternion globalOrientation( boneMatrix.ToQuaternion() );
						
						decQuaternion orientation( pGlobalOrientationToBoneOrientation(
							j, globalOrientation * rotation, baseInverseRot ) );
						if( iws.GetHasDampening() ){
							orientation = pApplyIKResistance( j,
								globalOrientation, baseInverseRot, orientation );
						}
						if( iws.GetHasLimits() ){
							orientation = pApplyIKLimits( j, orientation );
						}
						orientation = pBoneOrientationToGlobalOrientation( j, baseRot, orientation );
						
						rotation = globalOrientation.Conjugate() * orientation;
					}
				}
				
				boneMatrix.SetFromQuaternion( boneMatrix.ToQuaternion() * rotation );
#endif
			}
			
			const decVector bonePosition( targetPosition - boneMatrix.TransformNormal( iws.GetEndPosition() ) );
			
			boneMatrix.a14 = bonePosition.x;
			boneMatrix.a24 = bonePosition.y;
			boneMatrix.a34 = bonePosition.z;
			
			iws.SetGlobalMatrix( boneMatrix );
			if( params.hasIKLimits ){
				iws.SetInverseGlobalMatrix( boneMatrix.QuickInvert() );
			}
			
			targetPosition = bonePosition;
		}
		
#if defined FABRIK_MODIFIED && defined FABRIK_MODIFIED_FIX
		// if ik limits are used the forward stage can result in a pose causing the backward
		// stage to easily explode. this happens easily with legs and arms if the lower leg
		// respectively lower arm is y and z locked. to prevent this problem rotate the entire
		// chain from the tip bone position to line up with the base. this way the base has
		// a better starting position which should not explode
		if( params.hasIKLimits ){
			const int pivotIndex = pAdjustOrientation ? pChainCount - 2 : pChainCount - 1;
			
			const decVector pivot( pChain[ pivotIndex ].GetGlobalMatrix().GetPosition() );
			const decVector tip( pChain[ 0 ].GetGlobalMatrix().GetPosition() );
			
			const bool hasRotation = pCalcRotation( params, basePosition, tip, pivot, rotationAxis, rotationAngle );
			if( hasRotation ){
				const decMatrix rotation( decMatrix::CreateTranslation( -pivot ).
					QuickMultiply( decMatrix::CreateRotationAxis( rotationAxis, -rotationAngle ) ).
					QuickMultiply( decMatrix::CreateTranslation( pivot ) ) );
				
				for( j=0; j<pivotIndex; j++ ){
					pChain[ j ].SetGlobalMatrix( pChain[ j ].GetGlobalMatrix().QuickMultiply( rotation ) );
					pChain[ j ].SetInverseGlobalMatrix( pChain[ j ].GetGlobalMatrix().QuickInvert() );
				}
			}
		}
#endif
		
		// backward stage
		targetPosition = basePosition;
		
		for( j=0; j<pChainCount; j++ ){
			dearIKWorkState &iws = pChain[ j ];
			
			if( pAdjustOrientation && j == pChainCount - 1 ){
				boneMatrix.SetFromQuaternion( goalOrientation );
				
			}else{
				boneMatrix = iws.GetGlobalMatrix();
				
				decQuaternion rotation;
				if( pCalcRotation( params, targetPosition, boneMatrix.GetPosition(),
				iws.GetGlobalEnd(), rotationAxis, rotationAngle ) ){
					rotation.SetFromAxis( rotationAxis, rotationAngle * scale );
					
					if( iws.GetHasDampening() || iws.GetHasLimits() ){
						const decQuaternion globalOrientation( boneMatrix.ToQuaternion() );
						
						decQuaternion orientation( pGlobalOrientationToBoneOrientation(
							j, globalOrientation * rotation, baseInverseRot ) );
						if( iws.GetHasDampening() ){
							orientation = pApplyIKResistance( j,
								globalOrientation, baseInverseRot, orientation );
						}
						if( iws.GetHasLimits() ){
							orientation = pApplyIKLimits( j, orientation );
						}
						orientation = pBoneOrientationToGlobalOrientation( j, baseRot, orientation );
						
						rotation = globalOrientation.Conjugate() * orientation;
					}
				}
				
				boneMatrix.SetFromQuaternion( boneMatrix.ToQuaternion() * rotation );
			}
			
			boneMatrix.a14 = targetPosition.x;
			boneMatrix.a24 = targetPosition.y;
			boneMatrix.a34 = targetPosition.z;
			
			iws.SetGlobalMatrix( boneMatrix );
			if( params.hasIKLimits ){
				iws.SetInverseGlobalMatrix( boneMatrix.QuickInvert() );
			}
			
			targetPosition = iws.GetGlobalEnd();
		}
		
		// finish step
		const decVector end( pChain[ pChainCount - 1 ].GetGlobalEnd() );
		
		//if( end.IsEqualTo( params.goalPosition, params.goalReachedThreshold ) ){
		if( ( end - params.goalPosition ).Length() < params.goalReachedThreshold ){
			break;
		}
		
		//if( end.IsEqualTo( lastEnd, params.improvedThreshold ) ){
		if( ( end - lastEnd ).Length() < params.improvedThreshold ){
			// current solution is not any better than the last one. this typically happens
			// if the goal position is beyond the reach of the bone chain. in the case of
			// FABRIK algorithm stop here since we can not get any better no matter how
			break;
		}
		
		lastEnd = end;
	}
}

void dearRuleInverseKinematic::pUpdateBonesFromWorkingState(
dearBoneStateList &stalist, const sParameters &params ){
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	decMatrix matrix;
	int i;
	
	for( i=0; i<pChainCount; i++ ){
		const decMatrix &globalMatrix = pChain[ i ].GetGlobalMatrix();
		
		dearBoneState &boneState = *stalist.GetStateAt( pChain[ i ].GetBoneStateIndex() );
		pChain[ i ].SetInverseGlobalMatrix( globalMatrix.QuickInvert() );
		
		if( i == 0 ){
			if( boneState.GetParentState() ){
				matrix = globalMatrix
					.QuickMultiply( boneState.GetParentState()->GetInverseGlobalMatrix() )
					.QuickMultiply( boneState.GetInverseRigLocalMatrix() );
				
			}else{
				matrix = globalMatrix.QuickMultiply( boneState.GetInverseRigLocalMatrix() );
			}
			
		}else{
			matrix = globalMatrix
				.QuickMultiply( pChain[ i - 1 ].GetInverseGlobalMatrix() )
				.QuickMultiply( boneState.GetInverseRigLocalMatrix() );
		}
		
							// GetModule().LogInfoFormat("final: %d (%f,%f,%f)", i,
							// 	matrix.GetEulerAngles().x * RAD2DEG, matrix.GetEulerAngles().y * RAD2DEG, matrix.GetEulerAngles().z * RAD2DEG);
		boneState.BlendWith( matrix.GetPosition(), matrix.ToQuaternion(),
			blendMode, params.blendFactor, true, true );
	}
}

bool dearRuleInverseKinematic::pCalcRotation( const sParameters &params,
const decVector &goalPosition, const decVector &tipPosition,
const decVector &bonePosition, decVector &rotationAxis, float &rotationAngle ){
	const decVector tipVector( tipPosition - bonePosition );
	const float tipVectorLen = tipVector.Length();
	if( tipVectorLen < FLOAT_SAFE_EPSILON ){
		return false;
	}
	
	const decVector targetVector( goalPosition - bonePosition );
	const float targetVectorLen = targetVector.Length();
	if( targetVectorLen < FLOAT_SAFE_EPSILON ){
		return false;
	}
	
	const decVector tipVectorNor( tipVector / tipVectorLen );
	const decVector targetVectorNor( targetVector / targetVectorLen );
	
	rotationAxis = tipVectorNor % targetVectorNor;
	if( rotationAxis.IsZero( params.rotationCrossThreshold ) ){
		return false;
	}
	
	rotationAngle = acosf( tipVectorNor * targetVectorNor );
	return true;
}

decMatrix dearRuleInverseKinematic::pGlobalRotationMatrix( const decVector &pivot,
const decQuaternion &rotation ) const{
	return decMatrix::CreateTranslation( -pivot ).
		QuickMultiply( decMatrix::CreateFromQuaternion( rotation ) ).
		QuickMultiply( decMatrix::CreateTranslation( pivot ) );
}

decMatrix dearRuleInverseKinematic::pGlobalRotationMatrix( const decVector &pivot,
const decVector &axis, float angle ) const{
	return decMatrix::CreateTranslation( -pivot ).
		QuickMultiply( decMatrix::CreateRotationAxis( axis, -angle ) ).
		QuickMultiply( decMatrix::CreateTranslation( pivot ) ); 
}

decQuaternion dearRuleInverseKinematic::pGlobalOrientationToBoneOrientation( int index,
const decQuaternion &globalOrientation, const decQuaternion &baseInverseRotation ) const{
	if( index == 0 ){
		return globalOrientation * baseInverseRotation;
		
	}else{
		return globalOrientation * pChain[ index - 1 ].GetInverseGlobalMatrix().ToQuaternion()
			* pChain[ index ].GetInverseRigLocalRotation();
	}
}

decQuaternion dearRuleInverseKinematic::pApplyIKResistance( int index,
const decQuaternion &globalOrientation, const decQuaternion &baseInverseRotation,
const decQuaternion &rotation ) const{
	const decQuaternion orgRotation( pGlobalOrientationToBoneOrientation(
		index, globalOrientation, baseInverseRotation ) );
	
	decVector eulerDiff( ( rotation * orgRotation.Conjugate() ).GetEulerAngles() );
	const decVector &dampening = pChain[ index ].GetDampening();
	
	eulerDiff.x *= dampening.x;
	eulerDiff.y *= dampening.y;
	eulerDiff.z *= dampening.z;
	return decQuaternion::CreateFromEuler( eulerDiff ) * orgRotation;
}

decQuaternion dearRuleInverseKinematic::pApplyIKLimits( int index, const decQuaternion &orientation ) const{
	const dearIKWorkState &ikws = pChain[ index ];
	decVector euler( ( orientation * ikws.GetLimitZeroQuat() ).GetEulerAngles() );
	
	const decVector &lockedRotation = ikws.GetLockedRotation();
	const decVector &limitsLower = ikws.GetLimitLower();
	const decVector &limitsUpper = ikws.GetLimitUpper();
	
	switch( ikws.GetAxisTypeX() ){
	case dearIKWorkState::eatLocked:
		euler.x = lockedRotation.x;
		break;
		
	case dearIKWorkState::eatLimited:
		euler.x = decMath::clamp( euler.x, limitsLower.x, limitsUpper.x );
		break;
		
	default:
		break;
	}
	
	switch( ikws.GetAxisTypeY() ){
	case dearIKWorkState::eatLocked:
						// GetModule().LogInfoFormat("LIMIT: (%f,%f,%f) (%f,%f,%f)",
						// 	euler.x * RAD2DEG, euler.y * RAD2DEG, euler.z * RAD2DEG,
						// 	lockedRotation.x * RAD2DEG, lockedRotation.y * RAD2DEG, lockedRotation.z * RAD2DEG );
		euler.y = lockedRotation.y;
		break;
		
	case dearIKWorkState::eatLimited:
		euler.y = decMath::clamp( euler.y, limitsLower.y, limitsUpper.y );
		break;
		
	default:
		break;
	}
	
	switch( ikws.GetAxisTypeZ() ){
	case dearIKWorkState::eatLocked:
		euler.z = lockedRotation.z;
		break;
		
	case dearIKWorkState::eatLimited:
		euler.z = decMath::clamp( euler.z, limitsLower.z, limitsUpper.z );
		break;
		
	default:
		break;
	}
	
	return decQuaternion::CreateFromEuler( euler ) * ikws.GetLimitZeroQuatInv();
}

decQuaternion dearRuleInverseKinematic::pBoneOrientationToGlobalOrientation( int index,
const decQuaternion &baseRotation, const decQuaternion &orientation ) const{
	if( index == 0 ){
		return orientation * baseRotation;
		
	}else{
		return orientation * pChain[ index ].GetRigLocalRotation()
			* pChain[ index - 1 ].GetGlobalMatrix().ToQuaternion();
	}
}

decQuaternion dearRuleInverseKinematic::pApplyIKRestrictions( int index,
const decMatrix &globalMatrix, const decQuaternion &baseRotation,
const decQuaternion &baseInverseRotation, const decQuaternion &rotation ) const{
	decQuaternion boneLocalRot, boneLocalRotOrg, parentRot;
	const decQuaternion globRot( globalMatrix.ToQuaternion() );
	const dearIKWorkState &ikws = pChain[ index ];
	
	// calculate rotated and non-rotated orientation relative to rig local bone coordinate system
	if( index == 0 ){
		boneLocalRot = globRot * rotation * baseInverseRotation;
		boneLocalRotOrg = globRot * baseInverseRotation;
		
	}else{
		parentRot = ikws.GetRigLocalRotation() * pChain[ index - 1 ].GetGlobalMatrix().ToQuaternion();
		const decQuaternion invParentRot( parentRot.Conjugate() );
		
		boneLocalRot = globRot * rotation * invParentRot;
		boneLocalRotOrg = globRot * invParentRot;
	}
	
	// apply damping. this has to be done on the unclamped rig local orientation
	if( ikws.GetHasDampening() ){
		decVector eulerDiff( ( boneLocalRot * boneLocalRotOrg.Conjugate() ).GetEulerAngles() );
		const decVector &dampening = ikws.GetDampening();
		
		eulerDiff.x *= dampening.x;
		eulerDiff.y *= dampening.y;
		eulerDiff.z *= dampening.z;
		boneLocalRot = decQuaternion::CreateFromEuler( eulerDiff ) * boneLocalRotOrg;
	}
	
	// apply clamping
	if( ikws.GetHasLimits() ){
		decQuaternion rotated( boneLocalRot * ikws.GetLimitZeroQuat() );
		decVector eulerRotation( rotated.GetEulerAngles() );
		
		// apply limits on the rotation where required. includes dampening
		const decVector &lockedRotation = ikws.GetLockedRotation();
		const decVector &limitsLower = ikws.GetLimitLower();
		const decVector &limitsUpper = ikws.GetLimitUpper();
		
		switch( ikws.GetAxisTypeX() ){
		case dearIKWorkState::eatLocked:
			eulerRotation.x = lockedRotation.x;
			break;
			
		case dearIKWorkState::eatLimited:
			eulerRotation.x = decMath::clamp( eulerRotation.x, limitsLower.x, limitsUpper.x );
			break;
			
		default:
			break;
		}
		
		switch( ikws.GetAxisTypeY() ){
		case dearIKWorkState::eatLocked:
			eulerRotation.y = lockedRotation.y;
			break;
			
		case dearIKWorkState::eatLimited:
			eulerRotation.y = decMath::clamp( eulerRotation.y, limitsLower.y, limitsUpper.y );
			break;
			
		default:
			break;
		}
		
		switch( ikws.GetAxisTypeZ() ){
		case dearIKWorkState::eatLocked:
			eulerRotation.z = lockedRotation.z;
			break;
			
		case dearIKWorkState::eatLimited:
			eulerRotation.z = decMath::clamp( eulerRotation.z, limitsLower.z, limitsUpper.z );
			break;
			
		default:
			break;
		}
		
		boneLocalRot = decQuaternion::CreateFromEuler( eulerRotation ) * ikws.GetLimitZeroQuatInv();
	}
	
	// calculate new rotation
	if( index == 0 ){
		return globRot.Conjugate() * boneLocalRot * baseRotation;
		
	}else{
		return globRot.Conjugate() * boneLocalRot * parentRot;
	}
}
