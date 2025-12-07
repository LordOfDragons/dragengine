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

#include "aeRuleInverseKinematic.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleInverseKinematic.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleInverseKinematic
/////////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleInverseKinematic::aeRuleInverseKinematic() :
aeRule(deAnimatorRuleVisitorIdentify::ertInverseKinematic),
pAdjustOrientation(false),
pUseSolverBone(false),
pReachRange(0.0f)
{
	SetName("Inverse Kinematic");
}

aeRuleInverseKinematic::aeRuleInverseKinematic(const aeRuleInverseKinematic &copy) :
aeRule(copy),
pGoalPosition(copy.pGoalPosition),
pGoalOrientation(copy.pGoalOrientation),
pLocalPosition(copy.pLocalPosition),
pLocalOrientation(copy.pLocalOrientation),
pAdjustOrientation(copy.pAdjustOrientation),
pUseSolverBone(copy.pUseSolverBone),
pSolverBone(copy.pSolverBone),

pReachRange(copy.pReachRange),
pReachBone(copy.pReachBone),
pReachCenter(copy.pReachCenter),

pTargetGoalPosition(copy.pTargetGoalPosition),
pTargetGoalOrientation(copy.pTargetGoalOrientation),
pTargetLocalPosition(copy.pTargetLocalPosition),
pTargetLocalOrientation(copy.pTargetLocalOrientation),
pTargetReachRange(copy.pTargetReachRange),
pTargetReachCenter(copy.pTargetReachCenter){
}

aeRuleInverseKinematic::~aeRuleInverseKinematic(){
}



// Management
///////////////

void aeRuleInverseKinematic::SetGoalPosition(const decVector &position){
	if(! position.IsEqualTo(pGoalPosition)){
		deAnimatorRuleInverseKinematic *engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		
		pGoalPosition = position;
		
		if(engRule){
			engRule->SetGoalPosition(position);
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleInverseKinematic::SetGoalOrientation(const decVector &orientation){
	if(! orientation.IsEqualTo(pGoalOrientation)){
		deAnimatorRuleInverseKinematic *engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		
		pGoalOrientation = orientation;
		
		if(engRule){
			engRule->SetGoalOrientation(decMatrix::CreateRotation(orientation * DEG2RAD).ToQuaternion());
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleInverseKinematic::SetLocalPosition(const decVector &position){
	if(! position.IsEqualTo(pLocalPosition)){
		deAnimatorRuleInverseKinematic *engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		
		pLocalPosition = position;
		
		if(engRule){
			engRule->SetLocalPosition(position);
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleInverseKinematic::SetLocalOrientation(const decVector &orientation){
	if(! orientation.IsEqualTo(pLocalOrientation)){
		deAnimatorRuleInverseKinematic *engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		
		pLocalOrientation = orientation;
		
		if(engRule){
			engRule->SetLocalOrientation(decMatrix::CreateRotation(orientation * DEG2RAD).ToQuaternion());
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleInverseKinematic::SetAdjustOrientation(bool adjust){
	if(adjust != pAdjustOrientation){
		deAnimatorRuleInverseKinematic *engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		
		pAdjustOrientation = adjust;
		
		if(engRule){
			engRule->SetAdjustOrientation(adjust);
		}
		
		NotifyRuleChanged();
	}
}



void aeRuleInverseKinematic::SetUseSolverBone(bool useSolverBone){
	if(useSolverBone != pUseSolverBone){
		deAnimatorRuleInverseKinematic *engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		
		pUseSolverBone = useSolverBone;
		
		if(engRule){
			engRule->SetUseSolverBone(useSolverBone);
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleInverseKinematic::SetSolverBone(const char *solverBone){
	if(! solverBone) DETHROW(deeInvalidParam);
	
	if(! pSolverBone.Equals(solverBone)){
		deAnimatorRuleInverseKinematic *engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		
		pSolverBone = solverBone;
		
		if(engRule){
			engRule->SetSolverBone(solverBone);
		}
		
		NotifyRuleChanged();
	}
}



void aeRuleInverseKinematic::SetReachRange(float range){
	range = decMath::max(range, 0.0f);
	
	if(fabs(range - pReachRange) > FLOAT_SAFE_EPSILON){
		pReachRange = range;
		
		deAnimatorRuleInverseKinematic * const engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		if(engRule){
			engRule->SetReachRange(range);
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleInverseKinematic::SetReachBone(const char *bone){
	if(pReachBone != bone){
		pReachBone = bone;
		
		deAnimatorRuleInverseKinematic * const engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		if(engRule){
			engRule->SetReachBone(bone);
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleInverseKinematic::SetReachCenter(const decVector &center){
	if(! center.IsEqualTo(pReachCenter)){
		pReachCenter = center;
		
		deAnimatorRuleInverseKinematic * const engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
		if(engRule){
			engRule->SetReachCenter(center);
		}
		
		NotifyRuleChanged();
	}
}



void aeRuleInverseKinematic::UpdateTargets(){
	deAnimatorRuleInverseKinematic *engRule = (deAnimatorRuleInverseKinematic*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(engRule){
		aeAnimator *animator = GetAnimator();
		
		pTargetGoalPosition.UpdateEngineTarget(animator, engRule->GetTargetGoalPosition());
		pTargetGoalOrientation.UpdateEngineTarget(animator, engRule->GetTargetGoalOrientation());
		pTargetLocalPosition.UpdateEngineTarget(animator, engRule->GetTargetLocalPosition());
		pTargetLocalOrientation.UpdateEngineTarget(animator, engRule->GetTargetLocalOrientation());
		pTargetReachRange.UpdateEngineTarget(animator, engRule->GetTargetReachRange());
		pTargetReachCenter.UpdateEngineTarget(animator, engRule->GetTargetReachCenter());
	}
}

int aeRuleInverseKinematic::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetGoalPosition.HasLink(link)){
		usageCount++;
	}
	if(pTargetGoalOrientation.HasLink(link)){
		usageCount++;
	}
	if(pTargetLocalPosition.HasLink(link)){
		usageCount++;
	}
	if(pTargetLocalOrientation.HasLink(link)){
		usageCount++;
	}
	if(pTargetReachRange.HasLink(link)){
		usageCount++;
	}
	if(pTargetReachCenter.HasLink(link)){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleInverseKinematic::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetGoalPosition.HasLink(link)){
		pTargetGoalPosition.RemoveLink(link);
	}
	if(pTargetGoalOrientation.HasLink(link)){
		pTargetGoalOrientation.RemoveLink(link);
	}
	if(pTargetLocalPosition.HasLink(link)){
		pTargetLocalPosition.RemoveLink(link);
	}
	if(pTargetLocalOrientation.HasLink(link)){
		pTargetLocalOrientation.RemoveLink(link);
	}
	if(pTargetReachRange.HasLink(link)){
		pTargetReachRange.RemoveLink(link);
	}
	if(pTargetReachCenter.HasLink(link)){
		pTargetReachCenter.RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleInverseKinematic::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetGoalPosition.RemoveAllLinks();
	pTargetGoalOrientation.RemoveAllLinks();
	pTargetLocalPosition.RemoveAllLinks();
	pTargetLocalOrientation.RemoveAllLinks();
	pTargetReachRange.RemoveAllLinks();
	pTargetReachCenter.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule *aeRuleInverseKinematic::CreateEngineRule(){
	deAnimatorRuleInverseKinematic *engRule = NULL;
	aeAnimator *animator = GetAnimator();
	
	try{
		// create rule
		engRule = new deAnimatorRuleInverseKinematic;
		if(! engRule) DETHROW(deeOutOfMemory);
		
		// init rule
		InitEngineRule(engRule);
		
		engRule->SetGoalPosition(pGoalPosition);
		engRule->SetGoalOrientation(decMatrix::CreateRotation(pGoalOrientation * DEG2RAD).ToQuaternion());
		engRule->SetLocalPosition(pLocalPosition);
		engRule->SetLocalOrientation(decMatrix::CreateRotation(pLocalOrientation * DEG2RAD).ToQuaternion());
		engRule->SetAdjustOrientation(pAdjustOrientation);
		
		engRule->SetUseSolverBone(pUseSolverBone);
		engRule->SetSolverBone(pSolverBone);
		
		engRule->SetReachRange(pReachRange);
		engRule->SetReachCenter(pReachCenter);
		engRule->SetReachBone(pReachBone);
		
		pTargetGoalPosition.UpdateEngineTarget(animator, engRule->GetTargetGoalPosition());
		pTargetGoalOrientation.UpdateEngineTarget(animator, engRule->GetTargetGoalOrientation());
		pTargetLocalPosition.UpdateEngineTarget(animator, engRule->GetTargetLocalPosition());
		pTargetLocalOrientation.UpdateEngineTarget(animator, engRule->GetTargetLocalOrientation());
		pTargetReachRange.UpdateEngineTarget(animator, engRule->GetTargetReachRange());
		pTargetReachCenter.UpdateEngineTarget(animator, engRule->GetTargetReachCenter());
		
	}catch(const deException &){
		if(engRule){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleInverseKinematic::CreateCopy() const{
	return new aeRuleInverseKinematic(*this);
}

void aeRuleInverseKinematic::ListLinks(aeLinkList &list){
	aeRule::ListLinks(list);
	pTargetGoalPosition.AddLinksToList(list);
	pTargetGoalOrientation.AddLinksToList(list);
	pTargetLocalPosition.AddLinksToList(list);
	pTargetLocalOrientation.AddLinksToList(list);
	pTargetReachRange.AddLinksToList(list);
	pTargetReachCenter.AddLinksToList(list);
}



// Operators
//////////////

aeRuleInverseKinematic &aeRuleInverseKinematic::operator=(const aeRuleInverseKinematic &copy){
	SetGoalPosition(copy.pGoalPosition);
	SetGoalOrientation(copy.pGoalOrientation);
	SetLocalPosition(copy.pLocalPosition);
	SetLocalOrientation(copy.pLocalOrientation);
	SetAdjustOrientation(copy.pAdjustOrientation);
	SetUseSolverBone(copy.pUseSolverBone);
	SetSolverBone(copy.pSolverBone);
	SetReachRange(copy.pReachRange);
	SetReachCenter(copy.pReachCenter);
	SetReachBone(copy.pReachBone);
	pTargetGoalPosition = copy.pTargetGoalPosition;
	pTargetGoalOrientation = copy.pTargetGoalOrientation;
	pTargetLocalPosition = copy.pTargetLocalPosition;
	pTargetLocalOrientation = copy.pTargetLocalOrientation;
	pTargetReachRange = copy.pTargetReachRange;
	pTargetReachCenter = copy.pTargetReachCenter;
	aeRule::operator=(copy);
	return *this;
}
