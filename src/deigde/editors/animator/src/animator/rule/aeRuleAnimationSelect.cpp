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

#include "aeRuleAnimationSelect.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationSelect.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleAnimationSelect
////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleAnimationSelect::aeRuleAnimationSelect() :
aeRule(deAnimatorRuleVisitorIdentify::ertAnimationSelect),
pEnablePosition(true),
pEnableOrientation(true),
pEnableSize(false),
pEnableVertexPositionSet(true)
{
	SetName("Animation Select");
}

aeRuleAnimationSelect::aeRuleAnimationSelect(const aeRuleAnimationSelect &copy) :
aeRule(copy),
pMoves(copy.pMoves),
pEnablePosition(copy.pEnablePosition),
pEnableOrientation(copy.pEnableOrientation),
pEnableSize(copy.pEnableSize),
pEnableVertexPositionSet(copy.pEnableVertexPositionSet),
pTargetMoveTime(copy.pTargetMoveTime),
pTargetSelect(copy.pTargetSelect){
}

aeRuleAnimationSelect::~aeRuleAnimationSelect(){
}



// Management
///////////////

void aeRuleAnimationSelect::SetMoves(const decStringList &moves){
	if(pMoves == moves){
		return;
	}
	
	pMoves = moves;
	
	if(GetEngineRule()){
		((deAnimatorRuleAnimationSelect*)GetEngineRule())->GetMoves() = moves;
		NotifyRuleChanged();
	}
}

void aeRuleAnimationSelect::SetEnablePosition(bool enabled){
	if(enabled != pEnablePosition){
		pEnablePosition = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnablePosition(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimationSelect::SetEnableOrientation(bool enabled){
	if(enabled != pEnableOrientation){
		pEnableOrientation = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableOrientation(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimationSelect::SetEnableSize(bool enabled){
	if(enabled != pEnableSize){
		pEnableSize = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableSize(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimationSelect::SetEnableVertexPositionSet(bool enabled){
	if(enabled != pEnableVertexPositionSet){
		pEnableVertexPositionSet = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableVertexPositionSet(enabled);
			NotifyRuleChanged();
		}
	}
}



void aeRuleAnimationSelect::UpdateTargets(){
	deAnimatorRuleAnimationSelect *rule = (deAnimatorRuleAnimationSelect*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pTargetMoveTime.UpdateEngineTarget(GetAnimator(), rule->GetTargetMoveTime());
		pTargetSelect.UpdateEngineTarget(GetAnimator(), rule->GetTargetSelect());
	}
}

int aeRuleAnimationSelect::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetMoveTime.HasLink(link)){
		usageCount++;
	}
	if(pTargetSelect.HasLink(link)){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleAnimationSelect::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetMoveTime.HasLink(link)){
		pTargetMoveTime.RemoveLink(link);
	}
	
	if(pTargetSelect.HasLink(link)){
		pTargetSelect.RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleAnimationSelect::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetMoveTime.RemoveAllLinks();
	pTargetSelect.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule::Ref aeRuleAnimationSelect::CreateEngineRule(){
	deAnimatorRuleAnimationSelect::Ref engRule = NULL;
	
	try{
		engRule.TakeOver(new deAnimatorRuleAnimationSelect);
		
		InitEngineRule(engRule);
		
		engRule->GetMoves() = pMoves;
		engRule->SetEnablePosition(pEnablePosition);
		engRule->SetEnableOrientation(pEnableOrientation);
		engRule->SetEnableSize(pEnableSize);
		engRule->SetEnableVertexPositionSet(pEnableVertexPositionSet);
		
		pTargetMoveTime.UpdateEngineTarget(GetAnimator(), engRule->GetTargetMoveTime());
		pTargetSelect.UpdateEngineTarget(GetAnimator(), engRule->GetTargetSelect());
		
	}catch(const deException &){
		throw;
	}
	
	return engRule;
}



aeRule *aeRuleAnimationSelect::CreateCopy() const{
	return new aeRuleAnimationSelect(*this);
}

void aeRuleAnimationSelect::ListLinks(aeLinkList &list){
	aeRule::ListLinks(list);
	pTargetMoveTime.AddLinksToList(list);
	pTargetSelect.AddLinksToList(list);
}



// Operators
//////////////

aeRuleAnimationSelect &aeRuleAnimationSelect::operator=(const aeRuleAnimationSelect &copy){
	SetMoves(copy.pMoves);
	SetEnablePosition(copy.pEnablePosition);
	SetEnableOrientation(copy.pEnableOrientation);
	SetEnableSize(copy.pEnableSize);
	SetEnableVertexPositionSet(copy.pEnableVertexPositionSet);
	pTargetMoveTime = copy.pTargetMoveTime;
	pTargetSelect = copy.pTargetSelect;
	aeRule::operator=(copy);
	return *this;
}
