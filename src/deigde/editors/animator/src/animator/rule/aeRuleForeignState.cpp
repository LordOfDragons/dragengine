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

// Constructor, destructor
////////////////////////////

aeRuleForeignState::aeRuleForeignState(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleForeignState::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertForeignState, aname),
pMPForeignBone(windowMain.GetMCAnimatorProperties().ruleForeignState.foreignBone, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPForeignVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.foreignVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPScalePosition(windowMain.GetMCAnimatorProperties().ruleForeignState.scalePosition, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPScaleOrientation(windowMain.GetMCAnimatorProperties().ruleForeignState.scaleOrientation, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPScaleSize(windowMain.GetMCAnimatorProperties().ruleForeignState.scaleSize, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPScaleVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.scaleVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPEnablePosition(windowMain.GetMCAnimatorProperties().ruleForeignState.enablePosition, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPEnableOrientation(windowMain.GetMCAnimatorProperties().ruleForeignState.enableOrientation, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPEnableSize(windowMain.GetMCAnimatorProperties().ruleForeignState.enableSize, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPSourceCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleForeignState.sourceCoordinateFrame, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPDestCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleForeignState.destCoordinateFrame, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPTargetPosition(windowMain.GetMCAnimatorProperties().ruleForeignState.targetPosition, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPTargetOrientation(windowMain.GetMCAnimatorProperties().ruleForeignState.targetOrientation, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPTargetSize(windowMain.GetMCAnimatorProperties().ruleForeignState.targetSize, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
pMPTargetVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.targetVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleForeignState>())
{
	pMPForeignBone.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetForeignBone(pMPForeignBone);
		}
		NotifyRuleChanged();
	};
	
	pMPForeignVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetForeignVertexPositionSet(pMPForeignVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pMPScalePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScalePosition(pMPScalePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPScaleOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScaleOrientation(pMPScaleOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPScaleSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScaleSize(pMPScaleSize);
		}
		NotifyRuleChanged();
	};
	
	pMPScaleVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScaleVertexPositionSet(pMPScaleVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pMPEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnablePosition(pMPEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnableOrientation(pMPEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnableSize(pMPEnableSize);
		}
		NotifyRuleChanged();
	};
	
	pMPEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
	
	pMPSourceCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetSourceCoordinateFrame(pMPSourceCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	pMPDestCoordinateFrame.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetDestCoordinateFrame(pMPDestCoordinateFrame);
		}
		NotifyRuleChanged();
	};
	
	pTargetPosition = aeControllerTarget::Ref::New(pMPTargetPosition);
	pMPTargetPosition.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetPosition(), pMPTargetPosition);
		}
		pTargetPosition->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetOrientation = aeControllerTarget::Ref::New(pMPTargetOrientation);
	pMPTargetOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetOrientation(), pMPTargetOrientation);
		}
		pTargetOrientation->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetSize = aeControllerTarget::Ref::New(pMPTargetSize);
	pMPTargetSize.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetSize(), pMPTargetSize);
		}
		pTargetSize->OnStorageChanged();
		NotifyRuleChanged();
	};
	
	pTargetVertexPositionSet = aeControllerTarget::Ref::New(pMPTargetVertexPositionSet);
	pMPTargetVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetVertexPositionSet(), pMPTargetVertexPositionSet);
		}
		pTargetVertexPositionSet->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRuleForeignState::aeRuleForeignState(aeWindowMain &windowMain, const aeRuleForeignState &copy) :
aeRuleForeignState(windowMain, copy.GetName())
{
	pInitCopy(copy);
	pMPForeignVertexPositionSet.SetValue(copy.pMPForeignVertexPositionSet, false);
	pMPScalePosition.SetValue(copy.pMPScalePosition, false);
	pMPScaleOrientation.SetValue(copy.pMPScaleOrientation, false);
	pMPScaleSize.SetValue(copy.pMPScaleSize, false);
	pMPScaleVertexPositionSet.SetValue(copy.pMPScaleVertexPositionSet, false);
	pMPEnablePosition.SetValue(copy.pMPEnablePosition, false);
	pMPEnableOrientation.SetValue(copy.pMPEnableOrientation, false);
	pMPEnableSize.SetValue(copy.pMPEnableSize, false);
	pMPEnableVertexPositionSet.SetValue(copy.pMPEnableVertexPositionSet, false);
	pMPSourceCoordinateFrame.SetValue(copy.pMPSourceCoordinateFrame, false);
	pMPDestCoordinateFrame.SetValue(copy.pMPDestCoordinateFrame, false);
	
	pTargetPosition = aeControllerTarget::Ref::New(pMPTargetPosition, copy.pTargetPosition);
	pTargetOrientation = aeControllerTarget::Ref::New(pMPTargetOrientation, copy.pTargetOrientation);
	pTargetSize = aeControllerTarget::Ref::New(pMPTargetSize, copy.pTargetSize);
	pTargetVertexPositionSet = aeControllerTarget::Ref::New(pMPTargetVertexPositionSet, copy.pTargetVertexPositionSet);
}

aeRuleForeignState::~aeRuleForeignState() = default;


// Management
///////////////

void aeRuleForeignState::SetForeignBone(const char *value){
	pMPForeignBone = value;
}

void aeRuleForeignState::SetForeignVertexPositionSet(const char *value){
	pMPForeignVertexPositionSet = value;
}

void aeRuleForeignState::SetScalePosition(float value){
	pMPScalePosition = value;
}

void aeRuleForeignState::SetScaleOrientation(float value){
	pMPScaleOrientation = value;
}

void aeRuleForeignState::SetScaleSize(float value){
	pMPScaleSize = value;
}

void aeRuleForeignState::SetScaleVertexPositionSet(float value){
	pMPScaleVertexPositionSet = value;
}

void aeRuleForeignState::SetEnablePosition(bool value){
	pMPEnablePosition = value;
}

void aeRuleForeignState::SetEnableOrientation(bool value){
	pMPEnableOrientation = value;
}

void aeRuleForeignState::SetEnableSize(bool value){
	pMPEnableSize = value;
}

void aeRuleForeignState::SetEnableVertexPositionSet(bool value){
	pMPEnableVertexPositionSet = value;
}

void aeRuleForeignState::SetSourceCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames value){
	pMPSourceCoordinateFrame = value;
}

void aeRuleForeignState::SetDestCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames value){
	pMPDestCoordinateFrame = value;
}


void aeRuleForeignState::UpdateTargets(){
	deAnimatorRuleForeignState *rule = (deAnimatorRuleForeignState*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetPosition(), pMPTargetPosition);
		pUpdateEngineTarget(rule->GetTargetOrientation(), pMPTargetOrientation);
		pUpdateEngineTarget(rule->GetTargetSize(), pMPTargetSize);
		pUpdateEngineTarget(rule->GetTargetVertexPositionSet(), pMPTargetVertexPositionSet);
	}
}

int aeRuleForeignState::CountLinkUsage(aeLink *link) const{
	int usageCount = aeRule::CountLinkUsage(link);
	
	if(pTargetPosition->GetLinks().Has(link)) usageCount++;
	if(pTargetOrientation->GetLinks().Has(link)) usageCount++;
	if(pTargetSize->GetLinks().Has(link)) usageCount++;
	if(pTargetVertexPositionSet->GetLinks().Has(link)) usageCount++;
	
	return usageCount;
}

void aeRuleForeignState::RemoveLinkFromTargets(aeLink *link){
	aeRule::RemoveLinkFromTargets(link);
	
	if(pTargetPosition->GetLinks().Has(link)){
		pTargetPosition->RemoveLink(link);
	}
	if(pTargetOrientation->GetLinks().Has(link)){
		pTargetOrientation->RemoveLink(link);
	}
	if(pTargetSize->GetLinks().Has(link)){
		pTargetSize->RemoveLink(link);
	}
	if(pTargetVertexPositionSet->GetLinks().Has(link)){
		pTargetVertexPositionSet->RemoveLink(link);
	}
	
	UpdateTargets();
}

void aeRuleForeignState::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetPosition->RemoveAllLinks();
	pTargetOrientation->RemoveAllLinks();
	pTargetSize->RemoveAllLinks();
	pTargetVertexPositionSet->RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule::Ref aeRuleForeignState::CreateEngineRule(){
	const deAnimatorRuleForeignState::Ref engRule(deAnimatorRuleForeignState::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetForeignBone(pMPForeignBone);
	engRule->SetForeignVertexPositionSet(pMPForeignVertexPositionSet);
	engRule->SetScalePosition(pMPScalePosition);
	engRule->SetScaleOrientation(pMPScaleOrientation);
	engRule->SetScaleSize(pMPScaleSize);
	engRule->SetScaleVertexPositionSet(pMPScaleVertexPositionSet);
	engRule->SetEnablePosition(pMPEnablePosition);
	engRule->SetEnableOrientation(pMPEnableOrientation);
	engRule->SetEnableSize(pMPEnableSize);
	engRule->SetEnableVertexPositionSet(pMPEnableVertexPositionSet);
	engRule->SetSourceCoordinateFrame(pMPSourceCoordinateFrame);
	engRule->SetDestCoordinateFrame(pMPDestCoordinateFrame);
	
	pUpdateEngineTarget(engRule->GetTargetPosition(), pMPTargetPosition);
	pUpdateEngineTarget(engRule->GetTargetOrientation(), pMPTargetOrientation);
	pUpdateEngineTarget(engRule->GetTargetSize(), pMPTargetSize);
	pUpdateEngineTarget(engRule->GetTargetVertexPositionSet(), pMPTargetVertexPositionSet);
	
	return engRule;
}



aeRule::Ref aeRuleForeignState::CreateCopy(aeWindowMain &windowMain) const{
	return Ref::New(windowMain, *this);
}

void aeRuleForeignState::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
	pTargetOrientation->AddLinksToList(list);
	pTargetPosition->AddLinksToList(list);
	pTargetSize->AddLinksToList(list);
	pTargetVertexPositionSet->AddLinksToList(list);
}
