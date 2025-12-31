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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeRuleTrackTo.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleTrackTo
////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleTrackTo::aeRuleTrackTo() :
aeRule(deAnimatorRuleVisitorIdentify::ertTrackTo),
pTrackAxis(deAnimatorRuleTrackTo::etaPosZ),
pUpAxis(deAnimatorRuleTrackTo::etaPosY),
pUpTarget(deAnimatorRuleTrackTo::eutComponentY),
pLockedAxis(deAnimatorRuleTrackTo::elaNone),
pTargetPosition(aeControllerTarget::Ref::New()),
pTargetUp(aeControllerTarget::Ref::New())
{
	SetName("Track To");
}

aeRuleTrackTo::aeRuleTrackTo(const aeRuleTrackTo &copy) :
aeRule(copy),
pTrackBone(copy.pTrackBone),
pTrackAxis(copy.pTrackAxis),
pUpAxis(copy.pUpAxis),
pUpTarget(copy.pUpTarget),
pLockedAxis(copy.pLockedAxis),
pTargetPosition(aeControllerTarget::Ref::New(copy.pTargetPosition)),
pTargetUp(aeControllerTarget::Ref::New(copy.pTargetUp)){
}

aeRuleTrackTo::~aeRuleTrackTo(){
}



// Management
///////////////

void aeRuleTrackTo::SetTrackBone(const char *boneName){
	if(!boneName){
		DETHROW(deeInvalidParam);
	}
	
	if(pTrackBone != boneName){
		deAnimatorRuleTrackTo *engRule = (deAnimatorRuleTrackTo*)GetEngineRule();
		
		pTrackBone = boneName;
		
		if(engRule){
			engRule->SetTrackBone(boneName);
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleTrackTo::SetTrackAxis(deAnimatorRuleTrackTo::eTrackAxis axis){
	if(axis < deAnimatorRuleTrackTo::etaPosX || axis > deAnimatorRuleTrackTo::etaNegZ){
		DETHROW(deeInvalidParam);
	}
	
	if(axis != pTrackAxis){
		deAnimatorRuleTrackTo *engRule = (deAnimatorRuleTrackTo*)GetEngineRule();
		
		pTrackAxis = axis;
		
		if(engRule){
			engRule->SetTrackAxis(axis);
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleTrackTo::SetUpAxis(deAnimatorRuleTrackTo::eTrackAxis axis){
	if(axis < deAnimatorRuleTrackTo::etaPosX || axis > deAnimatorRuleTrackTo::etaNegZ){
		DETHROW(deeInvalidParam);
	}
	
	if(axis != pUpAxis){
		deAnimatorRuleTrackTo *engRule = (deAnimatorRuleTrackTo*)GetEngineRule();
		
		pUpAxis = axis;
		
		if(engRule){
			engRule->SetUpAxis(axis);
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleTrackTo::SetUpTarget(deAnimatorRuleTrackTo::eUpTarget target){
	if(target < deAnimatorRuleTrackTo::eutWorldX || target > deAnimatorRuleTrackTo::eutController){
		DETHROW(deeInvalidParam);
	}
	
	if(target != pUpTarget){
		deAnimatorRuleTrackTo *engRule = (deAnimatorRuleTrackTo*)GetEngineRule();
		
		pUpTarget = target;
		
		if(engRule){
			engRule->SetUpTarget(target);
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleTrackTo::SetLockedAxis(deAnimatorRuleTrackTo::eLockedAxis axis){
	if(axis < deAnimatorRuleTrackTo::elaNone || axis > deAnimatorRuleTrackTo::elaZ){
		DETHROW(deeInvalidParam);
	}
	
	if(axis != pLockedAxis){
		deAnimatorRuleTrackTo *engRule = (deAnimatorRuleTrackTo*)GetEngineRule();
		
		pLockedAxis = axis;
		
		if(engRule){
			engRule->SetLockedAxis(axis);
		}
		
		NotifyRuleChanged();
	}
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
	
	engRule->SetTrackBone(pTrackBone);
	engRule->SetTrackAxis(pTrackAxis);
	engRule->SetUpAxis(pUpAxis);
	engRule->SetUpTarget(pUpTarget);
	engRule->SetLockedAxis(pLockedAxis);
	
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
	SetTrackBone(copy.pTrackBone);
	SetTrackAxis(copy.pTrackAxis);
	SetUpAxis(copy.pUpAxis);
	SetUpTarget(copy.pUpTarget);
	SetLockedAxis(copy.pLockedAxis);
	pTargetPosition = copy.pTargetPosition;
	pTargetUp = copy.pTargetUp;
	aeRule::operator=(copy);
	return *this;
}
