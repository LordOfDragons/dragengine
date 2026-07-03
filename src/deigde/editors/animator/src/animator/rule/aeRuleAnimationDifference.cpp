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

#include "aeRuleAnimationDifference.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationDifference.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleAnimationDifference
////////////////////////////////////

aeRuleAnimationDifference::MetaContext::Ref aeRuleAnimationDifference::CreateMetaContext(aeWindowMain &windowMain, aeRuleAnimationDifference *rule){
	return MetaContext::Ref::New("animator.rule_animation_difference", "Rule Animation Difference", "Rule animation difference properties",
		windowMain.GetMCAnimatorProperties().ruleAnimationDifference.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleAnimationDifference::aeRuleAnimationDifference(aeWindowMain &windowMain, const char *aname) :
aeRuleAnimationDifference(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleAnimationDifference::aeRuleAnimationDifference(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertAnimationDifference, aname),
mpLeadingMoveName(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.leadingMoveName, metaContext),
mpLeadingMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.leadingMoveTime, metaContext),
mpReferenceMoveName(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.referenceMoveName, metaContext),
mpReferenceMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.referenceMoveTime, metaContext),
mpUseComponentSpace(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.useComponentSpace, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enablePosition, metaContext),
mpEnableOrientation(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enableOrientation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enableSize, metaContext),
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.enableVertexPositionSet, metaContext),
mpTargetLeadingMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.targetLeadMoveTime, metaContext),
mpTargetReferenceMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.targetRefMoveTime, metaContext)
{
	mpLeadingMoveName.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetLeadingMoveName(mpLeadingMoveName);
		}
		NotifyRuleChanged();
	};
	
	mpLeadingMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetLeadingMoveTime(mpLeadingMoveTime);
		}
		NotifyRuleChanged();
	};
	
	mpReferenceMoveName.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetReferenceMoveName(mpReferenceMoveName);
		}
		NotifyRuleChanged();
	};
	
	mpReferenceMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetReferenceMoveTime(mpReferenceMoveTime);
		}
		NotifyRuleChanged();
	};
	
	mpUseComponentSpace.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetUseComponentSpace(mpUseComponentSpace);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnableOrientation(mpEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationDifference*)GetEngineRule())->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpTargetLeadingMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimationDifference*)GetEngineRule())->GetTargetLeadingMoveTime(), mpTargetLeadingMoveTime);
		}
		NotifyRuleChanged();
	};
	
	mpTargetReferenceMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimationDifference*)GetEngineRule())->GetTargetReferenceMoveTime(), mpTargetReferenceMoveTime);
		}
		NotifyRuleChanged();
	};
}

aeRuleAnimationDifference::aeRuleAnimationDifference(const aeRuleAnimationDifference &copy) :
aeRuleAnimationDifference(copy.GetWindowMain(), copy.GetName())
{
	pInitCopy(copy);
	mpLeadingMoveName.SetValue(copy.mpLeadingMoveName, false);
	mpLeadingMoveTime.SetValue(copy.mpLeadingMoveTime, false);
	mpReferenceMoveName.SetValue(copy.mpReferenceMoveName, false);
	mpReferenceMoveTime.SetValue(copy.mpReferenceMoveTime, false);
	mpUseComponentSpace.SetValue(copy.mpUseComponentSpace, false);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableOrientation.SetValue(copy.mpEnableOrientation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpEnableVertexPositionSet.SetValue(copy.mpEnableVertexPositionSet, false);
	mpTargetLeadingMoveTime.SetValue(copy.mpTargetLeadingMoveTime, false);
	mpTargetReferenceMoveTime.SetValue(copy.mpTargetReferenceMoveTime, false);
}

aeRuleAnimationDifference::~aeRuleAnimationDifference() = default;


// Management
///////////////

void aeRuleAnimationDifference::SetLeadingMoveName(const char *value){
	mpLeadingMoveName = value;
}

void aeRuleAnimationDifference::SetLeadingMoveTime(float value){
	mpLeadingMoveTime = value;
}

void aeRuleAnimationDifference::SetReferenceMoveName(const char *value){
	mpReferenceMoveName = value;
}

void aeRuleAnimationDifference::SetReferenceMoveTime(float value){
	mpReferenceMoveTime = value;
}

void aeRuleAnimationDifference::SetUseComponentSpace(bool value){
	mpUseComponentSpace = value;
}

void aeRuleAnimationDifference::SetEnablePosition(bool value){
	mpEnablePosition = value;
}

void aeRuleAnimationDifference::SetEnableOrientation(bool value){
	mpEnableOrientation = value;
}

void aeRuleAnimationDifference::SetEnableSize(bool value){
	mpEnableSize = value;
}

void aeRuleAnimationDifference::SetEnableVertexPositionSet(bool value){
	mpEnableVertexPositionSet = value;
}


void aeRuleAnimationDifference::UpdateTargets(){
	deAnimatorRuleAnimationDifference *rule = (deAnimatorRuleAnimationDifference*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetLeadingMoveTime(), mpTargetLeadingMoveTime);
		pUpdateEngineTarget(rule->GetTargetReferenceMoveTime(), mpTargetReferenceMoveTime);
	}
}

int aeRuleAnimationDifference::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(mpTargetLeadingMoveTime->Has(link)){
		usageCount++;
	}
	if(mpTargetReferenceMoveTime->Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

deAnimatorRule::Ref aeRuleAnimationDifference::CreateEngineRule(){
	// create rule
	const deAnimatorRuleAnimationDifference::Ref engRule(deAnimatorRuleAnimationDifference::Ref::New());
	
	// init rule
	InitEngineRule(engRule);
	
	engRule->SetLeadingMoveName(mpLeadingMoveName);
	engRule->SetLeadingMoveTime(mpLeadingMoveTime);
	engRule->SetReferenceMoveName(mpReferenceMoveName);
	engRule->SetReferenceMoveTime(mpReferenceMoveTime);
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableOrientation(mpEnableOrientation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
	engRule->SetUseComponentSpace(mpUseComponentSpace);
	
	pUpdateEngineTarget(engRule->GetTargetLeadingMoveTime(), mpTargetLeadingMoveTime);
	pUpdateEngineTarget(engRule->GetTargetReferenceMoveTime(), mpTargetReferenceMoveTime);
	
	// finished
	return engRule;
}



aeRule::Ref aeRuleAnimationDifference::CreateCopy() const{
	return Ref::New(*this);
}
