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
mpRules(windowMain.GetMCAnimatorProperties().ruleGroup.rules, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleGroup.enablePosition, metaContext),
mpEnableOrientation(windowMain.GetMCAnimatorProperties().ruleGroup.enableOrientation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleGroup.enableSize, metaContext),
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleGroup.enableVertexPositionSet, metaContext),
mpUseCurrentState(windowMain.GetMCAnimatorProperties().ruleGroup.useCurrentState, metaContext),
mpApplicationType(windowMain.GetMCAnimatorProperties().ruleGroup.applicationType, metaContext),
mpTargetSelect(windowMain.GetMCAnimatorProperties().ruleGroup.targetSelect, metaContext)
{	
	uniqueNameRule.SetIsUnique([this](const decString &checkName){
		return !mpRules->HasMatching([&](const aeRule &each){
			return each.GetName() == checkName;
		});
	});
	
	mpRules.onValueChanged = [this](){
		pUpdateRuleIndices();
		if(GetAnimator()){
			GetAnimator()->RebuildRules();
			GetAnimator()->NotifyRuleStructureChanged();
		}
	};
	mpRules.onObjectAdded = [this](aeRule *rule){
		rule->SetParentGroup(this);
		rule->SetAnimator(GetAnimator());
	};
	mpRules.onObjectRemoved = [this](aeRule *rule){
		rule->SetParentGroup(nullptr);
		rule->SetAnimator(nullptr);
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnableOrientation(mpEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpUseCurrentState.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetUseCurrentState(mpUseCurrentState);
		}
		NotifyRuleChanged();
	};
	
	mpApplicationType.onValueChanged = [this](){
		if(GetEngineRule()){
			static_cast<deAnimatorRuleGroup*>(GetEngineRule())->SetApplicationType(mpApplicationType);
		}
		NotifyRuleChanged();
	};
	
	mpTargetSelect.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(static_cast<deAnimatorRuleGroup*>(GetEngineRule())->GetTargetSelect(), mpTargetSelect);
		}
		NotifyRuleChanged();
	};
}

aeRuleGroup::aeRuleGroup(const aeRuleGroup &copy) :
aeRuleGroup(copy.GetWindowMain(), copy.GetName())
{
	pInitCopy(copy);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableOrientation.SetValue(copy.mpEnableOrientation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpEnableVertexPositionSet.SetValue(copy.mpEnableVertexPositionSet, false);
	mpUseCurrentState.SetValue(copy.mpUseCurrentState, false);
	mpApplicationType.SetValue(copy.mpApplicationType, false);
	mpTargetSelect.SetValue(copy.mpTargetSelect, false);
	
	aeRule::List copyRules;
	copy.mpRules->Visit([&](const aeRule &rule){
		copyRules.Add(rule.CreateCopy());
	});
	mpRules.SetValue(copyRules, false);
}

aeRuleGroup::~aeRuleGroup() = default;


// Management
///////////////

void aeRuleGroup::SetEnablePosition(bool value){
	mpEnablePosition = value;
}

void aeRuleGroup::SetEnableOrientation(bool value){
	mpEnableOrientation = value;
}

void aeRuleGroup::SetEnableSize(bool value){
	mpEnableSize = value;
}

void aeRuleGroup::SetEnableVertexPositionSet(bool value){
	mpEnableVertexPositionSet = value;
}

void aeRuleGroup::SetUseCurrentState(bool value){
	mpUseCurrentState = value;
}

void aeRuleGroup::SetApplicationType(deAnimatorRuleGroup::eApplicationTypes value){
	mpApplicationType = value;
}


void aeRuleGroup::AddRule(aeRule *rule){
	auto list = mpRules.GetValue();
	list.AddOrThrow(rule);
	mpRules = list;
}

void aeRuleGroup::InsertRuleAt(aeRule *rule, int index){
	auto list = mpRules.GetValue();
	list.InsertOrThrow(rule, index);
	mpRules = list;
}

void aeRuleGroup::MoveRuleTo(aeRule *rule, int index){
	auto list = mpRules.GetValue();
	list.Move(rule, index);
	mpRules = list;
}

void aeRuleGroup::RemoveRule(aeRule *rule){
	auto list = mpRules.GetValue();
	list.RemoveOrThrow(rule);
	mpRules = list;
}

void aeRuleGroup::RemoveAllRules(){
	mpRules = {};
}


void aeRuleGroup::UpdateTargets(){
	aeRule::UpdateTargets();
	
	auto engRule = static_cast<deAnimatorRuleGroup*>(GetEngineRule());
	if(engRule){
		pUpdateEngineTarget(engRule->GetTargetSelect(), mpTargetSelect);
	}
	
	mpRules->Visit([&](aeRule &rule){
		rule.UpdateTargets();
	});
}

int aeRuleGroup::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(mpTargetSelect->Has(link)){
		usageCount++;
	}
	
	mpRules->Visit([&](const aeRule &rule){
		usageCount += rule.CountLinkUsage(link);
	});
	
	return usageCount;
}

deAnimatorRule::Ref aeRuleGroup::CreateEngineRule(){
	mpRules->Visit([&](aeRule &rule){
		rule.SetEngineRule(nullptr);
	});
	
	auto engRule = deAnimatorRuleGroup::Ref::New();
	
	InitEngineRule(engRule);
	
	mpRules->Visit([&](aeRule &rule){
		auto subEngRule = rule.CreateEngineRule();
		engRule->AddRule(subEngRule);
		rule.SetEngineRule(subEngRule);
	});
	
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableOrientation(mpEnableOrientation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
	
	engRule->SetUseCurrentState(mpUseCurrentState);
	engRule->SetApplicationType(mpApplicationType);
	
	pUpdateEngineTarget(engRule->GetTargetSelect(), mpTargetSelect);
	
	return engRule;
}


void aeRuleGroup::SetTreeListExpanded(bool value){
	pTreeListExpanded = value;
}


aeRule::Ref aeRuleGroup::CreateCopy() const{
	return Ref::New(*this);
}

void aeRuleGroup::OnParentAnimatorChanged(){
	auto animator = GetAnimator();
	mpRules->Visit([&](aeRule &rule){
		rule.SetAnimator(animator);
	});
}


// Private Functions
//////////////////////

void aeRuleGroup::pCleanUp(){
	RemoveAllRules();
}

void aeRuleGroup::pUpdateRuleIndices(){
	mpRules->VisitIndexed([](int i, aeRule &each){
		each.SetIndex(i);
	});
}
