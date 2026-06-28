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

// Constructor, destructor
////////////////////////////

aeRuleStateSnapshot::aeRuleStateSnapshot(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleStateSnapshot::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertStateSnapshot, aname),
pMPUseLastState(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.useLastState, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
pMPId(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.id, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
pMPEnablePosition(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enablePosition, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
pMPEnableOrientation(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enableOrientation, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
pMPEnableSize(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enableSize, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
pMPEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>())
{
	pMPUseLastState.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetUseLastState(pMPUseLastState);
		}
		NotifyRuleChanged();
	};
	
	pMPId.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetID(pMPId);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnablePosition(pMPEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableOrientation(pMPEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableSize(pMPEnableSize);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
}

aeRuleStateSnapshot::aeRuleStateSnapshot(aeWindowMain &windowMain, const aeRuleStateSnapshot &copy) :
aeRuleStateSnapshot(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPId.SetValue(copy.pMPId, false);
	pMPEnablePosition.SetValue(copy.pMPEnablePosition, false);
	pMPEnableOrientation.SetValue(copy.pMPEnableOrientation, false);
	pMPEnableSize.SetValue(copy.pMPEnableSize, false);
	pMPEnableVertexPositionSet.SetValue(copy.pMPEnableVertexPositionSet, false);
}

aeRuleStateSnapshot::~aeRuleStateSnapshot() = default;


// Management
///////////////

void aeRuleStateSnapshot::SetUseLastState(bool value){
	pMPUseLastState = value;
}

void aeRuleStateSnapshot::SetID(int value){
	pMPId = value;
}

void aeRuleStateSnapshot::SetEnablePosition(bool value){
	pMPEnablePosition = value;
}

void aeRuleStateSnapshot::SetEnableOrientation(bool value){
	pMPEnableOrientation = value;
}

void aeRuleStateSnapshot::SetEnableSize(bool value){
	pMPEnableSize = value;
}

void aeRuleStateSnapshot::SetEnableVertexPositionSet(bool value){
	pMPEnableVertexPositionSet = value;
}


deAnimatorRule::Ref aeRuleStateSnapshot::CreateEngineRule(){
	const deAnimatorRuleStateSnapshot::Ref engRule(deAnimatorRuleStateSnapshot::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetUseLastState(pMPUseLastState);
	engRule->SetID(pMPId);
	engRule->SetEnablePosition(pMPEnablePosition);
	engRule->SetEnableOrientation(pMPEnableOrientation);
	engRule->SetEnableSize(pMPEnableSize);
	engRule->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
	
	return engRule;
}


aeRule::Ref aeRuleStateSnapshot::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleStateSnapshot::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
}
