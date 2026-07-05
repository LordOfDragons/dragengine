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

#include "aeRuleMirror.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>



// Class aeRuleMirror::MatchName
//////////////////////////////////

aeRuleMirror::MatchName::MetaContext::Ref aeRuleMirror::MatchName::CreateMetaContext(
aeWindowMain &windowMain, MatchName *matchName){
	return MetaContext::Ref::New("animator.rule_mirror_matchname",
		"Match Name", "Match name properties",
		windowMain.GetMCAnimatorProperties().ruleMirror.matchName.metaProperties, matchName);
}

aeRuleMirror::MatchName::MatchName(aeWindowMain &awindowMain) :
windowMain(awindowMain),
metaContext(CreateMetaContext(windowMain, this)),
mpFirst(windowMain.GetMCAnimatorProperties().ruleMirror.matchName.first, metaContext),
mpSecond(windowMain.GetMCAnimatorProperties().ruleMirror.matchName.second, metaContext),
mpType(windowMain.GetMCAnimatorProperties().ruleMirror.matchName.type, metaContext){
}

aeRuleMirror::MatchName::MatchName(aeWindowMain &awindowMain, const char *first,
	const char *second, deAnimatorRuleMirror::eMatchNameType type) :
MatchName(awindowMain)
{
	mpFirst.SetValue(first, false);
	mpSecond.SetValue(second, false);
	mpType.SetValue(type, false);
}

aeRuleMirror::MatchName::MatchName(const MatchName &copy) :
MatchName(copy.windowMain, copy.mpFirst, copy.mpSecond, copy.mpType){}

aeRuleMirror::MatchName::~MatchName(){
	if(metaContext){
		metaContext->Dispose();
	}
}

bool aeRuleMirror::MatchName::operator==(const MatchName &matchName) const{
	return matchName.mpFirst == mpFirst.GetValue()
		&& matchName.mpSecond == mpSecond.GetValue()
		&& matchName.mpType == mpType.GetValue();
}

igdeEnvironment &aeRuleMirror::MatchName::GetEnvironment() const{
	return windowMain.GetEnvironment();
}

igdeUndoSystem *aeRuleMirror::MatchName::GetUndoSystem() const{
	return windowMain.GetAnimator()->GetUndoSystem();
}


// Class aeRuleMirror
///////////////////////

aeRuleMirror::MetaContext::Ref aeRuleMirror::CreateMetaContext(aeWindowMain &windowMain, aeRuleMirror *rule){
	return MetaContext::Ref::New("animator.rule_mirror", "Rule Mirror", "Rule mirror properties",
		windowMain.GetMCAnimatorProperties().ruleMirror.metaProperties, rule);
}

aeRuleMirror::Ref aeRuleMirror::CreateDefault(aeWindowMain &windowMain, const char *name){
	auto rule = Ref::New(windowMain, name);
	rule->mpMatchNames = decTObjectOrderedSet<MatchName>(devctag,
		MatchName::Ref::New(windowMain, ".l", ".r", deAnimatorRuleMirror::emntLast));
	return rule;
}

// Constructor, destructor
////////////////////////////

aeRuleMirror::aeRuleMirror(aeWindowMain &windowMain, const char *aname) :
aeRuleMirror(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleMirror::aeRuleMirror(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertMirror, aname),
pWindowMain(windowMain),
mpMirrorAxis(windowMain.GetMCAnimatorProperties().ruleMirror.mirrorAxis, metaContext),
mpMirrorBone(windowMain.GetMCAnimatorProperties().ruleMirror.mirrorBone, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleMirror.enablePosition, metaContext),
mpEnableOrientation(windowMain.GetMCAnimatorProperties().ruleMirror.enableOrientation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleMirror.enableSize, metaContext),
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleMirror.enableVertexPositionSet, metaContext),
mpMatchNames(windowMain.GetMCAnimatorProperties().ruleMirror.matchName.matchNames, metaContext),
mpMatchName(windowMain.GetMCAnimatorProperties().ruleMirror.matchName.matchName, metaContext)
{
	mpMirrorAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetMirrorAxis(mpMirrorAxis);
		}
		NotifyRuleChanged();
	};
	
	mpMirrorBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetMirrorBone(mpMirrorBone);
		}
		NotifyRuleChanged();
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnableOrientation(mpEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	mpMatchNames.onValueChanged = [this](){
		pUpdateMatchNames();
		NotifyRuleChanged();
	};
	mpMatchNames.onActiveChanged = [this](){
		mpMatchName = mpMatchNames.GetActive()
			? mpMatchNames.GetActive()->metaContext
			: mpMatchName.Property().GetDefaultValue();
	};
}

aeRuleMirror::aeRuleMirror(const aeRuleMirror &copy) :
aeRuleMirror(copy.pWindowMain, copy.mpName)
{
	pInitCopy(copy);
	mpMirrorBone.SetValue(copy.mpMirrorBone, false);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableOrientation.SetValue(copy.mpEnableOrientation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpEnableVertexPositionSet.SetValue(copy.mpEnableVertexPositionSet, false);
	mpMatchNames.SetValue(copy.mpMatchNames, false);
}

aeRuleMirror::~aeRuleMirror() = default;


// Management
///////////////

deAnimatorRule::Ref aeRuleMirror::CreateEngineRule(){
	const deAnimatorRuleMirror::Ref engRule(deAnimatorRuleMirror::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMirrorAxis(mpMirrorAxis);
	engRule->SetMirrorBone(mpMirrorBone);
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableOrientation(mpEnableOrientation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
	
	mpMatchNames->Visit([&](const MatchName &matchName){
		engRule->AddMatchName(matchName.mpFirst, matchName.mpSecond, matchName.mpType);
	});
	return engRule;
}


aeRule::Ref aeRuleMirror::CreateCopy() const{
	return Ref::New(*this);
}


// Private Functions
//////////////////////

void aeRuleMirror::pUpdateMatchNames(){
	if(!GetEngineRule()){
		return;
	}
	
	deAnimatorRuleMirror &rule = *((deAnimatorRuleMirror*)GetEngineRule());
	rule.RemoveAllMatchNames();
	
	mpMatchNames->Visit([&](const MatchName &matchName){
		rule.AddMatchName(matchName.mpFirst, matchName.mpSecond, matchName.mpType);
	});
	
	NotifyRuleChanged();
}
