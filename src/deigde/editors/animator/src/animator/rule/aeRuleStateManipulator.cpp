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

#include "aeRuleStateManipulator.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>


// Class aeRuleStateManipulator
/////////////////////////////////

aeRuleStateManipulator::MetaContext::Ref aeRuleStateManipulator::CreateMetaContext(aeWindowMain &windowMain, aeRuleStateManipulator *rule){
	return MetaContext::Ref::New("animator.rule_state_manipulator", "Rule State Manipulator", "Rule state manipulator properties",
		windowMain.GetMCAnimatorProperties().ruleStateManipulator.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleStateManipulator::aeRuleStateManipulator(aeWindowMain &windowMain, const char *aname) :
aeRuleStateManipulator(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleStateManipulator::aeRuleStateManipulator(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertStateManipulator, aname),
mpMinPosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minPosition, metaContext),
mpMaxPosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxPosition, metaContext),
mpMinRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minRotation, metaContext),
mpMaxRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxRotation, metaContext),
mpMinSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minSize, metaContext),
mpMaxSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxSize, metaContext),
mpMinVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.minVertexPositionSet, metaContext),
mpMaxVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.maxVertexPositionSet, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enablePosition, metaContext),
mpEnableRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enableRotation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enableSize, metaContext),
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.enableVertexPositionSet, metaContext),
mpTargetPosition(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetPosition, metaContext),
mpTargetRotation(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetRotation, metaContext),
mpTargetSize(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetSize, metaContext),
mpTargetVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateManipulator.targetVertexPositionSet, metaContext)
{
	mpMinPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumPosition(mpMinPosition);
		}
		NotifyRuleChanged();
	};
	
	mpMaxPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumPosition(mpMaxPosition);
		}
		NotifyRuleChanged();
	};
	
	mpMinRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumRotation(mpMinRotation.GetEulerAnglesRadians());
		}
		NotifyRuleChanged();
	};
	
	mpMaxRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumRotation(mpMaxRotation.GetEulerAnglesRadians());
		}
		NotifyRuleChanged();
	};
	
	mpMinSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumSize(mpMinSize);
		}
		NotifyRuleChanged();
	};
	
	mpMaxSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumSize(mpMaxSize);
		}
		NotifyRuleChanged();
	};
	
	mpMinVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMinimumVertexPositionSet(mpMinVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpMaxVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetMaximumVertexPositionSet(mpMaxVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnableRotation(mpEnableRotation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateManipulator*)GetEngineRule())->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpTargetPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetPosition(), mpTargetPosition);
		}
		NotifyRuleChanged();
	};
	
	mpTargetRotation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetRotation(), mpTargetRotation);
		}
		NotifyRuleChanged();
	};
	
	mpTargetSize.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetSize(), mpTargetSize);
		}
		NotifyRuleChanged();
	};
	
	mpTargetVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleStateManipulator*)GetEngineRule())->GetTargetVertexPositionSet(), mpTargetVertexPositionSet);
		}
		NotifyRuleChanged();
	};

}
aeRuleStateManipulator::aeRuleStateManipulator(const aeRuleStateManipulator &copy) :
aeRuleStateManipulator(copy.GetWindowMain(), copy.GetName())
{
	pInitCopy(copy);
	mpMinPosition.SetValue(copy.mpMinPosition, false);
	mpMaxPosition.SetValue(copy.mpMaxPosition, false);
	mpMinRotation.SetValue(copy.mpMinRotation, false);
	mpMaxRotation.SetValue(copy.mpMaxRotation, false);
	mpMinSize.SetValue(copy.mpMinSize, false);
	mpMaxSize.SetValue(copy.mpMaxSize, false);
	mpMinVertexPositionSet.SetValue(copy.mpMinVertexPositionSet, false);
	mpMaxVertexPositionSet.SetValue(copy.mpMaxVertexPositionSet, false);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableRotation.SetValue(copy.mpEnableRotation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpEnableVertexPositionSet.SetValue(copy.mpEnableVertexPositionSet, false);
}

aeRuleStateManipulator::~aeRuleStateManipulator() = default;

// Management
///////////////


void aeRuleStateManipulator::SetMinimumPosition(const decVector &value){
	mpMinPosition = value;
}

void aeRuleStateManipulator::SetMaximumPosition(const decVector &value){
	mpMaxPosition = value;
}

void aeRuleStateManipulator::SetMinimumRotation(const decVector &value){
	mpMinRotation = value;
}

void aeRuleStateManipulator::SetMaximumRotation(const decVector &value){
	mpMaxRotation = value;
}

void aeRuleStateManipulator::SetMinimumSize(const decVector &value){
	mpMinSize = value;
}

void aeRuleStateManipulator::SetMaximumSize(const decVector &value){
	mpMaxSize = value;
}

void aeRuleStateManipulator::SetMinimumVertexPositionSet(float value){
	mpMinVertexPositionSet = value;
}

void aeRuleStateManipulator::SetMaximumVertexPositionSet(float value){
	mpMaxVertexPositionSet = value;
}

void aeRuleStateManipulator::SetEnablePosition(bool value){
	mpEnablePosition = value;
}

void aeRuleStateManipulator::SetEnableRotation(bool value){
	mpEnableRotation = value;
}

void aeRuleStateManipulator::SetEnableSize(bool value){
	mpEnableSize = value;
}

void aeRuleStateManipulator::SetEnableVertexPositionSet(bool value){
	mpEnableVertexPositionSet = value;
}


void aeRuleStateManipulator::UpdateTargets(){
	deAnimatorRuleStateManipulator *rule = (deAnimatorRuleStateManipulator*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetPosition(), mpTargetPosition);
		pUpdateEngineTarget(rule->GetTargetRotation(), mpTargetRotation);
		pUpdateEngineTarget(rule->GetTargetSize(), mpTargetSize);
		pUpdateEngineTarget(rule->GetTargetVertexPositionSet(), mpTargetVertexPositionSet);
	}
}

int aeRuleStateManipulator::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(mpTargetPosition->Has(link)){
		usageCount++;
	}
	if(mpTargetRotation->Has(link)){
		usageCount++;
	}
	if(mpTargetSize->Has(link)){
		usageCount++;
	}
	if(mpTargetVertexPositionSet->Has(link)){
		usageCount++;
	}
	
	return usageCount;
}


deAnimatorRule::Ref aeRuleStateManipulator::CreateEngineRule(){
	const deAnimatorRuleStateManipulator::Ref engRule(deAnimatorRuleStateManipulator::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMinimumPosition(mpMinPosition);
	engRule->SetMaximumPosition(mpMaxPosition);
	engRule->SetMinimumRotation(mpMinRotation.GetEulerAnglesRadians());
	engRule->SetMaximumRotation(mpMaxRotation.GetEulerAnglesRadians());
	engRule->SetMinimumSize(mpMinSize);
	engRule->SetMaximumSize(mpMaxSize);
	engRule->SetMinimumVertexPositionSet(mpMinVertexPositionSet);
	engRule->SetMaximumVertexPositionSet(mpMaxVertexPositionSet);
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableRotation(mpEnableRotation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
	
	pUpdateEngineTarget(engRule->GetTargetPosition(), mpTargetPosition);
	pUpdateEngineTarget(engRule->GetTargetRotation(), mpTargetRotation);
	pUpdateEngineTarget(engRule->GetTargetSize(), mpTargetSize);
	pUpdateEngineTarget(engRule->GetTargetVertexPositionSet(), mpTargetVertexPositionSet);
	
	return engRule;
}



aeRule::Ref aeRuleStateManipulator::CreateCopy() const{
	return Ref::New(*this);
}
