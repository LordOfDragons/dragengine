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



// Definitions
////////////////

// MAX_ROT = sinf( DEG2RAD * 1.0 )
#define MAX_ROT	1.74532836e-2f



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
int firstLink, const deAnimatorRuleInverseKinematic &rule ) :
dearRule( instance, firstLink, rule ),

pInverseKinematic( rule ),

pChain( NULL ),
pChainCount( 0 ),
pSolverBone( -1 ),

pTargetGoalPosition( rule.GetTargetGoalPosition(), firstLink ),
pTargetGoalOrientation( rule.GetTargetGoalOrientation(), firstLink ),
pTargetLocalPosition( rule.GetTargetLocalPosition(), firstLink ),
pTargetLocalOrientation( rule.GetTargetLocalOrientation(), firstLink ),

pTargetReachRange( rule.GetTargetReachRange(), firstLink ),
pTargetReachCenter( rule.GetTargetReachCenter(), firstLink ),

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

void dearRuleInverseKinematic::Apply( dearBoneStateList &stalist ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() ){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if( blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	// controller affected values
	const dearAnimatorInstance &instance = GetInstance();
	
	decVector constGoalPosition( pGoalPosition );
	pTargetGoalPosition.GetVector( instance, constGoalPosition );
	
	decQuaternion constGoalOrientation( pGoalOrientation );
	pTargetGoalOrientation.GetQuaternion( instance, constGoalOrientation );
	
	decVector constLocalPosition( pLocalPosition );
	pTargetLocalPosition.GetVector( instance, constLocalPosition );
	
	decQuaternion constLocalOrientation( pLocalOrientation );
	pTargetLocalOrientation.GetQuaternion( instance, constLocalOrientation );
	
	const float reachRange = pReachRange * decMath::clamp( pTargetReachRange.GetValue( instance, 1.0f ), 0.0f, 1.0f );
	decVector reachCenter( pReachCenter );
	pTargetReachCenter.GetVector( instance, reachCenter );
	
	// if we have a full chain
	if( pChainCount > 1 ){
		//deDEAnimator &module = GetModule();
		bool hasSolverBone = ( pUseSolverBone && pSolverBone != -1 );
		decMatrix baseRotMatrix, baseInverseRotMat;
		const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
		decVector goalPosition( constGoalPosition );
		decVector localPosition( constLocalPosition );
		decMatrix goalMatrix, rotationMatrix, matrix;
		decMatrix boneLocalRot, boneLocalRotOrg;
		decVector tipPosition, bonePosition;
		decVector planeNormal, targetNormal, gradient;
		decVector tipVector, vector, rotation;
		float curDist, oldDist, angle;
		//float dot, oneTan = tanf( DEG2RAD );
		//float correctionFactor = 0.2f;
		float planeNormalLen;
		float tipDistSquared, goalDistSquared;
		int s, maxStepCount = 500;
		bool hasIKLimits = false;
		int i, j;
		
		/*
		float targetNormalLen;
		float rotationDistance;
		decVector DEBUG_tipposition1[ 10 ];
		decVector DEBUG_bonepos[ 10 ];
		decVector DEBUG_tipvector[ 10 ];
		decVector DEBUG_targetnormal[ 10 ];
		decVector DEBUG_planenormal[ 10 ];
		float DEBUG_rotationdist[ 10 ];
		float DEBUG_targetnormallen[ 10 ];
		float DEBUG_angle[ 10 ];
		decVector DEBUG_tipposition2[ 10 ];
		*/
		
		// calculate the target values. this depends on the presence of a solver
		// bone as well as if the orientation has to be adjusted
		if( hasSolverBone ){
			dearBoneState &boneState = *stalist.GetStateAt( pSolverBone );
			boneState.UpdateMatrices();
			
			goalPosition = boneState.GetGlobalMatrix() * goalPosition;
		}
		
		if( pAdjustOrientation ){
			if( hasSolverBone ){
				goalMatrix = decMatrix::CreateFromQuaternion( constLocalOrientation )
					.QuickMultiply( decMatrix::CreateFromQuaternion( constGoalOrientation ) )
					.QuickMultiply( stalist.GetStateAt( pSolverBone )->GetGlobalMatrix().GetRotationMatrix() );
				
			}else{
				goalMatrix = decMatrix::CreateFromQuaternion( constLocalOrientation )
					.QuickMultiply( decMatrix::CreateFromQuaternion( constGoalOrientation ) );
			}
		}
		
		// limit reach if enabled
		pLimitReach( stalist, goalPosition, reachRange, reachCenter );
		
		// determine if there are any ik limits. used to avoid calculating inverse global matrices
		for( i=0; i<pChainCount; i++ ){
			hasIKLimits |= pChain[ i ].GetHasLimits();
		}
		
		// copy the bone state to our work state
		for( i=0; i<pChainCount; i++ ){
			dearBoneState &boneState = *stalist.GetStateAt( pChain[ i ].GetBoneStateIndex() );
			boneState.UpdateMatrices();
			pChain[ i ].SetGlobalMatrix( boneState.GetGlobalMatrix() );
		}
		
		// store the reference rotation if required
		if( hasIKLimits ){
			for( i=0; i<pChainCount; i++ ){
				dearBoneState &boneState = *stalist.GetStateAt( pChain[ i ].GetBoneStateIndex() );
				boneState.UpdateFromGlobalMatrix();
				
				pChain[ i ].SetLockedRotation( ( boneState.GetOrientation()
					* pChain[ i ].GetLimitZeroQuat() ).GetEulerAngles() );
			}
		}
		
		// determine the base matrices
		if( hasIKLimits ){
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
		
		// determine the squared distance of the goal to the base of the chain. used later on to
		// check for singularity situations
		vector = goalPosition - pChain[ 0 ].GetGlobalMatrix().GetPosition();
		goalDistSquared = vector.LengthSquared();
		
		// adjust bones until the we get close enough to the target
		tipPosition = pChain[ pChainCount - 1 ].GetGlobalMatrix() * localPosition;
		curDist = ( tipPosition - goalPosition ).Length();
		
		for( s=0; s<maxStepCount; s++ ){
			// NOTE according to papers best is to start first with the bone closest to the target
			// then working the way down to the base. during each rotation constraints can be
			// taken into consideration (clamping for example). in the case of a non-reachable
			// state (singularity) some random rotation can be used to try to get out of the
			// locked situation. such a singularity happens if the end point can not be moved any
			// closer to the desired point during one chain loop and the point is not yet at the
			// end point.
			
			// update inverse global matrices
			if( hasIKLimits ){
				for( i=0; i<pChainCount; i++ ){
					pChain[ i ].SetInverseGlobalMatrix( pChain[ i ].GetGlobalMatrix().QuickInvert() );
				}
			}
			
			// adjust all bones in the chain to approach closer to the target
			//for( i=0; i<pChainCount; i++ ){
			for( i=pChainCount-1; i>=0; i-- ){
				bonePosition = pChain[ i ].GetGlobalMatrix().GetPosition();
				
				// determine tip vector and length. if the length is close to 0 the bone in question
				// is skipped since a 0 length bone corrupts the ik solving calculations
				tipVector = tipPosition - bonePosition;
				if( tipVector.IsZero( 1e-5f ) ){
					continue;
				}
				
				// calculate the optimal plane to bring the tip closer to the target
				targetNormal = goalPosition - tipPosition;
				if( targetNormal.IsZero( 1e-5f ) ){
					continue;
				}
				
				planeNormal = tipVector % targetNormal;
				planeNormalLen = planeNormal.Length();
				if( planeNormalLen < 1e-5f ){
					continue;
				}
				planeNormal /= planeNormalLen;
				
				// determine the amount of rotation to apply. for small rotation angles the distance between
				// the tip and the goal roughly equals the sine. for example if the tipVector is 1m long then
				// 1 degree of rotation results in a displacement of roughly 1.7cm along the gradient normal.
				// therefore the distance between the tip and the goal can be used to determine how large the
				// rotation has to be. if the distance is larger than 1 degree of rotation the full rotation
				// is applied. if the distance is less the rotation is scaled down to not overshoot.
				
				// for small rotations the gradient vector is a good approximization for the rotation path
				// along the rotation plane. the projection of the target normal onto the gradient yields
				// therefore roughly the amount of rotation required to move the tip onto the same line as
				// the goal is located on. the tip is though still usually farther away or close than the
				// goal position. the maximal rotation is clipped so the approximation of the rotation using
				// the gradient is valid.
			//	gradient = planeNormal % tipVector;
				gradient = tipVector % planeNormal;
				const float gradientLen = gradient.Length();
				if( gradientLen > 1e-5f ){
					angle = targetNormal * ( gradient / gradientLen );
					
				}else{
					angle = 0.0f;
				}
				
				if( angle > MAX_ROT ){
					angle = MAX_ROT;
				}
				// angle *= 0.95f; // slight reduction of the rotation to avoid jittering
				
				rotationMatrix.SetRotationAxis( planeNormal, angle );
				
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
					
					decQuaternion quatRotated( boneLocalRot.ToQuaternion() * pChain[ i ].GetLimitZeroQuat() );
					
					if( pChain[ i ].HasDampening() ){
						const decQuaternion quatCurrent( boneLocalRotOrg.ToQuaternion() * pChain[ i ].GetLimitZeroQuat() );
						decVector eulerDiff( ( quatRotated * quatCurrent.Conjugate() ).GetEulerAngles() );
						
						const decVector &dampening = pChain[ i ].GetDampening();
						eulerDiff.x *= dampening.x;
						eulerDiff.y *= dampening.y;
						eulerDiff.z *= dampening.z;
						quatRotated = decQuaternion::CreateFromEuler( eulerDiff ) * quatCurrent;
					}
					
					rotation = quatRotated.GetEulerAngles();
					
					// apply limits on the rotation where required. includes dampening
// 					const deRigBone &rigBone = *stalist.GetStateAt( pChain[ i ].GetBoneStateIndex() )->GetRigBone();
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
					
					quatRotated = decQuaternion::CreateFromEuler( rotation ) * pChain[ i ].GetLimitZeroQuatInv();
					
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
				}
				
				// rotate chains from the current link to the tip
				rotationMatrix = decMatrix::CreateTranslation( -bonePosition )
					.QuickMultiply( rotationMatrix )
					.QuickMultiply( decMatrix::CreateTranslation( bonePosition ) );
				
				for( j=i; j<pChainCount; j++ ){
					pChain[ j ].SetGlobalMatrix( pChain[ j ].GetGlobalMatrix().QuickMultiply( rotationMatrix ) );
				}
				
/*
DEBUG_tipposition1[ i ] = tipPosition;
DEBUG_bonepos[ i ] = bonePosition;
DEBUG_tipvector[ i ] = tipVector;
DEBUG_targetnormal[ i ] = targetNormal;
DEBUG_planenormal[ i ] = planeNormal;
DEBUG_rotationdist[ i ] = rotationDistance;
DEBUG_targetnormallen[ i ] = targetNormalLen;
DEBUG_angle[ i ] = angle / DEG2RAD;
				// update the tip position for the next bone in the chain
				tipPosition = pChain[ pChainCount - 1 ].GetGlobalMatrix() * localPosition;
DEBUG_tipposition2[ i ] = tipPosition;
*/
			}
			
			// if the orientation is adjusted modify the orientation of the last
			// bone in the chain to be exactly the goal orientation
			if( pAdjustOrientation ){
				bonePosition = pChain[ pChainCount - 1 ].GetGlobalMatrix().GetPosition();
				
				goalMatrix.a14 = bonePosition.x;
				goalMatrix.a24 = bonePosition.y;
				goalMatrix.a34 = bonePosition.z;
				
				pChain[ pChainCount - 1 ].SetGlobalMatrix( goalMatrix );
			}
			
			// calculate the new tip position for the next run
			tipPosition = pChain[ pChainCount - 1 ].GetGlobalMatrix() * localPosition;
			
			// check if another round is required
			oldDist = curDist;
			curDist = ( tipPosition - goalPosition ).Length();
			//module.LOGINFOFORMAT( "IK: oldDist %g, curDist %g, correctionFactor %g", oldDist, curDist, correctionFactor );
			
			// if the target is close enough to the goal a solution has been found. as close enough counts
			// a distance of less than 1mm
			if( curDist < 1e-3f ){
				break;
			}
			
			// if the change in distance is too small two situations can happen. either the target is outside of the
			// reach of the chain or the chain ended up in a singularity
			if( fabsf( oldDist - curDist ) < 1e-5f ){
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
				/*
				if( strcmp( pChain[ 0 ].GetBoneState()->GetRigBoneName(), "leg.u.l" ) == 0 ){
					printf( "singularity: base=(%s) step=%i curDist=%g oldDist=%g tipDist=%g goalDist=%g\n",
						pChain[ 0 ].GetBoneState()->GetRigBoneName(), s, curDist, oldDist,
						sqrtf( tipDistSquared ), sqrtf( goalDistSquared ) );
					for( i=pChainCount-1; i>=0; i-- ){
						printf( "  chain %i: tipPosition(%g,%g,%g) bonePosition(%g,%g,%g) tipVector(%g,%g,%g)\n",
							i, DEBUG_tipposition1[ i ].x, DEBUG_tipposition1[ i ].y, DEBUG_tipposition1[ i ].z,
							DEBUG_bonepos[ i ].x, DEBUG_bonepos[ i ].y, DEBUG_bonepos[ i ].z,
							DEBUG_tipvector[ i ].x, DEBUG_tipvector[ i ].y, DEBUG_tipvector[ i ].z );
						printf( "            targetNormal(%g,%g,%g) planeNormal(%g,%g,%g), rotationDist=%g\n",
							DEBUG_targetnormal[ i ].x, DEBUG_targetnormal[ i ].y, DEBUG_targetnormal[ i ].z,
							DEBUG_planenormal[ i ].x, DEBUG_planenormal[ i ].y, DEBUG_planenormal[ i ].z,
							DEBUG_rotationdist[ i ] );
						printf( "            targetNormalLen=%g angle=%g tipPosition(%g,%g,%g)\n",
							DEBUG_targetnormallen[ i ], DEBUG_angle[ i ], DEBUG_tipposition2[ i ].x,
							DEBUG_tipposition2[ i ].y, DEBUG_tipposition2[ i ].z );
					}
				}
				*/
				break;
			}
			/*
			if( oldDist - curDist < 0.005f ){
				
				if( correctionFactor < 0.7f ){
					correctionFactor += 0.2f;
					
				}else if( oldDist - curDist < 0.001f ){
					break;
				}
			}
			*/
		}
		
		// update the bone state from our work state
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
			
			boneState.BlendWith( matrix.GetPosition(), matrix.ToQuaternion(), blendMode, blendFactor, true, true );
		}
	
	// if we have a singular bone
	}else if( pChainCount == 1 ){
		const bool hasSolverBone = pUseSolverBone && pSolverBone != -1;
		const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
		decVector goalPosition( constGoalPosition );
		decMatrix goalMatrix;
		
		// calculate the target values. this depends on the presence of a solver
		// bone as well as if the orientation has to be adjusted
		if( hasSolverBone ){
			dearBoneState &solverState = *stalist.GetStateAt( pSolverBone );
			solverState.UpdateMatrices();
			
			goalPosition = solverState.GetGlobalMatrix() * goalPosition;
		}
		
		if( pAdjustOrientation ){
			if( hasSolverBone ){
				goalMatrix = decMatrix::CreateFromQuaternion( constLocalOrientation )
					.QuickMultiply( decMatrix::CreateFromQuaternion( constGoalOrientation ) )
					.QuickMultiply( stalist.GetStateAt( pSolverBone )->GetGlobalMatrix().GetRotationMatrix() );
				
			}else{
				goalMatrix = decMatrix::CreateFromQuaternion( constLocalOrientation )
					.QuickMultiply( decMatrix::CreateFromQuaternion( constGoalOrientation ) );
			}
		}
		
		// set position and orientation to the bone from the ik settings.
		// the position is only set if there is no parent bone.
		dearBoneState &boneState = *stalist.GetStateAt( pChain[ 0 ].GetBoneStateIndex() );
		
		boneState.UpdateMatrices();
		
		if( ! pAdjustOrientation ){
			goalMatrix = boneState.GetGlobalMatrix();
		}
		goalMatrix.a14 = goalPosition.x;
		goalMatrix.a24 = goalPosition.y;
		goalMatrix.a34 = goalPosition.z;
		if( boneState.GetParentState() ){
			goalMatrix = goalMatrix.QuickMultiply( boneState.GetParentState()->GetInverseGlobalMatrix() );
		}
		goalMatrix = goalMatrix.QuickMultiply( boneState.GetInverseRigLocalMatrix() );
		
		if( pAdjustOrientation ){
			boneState.BlendWith( goalMatrix * -constLocalPosition,
				goalMatrix.ToQuaternion(), blendMode, blendFactor, pAdjustPosition, true );
			
		}else{
			boneState.BlendWith( goalMatrix.GetPosition() - constLocalPosition,
				decQuaternion(), blendMode, blendFactor, pAdjustPosition, false );
		}
	}
DEBUG_PRINT_TIMER;
}

void dearRuleInverseKinematic::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateChain();
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
