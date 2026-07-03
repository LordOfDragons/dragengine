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
mpMinTranslation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minTranslation, metaContext),
mpMaxTranslation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxTranslation, metaContext),
mpMinRotation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minRotation, metaContext),
mpMaxRotation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxRotation, metaContext),
mpMinScaling(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minScaling, metaContext),
mpMaxScaling(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxScaling, metaContext),
mpAxis(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.axis, metaContext),
mpMinAngle(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.minAngle, metaContext),
mpMaxAngle(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.maxAngle, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.enablePosition, metaContext),
mpEnableOrientation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.enableOrientation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.enableSize, metaContext),
mpUseAxis(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.useAxis, metaContext),
mpTargetBone(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetBone, metaContext),
mpInputBone(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.inputBone, metaContext),
mpCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.coordinateFrame, metaContext),
mpInputSource(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.inputSource, metaContext),
mpTargetTranslation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetTranslation, metaContext),
mpTargetRotation(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetRotation, metaContext),
mpTargetScaling(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.targetScaling, metaContext)
{
	mpMinTranslation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumTranslation(mpMinTranslation);
		}
		NotifyRuleChanged();
	};
	
	mpMaxTranslation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumTranslation(mpMaxTranslation);
		}
		NotifyRuleChanged();
	};
	
	mpMinRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumRotation(mpMinRotation);
		}
		NotifyRuleChanged();
	};
	
	mpMaxRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumRotation(mpMaxRotation);
		}
		NotifyRuleChanged();
	};
	
	mpMinScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumScaling(mpMinScaling);
		}
		NotifyRuleChanged();
	};
	
	mpMaxScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumScaling(mpMaxScaling);
		}
		NotifyRuleChanged();
	};
	
	mpAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetAxis(mpAxis);
		}
		NotifyRuleChanged();
	};
	
	mpMinAngle.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMinimumAngle(mpMinAngle);
		}
		NotifyRuleChanged();
	};
	
	mpMaxAngle.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetMaximumAngle(mpMaxAngle);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetEnableOrientation(mpEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpUseAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetUseAxis(mpUseAxis);
		}
		NotifyRuleChanged();
	};
	
	mpTargetBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetTargetBone(mpTargetBone);
		}
		NotifyRuleChanged();
	};
	
	mpInputBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetInputBone(mpInputBone);
		}
		NotifyRuleChanged();
	};
	
	mpCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetCoordinateFrame(mpCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	mpInputSource.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetInputSource(mpInputSource);
		}
		NotifyRuleChanged();
	};
	
	mpCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetCoordinateFrame(mpCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	mpInputSource.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleBoneTransformator*)GetEngineRule())->SetInputSource(mpInputSource);
		}
		NotifyRuleChanged();
	};
	
	mpTargetTranslation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleBoneTransformator*)GetEngineRule())->GetTargetTranslation(), mpTargetTranslation);
		}
		NotifyRuleChanged();
	};
	
	mpTargetRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleBoneTransformator*)GetEngineRule())->GetTargetRotation(), mpTargetRotation);
		}
		NotifyRuleChanged();
	};
	
	mpTargetScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleBoneTransformator*)GetEngineRule())->GetTargetScaling(), mpTargetScaling);
		}
		NotifyRuleChanged();
	};
}

aeRuleBoneTransformator::aeRuleBoneTransformator(const aeRuleBoneTransformator &copy) :
aeRuleBoneTransformator(copy.GetWindowMain(), copy.GetName())
{
	pInitCopy(copy);
	mpMinTranslation.SetValue(copy.mpMinTranslation, false);
	mpMaxTranslation.SetValue(copy.mpMaxTranslation, false);
	mpMinRotation.SetValue(copy.mpMinRotation, false);
	mpMaxRotation.SetValue(copy.mpMaxRotation, false);
	mpMinScaling.SetValue(copy.mpMinScaling, false);
	mpMaxScaling.SetValue(copy.mpMaxScaling, false);
	mpAxis.SetValue(copy.mpAxis, false);
	mpMinAngle.SetValue(copy.mpMinAngle, false);
	mpMaxAngle.SetValue(copy.mpMaxAngle, false);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableOrientation.SetValue(copy.mpEnableOrientation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpUseAxis.SetValue(copy.mpUseAxis, false);
	mpTargetBone.SetValue(copy.mpTargetBone, false);
	mpInputBone.SetValue(copy.mpInputBone, false);
	mpCoordinateFrame.SetValue(copy.mpCoordinateFrame, false);
	mpInputSource.SetValue(copy.mpInputSource, false);
	mpTargetTranslation.SetValue(copy.mpTargetTranslation, false);
	mpTargetRotation.SetValue(copy.mpTargetRotation, false);
	mpTargetScaling.SetValue(copy.mpTargetScaling, false);
}

aeRuleBoneTransformator::~aeRuleBoneTransformator() = default;


// Management
///////////////

void aeRuleBoneTransformator::SetMinimumTranslation(const decVector &value){
	mpMinTranslation = value;
}

void aeRuleBoneTransformator::SetMaximumTranslation(const decVector &value){
	mpMaxTranslation = value;
}

void aeRuleBoneTransformator::SetMinimumRotation(const decVector &value){
	mpMinRotation = value;
}

void aeRuleBoneTransformator::SetMaximumRotation(const decVector &value){
	mpMaxRotation = value;
}

void aeRuleBoneTransformator::SetMinimumScaling(const decVector &value){
	mpMinScaling = value;
}

void aeRuleBoneTransformator::SetMaximumScaling(const decVector &value){
	mpMaxScaling = value;
}

void aeRuleBoneTransformator::SetAxis(const decVector &value){
	mpAxis = value;
}

void aeRuleBoneTransformator::SetMinimumAngle(float value){
	mpMinAngle = value;
}

void aeRuleBoneTransformator::SetMaximumAngle(float value){
	mpMaxAngle = value;
}

void aeRuleBoneTransformator::SetCoordinateFrame(deAnimatorRuleBoneTransformator::eCoordinateFrames value){
	mpCoordinateFrame = value;
}

void aeRuleBoneTransformator::SetEnablePosition(bool value){
	mpEnablePosition = value;
}

void aeRuleBoneTransformator::SetEnableOrientation(bool value){
	mpEnableOrientation = value;
}

void aeRuleBoneTransformator::SetEnableSize(bool value){
	mpEnableSize = value;
}

void aeRuleBoneTransformator::SetUseAxis(bool value){
	mpUseAxis = value;
}

void aeRuleBoneTransformator::SetTargetBone(const char *value){
	mpTargetBone = value;
}

void aeRuleBoneTransformator::SetInputBone(const char *value){
	mpInputBone = value;
}

void aeRuleBoneTransformator::SetInputSource(deAnimatorRuleBoneTransformator::eInputSources value){
	mpInputSource = value;
}


void aeRuleBoneTransformator::UpdateTargets(){
	aeRule::UpdateTargets();
	
	deAnimatorRuleBoneTransformator * const rule = (deAnimatorRuleBoneTransformator*)GetEngineRule();
	if(!rule){
		return;
	}
	
	pUpdateEngineTarget(rule->GetTargetTranslation(), mpTargetTranslation);
	pUpdateEngineTarget(rule->GetTargetRotation(), mpTargetRotation);
	pUpdateEngineTarget(rule->GetTargetScaling(), mpTargetScaling);
}

int aeRuleBoneTransformator::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(mpTargetTranslation->Has(link)){
		usageCount++;
	}
	if(mpTargetRotation->Has(link)){
		usageCount++;
	}
	if(mpTargetScaling->Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

deAnimatorRule::Ref aeRuleBoneTransformator::CreateEngineRule(){
	const deAnimatorRuleBoneTransformator::Ref engRule(deAnimatorRuleBoneTransformator::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMinimumTranslation(mpMinTranslation);
	engRule->SetMaximumTranslation(mpMaxTranslation);
	engRule->SetMinimumRotation(mpMinRotation.GetEulerAnglesRadians());
	engRule->SetMaximumRotation(mpMaxRotation.GetEulerAnglesRadians());
	engRule->SetMinimumScaling(mpMinScaling);
	engRule->SetMaximumScaling(mpMaxScaling);
	engRule->SetAxis(mpAxis);
	engRule->SetMinimumAngle(mpMinAngle * DEG2RAD);
	engRule->SetMaximumAngle(mpMaxAngle * DEG2RAD);
	engRule->SetCoordinateFrame(mpCoordinateFrame);
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableOrientation(mpEnableOrientation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetUseAxis(mpUseAxis);
	engRule->SetTargetBone(mpTargetBone);
	engRule->SetInputBone(mpInputBone);
	engRule->SetInputSource(mpInputSource);
	
	pUpdateEngineTarget(engRule->GetTargetTranslation(), mpTargetTranslation);
	pUpdateEngineTarget(engRule->GetTargetRotation(), mpTargetRotation);
	pUpdateEngineTarget(engRule->GetTargetScaling(), mpTargetScaling);
	
	// finished
	return engRule;
}



aeRule::Ref aeRuleBoneTransformator::CreateCopy() const{
	return Ref::New(*this);
}
