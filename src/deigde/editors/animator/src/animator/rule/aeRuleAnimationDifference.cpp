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

#include "aeRuleAnimationDifference.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationDifference.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleAnimationDifference
////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleAnimationDifference::aeRuleAnimationDifference(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleAnimationDifference::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertAnimationDifference, aname),
pMPLeadingMoveName(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.leadingMoveName, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPLeadingMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.leadingMoveTime, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPReferenceMoveName(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.referenceMoveName, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPReferenceMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.referenceMoveTime, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPUseComponentSpace(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.useComponentSpace, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPEnablePosition(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enablePosition, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPEnableOrientation(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enableOrientation, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPEnableSize(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enableSize, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPTargetLeadMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.targetLeadMoveTime, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
pMPTargetRefMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.targetRefMoveTime, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>())
{
	pMPLeadingMoveName.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetLeadingMoveName(pMPLeadingMoveName);
		}
		NotifyRuleChanged();
	};
	
	pMPLeadingMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetLeadingMoveTime(pMPLeadingMoveTime);
		}
		NotifyRuleChanged();
	};
	
	pMPReferenceMoveName.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetReferenceMoveName(pMPReferenceMoveName);
		}
		NotifyRuleChanged();
	};
	
	pMPReferenceMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetReferenceMoveTime(pMPReferenceMoveTime);
		}
		NotifyRuleChanged();
	};
	
	pMPUseComponentSpace.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetUseComponentSpace(pMPUseComponentSpace);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnablePosition(pMPEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnableOrientation(pMPEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnableSize(pMPEnableSize);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pTargetLeadMoveTime = aeControllerTarget::Ref::New(pMPTargetLeadMoveTime);
	pMPTargetLeadMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimationDifference*)GetEngineRule())->GetTargetLeadingMoveTime(), pMPTargetLeadMoveTime);
		}
		pTargetLeadMoveTime->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetRefMoveTime = aeControllerTarget::Ref::New(pMPTargetRefMoveTime);
	pMPTargetRefMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimationDifference*)GetEngineRule())->GetTargetReferenceMoveTime(), pMPTargetRefMoveTime);
		}
		pTargetRefMoveTime->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRuleAnimationDifference::aeRuleAnimationDifference(aeWindowMain &windowMain, const aeRuleAnimationDifference &copy) :
aeRuleAnimationDifference(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPLeadingMoveTime.SetValue(copy.pMPLeadingMoveTime, false);
	pMPReferenceMoveName.SetValue(copy.pMPReferenceMoveName, false);
	pMPReferenceMoveTime.SetValue(copy.pMPReferenceMoveTime, false);
	pMPUseComponentSpace.SetValue(copy.pMPUseComponentSpace, false);
	pMPEnablePosition.SetValue(copy.pMPEnablePosition, false);
	pMPEnableOrientation.SetValue(copy.pMPEnableOrientation, false);
	pMPEnableSize.SetValue(copy.pMPEnableSize, false);
	pMPEnableVertexPositionSet.SetValue(copy.pMPEnableVertexPositionSet, false);
	
	pTargetLeadMoveTime = aeControllerTarget::Ref::New(pMPTargetLeadMoveTime, copy.pTargetLeadMoveTime);
	pTargetRefMoveTime = aeControllerTarget::Ref::New(pMPTargetRefMoveTime, copy.pTargetRefMoveTime);
}

aeRuleAnimationDifference::~aeRuleAnimationDifference() = default;


// Management
///////////////

void aeRuleAnimationDifference::SetLeadingMoveName(const char *value){
	pMPLeadingMoveName = value;
}

void aeRuleAnimationDifference::SetLeadingMoveTime(float value){
	pMPLeadingMoveTime = value;
}

void aeRuleAnimationDifference::SetReferenceMoveName(const char *value){
	pMPReferenceMoveName = value;
}

void aeRuleAnimationDifference::SetReferenceMoveTime(float value){
	pMPReferenceMoveTime = value;
}

void aeRuleAnimationDifference::SetUseComponentSpace(bool value){
	pMPUseComponentSpace = value;
}

void aeRuleAnimationDifference::SetEnablePosition(bool value){
	pMPEnablePosition = value;
}

void aeRuleAnimationDifference::SetEnableOrientation(bool value){
	pMPEnableOrientation = value;
}

void aeRuleAnimationDifference::SetEnableSize(bool value){
	pMPEnableSize = value;
}

void aeRuleAnimationDifference::SetEnableVertexPositionSet(bool value){
	pMPEnableVertexPositionSet = value;
}


void aeRuleAnimationDifference::UpdateTargets(){
	deAnimatorRuleAnimationDifference *rule = (deAnimatorRuleAnimationDifference*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetLeadingMoveTime(), pMPTargetLeadMoveTime);
		pUpdateEngineTarget(rule->GetTargetReferenceMoveTime(), pMPTargetRefMoveTime);
	}
}

int aeRuleAnimationDifference::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetLeadMoveTime->GetLinks().Has(link)) usageCount++;
	if(pTargetRefMoveTime->GetLinks().Has(link)) usageCount++;
	
	return usageCount;
}

void aeRuleAnimationDifference::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetLeadMoveTime->GetLinks().Has(link)){
		pTargetLeadMoveTime->RemoveLink(link);
	}
	
	if(pTargetRefMoveTime->GetLinks().Has(link)){
		pTargetRefMoveTime->RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleAnimationDifference::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetLeadMoveTime->RemoveAllLinks();
	pTargetRefMoveTime->RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule::Ref aeRuleAnimationDifference::CreateEngineRule(){
	// create rule
	const deAnimatorRuleAnimationDifference::Ref engRule(deAnimatorRuleAnimationDifference::Ref::New());
	
	// init rule
	InitEngineRule(engRule);
	
	engRule->SetLeadingMoveName(pMPLeadingMoveName);
	engRule->SetLeadingMoveTime(pMPLeadingMoveTime);
	engRule->SetReferenceMoveName(pMPReferenceMoveName);
	engRule->SetReferenceMoveTime(pMPReferenceMoveTime);
	engRule->SetEnablePosition(pMPEnablePosition);
	engRule->SetEnableOrientation(pMPEnableOrientation);
	engRule->SetEnableSize(pMPEnableSize);
	engRule->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
	engRule->SetUseComponentSpace(pMPUseComponentSpace);
	
	pUpdateEngineTarget(engRule->GetTargetLeadingMoveTime(), pMPTargetLeadMoveTime);
	pUpdateEngineTarget(engRule->GetTargetReferenceMoveTime(), pMPTargetRefMoveTime);
	
	// finished
	return engRule;
}



aeRule::Ref aeRuleAnimationDifference::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleAnimationDifference::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetLeadMoveTime->AddLinksToList(list);
	pTargetRefMoveTime->AddLinksToList(list);
}
