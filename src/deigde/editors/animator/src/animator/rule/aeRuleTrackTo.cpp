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
mpTrackBone(windowMain.GetMCAnimatorProperties().ruleTrackTo.trackBone, metaContext),
mpTrackAxis(windowMain.GetMCAnimatorProperties().ruleTrackTo.trackAxis, metaContext),
mpUpAxis(windowMain.GetMCAnimatorProperties().ruleTrackTo.upAxis, metaContext),
mpUpTarget(windowMain.GetMCAnimatorProperties().ruleTrackTo.upTarget, metaContext),
mpLockedAxis(windowMain.GetMCAnimatorProperties().ruleTrackTo.lockedAxis, metaContext),
mpTargetPosition(windowMain.GetMCAnimatorProperties().ruleTrackTo.targetPosition, metaContext),
mpTargetUp(windowMain.GetMCAnimatorProperties().ruleTrackTo.targetUp, metaContext)
{
	mpTrackBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetTrackBone(mpTrackBone);
		}
		NotifyRuleChanged();
	};
	
	mpTrackAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetTrackAxis(mpTrackAxis);
		}
		NotifyRuleChanged();
	};
	
	mpUpAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetUpAxis(mpUpAxis);
		}
		NotifyRuleChanged();
	};
	
	mpUpTarget.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetUpTarget(mpUpTarget);
		}
		NotifyRuleChanged();
	};
	
	mpLockedAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleTrackTo*)GetEngineRule())->SetLockedAxis(mpLockedAxis);
		}
		NotifyRuleChanged();
	};
	
	mpTargetPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleTrackTo*)GetEngineRule())->GetTargetPosition(), mpTargetPosition);
		}
		NotifyRuleChanged();
	};
	
	mpTargetUp.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleTrackTo*)GetEngineRule())->GetTargetUp(), mpTargetUp);
		}
		NotifyRuleChanged();
	};
}

aeRuleTrackTo::aeRuleTrackTo(const aeRuleTrackTo &copy) :
aeRuleTrackTo(copy.GetWindowMain(), copy.GetName())
{
	pInitCopy(copy);
	mpTrackBone.SetValue(copy.mpTrackBone, false);
	mpTrackAxis.SetValue(copy.mpTrackAxis, false);
	mpUpAxis.SetValue(copy.mpUpAxis, false);
	mpUpTarget.SetValue(copy.mpUpTarget, false);
	mpLockedAxis.SetValue(copy.mpLockedAxis, false);
	mpTargetPosition.SetValue(copy.mpTargetPosition, false);
	mpTargetUp.SetValue(copy.mpTargetUp, false);
}

aeRuleTrackTo::~aeRuleTrackTo(){
}


// Management
///////////////

void aeRuleTrackTo::SetTrackBone(const char *value){
	mpTrackBone = value;
}

void aeRuleTrackTo::SetTrackAxis(deAnimatorRuleTrackTo::eTrackAxis value){
	mpTrackAxis = value;
}

void aeRuleTrackTo::SetUpAxis(deAnimatorRuleTrackTo::eTrackAxis value){
	mpUpAxis = value;
}

void aeRuleTrackTo::SetUpTarget(deAnimatorRuleTrackTo::eUpTarget value){
	mpUpTarget = value;
}

void aeRuleTrackTo::SetLockedAxis(deAnimatorRuleTrackTo::eLockedAxis value){
	mpLockedAxis = value;
}


void aeRuleTrackTo::UpdateTargets(){
	deAnimatorRuleTrackTo *engRule = (deAnimatorRuleTrackTo*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(engRule){
		pUpdateEngineTarget(engRule->GetTargetPosition(), mpTargetPosition);
		pUpdateEngineTarget(engRule->GetTargetUp(), mpTargetUp);
	}
}

int aeRuleTrackTo::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(mpTargetPosition->Has(link)){
		usageCount++;
	}
	if(mpTargetUp->Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

deAnimatorRule::Ref aeRuleTrackTo::CreateEngineRule(){
	const deAnimatorRuleTrackTo::Ref engRule(deAnimatorRuleTrackTo::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetTrackBone(mpTrackBone);
	engRule->SetTrackAxis(mpTrackAxis);
	engRule->SetUpAxis(mpUpAxis);
	engRule->SetUpTarget(mpUpTarget);
	engRule->SetLockedAxis(mpLockedAxis);
	
	pUpdateEngineTarget(engRule->GetTargetPosition(), mpTargetPosition);
	pUpdateEngineTarget(engRule->GetTargetUp(), mpTargetUp);
	
	return engRule;
}


aeRule::Ref aeRuleTrackTo::CreateCopy() const{
	return Ref::New(*this);
}
