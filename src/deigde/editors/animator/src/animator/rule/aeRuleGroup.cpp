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

aeRuleGroup::MetaContext::Ref aeRuleGroup::CreateMetaContext(aeWindowMain &windowMain, aeRuleGroup *rule){
	return MetaContext::Ref::New("animator.rule_group", "Rule Group", "Rule group properties",
		windowMain.GetMCAnimatorProperties().ruleGroup.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleGroup::aeRuleGroup(aeWindowMain &windowMain, const char *aname) :
aeRuleGroup(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleGroup::aeRuleGroup(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertGroup, aname),
pTreeListExpanded(false),
pMPRules(windowMain.GetMCAnimatorProperties().ruleGroup.rules, metaContext),
pMPEnablePosition(windowMain.GetMCAnimatorProperties().ruleGroup.enablePosition, metaContext),
pMPEnableOrientation(windowMain.GetMCAnimatorProperties().ruleGroup.enableOrientation, metaContext),
pMPEnableSize(windowMain.GetMCAnimatorProperties().ruleGroup.enableSize, metaContext),
pMPEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleGroup.enableVertexPositionSet, metaContext),
pMPUseCurrentState(windowMain.GetMCAnimatorProperties().ruleGroup.useCurrentState, metaContext),
pMPApplicationType(windowMain.GetMCAnimatorProperties().ruleGroup.applicationType, metaContext),
pMPTargetSelect(windowMain.GetMCAnimatorProperties().ruleGroup.targetSelect, metaContext)
{	
	uniqueNameRule.SetIsUnique([this](const decString &checkName){
		return !pMPRules->HasMatching([&](const aeRule &each){
			return each.GetName() == checkName;
		});
	});
	
	pMPRules.onValueChanged = [this](){
		pUpdateRuleIndices();
		if(GetAnimator()){
			GetAnimator()->RebuildRules();
			GetAnimator()->NotifyRuleStructureChanged();
		}
	};
	pMPRules.onObjectAdded = [this](aeRule *rule){
		rule->SetParentGroup(this);
		rule->SetAnimator(GetAnimator());
	};
	pMPRules.onObjectRemoved = [this](aeRule *rule){
		rule->SetParentGroup(nullptr);
		rule->SetAnimator(nullptr);
	};
	
	pMPEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnablePosition(pMPEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnableOrientation(pMPEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnableSize(pMPEnableSize);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pMPUseCurrentState.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetUseCurrentState(pMPUseCurrentState);
		}
		NotifyRuleChanged();
	};
	
	pMPApplicationType.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetApplicationType(pMPApplicationType);
		}
		NotifyRuleChanged();
	};
	
	pTargetSelect = aeControllerTarget::Ref::New(pMPTargetSelect);
	pMPTargetSelect.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(static_cast<deAnimatorRuleGroup*>(GetEngineRule())->GetTargetSelect(), pMPTargetSelect);
		}
		pTargetSelect->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRuleGroup::aeRuleGroup(aeWindowMain &windowMain, const aeRuleGroup &copy) :
aeRuleGroup(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPEnablePosition.SetValue(copy.pMPEnablePosition, false);
	pMPEnableOrientation.SetValue(copy.pMPEnableOrientation, false);
	pMPEnableSize.SetValue(copy.pMPEnableSize, false);
	pMPEnableVertexPositionSet.SetValue(copy.pMPEnableVertexPositionSet, false);
	pMPUseCurrentState.SetValue(copy.pMPUseCurrentState, false);
	pMPApplicationType.SetValue(copy.pMPApplicationType, false);
	pMPTargetSelect.SetValue(copy.pMPTargetSelect, false);
	
	pTargetSelect = aeControllerTarget::Ref::New(pMPTargetSelect, copy.pTargetSelect);
	
	aeRule::List copyRules;
	copy.pMPRules->Visit([&](const aeRule &rule){
		copyRules.Add(rule.CreateCopy(windowMain));
	});
	pMPRules.SetValue(copyRules, false);
}

aeRuleGroup::~aeRuleGroup() = default;


// Management
///////////////

void aeRuleGroup::SetEnablePosition(bool value){
	pMPEnablePosition = value;
}

void aeRuleGroup::SetEnableOrientation(bool value){
	pMPEnableOrientation = value;
}

void aeRuleGroup::SetEnableSize(bool value){
	pMPEnableSize = value;
}

void aeRuleGroup::SetEnableVertexPositionSet(bool value){
	pMPEnableVertexPositionSet = value;
}

void aeRuleGroup::SetUseCurrentState(bool value){
	pMPUseCurrentState = value;
}

void aeRuleGroup::SetApplicationType(deAnimatorRuleGroup::eApplicationTypes value){
	pMPApplicationType = value;
}


void aeRuleGroup::AddRule(aeRule *rule){
	auto list = pMPRules.GetValue();
	list.AddOrThrow(rule);
	pMPRules = list;
}

void aeRuleGroup::InsertRuleAt(aeRule *rule, int index){
	auto list = pMPRules.GetValue();
	list.InsertOrThrow(rule, index);
	pMPRules = list;
}

void aeRuleGroup::MoveRuleTo(aeRule *rule, int index){
	auto list = pMPRules.GetValue();
	list.Move(rule, index);
	pMPRules = list;
}

void aeRuleGroup::RemoveRule(aeRule *rule){
	auto list = pMPRules.GetValue();
	list.RemoveOrThrow(rule);
	pMPRules = list;
}

void aeRuleGroup::RemoveAllRules(){
	pMPRules = {};
}


void aeRuleGroup::UpdateTargets(){
	aeRule::UpdateTargets();
	
	auto engRule = static_cast<deAnimatorRuleGroup*>(GetEngineRule());
	if(engRule){
		pUpdateEngineTarget(engRule->GetTargetSelect(), pMPTargetSelect);
	}
	
	pMPRules->Visit([&](aeRule &rule){
		rule.UpdateTargets();
	});
}

int aeRuleGroup::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetSelect->GetLinks().Has(link)){
		usageCount++;
	}
	
	pMPRules->Visit([&](const aeRule &rule){
		usageCount += rule.CountLinkUsage(link);
	});
	
	return usageCount;
}

void aeRuleGroup::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetSelect->GetLinks().Has(link)){
		pTargetSelect->RemoveLink(link);
	}
	
	pMPRules->Visit([&](aeRule &rule){
		rule.RemoveLinkFromTargets(link);
	});
	
	aeRule::UpdateTargets();
}

void aeRuleGroup::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetSelect->RemoveAllLinks();
	
	pMPRules->Visit([&](aeRule &rule){
		rule.RemoveLinksFromAllTargets();
	});
	
	aeRule::UpdateTargets();
}


deAnimatorRule::Ref aeRuleGroup::CreateEngineRule(){
	pMPRules->Visit([&](aeRule &rule){
		rule.SetEngineRule(nullptr);
	});
	
	auto engRule = deAnimatorRuleGroup::Ref::New();
	
	InitEngineRule(engRule);
	
	pMPRules->Visit([&](aeRule &rule){
		auto subEngRule = rule.CreateEngineRule();
		engRule->AddRule(subEngRule);
		rule.SetEngineRule(subEngRule);
	});
	
	engRule->SetEnablePosition(pMPEnablePosition);
	engRule->SetEnableOrientation(pMPEnableOrientation);
	engRule->SetEnableSize(pMPEnableSize);
	engRule->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
	
	engRule->SetUseCurrentState(pMPUseCurrentState);
	engRule->SetApplicationType(pMPApplicationType);
	
	pUpdateEngineTarget(engRule->GetTargetSelect(), pMPTargetSelect);
	
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
	
	pMPRules->Visit([&](aeRule &rule){
		rule.ListLinks(list);
	});
}

void aeRuleGroup::OnParentAnimatorChanged(){
	auto animator = GetAnimator();
	pMPRules->Visit([&](aeRule &rule){
		rule.SetAnimator(animator);
	});
}


// Private Functions
//////////////////////

void aeRuleGroup::pCleanUp(){
	RemoveAllRules();
}

void aeRuleGroup::pUpdateRuleIndices(){
	pMPRules->VisitIndexed([](int i, aeRule &each){
		each.SetIndex(i);
	});
}
