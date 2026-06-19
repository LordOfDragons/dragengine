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
enablePosition(windowMain.GetMCAnimatorProperties().ruleGroup.enablePosition, GetMetaContext().StaticCast<aeMCRuleGroup>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleGroup.enableOrientation, GetMetaContext().StaticCast<aeMCRuleGroup>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleGroup.enableSize, GetMetaContext().StaticCast<aeMCRuleGroup>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleGroup.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleGroup>()),
useCurrentState(windowMain.GetMCAnimatorProperties().ruleGroup.useCurrentState, GetMetaContext().StaticCast<aeMCRuleGroup>()),
applicationType(windowMain.GetMCAnimatorProperties().ruleGroup.applicationType, GetMetaContext().StaticCast<aeMCRuleGroup>())
{
	enablePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	});
	
	enableOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	});
	
	enableSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	});
	
	enableVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	});
	
	useCurrentState.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetUseCurrentState(useCurrentState);
		}
		NotifyRuleChanged();
	});
	
	applicationType.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetApplicationType(applicationType);
		}
		NotifyRuleChanged();
	});
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
	pRules.AddOrThrow(rule);
	
	aeAnimator * const animator = GetAnimator();
	
	rule->SetParentGroup(this);
	rule->SetAnimator(animator);
	
	if(animator){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::InsertRuleAt(aeRule *rule, int index){
	pRules.InsertOrThrow(rule, index);
	
	aeAnimator * const animator = GetAnimator();
	rule->SetParentGroup(this);
	rule->SetAnimator(animator);
	
	if(animator){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::MoveRuleTo(aeRule *rule, int index){
	pRules.Move(rule, index);
	
	aeAnimator * const animator = GetAnimator();
	if(animator){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::RemoveRule(aeRule *rule){
	const aeRule::Ref guard(rule);
	const int index = pRules.IndexOf(rule);
	pRules.RemoveOrThrow(rule);
	
	aeAnimator * const animator = GetAnimator();
	
	if(animator && rule == animator->GetActiveRule()){
		if(pRules.IsNotEmpty()){
			animator->SetActiveRule(pRules.GetAt(decMath::min(index, pRules.GetCount() - 1)));
			
		}else{
			animator->SetActiveRule(this);
		}
	}
	
	rule->SetParentGroup(nullptr);
	rule->SetAnimator(nullptr);
	
	if(animator){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::RemoveAllRules(){
	if(pRules.IsEmpty()){
		return;
	}
	
	aeAnimator * const animator = GetAnimator();
	
	if(animator && pRules.Has(animator->GetActiveRule())){
		animator->SetActiveRule(this);
	}
	
	pRules.Visit([&](aeRule &rule){
		rule.SetParentGroup(nullptr);
		rule.SetAnimator(nullptr);
	});
	
	pRules.RemoveAll();
	
	if(animator){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}


void aeRuleGroup::UpdateTargets(){
	aeRule::UpdateTargets();
	
	deAnimatorRuleGroup * const rule = (deAnimatorRuleGroup*)GetEngineRule();
	if(rule){
		pTargetSelect->UpdateEngineTarget(GetAnimator(), rule->GetTargetSelect());
	}
	
	const int count = pRules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((aeRule*)pRules.GetAt(i))->UpdateTargets();
	}
}

int aeRuleGroup::CountLinkUsage(aeLink *link) const{
	int i, usageCount = aeRule::CountLinkUsage(link);
	const int count = pRules.GetCount();
	
	if(pTargetSelect->GetLinks().Has(link)){
		usageCount++;
	}
	
	for(i=0; i<count; i++){
		usageCount += pRules.GetAt(i)->CountLinkUsage(link);
	}
	
	return usageCount;
}

void aeRuleGroup::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetSelect->GetLinks().Has(link)){
		pTargetSelect->RemoveLink(link);
	}
	
	const int count = pRules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRules.GetAt(i)->RemoveLinkFromTargets(link);
	}
	
	aeRule::UpdateTargets();
}

void aeRuleGroup::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetSelect->RemoveAllLinks();
	
	const int count = pRules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRules.GetAt(i)->RemoveLinksFromAllTargets();
	}
	
	aeRule::UpdateTargets();
}


deAnimatorRule::Ref aeRuleGroup::CreateEngineRule(){
	const int count = pRules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRules.GetAt(i)->SetEngineRule(nullptr);
	}
	
	const deAnimatorRuleGroup::Ref engRule(deAnimatorRuleGroup::Ref::New());
	
	InitEngineRule(engRule);
	
	for(i=0; i<count; i++){
		aeRule * const rule = pRules.GetAt(i);
		
		const deAnimatorRule::Ref subEngRule(rule->CreateEngineRule());
		engRule->AddRule(subEngRule);
		rule->SetEngineRule(subEngRule);
	}
	
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	
	engRule->SetUseCurrentState(useCurrentState);
	engRule->SetApplicationType(applicationType);
	
	pTargetSelect->UpdateEngineTarget(GetAnimator(), engRule->GetTargetSelect());
	
	return engRule;
}


void aeRuleGroup::SetTreeListExpanded(bool value){
	pTreeListExpanded = value;
}


aeRule::Ref aeRuleGroup::CreateCopy() const{
	return Ref::New(*this);
}

void aeRuleGroup::ListLinks(aeLink::List &list){
	const int count = pRules.GetCount();
	int i;
	
	aeRule::ListLinks(list);
	
	pTargetSelect->AddLinksToList(list);
	
	for(i=0; i<count; i++){
		pRules.GetAt(i)->ListLinks(list);
	}
}

void aeRuleGroup::OnParentAnimatorChanged(){
	aeAnimator * const animator = GetAnimator();
	const int count = pRules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRules.GetAt(i)->SetAnimator(animator);
	}
}


// Operators
//////////////

aeRuleGroup &aeRuleGroup::operator=(const aeRuleGroup &copy){
	enablePosition = copy.enablePosition;
	enableOrientation = copy.enableOrientation;
	enableSize = copy.enableSize;
	enableVertexPositionSet = copy.enableVertexPositionSet;
	pTreeListExpanded = copy.pTreeListExpanded;
	useCurrentState = copy.useCurrentState;
	applicationType = copy.applicationType;
	pTargetSelect = copy.pTargetSelect;
	
	const int ruleCount = copy.pRules.GetCount();
	aeRule::Ref rule;
	int i;
	
	RemoveAllRules();
	try{
		for(i=0; i<ruleCount; i++){
			rule = copy.pRules.GetAt(i)->CreateCopy();
			AddRule(rule);
			rule->SetParentGroup(this);
			rule = nullptr;
		}
		
	}catch(const deException &){
		throw;
	}
	
	aeRule::operator=(copy);
	return *this;
}



// Private Functions
//////////////////////

void aeRuleGroup::pCleanUp(){
	RemoveAllRules();
}
