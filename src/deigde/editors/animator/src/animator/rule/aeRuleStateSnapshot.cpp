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
useLastState(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.useLastState, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
id(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.id, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enablePosition, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enableOrientation, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enableSize, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleStateSnapshot>())
{
	useLastState.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetUseLastState(useLastState);
		}
		NotifyRuleChanged();
	});
	
	id.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetID(id);
		}
		NotifyRuleChanged();
	});
	
	enablePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	});
	
	enableOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	});
	
	enableSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	});
	
	enableVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleStateSnapshot*)GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	});
}

aeRuleStateSnapshot::aeRuleStateSnapshot(aeWindowMain &windowMain, const aeRuleStateSnapshot &copy) :
aeRuleStateSnapshot(windowMain, copy.name)
{
	pInitCopy(copy);
	id.SetValue(copy.id, false);
	enablePosition.SetValue(copy.enablePosition, false);
	enableOrientation.SetValue(copy.enableOrientation, false);
	enableSize.SetValue(copy.enableSize, false);
	enableVertexPositionSet.SetValue(copy.enableVertexPositionSet, false);
}

aeRuleStateSnapshot::~aeRuleStateSnapshot() = default;


// Management
///////////////

void aeRuleStateSnapshot::SetUseLastState(bool value){
	useLastState = value;
}

void aeRuleStateSnapshot::SetID(int value){
	id = value;
}

void aeRuleStateSnapshot::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleStateSnapshot::SetEnableOrientation(bool value){
	enableOrientation = value;
}

void aeRuleStateSnapshot::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleStateSnapshot::SetEnableVertexPositionSet(bool value){
	enableVertexPositionSet = value;
}


deAnimatorRule::Ref aeRuleStateSnapshot::CreateEngineRule(){
	const deAnimatorRuleStateSnapshot::Ref engRule(deAnimatorRuleStateSnapshot::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetUseLastState(useLastState);
	engRule->SetID(id);
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	
	return engRule;
}


aeRule::Ref aeRuleStateSnapshot::CreateCopy() const{
	return Ref::New(*this);
}

void aeRuleStateSnapshot::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
}


// Operators
///////////////

aeRuleStateSnapshot &aeRuleStateSnapshot::operator=(const aeRuleStateSnapshot &copy){
	useLastState = copy.useLastState;
	id = copy.id;
	enablePosition = copy.enablePosition;
	enableOrientation = copy.enableOrientation;
	enableSize = copy.enableSize;
	enableVertexPositionSet = copy.enableVertexPositionSet;
	aeRule::operator=(copy);
	return *this;
}
