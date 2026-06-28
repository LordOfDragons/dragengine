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

#include "aeRuleAnimation.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleAnimation
//////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleAnimation::aeRuleAnimation(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleAnimation::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertAnimation, aname),
pMPMoveName(windowMain.GetMCAnimatorProperties().ruleAnimation.moveName, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
pMPMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimation.moveTime, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
pMPEnablePosition(windowMain.GetMCAnimatorProperties().ruleAnimation.enablePosition, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
pMPEnableOrientation(windowMain.GetMCAnimatorProperties().ruleAnimation.enableOrientation, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
pMPEnableSize(windowMain.GetMCAnimatorProperties().ruleAnimation.enableSize, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
pMPEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleAnimation.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
pMPTargetMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimation.targetMoveTime, GetMetaContext().StaticCast<aeMCRuleAnimation>())
{
	pMPMoveName.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetMoveName(pMPMoveName);
		}
		NotifyRuleChanged();
	};
	
	pMPMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetMoveTime(pMPMoveTime);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnablePosition(pMPEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnableOrientation(pMPEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnableSize(pMPEnableSize);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pTargetMoveTime = aeControllerTarget::Ref::New(pMPTargetMoveTime);
	pMPTargetMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimation*)GetEngineRule())->GetTargetMoveTime(), pMPTargetMoveTime);
		}
		pTargetMoveTime->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRuleAnimation::aeRuleAnimation(aeWindowMain &windowMain, const aeRuleAnimation &copy) :
aeRuleAnimation(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPMoveTime.SetValue(copy.pMPMoveTime, false);
	pMPEnablePosition.SetValue(copy.pMPEnablePosition, false);
	pMPEnableOrientation.SetValue(copy.pMPEnableOrientation, false);
	pMPEnableSize.SetValue(copy.pMPEnableSize, false);
	pMPEnableVertexPositionSet.SetValue(copy.pMPEnableVertexPositionSet, false);
	
	pTargetMoveTime = aeControllerTarget::Ref::New(pMPTargetMoveTime, copy.pTargetMoveTime);
}

aeRuleAnimation::~aeRuleAnimation() = default;



// Management
///////////////


void aeRuleAnimation::SetMoveName(const char *value){
	pMPMoveName = value;
}

void aeRuleAnimation::SetMoveTime(float value){
	pMPMoveTime = value;
}

void aeRuleAnimation::SetEnablePosition(bool value){
	pMPEnablePosition = value;
}

void aeRuleAnimation::SetEnableOrientation(bool value){
	pMPEnableOrientation = value;
}

void aeRuleAnimation::SetEnableSize(bool value){
	pMPEnableSize = value;
}

void aeRuleAnimation::SetEnableVertexPositionSet(bool value){
	pMPEnableVertexPositionSet = value;
}


void aeRuleAnimation::UpdateTargets(){
	deAnimatorRuleAnimation *rule = (deAnimatorRuleAnimation*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetMoveTime(), pMPTargetMoveTime);
	}
}

int aeRuleAnimation::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetMoveTime->GetLinks().Has(link)) usageCount++;
	
	return usageCount;
}

void aeRuleAnimation::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetMoveTime->GetLinks().Has(link)){
		pTargetMoveTime->RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleAnimation::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetMoveTime->RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule::Ref aeRuleAnimation::CreateEngineRule(){
	
	// create rule
	const deAnimatorRuleAnimation::Ref engRule(deAnimatorRuleAnimation::Ref::New());
	
	// init rule
	InitEngineRule(engRule);
	
	engRule->SetMoveName(pMPMoveName);
	engRule->SetMoveTime(pMPMoveTime);
	engRule->SetEnablePosition(pMPEnablePosition);
	engRule->SetEnableOrientation(pMPEnableOrientation);
	engRule->SetEnableSize(pMPEnableSize);
	engRule->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
	
	pUpdateEngineTarget(engRule->GetTargetMoveTime(), pMPTargetMoveTime);
	
	// finished
	return engRule;
}



aeRule::Ref aeRuleAnimation::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}



void aeRuleAnimation::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetMoveTime->AddLinksToList(list);
}
