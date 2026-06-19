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
pTargetLeadMoveTime(aeControllerTarget::Ref::New()),
pTargetRefMoveTime(aeControllerTarget::Ref::New()),
leadingMoveName(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.leadingMoveName, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
leadingMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.leadingMoveTime, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
referenceMoveName(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.referenceMoveName, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
referenceMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.referenceMoveTime, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
useSameMove(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.useSameMove, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
useComponentSpace(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.useComponentSpace, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enablePosition, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enableOrientation, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enableSize, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleAnimationDifference>())
{
	leadingMoveName.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetLeadingMoveName(leadingMoveName);
		}
		NotifyRuleChanged();
	});
	
	leadingMoveTime.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetLeadingMoveTime(leadingMoveTime);
		}
		NotifyRuleChanged();
	});
	
	referenceMoveName.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetReferenceMoveName(referenceMoveName);
		}
		NotifyRuleChanged();
	});
	
	referenceMoveTime.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetReferenceMoveTime(referenceMoveTime);
		}
		NotifyRuleChanged();
	});
	
	useSameMove.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetUseSameMove(useSameMove);
		}
		NotifyRuleChanged();
	});
	
	useComponentSpace.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetUseComponentSpace(useComponentSpace);
		}
		NotifyRuleChanged();
	});
	
	enablePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	});
	
	enableOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	});
	
	enableSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	});
	
	enableVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	});
}

aeRuleAnimationDifference::aeRuleAnimationDifference(aeWindowMain &windowMain, const aeRuleAnimationDifference &copy) :
aeRuleAnimationDifference(windowMain, copy.name)
{
	pInitCopy(copy);
	leadingMoveTime.SetValue(copy.leadingMoveTime, false);
	referenceMoveName.SetValue(copy.referenceMoveName, false);
	referenceMoveTime.SetValue(copy.referenceMoveTime, false);
	useSameMove.SetValue(copy.useSameMove, false);
	useComponentSpace.SetValue(copy.useComponentSpace, false);
	enablePosition.SetValue(copy.enablePosition, false);
	enableOrientation.SetValue(copy.enableOrientation, false);
	enableSize.SetValue(copy.enableSize, false);
	enableVertexPositionSet.SetValue(copy.enableVertexPositionSet, false);
	
	pTargetLeadMoveTime = aeControllerTarget::Ref::New(copy.pTargetLeadMoveTime);
	pTargetRefMoveTime = aeControllerTarget::Ref::New(copy.pTargetRefMoveTime);
}

aeRuleAnimationDifference::~aeRuleAnimationDifference() = default;


// Management
///////////////

void aeRuleAnimationDifference::SetLeadingMoveName(const char *value){
	leadingMoveName = value;
}

void aeRuleAnimationDifference::SetLeadingMoveTime(float value){
	leadingMoveTime = value;
}

void aeRuleAnimationDifference::SetReferenceMoveName(const char *value){
	referenceMoveName = value;
}

void aeRuleAnimationDifference::SetReferenceMoveTime(float value){
	referenceMoveTime = value;
}

void aeRuleAnimationDifference::SetUseSameMove(bool value){
	useSameMove = value;
}

void aeRuleAnimationDifference::SetUseComponentSpace(bool value){
	useComponentSpace = value;
}

void aeRuleAnimationDifference::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleAnimationDifference::SetEnableOrientation(bool value){
	enableOrientation = value;
}

void aeRuleAnimationDifference::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleAnimationDifference::SetEnableVertexPositionSet(bool value){
	enableVertexPositionSet = value;
}


void aeRuleAnimationDifference::UpdateTargets(){
	deAnimatorRuleAnimationDifference *rule = (deAnimatorRuleAnimationDifference*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pTargetLeadMoveTime->UpdateEngineTarget(GetAnimator(), rule->GetTargetLeadingMoveTime());
		pTargetRefMoveTime->UpdateEngineTarget(GetAnimator(), rule->GetTargetReferenceMoveTime());
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
	
	engRule->SetLeadingMoveName(leadingMoveName);
	engRule->SetLeadingMoveTime(leadingMoveTime);
	engRule->SetReferenceMoveName(referenceMoveName);
	engRule->SetReferenceMoveTime(referenceMoveTime);
	engRule->SetUseSameMove(useSameMove);
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	engRule->SetUseComponentSpace(useComponentSpace);
	
	pTargetLeadMoveTime->UpdateEngineTarget(GetAnimator(), engRule->GetTargetLeadingMoveTime());
	pTargetRefMoveTime->UpdateEngineTarget(GetAnimator(), engRule->GetTargetReferenceMoveTime());
	
	// finished
	return engRule;
}



aeRule::Ref aeRuleAnimationDifference::CreateCopy() const{
	return Ref::New(*this);
}

void aeRuleAnimationDifference::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetLeadMoveTime->AddLinksToList(list);
	pTargetRefMoveTime->AddLinksToList(list);
}



// Operators
//////////////

aeRuleAnimationDifference &aeRuleAnimationDifference::operator=(const aeRuleAnimationDifference &copy){
	leadingMoveName = copy.leadingMoveName;
	leadingMoveTime = copy.leadingMoveTime;
	referenceMoveName = copy.referenceMoveName;
	referenceMoveTime = copy.referenceMoveTime;
	useSameMove = copy.useSameMove;
	enablePosition = copy.enablePosition;
	enableOrientation = copy.enableOrientation;
	enableSize = copy.enableSize;
	enableVertexPositionSet = copy.enableVertexPositionSet;
	useComponentSpace = copy.useComponentSpace;
	pTargetLeadMoveTime = copy.pTargetLeadMoveTime;
	pTargetRefMoveTime = copy.pTargetRefMoveTime;
	aeRule::operator=(copy);
	return *this;
}
