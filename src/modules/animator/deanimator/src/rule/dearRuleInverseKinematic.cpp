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
			params.hasIKLimits |= pChain[ i ].GetHasLimits();
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
			pChain[ i ].SetHasLimits( hasLimits || pChain[ i ].HasDampening() );
			
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
			//boneState.UpdateFromGlobalMatrix();
			
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
			if( pChain[ i ].GetHasLimits() ){
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
				
				if( pChain[ i ].HasDampening() ){
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

void dearRuleInverseKinematic::pSolveFabrik( dearBoneStateList &stalist, const sParameters &params ){
	const int maxStepCount = 50;
	
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
	
	const decVector basePosition( pChain[ 0 ].GetGlobalMatrix().GetPosition() );
	decVector lastEnd( pChain[ pChainCount - 1 ].GetGlobalEnd() );
	
	decVector targetPosition, targetInBonePosition, rotationAxis;
	decMatrix boneMatrix;
	float rotationAngle;
	int i, j;
	
	for( i=0; i<maxStepCount; i++ ){
		if( params.hasIKLimits && i > 0 ){
			pUpdateInverseGlobal();
		}
		
		// forward stage
		targetPosition = params.goalPosition;
		
		for( j=pChainCount-1; j>=0; j-- ){
// 			const decMatrix &invMatGlobal = pChain[ j ].GetInverseGlobalMatrix();
// 			targetInBonePosition = invMatGlobal * targetPosition;
			
			if( pAdjustOrientation && j == pChainCount - 1 ){
				boneMatrix.SetFromQuaternion( goalOrientation );
				
			}else{
				boneMatrix = pChain[ j ].GetGlobalMatrix();
				
				decQuaternion rotation;
				if( pCalcRotation( params, targetPosition, pChain[ j ].GetGlobalEnd(),
				boneMatrix.GetPosition(), rotationAxis, rotationAngle ) ){
					rotation.SetFromAxis( rotationAxis, rotationAngle );
					
					if( pChain[ j ].GetHasLimits() ){
						rotation = pLimitRotation( j, boneMatrix, baseRot, baseInverseRot, rotation );
					}
				}
				
				boneMatrix.SetFromQuaternion( boneMatrix.ToQuaternion() * rotation );
			}
			
			const decVector bonePosition( targetPosition
				- boneMatrix.TransformNormal( pChain[ j ].GetEndPosition() ) );
			
			boneMatrix.a14 = bonePosition.x;
			boneMatrix.a24 = bonePosition.y;
			boneMatrix.a34 = bonePosition.z;
			pChain[ j ].SetGlobalMatrix( boneMatrix );
			
			targetPosition = bonePosition;
		}
		
		// backward stage
		targetPosition = basePosition;
		
		for( j=0; j<pChainCount; j++ ){
			if( pAdjustOrientation && j == pChainCount - 1 ){
				boneMatrix.SetFromQuaternion( goalOrientation );
				
			}else{
				boneMatrix = pChain[ j ].GetGlobalMatrix();
				
				decQuaternion rotation;
				if( pCalcRotation( params, targetPosition, boneMatrix.GetPosition(),
				pChain[ j ].GetGlobalEnd(), rotationAxis, rotationAngle ) ){
					rotation.SetFromAxis( rotationAxis, rotationAngle );
					
					if( pChain[ j ].GetHasLimits() ){
						rotation = pLimitRotation( j, boneMatrix, baseRot, baseInverseRot, rotation );
					}
				}
				
				boneMatrix.SetFromQuaternion( boneMatrix.ToQuaternion() * rotation );
			}
			
			boneMatrix.a14 = targetPosition.x;
			boneMatrix.a24 = targetPosition.y;
			boneMatrix.a34 = targetPosition.z;
			pChain[ j ].SetGlobalMatrix( boneMatrix );
			
			targetPosition = pChain[ j ].GetGlobalEnd();
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

decQuaternion dearRuleInverseKinematic::pLimitRotation( int index, const decMatrix &globalMatrix,
const decQuaternion &baseRotation, const decQuaternion &baseInverseRotation,
const decQuaternion &rotation ){
	const dearIKWorkState &ikws = pChain[ index ];
	
	const decQuaternion globRot( globalMatrix.ToQuaternion() );
	decQuaternion boneLocalRot, boneLocalRotOrg;
	
	if( index == 0 ){
		boneLocalRot = globRot * rotation * baseInverseRotation;
		boneLocalRotOrg = globRot * baseInverseRotation;
		
	}else{
		const decQuaternion tempRot(
			pChain[ index - 1 ].GetInverseGlobalMatrix().ToQuaternion()
			* ikws.GetInverseRigLocalRotation() );
		
		boneLocalRot = globRot * rotation * tempRot;
		boneLocalRotOrg = globRot * tempRot;
	}
	
	decQuaternion rotated( boneLocalRot * ikws.GetLimitZeroQuat() );
	
	if( ikws.HasDampening() ){
		const decQuaternion current( boneLocalRotOrg * ikws.GetLimitZeroQuat() );
		decVector eulerDiff( ( rotated * current.Conjugate() ).GetEulerAngles() );
		
		const decVector &dampening = ikws.GetDampening();
		eulerDiff.x *= dampening.x;
		eulerDiff.y *= dampening.y;
		eulerDiff.z *= dampening.z;
		rotated = decQuaternion::CreateFromEuler( eulerDiff ) * current;
	}
	
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
	
	rotated = decQuaternion::CreateFromEuler( eulerRotation )
		* ikws.GetLimitZeroQuatInv();
	
	// create new rotation quaternion
	if( index == 0 ){
		return ikws.GetInverseGlobalMatrix().ToQuaternion() * rotated * baseRotation;
		
	}else{
		return ikws.GetInverseGlobalMatrix().ToQuaternion() * rotated
			* ikws.GetRigLocalRotation()
			* pChain[ index - 1 ].GetGlobalMatrix().ToQuaternion();
	}
}
