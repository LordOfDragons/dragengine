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

#include "aeRuleAnimationSelect.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationSelect.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleAnimationSelect
////////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleAnimationSelect::aeRuleAnimationSelect(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleAnimationSelect::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertAnimationSelect, aname),
pMPMoves(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.moves, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
pMPEnablePosition(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enablePosition, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
pMPEnableOrientation(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enableOrientation, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
pMPEnableSize(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enableSize, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
pMPEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
pMPTargetMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.targetMoveTime, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
pMPTargetSelect(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.targetSelect, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>())
{
	pMPMoves.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->GetMoves() = pMPMoves;
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnablePosition(pMPEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableOrientation(pMPEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableSize(pMPEnableSize);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pTargetMoveTime = aeControllerTarget::Ref::New(pMPTargetMoveTime);
	pMPTargetMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimationSelect*)GetEngineRule())->GetTargetMoveTime(), pMPTargetMoveTime);
		}
		pTargetMoveTime->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetSelect = aeControllerTarget::Ref::New(pMPTargetSelect);
	pMPTargetSelect.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimationSelect*)GetEngineRule())->GetTargetSelect(), pMPTargetSelect);
		}
		pTargetSelect->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRuleAnimationSelect::aeRuleAnimationSelect(aeWindowMain &windowMain, const aeRuleAnimationSelect &copy) :
aeRuleAnimationSelect(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPMoves.SetValue(copy.pMPMoves, false);
	pMPEnablePosition.SetValue(copy.pMPEnablePosition, false);
	pMPEnableOrientation.SetValue(copy.pMPEnableOrientation, false);
	pMPEnableSize.SetValue(copy.pMPEnableSize, false);
	pMPEnableVertexPositionSet.SetValue(copy.pMPEnableVertexPositionSet, false);
	
	pTargetMoveTime = aeControllerTarget::Ref::New(pMPTargetMoveTime, copy.pTargetMoveTime);
	pTargetSelect = aeControllerTarget::Ref::New(pMPTargetSelect, copy.pTargetSelect);
}

aeRuleAnimationSelect::~aeRuleAnimationSelect() = default;


// Management
///////////////

void aeRuleAnimationSelect::SetEnablePosition(bool value){
	pMPEnablePosition.SetValue(value);
}

void aeRuleAnimationSelect::SetEnableOrientation(bool value){
	pMPEnableOrientation.SetValue(value);
}

void aeRuleAnimationSelect::SetEnableSize(bool value){
	pMPEnableSize.SetValue(value);
}

void aeRuleAnimationSelect::SetEnableVertexPositionSet(bool value){
	pMPEnableVertexPositionSet.SetValue(value);
}


void aeRuleAnimationSelect::SetMoves(const decStringList &value){
	pMPMoves.SetValue(value);
}


void aeRuleAnimationSelect::UpdateTargets(){
	deAnimatorRuleAnimationSelect *rule = (deAnimatorRuleAnimationSelect*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetMoveTime(), pMPTargetMoveTime);
		pUpdateEngineTarget(rule->GetTargetSelect(), pMPTargetSelect);
	}
}

int aeRuleAnimationSelect::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetMoveTime->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetSelect->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleAnimationSelect::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetMoveTime->GetLinks().Has(link)){
		pTargetMoveTime->RemoveLink(link);
	}
	
	if(pTargetSelect->GetLinks().Has(link)){
		pTargetSelect->RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleAnimationSelect::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetMoveTime->RemoveAllLinks();
	pTargetSelect->RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule::Ref aeRuleAnimationSelect::CreateEngineRule(){
	const deAnimatorRuleAnimationSelect::Ref engRule(deAnimatorRuleAnimationSelect::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->GetMoves() = pMPMoves;
	engRule->SetEnablePosition(pMPEnablePosition);
	engRule->SetEnableOrientation(pMPEnableOrientation);
	engRule->SetEnableSize(pMPEnableSize);
	engRule->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
	
	pUpdateEngineTarget(engRule->GetTargetMoveTime(), pMPTargetMoveTime);
	pUpdateEngineTarget(engRule->GetTargetSelect(), pMPTargetSelect);
	
	return engRule;
}



aeRule::Ref aeRuleAnimationSelect::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleAnimationSelect::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetMoveTime->AddLinksToList(list);
	pTargetSelect->AddLinksToList(list);
}
