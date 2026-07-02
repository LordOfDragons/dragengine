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

aeRuleTrackTo::MetaContext::Ref aeRuleTrackTo::CreateMetaContext(aeWindowMain &windowMain, aeRuleTrackTo *rule){
	return MetaContext::Ref::New("animator.rule_track_to", "Rule Track To", "Rule track to properties",
		windowMain.GetMCAnimatorProperties().ruleTrackTo.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleTrackTo::aeRuleTrackTo(aeWindowMain &windowMain, const char *aname) :
aeRuleTrackTo(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleTrackTo::aeRuleTrackTo(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertTrackTo, aname),
pMPTrackBone(windowMain.GetMCAnimatorProperties().ruleTrackTo.trackBone, metaContext),
pMPTrackAxis(windowMain.GetMCAnimatorProperties().ruleTrackTo.trackAxis, metaContext),
pMPUpAxis(windowMain.GetMCAnimatorProperties().ruleTrackTo.upAxis, metaContext),
pMPUpTarget(windowMain.GetMCAnimatorProperties().ruleTrackTo.upTarget, metaContext),
pMPLockedAxis(windowMain.GetMCAnimatorProperties().ruleTrackTo.lockedAxis, metaContext),
pMPTargetPosition(windowMain.GetMCAnimatorProperties().ruleTrackTo.targetPosition, metaContext),
pMPTargetUp(windowMain.GetMCAnimatorProperties().ruleTrackTo.targetUp, metaContext)
{
	pMPTrackBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetTrackBone(pMPTrackBone);
		}
		NotifyRuleChanged();
	};
	
	pMPTrackAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetTrackAxis(pMPTrackAxis);
		}
		NotifyRuleChanged();
	};
	
	pMPUpAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetUpAxis(pMPUpAxis);
		}
		NotifyRuleChanged();
	};
	
	pMPUpTarget.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetUpTarget(pMPUpTarget);
		}
		NotifyRuleChanged();
	};
	
	pMPLockedAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetLockedAxis(pMPLockedAxis);
		}
		NotifyRuleChanged();
	};
	
	pTargetPosition = aeControllerTarget::Ref::New(pMPTargetPosition);
	pMPTargetPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleTrackTo*)GetEngineRule())->GetTargetPosition(), pMPTargetPosition);
		}
		pTargetPosition->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetUp = aeControllerTarget::Ref::New(pMPTargetUp);
	pMPTargetUp.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleTrackTo*)GetEngineRule())->GetTargetUp(), pMPTargetUp);
		}
		pTargetUp->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRuleTrackTo::aeRuleTrackTo(aeWindowMain &windowMain, const aeRuleTrackTo &copy) :
aeRuleTrackTo(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPTrackBone.SetValue(copy.pMPTrackBone, false);
	pMPTrackAxis.SetValue(copy.pMPTrackAxis, false);
	pMPUpAxis.SetValue(copy.pMPUpAxis, false);
	pMPUpTarget.SetValue(copy.pMPUpTarget, false);
	pMPLockedAxis.SetValue(copy.pMPLockedAxis, false);
	
	pTargetPosition = aeControllerTarget::Ref::New(pMPTargetPosition, copy.pTargetPosition);
	pTargetUp = aeControllerTarget::Ref::New(pMPTargetUp, copy.pTargetUp);
}

aeRuleTrackTo::~aeRuleTrackTo(){
}


// Management
///////////////

void aeRuleTrackTo::SetTrackBone(const char *value){
	pMPTrackBone = value;
}

void aeRuleTrackTo::SetTrackAxis(deAnimatorRuleTrackTo::eTrackAxis value){
	pMPTrackAxis = value;
}

void aeRuleTrackTo::SetUpAxis(deAnimatorRuleTrackTo::eTrackAxis value){
	pMPUpAxis = value;
}

void aeRuleTrackTo::SetUpTarget(deAnimatorRuleTrackTo::eUpTarget value){
	pMPUpTarget = value;
}

void aeRuleTrackTo::SetLockedAxis(deAnimatorRuleTrackTo::eLockedAxis value){
	pMPLockedAxis = value;
}


void aeRuleTrackTo::UpdateTargets(){
	deAnimatorRuleTrackTo *engRule = (deAnimatorRuleTrackTo*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(engRule){
		pUpdateEngineTarget(engRule->GetTargetPosition(), pMPTargetPosition);
		pUpdateEngineTarget(engRule->GetTargetUp(), pMPTargetUp);
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
	
	engRule->SetTrackBone(pMPTrackBone);
	engRule->SetTrackAxis(pMPTrackAxis);
	engRule->SetUpAxis(pMPUpAxis);
	engRule->SetUpTarget(pMPUpTarget);
	engRule->SetLockedAxis(pMPLockedAxis);
	
	pUpdateEngineTarget(engRule->GetTargetPosition(), pMPTargetPosition);
	pUpdateEngineTarget(engRule->GetTargetUp(), pMPTargetUp);
	
	return engRule;
}


aeRule::Ref aeRuleTrackTo::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleTrackTo::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetPosition->AddLinksToList(list);
	pTargetUp->AddLinksToList(list);
}
