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

#include "aeRuleBoneTransformator.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleBoneTransformator
//////////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleBoneTransformator::aeRuleBoneTransformator(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleBoneTransformator::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertBoneTransformator, aname),
minTranslation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minTranslation, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
maxTranslation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxTranslation, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
minRotation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minRotation, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
maxRotation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxRotation, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
minScaling(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minScaling, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
maxScaling(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxScaling, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
axis(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.axis, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
minAngle(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minAngle, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
maxAngle(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxAngle, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.enablePosition, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.enableOrientation, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.enableSize, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
useAxis(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.useAxis, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
targetBone(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetBone, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
inputBone(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.inputBone, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
coordinateFrame(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.coordinateFrame, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
inputSource(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.inputSource, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
targetTranslation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetTranslation, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
targetRotation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetRotation, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>()),
targetScaling(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetScaling, GetMetaContext().StaticCast<aeMCRuleBoneTransformator>())
{
	minTranslation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumTranslation(minTranslation);
		}
		NotifyRuleChanged();
	});
	
	maxTranslation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumTranslation(maxTranslation);
		}
		NotifyRuleChanged();
	});
	
	minRotation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumRotation(minRotation);
		}
		NotifyRuleChanged();
	});
	
	maxRotation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumRotation(maxRotation);
		}
		NotifyRuleChanged();
	});
	
	minScaling.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumScaling(minScaling);
		}
		NotifyRuleChanged();
	});
	
	maxScaling.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumScaling(maxScaling);
		}
		NotifyRuleChanged();
	});
	
	axis.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetAxis(axis);
		}
		NotifyRuleChanged();
	});
	
	minAngle.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumAngle(minAngle);
		}
		NotifyRuleChanged();
	});
	
	maxAngle.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumAngle(maxAngle);
		}
		NotifyRuleChanged();
	});
	
	enablePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	});
	
	enableOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	});
	
	enableSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	});
	
	useAxis.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetUseAxis(useAxis);
		}
		NotifyRuleChanged();
	});
	
	targetBone.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetTargetBone(targetBone);
		}
		NotifyRuleChanged();
	});
	
	inputBone.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetInputBone(inputBone);
		}
		NotifyRuleChanged();
	});
	
	coordinateFrame.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetCoordinateFrame(coordinateFrame);
		}
		NotifyRuleChanged();
	});
	
	inputSource.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetInputSource(inputSource);
		}
		NotifyRuleChanged();
	});
	
	coordinateFrame.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetCoordinateFrame(coordinateFrame);
		}
		NotifyRuleChanged();
	});
	
	inputSource.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetInputSource(inputSource);
		}
		NotifyRuleChanged();
	});
	
	pTargetTranslation = aeControllerTarget::Ref::New(targetTranslation);
	targetTranslation.SetOnChanged([this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleBoneTransformator*)GetEngineRule())->GetTargetTranslation(), targetTranslation);
		}
		pTargetTranslation->OnStorageChanged();
		NotifyRuleChanged();
	});
	
	pTargetRotation = aeControllerTarget::Ref::New(targetRotation);
	targetRotation.SetOnChanged([this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleBoneTransformator*)GetEngineRule())->GetTargetRotation(), targetRotation);
		}
		pTargetRotation->OnStorageChanged();
		NotifyRuleChanged();
	});
	
	pTargetScaling = aeControllerTarget::Ref::New(targetScaling);
	targetScaling.SetOnChanged([this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleBoneTransformator*)GetEngineRule())->GetTargetScaling(), targetScaling);
		}
		pTargetScaling->OnStorageChanged();
		NotifyRuleChanged();
	});
}

aeRuleBoneTransformator::aeRuleBoneTransformator(aeWindowMain &windowMain, const aeRuleBoneTransformator &copy) :
aeRuleBoneTransformator(windowMain, copy.name)
{
	pInitCopy(copy);
	maxTranslation.SetValue(copy.maxTranslation, false);
	minRotation.SetValue(copy.minRotation, false);
	maxRotation.SetValue(copy.maxRotation, false);
	minScaling.SetValue(copy.minScaling, false);
	maxScaling.SetValue(copy.maxScaling, false);
	axis.SetValue(copy.axis, false);
	minAngle.SetValue(copy.minAngle, false);
	maxAngle.SetValue(copy.maxAngle, false);
	enablePosition.SetValue(copy.enablePosition, false);
	enableOrientation.SetValue(copy.enableOrientation, false);
	enableSize.SetValue(copy.enableSize, false);
	useAxis.SetValue(copy.useAxis, false);
	targetBone.SetValue(copy.targetBone, false);
	inputBone.SetValue(copy.inputBone, false);
	coordinateFrame.SetValue(copy.coordinateFrame, false);
	inputSource.SetValue(copy.inputSource, false);
	
	pTargetTranslation = aeControllerTarget::Ref::New(targetTranslation, copy.pTargetTranslation);
	pTargetRotation = aeControllerTarget::Ref::New(targetRotation, copy.pTargetRotation);
	pTargetScaling = aeControllerTarget::Ref::New(targetScaling, copy.pTargetScaling);
}

aeRuleBoneTransformator::~aeRuleBoneTransformator() = default;


// Management
///////////////

void aeRuleBoneTransformator::SetMinimumTranslation(const decVector &value){
	minTranslation = value;
}

void aeRuleBoneTransformator::SetMaximumTranslation(const decVector &value){
	maxTranslation = value;
}

void aeRuleBoneTransformator::SetMinimumRotation(const decVector &value){
	minRotation = value;
}

void aeRuleBoneTransformator::SetMaximumRotation(const decVector &value){
	maxRotation = value;
}

void aeRuleBoneTransformator::SetMinimumScaling(const decVector &value){
	minScaling = value;
}

void aeRuleBoneTransformator::SetMaximumScaling(const decVector &value){
	maxScaling = value;
}

void aeRuleBoneTransformator::SetAxis(const decVector &value){
	axis = value;
}

void aeRuleBoneTransformator::SetMinimumAngle(float value){
	minAngle = value;
}

void aeRuleBoneTransformator::SetMaximumAngle(float value){
	maxAngle = value;
}

void aeRuleBoneTransformator::SetCoordinateFrame(deAnimatorRuleBoneTransformator::eCoordinateFrames value){
	coordinateFrame = value;
}

void aeRuleBoneTransformator::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleBoneTransformator::SetEnableOrientation(bool value){
	enableOrientation = value;
}

void aeRuleBoneTransformator::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleBoneTransformator::SetUseAxis(bool value){
	useAxis = value;
}

void aeRuleBoneTransformator::SetTargetBone(const char *value){
	targetBone = value;
}

void aeRuleBoneTransformator::SetInputBone(const char *value){
	inputBone = value;
}

void aeRuleBoneTransformator::SetInputSource(deAnimatorRuleBoneTransformator::eInputSources value){
	inputSource = value;
}


void aeRuleBoneTransformator::UpdateTargets(){
	aeRule::UpdateTargets();
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(!rule){
		return;
	}
	
	pUpdateEngineTarget(rule->GetTargetTranslation(), targetTranslation);
	pUpdateEngineTarget(rule->GetTargetRotation(), targetRotation);
	pUpdateEngineTarget(rule->GetTargetScaling(), targetScaling);
}

int aeRuleBoneTransformator::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetTranslation->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetRotation->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetScaling->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleBoneTransformator::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetTranslation->GetLinks().Has(link)){
		pTargetTranslation->RemoveLink(link);
	}
	if(pTargetRotation->GetLinks().Has(link)){
		pTargetRotation->RemoveLink(link);
	}
	if(pTargetScaling->GetLinks().Has(link)){
		pTargetScaling->RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleBoneTransformator::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetTranslation->RemoveAllLinks();
	pTargetRotation->RemoveAllLinks();
	pTargetScaling->RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule::Ref aeRuleBoneTransformator::CreateEngineRule(){
	const deAnimatorRuleBoneTransformator::Ref engRule(deAnimatorRuleBoneTransformator::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMinimumTranslation(minTranslation);
	engRule->SetMaximumTranslation(maxTranslation);
	engRule->SetMinimumRotation(minRotation.GetValue() * DEG2RAD);
	engRule->SetMaximumRotation(maxRotation.GetValue() * DEG2RAD);
	engRule->SetMinimumScaling(minScaling);
	engRule->SetMaximumScaling(maxScaling);
	engRule->SetAxis(axis);
	engRule->SetMinimumAngle(minAngle * DEG2RAD);
	engRule->SetMaximumAngle(maxAngle * DEG2RAD);
	engRule->SetCoordinateFrame(coordinateFrame);
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetUseAxis(useAxis);
	engRule->SetTargetBone(targetBone);
	engRule->SetInputBone(inputBone);
	engRule->SetInputSource(inputSource);
	
	pUpdateEngineTarget(engRule->GetTargetTranslation(), targetTranslation);
	pUpdateEngineTarget(engRule->GetTargetRotation(), targetRotation);
	pUpdateEngineTarget(engRule->GetTargetScaling(), targetScaling);
	
	// finished
	return engRule;
}



aeRule::Ref aeRuleBoneTransformator::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleBoneTransformator::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetRotation->AddLinksToList(list);
	pTargetScaling->AddLinksToList(list);
	pTargetTranslation->AddLinksToList(list);
}
