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

// Constructor, destructor
////////////////////////////

aeRuleLimit::aeRuleLimit(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleLimit::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertLimit, aname),
minPosition(windowMain.GetMCAnimatorProperties().ruleLimit.minPosition, GetMetaContext().StaticCast<aeMCRuleLimit>()),
maxPosition(windowMain.GetMCAnimatorProperties().ruleLimit.maxPosition, GetMetaContext().StaticCast<aeMCRuleLimit>()),
minRotation(windowMain.GetMCAnimatorProperties().ruleLimit.minRotation, GetMetaContext().StaticCast<aeMCRuleLimit>()),
maxRotation(windowMain.GetMCAnimatorProperties().ruleLimit.maxRotation, GetMetaContext().StaticCast<aeMCRuleLimit>()),
minScaling(windowMain.GetMCAnimatorProperties().ruleLimit.minScaling, GetMetaContext().StaticCast<aeMCRuleLimit>()),
maxScaling(windowMain.GetMCAnimatorProperties().ruleLimit.maxScaling, GetMetaContext().StaticCast<aeMCRuleLimit>()),
minVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleLimit.minVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleLimit>()),
maxVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleLimit.maxVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleLimit>()),
targetBone(windowMain.GetMCAnimatorProperties().ruleLimit.targetBone, GetMetaContext().StaticCast<aeMCRuleLimit>()),
coordinateFrame(windowMain.GetMCAnimatorProperties().ruleLimit.coordinateFrame, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enablePositionXMin(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionXMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enablePositionXMax(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionXMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enablePositionYMin(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionYMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enablePositionYMax(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionYMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enablePositionZMin(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionZMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enablePositionZMax(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionZMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableRotationXMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationXMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableRotationXMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationXMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableRotationYMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationYMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableRotationYMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationYMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableRotationZMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationZMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableRotationZMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationZMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableScalingXMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingXMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableScalingXMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingXMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableScalingYMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingYMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableScalingYMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingYMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableScalingZMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingZMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableScalingZMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingZMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableVertexPositionSetMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableVertexPositionSetMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
enableVertexPositionSetMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableVertexPositionSetMax, GetMetaContext().StaticCast<aeMCRuleLimit>())
{
	minPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumPosition(minPosition);
		}
		NotifyRuleChanged();
	};
	
	maxPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumPosition(maxPosition);
		}
		NotifyRuleChanged();
	};
	
	minRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumRotation(minRotation);
		}
		NotifyRuleChanged();
	};
	
	maxRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumRotation(maxRotation);
		}
		NotifyRuleChanged();
	};
	
	minScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumScaling(minScaling);
		}
		NotifyRuleChanged();
	};
	
	maxScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumScaling(maxScaling);
		}
		NotifyRuleChanged();
	};
	
	minVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumVertexPositionSet(minVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	maxVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumVertexPositionSet(maxVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	targetBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetTargetBone(targetBone);
		}
		NotifyRuleChanged();
	};
	
	coordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetCoordinateFrame(coordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	enablePositionXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMin(enablePositionXMin);
		}
		NotifyRuleChanged();
	};
	
	enablePositionXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMax(enablePositionXMax);
		}
		NotifyRuleChanged();
	};
	
	enablePositionYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMin(enablePositionYMin);
		}
		NotifyRuleChanged();
	};
	
	enablePositionYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMax(enablePositionYMax);
		}
		NotifyRuleChanged();
	};
	
	enablePositionZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMin(enablePositionZMin);
		}
		NotifyRuleChanged();
	};
	
	enablePositionZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMax(enablePositionZMax);
		}
		NotifyRuleChanged();
	};
	
	enableRotationXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMin(enableRotationXMin);
		}
		NotifyRuleChanged();
	};
	
	enableRotationXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMax(enableRotationXMax);
		}
		NotifyRuleChanged();
	};
	
	enableRotationYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMin(enableRotationYMin);
		}
		NotifyRuleChanged();
	};
	
	enableRotationYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMax(enableRotationYMax);
		}
		NotifyRuleChanged();
	};
	
	enableRotationZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMin(enableRotationZMin);
		}
		NotifyRuleChanged();
	};
	
	enableRotationZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMax(enableRotationZMax);
		}
		NotifyRuleChanged();
	};
	
	enableScalingXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMin(enableScalingXMin);
		}
		NotifyRuleChanged();
	};
	
	enableScalingXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMax(enableScalingXMax);
		}
		NotifyRuleChanged();
	};
	
	enableScalingYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMin(enableScalingYMin);
		}
		NotifyRuleChanged();
	};
	
	enableScalingYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMax(enableScalingYMax);
		}
		NotifyRuleChanged();
	};
	
	enableScalingZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMin(enableScalingZMin);
		}
		NotifyRuleChanged();
	};
	
	enableScalingZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMax(enableScalingZMax);
		}
		NotifyRuleChanged();
	};
	
	enableVertexPositionSetMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMin(enableVertexPositionSetMin);
		}
		NotifyRuleChanged();
	};
	
	enableVertexPositionSetMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMax(enableVertexPositionSetMax);
		}
		NotifyRuleChanged();
	};
	
	enablePositionXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMin(enablePositionXMin);
		}
		NotifyRuleChanged();
	};
	
	enablePositionXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMax(enablePositionXMax);
		}
		NotifyRuleChanged();
	};
	
	enablePositionYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMin(enablePositionYMin);
		}
		NotifyRuleChanged();
	};
	
	enablePositionYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMax(enablePositionYMax);
		}
		NotifyRuleChanged();
	};
	
	enablePositionZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMin(enablePositionZMin);
		}
		NotifyRuleChanged();
	};
	
	enablePositionZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMax(enablePositionZMax);
		}
		NotifyRuleChanged();
	};
	
	enableRotationXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMin(enableRotationXMin);
		}
		NotifyRuleChanged();
	};
	
	enableRotationXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMax(enableRotationXMax);
		}
		NotifyRuleChanged();
	};
	
	enableRotationYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMin(enableRotationYMin);
		}
		NotifyRuleChanged();
	};
	
	enableRotationYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMax(enableRotationYMax);
		}
		NotifyRuleChanged();
	};
	
	enableRotationZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMin(enableRotationZMin);
		}
		NotifyRuleChanged();
	};
	
	enableRotationZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMax(enableRotationZMax);
		}
		NotifyRuleChanged();
	};
	
	enableScalingXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMin(enableScalingXMin);
		}
		NotifyRuleChanged();
	};
	
	enableScalingXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMax(enableScalingXMax);
		}
		NotifyRuleChanged();
	};
	
	enableScalingYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMin(enableScalingYMin);
		}
		NotifyRuleChanged();
	};
	
	enableScalingYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMax(enableScalingYMax);
		}
		NotifyRuleChanged();
	};
	
	enableScalingZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMin(enableScalingZMin);
		}
		NotifyRuleChanged();
	};
	
	enableScalingZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMax(enableScalingZMax);
		}
		NotifyRuleChanged();
	};
	
	enableVertexPositionSetMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMin(enableVertexPositionSetMin);
		}
		NotifyRuleChanged();
	};
	
	enableVertexPositionSetMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMax(enableVertexPositionSetMax);
		}
		NotifyRuleChanged();
	};
}

aeRuleLimit::aeRuleLimit(aeWindowMain &windowMain, const aeRuleLimit &copy) :
aeRuleLimit(windowMain, copy.name)
{
	pInitCopy(copy);
	maxPosition.SetValue(copy.maxPosition, false);
	minRotation.SetValue(copy.minRotation, false);
	maxRotation.SetValue(copy.maxRotation, false);
	minScaling.SetValue(copy.minScaling, false);
	maxScaling.SetValue(copy.maxScaling, false);
	minVertexPositionSet.SetValue(copy.minVertexPositionSet, false);
	maxVertexPositionSet.SetValue(copy.maxVertexPositionSet, false);
	targetBone.SetValue(copy.targetBone, false);
	coordinateFrame.SetValue(copy.coordinateFrame, false);
	enablePositionXMin.SetValue(copy.enablePositionXMin, false);
	enablePositionXMax.SetValue(copy.enablePositionXMax, false);
	enablePositionYMin.SetValue(copy.enablePositionYMin, false);
	enablePositionYMax.SetValue(copy.enablePositionYMax, false);
	enablePositionZMin.SetValue(copy.enablePositionZMin, false);
	enablePositionZMax.SetValue(copy.enablePositionZMax, false);
	enableRotationXMin.SetValue(copy.enableRotationXMin, false);
	enableRotationXMax.SetValue(copy.enableRotationXMax, false);
	enableRotationYMin.SetValue(copy.enableRotationYMin, false);
	enableRotationYMax.SetValue(copy.enableRotationYMax, false);
	enableRotationZMin.SetValue(copy.enableRotationZMin, false);
	enableRotationZMax.SetValue(copy.enableRotationZMax, false);
	enableScalingXMin.SetValue(copy.enableScalingXMin, false);
	enableScalingXMax.SetValue(copy.enableScalingXMax, false);
	enableScalingYMin.SetValue(copy.enableScalingYMin, false);
	enableScalingYMax.SetValue(copy.enableScalingYMax, false);
	enableScalingZMin.SetValue(copy.enableScalingZMin, false);
	enableScalingZMax.SetValue(copy.enableScalingZMax, false);
	enableVertexPositionSetMin.SetValue(copy.enableVertexPositionSetMin, false);
	enableVertexPositionSetMax.SetValue(copy.enableVertexPositionSetMax, false);
}

aeRuleLimit::~aeRuleLimit() = default;


// Management
///////////////

void aeRuleLimit::SetMinimumPosition(const decVector &value){
	minPosition = value;
}

void aeRuleLimit::SetMaximumPosition(const decVector &value){
	maxPosition = value;
}

void aeRuleLimit::SetMinimumRotation(const decVector &value){
	minRotation = value;
}

void aeRuleLimit::SetMaximumRotation(const decVector &value){
	maxRotation = value;
}

void aeRuleLimit::SetMinimumScaling(const decVector &value){
	minScaling = value;
}

void aeRuleLimit::SetMaximumScaling(const decVector &value){
	maxScaling = value;
}

void aeRuleLimit::SetMinimumVertexPositionSet(float value){
	minVertexPositionSet = value;
}

void aeRuleLimit::SetMaximumVertexPositionSet(float value){
	maxVertexPositionSet = value;
}

void aeRuleLimit::SetTargetBone(const char *value){
	targetBone = value;
}

void aeRuleLimit::SetCoordinateFrame(deAnimatorRuleLimit::eCoordinateFrames value){
	coordinateFrame = value;
}

void aeRuleLimit::SetEnablePositionXMin(bool value){
	enablePositionXMin = value;
}

void aeRuleLimit::SetEnablePositionXMax(bool value){
	enablePositionXMax = value;
}

void aeRuleLimit::SetEnablePositionYMin(bool value){
	enablePositionYMin = value;
}

void aeRuleLimit::SetEnablePositionYMax(bool value){
	enablePositionYMax = value;
}

void aeRuleLimit::SetEnablePositionZMin(bool value){
	enablePositionZMin = value;
}

void aeRuleLimit::SetEnablePositionZMax(bool value){
	enablePositionZMax = value;
}

void aeRuleLimit::SetEnableRotationXMin(bool value){
	enableRotationXMin = value;
}

void aeRuleLimit::SetEnableRotationXMax(bool value){
	enableRotationXMax = value;
}

void aeRuleLimit::SetEnableRotationYMin(bool value){
	enableRotationYMin = value;
}

void aeRuleLimit::SetEnableRotationYMax(bool value){
	enableRotationYMax = value;
}

void aeRuleLimit::SetEnableRotationZMin(bool value){
	enableRotationZMin = value;
}

void aeRuleLimit::SetEnableRotationZMax(bool value){
	enableRotationZMax = value;
}

void aeRuleLimit::SetEnableScalingXMin(bool value){
	enableScalingXMin = value;
}

void aeRuleLimit::SetEnableScalingXMax(bool value){
	enableScalingXMax = value;
}

void aeRuleLimit::SetEnableScalingYMin(bool value){
	enableScalingYMin = value;
}

void aeRuleLimit::SetEnableScalingYMax(bool value){
	enableScalingYMax = value;
}

void aeRuleLimit::SetEnableScalingZMin(bool value){
	enableScalingZMin = value;
}

void aeRuleLimit::SetEnableScalingZMax(bool value){
	enableScalingZMax = value;
}

void aeRuleLimit::SetEnableVertexPositionSetMin(bool value){
	enableVertexPositionSetMin = value;
}

void aeRuleLimit::SetEnableVertexPositionSetMax(bool value){
	enableVertexPositionSetMax = value;
}


deAnimatorRule::Ref aeRuleLimit::CreateEngineRule(){
	const deAnimatorRuleLimit::Ref engRule(deAnimatorRuleLimit::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMinimumPosition(minPosition);
	engRule->SetMaximumPosition(maxPosition);
	engRule->SetMinimumRotation(minRotation.GetValue() * DEG2RAD);
	engRule->SetMaximumRotation(maxRotation.GetValue() * DEG2RAD);
	engRule->SetMinimumScaling(minScaling);
	engRule->SetMaximumScaling(maxScaling);
	engRule->SetMinimumVertexPositionSet(minVertexPositionSet);
	engRule->SetMaximumVertexPositionSet(maxVertexPositionSet);
	engRule->SetCoordinateFrame(coordinateFrame);
	engRule->SetTargetBone(targetBone);
	
	engRule->SetEnablePositionXMin(enablePositionXMin);
	engRule->SetEnablePositionXMax(enablePositionXMax);
	engRule->SetEnablePositionYMin(enablePositionYMin);
	engRule->SetEnablePositionYMax(enablePositionYMax);
	engRule->SetEnablePositionZMin(enablePositionZMin);
	engRule->SetEnablePositionZMax(enablePositionZMax);
	
	engRule->SetEnableRotationXMin(enableRotationXMin);
	engRule->SetEnableRotationXMax(enableRotationXMax);
	engRule->SetEnableRotationYMin(enableRotationYMin);
	engRule->SetEnableRotationYMax(enableRotationYMax);
	engRule->SetEnableRotationZMin(enableRotationZMin);
	engRule->SetEnableRotationZMax(enableRotationZMax);
	
	engRule->SetEnableScalingXMin(enableScalingXMin);
	engRule->SetEnableScalingXMax(enableScalingXMax);
	engRule->SetEnableScalingYMin(enableScalingYMin);
	engRule->SetEnableScalingYMax(enableScalingYMax);
	engRule->SetEnableScalingZMin(enableScalingZMin);
	engRule->SetEnableScalingZMax(enableScalingZMax);
	
	engRule->SetEnableVertexPositionSetMin(enableVertexPositionSetMin);
	engRule->SetEnableVertexPositionSetMax(enableVertexPositionSetMax);
	
	return engRule;
}



aeRule::Ref aeRuleLimit::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleLimit::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
}
