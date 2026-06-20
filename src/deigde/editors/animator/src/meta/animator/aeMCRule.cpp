/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "aeMCRule.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/rule/aeRuleAnimation.h"
#include "../../animator/rule/aeRuleAnimationDifference.h"
#include "../../animator/rule/aeRuleAnimationSelect.h"
#include "../../animator/rule/aeRuleBoneTransformator.h"
#include "../../animator/rule/aeRuleForeignState.h"
#include "../../animator/rule/aeRuleGroup.h"
#include "../../animator/rule/aeRuleInverseKinematic.h"
#include "../../animator/rule/aeRuleLimit.h"
#include "../../animator/rule/aeRuleMirror.h"
#include "../../animator/rule/aeRuleStateManipulator.h"
#include "../../animator/rule/aeRuleStateSnapshot.h"
#include "../../animator/rule/aeRuleSubAnimator.h"
#include "../../animator/rule/aeRuleTrackTo.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/common/exceptions.h>


// Class aeMCRule
///////////////////

// Constructor, destructor
////////////////////////////

aeMCRule::aeMCRule(aeWindowMain &windowMain, aeRule *rule, bool guardRule) :
igdeMetaContext("rule"),
pWindowMain(windowMain),
pRule(rule)
{
	SetLabel("Rule");
	SetDescription("Rule properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
	if(guardRule){
		pGuardRule = rule;
	}
}

aeMCRule::~aeMCRule() = default;


// Management
///////////////

aeRule &aeMCRule::GetRuleRef() const{
	DEASSERT_NOTNULL(pRule)
	return *pRule;
}

igdeMetaContext::Ref aeMCRule::Capture() const{
	return Ref::New(pWindowMain, pRule, true);
}

igdeEnvironment &aeMCRule::GetEnvironment() const{
	return pWindowMain.GetEnvironment();
}

igdeUndoSystem *aeMCRule::GetUndoSystem() const{
	return pRule && pRule->GetAnimator() ? pRule->GetAnimator()->GetUndoSystem() : nullptr;
}

igdeClipboard *aeMCRule::GetClipboard() const{
	return &pWindowMain.GetClipboard();
}


// Class aeMCRuleAnimation
////////////////////////////

aeMCRuleAnimation::aeMCRuleAnimation(aeWindowMain &windowMain, aeRuleAnimation *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Animation");
	SetDescription("Rule animation properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleAnimation.metaProperties);
}

aeMCRuleAnimation::~aeMCRuleAnimation() = default;

aeRuleAnimation *aeMCRuleAnimation::GetRuleType() const{
	return static_cast<aeRuleAnimation*>(GetRule());
}
aeRuleAnimation &aeMCRuleAnimation::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleAnimation::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleAnimationDifference
//////////////////////////////////////

aeMCRuleAnimationDifference::aeMCRuleAnimationDifference(aeWindowMain &windowMain, aeRuleAnimationDifference *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Animation Difference");
	SetDescription("Rule animation difference properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleAnimationDifference.metaProperties);
}

aeMCRuleAnimationDifference::~aeMCRuleAnimationDifference() = default;

aeRuleAnimationDifference *aeMCRuleAnimationDifference::GetRuleType() const{
	return static_cast<aeRuleAnimationDifference*>(GetRule());
}

aeRuleAnimationDifference &aeMCRuleAnimationDifference::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleAnimationDifference::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleAnimationSelect
//////////////////////////////////

aeMCRuleAnimationSelect::aeMCRuleAnimationSelect(aeWindowMain &windowMain, aeRuleAnimationSelect *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Animation Select");
	SetDescription("Rule animation select properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleAnimationSelect.metaProperties);
}

aeMCRuleAnimationSelect::~aeMCRuleAnimationSelect() = default;

aeRuleAnimationSelect *aeMCRuleAnimationSelect::GetRuleType() const{
	return static_cast<aeRuleAnimationSelect*>(GetRule());
}

aeRuleAnimationSelect &aeMCRuleAnimationSelect::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleAnimationSelect::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleBoneTransformator
////////////////////////////////////

aeMCRuleBoneTransformator::aeMCRuleBoneTransformator(aeWindowMain &windowMain, aeRuleBoneTransformator *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Bone Transformator");
	SetDescription("Rule bone transformator properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleBoneTransformator.metaProperties);
}

aeMCRuleBoneTransformator::~aeMCRuleBoneTransformator() = default;

aeRuleBoneTransformator *aeMCRuleBoneTransformator::GetRuleType() const{
	return static_cast<aeRuleBoneTransformator*>(GetRule());
}

aeRuleBoneTransformator &aeMCRuleBoneTransformator::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleBoneTransformator::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleForeignState
///////////////////////////////

aeMCRuleForeignState::aeMCRuleForeignState(aeWindowMain &windowMain, aeRuleForeignState *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Foreign State");
	SetDescription("Rule foreign state properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleForeignState.metaProperties);
}

aeMCRuleForeignState::~aeMCRuleForeignState() = default;

aeRuleForeignState *aeMCRuleForeignState::GetRuleType() const{
	return static_cast<aeRuleForeignState*>(GetRule());
}

aeRuleForeignState &aeMCRuleForeignState::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleForeignState::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleGroup
////////////////////////

aeMCRuleGroup::aeMCRuleGroup(aeWindowMain &windowMain, aeRuleGroup *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Group");
	SetDescription("Rule group properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleGroup.metaProperties);
}

aeMCRuleGroup::~aeMCRuleGroup() = default;

aeRuleGroup *aeMCRuleGroup::GetRuleType() const{
	return static_cast<aeRuleGroup*>(GetRule());
}

aeRuleGroup &aeMCRuleGroup::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleGroup::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleInverseKinematic
///////////////////////////////////

aeMCRuleInverseKinematic::aeMCRuleInverseKinematic(aeWindowMain &windowMain, aeRuleInverseKinematic *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Inverse Kinematic");
	SetDescription("Rule inverse kinematic properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleInverseKinematic.metaProperties);
}

aeMCRuleInverseKinematic::~aeMCRuleInverseKinematic() = default;

aeRuleInverseKinematic *aeMCRuleInverseKinematic::GetRuleType() const{
	return static_cast<aeRuleInverseKinematic*>(GetRule());
}

aeRuleInverseKinematic &aeMCRuleInverseKinematic::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleInverseKinematic::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleLimit
////////////////////////

aeMCRuleLimit::aeMCRuleLimit(aeWindowMain &windowMain, aeRuleLimit *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Limit");
	SetDescription("Rule limit properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleLimit.metaProperties);
}

aeMCRuleLimit::~aeMCRuleLimit() = default;

aeRuleLimit *aeMCRuleLimit::GetRuleType() const{
	return static_cast<aeRuleLimit*>(GetRule());
}

aeRuleLimit &aeMCRuleLimit::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleLimit::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleMirror
/////////////////////////

aeMCRuleMirror::aeMCRuleMirror(aeWindowMain &windowMain, aeRuleMirror *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Mirror");
	SetDescription("Rule mirror properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleMirror.metaProperties);
}

aeMCRuleMirror::~aeMCRuleMirror() = default;

aeRuleMirror *aeMCRuleMirror::GetRuleType() const{
	return static_cast<aeRuleMirror*>(GetRule());
}

aeRuleMirror &aeMCRuleMirror::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleMirror::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleStateManipulator
///////////////////////////////////

aeMCRuleStateManipulator::aeMCRuleStateManipulator(aeWindowMain &windowMain, aeRuleStateManipulator *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule State Manipulator");
	SetDescription("Rule state manipulator properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleStateManipulator.metaProperties);
}

aeMCRuleStateManipulator::~aeMCRuleStateManipulator() = default;

aeRuleStateManipulator *aeMCRuleStateManipulator::GetRuleType() const{
	return static_cast<aeRuleStateManipulator*>(GetRule());
}

aeRuleStateManipulator &aeMCRuleStateManipulator::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleStateManipulator::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleStateSnapshot
////////////////////////////////

aeMCRuleStateSnapshot::aeMCRuleStateSnapshot(aeWindowMain &windowMain, aeRuleStateSnapshot *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule State Snapshot");
	SetDescription("Rule state snapshot properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleStateSnapshot.metaProperties);
}

aeMCRuleStateSnapshot::~aeMCRuleStateSnapshot() = default;

aeRuleStateSnapshot *aeMCRuleStateSnapshot::GetRuleType() const{
	return static_cast<aeRuleStateSnapshot*>(GetRule());
}

aeRuleStateSnapshot &aeMCRuleStateSnapshot::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleStateSnapshot::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleSubAnimator
//////////////////////////////

aeMCRuleSubAnimator::aeMCRuleSubAnimator(aeWindowMain &windowMain, aeRuleSubAnimator *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Sub Animator");
	SetDescription("Rule sub animator properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleSubAnimator.metaProperties);
}

aeMCRuleSubAnimator::~aeMCRuleSubAnimator() = default;

aeRuleSubAnimator *aeMCRuleSubAnimator::GetRuleType() const{
	return static_cast<aeRuleSubAnimator*>(GetRule());
}

aeRuleSubAnimator &aeMCRuleSubAnimator::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleSubAnimator::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}


// Class aeMCRuleTrackTo
//////////////////////////

aeMCRuleTrackTo::aeMCRuleTrackTo(aeWindowMain &windowMain, aeRuleTrackTo *rule, bool guardRule) :
aeMCRule(windowMain, rule, guardRule)
{
	SetLabel("Rule Track To");
	SetDescription("Rule track to properties");
	SetProperties(windowMain.GetMCAnimatorProperties().ruleTrackTo.metaProperties);
}

aeMCRuleTrackTo::~aeMCRuleTrackTo() = default;

aeRuleTrackTo *aeMCRuleTrackTo::GetRuleType() const{
	return static_cast<aeRuleTrackTo*>(GetRule());
}

aeRuleTrackTo &aeMCRuleTrackTo::GetRuleTypeRef() const{
	auto r = GetRuleType();
	DEASSERT_NOTNULL(r)
	return *r;
}

igdeMetaContext::Ref aeMCRuleTrackTo::Capture() const{
	return Ref::New(GetWindowMain(), GetRuleType(), true);
}

