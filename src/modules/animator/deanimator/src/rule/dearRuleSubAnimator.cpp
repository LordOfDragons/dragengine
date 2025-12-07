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

#include "dearRuleSubAnimator.h"
#include "dearCreateRuleVisitor.h"
#include "../dearAnimator.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearVPSState.h"
#include "../dearVPSStateList.h"
#include "../dearControllerStates.h"
#include "../dearAnimatorInstance.h"
#include "../dearLink.h"
#include "../animation/dearAnimation.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>



// class dearRuleSubAnimator
//////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER(what)	GetModule().LogInfoFormat("Rule Sub Animator: %s = %iys", what, (int)(timer.GetElapsedTime() * 1000000.0)); timer.Reset()
	#define DEBUG_PRINT_TIMER_TOTAL	GetModule().LogInfoFormat("Rule Sub Animator = %iys", (int)(timerTotal.GetElapsedTime() * 1000000.0))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
	#define DEBUG_PRINT_TIMER_TOTAL
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleSubAnimator::dearRuleSubAnimator(dearAnimatorInstance &instance, const dearAnimator &animator,
	int firstLink, const deAnimatorRuleSubAnimator &rule, const decIntList &controllerMapping) :
dearRule(instance, animator, firstLink, rule),
pSubAnimator(rule),

pArSubAnimator(nullptr),
pSubAnimatorUpdateTracker(0),

pRules(nullptr),
pRuleCount(0),

pStateList(nullptr),
pVPSStateList(nullptr),

pEnablePosition(rule.GetEnablePosition()),
pEnableOrientation(rule.GetEnableOrientation()),
pEnableSize(rule.GetEnableSize()),
pEnableVPS(rule.GetEnableVertexPositionSet()),

// determine if the sub animator rules can be applied directly using our bone state list. this
// avoids the need for the copy and the apply phase. this optimization is only possible if:
// - blend mode is 'blend'
// - blend factor is 1
// - no rule bones are set
// - position modification is enabled
// - orientation modification is enabled
// - scale modification is enabled
// all except the blend factor can be precalculated since they stay the same
pDirectUseStates(
	  (rule.GetBlendMode() == deAnimatorRule::ebmBlend)
	&& (rule.GetListBones().GetCount() == 0)
	&& (rule.GetListVertexPositionSets().GetCount() == 0)
	&& rule.GetEnablePosition()
	&& rule.GetEnableOrientation()
	&& rule.GetEnableSize()
	&& rule.GetEnableVertexPositionSet()),

pHasSubAnimator(rule.GetSubAnimator())
{
	try{
		pStateList = instance.GetBoneStateList().CreateCopy();
		pVPSStateList = instance.GetVPSStateList().CreateCopy();
		
		if(pHasSubAnimator){
			pArSubAnimator = (dearAnimator*)rule.GetSubAnimator()->GetPeerAnimator();
			pSubAnimatorUpdateTracker = pArSubAnimator->GetUpdateTracker();
			pCreateRules(controllerMapping);
		}
		
		RuleChanged();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dearRuleSubAnimator::~dearRuleSubAnimator(){
	pCleanUp();
}



// Management
///////////////

void dearRuleSubAnimator::CaptureStateInto(int identifier){
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->CaptureStateInto(identifier);
	}
}

void dearRuleSubAnimator::StoreFrameInto(int identifier, const char *moveName, float moveTime){
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->StoreFrameInto(identifier, moveName, moveTime);
	}
}

bool dearRuleSubAnimator::RebuildInstance() const{
	deAnimator * const animator = pSubAnimator.GetSubAnimator();
	dearAnimator * arAnimator = nullptr;
	
	if(animator){
		arAnimator = (dearAnimator*)animator->GetPeerAnimator();
	}
	
	if(pArSubAnimator != arAnimator){
		return true;
	}
	
	if(pArSubAnimator && pArSubAnimator->GetUpdateTracker() != pSubAnimatorUpdateTracker){
		return true;
	}
	
	int i;
	bool rebuild = false;
	for(i=0; i<pRuleCount; i++){
		rebuild |= pRules[i]->RebuildInstance();
	}
	
	return rebuild;
}

void dearRuleSubAnimator::Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist){
DEBUG_RESET_TIMERS;
	if(! GetEnabled()){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if(blendFactor < FLOAT_SAFE_EPSILON){
		return;
	}
	
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	int i;
	
DEBUG_PRINT_TIMER("Prepare");
	
	if(pHasSubAnimator){
		// if we have a valid sub animator we apply it
		
		// determine if the sub animator rules can be applied directly
		// using our bone state list. this avoids the need for the copy
		// and the apply phase. this optimization is only possible if
		// blend mode is blend and blend factor is 1
		const bool directUse = pDirectUseStates && fabsf(1.0f - blendFactor) < FLOAT_SAFE_EPSILON;
		
		if(directUse){
			// direct use allows to directly use our states list without needing a copy/apply
			for(i=0; i<pRuleCount; i++){
				pRules[i]->Apply(stalist, vpsstalist);
			}
			
DEBUG_PRINT_TIMER("Update Bone States Directly");
			
		}else{
			// otherwise use the safer but slower way
			const int boneCount = GetBoneMappingCount();
			for(i=0; i<boneCount; i++){
				const int animatorBone = GetBoneMappingFor(i);
				if(animatorBone == -1){
					continue;
				}
				
				pStateList->GetStateAt(animatorBone)->SetFrom(*stalist.GetStateAt(animatorBone));
			}
			
			const int vpsCount = GetVPSMappingCount();
			for(i=0; i<vpsCount; i++){
				const int animatorVps = GetVPSMappingFor(i);
				if(animatorVps == -1){
					continue;
				}
				
				pVPSStateList->GetStateAt(animatorVps).SetFrom(vpsstalist.GetStateAt(animatorVps));
			}
DEBUG_PRINT_TIMER("Copy States");
			
			for(i=0; i<pRuleCount; i++){
				pRules[i]->Apply(*pStateList, *pVPSStateList);
			}
DEBUG_PRINT_TIMER("Apply Rules");
			
			for(i=0; i<boneCount; i++){
				const int animatorBone = GetBoneMappingFor(i);
				if(animatorBone == -1){
					continue;
				}
				
				stalist.GetStateAt(animatorBone)->BlendWith(
					*pStateList->GetStateAt( animatorBone ), blendMode, blendFactor,
					pEnablePosition, pEnableOrientation, pEnableSize);
			}
			
			for(i=0; i<vpsCount; i++){
				const int animatorVps = GetVPSMappingFor(i);
				if(animatorVps == -1){
					continue;
				}
				
				vpsstalist.GetStateAt(animatorVps).BlendWith(
					pVPSStateList->GetStateAt(animatorVps), blendMode, blendFactor, pEnableVPS);
			}
DEBUG_PRINT_TIMER("Apply Temporary State");
		}
		
	}else{
		// if the animator does not exist or is not valid we apply a reference state instead
		const int boneCount = GetBoneMappingCount();
		for(i =0; i <boneCount; i++){
			const int animatorBone = GetBoneMappingFor(i);
			if(animatorBone != -1){
				stalist.GetStateAt(animatorBone)->BlendWithDefault(blendMode,
					blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
			}
		}
		
		const int vpsCount = GetVPSMappingCount();
		for(i =0; i <vpsCount; i++){
			const int animatorVps = GetVPSMappingFor(i);
			if(animatorVps != -1){
				vpsstalist.GetStateAt(animatorVps).BlendWithDefault(
					blendMode, blendFactor, pEnableVPS);
			}
		}
	}
DEBUG_PRINT_TIMER_TOTAL;
}



// Private Functions
//////////////////////

void dearRuleSubAnimator::pCleanUp(){
	if(pRules){
		while(pRuleCount > 0){
			delete pRules[pRuleCount - 1];
			pRuleCount--;
		}
		
		delete [] pRules;
	}
	
	if(pVPSStateList){
		delete pVPSStateList;
	}
	if(pStateList){
		delete pStateList;
	}
}

void dearRuleSubAnimator::pCreateRules(const decIntList &controllerMapping){
	const deAnimator * const animator = pSubAnimator.GetSubAnimator();
	if(! animator){
		return;
	}
	
	const int ruleCount = animator->GetRuleCount();
	if(ruleCount == 0){
		return;
	}
	
	dearAnimatorInstance &instance = GetInstance();
	int i;
	
	// create sub controller mapping
	const int controllerCount = animator->GetControllerCount();
	const int connectionCount = pSubAnimator.GetConnectionCount();
	decIntList subControllerMapping;
	
	for(i=0; i<controllerCount; i++){
		if(i < connectionCount){
			const int localController = pSubAnimator.GetConnectionAt(i);
			
			if(localController == -1){
				subControllerMapping.Add(-1);
				
			}else{
				subControllerMapping.Add(controllerMapping.GetAt(localController));
			}
			
		}else{
			subControllerMapping.Add(-1);
		}
	}
	
	// add sub links
	const int linkCount = animator->GetLinkCount();
	const int firstLink = instance.GetLinkCount();
	
	if(linkCount > 0){
		dearLink *link = nullptr;
		
		try{
			for(i=0; i<linkCount; i++){
				link = new dearLink(instance, *animator->GetLinkAt(i), subControllerMapping);
				instance.AddLink(link);
				link = nullptr;
			}
			
		}catch(const deException &){
			if(link){
				delete link;
			}
			throw;
		}
	}
	
	// create rules
	dearCreateRuleVisitor visitor(instance, *(dearAnimator*)animator->GetPeerAnimator(),
		subControllerMapping, firstLink);
	
	pRules = new dearRule*[ruleCount];
	
	for(i=0; i<ruleCount; i++){
		pRules[pRuleCount] = visitor.CreateRuleFrom(*animator->GetRuleAt(i));
		if(pRules[pRuleCount]){
			pRuleCount++;
		}
	}
}
