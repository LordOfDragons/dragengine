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

#include "dearRuleStateSnapshot.h"
#include "../dearAnimatorInstance.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearAnimationState.h"
#include "../deDEAnimator.h"
#include "../animation/dearAnimation.h"
#include "../animation/dearAnimationMove.h"
#include "../animation/dearAnimationKeyframeList.h"
#include "../animation/dearAnimationKeyframe.h"
#include "../component/dearComponent.h"
#include "../component/dearComponentBoneState.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationBone.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animation/deAnimationKeyframe.h>
#include <dragengine/resources/animation/deAnimationKeyframeList.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponent.h>



// Class dearRuleStateSnapshot
////////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat( "Rule State Snapshot = %iys", ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleStateSnapshot::dearRuleStateSnapshot( dearAnimatorInstance &instance,
const dearAnimator &animator, int firstLink, const deAnimatorRuleStateSnapshot &rule ) :
dearRule( instance, animator, firstLink, rule ),
//pStateSnapshot( rule ),

pAnimStates( NULL ),
pAnimStateCount( 0 ),

pEnablePosition( rule.GetEnablePosition() ),
pEnableOrientation( rule.GetEnableOrientation() ),
pEnableSize( rule.GetEnableSize() ),
pUseLastState( rule.GetUseLastState() ),
pID( rule.GetID() )
{
	instance.SetCaptureComponentState(); // gives us access to a captured state during apply
	RuleChanged();
}

dearRuleStateSnapshot::~dearRuleStateSnapshot(){
	if( pAnimStates ){
		delete [] pAnimStates;
	}
}



// Management
///////////////

void dearRuleStateSnapshot::Apply( dearBoneStateList &stalist ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() ){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if( blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const dearAnimatorInstance &instance = GetInstance();
	const int boneCount = GetBoneMappingCount();
	int i;
	
	if( pUseLastState ){
		dearComponent * const arcomponent = instance.GetComponent();
		
		if( arcomponent ){
			for( i=0; i<boneCount; i++ ){
				const int animatorBone = GetBoneMappingFor( i );
				if( animatorBone == -1 ){
					continue;
				}
				
				dearBoneState &state = *stalist.GetStateAt( animatorBone );
				if( state.GetRigIndex() == -1 ){
					continue;
				}
				
				stalist.GetStateAt( animatorBone )->BlendWith(
					arcomponent->GetBoneStateAt( state.GetRigIndex() ), blendMode,
					blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
			}
			
		}else{
			for( i=0; i<boneCount; i++ ){
				const int animatorBone = GetBoneMappingFor( i );
				if( animatorBone == -1 ){
					continue;
				}
				
				stalist.GetStateAt( animatorBone )->BlendWithDefault( blendMode,
					blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
			}
		}
		
	}else{
		for( i=0; i<boneCount; i++ ){
			const int animatorBone = GetBoneMappingFor( i );
			if( animatorBone == -1 ){
				continue;
			}
			
			stalist.GetStateAt( animatorBone )->BlendWith( pAnimStates[ i ],
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
		}
	}
DEBUG_PRINT_TIMER;
}

void dearRuleStateSnapshot::CaptureStateInto( int identifier ){
	if( pUseLastState ){
		return;
	}
	if( pID != identifier ){
		return;
	}
	
	// the parent animator instance ensures this is called only after pending animator tasks
	// have been finished. accessing the pAnimStates array here is thus safe
	const int boneCount = GetBoneMappingCount();
	const dearAnimatorInstance &instance = GetInstance();
	const dearComponent * const arcomponent = instance.GetComponent();
	int i;
	
	if( arcomponent ){
		deComponent &component = arcomponent->GetComponent();
		
		for( i=0; i<boneCount; i++ ){
			const int animatorBone = GetBoneMappingFor( i );
			if( animatorBone == -1 ){
				pAnimStates[ i ].Reset();
				continue;
			}
			
			const deComponentBone &componentBone = component.GetBoneAt( animatorBone );
			pAnimStates[ i ].SetPosition( componentBone.GetPosition() );
			pAnimStates[ i ].SetOrientation( componentBone.GetRotation() );
			pAnimStates[ i ].SetSize( componentBone.GetScale() );
		}
		
	}else{
		for( i=0; i<boneCount; i++ ){
			pAnimStates[ i ].Reset();
		}
	}
}

void dearRuleStateSnapshot::StoreFrameInto( int identifier, const char *moveName, float moveTime ){
	if( ! moveName ){
		DETHROW( deeInvalidParam );
	}
	
	if( pUseLastState ){
		return;
	}
	if( pID != identifier ){
		return;
	}
	
	// the parent animator instance ensures this is called only after pending animator tasks
	// have been finished. accessing the pAnimStates array here is thus safe
	const int boneCount = GetBoneMappingCount();
	const dearAnimatorInstance &instance = GetInstance();
	const dearBoneStateList &stateList = instance.GetBoneStateList();
	int i;
	
	const dearAnimation * const animation = GetUseAnimation();
	dearAnimationMove *move = nullptr;
	if( animation ){
		move = animation->GetMoveNamed( moveName );
	}
	
	if( move ){
		for( i=0; i<boneCount; i++ ){
			const int animatorBone = GetBoneMappingFor( i );
			if( animatorBone == -1 ){
				continue;
			}
			
			const int animationBone = stateList.GetStateAt( animatorBone )->GetAnimationBone();
			if( animationBone == -1 ){
				pAnimStates[ i ].Reset();
				continue;
			}
			
			// determine keyframe containing the move time
			const dearAnimationKeyframeList &kflist = *move->GetKeyframeListAt( animationBone );
			const dearAnimationKeyframe * const keyframe = kflist.GetWithTime( moveTime );
			
			// if there are no keyframes use the default state
			if( ! keyframe ){
				pAnimStates[ i ].Reset();
				continue;
			}
			
			// calculate bone data
			const float time = moveTime - keyframe->GetTime();
			
			pAnimStates[ i ].SetPosition( keyframe->InterpolatePosition( time ) );
			pAnimStates[ i ].SetOrientation( keyframe->InterpolateRotation( time ) );
			pAnimStates[ i ].SetSize( keyframe->InterpolateScaling( time ) );
		}
		
	}else{
		for( i=0; i<boneCount; i++ ){
			pAnimStates[ i ].Reset();
		}
	}
}

void dearRuleStateSnapshot::RuleChanged(){
	dearRule::RuleChanged();
	
	if( ! pUseLastState ){
		pUpdateStates();
	}
}



// Private functions
//////////////////////

void dearRuleStateSnapshot::pUpdateStates(){
	const int boneCount = GetBoneMappingCount();
	if( pAnimStateCount == boneCount ){
		return;
	}
	
	if( pAnimStates ){
		delete [] pAnimStates;
		pAnimStates = NULL;
		pAnimStateCount = 0;
	}
	
	if( boneCount > 0 ){
		pAnimStates = new dearAnimationState[ boneCount ];
		pAnimStateCount = boneCount;
	}
}
