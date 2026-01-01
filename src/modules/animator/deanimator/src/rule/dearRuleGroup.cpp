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

#include "dearRuleGroup.h"
#include "dearCreateRuleVisitor.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearVPSState.h"
#include "../dearVPSStateList.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>



// class dearRuleGroup
////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat("Rule Group = %iys", (int)(timer.GetElapsedTime() * 1000000.0))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleGroup::dearRuleGroup(dearAnimatorInstance &instance, const dearAnimator &animator,
	int firstLink, const deAnimatorRuleGroup &rule, const decIntList &controllerMapping) :
dearRule(instance, animator, firstLink, rule),

pGroup(rule),

pStateList(nullptr),
pStateList2(nullptr),

pVPSStateList(nullptr),
pVPSStateList2(nullptr),

pRules(nullptr),
pRuleCount(0),

pTargetSelect(rule.GetTargetSelect(), firstLink),

pApplicationType(rule.GetApplicationType()),
pUseCurrentState(rule.GetUseCurrentState()),
pEnablePosition(rule.GetEnablePosition()),
pEnableOrientation(rule.GetEnableOrientation()),
pEnableSize(rule.GetEnableSize()),
pEnableVPS(rule.GetEnableVertexPositionSet())
{
	try{
		pCreateRules(firstLink, controllerMapping);
		RuleChanged();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dearRuleGroup::~dearRuleGroup(){
	pCleanUp();
}



// Management
///////////////

void dearRuleGroup::CaptureStateInto(int identifier){
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->CaptureStateInto(identifier);
	}
}

void dearRuleGroup::StoreFrameInto(int identifier, const char *moveName, float moveTime){
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->StoreFrameInto(identifier, moveName, moveTime);
	}
}

bool dearRuleGroup::RebuildInstance() const{
	int i;
	bool rebuild = false;
	for(i=0; i<pRuleCount; i++){
		rebuild |= pRules[i]->RebuildInstance();
	}
	
	return rebuild;
}

void dearRuleGroup::Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist){
DEBUG_RESET_TIMERS;
	if(pRuleCount < 1 || !GetEnabled()){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if(blendFactor < FLOAT_SAFE_EPSILON){
		return;
	}
	
	dearAnimatorInstance &instance = GetInstance();
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const int boneCount = GetBoneMappingCount();
	const int vpsCount = GetVPSMappingCount();
	int i;
	
	// controller affected values
	float selectBlend = 0.0f;
	int selectIndex = 0;
	
	if(pApplicationType == deAnimatorRuleGroup::eatSelect && pRuleCount > 1){
		const float value = pTargetSelect.GetValue(instance, 0.0f);
		
		if(value <= 0.0f){
			selectIndex = 0;
			selectBlend = 0.0f;
			
		}else if(value >= 1.0f){
			selectIndex = pRuleCount - 1;
			selectBlend = 0.0f;
			
		}else{
			float intpart;
			selectBlend = modff(value * (float)(pRuleCount - 1), &intpart);
			selectIndex = (int)intpart;
		}
	}
	
	// apply all rules
	switch(pApplicationType){
	case deAnimatorRuleGroup::eatAll:
		// use current state if required. some rule setups can not be done without
		if(pUseCurrentState){
			for(i=0; i<boneCount; i++){
				const int animatorBone = GetBoneMappingFor(i);
				if(animatorBone == -1){
					continue;
				}
				pStateList->GetStateAt(animatorBone)->SetFrom(*stalist.GetStateAt(animatorBone));
			}
			
			for(i=0; i<vpsCount; i++){
				const int animatorVps = GetVPSMappingFor(i);
				if(animatorVps == -1){
					continue;
				}
				pVPSStateList->GetStateAt(animatorVps).SetFrom(vpsstalist.GetStateAt(animatorVps));
			}
		}
		
		// apply rules
		for(i=0; i<pRuleCount; i++){
			pRules[i]->Apply(*pStateList, *pVPSStateList);
		}
		
		// apply the state
		for(i=0; i<boneCount; i++){
			const int animatorBone = GetBoneMappingFor(i);
			if(animatorBone == -1){
				continue;
			}
			
			stalist.GetStateAt(animatorBone)->BlendWith(*pStateList->GetStateAt(animatorBone),
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
		}
		
		for(i=0; i<vpsCount; i++){
			const int animatorVps = GetVPSMappingFor(i);
			if(animatorVps == -1){
				continue;
			}
			
			vpsstalist.GetStateAt(animatorVps).BlendWith(
				pVPSStateList->GetStateAt(animatorVps), blendMode, blendFactor, pEnableVPS);
		}
		break;
		
	// apply a blend between two selected rules
	case deAnimatorRuleGroup::eatSelect:
		// if there are no rules apply the empty state
		if(pRuleCount == 0){
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
				vpsstalist.GetStateAt(animatorVps).BlendWithDefault(blendMode, blendFactor, pEnableVPS);
			}
		}
		
		// use current state if required. some rule setups can not be done without
		if(pUseCurrentState){
			for(i=0; i<boneCount; i++){
				const int animatorBone = GetBoneMappingFor(i);
				if(animatorBone == -1){
					continue;
				}
				
				const dearBoneState &stateFrom = *stalist.GetStateAt(animatorBone);
				pStateList->GetStateAt(animatorBone)->SetFrom(stateFrom);
				if(pStateList2){
					pStateList2->GetStateAt(animatorBone)->SetFrom(stateFrom);
				}
			}
			
			for(i=0; i<vpsCount; i++){
				const int animatorVps = GetVPSMappingFor(i);
				if(animatorVps == -1){
					continue;
				}
				
				const dearVPSState &stateFrom = vpsstalist.GetStateAt(animatorVps);
				pVPSStateList->GetStateAt(animatorVps).SetFrom(stateFrom);
				if(pVPSStateList2){
					pVPSStateList2->GetStateAt(animatorVps).SetFrom(stateFrom);
				}
			}
		}
		
		// apply the blend between the two selected rules
		// TODO there is an optimization possible if pValueSelectBlend is 0.0 .
		//      in this case the rule could be applied directly to the current state.
		//      the problem is that this would apply the rule with it's own blending
		//      state if rule->Apply(instance,statelist) is called. it is though required
		//      to merge in our own blending. in case of blending being blend(1) this
		//      shortcut though would work
		pRules[selectIndex]->Apply(*pStateList, *pVPSStateList);
		
		if(selectIndex < pRuleCount - 1){
			pRules[selectIndex + 1]->Apply(*pStateList2, *pVPSStateList2);
			
			for(i=0; i<boneCount; i++){
				const int animatorBone = GetBoneMappingFor(i);
				if(animatorBone == -1){
					continue;
				}
				
				pStateList->GetStateAt(animatorBone)->BlendWith(
					*pStateList2->GetStateAt( animatorBone ), deAnimatorRule::ebmBlend,
					selectBlend, pEnablePosition, pEnableOrientation, pEnableSize);
			}
			
			for(i=0; i<vpsCount; i++){
				const int animatorVps = GetVPSMappingFor(i);
				if(animatorVps == -1){
					continue;
				}
				
				pVPSStateList->GetStateAt(animatorVps).BlendWith(
					pVPSStateList2->GetStateAt(animatorVps),
					deAnimatorRule::ebmBlend, selectBlend, pEnableVPS);
			}
		}
		
		// apply the state
		for(i=0; i<boneCount; i++){
			const int animatorBone = GetBoneMappingFor(i);
			if(animatorBone == -1){
				continue;
			}
			
			stalist.GetStateAt(animatorBone)->BlendWith(*pStateList->GetStateAt(animatorBone),
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize);
		}
		
		for(i=0; i<vpsCount; i++){
			const int animatorVps = GetVPSMappingFor(i);
			if(animatorVps == -1){
				continue;
			}
			
			vpsstalist.GetStateAt(animatorVps).BlendWith(
				pVPSStateList->GetStateAt(animatorVps), blendMode, blendFactor, pEnableVPS);
		}
		break;
	}
DEBUG_PRINT_TIMER;
}

void dearRuleGroup::ControllerChanged(int controller){
	dearRule::ControllerChanged(controller);
	
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->ControllerChanged(controller);
	}
}

void dearRuleGroup::RuleChanged(){
	dearRule::RuleChanged();
	
	dearAnimatorInstance &instance = GetInstance();
	
	// free old state lists. they are potentially out of date
	if(pVPSStateList2){
		delete pVPSStateList2;
		pVPSStateList2 = nullptr;
	}
	if(pVPSStateList){
		delete pVPSStateList;
		pVPSStateList = nullptr;
	}
	
	if(pStateList2){
		delete pStateList2;
		pStateList2 = nullptr;
	}
	if(pStateList){
		delete pStateList;
		pStateList = nullptr;
	}
	
	// update all child rules
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->RuleChanged();
	}
	
	// create copies of the current bone state list if required
	pStateList = instance.GetBoneStateList().CreateCopy();
	pVPSStateList = instance.GetVPSStateList().CreateCopy();
	
	if(pApplicationType == deAnimatorRuleGroup::eatSelect && pRuleCount > 1){
		pStateList2 = instance.GetBoneStateList().CreateCopy();
		pVPSStateList2 = instance.GetVPSStateList().CreateCopy();
	}
}



// Private Functions
//////////////////////

void dearRuleGroup::pCleanUp(){
	if(pRules){
		while(pRuleCount > 0){
			delete pRules[pRuleCount - 1];
			pRuleCount--;
		}
		
		delete [] pRules;
	}
	
	if(pStateList2){
		delete pStateList2;
	}
	if(pStateList){
		delete pStateList;
	}
	if(pVPSStateList){
		delete pVPSStateList;
	}
	if(pVPSStateList2){
		delete pVPSStateList2;
	}
}

void dearRuleGroup::pCreateRules(int firstLink, const decIntList &controllerMapping){
	const int ruleCount = pGroup.GetRuleCount();
	if(ruleCount == 0){
		return;
	}
	
	dearCreateRuleVisitor visitor(GetInstance(), GetAnimator(), controllerMapping, firstLink);
	int i;
	
	pRules = new dearRule*[ruleCount];
	
	pRuleCount = 0;
	for(i=0; i<ruleCount; i++){
		pRules[pRuleCount] = visitor.CreateRuleFrom(*pGroup.GetRuleAt(i));
		
		if(pRules[pRuleCount]){
			pRuleCount++;
		}
	}
}
