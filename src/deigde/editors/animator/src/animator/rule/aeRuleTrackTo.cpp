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

#include <math.h>

#include "aeRuleTrackTo.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleTrackTo
////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleTrackTo::aeRuleTrackTo(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleTrackTo::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertTrackTo, aname),
pTargetPosition(aeControllerTarget::Ref::New()),
pTargetUp(aeControllerTarget::Ref::New()),
trackBone(windowMain.GetMCAnimatorProperties().ruleTrackTo.trackBone, GetMetaContext().StaticCast<aeMCRuleTrackTo>()),
trackAxis(windowMain.GetMCAnimatorProperties().ruleTrackTo.trackAxis, GetMetaContext().StaticCast<aeMCRuleTrackTo>()),
upAxis(windowMain.GetMCAnimatorProperties().ruleTrackTo.upAxis, GetMetaContext().StaticCast<aeMCRuleTrackTo>()),
upTarget(windowMain.GetMCAnimatorProperties().ruleTrackTo.upTarget, GetMetaContext().StaticCast<aeMCRuleTrackTo>()),
lockedAxis(windowMain.GetMCAnimatorProperties().ruleTrackTo.lockedAxis, GetMetaContext().StaticCast<aeMCRuleTrackTo>())
{
	trackBone.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetTrackBone(trackBone);
		}
		NotifyRuleChanged();
	});
	
	trackAxis.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetTrackAxis(trackAxis);
		}
		NotifyRuleChanged();
	});
	
	upAxis.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetUpAxis(upAxis);
		}
		NotifyRuleChanged();
	});
	
	upTarget.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetUpTarget(upTarget);
		}
		NotifyRuleChanged();
	});
	
	lockedAxis.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetLockedAxis(lockedAxis);
		}
		NotifyRuleChanged();
	});
}

aeRuleTrackTo::aeRuleTrackTo(aeWindowMain &windowMain, const aeRuleTrackTo &copy) :
aeRuleTrackTo(windowMain, copy.name)
{
	pInitCopy(copy);
	trackAxis.SetValue(copy.trackAxis, false);
	upAxis.SetValue(copy.upAxis, false);
	upTarget.SetValue(copy.upTarget, false);
	lockedAxis.SetValue(copy.lockedAxis, false);
	
	pTargetPosition = aeControllerTarget::Ref::New(copy.pTargetPosition);
	pTargetUp = aeControllerTarget::Ref::New(copy.pTargetUp);
}

aeRuleTrackTo::~aeRuleTrackTo(){
}


// Management
///////////////

void aeRuleTrackTo::SetTrackBone(const char *value){
	trackBone = value;
}

void aeRuleTrackTo::SetTrackAxis(deAnimatorRuleTrackTo::eTrackAxis value){
	trackAxis = value;
}

void aeRuleTrackTo::SetUpAxis(deAnimatorRuleTrackTo::eTrackAxis value){
	upAxis = value;
}

void aeRuleTrackTo::SetUpTarget(deAnimatorRuleTrackTo::eUpTarget value){
	upTarget = value;
}

void aeRuleTrackTo::SetLockedAxis(deAnimatorRuleTrackTo::eLockedAxis value){
	lockedAxis = value;
}


void aeRuleTrackTo::UpdateTargets(){
	deAnimatorRuleTrackTo *engRule = (deAnimatorRuleTrackTo*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(engRule){
		aeAnimator *animator = GetAnimator();
		
		pTargetPosition->UpdateEngineTarget(animator, engRule->GetTargetPosition());
		pTargetUp->UpdateEngineTarget(animator, engRule->GetTargetUp());
	}
}

int aeRuleTrackTo::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetPosition->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetUp->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleTrackTo::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetPosition->GetLinks().Has(link)){
		pTargetPosition->RemoveLink(link);
	}
	
	if(pTargetUp->GetLinks().Has(link)){
		pTargetUp->RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleTrackTo::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetPosition->RemoveAllLinks();
	pTargetUp->RemoveAllLinks();
	
	UpdateTargets();
}


deAnimatorRule::Ref aeRuleTrackTo::CreateEngineRule(){
	const deAnimatorRuleTrackTo::Ref engRule(deAnimatorRuleTrackTo::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetTrackBone(trackBone);
	engRule->SetTrackAxis(trackAxis);
	engRule->SetUpAxis(upAxis);
	engRule->SetUpTarget(upTarget);
	engRule->SetLockedAxis(lockedAxis);
	
	aeAnimator * const animator = GetAnimator();
	pTargetPosition->UpdateEngineTarget(animator, engRule->GetTargetPosition());
	pTargetUp->UpdateEngineTarget(animator, engRule->GetTargetUp());
	
	return engRule;
}


aeRule::Ref aeRuleTrackTo::CreateCopy() const{
	return Ref::New(*this);
}

void aeRuleTrackTo::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetPosition->AddLinksToList(list);
	pTargetUp->AddLinksToList(list);
}


// Operators
///////////////

aeRuleTrackTo &aeRuleTrackTo::operator=(const aeRuleTrackTo &copy){
	trackBone = copy.trackBone;
	trackAxis = copy.trackAxis;
	upAxis = copy.upAxis;
	upTarget = copy.upTarget;
	lockedAxis = copy.lockedAxis;
	pTargetPosition = copy.pTargetPosition;
	pTargetUp = copy.pTargetUp;
	aeRule::operator=(copy);
	return *this;
}
