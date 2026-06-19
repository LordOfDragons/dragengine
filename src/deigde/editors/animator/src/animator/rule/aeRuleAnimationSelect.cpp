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
pTargetMoveTime(aeControllerTarget::Ref::New()),
pTargetSelect(aeControllerTarget::Ref::New()),
moves(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.moves, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enablePosition, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enableOrientation, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enableSize, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleAnimationSelect>())
{
	moves.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->GetMoves() = moves;
		}
		NotifyRuleChanged();
	});
	
	enablePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	});
	
	enableOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	});
	
	enableSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	});
	
	enableVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	});
}

aeRuleAnimationSelect::aeRuleAnimationSelect(aeWindowMain &windowMain, const aeRuleAnimationSelect &copy) :
aeRuleAnimationSelect(windowMain, copy.name)
{
	pInitCopy(copy);
	moves.SetValue(copy.moves, false);
	enablePosition.SetValue(copy.enablePosition, false);
	enableOrientation.SetValue(copy.enableOrientation, false);
	enableSize.SetValue(copy.enableSize, false);
	enableVertexPositionSet.SetValue(copy.enableVertexPositionSet, false);
	
	pTargetMoveTime = aeControllerTarget::Ref::New(copy.pTargetMoveTime);
	pTargetSelect = aeControllerTarget::Ref::New(copy.pTargetSelect);
}

aeRuleAnimationSelect::~aeRuleAnimationSelect() = default;


// Management
///////////////

void aeRuleAnimationSelect::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleAnimationSelect::SetEnableOrientation(bool value){
	enableOrientation = value;
}

void aeRuleAnimationSelect::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleAnimationSelect::SetEnableVertexPositionSet(bool value){
	enableVertexPositionSet = value;
}


void aeRuleAnimationSelect::SetMoves(const decStringList &value){
	moves = value;
}


void aeRuleAnimationSelect::UpdateTargets(){
	deAnimatorRuleAnimationSelect *rule = (deAnimatorRuleAnimationSelect*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pTargetMoveTime->UpdateEngineTarget(GetAnimator(), rule->GetTargetMoveTime());
		pTargetSelect->UpdateEngineTarget(GetAnimator(), rule->GetTargetSelect());
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
	
	engRule->GetMoves() = moves;
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	
	pTargetMoveTime->UpdateEngineTarget(GetAnimator(), engRule->GetTargetMoveTime());
	pTargetSelect->UpdateEngineTarget(GetAnimator(), engRule->GetTargetSelect());
	
	return engRule;
}



aeRule::Ref aeRuleAnimationSelect::CreateCopy() const{
	return Ref::New(*this);
}

void aeRuleAnimationSelect::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetMoveTime->AddLinksToList(list);
	pTargetSelect->AddLinksToList(list);
}



// Operators
//////////////

aeRuleAnimationSelect &aeRuleAnimationSelect::operator=(const aeRuleAnimationSelect &copy){
	moves = copy.moves;
	enablePosition = copy.enablePosition;
	enableOrientation = copy.enableOrientation;
	enableSize = copy.enableSize;
	enableVertexPositionSet = copy.enableVertexPositionSet;
	pTargetMoveTime = copy.pTargetMoveTime;
	pTargetSelect = copy.pTargetSelect;
	aeRule::operator=(copy);
	return *this;
}
