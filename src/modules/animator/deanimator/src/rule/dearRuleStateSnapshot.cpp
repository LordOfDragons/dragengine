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
#include <string.h>

#include "dearRuleStateSnapshot.h"
#include "../deDEAnimator.h"
#include "../dearAnimatorInstance.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearVPSState.h"
#include "../dearVPSStateList.h"
#include "../dearAnimationState.h"
#include "../dearAnimationVPSState.h"
#include "../animation/dearAnimation.h"
#include "../animation/dearAnimationMove.h"
#include "../animation/dearAnimationKeyframe.h"
#include "../animation/dearAnimationKeyframeList.h"
#include "../animation/dearAnimationKeyframeVPS.h"
#include "../animation/dearAnimationKeyframeVPSList.h"
#include "../component/dearComponent.h"
#include "../component/dearComponentBoneState.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationBone.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animation/deAnimationKeyframe.h>
#include <dragengine/resources/animation/deAnimationKeyframeList.h>
#include <dragengine/resources/animation/deAnimationKeyframeVertexPositionSet.h>
#include <dragengine/resources/animation/deAnimationKeyframeVertexPositionSetList.h>
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
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat("Rule State Snapshot = %iys", (int)(timer.GetElapsedTime() * 1000000.0))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleStateSnapshot::dearRuleStateSnapshot(dearAnimatorInstance &instance,
const dearAnimator &animator, int firstLink, const deAnimatorRuleStateSnapshot &rule) :
dearRule(instance, animator, firstLink, rule),
//pStateSnapshot( rule ),

pAnimStates(nullptr),
pAnimStateCount(0),

pAnimVPSStates(nullptr),
pAnimVPSStateCount(0),

pEnablePosition(rule.GetEnablePosition()),
pEnableOrientation(rule.GetEnableOrientation()),
pEnableSize(rule.GetEnableSize()),
pEnableVPS(rule.GetEnableVertexPositionSet()),
pUseLastState(rule.GetUseLastState()),
pID(rule.GetID())
{
	instance.SetCaptureComponentState(); // gives us access to a captured state during apply
	RuleChanged();
}

dearRuleStateSnapshot::~dearRuleStateSnapshot(){
	if(pAnimStates){
		delete [] pAnimStates;
	}
}



// Management
///////////////

void dearRuleStateSnapshot::Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist){
DEBUG_RESET_TIMERS;
	if(! GetEnabled()){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if(blendFactor < FLOAT_SAFE_EPSILON){
		return;
	}
	
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const dearAnimatorInstance &instance = GetInstance();
	const int boneCount = GetBoneMappingCount();
	const int vpsCount = GetVPSMappingCount();
	int i;
	
	if(pUseLastState){
		dearComponent * const arcomponent = instance.GetComponent();
		
		if(arcomponent){
			for(i=0; i<boneCount; i++){
				const int animatorBone = GetBoneMappingFor(i);
				if(animatorBone == -1){
					continue;
				}
				
				dearBoneState &state = *stalist.GetStateAt(animatorBone);
				if(state.GetRigIndex() == -1){
					continue;
				}
				
				stalist.GetStateAt(animatorBone)->BlendWith(
					arcomponent->GetBoneStateAt(state.GetRigIndex()), blendMode,
					blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
			}
			
			for(i=0; i<vpsCount; i++){
				const int animatorVps = GetVPSMappingFor(i);
				if(animatorVps == -1){
					continue;
				}
				
				dearVPSState &state = vpsstalist.GetStateAt(animatorVps);
				if(state.GetModelIndex() == -1){
					continue;
				}
				
				vpsstalist.GetStateAt(animatorVps).BlendWith(
					arcomponent->GetVPSStateAt(state.GetModelIndex()),
					blendMode, blendFactor, pEnableVPS);
			}
			
		}else{
			for(i=0; i<boneCount; i++){
				const int animatorBone = GetBoneMappingFor(i);
				if(animatorBone == -1){
					continue;
				}
				
				stalist.GetStateAt(animatorBone)->BlendWithDefault(blendMode,
					blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
			}
			
			for(i=0; i<vpsCount; i++){
				const int animatorVps = GetVPSMappingFor(i);
				if(animatorVps == -1){
					continue;
				}
				
				vpsstalist.GetStateAt(animatorVps).BlendWithDefault(
					blendMode, blendFactor, pEnableVPS);
			}
		}
		
	}else{
		for(i=0; i<boneCount; i++){
			const int animatorBone = GetBoneMappingFor(i);
			if(animatorBone == -1){
				continue;
			}
			
			stalist.GetStateAt(animatorBone)->BlendWith(pAnimStates[i],
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
		}
		
		for(i=0; i<vpsCount; i++){
			const int animatorVps = GetVPSMappingFor(i);
			if(animatorVps == -1){
				continue;
			}
			
			vpsstalist.GetStateAt(animatorVps).BlendWith(pAnimVPSStates[i],
				blendMode, blendFactor, pEnableVPS);
		}
	}
DEBUG_PRINT_TIMER;
}

void dearRuleStateSnapshot::CaptureStateInto(int identifier){
	if(pUseLastState || pID != identifier){
		return;
	}
	
	// the parent animator instance ensures this is called only after pending animator tasks
	// have been finished. accessing the pAnimStates array here is thus safe
	const int boneCount = GetBoneMappingCount();
	const int vpsCount = GetVPSMappingCount();
	const dearAnimatorInstance &instance = GetInstance();
	const dearComponent * const arcomponent = instance.GetComponent();
	int i;
	
	if(arcomponent){
		deComponent &component = arcomponent->GetComponent();
		
		for(i=0; i<boneCount; i++){
			const int animatorBone = GetBoneMappingFor(i);
			if(animatorBone == -1){
				pAnimStates[i].Reset();
				continue;
			}
			
			const deComponentBone &componentBone = component.GetBoneAt(animatorBone);
			pAnimStates[i].SetPosition(componentBone.GetPosition());
			pAnimStates[i].SetOrientation(componentBone.GetRotation());
			pAnimStates[i].SetSize(componentBone.GetScale());
		}
		
		for(i=0; i<vpsCount; i++){
			const int animatorVps = GetVPSMappingFor(i);
			if(animatorVps == -1){
				pAnimVPSStates[i].Reset();
				continue;
			}
			
			pAnimVPSStates[i].SetWeight(component.GetVertexPositionSetWeightAt(animatorVps));
		}
		
	}else{
		for(i=0; i<boneCount; i++){
			pAnimStates[i].Reset();
		}
		
		for(i=0; i<vpsCount; i++){
			pAnimVPSStates[i].Reset();
		}
	}
}

void dearRuleStateSnapshot::StoreFrameInto(int identifier, const char *moveName, float moveTime){
	DEASSERT_NOTNULL(moveName)
	
	if(pUseLastState || pID != identifier){
		return;
	}
	
	// the parent animator instance ensures this is called only after pending animator tasks
	// have been finished. accessing the pAnimStates array here is thus safe
	const int boneCount = GetBoneMappingCount();
	const int vpsCount = GetVPSMappingCount();
	const dearAnimatorInstance &instance = GetInstance();
	const dearBoneStateList &stateList = instance.GetBoneStateList();
	const dearVPSStateList &vpsstateList = instance.GetVPSStateList();
	int i;
	
	const dearAnimation * const animation = GetUseAnimation();
	dearAnimationMove *move = nullptr;
	if(animation){
		move = animation->GetMoveNamed(moveName);
	}
	
	if(move){
		const deAnimation &engAnimation = *animation->GetAnimation();
		
		for(i=0; i<boneCount; i++){
			const int animatorBone = GetBoneMappingFor(i);
			if(animatorBone == -1){
				continue;
			}
			
			const int animationBone = engAnimation.FindBone(
				stateList.GetStateAt(animatorBone)->GetRigBoneName());
			if(animationBone == -1){
				pAnimStates[i].Reset();
				continue;
			}
			
			// determine keyframe containing the move time
			const dearAnimationKeyframeList &kflist = *move->GetKeyframeListAt(animationBone);
			const dearAnimationKeyframe * const keyframe = kflist.GetWithTime(moveTime);
			
			// if there are no keyframes use the default state
			if(! keyframe){
				pAnimStates[i].Reset();
				continue;
			}
			
			// calculate bone data
			const float time = moveTime - keyframe->GetTime();
			
			pAnimStates[i].SetPosition(keyframe->InterpolatePosition(time));
			pAnimStates[i].SetOrientation(keyframe->InterpolateRotation(time));
			pAnimStates[i].SetSize(keyframe->InterpolateScaling(time));
		}
		
		for(i=0; i<vpsCount; i++){
			const int animatorVps = GetVPSMappingFor(i);
			if(animatorVps == -1){
				continue;
			}
			
			const int animationVps = engAnimation.GetVertexPositionSets().IndexOf(
				vpsstateList.GetStateAt(animatorVps).GetName());
			if(animationVps == -1){
				pAnimVPSStates[i].Reset();
				continue;
			}
			
			// determine keyframe containing the move time
			const dearAnimationKeyframeVPSList &kflist = *move->GetKeyframeVPSListAt(animationVps);
			const dearAnimationKeyframeVPS * const keyframe = kflist.GetWithTime(moveTime);
			
			// if there are no keyframes use the default state
			if(! keyframe){
				pAnimVPSStates[i].Reset();
				continue;
			}
			
			// calculate bone data
			const float time = moveTime - keyframe->GetTime();
			pAnimVPSStates[i].SetWeight(keyframe->InterpolateWeight(time));
		}
		
	}else{
		for(i=0; i<boneCount; i++){
			pAnimStates[i].Reset();
		}
		
		for(i=0; i<vpsCount; i++){
			pAnimVPSStates[i].Reset();
		}
	}
}

void dearRuleStateSnapshot::RuleChanged(){
	dearRule::RuleChanged();
	
	if(! pUseLastState){
		pUpdateStates();
		pUpdateVPSStates();
	}
}



// Private functions
//////////////////////

void dearRuleStateSnapshot::pUpdateStates(){
	const int boneCount = GetBoneMappingCount();
	if(pAnimStateCount == boneCount){
		return;
	}
	
	if(pAnimStates){
		delete [] pAnimStates;
		pAnimStates = nullptr;
		pAnimStateCount = 0;
	}
	
	if(boneCount > 0){
		pAnimStates = new dearAnimationState[boneCount];
		pAnimStateCount = boneCount;
	}
}

void dearRuleStateSnapshot::pUpdateVPSStates(){
	const int vpsCount = GetVPSMappingCount();
	if(pAnimVPSStateCount == vpsCount){
		return;
	}
	
	if(pAnimVPSStates){
		delete [] pAnimVPSStates;
		pAnimVPSStates = nullptr;
		pAnimVPSStateCount = 0;
	}
	
	if(vpsCount > 0){
		pAnimVPSStates = new dearAnimationVPSState[vpsCount];
		pAnimVPSStateCount = vpsCount;
	}
}
