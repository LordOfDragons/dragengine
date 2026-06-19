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

// Constructor, destructor
////////////////////////////

aeRuleMirror::aeRuleMirror(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleMirror::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertMirror, aname),
mirrorAxis(windowMain.GetMCAnimatorProperties().ruleMirror.mirrorAxis, GetMetaContext().StaticCast<aeMCRuleMirror>()),
mirrorBone(windowMain.GetMCAnimatorProperties().ruleMirror.mirrorBone, GetMetaContext().StaticCast<aeMCRuleMirror>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleMirror.enablePosition, GetMetaContext().StaticCast<aeMCRuleMirror>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleMirror.enableOrientation, GetMetaContext().StaticCast<aeMCRuleMirror>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleMirror.enableSize, GetMetaContext().StaticCast<aeMCRuleMirror>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleMirror.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleMirror>())
{
	mirrorAxis.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetMirrorAxis(mirrorAxis);
		}
		NotifyRuleChanged();
	});
	
	mirrorBone.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetMirrorBone(mirrorBone);
		}
		NotifyRuleChanged();
	});
	
	enablePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	});
	
	enableOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	});
	
	enableSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	});
	
	enableVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleMirror*)GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	});
}

aeRuleMirror::aeRuleMirror(aeWindowMain &windowMain, const aeRuleMirror &copy) :
aeRuleMirror(windowMain, copy.name)
{
	pInitCopy(copy);
	mirrorBone.SetValue(copy.mirrorBone, false);
	enablePosition.SetValue(copy.enablePosition, false);
	enableOrientation.SetValue(copy.enableOrientation, false);
	enableSize.SetValue(copy.enableSize, false);
	enableVertexPositionSet.SetValue(copy.enableVertexPositionSet, false);
}

aeRuleMirror::~aeRuleMirror() = default;


// Management
///////////////

void aeRuleMirror::SetMirrorAxis(deAnimatorRuleMirror::eMirrorAxis value){
	mirrorAxis = value;
}

void aeRuleMirror::SetMirrorBone(const char *value){
	mirrorBone = value;
}

void aeRuleMirror::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleMirror::SetEnableOrientation(bool value){
	enableOrientation = value;
}

void aeRuleMirror::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleMirror::SetEnableVertexPositionSet(bool value){
	enableVertexPositionSet = value;
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
	
	engRule->SetMirrorAxis(mirrorAxis);
	engRule->SetMirrorBone(mirrorBone);
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	
	pMatchNames.Visit([&](const MatchName &matchName){
		engRule->AddMatchName(matchName.first, matchName.second, matchName.type);
	});
	return engRule;
}



aeRule::Ref aeRuleMirror::CreateCopy() const{
	return Ref::New(*this);
}



// Operators
//////////////

aeRuleMirror &aeRuleMirror::operator=(const aeRuleMirror &copy){
	mirrorAxis = copy.mirrorAxis;
	mirrorBone = copy.mirrorBone;
	pMatchNames = copy.pMatchNames;
	enablePosition = copy.enablePosition;
	enableOrientation = copy.enableOrientation;
	enableSize = copy.enableSize;
	enableVertexPositionSet = copy.enableVertexPositionSet;
	aeRule::operator=(copy);
	return *this;
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
