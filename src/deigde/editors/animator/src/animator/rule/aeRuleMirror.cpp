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



// Class aeRuleMirror::cMatchName
///////////////////////////////////

aeRuleMirror::cMatchName::cMatchName(const char *first, const char *second,
deAnimatorRuleMirror::eMatchNameType type) :
pFirst(first),
pSecond(second),
pType(type)
{
	if(pFirst.IsEmpty() || pSecond.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
}

bool aeRuleMirror::cMatchName::operator==(const cMatchName &matchName) const{
	return matchName.pFirst == pFirst && matchName.pSecond == pSecond && matchName.pType == pType;
}

bool aeRuleMirror::cMatchName::operator!=(const cMatchName &matchName) const{
	return matchName.pFirst != pFirst || matchName.pSecond != pSecond || matchName.pType != pType;
}

aeRuleMirror *aeRuleMirror::CreateDefault(){
	Ref rule(Ref::New(new aeRuleMirror));
	rule->pMatchNames.Add(aeRuleMirror::cMatchName::Ref::NewWith(
		".l", ".r", deAnimatorRuleMirror::emntLast));
	rule->AddReference(); // caller inherits reference
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

int aeRuleMirror::GetMatchNameCount() const{
	return pMatchNames.GetCount();
}

aeRuleMirror::cMatchName *aeRuleMirror::GetMatchNameAt(int index) const{
	return (cMatchName*)pMatchNames.GetAt(index);
}

bool aeRuleMirror::HasMatchName(cMatchName *matchName) const{
	return pMatchNames.Has(matchName);
}

int aeRuleMirror::IndexOfMatchName(cMatchName *matchName) const{
	return pMatchNames.IndexOf(matchName);
}

void aeRuleMirror::AddMatchName(aeRuleMirror::cMatchName *matchName){
	if(!matchName || pMatchNames.Has(matchName)){
		DETHROW(deeInvalidParam);
	}
	
	pMatchNames.Add(matchName);
	pUpdateMatchNames();
}

void aeRuleMirror::InsertMatchName(cMatchName *matchName, int index){
	if(!matchName || pMatchNames.Has(matchName)){
		DETHROW(deeInvalidParam);
	}
	
	pMatchNames.Insert(matchName, index);
	pUpdateMatchNames();
}

void aeRuleMirror::SetMatchNameAt(int index, cMatchName *matchName){
	if(!matchName || pMatchNames.Has(matchName)){
		DETHROW(deeInvalidParam);
	}
	
	pMatchNames.SetAt(index, matchName);
	pUpdateMatchNames();
}

void aeRuleMirror::RemoveMatchName(cMatchName *matchName){
	pMatchNames.Remove(matchName);
	pUpdateMatchNames();
}

void aeRuleMirror::RemoveAllMatchNames(){
	pMatchNames.RemoveAll();
	pUpdateMatchNames();
}



deAnimatorRule *aeRuleMirror::CreateEngineRule(){
	const deAnimatorRuleMirror::Ref engRule(deAnimatorRuleMirror::Ref::NewWith());
	
	InitEngineRule(engRule);
	
	engRule->SetMirrorAxis(pMirrorAxis);
	engRule->SetMirrorBone(pMirrorBone);
	engRule->SetEnablePosition(pEnablePosition);
	engRule->SetEnableOrientation(pEnableOrientation);
	engRule->SetEnableSize(pEnableSize);
	engRule->SetEnableVertexPositionSet(pEnableVertexPositionSet);
	
	const int count = pMatchNames.GetCount();
	int i;
	for(i=0; i<count; i++){
		const cMatchName &matchName = *((cMatchName*)pMatchNames.GetAt(i));
		engRule->AddMatchName(matchName.GetFirst(), matchName.GetSecond(), matchName.GetType());
	}
	
	engRule->AddReference(); // caller inherits reference
	return engRule;
}



aeRule *aeRuleMirror::CreateCopy() const{
	return new aeRuleMirror(*this);
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
	
	const int count = pMatchNames.GetCount();
	int i;
	for(i=0; i<count; i++){
		const cMatchName &matchName = *((cMatchName*)pMatchNames.GetAt(i));
		rule.AddMatchName(matchName.GetFirst(), matchName.GetSecond(), matchName.GetType());
	}
	
	NotifyRuleChanged();
}
