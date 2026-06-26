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

#include <math.h>

#include "aeRule.h"
#include "aeRuleAnimation.h"
#include "aeRuleAnimationDifference.h"
#include "aeRuleAnimationSelect.h"
#include "aeRuleBoneTransformator.h"
#include "aeRuleForeignState.h"
#include "aeRuleGroup.h"
#include "aeRuleInverseKinematic.h"
#include "aeRuleLimit.h"
#include "aeRuleStateManipulator.h"
#include "aeRuleStateSnapshot.h"
#include "aeRuleSubAnimator.h"
#include "aeRuleTrackTo.h"
#include "aeRuleMirror.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <deigde/localization/igdeTranslationManager.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeRule
/////////////////

// Constructor, destructor
////////////////////////////

aeRule::aeRule(aeWindowMain &windowMain, aeMCRule::Ref &&metaContext,
	deAnimatorRuleVisitorIdentify::eRuleTypes type, const char *aname) :
pAnimator(nullptr),
pParentGroup(nullptr),
pEngRule(nullptr),
pIndex(-1),
pMetaContext(std::move(metaContext)),
pType(type),
name(windowMain.GetMCAnimatorProperties().rule.name, pMetaContext, aname),
blendMode(windowMain.GetMCAnimatorProperties().rule.blendMode, pMetaContext),
blendFactor(windowMain.GetMCAnimatorProperties().rule.blendFactor, pMetaContext),
invertBlendFactor(windowMain.GetMCAnimatorProperties().rule.invertBlendFactor, pMetaContext),
enabled(windowMain.GetMCAnimatorProperties().rule.enabled, pMetaContext),
affectedBones(windowMain.GetMCAnimatorProperties().rule.affectedBones, pMetaContext),
affectedVertexPositionSets(windowMain.GetMCAnimatorProperties().rule.affectedVertexPositionSets, pMetaContext),
targetBlendFactor(windowMain.GetMCAnimatorProperties().rule.targetBlendFactor, pMetaContext)
{
	name.onValueChanged = [this](){
		if(pAnimator){
			pAnimator->NotifyRuleNameChanged(this);
		}
	};
	
	blendMode.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetBlendMode(blendMode);
		}
		NotifyRuleChanged();
	};
	
	blendFactor.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetBlendFactor(blendFactor);
		}
		NotifyRuleChanged();
	};
	
	invertBlendFactor.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetInvertBlendFactor(invertBlendFactor);
		}
		NotifyRuleChanged();
	};
	
	affectedBones.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->GetListBones() = affectedBones;
		}
		NotifyRuleChanged();
	};
	
	affectedVertexPositionSets.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->GetListVertexPositionSets() = affectedVertexPositionSets;
		}
		NotifyRuleChanged();
	};
	
	enabled.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetEnabled(enabled);
		}
		NotifyRuleChanged();
	};
	
	pTargetBlendFactor = aeControllerTarget::Ref::New(targetBlendFactor);
	targetBlendFactor.onValueChanged = [this](){
		if(pEngRule){
			pUpdateEngineTarget(pEngRule->GetTargetBlendFactor(), targetBlendFactor);
		}
		pTargetBlendFactor->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRule::aeRule(aeWindowMain &windowMain, aeMCRule::Ref &&metaContext, const aeRule &copy) :
aeRule(windowMain, std::move(metaContext), copy.pType, copy.name)
{
	pInitCopy(copy);
}

aeRule::~aeRule(){
	SetAnimator(nullptr);
	if(pMetaContext){
		pMetaContext->Dispose();
	}
}



// Management
///////////////

aeAnimator *aeRule::GetAnimator() const{
	if(pParentGroup){
		return pParentGroup->GetAnimator();
		
	}else{
		return pAnimator;
	}
}

void aeRule::SetAnimator(aeAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	pEngRule = nullptr;
	pAnimator = animator;
	
	OnParentAnimatorChanged();
}

aeAnimator &aeRule::GetAnimatorRef() const{
	DEASSERT_NOTNULL(pAnimator);
	return *pAnimator;
}


void aeRule::SetEngineRule(deAnimatorRule *engRule){
	pEngRule = engRule;
}

void aeRule::SetIndex(int index){
	pIndex = index;
}

void aeRule::InitEngineRule(deAnimatorRule *engRule) const{
	if(!engRule){
		DETHROW(deeInvalidParam);
	}
	
	engRule->SetEnabled(enabled);
	engRule->SetBlendMode(blendMode);
	engRule->SetBlendFactor(blendFactor);
	engRule->SetInvertBlendFactor(invertBlendFactor);
	engRule->GetListBones() = affectedBones;
	engRule->GetListVertexPositionSets() = affectedVertexPositionSets;
	
	pUpdateEngineTarget(engRule->GetTargetBlendFactor(), targetBlendFactor);
}



void aeRule::SetParentGroup(aeRuleGroup *group){
	pParentGroup = group;
}



void aeRule::SetName(const char *value){
	name = value;
}

void aeRule::SetEnabled(bool value){
	enabled = value;
}

void aeRule::SetBlendMode(deAnimatorRule::eBlendModes value){
	blendMode = value;
}

void aeRule::SetBlendFactor(float value){
	blendFactor = value;
}

void aeRule::SetInvertBlendFactor(bool value){
	invertBlendFactor = value;
}


void aeRule::UpdateCompAnim(){
}

void aeRule::UpdateTargets(){
	aeAnimator * const animator = GetAnimator();
	if(pEngRule && animator){
		pUpdateEngineTarget(pEngRule->GetTargetBlendFactor(), targetBlendFactor);
	}
}

int aeRule::CountLinkUsage(aeLink *link) const{
	int usageCount = 0;
	
	if(pTargetBlendFactor->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void aeRule::RemoveLinkFromTargets(aeLink *link){
	if(pTargetBlendFactor->GetLinks().Has(link)){
		pTargetBlendFactor->RemoveLink(link);
	}
}

void aeRule::RemoveLinksFromAllTargets(){
	pTargetBlendFactor->RemoveAllLinks();
}



void aeRule::ListLinks(aeLink::List &list){
	pTargetBlendFactor->AddLinksToList(list);
}



void aeRule::NotifyRuleChanged(){
	aeAnimator * const animator = GetAnimator();
	if(animator){
		UpdateTargets();
		
		if(pEngRule){
			animator->GetEngineAnimator()->NotifyRulesChanged();
		}
		
		animator->NotifyRuleChanged(this);
	}
}



void aeRule::OnParentAnimatorChanged(){
}



// Bone Management
////////////////////

void aeRule::SetListBones(const decStringSet &bones){
	affectedBones = bones;
}

void aeRule::AddBone(const char *bone){
	affectedBones = affectedBones.GetValue() + decStringSet(devctag, bone);
}

void aeRule::RemoveBone(const char *bone){
	affectedBones = affectedBones.GetValue() - decStringSet(devctag, bone);
}

void aeRule::RemoveAllBones(){
	affectedBones = {};
}



// Vertex position set management
///////////////////////////////////

void aeRule::SetListVertexPositionSets(const decStringSet &sets){
	affectedVertexPositionSets = sets;
}

void aeRule::AddVertexPositionSet(const char *vps){
	affectedVertexPositionSets = affectedVertexPositionSets.GetValue() + decStringSet(devctag, vps);
}

void aeRule::RemoveVertexPositionSet(const char *vps){
	affectedVertexPositionSets = affectedVertexPositionSets.GetValue() - decStringSet(devctag, vps);
}

void aeRule::RemoveAllVertexPositionSets(){
	affectedVertexPositionSets = {};
}



// Helper
///////////

aeRule::Ref aeRule::CreateRuleFromType(aeWindowMain &windowMain,
deAnimatorRuleVisitorIdentify::eRuleTypes type,
const igdeTranslationManager &tm){
	switch(type){
	case deAnimatorRuleVisitorIdentify::ertAnimation:
		return aeRuleAnimation::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.Animation").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertAnimationDifference:
		return aeRuleAnimationDifference::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.AnimationDifference").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertAnimationSelect:
		return aeRuleAnimationSelect::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.AnimationSelect").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertBoneTransformator:
		return aeRuleBoneTransformator::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.BoneTransformator").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertForeignState:
		return aeRuleForeignState::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.ForeignState").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertGroup:
		return aeRuleGroup::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.Group").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertInverseKinematic:
		return aeRuleInverseKinematic::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.InverseKinematic").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertLimit:
		return aeRuleLimit::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.Limit").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertStateManipulator:
		return aeRuleStateManipulator::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.StateManipulator").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertStateSnapshot:
		return aeRuleStateSnapshot::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.StateSnapshot").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertSubAnimator:
		return aeRuleSubAnimator::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.SubAnimator").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertTrackTo:
		return aeRuleTrackTo::Ref::New(windowMain, tm.Translate("Animator.Action.Rule.TrackTo").ToUTF8());
		
	case deAnimatorRuleVisitorIdentify::ertMirror:
		return aeRuleMirror::CreateDefault(windowMain, tm.Translate("Animator.Action.Rule.Mirror").ToUTF8());
		
	default:
		DETHROW(deeInvalidParam);
	}
}


// Protected Functions
////////////////////////

void aeRule::pInitCopy(const aeRule &copy){
	name.SetValue(copy.name, false);
	blendMode.SetValue(copy.blendMode, false);
	blendFactor.SetValue(copy.blendFactor, false);
	invertBlendFactor.SetValue(copy.invertBlendFactor, false);
	enabled.SetValue(copy.enabled, false);
	affectedBones.SetValue(copy.affectedBones, false);
	affectedVertexPositionSets.SetValue(copy.affectedVertexPositionSets, false);
	pTargetBlendFactor = aeControllerTarget::Ref::New(targetBlendFactor, copy.pTargetBlendFactor);
}

void aeRule::pUpdateEngineTarget(deAnimatorControllerTarget &target,
const igdeMetaPropertyObjectSetStorage<aeLink>::Storage &storage) const{
	if(!pAnimator){
		return;
	}
	
	target.RemoveAllLinks();
	
	deAnimator * const engAnimator = pAnimator->GetEngineAnimator();
	if(!engAnimator){
		return;
	}
	
	storage->Visit([&](const aeLink &link){
		const auto engLink = link.GetEngineLink();
		if(engLink){
			const int indexLink = engAnimator->GetLinks().IndexOf(engLink);
			if(indexLink != -1){
				target.AddLink(indexLink);
			}
		}
	});
}
