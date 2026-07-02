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

aeRuleBoneTransformator::MetaContext::Ref aeRuleBoneTransformator::CreateMetaContext(aeWindowMain &windowMain, aeRuleBoneTransformator *rule){
	return MetaContext::Ref::New("animator.rule_bone_transformator", "Rule Bone Transformator", "Rule bone transformator properties",
		windowMain.GetMCAnimatorProperties().ruleBoneTransformator.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleBoneTransformator::aeRuleBoneTransformator(aeWindowMain &windowMain, const char *aname) :
aeRuleBoneTransformator(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleBoneTransformator::aeRuleBoneTransformator(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertBoneTransformator, aname),
pMPMinTranslation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minTranslation, metaContext),
pMPMaxTranslation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxTranslation, metaContext),
pMPMinRotation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minRotation, metaContext),
pMPMaxRotation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxRotation, metaContext),
pMPMinScaling(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minScaling, metaContext),
pMPMaxScaling(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxScaling, metaContext),
pMPAxis(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.axis, metaContext),
pMPMinAngle(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minAngle, metaContext),
pMPMaxAngle(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxAngle, metaContext),
pMPEnablePosition(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.enablePosition, metaContext),
pMPEnableOrientation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.enableOrientation, metaContext),
pMPEnableSize(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.enableSize, metaContext),
pMPUseAxis(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.useAxis, metaContext),
pMPTargetBone(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetBone, metaContext),
pMPInputBone(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.inputBone, metaContext),
pMPCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.coordinateFrame, metaContext),
pMPInputSource(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.inputSource, metaContext),
pMPTargetTranslation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetTranslation, metaContext),
pMPTargetRotation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetRotation, metaContext),
pMPTargetScaling(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetScaling, metaContext)
{
	pMPMinTranslation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumTranslation(pMPMinTranslation);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxTranslation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumTranslation(pMPMaxTranslation);
		}
		NotifyRuleChanged();
	};
	
	pMPMinRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumRotation(pMPMinRotation);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumRotation(pMPMaxRotation);
		}
		NotifyRuleChanged();
	};
	
	pMPMinScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumScaling(pMPMinScaling);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumScaling(pMPMaxScaling);
		}
		NotifyRuleChanged();
	};
	
	pMPAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetAxis(pMPAxis);
		}
		NotifyRuleChanged();
	};
	
	pMPMinAngle.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumAngle(pMPMinAngle);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxAngle.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumAngle(pMPMaxAngle);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetEnablePosition(pMPEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetEnableOrientation(pMPEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetEnableSize(pMPEnableSize);
		}
		NotifyRuleChanged();
	};
	
	pMPUseAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetUseAxis(pMPUseAxis);
		}
		NotifyRuleChanged();
	};
	
	pMPTargetBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetTargetBone(pMPTargetBone);
		}
		NotifyRuleChanged();
	};
	
	pMPInputBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetInputBone(pMPInputBone);
		}
		NotifyRuleChanged();
	};
	
	pMPCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetCoordinateFrame(pMPCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	pMPInputSource.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetInputSource(pMPInputSource);
		}
		NotifyRuleChanged();
	};
	
	pMPCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetCoordinateFrame(pMPCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	pMPInputSource.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetInputSource(pMPInputSource);
		}
		NotifyRuleChanged();
	};
	
	pTargetTranslation = aeControllerTarget::Ref::New(pMPTargetTranslation);
	pMPTargetTranslation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleBoneTransformator*)GetEngineRule())->GetTargetTranslation(), pMPTargetTranslation);
		}
		pTargetTranslation->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetRotation = aeControllerTarget::Ref::New(pMPTargetRotation);
	pMPTargetRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleBoneTransformator*)GetEngineRule())->GetTargetRotation(), pMPTargetRotation);
		}
		pTargetRotation->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetScaling = aeControllerTarget::Ref::New(pMPTargetScaling);
	pMPTargetScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleBoneTransformator*)GetEngineRule())->GetTargetScaling(), pMPTargetScaling);
		}
		pTargetScaling->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRuleBoneTransformator::aeRuleBoneTransformator(aeWindowMain &windowMain, const aeRuleBoneTransformator &copy) :
aeRuleBoneTransformator(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPMinTranslation.SetValue(copy.pMPMinTranslation, false);
	pMPMaxTranslation.SetValue(copy.pMPMaxTranslation, false);
	pMPMinRotation.SetValue(copy.pMPMinRotation, false);
	pMPMaxRotation.SetValue(copy.pMPMaxRotation, false);
	pMPMinScaling.SetValue(copy.pMPMinScaling, false);
	pMPMaxScaling.SetValue(copy.pMPMaxScaling, false);
	pMPAxis.SetValue(copy.pMPAxis, false);
	pMPMinAngle.SetValue(copy.pMPMinAngle, false);
	pMPMaxAngle.SetValue(copy.pMPMaxAngle, false);
	pMPEnablePosition.SetValue(copy.pMPEnablePosition, false);
	pMPEnableOrientation.SetValue(copy.pMPEnableOrientation, false);
	pMPEnableSize.SetValue(copy.pMPEnableSize, false);
	pMPUseAxis.SetValue(copy.pMPUseAxis, false);
	pMPTargetBone.SetValue(copy.pMPTargetBone, false);
	pMPInputBone.SetValue(copy.pMPInputBone, false);
	pMPCoordinateFrame.SetValue(copy.pMPCoordinateFrame, false);
	pMPInputSource.SetValue(copy.pMPInputSource, false);
	
	pTargetTranslation = aeControllerTarget::Ref::New(pMPTargetTranslation, copy.pTargetTranslation);
	pTargetRotation = aeControllerTarget::Ref::New(pMPTargetRotation, copy.pTargetRotation);
	pTargetScaling = aeControllerTarget::Ref::New(pMPTargetScaling, copy.pTargetScaling);
}

aeRuleBoneTransformator::~aeRuleBoneTransformator() = default;


// Management
///////////////

void aeRuleBoneTransformator::SetMinimumTranslation(const decVector &value){
	pMPMinTranslation = value;
}

void aeRuleBoneTransformator::SetMaximumTranslation(const decVector &value){
	pMPMaxTranslation = value;
}

void aeRuleBoneTransformator::SetMinimumRotation(const decVector &value){
	pMPMinRotation = value;
}

void aeRuleBoneTransformator::SetMaximumRotation(const decVector &value){
	pMPMaxRotation = value;
}

void aeRuleBoneTransformator::SetMinimumScaling(const decVector &value){
	pMPMinScaling = value;
}

void aeRuleBoneTransformator::SetMaximumScaling(const decVector &value){
	pMPMaxScaling = value;
}

void aeRuleBoneTransformator::SetAxis(const decVector &value){
	pMPAxis = value;
}

void aeRuleBoneTransformator::SetMinimumAngle(float value){
	pMPMinAngle = value;
}

void aeRuleBoneTransformator::SetMaximumAngle(float value){
	pMPMaxAngle = value;
}

void aeRuleBoneTransformator::SetCoordinateFrame(deAnimatorRuleBoneTransformator::eCoordinateFrames value){
	pMPCoordinateFrame = value;
}

void aeRuleBoneTransformator::SetEnablePosition(bool value){
	pMPEnablePosition = value;
}

void aeRuleBoneTransformator::SetEnableOrientation(bool value){
	pMPEnableOrientation = value;
}

void aeRuleBoneTransformator::SetEnableSize(bool value){
	pMPEnableSize = value;
}

void aeRuleBoneTransformator::SetUseAxis(bool value){
	pMPUseAxis = value;
}

void aeRuleBoneTransformator::SetTargetBone(const char *value){
	pMPTargetBone = value;
}

void aeRuleBoneTransformator::SetInputBone(const char *value){
	pMPInputBone = value;
}

void aeRuleBoneTransformator::SetInputSource(deAnimatorRuleBoneTransformator::eInputSources value){
	pMPInputSource = value;
}


void aeRuleBoneTransformator::UpdateTargets(){
	aeRule::UpdateTargets();
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(!rule){
		return;
	}
	
	pUpdateEngineTarget(rule->GetTargetTranslation(), pMPTargetTranslation);
	pUpdateEngineTarget(rule->GetTargetRotation(), pMPTargetRotation);
	pUpdateEngineTarget(rule->GetTargetScaling(), pMPTargetScaling);
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
	
	engRule->SetMinimumTranslation(pMPMinTranslation);
	engRule->SetMaximumTranslation(pMPMaxTranslation);
	engRule->SetMinimumRotation(pMPMinRotation.GetEulerAnglesRadians());
	engRule->SetMaximumRotation(pMPMaxRotation.GetEulerAnglesRadians());
	engRule->SetMinimumScaling(pMPMinScaling);
	engRule->SetMaximumScaling(pMPMaxScaling);
	engRule->SetAxis(pMPAxis);
	engRule->SetMinimumAngle(pMPMinAngle * DEG2RAD);
	engRule->SetMaximumAngle(pMPMaxAngle * DEG2RAD);
	engRule->SetCoordinateFrame(pMPCoordinateFrame);
	engRule->SetEnablePosition(pMPEnablePosition);
	engRule->SetEnableOrientation(pMPEnableOrientation);
	engRule->SetEnableSize(pMPEnableSize);
	engRule->SetUseAxis(pMPUseAxis);
	engRule->SetTargetBone(pMPTargetBone);
	engRule->SetInputBone(pMPInputBone);
	engRule->SetInputSource(pMPInputSource);
	
	pUpdateEngineTarget(engRule->GetTargetTranslation(), pMPTargetTranslation);
	pUpdateEngineTarget(engRule->GetTargetRotation(), pMPTargetRotation);
	pUpdateEngineTarget(engRule->GetTargetScaling(), pMPTargetScaling);
	
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
