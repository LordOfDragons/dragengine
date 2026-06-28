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
pMPMinPosition(windowMain.GetMCAnimatorProperties().ruleLimit.minPosition, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPMaxPosition(windowMain.GetMCAnimatorProperties().ruleLimit.maxPosition, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPMinRotation(windowMain.GetMCAnimatorProperties().ruleLimit.minRotation, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPMaxRotation(windowMain.GetMCAnimatorProperties().ruleLimit.maxRotation, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPMinScaling(windowMain.GetMCAnimatorProperties().ruleLimit.minScaling, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPMaxScaling(windowMain.GetMCAnimatorProperties().ruleLimit.maxScaling, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPMinVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleLimit.minVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPMaxVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleLimit.maxVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPTargetBone(windowMain.GetMCAnimatorProperties().ruleLimit.targetBone, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleLimit.coordinateFrame, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnablePositionXMin(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionXMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnablePositionXMax(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionXMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnablePositionYMin(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionYMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnablePositionYMax(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionYMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnablePositionZMin(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionZMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnablePositionZMax(windowMain.GetMCAnimatorProperties().ruleLimit.enablePositionZMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableRotationXMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationXMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableRotationXMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationXMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableRotationYMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationYMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableRotationYMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationYMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableRotationZMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationZMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableRotationZMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableRotationZMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableScalingXMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingXMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableScalingXMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingXMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableScalingYMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingYMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableScalingYMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingYMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableScalingZMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingZMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableScalingZMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableScalingZMax, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableVertexPositionSetMin(windowMain.GetMCAnimatorProperties().ruleLimit.enableVertexPositionSetMin, GetMetaContext().StaticCast<aeMCRuleLimit>()),
pMPEnableVertexPositionSetMax(windowMain.GetMCAnimatorProperties().ruleLimit.enableVertexPositionSetMax, GetMetaContext().StaticCast<aeMCRuleLimit>())
{
	pMPMinPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumPosition(pMPMinPosition);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumPosition(pMPMaxPosition);
		}
		NotifyRuleChanged();
	};
	
	pMPMinRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumRotation(pMPMinRotation);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumRotation(pMPMaxRotation);
		}
		NotifyRuleChanged();
	};
	
	pMPMinScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumScaling(pMPMinScaling);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxScaling.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumScaling(pMPMaxScaling);
		}
		NotifyRuleChanged();
	};
	
	pMPMinVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMinimumVertexPositionSet(pMPMinVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pMPMaxVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetMaximumVertexPositionSet(pMPMaxVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pMPTargetBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetTargetBone(pMPTargetBone);
		}
		NotifyRuleChanged();
	};
	
	pMPCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetCoordinateFrame(pMPCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMin(pMPEnablePositionXMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMax(pMPEnablePositionXMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMin(pMPEnablePositionYMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMax(pMPEnablePositionYMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMin(pMPEnablePositionZMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMax(pMPEnablePositionZMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMin(pMPEnableRotationXMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMax(pMPEnableRotationXMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMin(pMPEnableRotationYMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMax(pMPEnableRotationYMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMin(pMPEnableRotationZMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMax(pMPEnableRotationZMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMin(pMPEnableScalingXMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMax(pMPEnableScalingXMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMin(pMPEnableScalingYMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMax(pMPEnableScalingYMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMin(pMPEnableScalingZMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMax(pMPEnableScalingZMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSetMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMin(pMPEnableVertexPositionSetMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSetMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMax(pMPEnableVertexPositionSetMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMin(pMPEnablePositionXMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionXMax(pMPEnablePositionXMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMin(pMPEnablePositionYMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionYMax(pMPEnablePositionYMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMin(pMPEnablePositionZMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePositionZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnablePositionZMax(pMPEnablePositionZMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMin(pMPEnableRotationXMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationXMax(pMPEnableRotationXMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMin(pMPEnableRotationYMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationYMax(pMPEnableRotationYMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMin(pMPEnableRotationZMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableRotationZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableRotationZMax(pMPEnableRotationZMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingXMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMin(pMPEnableScalingXMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingXMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingXMax(pMPEnableScalingXMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingYMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMin(pMPEnableScalingYMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingYMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingYMax(pMPEnableScalingYMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingZMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMin(pMPEnableScalingZMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableScalingZMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableScalingZMax(pMPEnableScalingZMax);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSetMin.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMin(pMPEnableVertexPositionSetMin);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSetMax.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleLimit*)GetEngineRule())->SetEnableVertexPositionSetMax(pMPEnableVertexPositionSetMax);
		}
		NotifyRuleChanged();
	};
}

aeRuleLimit::aeRuleLimit(aeWindowMain &windowMain, const aeRuleLimit &copy) :
aeRuleLimit(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPMaxPosition.SetValue(copy.pMPMaxPosition, false);
	pMPMinRotation.SetValue(copy.pMPMinRotation, false);
	pMPMaxRotation.SetValue(copy.pMPMaxRotation, false);
	pMPMinScaling.SetValue(copy.pMPMinScaling, false);
	pMPMaxScaling.SetValue(copy.pMPMaxScaling, false);
	pMPMinVertexPositionSet.SetValue(copy.pMPMinVertexPositionSet, false);
	pMPMaxVertexPositionSet.SetValue(copy.pMPMaxVertexPositionSet, false);
	pMPTargetBone.SetValue(copy.pMPTargetBone, false);
	pMPCoordinateFrame.SetValue(copy.pMPCoordinateFrame, false);
	pMPEnablePositionXMin.SetValue(copy.pMPEnablePositionXMin, false);
	pMPEnablePositionXMax.SetValue(copy.pMPEnablePositionXMax, false);
	pMPEnablePositionYMin.SetValue(copy.pMPEnablePositionYMin, false);
	pMPEnablePositionYMax.SetValue(copy.pMPEnablePositionYMax, false);
	pMPEnablePositionZMin.SetValue(copy.pMPEnablePositionZMin, false);
	pMPEnablePositionZMax.SetValue(copy.pMPEnablePositionZMax, false);
	pMPEnableRotationXMin.SetValue(copy.pMPEnableRotationXMin, false);
	pMPEnableRotationXMax.SetValue(copy.pMPEnableRotationXMax, false);
	pMPEnableRotationYMin.SetValue(copy.pMPEnableRotationYMin, false);
	pMPEnableRotationYMax.SetValue(copy.pMPEnableRotationYMax, false);
	pMPEnableRotationZMin.SetValue(copy.pMPEnableRotationZMin, false);
	pMPEnableRotationZMax.SetValue(copy.pMPEnableRotationZMax, false);
	pMPEnableScalingXMin.SetValue(copy.pMPEnableScalingXMin, false);
	pMPEnableScalingXMax.SetValue(copy.pMPEnableScalingXMax, false);
	pMPEnableScalingYMin.SetValue(copy.pMPEnableScalingYMin, false);
	pMPEnableScalingYMax.SetValue(copy.pMPEnableScalingYMax, false);
	pMPEnableScalingZMin.SetValue(copy.pMPEnableScalingZMin, false);
	pMPEnableScalingZMax.SetValue(copy.pMPEnableScalingZMax, false);
	pMPEnableVertexPositionSetMin.SetValue(copy.pMPEnableVertexPositionSetMin, false);
	pMPEnableVertexPositionSetMax.SetValue(copy.pMPEnableVertexPositionSetMax, false);
}

aeRuleLimit::~aeRuleLimit() = default;


// Management
///////////////

void aeRuleLimit::SetMinimumPosition(const decVector &value){
	pMPMinPosition = value;
}

void aeRuleLimit::SetMaximumPosition(const decVector &value){
	pMPMaxPosition = value;
}

void aeRuleLimit::SetMinimumRotation(const decVector &value){
	pMPMinRotation = value;
}

void aeRuleLimit::SetMaximumRotation(const decVector &value){
	pMPMaxRotation = value;
}

void aeRuleLimit::SetMinimumScaling(const decVector &value){
	pMPMinScaling = value;
}

void aeRuleLimit::SetMaximumScaling(const decVector &value){
	pMPMaxScaling = value;
}

void aeRuleLimit::SetMinimumVertexPositionSet(float value){
	pMPMinVertexPositionSet = value;
}

void aeRuleLimit::SetMaximumVertexPositionSet(float value){
	pMPMaxVertexPositionSet = value;
}

void aeRuleLimit::SetTargetBone(const char *value){
	pMPTargetBone = value;
}

void aeRuleLimit::SetCoordinateFrame(deAnimatorRuleLimit::eCoordinateFrames value){
	pMPCoordinateFrame = value;
}

void aeRuleLimit::SetEnablePositionXMin(bool value){
	pMPEnablePositionXMin = value;
}

void aeRuleLimit::SetEnablePositionXMax(bool value){
	pMPEnablePositionXMax = value;
}

void aeRuleLimit::SetEnablePositionYMin(bool value){
	pMPEnablePositionYMin = value;
}

void aeRuleLimit::SetEnablePositionYMax(bool value){
	pMPEnablePositionYMax = value;
}

void aeRuleLimit::SetEnablePositionZMin(bool value){
	pMPEnablePositionZMin = value;
}

void aeRuleLimit::SetEnablePositionZMax(bool value){
	pMPEnablePositionZMax = value;
}

void aeRuleLimit::SetEnableRotationXMin(bool value){
	pMPEnableRotationXMin = value;
}

void aeRuleLimit::SetEnableRotationXMax(bool value){
	pMPEnableRotationXMax = value;
}

void aeRuleLimit::SetEnableRotationYMin(bool value){
	pMPEnableRotationYMin = value;
}

void aeRuleLimit::SetEnableRotationYMax(bool value){
	pMPEnableRotationYMax = value;
}

void aeRuleLimit::SetEnableRotationZMin(bool value){
	pMPEnableRotationZMin = value;
}

void aeRuleLimit::SetEnableRotationZMax(bool value){
	pMPEnableRotationZMax = value;
}

void aeRuleLimit::SetEnableScalingXMin(bool value){
	pMPEnableScalingXMin = value;
}

void aeRuleLimit::SetEnableScalingXMax(bool value){
	pMPEnableScalingXMax = value;
}

void aeRuleLimit::SetEnableScalingYMin(bool value){
	pMPEnableScalingYMin = value;
}

void aeRuleLimit::SetEnableScalingYMax(bool value){
	pMPEnableScalingYMax = value;
}

void aeRuleLimit::SetEnableScalingZMin(bool value){
	pMPEnableScalingZMin = value;
}

void aeRuleLimit::SetEnableScalingZMax(bool value){
	pMPEnableScalingZMax = value;
}

void aeRuleLimit::SetEnableVertexPositionSetMin(bool value){
	pMPEnableVertexPositionSetMin = value;
}

void aeRuleLimit::SetEnableVertexPositionSetMax(bool value){
	pMPEnableVertexPositionSetMax = value;
}


deAnimatorRule::Ref aeRuleLimit::CreateEngineRule(){
	const deAnimatorRuleLimit::Ref engRule(deAnimatorRuleLimit::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMinimumPosition(pMPMinPosition);
	engRule->SetMaximumPosition(pMPMaxPosition);
	engRule->SetMinimumRotation(pMPMinRotation.GetEulerAnglesRadians());
	engRule->SetMaximumRotation(pMPMaxRotation.GetEulerAnglesRadians());
	engRule->SetMinimumScaling(pMPMinScaling);
	engRule->SetMaximumScaling(pMPMaxScaling);
	engRule->SetMinimumVertexPositionSet(pMPMinVertexPositionSet);
	engRule->SetMaximumVertexPositionSet(pMPMaxVertexPositionSet);
	engRule->SetCoordinateFrame(pMPCoordinateFrame);
	engRule->SetTargetBone(pMPTargetBone);
	
	engRule->SetEnablePositionXMin(pMPEnablePositionXMin);
	engRule->SetEnablePositionXMax(pMPEnablePositionXMax);
	engRule->SetEnablePositionYMin(pMPEnablePositionYMin);
	engRule->SetEnablePositionYMax(pMPEnablePositionYMax);
	engRule->SetEnablePositionZMin(pMPEnablePositionZMin);
	engRule->SetEnablePositionZMax(pMPEnablePositionZMax);
	
	engRule->SetEnableRotationXMin(pMPEnableRotationXMin);
	engRule->SetEnableRotationXMax(pMPEnableRotationXMax);
	engRule->SetEnableRotationYMin(pMPEnableRotationYMin);
	engRule->SetEnableRotationYMax(pMPEnableRotationYMax);
	engRule->SetEnableRotationZMin(pMPEnableRotationZMin);
	engRule->SetEnableRotationZMax(pMPEnableRotationZMax);
	
	engRule->SetEnableScalingXMin(pMPEnableScalingXMin);
	engRule->SetEnableScalingXMax(pMPEnableScalingXMax);
	engRule->SetEnableScalingYMin(pMPEnableScalingYMin);
	engRule->SetEnableScalingYMax(pMPEnableScalingYMax);
	engRule->SetEnableScalingZMin(pMPEnableScalingZMin);
	engRule->SetEnableScalingZMax(pMPEnableScalingZMax);
	
	engRule->SetEnableVertexPositionSetMin(pMPEnableVertexPositionSetMin);
	engRule->SetEnableVertexPositionSetMax(pMPEnableVertexPositionSetMax);
	
	return engRule;
}



aeRule::Ref aeRuleLimit::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleLimit::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
}
