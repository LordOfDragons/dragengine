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

#include "aeRuleAnimationSelect.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationSelect.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleAnimationSelect
////////////////////////////////

aeRuleAnimationSelect::MetaContext::Ref aeRuleAnimationSelect::CreateMetaContext(aeWindowMain &windowMain, aeRuleAnimationSelect *rule){
	return MetaContext::Ref::New("animator.rule_animation_select", "Rule Animation Select", "Rule animation select properties",
		windowMain.GetMCAnimatorProperties().ruleAnimationSelect.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleAnimationSelect::aeRuleAnimationSelect(aeWindowMain &windowMain, const char *aname) :
aeRuleAnimationSelect(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleAnimationSelect::aeRuleAnimationSelect(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertAnimationSelect, aname),
mpMoves(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.moves, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enablePosition, metaContext),
mpEnableOrientation(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enableOrientation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enableSize, metaContext),
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.enableVertexPositionSet, metaContext),
mpTargetMoveTime(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.targetMoveTime, metaContext),
mpTargetSelect(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.targetSelect, metaContext)
{
	mpMoves.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->GetMoves() = mpMoves;
		}
		NotifyRuleChanged();
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableOrientation(mpEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleAnimationSelect*)GetEngineRule())->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpTargetMoveTime.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimationSelect*)GetEngineRule())->GetTargetMoveTime(), mpTargetMoveTime);
		}
		NotifyRuleChanged();
	};
	
	mpTargetSelect.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleAnimationSelect*)GetEngineRule())->GetTargetSelect(), mpTargetSelect);
		}
		NotifyRuleChanged();
	};
}

aeRuleAnimationSelect::aeRuleAnimationSelect(const aeRuleAnimationSelect &copy) :
aeRuleAnimationSelect(copy.GetWindowMain(), copy.GetName())
{
	pInitCopy(copy);
	mpMoves.SetValue(copy.mpMoves, false);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableOrientation.SetValue(copy.mpEnableOrientation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpEnableVertexPositionSet.SetValue(copy.mpEnableVertexPositionSet, false);
	mpTargetMoveTime.SetValue(copy.mpTargetMoveTime, false);
	mpTargetSelect.SetValue(copy.mpTargetSelect, false);
}

aeRuleAnimationSelect::~aeRuleAnimationSelect() = default;


// Management
///////////////

void aeRuleAnimationSelect::SetEnablePosition(bool value){
	mpEnablePosition.SetValue(value);
}

void aeRuleAnimationSelect::SetEnableOrientation(bool value){
	mpEnableOrientation.SetValue(value);
}

void aeRuleAnimationSelect::SetEnableSize(bool value){
	mpEnableSize.SetValue(value);
}

void aeRuleAnimationSelect::SetEnableVertexPositionSet(bool value){
	mpEnableVertexPositionSet.SetValue(value);
}


void aeRuleAnimationSelect::SetMoves(const decStringList &value){
	mpMoves.SetValue(value);
}


void aeRuleAnimationSelect::UpdateTargets(){
	deAnimatorRuleAnimationSelect *rule = (deAnimatorRuleAnimationSelect*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetMoveTime(), mpTargetMoveTime);
		pUpdateEngineTarget(rule->GetTargetSelect(), mpTargetSelect);
	}
}

int aeRuleAnimationSelect::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(mpTargetMoveTime->Has(link)){
		usageCount++;
	}
	if(mpTargetSelect->Has(link)){
		usageCount++;
	}
	
	return usageCount;
}


deAnimatorRule::Ref aeRuleAnimationSelect::CreateEngineRule(){
	const deAnimatorRuleAnimationSelect::Ref engRule(deAnimatorRuleAnimationSelect::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->GetMoves() = mpMoves;
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableOrientation(mpEnableOrientation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
	
	pUpdateEngineTarget(engRule->GetTargetMoveTime(), mpTargetMoveTime);
	pUpdateEngineTarget(engRule->GetTargetSelect(), mpTargetSelect);
	
	return engRule;
}



aeRule::Ref aeRuleAnimationSelect::CreateCopy() const{
	return Ref::New(*this);
}
