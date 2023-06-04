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

#include "dearRuleAnimationDifference.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearVPSState.h"
#include "../dearVPSStateList.h"
#include "../dearAnimationState.h"
#include "../deDEAnimator.h"
#include "../animation/dearAnimation.h"
#include "../animation/dearAnimationKeyframe.h"
#include "../animation/dearAnimationKeyframeList.h"
#include "../animation/dearAnimationKeyframeVPS.h"
#include "../animation/dearAnimationKeyframeVPSList.h"
#include "../animation/dearAnimationMove.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationDifference.h>
#include <dragengine/deEngine.h>



// class dearRuleAnimationDifference
//////////////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat( "Rule Animation Difference = %iys", ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleAnimationDifference::dearRuleAnimationDifference( dearAnimatorInstance &instance,
const dearAnimator &animator, int firstLink, const deAnimatorRuleAnimationDifference &rule ) :
dearRule( instance, animator, firstLink, rule ),

pAnimationDifference( rule ),

pMove1( NULL ),
pMove2( NULL ),

pTargetLeadingMoveTime( rule.GetTargetLeadingMoveTime(), firstLink ),
pTargetReferenceMoveTime( rule.GetTargetReferenceMoveTime(), firstLink ),

pEnablePosition( rule.GetEnablePosition() ),
pEnableOrientation( rule.GetEnableOrientation() ),
pEnableSize( rule.GetEnableSize() ),
pEnableVPS( rule.GetEnableVertexPositionSet() )
{
	RuleChanged();
}

dearRuleAnimationDifference::~dearRuleAnimationDifference(){
	if( pMove1 ){
		pMove1->FreeReference();
	}
	if( pMove2 ){
		pMove2->FreeReference();
	}
}
	


// Management
///////////////

void dearRuleAnimationDifference::Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() || ! pMove1 || ! pMove2 ){
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
	
	bool newBlendMode = true;//true; // temporary hack
	
	// move times
	const float ltime = pMove1->GetPlaytime() * decMath::clamp(
		pTargetLeadingMoveTime.GetValue( GetInstance(), pAnimationDifference.GetLeadingMoveTime() ), 0.0f, 1.0f );
	
	const float rtime = pMove2->GetPlaytime() * decMath::clamp(
		pTargetReferenceMoveTime.GetValue( GetInstance(), pAnimationDifference.GetReferenceMoveTime() ), 0.0f, 1.0f );
	
	// step through all bones and set animation
	for( i=0; i<boneCount; i++ ){
		const int animatorBone = GetBoneMappingFor( i );
		if( animatorBone == -1 ){
			continue;
		}
		
		// if there is no valid bone index there is no difference
		const int animationBone = pMapAnimationBones.GetAt( i );
		if( animationBone == -1 ){
			continue;
		}
		
		// determine leading animation state
		const dearAnimationKeyframeList &kflist1 = *pMove1->GetKeyframeListAt( animationBone );
		const dearAnimationKeyframe * const keyframe1 = kflist1.GetWithTime( ltime );
		
		decVector lscale( 1.0f, 1.0f, 1.0f );
		decQuaternion lorientation;
		decVector lposition;
		
		if( keyframe1 ){
			const float time = ltime - keyframe1->GetTime();
			if( pEnablePosition ){
				lposition = keyframe1->InterpolatePosition( time );
			}
			if( pEnableOrientation ){
				lorientation = keyframe1->InterpolateRotation( time );
			}
			if( pEnableSize ){
				lscale = keyframe1->InterpolateScaling( time );
			}
		}
		
		// determine reference animation state
		const dearAnimationKeyframeList &kflist2 = *pMove2->GetKeyframeListAt( animationBone );
		const dearAnimationKeyframe * const keyframe2 = kflist2.GetWithTime( rtime );
		
		decVector rscale( 1.0f, 1.0f, 1.0f );
		decQuaternion rorientation;
		decVector rposition;
		
		if( keyframe2 ){
			const float time = rtime - keyframe2->GetTime();
			if( pEnablePosition ){
				rposition = keyframe2->InterpolatePosition( time );
			}
			if( pEnableOrientation ){
				rorientation = keyframe2->InterpolateRotation( time );
			}
			if( pEnableSize ){
				rscale = keyframe2->InterpolateScaling( time );
			}
		}
		
		// blend difference with current state
		dearBoneState &boneState = *stalist.GetStateAt( animatorBone );
		
		if( newBlendMode ){
			rposition = boneState.GetPosition() + ( lposition - rposition );
			rorientation = ( lorientation * rorientation.Conjugate() ) * boneState.GetOrientation();
			lscale = boneState.GetScale() + ( lscale - rscale );
			
		}else{
			rposition = boneState.GetPosition() + ( lposition - rposition );
			rorientation = boneState.GetOrientation() + ( lorientation - rorientation );
			rscale = boneState.GetScale() + ( lscale - rscale );
			
			/*
			if( sourceFactor == 1.0f && destFactor == 0.0f ){
				boneState->SetPosition( lposition - rposition );
				boneState->SetOrientation( lorientation - rorientation );
				//boneState->SetSize( size1 - size2 );
			}else{
//				module.LOGINFOFORMAT( "animation difference: bone='%s'->%i drot=(%g,%g,%g,%g)", boneState->GetRigBoneName(), animationBone,
//					( lorientation - rorientation ).x, ( lorientation - rorientation ).y, ( lorientation - rorientation ).z,
//					( lorientation - rorientation ).w );
				boneState->SetPosition( boneState->GetPosition() * destFactor + ( lposition - rposition ) * sourceFactor );
				boneState->SetOrientation( boneState->GetOrientation() * destFactor + ( lorientation - rorientation ) * sourceFactor );
				//boneState->SetSize( boneState->GetSize() * destFactor
				//	+ ( size1 - size2 ) * sourceFactor );
			}*/
		}
		
		boneState.BlendWith( rposition, rorientation, rscale, blendMode,
			blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
	}
	
	// step through all vertex position sets and set animation
	for( i=0; i<vpsCount; i++ ){
		const int animatorVps = GetVPSMappingFor( i );
		if( animatorVps == -1 ){
			continue;
		}
		
		// if there is no valid bone index there is no difference
		const int animationVps = pMapAnimationVPS.GetAt( i );
		if( animationVps == -1 ){
			continue;
		}
		
		// determine leading animation state
		const dearAnimationKeyframeVPSList &kflist1 = *pMove1->GetKeyframeVPSListAt( animationVps );
		const dearAnimationKeyframeVPS * const keyframe1 = kflist1.GetWithTime( ltime );
		
		float lweight = 0.0f;
		
		if( keyframe1 ){
			const float time = ltime - keyframe1->GetTime();
			if( pEnableVPS ){
				lweight = keyframe1->InterpolateWeight( time );
			}
		}
		
		// determine reference animation state
		const dearAnimationKeyframeVPSList &kflist2 = *pMove2->GetKeyframeVPSListAt( animationVps );
		const dearAnimationKeyframeVPS * const keyframe2 = kflist2.GetWithTime( rtime );
		
		float rweight = 0.0f;
		
		if( keyframe2 ){
			const float time = rtime - keyframe2->GetTime();
			if( pEnableVPS ){
				rweight = keyframe2->InterpolateWeight( time );
			}
		}
		
		// blend difference with current state
		dearVPSState &vpsState = vpsstalist.GetStateAt( animatorVps );
		rweight = vpsState.GetWeight() + ( lweight - rweight );
		
		vpsState.BlendWith( rweight, blendMode, blendFactor, pEnableVPS );
	}
DEBUG_PRINT_TIMER;
}

void dearRuleAnimationDifference::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateMove();
	pMapAnimationBones.Init( *this );
	pMapAnimationVPS.Init( *this );
}



// Private functions
//////////////////////

void dearRuleAnimationDifference::pUpdateMove(){
	if( pMove1 ){
		pMove1->FreeReference();
		pMove1 = NULL;
	}
	if( pMove2 ){
		pMove2->FreeReference();
		pMove2 = NULL;
	}
	
	const dearAnimation * const animation = GetUseAnimation();
	if( animation ){
		pMove1 = animation->GetMoveNamed( pAnimationDifference.GetLeadingMoveName() );
		if( pMove1 ){
			pMove1->AddReference();
		}
		
		pMove2 = animation->GetMoveNamed( pAnimationDifference.GetReferenceMoveName() );
		if( pMove2 ){
			pMove2->AddReference();
		}
	}
}
