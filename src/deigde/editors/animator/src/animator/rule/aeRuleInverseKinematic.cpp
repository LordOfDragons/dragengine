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

// Constructor, destructor
////////////////////////////

aeRuleInverseKinematic::aeRuleInverseKinematic(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleInverseKinematic::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertInverseKinematic, aname),
pMPGoalPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.goalPosition, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPGoalOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.goalOrientation, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPLocalPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.localPosition, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPLocalOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.localOrientation, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPAdjustOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.adjustOrientation, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPUseSolverBone(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.useSolverBone, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPSolverBone(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.solverBone, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPReachRange(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.reachRange, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPReachBone(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.reachBone, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPReachCenter(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.reachCenter, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPTargetGoalPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetGoalPosition, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPTargetGoalOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetGoalOrientation, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPTargetLocalPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetLocalPosition, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPTargetLocalOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetLocalOrientation, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPTargetReachRange(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetReachRange, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
pMPTargetReachCenter(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.targetReachCenter, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>())
{
	pMPGoalPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetGoalPosition(pMPGoalPosition);
		}
		NotifyRuleChanged();
	};
	
	pMPGoalOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetGoalOrientation(pMPGoalOrientation);
			}
			NotifyRuleChanged();
	};
	
	pMPLocalPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetLocalPosition(pMPLocalPosition);
		}
		NotifyRuleChanged();
	};
	
	pMPLocalOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetLocalOrientation(pMPLocalOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPAdjustOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetAdjustOrientation(pMPAdjustOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPUseSolverBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetUseSolverBone(pMPUseSolverBone);
		}
		NotifyRuleChanged();
	};
	
	pMPSolverBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetSolverBone(pMPSolverBone);
		}
		NotifyRuleChanged();
	};
	
	pMPReachRange.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetReachRange(pMPReachRange);
		}
		NotifyRuleChanged();
	};
	
	pMPReachBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetReachBone(pMPReachBone);
		}
		NotifyRuleChanged();
	};
	
	pMPReachCenter.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetReachCenter(pMPReachCenter);
		}
		NotifyRuleChanged();
	};
	
	pTargetGoalPosition = aeControllerTarget::Ref::New(pMPTargetGoalPosition);
	pMPTargetGoalPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetGoalPosition(), pMPTargetGoalPosition);
		}
		pTargetGoalPosition->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetGoalOrientation = aeControllerTarget::Ref::New(pMPTargetGoalOrientation);
	pMPTargetGoalOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetGoalOrientation(), pMPTargetGoalOrientation);
		}
		pTargetGoalOrientation->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetLocalPosition = aeControllerTarget::Ref::New(pMPTargetLocalPosition);
	pMPTargetLocalPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetLocalPosition(), pMPTargetLocalPosition);
		}
		pTargetLocalPosition->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetLocalOrientation = aeControllerTarget::Ref::New(pMPTargetLocalOrientation);
	pMPTargetLocalOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetLocalOrientation(), pMPTargetLocalOrientation);
		}
		pTargetLocalOrientation->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetReachRange = aeControllerTarget::Ref::New(pMPTargetReachRange);
	pMPTargetReachRange.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetReachRange(), pMPTargetReachRange);
		}
		pTargetReachRange->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetReachCenter = aeControllerTarget::Ref::New(pMPTargetReachCenter);
	pMPTargetReachCenter.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleInverseKinematic*)GetEngineRule())->GetTargetReachCenter(), pMPTargetReachCenter);
		}
		pTargetReachCenter->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRuleInverseKinematic::aeRuleInverseKinematic(aeWindowMain &windowMain, const aeRuleInverseKinematic &copy) :
aeRuleInverseKinematic(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPGoalOrientation.SetValue(copy.pMPGoalOrientation, false);
	pMPLocalPosition.SetValue(copy.pMPLocalPosition, false);
	pMPLocalOrientation.SetValue(copy.pMPLocalOrientation, false);
	pMPAdjustOrientation.SetValue(copy.pMPAdjustOrientation, false);
	pMPUseSolverBone.SetValue(copy.pMPUseSolverBone, false);
	pMPSolverBone.SetValue(copy.pMPSolverBone, false);
	pMPReachRange.SetValue(copy.pMPReachRange, false);
	pMPReachBone.SetValue(copy.pMPReachBone, false);
	pMPReachCenter.SetValue(copy.pMPReachCenter, false);
	
	pTargetGoalPosition = aeControllerTarget::Ref::New(pMPTargetGoalPosition, copy.pTargetGoalPosition);
	pTargetGoalOrientation = aeControllerTarget::Ref::New(pMPTargetGoalOrientation, copy.pTargetGoalOrientation);
	pTargetLocalPosition = aeControllerTarget::Ref::New(pMPTargetLocalPosition, copy.pTargetLocalPosition);
	pTargetLocalOrientation = aeControllerTarget::Ref::New(pMPTargetLocalOrientation, copy.pTargetLocalOrientation);
	pTargetReachRange = aeControllerTarget::Ref::New(pMPTargetReachRange, copy.pTargetReachRange);
	pTargetReachCenter = aeControllerTarget::Ref::New(pMPTargetReachCenter, copy.pTargetReachCenter);
}

aeRuleInverseKinematic::~aeRuleInverseKinematic() = default;


// Management
///////////////

void aeRuleInverseKinematic::SetGoalPosition(const decVector &value){
	pMPGoalPosition = value;
}

void aeRuleInverseKinematic::SetGoalOrientation(const decVector &value){
	pMPGoalOrientation = value;
}

void aeRuleInverseKinematic::SetLocalPosition(const decVector &value){
	pMPLocalPosition = value;
}

void aeRuleInverseKinematic::SetLocalOrientation(const decVector &value){
	pMPLocalOrientation = value;
}

void aeRuleInverseKinematic::SetAdjustOrientation(bool value){
	pMPAdjustOrientation = value;
}

void aeRuleInverseKinematic::SetUseSolverBone(bool value){
	pMPUseSolverBone = value;
}

void aeRuleInverseKinematic::SetSolverBone(const char *value){
	pMPSolverBone = value;
}

void aeRuleInverseKinematic::SetReachRange(float value){
	pMPReachRange = value;
}

void aeRuleInverseKinematic::SetReachBone(const char *value){
	pMPReachBone = value;
}

void aeRuleInverseKinematic::SetReachCenter(const decVector &value){
	pMPReachCenter = value;
}


void aeRuleInverseKinematic::UpdateTargets(){
	auto engRule = static_cast<deAnimatorRuleInverseKinematic*>(GetEngineRule());
	
	aeRule::UpdateTargets();
	
	if(engRule){
		pUpdateEngineTarget(engRule->GetTargetGoalPosition(), pMPTargetGoalPosition);
		pUpdateEngineTarget(engRule->GetTargetGoalOrientation(), pMPTargetGoalOrientation);
		pUpdateEngineTarget(engRule->GetTargetLocalPosition(), pMPTargetLocalPosition);
		pUpdateEngineTarget(engRule->GetTargetLocalOrientation(), pMPTargetLocalOrientation);
		pUpdateEngineTarget(engRule->GetTargetReachRange(), pMPTargetReachRange);
		pUpdateEngineTarget(engRule->GetTargetReachCenter(), pMPTargetReachCenter);
	}
}

int aeRuleInverseKinematic::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetGoalPosition->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetGoalOrientation->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetLocalPosition->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetLocalOrientation->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetReachRange->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetReachCenter->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleInverseKinematic::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetGoalPosition->GetLinks().Has(link)){
		pTargetGoalPosition->RemoveLink(link);
	}
	if(pTargetGoalOrientation->GetLinks().Has(link)){
		pTargetGoalOrientation->RemoveLink(link);
	}
	if(pTargetLocalPosition->GetLinks().Has(link)){
		pTargetLocalPosition->RemoveLink(link);
	}
	if(pTargetLocalOrientation->GetLinks().Has(link)){
		pTargetLocalOrientation->RemoveLink(link);
	}
	if(pTargetReachRange->GetLinks().Has(link)){
		pTargetReachRange->RemoveLink(link);
	}
	if(pTargetReachCenter->GetLinks().Has(link)){
		pTargetReachCenter->RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleInverseKinematic::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetGoalPosition->RemoveAllLinks();
	pTargetGoalOrientation->RemoveAllLinks();
	pTargetLocalPosition->RemoveAllLinks();
	pTargetLocalOrientation->RemoveAllLinks();
	pTargetReachRange->RemoveAllLinks();
	pTargetReachCenter->RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule::Ref aeRuleInverseKinematic::CreateEngineRule(){
	const deAnimatorRuleInverseKinematic::Ref engRule(deAnimatorRuleInverseKinematic::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetGoalPosition(pMPGoalPosition);
	engRule->SetGoalOrientation(pMPGoalOrientation);
	engRule->SetLocalPosition(pMPLocalPosition);
	engRule->SetLocalOrientation(pMPLocalOrientation);
	engRule->SetAdjustOrientation(pMPAdjustOrientation);
	engRule->SetUseSolverBone(pMPUseSolverBone);
	engRule->SetSolverBone(pMPSolverBone);
	engRule->SetReachRange(pMPReachRange);
	engRule->SetReachCenter(pMPReachCenter);
	engRule->SetReachBone(pMPReachBone);
	
	pUpdateEngineTarget(engRule->GetTargetGoalPosition(), pMPTargetGoalPosition);
	pUpdateEngineTarget(engRule->GetTargetGoalOrientation(), pMPTargetGoalOrientation);
	pUpdateEngineTarget(engRule->GetTargetLocalPosition(), pMPTargetLocalPosition);
	pUpdateEngineTarget(engRule->GetTargetLocalOrientation(), pMPTargetLocalOrientation);
	pUpdateEngineTarget(engRule->GetTargetReachRange(), pMPTargetReachRange);
	pUpdateEngineTarget(engRule->GetTargetReachCenter(), pMPTargetReachCenter);
	
	return engRule;
}



aeRule::Ref aeRuleInverseKinematic::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleInverseKinematic::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetGoalPosition->AddLinksToList(list);
	pTargetGoalOrientation->AddLinksToList(list);
	pTargetLocalPosition->AddLinksToList(list);
	pTargetLocalOrientation->AddLinksToList(list);
	pTargetReachRange->AddLinksToList(list);
	pTargetReachCenter->AddLinksToList(list);
}
