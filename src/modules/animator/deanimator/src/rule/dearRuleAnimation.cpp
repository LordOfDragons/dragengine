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

#include "dearRuleAnimation.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearVPSState.h"
#include "../dearVPSStateList.h"
#include "../animation/dearAnimation.h"
#include "../animation/dearAnimationMove.h"
#include "../animation/dearAnimationKeyframe.h"
#include "../animation/dearAnimationKeyframeList.h"
#include "../animation/dearAnimationKeyframeVPS.h"
#include "../animation/dearAnimationKeyframeVPSList.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationBone.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animation/deAnimationKeyframe.h>
#include <dragengine/resources/animation/deAnimationKeyframeList.h>
#include <dragengine/resources/animation/deAnimationKeyframeVertexPositionSet.h>
#include <dragengine/resources/animation/deAnimationKeyframeVertexPositionSetList.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/deEngine.h>



// class dearRuleAnimation
////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat( "Rule Animation = %iys", ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleAnimation::dearRuleAnimation( dearAnimatorInstance &instance,
const dearAnimator &animator, int firstLink, const deAnimatorRuleAnimation &rule ) :
dearRule( instance, animator, firstLink, rule ),
pAnimation( rule ),

pMove( NULL ),

pTargetMoveTime( rule.GetTargetMoveTime(), firstLink ),

pEnablePosition( rule.GetEnablePosition() ),
pEnableOrientation( rule.GetEnableOrientation() ),
pEnableSize( rule.GetEnableSize() ),
pEnableVPS( rule.GetEnableVertexPositionSet() )
{
	RuleChanged();
}

dearRuleAnimation::~dearRuleAnimation(){
	if( pMove ){
		pMove->FreeReference();
	}
}



// Management
///////////////

void dearRuleAnimation::Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() || ! pMove ){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if( blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const int boneCount = GetBoneMappingCount();
	const int vpsCount = GetVPSMappingCount();
	int i;
	
	const float moveTime = pMove->GetPlaytime() * decMath::clamp(
		pTargetMoveTime.GetValue( GetInstance(), pAnimation.GetMoveTime()
			/ decMath::max( pMove->GetPlaytime(), 0.01f ) ), 0.0f, 1.0f );
	
	// step through all bones and set animation
	for( i=0; i<boneCount; i++ ){
		const int animatorBone = GetBoneMappingFor( i );
		if( animatorBone == -1 ){
			continue;
		}
		
		// determine animation state
		dearBoneState &boneState = *stalist.GetStateAt( animatorBone );
		const int animationBone = pMapAnimationBones.GetAt( i );
		
		if( animationBone == -1 ){
			boneState.BlendWithDefault( blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
			
		}else{
			// determine keyframe containing the move time
			const dearAnimationKeyframeList &kflist = *pMove->GetKeyframeListAt( animationBone );
			const dearAnimationKeyframe * const keyframe = kflist.GetWithTime( moveTime );
			
			// if there are no keyframes use the default state
			if( ! keyframe ){
				boneState.BlendWithDefault( blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
				continue;
			}
			
			// calculate bone data
			const float time = moveTime - keyframe->GetTime();
			
			decVector position;
			decQuaternion orientation;
			decVector scale( 1.0f, 1.0f, 1.0f );
			
			if( pEnablePosition ){
				position = keyframe->InterpolatePosition( time );
			}
			if( pEnableOrientation ){
				orientation = keyframe->InterpolateRotation( time );
			}
			if( pEnableSize ){
				scale = keyframe->InterpolateScaling( time );
			}
			
			boneState.BlendWith( position, orientation, scale, blendMode,
				blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
		}
	}
	
	// step through all vertex position sets and set animation
	for( i=0; i<vpsCount; i++ ){
		const int animatorVps = GetVPSMappingFor( i );
		if( animatorVps == -1 ){
			continue;
		}
		
		// determine animation state
		dearVPSState &vpsState = vpsstalist.GetStateAt( animatorVps );
		const int animationVps = pMapAnimationVPS.GetAt( i );
		
		if( animationVps == -1 ){
			vpsState.BlendWithDefault( blendMode, blendFactor, pEnableVPS );
			
		}else{
			// determine keyframe containing the move time
			const dearAnimationKeyframeVPSList &kflist = *pMove->GetKeyframeVPSListAt( animationVps );
			const dearAnimationKeyframeVPS * const keyframe = kflist.GetWithTime( moveTime );
			
			// if there are no keyframes use the default state
			if( ! keyframe ){
				vpsState.BlendWithDefault( blendMode, blendFactor, pEnableVPS );
				continue;
			}
			
			// calculate bone data
			const float time = moveTime - keyframe->GetTime();
			float weight = pEnableVPS ? keyframe->InterpolateWeight( time ) : 0.0f;
			
			vpsState.BlendWith( weight, blendMode, blendFactor, pEnableVPS );
		}
	}
DEBUG_PRINT_TIMER;
}

void dearRuleAnimation::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateMove();
	pMapAnimationBones.Init( *this );
	pMapAnimationVPS.Init( *this );
}



// Private functions
//////////////////////

void dearRuleAnimation::pUpdateMove(){
	if( pMove ){
		pMove->FreeReference();
		pMove = NULL;
	}
	
	const dearAnimation * const animation = GetUseAnimation();
	if( animation ){
		pMove = animation->GetMoveNamed( pAnimation.GetMoveName() );
		if( pMove ){
			pMove->AddReference();
		}
	}
}
