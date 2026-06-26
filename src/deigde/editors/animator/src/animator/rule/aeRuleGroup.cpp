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

#include "aeRuleGroup.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleGroup
//////////////////////

// Constructor, destructor
////////////////////////////

aeRuleGroup::aeRuleGroup(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleGroup::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertGroup, aname),
pTreeListExpanded(false),
rules(windowMain.GetMCAnimatorProperties().ruleGroup.rules, GetMetaContext().StaticCast<aeMCRuleGroup>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleGroup.enablePosition, GetMetaContext().StaticCast<aeMCRuleGroup>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleGroup.enableOrientation, GetMetaContext().StaticCast<aeMCRuleGroup>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleGroup.enableSize, GetMetaContext().StaticCast<aeMCRuleGroup>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleGroup.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleGroup>()),
useCurrentState(windowMain.GetMCAnimatorProperties().ruleGroup.useCurrentState, GetMetaContext().StaticCast<aeMCRuleGroup>()),
applicationType(windowMain.GetMCAnimatorProperties().ruleGroup.applicationType, GetMetaContext().StaticCast<aeMCRuleGroup>()),
targetSelect(windowMain.GetMCAnimatorProperties().ruleGroup.targetSelect, GetMetaContext().StaticCast<aeMCRuleGroup>())
{	
	uniqueNameRule.SetIsUnique([this](const decString &checkName){
		return !rules->HasMatching([&](const aeRule &each){
			return each.GetName() == checkName;
		});
	});
	
	rules.onValueChanged = [this](){
		pUpdateRuleIndices();
		if(GetAnimator()){
			GetAnimator()->RebuildRules();
			GetAnimator()->NotifyRuleStructureChanged();
		}
	};
	rules.onObjectAdded = [this](aeRule *rule){
		rule->SetParentGroup(this);
		rule->SetAnimator(GetAnimator());
	};
	rules.onObjectRemoved = [this](aeRule *rule){
		rule->SetParentGroup(nullptr);
		rule->SetAnimator(nullptr);
	};
	
	enablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	};
	
	enableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	};
	
	enableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	};
	
	enableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	useCurrentState.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetUseCurrentState(useCurrentState);
		}
		NotifyRuleChanged();
	};
	
	applicationType.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetApplicationType(applicationType);
		}
		NotifyRuleChanged();
	};
	
	pTargetSelect = aeControllerTarget::Ref::New(targetSelect);
	targetSelect.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(static_cast<deAnimatorRuleGroup*>(GetEngineRule())->GetTargetSelect(), targetSelect);
		}
		pTargetSelect->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRuleGroup::aeRuleGroup(aeWindowMain &windowMain, const aeRuleGroup &copy) :
aeRuleGroup(windowMain, copy.name)
{
	pInitCopy(copy);
	enableOrientation.SetValue(copy.enableOrientation, false);
	enableSize.SetValue(copy.enableSize, false);
	enableVertexPositionSet.SetValue(copy.enableVertexPositionSet, false);
	useCurrentState.SetValue(copy.useCurrentState, false);
	applicationType.SetValue(copy.applicationType, false);
	
	pTargetSelect = aeControllerTarget::Ref::New(targetSelect, copy.pTargetSelect);
	
	aeRule::List copyRules;
	copy.rules->Visit([&](const aeRule &rule){
		copyRules.Add(rule.CreateCopy(windowMain));
	});
	rules.SetValue(copyRules);
}

aeRuleGroup::~aeRuleGroup() = default;


// Management
///////////////

void aeRuleGroup::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleGroup::SetEnableOrientation(bool value){
	enableOrientation = value;
}

void aeRuleGroup::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleGroup::SetEnableVertexPositionSet(bool value){
	enableVertexPositionSet = value;
}

void aeRuleGroup::SetUseCurrentState(bool value){
	useCurrentState = value;
}

void aeRuleGroup::SetApplicationType(deAnimatorRuleGroup::eApplicationTypes value){
	applicationType = value;
}


void aeRuleGroup::AddRule(aeRule *rule){
	auto list = rules.GetValue();
	list.AddOrThrow(rule);
	rules = list;
}

void aeRuleGroup::InsertRuleAt(aeRule *rule, int index){
	auto list = rules.GetValue();
	list.InsertOrThrow(rule, index);
	rules = list;
}

void aeRuleGroup::MoveRuleTo(aeRule *rule, int index){
	auto list = rules.GetValue();
	list.Move(rule, index);
	rules = list;
}

void aeRuleGroup::RemoveRule(aeRule *rule){
	auto list = rules.GetValue();
	list.RemoveOrThrow(rule);
	rules = list;
}

void aeRuleGroup::RemoveAllRules(){
	rules = {};
}


void aeRuleGroup::UpdateTargets(){
	aeRule::UpdateTargets();
	
	auto engRule = static_cast<deAnimatorRuleGroup*>(GetEngineRule());
	if(engRule){
		pUpdateEngineTarget(engRule->GetTargetSelect(), targetSelect);
	}
	
	rules->Visit([&](aeRule &rule){
		rule.UpdateTargets();
	});
}

int aeRuleGroup::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetSelect->GetLinks().Has(link)){
		usageCount++;
	}
	
	rules->Visit([&](const aeRule &rule){
		usageCount += rule.CountLinkUsage(link);
	});
	
	return usageCount;
}

void aeRuleGroup::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetSelect->GetLinks().Has(link)){
		pTargetSelect->RemoveLink(link);
	}
	
	rules->Visit([&](aeRule &rule){
		rule.RemoveLinkFromTargets(link);
	});
	
	aeRule::UpdateTargets();
}

void aeRuleGroup::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetSelect->RemoveAllLinks();
	
	rules->Visit([&](aeRule &rule){
		rule.RemoveLinksFromAllTargets();
	});
	
	aeRule::UpdateTargets();
}


deAnimatorRule::Ref aeRuleGroup::CreateEngineRule(){
	rules->Visit([&](aeRule &rule){
		rule.SetEngineRule(nullptr);
	});
	
	auto engRule = deAnimatorRuleGroup::Ref::New();
	
	InitEngineRule(engRule);
	
	rules->Visit([&](aeRule &rule){
		auto subEngRule = rule.CreateEngineRule();
		engRule->AddRule(subEngRule);
		rule.SetEngineRule(subEngRule);
	});
	
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	
	engRule->SetUseCurrentState(useCurrentState);
	engRule->SetApplicationType(applicationType);
	
	pUpdateEngineTarget(engRule->GetTargetSelect(), targetSelect);
	
	return engRule;
}


void aeRuleGroup::SetTreeListExpanded(bool value){
	pTreeListExpanded = value;
}


aeRule::Ref aeRuleGroup::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleGroup::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	
	pTargetSelect->AddLinksToList(list);
	
	rules->Visit([&](aeRule &rule){
		rule.ListLinks(list);
	});
}

void aeRuleGroup::OnParentAnimatorChanged(){
	auto animator = GetAnimator();
	rules->Visit([&](aeRule &rule){
		rule.SetAnimator(animator);
	});
}


// Private Functions
//////////////////////

void aeRuleGroup::pCleanUp(){
	RemoveAllRules();
}

void aeRuleGroup::pUpdateRuleIndices(){
	rules.GetValue().VisitIndexed([](int i, aeRule &each){
		each.SetIndex(i);
	});
}
