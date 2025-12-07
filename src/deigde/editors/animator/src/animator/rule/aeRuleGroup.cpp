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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeRuleGroup.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleGroup
//////////////////////

// Constructor, destructor
////////////////////////////

aeRuleGroup::aeRuleGroup() :
aeRule(deAnimatorRuleVisitorIdentify::ertGroup),
pEnablePosition(true),
pEnableOrientation(true),
pEnableSize(false),
pEnableVertexPositionSet(true),
pUseCurrentState(false),
pApplicationType(deAnimatorRuleGroup::eatAll),
pTreeListExpanded(false)
{
	SetName("Group");
}

aeRuleGroup::aeRuleGroup(const aeRuleGroup &copy) :
aeRule(copy),
pEnablePosition(copy.pEnablePosition),
pEnableOrientation(copy.pEnableOrientation),
pEnableSize(copy.pEnableSize),
pEnableVertexPositionSet(copy.pEnableVertexPositionSet),
pUseCurrentState(copy.pUseCurrentState),
pApplicationType(copy.pApplicationType),
pTargetSelect(copy.pTargetSelect),
pTreeListExpanded(copy.pTreeListExpanded)
{
	const int ruleCount = copy.pRules.GetCount();
	aeRule *rule = NULL;
	int i;
	
	try{
		for(i=0; i<ruleCount; i++){
			rule = copy.pRules.GetAt(i)->CreateCopy();
			pRules.Add(rule);
			rule->SetParentGroup(this);
			rule->FreeReference();
			rule = NULL;
		}
		
	}catch(const deException &){
		if(rule){
			rule->FreeReference();
		}
		throw;
	}
}

aeRuleGroup::~aeRuleGroup(){
	pCleanUp();
}



// Management
///////////////

void aeRuleGroup::AddRule(aeRule *rule){
	pRules.Add(rule);
	
	aeAnimator * const animator = GetAnimator();
	
	rule->SetParentGroup(this);
	rule->SetAnimator(animator);
	
	if(animator){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::InsertRuleAt(aeRule *rule, int index){
	pRules.Insert(rule, index);
	
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
	if(! pRules.Has(rule)){
		DETHROW(deeInvalidParam);
	}
	
	aeAnimator * const animator = GetAnimator();
	
	if(animator && rule == animator->GetActiveRule()){
		const int index = pRules.IndexOf(rule);
		const int ruleCount = pRules.GetCount();
		
		if(ruleCount == 1){
			animator->SetActiveRule(this);
			
		}else if(index < ruleCount - 1){
			animator->SetActiveRule(pRules.GetAt(index + 1));
			
		}else{
			animator->SetActiveRule(pRules.GetAt(index - 1));
		}
	}
	
	rule->SetParentGroup(NULL);
	rule->SetAnimator(NULL);
	
	pRules.Remove(rule);
	
	if(animator){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::RemoveAllRules(){
	const int count = pRules.GetCount();
	int i;
	
	aeAnimator * const animator = GetAnimator();
	
	if(animator && pRules.Has(animator->GetActiveRule())){
		animator->SetActiveRule(this);
	}
	
	for(i=0; i<count; i++){
		aeRule * const rule = pRules.GetAt(i);
		rule->SetParentGroup(NULL);
		rule->SetAnimator(NULL);
	}
	
	pRules.RemoveAll();
	
	if(animator){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}



void aeRuleGroup::SetEnablePosition(bool enabled){
	if(enabled != pEnablePosition){
		pEnablePosition = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetEnablePosition(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleGroup::SetEnableOrientation(bool enabled){
	if(enabled != pEnableOrientation){
		pEnableOrientation = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetEnableOrientation(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleGroup::SetEnableSize(bool enabled){
	if(enabled != pEnableSize){
		pEnableSize = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetEnableSize(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleGroup::SetEnableVertexPositionSet(bool enabled){
	if(enabled != pEnableVertexPositionSet){
		pEnableVertexPositionSet = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetEnableVertexPositionSet(enabled);
			NotifyRuleChanged();
		}
	}
}



void aeRuleGroup::SetUseCurrentState(bool useCurrentState){
	if(useCurrentState == pUseCurrentState){
		return;
	}
	
	pUseCurrentState = useCurrentState;
	
	if(GetEngineRule()){
		((deAnimatorRuleGroup*)GetEngineRule())->SetUseCurrentState(useCurrentState);
		NotifyRuleChanged();
	}
}

void aeRuleGroup::SetApplicationType(deAnimatorRuleGroup::eApplicationTypes type){
	if(type != pApplicationType){
		pApplicationType = type;
		
		if(GetEngineRule()){
			((deAnimatorRuleGroup*)GetEngineRule())->SetApplicationType(type);
			NotifyRuleChanged();
		}
	}
}



void aeRuleGroup::UpdateTargets(){
	aeRule::UpdateTargets();
	
	deAnimatorRuleGroup * const rule = (deAnimatorRuleGroup*)GetEngineRule();
	if(rule){
		pTargetSelect.UpdateEngineTarget(GetAnimator(), rule->GetTargetSelect());
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
	
	if(pTargetSelect.HasLink(link)){
		usageCount++;
	}
	
	for(i=0; i<count; i++){
		usageCount += pRules.GetAt(i)->CountLinkUsage(link);
	}
	
	return usageCount;
}

void aeRuleGroup::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetSelect.HasLink(link)){
		pTargetSelect.RemoveLink(link);
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
	
	pTargetSelect.RemoveAllLinks();
	
	const int count = pRules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRules.GetAt(i)->RemoveLinksFromAllTargets();
	}
	
	aeRule::UpdateTargets();
}



deAnimatorRule *aeRuleGroup::CreateEngineRule(){
	deAnimatorRuleGroup *engRule = NULL;
	deAnimatorRule *subEngRule = NULL;
	const int count = pRules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRules.GetAt(i)->SetEngineRule(NULL);
	}
	
	try{
		engRule = new deAnimatorRuleGroup;
		
		InitEngineRule(engRule);
		
		for(i=0; i<count; i++){
			aeRule * const rule = pRules.GetAt(i);
			
			subEngRule = rule->CreateEngineRule();
			engRule->AddRule(subEngRule);
			rule->SetEngineRule(subEngRule);
			subEngRule->FreeReference();
			subEngRule = NULL;
		}
		
		engRule->SetEnablePosition(pEnablePosition);
		engRule->SetEnableOrientation(pEnableOrientation);
		engRule->SetEnableSize(pEnableSize);
		engRule->SetEnableVertexPositionSet(pEnableVertexPositionSet);
		
		engRule->SetUseCurrentState(pUseCurrentState);
		engRule->SetApplicationType(pApplicationType);
		
		pTargetSelect.UpdateEngineTarget(GetAnimator(), engRule->GetTargetSelect());
		
	}catch(const deException &){
		if(subEngRule){
			subEngRule->FreeReference();
		}
		if(engRule){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



void aeRuleGroup::SetTreeListExpanded(bool expanded){
	pTreeListExpanded = expanded;
}



aeRule *aeRuleGroup::CreateCopy() const{
	return new aeRuleGroup(*this);
}

void aeRuleGroup::ListLinks(aeLinkList &list){
	const int count = pRules.GetCount();
	int i;
	
	aeRule::ListLinks(list);
	
	pTargetSelect.AddLinksToList(list);
	
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
	SetEnablePosition(copy.pEnablePosition);
	SetEnableOrientation(copy.pEnableOrientation);
	SetEnableSize(copy.pEnableSize);
	SetEnableVertexPositionSet(copy.pEnableVertexPositionSet);
	SetTreeListExpanded(copy.pTreeListExpanded);
	SetUseCurrentState(copy.pUseCurrentState);
	SetApplicationType(copy.pApplicationType);
	pTargetSelect = copy.pTargetSelect;
	
	const int ruleCount = copy.pRules.GetCount();
	aeRule *rule = NULL;
	int i;
	
	RemoveAllRules();
	try{
		for(i=0; i<ruleCount; i++){
			rule = copy.pRules.GetAt(i)->CreateCopy();
			AddRule(rule);
			rule->SetParentGroup(this);
			rule->FreeReference();
			rule = NULL;
		}
		
	}catch(const deException &){
		if(rule){
			rule->FreeReference();
		}
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
