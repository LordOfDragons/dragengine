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

#include "aeRuleStateManipulator.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>


// Class aeRuleStateManipulator
/////////////////////////////////

aeRuleStateManipulator::MetaContext::Ref aeRuleStateManipulator::CreateMetaContext(aeWindowMain &windowMain, aeRuleStateManipulator *rule){
	return MetaContext::Ref::New("animator.rule_state_manipulator", "Rule State Manipulator", "Rule state manipulator properties",
		windowMain.GetMCAnimatorProperties().ruleStateManipulator.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleStateManipulator::aeRuleStateManipulator(aeWindowMain &windowMain, const char *aname) :
aeRuleStateManipulator(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleStateManipulator::aeRuleStateManipulator(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertStateManipulator, aname),
pMPMinPosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minPosition, metaContext),
pMPMaxPosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxPosition, metaContext),
pMPMinRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minRotation, metaContext),
pMPMaxRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxRotation, metaContext),
pMPMinSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minSize, metaContext),
pMPMaxSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxSize, metaContext),
pMPMinVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minVertexPositionSet, metaContext),
pMPMaxVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxVertexPositionSet, metaContext),
pMPEnablePosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enablePosition, metaContext),
pMPEnableRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enableRotation, metaContext),
pMPEnableSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enableSize, metaContext),
pMPEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enableVertexPositionSet, metaContext),
pMPTargetPosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetPosition, metaContext),
pMPTargetRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetRotation, metaContext),
pMPTargetSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetSize, metaContext),
pMPTargetVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetVertexPositionSet, metaContext)
{
	pMPMinPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumPosition(pMPMinPosition);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumPosition(pMPMaxPosition);
		}
		NotifyRuleChanged();
	};
	
	pMPMinRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumRotation(pMPMinRotation.GetEulerAnglesRadians());
		}
		NotifyRuleChanged();
	};
	
	pMPMaxRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumRotation(pMPMaxRotation.GetEulerAnglesRadians());
		}
		NotifyRuleChanged();
	};
	
	pMPMinSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumSize(pMPMinSize);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumSize(pMPMaxSize);
		}
		NotifyRuleChanged();
	};
	
	pMPMinVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumVertexPositionSet(pMPMinVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumVertexPositionSet(pMPMaxVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnablePosition(pMPEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnableRotation(pMPEnableRotation);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnableSize(pMPEnableSize);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pTargetPosition = aeControllerTarget::Ref::New(pMPTargetPosition);
	pMPTargetPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetPosition(), pMPTargetPosition);
		}
		pTargetPosition->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetRotation = aeControllerTarget::Ref::New(pMPTargetRotation);
	pMPTargetRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetRotation(), pMPTargetRotation);
		}
		pTargetRotation->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetSize = aeControllerTarget::Ref::New(pMPTargetSize);
	pMPTargetSize.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetSize(), pMPTargetSize);
		}
		pTargetSize->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetVertexPositionSet = aeControllerTarget::Ref::New(pMPTargetVertexPositionSet);
	pMPTargetVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetVertexPositionSet(), pMPTargetVertexPositionSet);
		}
		pTargetVertexPositionSet->OnStorageChanged();
		NotifyRuleChanged();
	};

}
aeRuleStateManipulator::aeRuleStateManipulator(aeWindowMain &windowMain, const aeRuleStateManipulator &copy) :
aeRuleStateManipulator(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPMinPosition.SetValue(copy.pMPMinPosition, false);
	pMPMaxPosition.SetValue(copy.pMPMaxPosition, false);
	pMPMinRotation.SetValue(copy.pMPMinRotation, false);
	pMPMaxRotation.SetValue(copy.pMPMaxRotation, false);
	pMPMinSize.SetValue(copy.pMPMinSize, false);
	pMPMaxSize.SetValue(copy.pMPMaxSize, false);
	pMPMinVertexPositionSet.SetValue(copy.pMPMinVertexPositionSet, false);
	pMPMaxVertexPositionSet.SetValue(copy.pMPMaxVertexPositionSet, false);
	pMPEnablePosition.SetValue(copy.pMPEnablePosition, false);
	pMPEnableRotation.SetValue(copy.pMPEnableRotation, false);
	pMPEnableSize.SetValue(copy.pMPEnableSize, false);
	pMPEnableVertexPositionSet.SetValue(copy.pMPEnableVertexPositionSet, false);
	
	pTargetPosition = aeControllerTarget::Ref::New(pMPTargetPosition, copy.pTargetPosition);
	pTargetRotation = aeControllerTarget::Ref::New(pMPTargetRotation, copy.pTargetRotation);
	pTargetSize = aeControllerTarget::Ref::New(pMPTargetSize, copy.pTargetSize);
	pTargetVertexPositionSet = aeControllerTarget::Ref::New(pMPTargetVertexPositionSet, copy.pTargetVertexPositionSet);
}

aeRuleStateManipulator::~aeRuleStateManipulator() = default;

// Management
///////////////


void aeRuleStateManipulator::SetMinimumPosition(const decVector &value){
	pMPMinPosition = value;
}

void aeRuleStateManipulator::SetMaximumPosition(const decVector &value){
	pMPMaxPosition = value;
}

void aeRuleStateManipulator::SetMinimumRotation(const decVector &value){
	pMPMinRotation = value;
}

void aeRuleStateManipulator::SetMaximumRotation(const decVector &value){
	pMPMaxRotation = value;
}

void aeRuleStateManipulator::SetMinimumSize(const decVector &value){
	pMPMinSize = value;
}

void aeRuleStateManipulator::SetMaximumSize(const decVector &value){
	pMPMaxSize = value;
}

void aeRuleStateManipulator::SetMinimumVertexPositionSet(float value){
	pMPMinVertexPositionSet = value;
}

void aeRuleStateManipulator::SetMaximumVertexPositionSet(float value){
	pMPMaxVertexPositionSet = value;
}

void aeRuleStateManipulator::SetEnablePosition(bool value){
	pMPEnablePosition = value;
}

void aeRuleStateManipulator::SetEnableRotation(bool value){
	pMPEnableRotation = value;
}

void aeRuleStateManipulator::SetEnableSize(bool value){
	pMPEnableSize = value;
}

void aeRuleStateManipulator::SetEnableVertexPositionSet(bool value){
	pMPEnableVertexPositionSet = value;
}


void aeRuleStateManipulator::UpdateTargets(){
	deAnimatorRuleStateManipulator *rule = (deAnimatorRuleStateManipulator*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetPosition(), pMPTargetPosition);
		pUpdateEngineTarget(rule->GetTargetRotation(), pMPTargetRotation);
		pUpdateEngineTarget(rule->GetTargetSize(), pMPTargetSize);
		pUpdateEngineTarget(rule->GetTargetVertexPositionSet(), pMPTargetVertexPositionSet);
	}
}

int aeRuleStateManipulator::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetPosition->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetRotation->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetSize->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetVertexPositionSet->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleStateManipulator::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetPosition->GetLinks().Has(link)){
		pTargetPosition->RemoveLink(link);
	}
	if(pTargetRotation->GetLinks().Has(link)){
		pTargetRotation->RemoveLink(link);
	}
	if(pTargetSize->GetLinks().Has(link)){
		pTargetSize->RemoveLink(link);
	}
	if(pTargetVertexPositionSet->GetLinks().Has(link)){
		pTargetVertexPositionSet->RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleStateManipulator::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetPosition->RemoveAllLinks();
	pTargetRotation->RemoveAllLinks();
	pTargetSize->RemoveAllLinks();
	pTargetVertexPositionSet->RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule::Ref aeRuleStateManipulator::CreateEngineRule(){
	const deAnimatorRuleStateManipulator::Ref engRule(deAnimatorRuleStateManipulator::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMinimumPosition(pMPMinPosition);
	engRule->SetMaximumPosition(pMPMaxPosition);
	engRule->SetMinimumRotation(pMPMinRotation.GetEulerAnglesRadians());
	engRule->SetMaximumRotation(pMPMaxRotation.GetEulerAnglesRadians());
	engRule->SetMinimumSize(pMPMinSize);
	engRule->SetMaximumSize(pMPMaxSize);
	engRule->SetMinimumVertexPositionSet(pMPMinVertexPositionSet);
	engRule->SetMaximumVertexPositionSet(pMPMaxVertexPositionSet);
	engRule->SetEnablePosition(pMPEnablePosition);
	engRule->SetEnableRotation(pMPEnableRotation);
	engRule->SetEnableSize(pMPEnableSize);
	engRule->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
	
	pUpdateEngineTarget(engRule->GetTargetPosition(), pMPTargetPosition);
	pUpdateEngineTarget(engRule->GetTargetRotation(), pMPTargetRotation);
	pUpdateEngineTarget(engRule->GetTargetSize(), pMPTargetSize);
	pUpdateEngineTarget(engRule->GetTargetVertexPositionSet(), pMPTargetVertexPositionSet);
	
	return engRule;
}



aeRule::Ref aeRuleStateManipulator::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleStateManipulator::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetPosition->AddLinksToList(list);
	pTargetRotation->AddLinksToList(list);
	pTargetSize->AddLinksToList(list);
	pTargetVertexPositionSet->AddLinksToList(list);
}
