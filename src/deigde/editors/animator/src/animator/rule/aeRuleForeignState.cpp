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

#include "aeRuleForeignState.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>


// Class aeRuleForeignState
/////////////////////////////

aeRuleForeignState::MetaContext::Ref aeRuleForeignState::CreateMetaContext(aeWindowMain &windowMain, aeRuleForeignState *rule){
	return MetaContext::Ref::New("animator.rule_foreign_state", "Rule Foreign State", "Rule foreign state properties",
		windowMain.GetMCAnimatorProperties().ruleForeignState.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleForeignState::aeRuleForeignState(aeWindowMain &windowMain, const char *aname) :
aeRuleForeignState(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleForeignState::aeRuleForeignState(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertForeignState, aname),
mpForeignBone(windowMain.GetMCAnimatorProperties().ruleForeignState.foreignBone, metaContext),
mpForeignVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.foreignVertexPositionSet, metaContext),
mpScalePosition(windowMain.GetMCAnimatorProperties().ruleForeignState.scalePosition, metaContext),
mpScaleOrientation(windowMain.GetMCAnimatorProperties().ruleForeignState.scaleOrientation, metaContext),
mpScaleSize(windowMain.GetMCAnimatorProperties().ruleForeignState.scaleSize, metaContext),
mpScaleVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.scaleVertexPositionSet, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleForeignState.enablePosition, metaContext),
mpEnableOrientation(windowMain.GetMCAnimatorProperties().ruleForeignState.enableOrientation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleForeignState.enableSize, metaContext),
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.enableVertexPositionSet, metaContext),
mpSourceCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleForeignState.sourceCoordinateFrame, metaContext),
mpDestCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleForeignState.destCoordinateFrame, metaContext),
mpTargetPosition(windowMain.GetMCAnimatorProperties().ruleForeignState.targetPosition, metaContext),
mpTargetOrientation(windowMain.GetMCAnimatorProperties().ruleForeignState.targetOrientation, metaContext),
mpTargetSize(windowMain.GetMCAnimatorProperties().ruleForeignState.targetSize, metaContext),
mpTargetVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.targetVertexPositionSet, metaContext)
{
	mpForeignBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetForeignBone(mpForeignBone);
		}
		NotifyRuleChanged();
	};
	
	mpForeignVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetForeignVertexPositionSet(mpForeignVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpScalePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScalePosition(mpScalePosition);
		}
		NotifyRuleChanged();
	};
	
	mpScaleOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScaleOrientation(mpScaleOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpScaleSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScaleSize(mpScaleSize);
		}
		NotifyRuleChanged();
	};
	
	mpScaleVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScaleVertexPositionSet(mpScaleVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnableOrientation(mpEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpSourceCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetSourceCoordinateFrame(mpSourceCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	mpDestCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetDestCoordinateFrame(mpDestCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	mpTargetPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetPosition(), mpTargetPosition);
		}
		NotifyRuleChanged();
	};
	
	mpTargetOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetOrientation(), mpTargetOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpTargetSize.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetSize(), mpTargetSize);
		}
		NotifyRuleChanged();
	};
	
	mpTargetVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetVertexPositionSet(), mpTargetVertexPositionSet);
		}
		NotifyRuleChanged();
	};
}

aeRuleForeignState::aeRuleForeignState(const aeRuleForeignState &copy) :
aeRuleForeignState(copy.GetWindowMain(), copy.mpName)
{
	pInitCopy(copy);
	mpForeignBone.SetValue(copy.mpForeignBone, false);
	mpForeignVertexPositionSet.SetValue(copy.mpForeignVertexPositionSet, false);
	mpScalePosition.SetValue(copy.mpScalePosition, false);
	mpScaleOrientation.SetValue(copy.mpScaleOrientation, false);
	mpScaleSize.SetValue(copy.mpScaleSize, false);
	mpScaleVertexPositionSet.SetValue(copy.mpScaleVertexPositionSet, false);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableOrientation.SetValue(copy.mpEnableOrientation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpEnableVertexPositionSet.SetValue(copy.mpEnableVertexPositionSet, false);
	mpSourceCoordinateFrame.SetValue(copy.mpSourceCoordinateFrame, false);
	mpDestCoordinateFrame.SetValue(copy.mpDestCoordinateFrame, false);
	mpTargetPosition.SetValue(copy.mpTargetPosition, false);
	mpTargetOrientation.SetValue(copy.mpTargetOrientation, false);
	mpTargetSize.SetValue(copy.mpTargetSize, false);
	mpTargetVertexPositionSet.SetValue(copy.mpTargetVertexPositionSet, false);
}

aeRuleForeignState::~aeRuleForeignState() = default;


// Management
///////////////

void aeRuleForeignState::UpdateTargets(){
	deAnimatorRuleForeignState *rule = (deAnimatorRuleForeignState*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetPosition(), mpTargetPosition);
		pUpdateEngineTarget(rule->GetTargetOrientation(), mpTargetOrientation);
		pUpdateEngineTarget(rule->GetTargetSize(), mpTargetSize);
		pUpdateEngineTarget(rule->GetTargetVertexPositionSet(), mpTargetVertexPositionSet);
	}
}

int aeRuleForeignState::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(mpTargetPosition->Has(link)){
		usageCount++;
	}
	if(mpTargetOrientation->Has(link)){
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


deAnimatorRule::Ref aeRuleForeignState::CreateEngineRule(){
	const deAnimatorRuleForeignState::Ref engRule(deAnimatorRuleForeignState::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetForeignBone(mpForeignBone);
	engRule->SetForeignVertexPositionSet(mpForeignVertexPositionSet);
	engRule->SetScalePosition(mpScalePosition);
	engRule->SetScaleOrientation(mpScaleOrientation);
	engRule->SetScaleSize(mpScaleSize);
	engRule->SetScaleVertexPositionSet(mpScaleVertexPositionSet);
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableOrientation(mpEnableOrientation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
	engRule->SetSourceCoordinateFrame(mpSourceCoordinateFrame);
	engRule->SetDestCoordinateFrame(mpDestCoordinateFrame);
	
	pUpdateEngineTarget(engRule->GetTargetPosition(), mpTargetPosition);
	pUpdateEngineTarget(engRule->GetTargetOrientation(), mpTargetOrientation);
	pUpdateEngineTarget(engRule->GetTargetSize(), mpTargetSize);
	pUpdateEngineTarget(engRule->GetTargetVertexPositionSet(), mpTargetVertexPositionSet);
	
	return engRule;
}



aeRule::Ref aeRuleForeignState::CreateCopy() const{
	return Ref::New(*this);
}
