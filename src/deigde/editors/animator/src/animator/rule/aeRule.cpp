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
pWindowMain(windowMain),
pAnimator(nullptr),
pParentGroup(nullptr),
pEngRule(nullptr),
pIndex(-1),
pMetaContext(metaContext),
pType(type),
mpName(windowMain.GetMCAnimatorProperties().rule.name, pMetaContext, aname),
mpBlendMode(windowMain.GetMCAnimatorProperties().rule.blendMode, pMetaContext),
mpBlendFactor(windowMain.GetMCAnimatorProperties().rule.blendFactor, pMetaContext),
mpInvertBlendFactor(windowMain.GetMCAnimatorProperties().rule.invertBlendFactor, pMetaContext),
mpEnabled(windowMain.GetMCAnimatorProperties().rule.enabled, pMetaContext),
mpAffectedBones(windowMain.GetMCAnimatorProperties().rule.affectedBones, pMetaContext),
mpAffectedVps(windowMain.GetMCAnimatorProperties().rule.affectedVertexPositionSets, pMetaContext),
mpTargetBlendFactor(windowMain.GetMCAnimatorProperties().rule.targetBlendFactor, pMetaContext)
{
	mpName.onValueChanged = [this](){
		if(pAnimator){
			pAnimator->SetChanged(true);
		}
	};
	
	mpBlendMode.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetBlendMode(mpBlendMode);
		}
		NotifyRuleChanged();
	};
	
	mpBlendFactor.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetBlendFactor(mpBlendFactor);
		}
		NotifyRuleChanged();
	};
	
	mpInvertBlendFactor.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetInvertBlendFactor(mpInvertBlendFactor);
		}
		NotifyRuleChanged();
	};
	
	mpAffectedBones.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->GetListBones() = mpAffectedBones;
		}
		NotifyRuleChanged();
	};
	
	mpAffectedVps.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->GetListVertexPositionSets() = mpAffectedVps;
		}
		NotifyRuleChanged();
	};
	
	mpEnabled.onValueChanged = [this](){
		if(pEngRule){
			pEngRule->SetEnabled(mpEnabled);
		}
		NotifyRuleChanged();
	};
	
	mpTargetBlendFactor.onValueChanged = [this](){
		if(pEngRule){
			pUpdateEngineTarget(pEngRule->GetTargetBlendFactor(), mpTargetBlendFactor);
		}
		NotifyRuleChanged();
	};
}

aeRule::aeRule(const MetaContext::Ref &metaContext, const aeRule &copy) :
aeRule(copy.pWindowMain, metaContext, copy.pType, copy.mpName)
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
	
	engRule->SetEnabled(mpEnabled);
	engRule->SetBlendMode(mpBlendMode);
	engRule->SetBlendFactor(mpBlendFactor);
	engRule->SetInvertBlendFactor(mpInvertBlendFactor);
	engRule->GetListBones() = mpAffectedBones;
	engRule->GetListVertexPositionSets() = mpAffectedVps;
	
	pUpdateEngineTarget(engRule->GetTargetBlendFactor(), mpTargetBlendFactor);
}



void aeRule::SetParentGroup(aeRuleGroup *group){
	pParentGroup = group;
}


void aeRule::UpdateCompAnim(){
}

void aeRule::UpdateTargets(){
	aeAnimator * const animator = GetAnimator();
	if(pEngRule && animator){
		pUpdateEngineTarget(pEngRule->GetTargetBlendFactor(), mpTargetBlendFactor);
	}
}

int aeRule::CountLinkUsage(aeLink *link) const{
	int usageCount = 0;
	if(mpTargetBlendFactor->Has(link)){
		usageCount++;
	}
	return usageCount;
}

void aeRule::NotifyRuleChanged(){
	aeAnimator * const animator = GetAnimator();
	if(animator){
		UpdateTargets();
		
		if(pEngRule){
			animator->GetEngineAnimator()->NotifyRulesChanged();
		}
		
		animator->SetChanged(true);
	}
}



void aeRule::OnParentAnimatorChanged(){
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
	mpName.SetValue(copy.mpName, false);
	mpBlendMode.SetValue(copy.mpBlendMode, false);
	mpBlendFactor.SetValue(copy.mpBlendFactor, false);
	mpInvertBlendFactor.SetValue(copy.mpInvertBlendFactor, false);
	mpEnabled.SetValue(copy.mpEnabled, false);
	mpAffectedBones.SetValue(copy.mpAffectedBones, false);
	mpAffectedVps.SetValue(copy.mpAffectedVps, false);
	mpTargetBlendFactor.SetValue(copy.mpTargetBlendFactor, false);
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
