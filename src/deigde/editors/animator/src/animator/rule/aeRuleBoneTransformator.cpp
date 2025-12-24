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

#include "aeRuleBoneTransformator.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleBoneTransformator
////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleBoneTransformator::aeRuleBoneTransformator() :
aeRule(deAnimatorRuleVisitorIdentify::ertBoneTransformator),
pMinScaling(1.0f, 1.0f, 1.0f),
pMaxScaling(1.0f, 1.0f, 1.0f),
pAxis(0.0f, 0.0f, 1.0f),
pMinAngle(0.0f),
pMaxAngle(0.0f),
pCoordinateFrame(deAnimatorRuleBoneTransformator::ecfComponent),
pEnablePosition(false),
pEnableOrientation(true),
pEnableSize(false),
pUseAxis(false),
pInputSource(deAnimatorRuleBoneTransformator::eisTargetBlend)
{
	SetName("Bone Transformator");
}

aeRuleBoneTransformator::aeRuleBoneTransformator(const aeRuleBoneTransformator &copy) :
aeRule(copy),
pMinTranslation(copy.pMinTranslation),
pMaxTranslation(copy.pMaxTranslation),
pMinRotation(copy.pMinRotation),
pMaxRotation(copy.pMaxRotation),
pMinScaling(copy.pMinScaling),
pMaxScaling(copy.pMaxScaling),
pAxis(copy.pAxis),
pMinAngle(copy.pMinAngle),
pMaxAngle(copy.pMaxAngle),
pCoordinateFrame(copy.pCoordinateFrame),
pEnablePosition(copy.pEnablePosition),
pEnableOrientation(copy.pEnableOrientation),
pEnableSize(copy.pEnableSize),
pUseAxis(copy.pUseAxis),
pTargetBone(copy.pTargetBone),
pInputBone(copy.pInputBone),
pInputSource(copy.pInputSource),
pTargetTranslation(copy.pTargetTranslation),
pTargetRotation(copy.pTargetRotation),
pTargetScaling(copy.pTargetScaling){
}

aeRuleBoneTransformator::~aeRuleBoneTransformator(){
}



// Management
///////////////

void aeRuleBoneTransformator::SetMinimumTranslation(const decVector &translation){
	if(translation.IsEqualTo(pMinTranslation)){
		return;
	}
	
	pMinTranslation = translation;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetMinimumTranslation(translation);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMaximumTranslation(const decVector &translation){
	if(translation.IsEqualTo(pMaxTranslation)){
		return;
	}
	
	pMaxTranslation = translation;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetMaximumTranslation(translation);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMinimumRotation(const decVector &rotation){
	if(rotation.IsEqualTo(pMinRotation)){
		return;
	}
	
	pMinRotation = rotation;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetMinimumRotation(rotation * DEG2RAD);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMaximumRotation(const decVector &rotation){
	if(rotation.IsEqualTo(pMaxRotation)){
		return;
	}
	
	pMaxRotation = rotation;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetMaximumRotation(rotation * DEG2RAD);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMinimumScaling(const decVector &scaling){
	if(scaling.IsEqualTo(pMinScaling)){
		return;
	}
	
	pMinScaling = scaling;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetMinimumScaling(scaling);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMaximumScaling(const decVector &scaling){
	if(scaling.IsEqualTo(pMaxScaling)){
		return;
	}
	
	pMaxScaling = scaling;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetMaximumScaling(scaling);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetAxis(const decVector &axis){
	if(axis.IsEqualTo(pAxis)){
		return;
	}
	
	pAxis = axis;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetAxis(axis);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMinimumAngle(float angle){
	if(fabsf(angle - pMinAngle) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinAngle = angle;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetMinimumAngle(angle * DEG2RAD);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMaximumAngle(float angle){
	if(fabsf(angle - pMaxAngle) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxAngle = angle;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetMaximumAngle(angle * DEG2RAD);
		NotifyRuleChanged();
	}
}


void aeRuleBoneTransformator::SetCoordinateFrame(deAnimatorRuleBoneTransformator::eCoordinateFrames coordinateFrame){
	if(coordinateFrame < deAnimatorRuleBoneTransformator::ecfBoneLocal
	|| coordinateFrame > deAnimatorRuleBoneTransformator::ecfTargetBone){
		DETHROW(deeInvalidParam);
	}
	
	if(coordinateFrame == pCoordinateFrame){
		return;
	}
	
	pCoordinateFrame = coordinateFrame;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetCoordinateFrame(coordinateFrame);
		NotifyRuleChanged();
	}
}


void aeRuleBoneTransformator::SetEnablePosition(bool enable){
	if(enable == pEnablePosition){
		return;
	}
	
	pEnablePosition = enable;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetEnablePosition(enable);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetEnableOrientation(bool enable){
	if(enable == pEnableOrientation){
		return;
	}
	
	pEnableOrientation = enable;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetEnableOrientation(enable);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetEnableSize(bool enable){
	if(enable == pEnableSize){
		return;
	}
	
	pEnableSize = enable;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetEnableSize(enable);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetUseAxis(bool useAxis){
	if(useAxis == pUseAxis){
		return;
	}
	
	pUseAxis = useAxis;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetUseAxis(useAxis);
		NotifyRuleChanged();
	}
}


void aeRuleBoneTransformator::SetTargetBone(const char *boneName){
	if(pTargetBone == boneName){
		return;
	}
	
	pTargetBone = boneName;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetTargetBone(boneName);
		NotifyRuleChanged();
	}
}


void aeRuleBoneTransformator::SetInputBone(const char *boneName){
	if(pInputBone == boneName){
		return;
	}
	
	pInputBone = boneName;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetInputBone(boneName);
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetInputSource(deAnimatorRuleBoneTransformator::eInputSources source){
	if(source == pInputSource){
		return;
	}
	
	pInputSource = source;
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(rule){
		rule->SetInputSource(source);
		NotifyRuleChanged();
	}
}


void aeRuleBoneTransformator::UpdateTargets(){
	aeRule::UpdateTargets();
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(!rule){
		return;
	}
	
	pTargetTranslation.UpdateEngineTarget(GetAnimator(), rule->GetTargetTranslation());
	pTargetRotation.UpdateEngineTarget(GetAnimator(), rule->GetTargetRotation());
	pTargetScaling.UpdateEngineTarget(GetAnimator(), rule->GetTargetScaling());
}

int aeRuleBoneTransformator::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetTranslation.GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetRotation.GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetScaling.GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleBoneTransformator::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetTranslation.GetLinks().Has(link)){
		pTargetTranslation.RemoveLink(link);
	}
	if(pTargetRotation.GetLinks().Has(link)){
		pTargetRotation.RemoveLink(link);
	}
	if(pTargetScaling.GetLinks().Has(link)){
		pTargetScaling.RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleBoneTransformator::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetTranslation.RemoveAllLinks();
	pTargetRotation.RemoveAllLinks();
	pTargetScaling.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule::Ref aeRuleBoneTransformator::CreateEngineRule(){
	const deAnimatorRuleBoneTransformator::Ref engRule(deAnimatorRuleBoneTransformator::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMinimumTranslation(pMinTranslation);
	engRule->SetMaximumTranslation(pMaxTranslation);
	engRule->SetMinimumRotation(pMinRotation * DEG2RAD);
	engRule->SetMaximumRotation(pMaxRotation * DEG2RAD);
	engRule->SetMinimumScaling(pMinScaling);
	engRule->SetMaximumScaling(pMaxScaling);
	engRule->SetAxis(pAxis);
	engRule->SetMinimumAngle(pMinAngle * DEG2RAD);
	engRule->SetMaximumAngle(pMaxAngle * DEG2RAD);
	engRule->SetCoordinateFrame(pCoordinateFrame);
	engRule->SetEnablePosition(pEnablePosition);
	engRule->SetEnableOrientation(pEnableOrientation);
	engRule->SetEnableSize(pEnableSize);
	engRule->SetUseAxis(pUseAxis);
	engRule->SetTargetBone(pTargetBone);
	engRule->SetInputBone(pInputBone);
	engRule->SetInputSource(pInputSource);
	
	pTargetTranslation.UpdateEngineTarget(GetAnimator(), engRule->GetTargetTranslation());
	pTargetRotation.UpdateEngineTarget(GetAnimator(), engRule->GetTargetRotation());
	pTargetScaling.UpdateEngineTarget(GetAnimator(), engRule->GetTargetScaling());
	
	// finished
	return engRule;
}



aeRule::Ref aeRuleBoneTransformator::CreateCopy() const{
	return Ref::New(*this);
}

void aeRuleBoneTransformator::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetRotation.AddLinksToList(list);
	pTargetScaling.AddLinksToList(list);
	pTargetTranslation.AddLinksToList(list);
}



// Operators
//////////////

aeRuleBoneTransformator &aeRuleBoneTransformator::operator=(const aeRuleBoneTransformator &copy){
	SetMinimumTranslation(copy.pMinTranslation);
	SetMaximumTranslation(copy.pMaxTranslation);
	SetMinimumRotation(copy.pMinRotation);
	SetMaximumRotation(copy.pMaxRotation);
	SetMinimumScaling(copy.pMinScaling);
	SetMaximumScaling(copy.pMaxScaling);
	SetAxis(copy.pAxis);
	SetMinimumAngle(copy.pMinAngle);
	SetMaximumAngle(copy.pMaxAngle);
	SetCoordinateFrame(copy.pCoordinateFrame);
	SetEnablePosition(copy.pEnablePosition);
	SetEnableOrientation(copy.pEnableOrientation);
	SetEnableSize(copy.pEnableSize);
	SetUseAxis(copy.pUseAxis);
	SetTargetBone(copy.pTargetBone);
	SetInputBone(copy.pInputBone);
	SetInputSource(copy.pInputSource);
	pTargetTranslation = copy.pTargetTranslation;
	pTargetRotation = copy.pTargetRotation;
	pTargetScaling = copy.pTargetScaling;
	aeRule::operator=(copy);
	return *this;
}
