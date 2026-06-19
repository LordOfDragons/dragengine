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
pTargetGoalPosition(aeControllerTarget::Ref::New()),
pTargetGoalOrientation(aeControllerTarget::Ref::New()),
pTargetLocalPosition(aeControllerTarget::Ref::New()),
pTargetLocalOrientation(aeControllerTarget::Ref::New()),
pTargetReachRange(aeControllerTarget::Ref::New()),
pTargetReachCenter(aeControllerTarget::Ref::New()),
goalPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.goalPosition, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
goalOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.goalOrientation, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
localPosition(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.localPosition, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
localOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.localOrientation, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
adjustOrientation(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.adjustOrientation, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
useSolverBone(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.useSolverBone, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
solverBone(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.solverBone, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
reachRange(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.reachRange, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
reachBone(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.reachBone, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>()),
reachCenter(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.reachCenter, GetMetaContext().StaticCast<aeMCRuleInverseKinematic>())
{
	goalPosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetGoalPosition(goalPosition);
		}
		NotifyRuleChanged();
	});
	
	goalOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetGoalOrientation(
				decQuaternion::CreateFromEuler(goalOrientation.GetValue() * DEG2RAD));
			}
			NotifyRuleChanged();
	});
	
	localPosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetLocalPosition(localPosition);
		}
		NotifyRuleChanged();
	});
	
	localOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetLocalOrientation(
				decQuaternion::CreateFromEuler(localOrientation.GetValue() * DEG2RAD));
		}
		NotifyRuleChanged();
	});
	
	adjustOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetAdjustOrientation(adjustOrientation);
		}
		NotifyRuleChanged();
	});
	
	useSolverBone.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetUseSolverBone(useSolverBone);
		}
		NotifyRuleChanged();
	});
	
	solverBone.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetSolverBone(solverBone);
		}
		NotifyRuleChanged();
	});
	
	reachRange.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetReachRange(reachRange);
		}
		NotifyRuleChanged();
	});
	
	reachBone.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetReachBone(reachBone);
		}
		NotifyRuleChanged();
	});
	
	reachCenter.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleInverseKinematic*)GetEngineRule())->SetReachCenter(reachCenter);
		}
		NotifyRuleChanged();
	});
}

aeRuleInverseKinematic::aeRuleInverseKinematic(aeWindowMain &windowMain, const aeRuleInverseKinematic &copy) :
aeRuleInverseKinematic(windowMain, copy.name)
{
	pInitCopy(copy);
	goalOrientation.SetValue(copy.goalOrientation, false);
	localPosition.SetValue(copy.localPosition, false);
	localOrientation.SetValue(copy.localOrientation, false);
	adjustOrientation.SetValue(copy.adjustOrientation, false);
	useSolverBone.SetValue(copy.useSolverBone, false);
	solverBone.SetValue(copy.solverBone, false);
	reachRange.SetValue(copy.reachRange, false);
	reachBone.SetValue(copy.reachBone, false);
	reachCenter.SetValue(copy.reachCenter, false);
	
	pTargetGoalPosition = aeControllerTarget::Ref::New(copy.pTargetGoalPosition);
	pTargetGoalOrientation = aeControllerTarget::Ref::New(copy.pTargetGoalOrientation);
	pTargetLocalPosition = aeControllerTarget::Ref::New(copy.pTargetLocalPosition);
	pTargetLocalOrientation = aeControllerTarget::Ref::New(copy.pTargetLocalOrientation);
	pTargetReachRange = aeControllerTarget::Ref::New(copy.pTargetReachRange);
	pTargetReachCenter = aeControllerTarget::Ref::New(copy.pTargetReachCenter);
}

aeRuleInverseKinematic::~aeRuleInverseKinematic() = default;


// Management
///////////////

void aeRuleInverseKinematic::SetGoalPosition(const decVector &value){
	goalPosition = value;
}

void aeRuleInverseKinematic::SetGoalOrientation(const decVector &value){
	goalOrientation = value;
}

void aeRuleInverseKinematic::SetLocalPosition(const decVector &value){
	localPosition = value;
}

void aeRuleInverseKinematic::SetLocalOrientation(const decVector &value){
	localOrientation = value;
}

void aeRuleInverseKinematic::SetAdjustOrientation(bool value){
	adjustOrientation = value;
}

void aeRuleInverseKinematic::SetUseSolverBone(bool value){
	useSolverBone = value;
}

void aeRuleInverseKinematic::SetSolverBone(const char *value){
	solverBone = value;
}

void aeRuleInverseKinematic::SetReachRange(float value){
	reachRange = value;
}

void aeRuleInverseKinematic::SetReachBone(const char *value){
	reachBone = value;
}

void aeRuleInverseKinematic::SetReachCenter(const decVector &value){
	reachCenter = value;
}


void aeRuleInverseKinematic::UpdateTargets(){
	auto engRule = static_cast<deAnimatorRuleInverseKinematic*>(GetEngineRule());
	
	aeRule::UpdateTargets();
	
	if(engRule){
		aeAnimator * const animator = GetAnimator();
		
		pTargetGoalPosition->UpdateEngineTarget(animator, engRule->GetTargetGoalPosition());
		pTargetGoalOrientation->UpdateEngineTarget(animator, engRule->GetTargetGoalOrientation());
		pTargetLocalPosition->UpdateEngineTarget(animator, engRule->GetTargetLocalPosition());
		pTargetLocalOrientation->UpdateEngineTarget(animator, engRule->GetTargetLocalOrientation());
		pTargetReachRange->UpdateEngineTarget(animator, engRule->GetTargetReachRange());
		pTargetReachCenter->UpdateEngineTarget(animator, engRule->GetTargetReachCenter());
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
	
	engRule->SetGoalPosition(goalPosition);
	engRule->SetGoalOrientation(decQuaternion::CreateFromEuler(goalOrientation.GetValue() * DEG2RAD));
	engRule->SetLocalPosition(localPosition);
	engRule->SetLocalOrientation(decQuaternion::CreateFromEuler(localOrientation.GetValue() * DEG2RAD));
	engRule->SetAdjustOrientation(adjustOrientation);
	engRule->SetUseSolverBone(useSolverBone);
	engRule->SetSolverBone(solverBone);
	engRule->SetReachRange(reachRange);
	engRule->SetReachCenter(reachCenter);
	engRule->SetReachBone(reachBone);
	
	aeAnimator * const animator = GetAnimator();
	pTargetGoalPosition->UpdateEngineTarget(animator, engRule->GetTargetGoalPosition());
	pTargetGoalOrientation->UpdateEngineTarget(animator, engRule->GetTargetGoalOrientation());
	pTargetLocalPosition->UpdateEngineTarget(animator, engRule->GetTargetLocalPosition());
	pTargetLocalOrientation->UpdateEngineTarget(animator, engRule->GetTargetLocalOrientation());
	pTargetReachRange->UpdateEngineTarget(animator, engRule->GetTargetReachRange());
	pTargetReachCenter->UpdateEngineTarget(animator, engRule->GetTargetReachCenter());
	
	return engRule;
}



aeRule::Ref aeRuleInverseKinematic::CreateCopy() const{
	return Ref::New(*this);
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



// Operators
//////////////

aeRuleInverseKinematic &aeRuleInverseKinematic::operator=(const aeRuleInverseKinematic &copy){
	goalPosition = copy.goalPosition;
	goalOrientation = copy.goalOrientation;
	localPosition = copy.localPosition;
	localOrientation = copy.localOrientation;
	adjustOrientation = copy.adjustOrientation;
	useSolverBone = copy.useSolverBone;
	solverBone = copy.solverBone;
	reachRange = copy.reachRange;
	reachCenter = copy.reachCenter;
	reachBone = copy.reachBone;
	pTargetGoalPosition = copy.pTargetGoalPosition;
	pTargetGoalOrientation = copy.pTargetGoalOrientation;
	pTargetLocalPosition = copy.pTargetLocalPosition;
	pTargetLocalOrientation = copy.pTargetLocalOrientation;
	pTargetReachRange = copy.pTargetReachRange;
	pTargetReachCenter = copy.pTargetReachCenter;
	aeRule::operator=(copy);
	return *this;
}
