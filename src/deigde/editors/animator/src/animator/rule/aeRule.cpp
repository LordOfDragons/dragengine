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

aeRule::MetaContext::Ref aeRule::CreateMetaContext(aeWindowMain &windowMain, aeRule *rule){
	return MetaContext::Ref::New("animator.rule", "Rule", "Rule properties",
		windowMain.GetMCAnimatorProperties().rule.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRule::aeRule(aeWindowMain &windowMain, const MetaContext::Ref &metaContext,
	deAnimatorRuleVisitorIdentify::eRuleTypes type, const char *aname) :
pAnimator(nullptr),
pParentGroup(nullptr),
pEngRule(nullptr),
pIndex(-1),
pMetaContext(metaContext),
pType(type),
pMPName(windowMain.GetMCAnimatorProperties().rule.name, pMetaContext, aname),
pMPBlendMode(windowMain.GetMCAnimatorProperties().rule.blendMode, pMetaContext),
pMPBlendFactor(windowMain.GetMCAnimatorProperties().rule.blendFactor, pMetaContext),
pMPInvertBlendFactor(windowMain.GetMCAnimatorProperties().rule.invertBlendFactor, pMetaContext),
pMPEnabled(windowMain.GetMCAnimatorProperties().rule.enabled, pMetaContext),
pMPAffectedBones(windowMain.GetMCAnimatorProperties().rule.affectedBones, pMetaContext),
pMPAffectedVps(windowMain.GetMCAnimatorProperties().rule.affectedVertexPositionSets, pMetaContext),
pMPTargetBlendFactor(windowMain.GetMCAnimatorProperties().rule.targetBlendFactor, pMetaContext)
{
	pMPName.onValueChanged = [this](){
		if(pAnimator){
			pAnimator->NotifyRuleNameChanged(this);
		}
	};
	
	pMPBlendMode.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetBlendMode(pMPBlendMode);
		}
		NotifyRuleChanged();
	};
	
	pMPBlendFactor.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetBlendFactor(pMPBlendFactor);
		}
		NotifyRuleChanged();
	};
	
	pMPInvertBlendFactor.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetInvertBlendFactor(pMPInvertBlendFactor);
		}
		NotifyRuleChanged();
	};
	
	pMPAffectedBones.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->GetListBones() = pMPAffectedBones;
		}
		NotifyRuleChanged();
	};
	
	pMPAffectedVps.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->GetListVertexPositionSets() = pMPAffectedVps;
		}
		NotifyRuleChanged();
	};
	
	pMPEnabled.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetEnabled(pMPEnabled);
		}
		NotifyRuleChanged();
	};
	
	pTargetBlendFactor = aeControllerTarget::Ref::New(pMPTargetBlendFactor);
	pMPTargetBlendFactor.onValueChanged = [this](){
		if(pEngRule){
			pUpdateEngineTarget(pEngRule->GetTargetBlendFactor(), pMPTargetBlendFactor);
		}
		pTargetBlendFactor->OnStorageChanged();
		NotifyRuleChanged();
	};
}

aeRule::aeRule(aeWindowMain &windowMain, const MetaContext::Ref &metaContext, const aeRule &copy) :
aeRule(windowMain, metaContext, copy.pType, copy.pMPName)
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

igdeEnvironment &aeRule::GetEnvironment() const{
	return GetAnimatorRef().GetEnvironment();
}

igdeUndoSystem *aeRule::GetUndoSystem() const{
	return GetAnimatorRef().GetUndoSystem();
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
	
	engRule->SetEnabled(pMPEnabled);
	engRule->SetBlendMode(pMPBlendMode);
	engRule->SetBlendFactor(pMPBlendFactor);
	engRule->SetInvertBlendFactor(pMPInvertBlendFactor);
	engRule->GetListBones() = pMPAffectedBones;
	engRule->GetListVertexPositionSets() = pMPAffectedVps;
	
	pUpdateEngineTarget(engRule->GetTargetBlendFactor(), pMPTargetBlendFactor);
}



void aeRule::SetParentGroup(aeRuleGroup *group){
	pParentGroup = group;
}



void aeRule::SetName(const char *value){
	pMPName = value;
}

void aeRule::SetEnabled(bool value){
	pMPEnabled = value;
}

void aeRule::SetBlendMode(deAnimatorRule::eBlendModes value){
	pMPBlendMode = value;
}

void aeRule::SetBlendFactor(float value){
	pMPBlendFactor = value;
}

void aeRule::SetInvertBlendFactor(bool value){
	pMPInvertBlendFactor = value;
}


void aeRule::UpdateCompAnim(){
}

void aeRule::UpdateTargets(){
	aeAnimator * const animator = GetAnimator();
	if(pEngRule && animator){
		pUpdateEngineTarget(pEngRule->GetTargetBlendFactor(), pMPTargetBlendFactor);
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
	pMPAffectedBones = bones;
}

void aeRule::AddBone(const char *bone){
	pMPAffectedBones = pMPAffectedBones.GetValue() + decStringSet(devctag, bone);
}

void aeRule::RemoveBone(const char *bone){
	pMPAffectedBones = pMPAffectedBones.GetValue() - decStringSet(devctag, bone);
}

void aeRule::RemoveAllBones(){
	pMPAffectedBones = {};
}



// Vertex position set management
///////////////////////////////////

void aeRule::SetListVertexPositionSets(const decStringSet &sets){
	pMPAffectedVps = sets;
}

void aeRule::AddVertexPositionSet(const char *vps){
	pMPAffectedVps = pMPAffectedVps.GetValue() + decStringSet(devctag, vps);
}

void aeRule::RemoveVertexPositionSet(const char *vps){
	pMPAffectedVps = pMPAffectedVps.GetValue() - decStringSet(devctag, vps);
}

void aeRule::RemoveAllVertexPositionSets(){
	pMPAffectedVps = {};
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
	pMPName.SetValue(copy.pMPName, false);
	pMPBlendMode.SetValue(copy.pMPBlendMode, false);
	pMPBlendFactor.SetValue(copy.pMPBlendFactor, false);
	pMPInvertBlendFactor.SetValue(copy.pMPInvertBlendFactor, false);
	pMPEnabled.SetValue(copy.pMPEnabled, false);
	pMPAffectedBones.SetValue(copy.pMPAffectedBones, false);
	pMPAffectedVps.SetValue(copy.pMPAffectedVps, false);
	pTargetBlendFactor = aeControllerTarget::Ref::New(pMPTargetBlendFactor, copy.pTargetBlendFactor);
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
