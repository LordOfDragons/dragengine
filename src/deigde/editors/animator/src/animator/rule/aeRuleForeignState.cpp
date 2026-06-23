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
foreignBone(windowMain.GetMCAnimatorProperties().ruleForeignState.foreignBone, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
foreignVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.foreignVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
scalePosition(windowMain.GetMCAnimatorProperties().ruleForeignState.scalePosition, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
scaleOrientation(windowMain.GetMCAnimatorProperties().ruleForeignState.scaleOrientation, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
scaleSize(windowMain.GetMCAnimatorProperties().ruleForeignState.scaleSize, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
scaleVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.scaleVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleForeignState.enablePosition, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleForeignState.enableOrientation, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleForeignState.enableSize, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
sourceCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleForeignState.sourceCoordinateFrame, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
destCoordinateFrame(windowMain.GetMCAnimatorProperties().ruleForeignState.destCoordinateFrame, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
targetPosition(windowMain.GetMCAnimatorProperties().ruleForeignState.targetPosition, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
targetOrientation(windowMain.GetMCAnimatorProperties().ruleForeignState.targetOrientation, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
targetSize(windowMain.GetMCAnimatorProperties().ruleForeignState.targetSize, GetMetaContext().StaticCast<aeMCRuleForeignState>()),
targetVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleForeignState.targetVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleForeignState>())
{
	foreignBone.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetForeignBone(foreignBone);
		}
		NotifyRuleChanged();
	});
	
	foreignVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetForeignVertexPositionSet(foreignVertexPositionSet);
		}
		NotifyRuleChanged();
	});
	
	scalePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScalePosition(scalePosition);
		}
		NotifyRuleChanged();
	});
	
	scaleOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScaleOrientation(scaleOrientation);
		}
		NotifyRuleChanged();
	});
	
	scaleSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScaleSize(scaleSize);
		}
		NotifyRuleChanged();
	});
	
	scaleVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetScaleVertexPositionSet(scaleVertexPositionSet);
		}
		NotifyRuleChanged();
	});
	
	enablePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	});
	
	enableOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	});
	
	enableSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	});
	
	enableVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	});
	
	sourceCoordinateFrame.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetSourceCoordinateFrame(sourceCoordinateFrame);
		}
		NotifyRuleChanged();
	});
	
	destCoordinateFrame.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleForeignState*)GetEngineRule())->SetDestCoordinateFrame(destCoordinateFrame);
		}
		NotifyRuleChanged();
	});
	
	pTargetPosition = aeControllerTarget::Ref::New(targetPosition);
	targetPosition.SetOnChanged([this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetPosition(), targetPosition);
		}
		pTargetPosition->OnStorageChanged();
		NotifyRuleChanged();
	});
	
	pTargetOrientation = aeControllerTarget::Ref::New(targetOrientation);
	targetOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetOrientation(), targetOrientation);
		}
		pTargetOrientation->OnStorageChanged();
		NotifyRuleChanged();
	});
	
	pTargetSize = aeControllerTarget::Ref::New(targetSize);
	targetSize.SetOnChanged([this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetSize(), targetSize);
		}
		pTargetSize->OnStorageChanged();
		NotifyRuleChanged();
	});
	
	pTargetVertexPositionSet = aeControllerTarget::Ref::New(targetVertexPositionSet);
	targetVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			pUpdateEngineTarget(((deAnimatorRuleForeignState*)GetEngineRule())->GetTargetVertexPositionSet(), targetVertexPositionSet);
		}
		pTargetVertexPositionSet->OnStorageChanged();
		NotifyRuleChanged();
	});
}

aeRuleForeignState::aeRuleForeignState(aeWindowMain &windowMain, const aeRuleForeignState &copy) :
aeRuleForeignState(windowMain, copy.name)
{
	pInitCopy(copy);
	foreignVertexPositionSet.SetValue(copy.foreignVertexPositionSet, false);
	scalePosition.SetValue(copy.scalePosition, false);
	scaleOrientation.SetValue(copy.scaleOrientation, false);
	scaleSize.SetValue(copy.scaleSize, false);
	scaleVertexPositionSet.SetValue(copy.scaleVertexPositionSet, false);
	enablePosition.SetValue(copy.enablePosition, false);
	enableOrientation.SetValue(copy.enableOrientation, false);
	enableSize.SetValue(copy.enableSize, false);
	enableVertexPositionSet.SetValue(copy.enableVertexPositionSet, false);
	sourceCoordinateFrame.SetValue(copy.sourceCoordinateFrame, false);
	destCoordinateFrame.SetValue(copy.destCoordinateFrame, false);
	
	pTargetPosition = aeControllerTarget::Ref::New(targetPosition, copy.pTargetPosition);
	pTargetOrientation = aeControllerTarget::Ref::New(targetOrientation, copy.pTargetOrientation);
	pTargetSize = aeControllerTarget::Ref::New(targetSize, copy.pTargetSize);
	pTargetVertexPositionSet = aeControllerTarget::Ref::New(targetVertexPositionSet, copy.pTargetVertexPositionSet);
}

aeRuleForeignState::~aeRuleForeignState() = default;


// Management
///////////////

void aeRuleForeignState::SetForeignBone(const char *value){
	foreignBone = value;
}

void aeRuleForeignState::SetForeignVertexPositionSet(const char *value){
	foreignVertexPositionSet = value;
}

void aeRuleForeignState::SetScalePosition(float value){
	scalePosition = value;
}

void aeRuleForeignState::SetScaleOrientation(float value){
	scaleOrientation = value;
}

void aeRuleForeignState::SetScaleSize(float value){
	scaleSize = value;
}

void aeRuleForeignState::SetScaleVertexPositionSet(float value){
	scaleVertexPositionSet = value;
}

void aeRuleForeignState::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleForeignState::SetEnableOrientation(bool value){
	enableOrientation = value;
}

void aeRuleForeignState::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleForeignState::SetEnableVertexPositionSet(bool value){
	enableVertexPositionSet = value;
}

void aeRuleForeignState::SetSourceCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames value){
	sourceCoordinateFrame = value;
}

void aeRuleForeignState::SetDestCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames value){
	destCoordinateFrame = value;
}


void aeRuleForeignState::UpdateTargets(){
	deAnimatorRuleForeignState *rule = (deAnimatorRuleForeignState*)GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if(rule){
		pUpdateEngineTarget(rule->GetTargetPosition(), targetPosition);
		pUpdateEngineTarget(rule->GetTargetOrientation(), targetOrientation);
		pUpdateEngineTarget(rule->GetTargetSize(), targetSize);
		pUpdateEngineTarget(rule->GetTargetVertexPositionSet(), targetVertexPositionSet);
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
	
	engRule->SetForeignBone(foreignBone);
	engRule->SetForeignVertexPositionSet(foreignBone);
	engRule->SetScalePosition(scalePosition);
	engRule->SetScaleOrientation(scaleOrientation);
	engRule->SetScaleSize(scaleSize);
	engRule->SetScaleVertexPositionSet(scaleVertexPositionSet);
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	engRule->SetSourceCoordinateFrame(sourceCoordinateFrame);
	engRule->SetDestCoordinateFrame(destCoordinateFrame);
	
	pUpdateEngineTarget(engRule->GetTargetPosition(), targetPosition);
	pUpdateEngineTarget(engRule->GetTargetOrientation(), targetOrientation);
	pUpdateEngineTarget(engRule->GetTargetSize(), targetSize);
	pUpdateEngineTarget(engRule->GetTargetVertexPositionSet(), targetVertexPositionSet);
	
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
