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

// Constructor, destructor
////////////////////////////

aeRuleStateManipulator::aeRuleStateManipulator(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleStateManipulator::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertStateManipulator, aname),
minPosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minPosition, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
maxPosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxPosition, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
minRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minRotation, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
maxRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxRotation, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
minSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minSize, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
maxSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxSize, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
minVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
maxVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enablePosition, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
enableRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enableRotation, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enableSize, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
targetPosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetPosition, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
targetRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetRotation, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
targetSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetSize, GetMetaContext().StaticCast<aeMCRuleStateManipulator>()),
targetVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleStateManipulator>())
{
	minPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumPosition(minPosition);
		}
		NotifyRuleChanged();
	};
	
	maxPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumPosition(maxPosition);
		}
		NotifyRuleChanged();
	};
	
	minRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumRotation(minRotation.GetValue() * DEG2RAD);
		}
		NotifyRuleChanged();
	};
	
	maxRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumRotation(maxRotation.GetValue() * DEG2RAD);
		}
		NotifyRuleChanged();
	};
	
	minSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumSize(minSize);
		}
		NotifyRuleChanged();
	};
	
	maxSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumSize(maxSize);
		}
		NotifyRuleChanged();
	};
	
	minVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumVertexPositionSet(minVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	maxVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumVertexPositionSet(maxVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	enablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	};
	
	enableRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnableRotation(enableRotation);
		}
		NotifyRuleChanged();
	};
	
	enableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	};
	
	enableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pTargetPosition = aeControllerTarget::Ref::New(targetPosition);
	targetPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetPosition(), targetPosition);
		}
		pTargetPosition->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetRotation = aeControllerTarget::Ref::New(targetRotation);
	targetRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetRotation(), targetRotation);
		}
		pTargetRotation->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetSize = aeControllerTarget::Ref::New(targetSize);
	targetSize.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetSize(), targetSize);
		}
		pTargetSize->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetVertexPositionSet = aeControllerTarget::Ref::New(targetVertexPositionSet);
	targetVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetVertexPositionSet(), targetVertexPositionSet);
		}
		pTargetVertexPositionSet->OnStorageChanged();
		NotifyRuleChanged();
	};

}
aeRuleStateManipulator::aeRuleStateManipulator(aeWindowMain &windowMain, const aeRuleStateManipulator &copy) :
aeRuleStateManipulator(windowMain, copy.name)
{
	pInitCopy(copy);
	maxPosition.SetValue(copy.maxPosition, false);
	minRotation.SetValue(copy.minRotation, false);
	maxRotation.SetValue(copy.maxRotation, false);
	minSize.SetValue(copy.minSize, false);
	maxSize.SetValue(copy.maxSize, false);
	minVertexPositionSet.SetValue(copy.minVertexPositionSet, false);
	maxVertexPositionSet.SetValue(copy.maxVertexPositionSet, false);
	enablePosition.SetValue(copy.enablePosition, false);
	enableRotation.SetValue(copy.enableRotation, false);
	enableSize.SetValue(copy.enableSize, false);
	enableVertexPositionSet.SetValue(copy.enableVertexPositionSet, false);
	
	pTargetPosition = aeControllerTarget::Ref::New(targetPosition, copy.pTargetPosition);
	pTargetRotation = aeControllerTarget::Ref::New(targetRotation, copy.pTargetRotation);
	pTargetSize = aeControllerTarget::Ref::New(targetSize, copy.pTargetSize);
	pTargetVertexPositionSet = aeControllerTarget::Ref::New(targetVertexPositionSet, copy.pTargetVertexPositionSet);
}

aeRuleStateManipulator::~aeRuleStateManipulator() = default;

// Management
///////////////


void aeRuleStateManipulator::SetMinimumPosition(const decVector &value){
	minPosition = value;
}

void aeRuleStateManipulator::SetMaximumPosition(const decVector &value){
	maxPosition = value;
}

void aeRuleStateManipulator::SetMinimumRotation(const decVector &value){
	minRotation = value;
}

void aeRuleStateManipulator::SetMaximumRotation(const decVector &value){
	maxRotation = value;
}

void aeRuleStateManipulator::SetMinimumSize(const decVector &value){
	minSize = value;
}

void aeRuleStateManipulator::SetMaximumSize(const decVector &value){
	maxSize = value;
}

void aeRuleStateManipulator::SetMinimumVertexPositionSet(float value){
	minVertexPositionSet = value;
}

void aeRuleStateManipulator::SetMaximumVertexPositionSet(float value){
	maxVertexPositionSet = value;
}

void aeRuleStateManipulator::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleStateManipulator::SetEnableRotation(bool value){
	enableRotation = value;
}

void aeRuleStateManipulator::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleStateManipulator::SetEnableVertexPositionSet(bool value){
	enableVertexPositionSet = value;
}


void aeRuleStateManipulator::UpdateTargets(){
	deAnimatorRuleStateManipulator *rule = (deAnimatorRuleStateManipulator*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetPosition(), targetPosition);
		pUpdateEngineTarget(rule->GetTargetRotation(), targetRotation);
		pUpdateEngineTarget(rule->GetTargetSize(), targetSize);
		pUpdateEngineTarget(rule->GetTargetVertexPositionSet(), targetVertexPositionSet);
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
	
	engRule->SetMinimumPosition(minPosition);
	engRule->SetMaximumPosition(maxPosition);
	engRule->SetMinimumRotation(minRotation.GetValue() * DEG2RAD);
	engRule->SetMaximumRotation(maxRotation.GetValue() * DEG2RAD);
	engRule->SetMinimumSize(minSize);
	engRule->SetMaximumSize(maxSize);
	engRule->SetMinimumVertexPositionSet(minVertexPositionSet);
	engRule->SetMaximumVertexPositionSet(maxVertexPositionSet);
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableRotation(enableRotation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	
	pUpdateEngineTarget(engRule->GetTargetPosition(), targetPosition);
	pUpdateEngineTarget(engRule->GetTargetRotation(), targetRotation);
	pUpdateEngineTarget(engRule->GetTargetSize(), targetSize);
	pUpdateEngineTarget(engRule->GetTargetVertexPositionSet(), targetVertexPositionSet);
	
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
