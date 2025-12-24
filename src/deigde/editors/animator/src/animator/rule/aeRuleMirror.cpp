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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeRuleMirror.h"
#include "../aeAnimator.h"

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

aeRuleMirror::Ref aeRuleMirror::CreateDefault(){
	const Ref rule(aeRuleMirror::Ref::New());
	rule->pMatchNames.Add(aeRuleMirror::MatchName::Ref::New(".l", ".r", deAnimatorRuleMirror::emntLast));
	return rule;
}



// Class aeRuleMirror
///////////////////////

// Constructor, destructor
////////////////////////////

aeRuleMirror::aeRuleMirror() :
aeRule(deAnimatorRuleVisitorIdentify::ertMirror),
pMirrorAxis(deAnimatorRuleMirror::emaX),
pEnablePosition(true),
pEnableOrientation(true),
pEnableSize(false),
pEnableVertexPositionSet(true)
{
	SetName("Mirror");
}

aeRuleMirror::aeRuleMirror(const aeRuleMirror &copy) :
aeRule(copy),
pMirrorAxis(copy.pMirrorAxis),
pMirrorBone(copy.pMirrorBone),
pMatchNames(copy.pMatchNames),
pEnablePosition(copy.pEnablePosition),
pEnableOrientation(copy.pEnableOrientation),
pEnableSize(copy.pEnableSize),
pEnableVertexPositionSet(copy.pEnableVertexPositionSet){
}

aeRuleMirror::~aeRuleMirror(){
}



// Management
///////////////

void aeRuleMirror::SetMirrorAxis(deAnimatorRuleMirror::eMirrorAxis axis){
	pMirrorAxis = axis;
	
	if(GetEngineRule()){
		((deAnimatorRuleMirror*)GetEngineRule())->SetMirrorAxis(axis);
		NotifyRuleChanged();
	}
}

void aeRuleMirror::SetMirrorBone(const char *boneName){
	pMirrorBone = boneName;
	
	if(GetEngineRule()){
		((deAnimatorRuleMirror*)GetEngineRule())->SetMirrorBone(boneName);
		NotifyRuleChanged();
	}
}

void aeRuleMirror::SetEnablePosition(bool enable){
	pEnablePosition = enable;
	
	if(GetEngineRule()){
		((deAnimatorRuleMirror*)GetEngineRule())->SetEnablePosition(enable);
		NotifyRuleChanged();
	}
}

void aeRuleMirror::SetEnableOrientation(bool enable){
	pEnableOrientation = enable;
	
	if(GetEngineRule()){
		((deAnimatorRuleMirror*)GetEngineRule())->SetEnableOrientation(enable);
		NotifyRuleChanged();
	}
}

void aeRuleMirror::SetEnableSize(bool enable){
	pEnableSize = enable;
	
	if(GetEngineRule()){
		((deAnimatorRuleMirror*)GetEngineRule())->SetEnableSize(enable);
		NotifyRuleChanged();
	}
}

void aeRuleMirror::SetEnableVertexPositionSet(bool enable){
	pEnableVertexPositionSet = enable;
	
	if(GetEngineRule()){
		((deAnimatorRuleMirror*)GetEngineRule())->SetEnableVertexPositionSet(enable);
		NotifyRuleChanged();
	}
}

void aeRuleMirror::AddMatchName(aeRuleMirror::MatchName *matchName){
	DEASSERT_NOTNULL(matchName)
	DEASSERT_FALSE(pMatchNames.Has(matchName))
	
	pMatchNames.Add(matchName);
	pUpdateMatchNames();
}

void aeRuleMirror::InsertMatchName(MatchName *matchName, int index){
	DEASSERT_NOTNULL(matchName)
	DEASSERT_FALSE(pMatchNames.Has(matchName))
	
	pMatchNames.Insert(matchName, index);
	pUpdateMatchNames();
}

void aeRuleMirror::SetMatchNameAt(int index, MatchName *matchName){
	DEASSERT_NOTNULL(matchName)
	DEASSERT_FALSE(pMatchNames.Has(matchName))
	
	pMatchNames.SetAt(index, matchName);
	pUpdateMatchNames();
}

void aeRuleMirror::RemoveMatchName(MatchName *matchName){
	pMatchNames.Remove(matchName);
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
	
	engRule->SetMirrorAxis(pMirrorAxis);
	engRule->SetMirrorBone(pMirrorBone);
	engRule->SetEnablePosition(pEnablePosition);
	engRule->SetEnableOrientation(pEnableOrientation);
	engRule->SetEnableSize(pEnableSize);
	engRule->SetEnableVertexPositionSet(pEnableVertexPositionSet);
	
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
	SetMirrorAxis(copy.pMirrorAxis);
	SetMirrorBone(copy.pMirrorBone);
	pMatchNames = copy.pMatchNames;
	SetEnablePosition(copy.pEnablePosition);
	SetEnableOrientation(copy.pEnableOrientation);
	SetEnableSize(copy.pEnableSize);
	SetEnableVertexPositionSet(copy.pEnableVertexPositionSet);
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
