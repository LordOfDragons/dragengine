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

#include "aeRuleStateSnapshot.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>


// Class aeRuleStateSnapshot
//////////////////////////////

aeRuleStateSnapshot::MetaContext::Ref aeRuleStateSnapshot::CreateMetaContext(aeWindowMain &windowMain, aeRuleStateSnapshot *rule){
	return MetaContext::Ref::New("animator.rule_state_snapshot", "Rule State Snapshot", "Rule state snapshot properties",
		windowMain.GetMCAnimatorProperties().ruleStateSnapshot.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleStateSnapshot::aeRuleStateSnapshot(aeWindowMain &windowMain, const char *aname) :
aeRuleStateSnapshot(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleStateSnapshot::aeRuleStateSnapshot(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertStateSnapshot, aname),
mpUseLastState(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.useLastState, metaContext),
mpId(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.id, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enablePosition, metaContext),
mpEnableOrientation(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enableOrientation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enableSize, metaContext),
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enableVertexPositionSet, metaContext)
{
	mpUseLastState.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetUseLastState(mpUseLastState);
		}
		NotifyRuleChanged();
	};
	
	mpId.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetID(mpId);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableOrientation(mpEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
}

aeRuleStateSnapshot::aeRuleStateSnapshot(const aeRuleStateSnapshot &copy) :
aeRuleStateSnapshot(copy.GetWindowMain(), copy.mpName)
{
	pInitCopy(copy);
	mpUseLastState.SetValue(copy.mpUseLastState, false);
	mpId.SetValue(copy.mpId, false);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableOrientation.SetValue(copy.mpEnableOrientation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpEnableVertexPositionSet.SetValue(copy.mpEnableVertexPositionSet, false);
}

aeRuleStateSnapshot::~aeRuleStateSnapshot() = default;


// Management
///////////////

deAnimatorRule::Ref aeRuleStateSnapshot::CreateEngineRule(){
	const deAnimatorRuleStateSnapshot::Ref engRule(deAnimatorRuleStateSnapshot::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetUseLastState(mpUseLastState);
	engRule->SetID(mpId);
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableOrientation(mpEnableOrientation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
	
	return engRule;
}


aeRule::Ref aeRuleStateSnapshot::CreateCopy() const{
	return Ref::New(*this);
}
