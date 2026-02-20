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
#include <stdlib.h>
#include <string.h>

#include "aeSubAnimator.h"
#include "link/aeLink.h"
#include "locomotion/aeAnimatorLocomotion.h"
#include "aeAnimator.h"
#include "controller/aeController.h"
#include "rule/aeRule.h"
#include "../loadsave/aeLoadSaveSystem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleInverseKinematic.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/component/deComponent.h>



// Class aeSubAnimator
////////////////////////

// Constructor, destructor
////////////////////////////

aeSubAnimator::aeSubAnimator(deEngine *engine){
	if(!engine) DETHROW(deeInvalidParam);
	
	pEngine = engine;
	
	pEngAnimator = nullptr;
	pEngAnimatorInstance = nullptr;
	
	try{
		pEngAnimator = engine->GetAnimatorManager()->CreateAnimator();
		
		pEngAnimatorInstance = engine->GetAnimatorInstanceManager()->CreateAnimatorInstance();
		pEngAnimatorInstance->SetAnimator(pEngAnimator);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

aeSubAnimator::~aeSubAnimator(){
	pCleanUp();
}



// Management
///////////////

void aeSubAnimator::SetPathAnimator(const char *path){
	if(!path) DETHROW(deeInvalidParam);
	
	pPathAnimator = path;
}

void aeSubAnimator::LoadAnimator(aeLoadSaveSystem &lssys){
	pEngAnimatorInstance->SetAnimation(nullptr);
	
	if(pEngAnimator){
		pEngAnimator = nullptr;
	}
	
	if(pPathAnimator.IsEmpty()){
		pEngAnimatorInstance->SetAnimator(nullptr);
		return;
	}
	
	aeAnimator::Ref animator;
	int i;
	
	pEngine->GetLogger()->LogInfoFormat("Animator Editor",
		"Loading animator %s", pPathAnimator.GetString());
	
	try{
		// load from file
		animator = lssys.LoadAnimator(pPathAnimator);
		
		const int controllerCount = animator->GetControllers().GetCount();
		const int linkCount = animator->GetLinks().GetCount();
		const int ruleCount = animator->GetRules().GetCount();
		
		// create animator
		pEngAnimator = pEngine->GetAnimatorManager()->CreateAnimator();
		pEngAnimator->SetRig(animator->GetEngineAnimator()->GetRig());
		pEngAnimator->SetAnimation(animator->GetEngineAnimator()->GetAnimation());
		
		// add controllers
		for(i=0; i<controllerCount; i++){
			const aeController &controller = *animator->GetControllers().GetAt(i);
			
			const deAnimatorController::Ref engController(deAnimatorController::Ref::New());
			engController->SetName(controller.GetName());
			engController->SetValueRange(controller.GetMinimumValue(), controller.GetMaximumValue());
			engController->SetCurrentValue(controller.GetCurrentValue());
			engController->SetFrozen(controller.GetFrozen());
			engController->SetClamp(controller.GetClamp());
			engController->SetVector(controller.GetVector());
			
			pEngAnimator->AddController(engController);
		}
		
		// add links
		for(i=0; i<linkCount; i++){
			const aeLink &link = *animator->GetLinks().GetAt(i);
			
			const deAnimatorLink::Ref engLink(deAnimatorLink::Ref::New());
			
			if(link.GetController()){
				engLink->SetController(animator->GetControllers().IndexOf(link.GetController()));
			}
			engLink->SetCurve(link.GetCurve());
			engLink->SetRepeat(link.GetRepeat());
			
			pEngAnimator->AddLink(engLink);
		}
		
		// add rules
		for(i=0; i<ruleCount; i++){
			pEngAnimator->AddRule(animator->GetRules().GetAt(i)->CreateEngineRule());
		}
		
	}catch(const deException &e){
		pEngine->GetLogger()->LogException("Animator Editor", e);
		pEngAnimator = nullptr;
	}
	
	pEngAnimatorInstance->SetAnimator(pEngAnimator);
}



void aeSubAnimator::ClearAnimator(){
	/* this seems wrong
	pEngAnimator->RemoveAllRules();
	pEngAnimator->RemoveAllLinks();
	pEngAnimator->RemoveAllControllers();
	*/
}

void aeSubAnimator::AddController(const char *name, float minimum, float maximum, bool clamp){
	pEngAnimatorInstance->SetAnimator(nullptr);
	
	const deAnimatorController::Ref engController(deAnimatorController::Ref::New());
	engController->SetName(name);
	engController->SetValueRange(minimum, maximum);
	engController->SetFrozen(false);
	engController->SetClamp(clamp);
	
	pEngAnimator->AddController(engController);
	
	pEngAnimatorInstance->SetAnimator(pEngAnimator);
}

void aeSubAnimator::SetControllerValue(int controller, float value){
	pEngAnimatorInstance->GetControllers().GetAt(controller)->SetCurrentValue(value);
	pEngAnimatorInstance->NotifyControllerChangedAt(controller);
}

void aeSubAnimator::AddLink(int controller, const decCurveBezier &curve){
	const deAnimatorLink::Ref engLink(deAnimatorLink::Ref::New());
	engLink->SetController(controller);
	engLink->SetCurve(curve);
	
	pEngAnimator->AddLink(engLink);
}

void aeSubAnimator::AddRuleSS(){
	const deAnimatorRuleStateSnapshot::Ref engRule(deAnimatorRuleStateSnapshot::Ref::New());
	engRule->SetUseLastState(true);
	pEngAnimator->AddRule(engRule);
}

void aeSubAnimator::AddRuleIK(const decVector &localPosition, const decVector &localOrientation,
const char *solverBone, int linkBlendFactor){
	const deAnimatorRuleInverseKinematic::Ref engRule(deAnimatorRuleInverseKinematic::Ref::New());
	
	engRule->SetLocalPosition(localPosition);
	engRule->SetLocalOrientation(decMatrix::CreateRotation(localOrientation * DEG2RAD).ToQuaternion());
	engRule->SetUseSolverBone(true);
	engRule->SetSolverBone(solverBone);
	engRule->SetAdjustOrientation(true);
	engRule->SetBlendMode(deAnimatorRule::ebmBlend);
	
	if(linkBlendFactor != -1){
		engRule->GetTargetBlendFactor().AddLink(linkBlendFactor);
	}
	
	pEngAnimator->AddRule(engRule);
}

void aeSubAnimator::AddBoneToRule(int rule, const char *bone){
	pEngAnimator->GetRules().GetAt(rule)->GetListBones().Add(bone);
	pEngAnimator->NotifyRulesChanged();
}

void aeSubAnimator::EnableRule(int rule, bool enable){
	pEngAnimator->GetRules().GetAt(rule)->SetEnabled(enable);
	pEngAnimator->NotifyRulesChanged();
}



void aeSubAnimator::SetComponent(deComponent *component){
	pEngAnimator->SetRig(nullptr);
	pEngAnimatorInstance->SetComponent(nullptr);
	
	if(component){
		pEngAnimator->SetRig(component->GetRig());
		
	}else{
		pEngAnimator->SetRig(nullptr);
	}
	pEngAnimatorInstance->SetComponent(component);
}

void aeSubAnimator::SetComponentAndAnimation(deComponent *component, deAnimation *animation){
	pEngAnimator->SetAnimation(nullptr);
	pEngAnimator->SetRig(nullptr);
	pEngAnimatorInstance->SetComponent(nullptr);
	
	pEngAnimator->SetAnimation(animation);
	if(component){
		pEngAnimator->SetRig(component->GetRig());
		
	}else{
		pEngAnimator->SetRig(nullptr);
	}
	pEngAnimatorInstance->SetComponent(component);
}

void aeSubAnimator::CopyControllers(deAnimatorInstance &instance){
	pEngAnimatorInstance->GetControllers().VisitIndexed([&](int i, deAnimatorController &c){
		const int index = instance.IndexOfControllerNamed(c.GetName());
		if(index == -1){
			return;
		}
		
		const deAnimatorController &from = instance.GetControllers().GetAt(index);
		c.SetValueRange(from.GetMinimumValue(), from.GetMaximumValue());
		c.SetCurrentValue(from.GetCurrentValue());
		c.SetVector(from.GetVector());
		pEngAnimatorInstance->NotifyControllerChangedAt(i);
	});
}

void aeSubAnimator::Apply(){
	pEngAnimatorInstance->Apply();
}



// Private Functions
//////////////////////

void aeSubAnimator::pCleanUp(){
	if(pEngAnimatorInstance){
		pEngAnimatorInstance->SetAnimator(nullptr);
		pEngAnimatorInstance->SetComponent(nullptr);
	}
	
	if(pEngAnimator){
		pEngAnimator->SetRig(nullptr);
		pEngAnimator->SetAnimation(nullptr);
	}
}
