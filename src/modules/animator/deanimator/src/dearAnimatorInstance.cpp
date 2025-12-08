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

#include "dearAnimator.h"
#include "dearAnimatorInstance.h"
#include "dearBoneState.h"
#include "dearBoneStateList.h"
#include "dearVPSState.h"
#include "dearVPSStateList.h"
#include "dearControllerStates.h"
#include "deDEAnimator.h"
#include "rule/dearCreateRuleVisitor.h"
#include "rule/dearRule.h"
#include "animation/dearAnimation.h"
#include "dearLink.h"
#include "task/dearTaskApplyRules.h"
#include "component/dearComponent.h"
#include "component/dearComponentBoneState.h"
#include "component/dearComponentVPSState.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>



// Class dearAnimatorInstance
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimatorInstance::dearAnimatorInstance(deDEAnimator &module, deAnimatorInstance &instance) :
pModule(module),
pAnimatorInstance(instance),

pAnimator(nullptr),

pDirtyMappings(true),
pDirtyAnimator(true),
pDirtyRuleParams(false),

pUseBlending(false),
pSkipApply(false),
pUseAllBones(true),
pUseAllVPS(true),

pAnimatorUpdateTracker(0),

pAnimation(nullptr),
pComponent(nullptr),

pRules(nullptr),
pRuleCount(0),
pDirtyRules(true),

pCaptureComponentState(false),

pUseParallelTask(true),
pActiveTaskApplyRule(nullptr)
{
	try{
		AnimatorChanged();
		AnimationChanged();
		BlendFactorChanged();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dearAnimatorInstance::~dearAnimatorInstance(){
	pCleanUp();
}



// Management
///////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER(what)			pModule.LogInfoFormat("Animator Instance Timer: %s = %iys", what, (int)(timer.GetElapsedTime() * 1000000.0)); timer.Reset()
	#define DEBUG_PRINT_TIMER_TOTAL(what)	pModule.LogInfoFormat("Animator Instance Timer-Total: %s = %iys", what, (int)(timerTotal.GetElapsedTime() * 1000000.0))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
	#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif



int dearAnimatorInstance::GetLinkCount() const{
	return pLinks.GetCount();
}

dearLink *dearAnimatorInstance::GetLinkAt(int index) const{
	return (dearLink*)pLinks.GetAt(index);
}

void dearAnimatorInstance::AddLink(dearLink *link){
	DEASSERT_NOTNULL(link)
	
	pLinks.Add(link);
}



void dearAnimatorInstance::SetCaptureComponentState(){
	pCaptureComponentState = true;
}

void dearAnimatorInstance::ApplyRules(){
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->Apply(pBoneStateList, pVPSStateList);
	}
}

void dearAnimatorInstance::ApplyStateToArComponent() const{
	if(!pAnimator || !pComponent){
		return;
	}
	
	const deAnimatorRule::eBlendModes blendMode = pAnimatorInstance.GetBlendMode();
	const deRig * const sourceRig = pAnimator->GetAnimator().GetRig();
	const deRig * const destRig = pComponent->GetComponent().GetRig();
	
	if(sourceRig == destRig){
		if(pUseBlending){
			const float blendFactor = pAnimatorInstance.GetBlendFactor();
			pBoneStateList.ApplyToComponent(*pComponent, blendMode, blendFactor);
			pVPSStateList.ApplyToComponent(*pComponent, blendMode, blendFactor);
DEBUG_PRINT_TIMER("ApplyStateToArComponent with blending");
			
		}else{
			pBoneStateList.ApplyToComponent(*pComponent);
			pVPSStateList.ApplyToComponent(*pComponent);
DEBUG_PRINT_TIMER("ApplyStateToArComponent directly");
		}
		
	}else{
		// TODO retarget
	}
}

void dearAnimatorInstance::pStartTaskApplyRules(){
	if(!pComponent){
		return;
	}
	
	if(pUseParallelTask){
		dearTaskApplyRules * const task = pNewTaskApplyRules();
		
		// if we are the first task on the component update the arcomponent. if not set
		// component animator task. also make the task to depend on the previous set task.
		// this allows to properly do chained animator instances.
		deComponent &component = pComponent->GetComponent();
		if(component.GetAnimatorTask()){
			task->AddDependsOn(component.GetAnimatorTask());
			
		}else{
			pComponent->UpdateFromComponent();
		}
		
		// dispatch task
		deParallelProcessing &parallelProcessing = pModule.GetGameEngine()->GetParallelProcessing();
		parallelProcessing.AddTask(task);
		pActiveTaskApplyRule = task;
		if(parallelProcessing.GetOutputDebugMessages()){
			pModule.LogInfoFormat("Task %p dispatched (instance=%p component=%p list=%d)",
				task, this, pComponent, pTaskApplyRules.GetCount());
		}
		
		// it is required to set the animator task only after it has been added to the parallal processing
		// systen. if not there is the possibility for a dead-loop if a module reacts to the invalidate
		// bones situaton with a prepare bones. in this case the SetAnimatorTask task above would results
		// indirectly in a prepare bones which in turn tries to wait on the set task which though is not
		// yet known by the parallel processing system and thus the wait never finishes. there should be
		// no problem by using this run order
		component.SetAnimatorTask(task);
		
	}else{
		pComponent->UpdateFromComponent(); // does also UpdateMatrixFromComponent()
		//pComponent->UpdateMatrixFromComponent(); // required for track to rule
		
		ApplyRules();
		DEBUG_PRINT_TIMER("ApplyRules");
		
		pApplyStateToComponent();
		DEBUG_PRINT_TIMER("ApplyStateToComponent");
	}
}

void dearAnimatorInstance::StopTaskApplyRules(){
	// WARNING called by dearTaskApplyRules::Finished() only!
	
	if(!pActiveTaskApplyRule){
		return;
	}
	
	if(pComponent){
		deComponent &component = pComponent->GetComponent();
		if(component.GetAnimatorTask() == pActiveTaskApplyRule){
			component.SetAnimatorTask(nullptr);
		}
	}
	
	if(pModule.GetGameEngine()->GetParallelProcessing().GetOutputDebugMessages()){
		pModule.LogInfoFormat("Stop task %p (instance=%p component=%p list=%d)",
			pActiveTaskApplyRule, this, pComponent, pTaskApplyRules.GetCount());
	}
	pActiveTaskApplyRule = nullptr;
}



void dearAnimatorInstance::Apply(bool direct){
	if(pSkipApply || !pComponent || !pAnimator){
		return;
	}
	
	// it is important to ensure a potentially running parallel task is finished beyond here.
	// the calll to StartTaskApplyRules() does wait for the task but until then the code below
	// applies changes which potentially cause segfaults. wait has to be called no matter if
	// direct or indirect application is requested
	pWaitTaskApplyRules();
	
	DEBUG_RESET_TIMERS;
	
	pCheckAnimatorChanged(); // this has to be first since changes to animator invalidates rules
	
	pCheckRequireRebuild();
	DEBUG_PRINT_TIMER("CheckRequireRebuild");
	
	pUpdateAnimator();
	DEBUG_PRINT_TIMER("UpdateAnimator");
	
	pUpdateMappings();
	DEBUG_PRINT_TIMER("UpdateMappings");
	
	pUpdateRules();
	pUpdateRuleParams();
	DEBUG_PRINT_TIMER("UpdateRules");
	
	pUpdateControllerStates();
	DEBUG_PRINT_TIMER("UpdateControllerStates");
	
	pUpdateFakeRootBones();
	DEBUG_PRINT_TIMER("pUpdateFakeRootBones");
	
	const deRig * const sourceRig = pAnimator->GetAnimator().GetRig();
	const deRig * const destRig = pComponent->GetComponent().GetRig();
	
	if(sourceRig == destRig){
		// this is a temporary hack. if the animator and component have two different rigs assigned the
		// bone mappings are likely to be different. currently the code can not handle different bone
		// mapping counts between the animator and the component which leads to exceptions. hence for
		// the time being this is disabled until proper re-targeting code is in place
		
		if(direct){
			pComponent->UpdateFromComponent(); // does also UpdateMatrixFromComponent()
			//pComponent->UpdateMatrixFromComponent(); // required for track to rule
			ApplyRules();
			pApplyStateToComponent();
			
		}else{
			pStartTaskApplyRules();
		}
	}
	DEBUG_PRINT_TIMER_TOTAL("Apply");
}

void dearAnimatorInstance::CaptureStateInto(int identifier){
	// if there is a bound component and it has an animator task assigned we have to wait first for
	// the task to finish before we can capture the correct state
	pWaitAnimTaskFinished();
	
	// better safe than sorry. ensure no parallel task is running beyond this point
	pWaitTaskApplyRules();
	
	// ensure rules are ready to capture state
	pCheckAnimatorChanged(); // this has to be first since changes to animator invalidates rules
	pCheckRequireRebuild();
	pUpdateAnimator();
	pUpdateMappings();
	pUpdateRules();
	pUpdateRuleParams();
	
	// now the state can be captured
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->CaptureStateInto(identifier);
	}
}

void dearAnimatorInstance::StoreFrameInto(int identifier, const char *moveName, float moveTime){
	// if there is a bound component and it has an animator task assigned we have to wait first for
	// the task to finish before we can capture the correct state
	pWaitAnimTaskFinished();
	
	// better safe than sorry. ensure no parallel task is running beyond this point
	pWaitTaskApplyRules();
	
	// ensure rules are ready to capture state
	pCheckAnimatorChanged(); // this has to be first since changes to animator invalidates rules
	pCheckRequireRebuild();
	pUpdateAnimator();
	pUpdateMappings();
	pUpdateRules();
	pUpdateRuleParams();
	
	// now the frame can be stored
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->StoreFrameInto(identifier, moveName, moveTime);
	}
}



// Notifications
//////////////////

void dearAnimatorInstance::AnimatorChanged(){
	// this is safe with parallel tasks since animator is only accessed to create local copies
	deAnimator * const engAnimator = pAnimatorInstance.GetAnimator();
	
	pAnimator = engAnimator ? (dearAnimator*)engAnimator->GetPeerAnimator() : nullptr;
	
	pDirtyAnimator = true;
	pDirtyRules = true;
}

void dearAnimatorInstance::ComponentChanged(){
	// parallel task rules can access the component. we have to wait to not segfault
	pWaitTaskApplyRules();
	
	deComponent * const component = pAnimatorInstance.GetComponent();
	dearComponent * const arcomponent = component
		? (dearComponent*)component->GetPeerAnimator() : nullptr;
	
	if(arcomponent != pComponent){
		if(pComponent){
			pComponent->GetComponent().FreeReference();
		}
		pComponent = arcomponent;
	}
	
	pDirtyMappings = true;
	pDirtyRuleParams = true;
}

void dearAnimatorInstance::BlendFactorChanged(){
	// this is safe with parallel tasks since blend factor is only accessed only during
	// pApplyStateToComponent.
	const deAnimatorRule::eBlendModes blendMode = pAnimatorInstance.GetBlendMode();
	const float blendFactor = pAnimatorInstance.GetBlendFactor();
	
	if(blendMode == deAnimatorRule::ebmBlend){
		pUseBlending = fabsf(blendFactor - 1.0f) > FLOAT_SAFE_EPSILON;
		
	}else if(blendMode == deAnimatorRule::ebmOverlay){
		pUseBlending = true;
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	pSkipApply = fabsf(blendFactor) < FLOAT_SAFE_EPSILON;
}

void dearAnimatorInstance::AnimationChanged(){
	// this is safe with parallel tasks since animation is only accessed during prepare
	pAnimation = pAnimatorInstance.GetAnimation()
		? (dearAnimation*)pAnimatorInstance.GetAnimation()->GetPeerAnimator() : nullptr;
	pDirtyRuleParams = true;
}

void dearAnimatorInstance::EnableRetargetingChanged(){
}

void dearAnimatorInstance::ProtectDynamicBonesChanged(){
	if(!pAnimator){
		return;
	}
	
	const int count = pBoneStateList.GetStateCount();
	int i;
	
	if(pAnimatorInstance.GetProtectDynamicBones()){
		for(i=0; i<count; i++){
			dearBoneState &boneState = *pBoneStateList.GetStateAt(i);
			boneState.SetProtected(boneState.GetRigBone() && boneState.GetRigBone()->GetDynamic());
		}
		
	}else{
		for(i=0; i<count; i++){
			pBoneStateList.GetStateAt(i)->SetProtected(false);
		}
	}
}

void dearAnimatorInstance::ControllerChanged(int){
}



// Private functions
//////////////////////

void dearAnimatorInstance::pCleanUp(){
	pCancelTaskApplyRules();
	pTaskApplyRules.RemoveAll();
	
	pDropRules();
	
	if(pComponent){
		pComponent->GetComponent().FreeReference();
	}
}

void dearAnimatorInstance::pCheckRequireRebuild(){
	if(pDirtyAnimator){ // protect against segfaults in rules if animator changed
		pDirtyRules = true;
		return;
	}
	
	if(pRuleCount == 0){
		return;
	}
	if(pDirtyRules){
		pDirtyAnimator = true; // just to make sure it is set
		return;
	}
	
	int i;
	for(i=0; i<pRuleCount; i++){
		if(pRules[i]->RebuildInstance()){
			pDirtyRules = true;
			pDirtyAnimator = true;
		}
	}
}

void dearAnimatorInstance::pUpdateMappings(){
	if(!pDirtyMappings){
		return;
	}
	
	if(!pAnimator){
		pBoneStateList.SetStateCount(0);
		pMappingRigToState.RemoveAll();
		pUseAllBones = true;
		
		pVPSStateList.SetStateCount(0);
		pMappingModelToVPSState.RemoveAll();
		pUseAllVPS = true;
		
		pDirtyMappings = false;
		return;
	}
	
	const deComponent * const component = pAnimatorInstance.GetComponent();
	const deModel * const model = component ? component->GetModel() : nullptr;
	const deAnimator &animator = pAnimator->GetAnimator();
	const deRig * const rig = animator.GetRig();
	
	pBoneStateList.UpdateMappings(animator);
	pUseAllBones = animator.GetListBones().GetCount() == 0;
	
	pMappingRigToState.RemoveAll();
	
	if(rig){
		int i, count = rig->GetBoneCount();
		for(i=0; i<count; i++){
			pMappingRigToState.Add(-1);
		}
		
		count = pBoneStateList.GetStateCount();
		for(i=0; i<count; i++){
			pMappingRigToState.SetAt(pBoneStateList.GetStateAt(i)->GetRigIndex(), i);
		}
		
		if(pAnimatorInstance.GetProtectDynamicBones()){
			for(i=0; i<count; i++){
				dearBoneState &boneState = *pBoneStateList.GetStateAt(i);
				
				boneState.SetProtected(boneState.GetRigBone() && boneState.GetRigBone()->GetDynamic());
				//printf( "bone='%s' dynamic=%i protected=%i\n", boneState.GetRigBoneName(),
				//  (boneState.GetRigBone()&&boneState.GetRigBone()->GetDynamic())?1:0, boneState.GetProtected()?1:0 );
			}
		}
	}
	
	pVPSStateList.UpdateMappings(animator, component);
	pUseAllVPS = animator.GetListVertexPositionSets().GetCount() == 0;
	
	pMappingModelToVPSState.RemoveAll();
	
	if(model){
		int i, count = model->GetVertexPositionSetCount();
		for(i=0; i<count; i++){
			pMappingModelToVPSState.Add(-1);
		}
		
		count = pVPSStateList.GetStateCount();
		for(i=0; i<count; i++){
			pMappingModelToVPSState.SetAt(pVPSStateList.GetStateAt(i).GetModelIndex(), i);
		}
	}
	
	pDirtyMappings = false;
}

void dearAnimatorInstance::pCheckAnimatorChanged(){
	if(!pAnimator){
		return;
	}
	
	if(pAnimatorUpdateTracker != pAnimator->GetUpdateTracker()){
		pDirtyAnimator = true;
	}
}

void dearAnimatorInstance::pUpdateAnimator(){
	if(!pDirtyAnimator){
		return;
	}
	
	if(pAnimator){
		pAnimatorUpdateTracker = pAnimator->GetUpdateTracker();
		pControllerStates.SetStateCount(pAnimator->GetAnimator().GetControllerCount());
		
	}else{
		pAnimatorUpdateTracker = 0;
		pControllerStates.SetStateCount(0);
	}
	
	AnimatorChanged();
	AnimationChanged();
	
	pDirtyAnimator = false;
	pDirtyMappings = true;
	pDirtyRules = true;
}

void dearAnimatorInstance::pUpdateFakeRootBones(){
	if(pUseAllBones){
		return;
	}
	
	// if bones are defined it is possible a bone state has no parent state although it has a
	// rig parent. in this case update the local rig matrix to be a root bone like matrix.
	// we do not use component.PrepareBones() here to calculate only those global matrices
	// we really need to get the correct result
	const deRig * const rig = pAnimator->GetAnimator().GetRig();
	if(!rig){
		return;
	}
	
	const deComponent &component = pComponent->GetComponent();
	const int boneCount = pBoneStateList.GetStateCount();
	int i;
	
	for(i=0; i<boneCount; i++){
		dearBoneState &state = *pBoneStateList.GetStateAt(i);
		if(state.GetParentState()){
			continue;
		}
		
		int rigParent = state.GetRigBone()->GetParent();
		if(rigParent == -1){
			continue;
		}
		
		decMatrix matrix(decMatrix::CreateRT(state.GetRigBone()->GetRotation(),
			state.GetRigBone()->GetPosition()));
		while(rigParent != -1){
			const deComponentBone &compBone = component.GetBoneAt(rigParent);
			const deRigBone &rigBone = rig->GetBoneAt(rigParent);
			
			matrix = matrix
				.QuickMultiply(decMatrix::CreateWorld(compBone.GetPosition(),
					compBone.GetRotation(), compBone.GetScale()))
				.QuickMultiply(decMatrix::CreateRT(
					rigBone.GetRotation(), rigBone.GetPosition()));
			
			rigParent = rigBone.GetParent();
		}
		
		state.SetRigLocalMatrix(matrix);
		state.SetDirty(true);
	}
}



void dearAnimatorInstance::pRemoveAllLinks(){
	const int linkCount = pLinks.GetCount();
	int i;
	
	for(i=0; i<linkCount; i++){
		delete (dearLink*)pLinks.GetAt(i);
	}
	
	pLinks.RemoveAll();
}

void dearAnimatorInstance::pAddAnimatorLinks(){
	if(!pAnimator){
		return;
	}
	
	const deAnimator &animator = pAnimator->GetAnimator();
	const int linkCount = animator.GetLinkCount();
	
	if(linkCount > 0){
		const int controllerCount = pAnimatorInstance.GetControllerCount();
		decIntList controllerMapping;
		dearLink *link = nullptr;
		int i;
		
		try{
			for(i=0; i<controllerCount; i++){
				controllerMapping.Add(i);
			}
			
			for(i=0; i<linkCount; i++){
				link = new dearLink(*this, *animator.GetLinkAt(i), controllerMapping);
				AddLink(link);
				link = nullptr;
			}
			
		}catch(const deException &){
			if(link){
				delete link;
			}
			throw;
		}
	}
}

void dearAnimatorInstance::pDropRules(){
	if(pRules){
		while(pRuleCount > 0){
			delete pRules[pRuleCount - 1];
			pRuleCount--;
		}
		
		delete [] pRules;
		pRules = nullptr;
	}
	
	pRemoveAllLinks();
	
	pCaptureComponentState = false;
}

void dearAnimatorInstance::pUpdateRules(){
	if(!pDirtyRules){
		return;
	}
	
	pDropRules();
	
	if(pAnimator){
		const deAnimator &animator = pAnimator->GetAnimator();
		const int ruleCount = animator.GetRuleCount();
		
		pAddAnimatorLinks();
		
		if(ruleCount > 0){
			const int controllerCount = pAnimatorInstance.GetControllerCount();
			decIntList controllerMapping;
			int i;
			
			for(i=0; i<controllerCount; i++){
				controllerMapping.Add(i);
			}
			
			dearCreateRuleVisitor visitor(*this, *pAnimator, controllerMapping, 0);
			
			pRules = new dearRule*[ruleCount];
			
			for(i=0; i<ruleCount; i++){
				pRules[pRuleCount] = visitor.CreateRuleFrom(*animator.GetRuleAt(i));
				
				if(pRules[pRuleCount]){
					pRuleCount++;
				}
			}
		}
	}
	
	pDirtyRules = false;
	pDirtyRuleParams = false; // we just build them
}

void dearAnimatorInstance::pUpdateRuleParams(){
	if(!pDirtyRuleParams){
		return;
	}
	
	int i;
	for(i=0; i<pRuleCount; i++){
		pRules[i]->RuleChanged();
	}
}



void dearAnimatorInstance::pUpdateControllerStates(){
	const int stateCount = pControllerStates.GetStateCount();
	int i;
	
	for(i=0; i<stateCount; i++){
		const deAnimatorController &controller = pAnimatorInstance.GetControllerAt(i);
		
		const float minValue = controller.GetMinimumValue();
		const float range = controller.GetMaximumValue() - minValue;
		
		if(range > 1e-5f){
			pControllerStates.SetStateAt(i,
				(controller.GetCurrentValue() - minValue) / range, controller.GetVector());
			
		}else{
			pControllerStates.ResetStateAt(i);
		}
	}
}

void dearAnimatorInstance::pApplyStateToComponent() const{
	if(!pAnimator || !pComponent){
		return;
	}
	
	deComponent &component = pComponent->GetComponent();
	const deAnimatorRule::eBlendModes blendMode = pAnimatorInstance.GetBlendMode();
	const deRig * const sourceRig = pAnimator->GetAnimator().GetRig();
	const deRig * const destRig = component.GetRig();
	float blendFactor = 1.0f;
	
	if(pUseBlending){
		blendFactor = pAnimatorInstance.GetBlendFactor();
	}
	
	if(sourceRig == destRig){
		if(pUseBlending){
			pBoneStateList.ApplyToComponent(&component, blendMode, blendFactor);
			pVPSStateList.ApplyToComponent(component, blendMode, blendFactor);
DEBUG_PRINT_TIMER("ApplyStateToComponent with blending");
			
		}else{
			pBoneStateList.ApplyToComponent(&component);
			pVPSStateList.ApplyToComponent(component);
DEBUG_PRINT_TIMER("ApplyStateToComponent directly");
		}
		
	}else{
		// TODO retarget
	}
}


dearTaskApplyRules *dearAnimatorInstance::pNewTaskApplyRules(){
	// find task to reuse. only tasks that are finished and not depended on by any other task
	// can be reused. just checking the finished state is not enough since tasks can be fully
	// finished while other tasks still wait for them. the waiting for task is only cleared
	// once such a task has finished. this consume a bit more memory but is fail safe
	// 
	// NOTE the running task is also kept in this list. hence the check for finished is required
	const int count = pTaskApplyRules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dearTaskApplyRules * const task = (dearTaskApplyRules*)pTaskApplyRules.GetAt(i);
		if(task->GetFinished() && task->GetDependedOnBy().GetCount() == 0){
			return task;
		}
	}
	
	// no such task exists. create a new one and add it to the list
	const dearTaskApplyRules::Ref task(dearTaskApplyRules::Ref::NewWith(*this));
	pTaskApplyRules.Add(task);
	return task;
}

void dearAnimatorInstance::pCancelTaskApplyRules(){
	if(!pActiveTaskApplyRule){
		return;
	}
	
	if(pModule.GetGameEngine()->GetParallelProcessing().GetOutputDebugMessages()){
		pModule.LogInfoFormat("Cancel task %p (instance=%p component=%p list=%d)",
			pActiveTaskApplyRule, this, pComponent, pTaskApplyRules.GetCount());
	}
	pActiveTaskApplyRule->Cancel();
	
	pWaitTaskApplyRules();
}

void dearAnimatorInstance::pWaitTaskApplyRules(){
	if(!pActiveTaskApplyRule){
		return;
	}
	
	deParallelProcessing &parallelProcessing = pModule.GetGameEngine()->GetParallelProcessing();
	if(parallelProcessing.GetPaused()){
		pActiveTaskApplyRule->Drop();
		
	}else{
		// parallel processing is paused if the engine is in progress of being stopped.
		// in this case all tasks have been waited for already and no tasks are running
		parallelProcessing.WaitForTask(pActiveTaskApplyRule);
	}
	pActiveTaskApplyRule = nullptr;
}

void dearAnimatorInstance::pWaitAnimTaskFinished(){
	if(pComponent){
		deParallelTask * const task = pComponent->GetComponent().GetAnimatorTask();
		if(task){
			pModule.GetGameEngine()->GetParallelProcessing().WaitForTask(task);
		}
	}
}
