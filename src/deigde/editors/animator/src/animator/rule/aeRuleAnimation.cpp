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

#include "aeRuleAnimation.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleAnimation
//////////////////////////

aeRuleAnimation::MetaContext::Ref aeRuleAnimation::CreateMetaContext(aeWindowMain &windowMain, aeRuleAnimation *rule){
	return MetaContext::Ref::New("animator.rule_animation", "Rule Animation", "Rule animation properties",
		windowMain.GetMCAnimatorProperties().ruleAnimation.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleAnimation::aeRuleAnimation(aeWindowMain &windowMain, const char *aname) :
aeRuleAnimation(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleAnimation::aeRuleAnimation(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertAnimation, aname),
mpMoveName(windowMain.GetMCAnimatorProperties().ruleAnimation.moveName, metaContext),
mpMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimation.moveTime, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleAnimation.enablePosition, metaContext),
mpEnableOrientation(windowMain.GetMCAnimatorProperties().ruleAnimation.enableOrientation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleAnimation.enableSize, metaContext),
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleAnimation.enableVertexPositionSet, metaContext),
mpTargetMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimation.targetMoveTime, metaContext)
{
	mpMoveName.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetMoveName(mpMoveName);
		}
		NotifyRuleChanged();
	};
	
	mpMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetMoveTime(mpMoveTime);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnableOrientation(mpEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimation*)GetEngineRule())->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpTargetMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimation*)GetEngineRule())->GetTargetMoveTime(), mpTargetMoveTime);
		}
		NotifyRuleChanged();
	};
}

aeRuleAnimation::aeRuleAnimation(const aeRuleAnimation &copy) :
aeRuleAnimation(copy.GetWindowMain(), copy.mpName)
{
	pInitCopy(copy);
	mpMoveName.SetValue(copy.mpMoveName, false);
	mpMoveTime.SetValue(copy.mpMoveTime, false);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableOrientation.SetValue(copy.mpEnableOrientation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpEnableVertexPositionSet.SetValue(copy.mpEnableVertexPositionSet, false);
	mpTargetMoveTime.SetValue(copy.mpTargetMoveTime, false);
}

aeRuleAnimation::~aeRuleAnimation() = default;



// Management
///////////////

void aeRuleAnimation::UpdateTargets(){
	deAnimatorRuleAnimation *rule = (deAnimatorRuleAnimation*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetMoveTime(), mpTargetMoveTime);
	}
}

int aeRuleAnimation::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	if(mpTargetMoveTime->Has(link)){
		usageCount++;
	}
	return usageCount;
}


deAnimatorRule::Ref aeRuleAnimation::CreateEngineRule(){
	
	// create rule
	const deAnimatorRuleAnimation::Ref engRule(deAnimatorRuleAnimation::Ref::New());
	
	// init rule
	InitEngineRule(engRule);
	
	engRule->SetMoveName(mpMoveName);
	engRule->SetMoveTime(mpMoveTime);
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableOrientation(mpEnableOrientation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
	
	pUpdateEngineTarget(engRule->GetTargetMoveTime(), mpTargetMoveTime);
	
	// finished
	return engRule;
}



aeRule::Ref aeRuleAnimation::CreateCopy() const{
	return Ref::New(*this);
}
