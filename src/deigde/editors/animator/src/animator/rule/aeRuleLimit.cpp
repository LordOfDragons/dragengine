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

#include "aeRuleLimit.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleLimit
//////////////////////

aeRuleLimit::MetaContext::Ref aeRuleLimit::CreateMetaContext(aeWindowMain &windowMain, aeRuleLimit *rule){
	return MetaContext::Ref::New("animator.rule_limit", "Rule Limit", "Rule limit properties",
		windowMain.GetMCAnimatorProperties().ruleLimit.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleLimit::aeRuleLimit(aeWindowMain &windowMain, const char *aname) :
aeRuleLimit(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleLimit::aeRuleLimit(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertLimit, aname),
mpMinPosition(windowMain.GetMCAnimatorProperties().ruleLimit.minPosition, metaContext),
mpMaxPosition(windowMain.GetMCAnimatorProperties().ruleLimit.maxPosition, metaContext),
mpMinRotation(windowMain.GetMCAnimatorProperties().ruleLimit.minRotation, metaContext),
mpMaxRotation(windowMain.GetMCAnimatorProperties().ruleLimit.maxRotation, metaContext),
mpMinScaling(windowMain.GetMCAnimatorProperties().ruleLimit.minScaling, metaContext),
mpMaxScaling(windowMain.GetMCAnimatorProperties().ruleLimit.maxScaling, metaContext),
mpMinVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleLimit.minVertexPositionSet, metaContext),
mpMaxVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleLimit.maxVertexPositionSet, metaContext),
mpTargetBone(windowMain.GetMCAnimatorProperties().ruleLimit.targetBone, metaContext),
mpCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleLimit.coordinateFrame, metaContext),
mpEnablePositionXMin(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionXMin, metaContext),
mpEnablePositionXMax(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionXMax, metaContext),
mpEnablePositionYMin(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionYMin, metaContext),
mpEnablePositionYMax(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionYMax, metaContext),
mpEnablePositionZMin(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionZMin, metaContext),
mpEnablePositionZMax(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionZMax, metaContext),
mpEnableRotationXMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationXMin, metaContext),
mpEnableRotationXMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationXMax, metaContext),
mpEnableRotationYMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationYMin, metaContext),
mpEnableRotationYMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationYMax, metaContext),
mpEnableRotationZMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationZMin, metaContext),
mpEnableRotationZMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationZMax, metaContext),
mpEnableScalingXMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingXMin, metaContext),
mpEnableScalingXMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingXMax, metaContext),
mpEnableScalingYMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingYMin, metaContext),
mpEnableScalingYMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingYMax, metaContext),
mpEnableScalingZMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingZMin, metaContext),
mpEnableScalingZMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingZMax, metaContext),
mpEnableVertexPositionSetMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableVertexPositionSetMin, metaContext),
mpEnableVertexPositionSetMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableVertexPositionSetMax, metaContext)
{
	mpMinPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumPosition(mpMinPosition);
		}
		NotifyRuleChanged();
	};
	
	mpMaxPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumPosition(mpMaxPosition);
		}
		NotifyRuleChanged();
	};
	
	mpMinRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumRotation(mpMinRotation);
		}
		NotifyRuleChanged();
	};
	
	mpMaxRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumRotation(mpMaxRotation);
		}
		NotifyRuleChanged();
	};
	
	mpMinScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumScaling(mpMinScaling);
		}
		NotifyRuleChanged();
	};
	
	mpMaxScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumScaling(mpMaxScaling);
		}
		NotifyRuleChanged();
	};
	
	mpMinVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumVertexPositionSet(mpMinVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpMaxVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumVertexPositionSet(mpMaxVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpTargetBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetTargetBone(mpTargetBone);
		}
		NotifyRuleChanged();
	};
	
	mpCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetCoordinateFrame(mpCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMin(mpEnablePositionXMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMax(mpEnablePositionXMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMin(mpEnablePositionYMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMax(mpEnablePositionYMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMin(mpEnablePositionZMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMax(mpEnablePositionZMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMin(mpEnableRotationXMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMax(mpEnableRotationXMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMin(mpEnableRotationYMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMax(mpEnableRotationYMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMin(mpEnableRotationZMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMax(mpEnableRotationZMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMin(mpEnableScalingXMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMax(mpEnableScalingXMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMin(mpEnableScalingYMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMax(mpEnableScalingYMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMin(mpEnableScalingZMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMax(mpEnableScalingZMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSetMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMin(mpEnableVertexPositionSetMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSetMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMax(mpEnableVertexPositionSetMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMin(mpEnablePositionXMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMax(mpEnablePositionXMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMin(mpEnablePositionYMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMax(mpEnablePositionYMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMin(mpEnablePositionZMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePositionZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMax(mpEnablePositionZMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMin(mpEnableRotationXMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMax(mpEnableRotationXMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMin(mpEnableRotationYMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMax(mpEnableRotationYMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMin(mpEnableRotationZMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotationZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMax(mpEnableRotationZMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMin(mpEnableScalingXMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMax(mpEnableScalingXMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMin(mpEnableScalingYMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMax(mpEnableScalingYMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMin(mpEnableScalingZMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableScalingZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMax(mpEnableScalingZMax);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSetMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMin(mpEnableVertexPositionSetMin);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSetMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMax(mpEnableVertexPositionSetMax);
		}
		NotifyRuleChanged();
	};
}

aeRuleLimit::aeRuleLimit(const aeRuleLimit &copy) :
aeRuleLimit(copy.GetWindowMain(), copy.GetName())
{
	pInitCopy(copy);
	mpMinPosition.SetValue(copy.mpMinPosition, false);
	mpMaxPosition.SetValue(copy.mpMaxPosition, false);
	mpMinRotation.SetValue(copy.mpMinRotation, false);
	mpMaxRotation.SetValue(copy.mpMaxRotation, false);
	mpMinScaling.SetValue(copy.mpMinScaling, false);
	mpMaxScaling.SetValue(copy.mpMaxScaling, false);
	mpMinVertexPositionSet.SetValue(copy.mpMinVertexPositionSet, false);
	mpMaxVertexPositionSet.SetValue(copy.mpMaxVertexPositionSet, false);
	mpTargetBone.SetValue(copy.mpTargetBone, false);
	mpCoordinateFrame.SetValue(copy.mpCoordinateFrame, false);
	mpEnablePositionXMin.SetValue(copy.mpEnablePositionXMin, false);
	mpEnablePositionXMax.SetValue(copy.mpEnablePositionXMax, false);
	mpEnablePositionYMin.SetValue(copy.mpEnablePositionYMin, false);
	mpEnablePositionYMax.SetValue(copy.mpEnablePositionYMax, false);
	mpEnablePositionZMin.SetValue(copy.mpEnablePositionZMin, false);
	mpEnablePositionZMax.SetValue(copy.mpEnablePositionZMax, false);
	mpEnableRotationXMin.SetValue(copy.mpEnableRotationXMin, false);
	mpEnableRotationXMax.SetValue(copy.mpEnableRotationXMax, false);
	mpEnableRotationYMin.SetValue(copy.mpEnableRotationYMin, false);
	mpEnableRotationYMax.SetValue(copy.mpEnableRotationYMax, false);
	mpEnableRotationZMin.SetValue(copy.mpEnableRotationZMin, false);
	mpEnableRotationZMax.SetValue(copy.mpEnableRotationZMax, false);
	mpEnableScalingXMin.SetValue(copy.mpEnableScalingXMin, false);
	mpEnableScalingXMax.SetValue(copy.mpEnableScalingXMax, false);
	mpEnableScalingYMin.SetValue(copy.mpEnableScalingYMin, false);
	mpEnableScalingYMax.SetValue(copy.mpEnableScalingYMax, false);
	mpEnableScalingZMin.SetValue(copy.mpEnableScalingZMin, false);
	mpEnableScalingZMax.SetValue(copy.mpEnableScalingZMax, false);
	mpEnableVertexPositionSetMin.SetValue(copy.mpEnableVertexPositionSetMin, false);
	mpEnableVertexPositionSetMax.SetValue(copy.mpEnableVertexPositionSetMax, false);
}

aeRuleLimit::~aeRuleLimit() = default;


// Management
///////////////

void aeRuleLimit::SetMinimumPosition(const decVector &value){
	mpMinPosition = value;
}

void aeRuleLimit::SetMaximumPosition(const decVector &value){
	mpMaxPosition = value;
}

void aeRuleLimit::SetMinimumRotation(const decVector &value){
	mpMinRotation = value;
}

void aeRuleLimit::SetMaximumRotation(const decVector &value){
	mpMaxRotation = value;
}

void aeRuleLimit::SetMinimumScaling(const decVector &value){
	mpMinScaling = value;
}

void aeRuleLimit::SetMaximumScaling(const decVector &value){
	mpMaxScaling = value;
}

void aeRuleLimit::SetMinimumVertexPositionSet(float value){
	mpMinVertexPositionSet = value;
}

void aeRuleLimit::SetMaximumVertexPositionSet(float value){
	mpMaxVertexPositionSet = value;
}

void aeRuleLimit::SetTargetBone(const char *value){
	mpTargetBone = value;
}

void aeRuleLimit::SetCoordinateFrame(deAnimatorRuleLimit::eCoordinateFrames value){
	mpCoordinateFrame = value;
}

void aeRuleLimit::SetEnablePositionXMin(bool value){
	mpEnablePositionXMin = value;
}

void aeRuleLimit::SetEnablePositionXMax(bool value){
	mpEnablePositionXMax = value;
}

void aeRuleLimit::SetEnablePositionYMin(bool value){
	mpEnablePositionYMin = value;
}

void aeRuleLimit::SetEnablePositionYMax(bool value){
	mpEnablePositionYMax = value;
}

void aeRuleLimit::SetEnablePositionZMin(bool value){
	mpEnablePositionZMin = value;
}

void aeRuleLimit::SetEnablePositionZMax(bool value){
	mpEnablePositionZMax = value;
}

void aeRuleLimit::SetEnableRotationXMin(bool value){
	mpEnableRotationXMin = value;
}

void aeRuleLimit::SetEnableRotationXMax(bool value){
	mpEnableRotationXMax = value;
}

void aeRuleLimit::SetEnableRotationYMin(bool value){
	mpEnableRotationYMin = value;
}

void aeRuleLimit::SetEnableRotationYMax(bool value){
	mpEnableRotationYMax = value;
}

void aeRuleLimit::SetEnableRotationZMin(bool value){
	mpEnableRotationZMin = value;
}

void aeRuleLimit::SetEnableRotationZMax(bool value){
	mpEnableRotationZMax = value;
}

void aeRuleLimit::SetEnableScalingXMin(bool value){
	mpEnableScalingXMin = value;
}

void aeRuleLimit::SetEnableScalingXMax(bool value){
	mpEnableScalingXMax = value;
}

void aeRuleLimit::SetEnableScalingYMin(bool value){
	mpEnableScalingYMin = value;
}

void aeRuleLimit::SetEnableScalingYMax(bool value){
	mpEnableScalingYMax = value;
}

void aeRuleLimit::SetEnableScalingZMin(bool value){
	mpEnableScalingZMin = value;
}

void aeRuleLimit::SetEnableScalingZMax(bool value){
	mpEnableScalingZMax = value;
}

void aeRuleLimit::SetEnableVertexPositionSetMin(bool value){
	mpEnableVertexPositionSetMin = value;
}

void aeRuleLimit::SetEnableVertexPositionSetMax(bool value){
	mpEnableVertexPositionSetMax = value;
}


deAnimatorRule::Ref aeRuleLimit::CreateEngineRule(){
	const deAnimatorRuleLimit::Ref engRule(deAnimatorRuleLimit::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMinimumPosition(mpMinPosition);
	engRule->SetMaximumPosition(mpMaxPosition);
	engRule->SetMinimumRotation(mpMinRotation.GetEulerAnglesRadians());
	engRule->SetMaximumRotation(mpMaxRotation.GetEulerAnglesRadians());
	engRule->SetMinimumScaling(mpMinScaling);
	engRule->SetMaximumScaling(mpMaxScaling);
	engRule->SetMinimumVertexPositionSet(mpMinVertexPositionSet);
	engRule->SetMaximumVertexPositionSet(mpMaxVertexPositionSet);
	engRule->SetCoordinateFrame(mpCoordinateFrame);
	engRule->SetTargetBone(mpTargetBone);
	
	engRule->SetEnablePositionXMin(mpEnablePositionXMin);
	engRule->SetEnablePositionXMax(mpEnablePositionXMax);
	engRule->SetEnablePositionYMin(mpEnablePositionYMin);
	engRule->SetEnablePositionYMax(mpEnablePositionYMax);
	engRule->SetEnablePositionZMin(mpEnablePositionZMin);
	engRule->SetEnablePositionZMax(mpEnablePositionZMax);
	
	engRule->SetEnableRotationXMin(mpEnableRotationXMin);
	engRule->SetEnableRotationXMax(mpEnableRotationXMax);
	engRule->SetEnableRotationYMin(mpEnableRotationYMin);
	engRule->SetEnableRotationYMax(mpEnableRotationYMax);
	engRule->SetEnableRotationZMin(mpEnableRotationZMin);
	engRule->SetEnableRotationZMax(mpEnableRotationZMax);
	
	engRule->SetEnableScalingXMin(mpEnableScalingXMin);
	engRule->SetEnableScalingXMax(mpEnableScalingXMax);
	engRule->SetEnableScalingYMin(mpEnableScalingYMin);
	engRule->SetEnableScalingYMax(mpEnableScalingYMax);
	engRule->SetEnableScalingZMin(mpEnableScalingZMin);
	engRule->SetEnableScalingZMax(mpEnableScalingZMax);
	
	engRule->SetEnableVertexPositionSetMin(mpEnableVertexPositionSetMin);
	engRule->SetEnableVertexPositionSetMax(mpEnableVertexPositionSetMax);
	
	return engRule;
}



aeRule::Ref aeRuleLimit::CreateCopy() const{
	return Ref::New(*this);
}
