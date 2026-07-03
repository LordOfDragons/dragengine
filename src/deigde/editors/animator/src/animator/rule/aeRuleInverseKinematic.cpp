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

#include "aeRuleInverseKinematic.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleInverseKinematic.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleInverseKinematic
/////////////////////////////////

aeRuleInverseKinematic::MetaContext::Ref aeRuleInverseKinematic::CreateMetaContext(aeWindowMain &windowMain, aeRuleInverseKinematic *rule){
	return MetaContext::Ref::New("animator.rule_inverse_kinematic", "Rule Inverse Kinematic", "Rule inverse kinematic properties",
		windowMain.GetMCAnimatorProperties().ruleInverseKinematic.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleInverseKinematic::aeRuleInverseKinematic(aeWindowMain &windowMain, const char *aname) :
aeRuleInverseKinematic(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleInverseKinematic::aeRuleInverseKinematic(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertInverseKinematic, aname),
mpGoalPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.goalPosition, metaContext),
mpGoalOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.goalOrientation, metaContext),
mpLocalPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.localPosition, metaContext),
mpLocalOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.localOrientation, metaContext),
mpAdjustOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.adjustOrientation, metaContext),
mpUseSolverBone(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.useSolverBone, metaContext),
mpSolverBone(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.solverBone, metaContext),
mpReachRange(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.reachRange, metaContext),
mpReachBone(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.reachBone, metaContext),
mpReachCenter(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.reachCenter, metaContext),
mpTargetGoalPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetGoalPosition, metaContext),
mpTargetGoalOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetGoalOrientation, metaContext),
mpTargetLocalPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetLocalPosition, metaContext),
mpTargetLocalOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetLocalOrientation, metaContext),
mpTargetReachRange(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetReachRange, metaContext),
mpTargetReachCenter(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetReachCenter, metaContext)
{
	mpGoalPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetGoalPosition(mpGoalPosition);
		}
		NotifyRuleChanged();
	};
	
	mpGoalOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetGoalOrientation(mpGoalOrientation);
			}
			NotifyRuleChanged();
	};
	
	mpLocalPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetLocalPosition(mpLocalPosition);
		}
		NotifyRuleChanged();
	};
	
	mpLocalOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetLocalOrientation(mpLocalOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpAdjustOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetAdjustOrientation(mpAdjustOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpUseSolverBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetUseSolverBone(mpUseSolverBone);
		}
		NotifyRuleChanged();
	};
	
	mpSolverBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetSolverBone(mpSolverBone);
		}
		NotifyRuleChanged();
	};
	
	mpReachRange.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetReachRange(mpReachRange);
		}
		NotifyRuleChanged();
	};
	
	mpReachBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetReachBone(mpReachBone);
		}
		NotifyRuleChanged();
	};
	
	mpReachCenter.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetReachCenter(mpReachCenter);
		}
		NotifyRuleChanged();
	};
	
	mpTargetGoalPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetGoalPosition(), mpTargetGoalPosition);
		}
		NotifyRuleChanged();
	};
	
	mpTargetGoalOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetGoalOrientation(), mpTargetGoalOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpTargetLocalPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetLocalPosition(), mpTargetLocalPosition);
		}
		NotifyRuleChanged();
	};
	
	mpTargetLocalOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetLocalOrientation(), mpTargetLocalOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpTargetReachRange.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetReachRange(), mpTargetReachRange);
		}
		NotifyRuleChanged();
	};
	
	mpTargetReachCenter.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetReachCenter(), mpTargetReachCenter);
		}
		NotifyRuleChanged();
	};
}

aeRuleInverseKinematic::aeRuleInverseKinematic(const aeRuleInverseKinematic &copy) :
aeRuleInverseKinematic(copy.GetWindowMain(), copy.GetName())
{
	pInitCopy(copy);
	mpGoalPosition.SetValue(copy.mpGoalPosition, false);
	mpGoalOrientation.SetValue(copy.mpGoalOrientation, false);
	mpLocalPosition.SetValue(copy.mpLocalPosition, false);
	mpLocalOrientation.SetValue(copy.mpLocalOrientation, false);
	mpAdjustOrientation.SetValue(copy.mpAdjustOrientation, false);
	mpUseSolverBone.SetValue(copy.mpUseSolverBone, false);
	mpSolverBone.SetValue(copy.mpSolverBone, false);
	mpReachRange.SetValue(copy.mpReachRange, false);
	mpReachBone.SetValue(copy.mpReachBone, false);
	mpReachCenter.SetValue(copy.mpReachCenter, false);
	mpTargetGoalPosition.SetValue(copy.mpTargetGoalPosition, false);
	mpTargetGoalOrientation.SetValue(copy.mpTargetGoalOrientation, false);
	mpTargetLocalPosition.SetValue(copy.mpTargetLocalPosition, false);
	mpTargetLocalOrientation.SetValue(copy.mpTargetLocalOrientation, false);
	mpTargetReachRange.SetValue(copy.mpTargetReachRange, false);
	mpTargetReachCenter.SetValue(copy.mpTargetReachCenter, false);
}

aeRuleInverseKinematic::~aeRuleInverseKinematic() = default;


// Management
///////////////

void aeRuleInverseKinematic::SetGoalPosition(const decVector &value){
	mpGoalPosition = value;
}

void aeRuleInverseKinematic::SetGoalOrientation(const decVector &value){
	mpGoalOrientation = value;
}

void aeRuleInverseKinematic::SetLocalPosition(const decVector &value){
	mpLocalPosition = value;
}

void aeRuleInverseKinematic::SetLocalOrientation(const decVector &value){
	mpLocalOrientation = value;
}

void aeRuleInverseKinematic::SetAdjustOrientation(bool value){
	mpAdjustOrientation = value;
}

void aeRuleInverseKinematic::SetUseSolverBone(bool value){
	mpUseSolverBone = value;
}

void aeRuleInverseKinematic::SetSolverBone(const char *value){
	mpSolverBone = value;
}

void aeRuleInverseKinematic::SetReachRange(float value){
	mpReachRange = value;
}

void aeRuleInverseKinematic::SetReachBone(const char *value){
	mpReachBone = value;
}

void aeRuleInverseKinematic::SetReachCenter(const decVector &value){
	mpReachCenter = value;
}


void aeRuleInverseKinematic::UpdateTargets(){
	auto engRule = static_cast<deAnimatorRuleInverseKinematic*>(GetEngineRule());
	
	aeRule::UpdateTargets();
	
	if(engRule){
		pUpdateEngineTarget(engRule->GetTargetGoalPosition(), mpTargetGoalPosition);
		pUpdateEngineTarget(engRule->GetTargetGoalOrientation(), mpTargetGoalOrientation);
		pUpdateEngineTarget(engRule->GetTargetLocalPosition(), mpTargetLocalPosition);
		pUpdateEngineTarget(engRule->GetTargetLocalOrientation(), mpTargetLocalOrientation);
		pUpdateEngineTarget(engRule->GetTargetReachRange(), mpTargetReachRange);
		pUpdateEngineTarget(engRule->GetTargetReachCenter(), mpTargetReachCenter);
	}
}

int aeRuleInverseKinematic::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(mpTargetGoalPosition->Has(link)){
		usageCount++;
	}
	if(mpTargetGoalOrientation->Has(link)){
		usageCount++;
	}
	if(mpTargetLocalPosition->Has(link)){
		usageCount++;
	}
	if(mpTargetLocalOrientation->Has(link)){
		usageCount++;
	}
	if(mpTargetReachRange->Has(link)){
		usageCount++;
	}
	if(mpTargetReachCenter->Has(link)){
		usageCount++;
	}
	
	return usageCount;
}


deAnimatorRule::Ref aeRuleInverseKinematic::CreateEngineRule(){
	const deAnimatorRuleInverseKinematic::Ref engRule(deAnimatorRuleInverseKinematic::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetGoalPosition(mpGoalPosition);
	engRule->SetGoalOrientation(mpGoalOrientation);
	engRule->SetLocalPosition(mpLocalPosition);
	engRule->SetLocalOrientation(mpLocalOrientation);
	engRule->SetAdjustOrientation(mpAdjustOrientation);
	engRule->SetUseSolverBone(mpUseSolverBone);
	engRule->SetSolverBone(mpSolverBone);
	engRule->SetReachRange(mpReachRange);
	engRule->SetReachCenter(mpReachCenter);
	engRule->SetReachBone(mpReachBone);
	
	pUpdateEngineTarget(engRule->GetTargetGoalPosition(), mpTargetGoalPosition);
	pUpdateEngineTarget(engRule->GetTargetGoalOrientation(), mpTargetGoalOrientation);
	pUpdateEngineTarget(engRule->GetTargetLocalPosition(), mpTargetLocalPosition);
	pUpdateEngineTarget(engRule->GetTargetLocalOrientation(), mpTargetLocalOrientation);
	pUpdateEngineTarget(engRule->GetTargetReachRange(), mpTargetReachRange);
	pUpdateEngineTarget(engRule->GetTargetReachCenter(), mpTargetReachCenter);
	
	return engRule;
}



aeRule::Ref aeRuleInverseKinematic::CreateCopy() const{
	return Ref::New(*this);
}
