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
pTargetMoveTime(aeControllerTarget::Ref::New()),
moveName(windowMain.GetMCAnimatorProperties().ruleAnimation.moveName, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
moveTime(windowMain.GetMCAnimatorProperties().ruleAnimation.moveTime, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleAnimation.enablePosition, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleAnimation.enableOrientation, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleAnimation.enableSize, GetMetaContext().StaticCast<aeMCRuleAnimation>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleAnimation.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleAnimation>())
{
	moveName.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetMoveName(moveName);
		}
		NotifyRuleChanged();
	});
	
	moveTime.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetMoveTime(moveTime);
		}
		NotifyRuleChanged();
	});
	
	enablePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	});
	
	enableOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	});
	
	enableSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	});
	
	enableVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	});
}

aeRuleAnimation::aeRuleAnimation(aeWindowMain &windowMain, const aeRuleAnimation &copy) :
aeRuleAnimation(windowMain, copy.name)
{
	pInitCopy(copy);
	moveTime.SetValue(copy.moveTime, false);
	enablePosition.SetValue(copy.enablePosition, false);
	enableOrientation.SetValue(copy.enableOrientation, false);
	enableSize.SetValue(copy.enableSize, false);
	enableVertexPositionSet.SetValue(copy.enableVertexPositionSet, false);
	
	pTargetMoveTime = aeControllerTarget::Ref::New(copy.pTargetMoveTime);
}

aeRuleAnimation::~aeRuleAnimation() = default;



// Management
///////////////


void aeRuleAnimation::SetMoveName(const char *value){
	moveName = value;
}

void aeRuleAnimation::SetMoveTime(float value){
	moveTime = value;
}

void aeRuleAnimation::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleAnimation::SetEnableOrientation(bool value){
	enableOrientation = value;
}

void aeRuleAnimation::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleAnimation::SetEnableVertexPositionSet(bool value){
	enableVertexPositionSet = value;
}


void aeRuleAnimation::UpdateTargets(){
	deAnimatorRuleAnimation *rule = (deAnimatorRuleAnimation*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pTargetMoveTime->UpdateEngineTarget(GetAnimator(), rule->GetTargetMoveTime());
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
	
	engRule->SetMoveName(moveName);
	engRule->SetMoveTime(moveTime);
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	
	pTargetMoveTime->UpdateEngineTarget(GetAnimator(), engRule->GetTargetMoveTime());
	
	// finished
	return engRule;
}



aeRule::Ref aeRuleAnimation::CreateCopy() const{
	return Ref::New(*this);
}



void aeRuleAnimation::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetMoveTime->AddLinksToList(list);
}



// Operators
//////////////

aeRuleAnimation &aeRuleAnimation::operator=(const aeRuleAnimation &copy){
	moveName = copy.moveName;
	moveTime = copy.moveTime;
	enablePosition = copy.enablePosition;
	enableOrientation = copy.enableOrientation;
	enableSize = copy.enableSize;
	enableVertexPositionSet = copy.enableVertexPositionSet;
	pTargetMoveTime = copy.pTargetMoveTime;
	aeRule::operator=(copy);
	return *this;
}
