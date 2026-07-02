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

aeRuleMirror::MatchName::MatchName(const char *afirst, const char *asecond,
deAnimatorRuleMirror::eMatchNameType atype) :
first(afirst),
second(asecond),
type(atype)
{
	DEASSERT_FALSE(first.IsEmpty())
	DEASSERT_FALSE(second.IsEmpty())
}

bool aeRuleMirror::MatchName::operator==(const MatchName &matchName) const{
	return matchName.first == first && matchName.second == second && matchName.type == type;
}

bool aeRuleMirror::MatchName::operator!=(const MatchName &matchName) const{
	return !operator==(matchName);
}

aeRuleMirror::Ref aeRuleMirror::CreateDefault(aeWindowMain &windowMain, const char *name){
	const Ref rule(aeRuleMirror::Ref::New(windowMain, name));
	rule->pMatchNames.Add(aeRuleMirror::MatchName::Ref::New(".l", ".r", deAnimatorRuleMirror::emntLast));
	return rule;
}



// Class aeRuleMirror
///////////////////////

aeRuleMirror::MetaContext::Ref aeRuleMirror::CreateMetaContext(aeWindowMain &windowMain, aeRuleMirror *rule){
	return MetaContext::Ref::New("animator.rule_mirror", "Rule Mirror", "Rule mirror properties",
		windowMain.GetMCAnimatorProperties().ruleMirror.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleMirror::aeRuleMirror(aeWindowMain &windowMain, const char *aname) :
aeRuleMirror(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleMirror::aeRuleMirror(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertMirror, aname),
pMPMirrorAxis(windowMain.GetMCAnimatorProperties().ruleMirror.mirrorAxis, metaContext),
pMPMirrorBone(windowMain.GetMCAnimatorProperties().ruleMirror.mirrorBone, metaContext),
pMPEnablePosition(windowMain.GetMCAnimatorProperties().ruleMirror.enablePosition, metaContext),
pMPEnableOrientation(windowMain.GetMCAnimatorProperties().ruleMirror.enableOrientation, metaContext),
pMPEnableSize(windowMain.GetMCAnimatorProperties().ruleMirror.enableSize, metaContext),
pMPEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleMirror.enableVertexPositionSet, metaContext)
{
	pMPMirrorAxis.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetMirrorAxis(pMPMirrorAxis);
		}
		NotifyRuleChanged();
	};
	
	pMPMirrorBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetMirrorBone(pMPMirrorBone);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnablePosition(pMPEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnableOrientation(pMPEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnableSize(pMPEnableSize);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
}

aeRuleMirror::aeRuleMirror(aeWindowMain &windowMain, const aeRuleMirror &copy) :
aeRuleMirror(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPMirrorBone.SetValue(copy.pMPMirrorBone, false);
	pMPEnablePosition.SetValue(copy.pMPEnablePosition, false);
	pMPEnableOrientation.SetValue(copy.pMPEnableOrientation, false);
	pMPEnableSize.SetValue(copy.pMPEnableSize, false);
	pMPEnableVertexPositionSet.SetValue(copy.pMPEnableVertexPositionSet, false);
}

aeRuleMirror::~aeRuleMirror() = default;


// Management
///////////////

void aeRuleMirror::SetMirrorAxis(deAnimatorRuleMirror::eMirrorAxis value){
	pMPMirrorAxis = value;
}

void aeRuleMirror::SetMirrorBone(const char *value){
	pMPMirrorBone = value;
}

void aeRuleMirror::SetEnablePosition(bool value){
	pMPEnablePosition = value;
}

void aeRuleMirror::SetEnableOrientation(bool value){
	pMPEnableOrientation = value;
}

void aeRuleMirror::SetEnableSize(bool value){
	pMPEnableSize = value;
}

void aeRuleMirror::SetEnableVertexPositionSet(bool value){
	pMPEnableVertexPositionSet = value;
}


void aeRuleMirror::AddMatchName(aeRuleMirror::MatchName *matchName){
	DEASSERT_NOTNULL(matchName)
	
	pMatchNames.AddOrThrow(matchName);
	pUpdateMatchNames();
}

void aeRuleMirror::InsertMatchName(MatchName *matchName, int index){
	DEASSERT_NOTNULL(matchName)
	
	pMatchNames.InsertOrThrow(matchName, index);
	pUpdateMatchNames();
}

void aeRuleMirror::SetMatchNameAt(int index, MatchName *matchName){
	DEASSERT_NOTNULL(matchName)
	
	pMatchNames.SetAtOrThrow(index, matchName);
	pUpdateMatchNames();
}

void aeRuleMirror::RemoveMatchName(MatchName *matchName){
	pMatchNames.RemoveOrThrow(matchName);
	pUpdateMatchNames();
}

void aeRuleMirror::RemoveAllMatchNames(){
	if(pMatchNames.IsEmpty()){
		return;
	}
	
	pMatchNames.RemoveAll();
	pUpdateMatchNames();
}



deAnimatorRule::Ref aeRuleMirror::CreateEngineRule(){
	const deAnimatorRuleMirror::Ref engRule(deAnimatorRuleMirror::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetMirrorAxis(pMPMirrorAxis);
	engRule->SetMirrorBone(pMPMirrorBone);
	engRule->SetEnablePosition(pMPEnablePosition);
	engRule->SetEnableOrientation(pMPEnableOrientation);
	engRule->SetEnableSize(pMPEnableSize);
	engRule->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
	
	pMatchNames.Visit([&](const MatchName &matchName){
		engRule->AddMatchName(matchName.first, matchName.second, matchName.type);
	});
	return engRule;
}



aeRule::Ref aeRuleMirror::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}



// Private Functions
//////////////////////

void aeRuleMirror::pUpdateMatchNames(){
	if(!GetEngineRule()){
		return;
	}
	
	deAnimatorRuleMirror &rule = *((deAnimatorRuleMirror*)GetEngineRule());
	rule.RemoveAllMatchNames();
	
	pMatchNames.Visit([&](const MatchName &matchName){
		rule.AddMatchName(matchName.first, matchName.second, matchName.type);
	});
	
	NotifyRuleChanged();
}
