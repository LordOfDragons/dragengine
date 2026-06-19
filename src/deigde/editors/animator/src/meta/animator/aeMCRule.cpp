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

aeMCRule::aeMCRule(aeWindowMain &windowMain, aeRule *rule) :
igdeMetaContext("rule"),
pWindowMain(windowMain),
pRule(rule)
{
	SetLabel("Rule");
	SetDescription("Rule properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
}

aeMCRule::~aeMCRule() = default;


// Management
///////////////

aeRule &aeMCRule::GetRuleRef() const{
	DEASSERT_NOTNULL(pRule)
	return *pRule;
}

aeMCRule::Ref aeMCRule::Capture() const{
	auto context = Ref::New(pWindowMain, pRule);
	context->pGuardRule = pRule;
	return context;
}

igdeUndoSystem *aeMCRule::GetUndoSystem() const{
	return pRule && pRule->GetAnimator() ? pRule->GetAnimator()->GetUndoSystem() : nullptr;
}

igdeClipboard *aeMCRule::GetClipboard() const{
	return &pWindowMain.GetClipboard();
}


// Class aeMCRuleAnimation
////////////////////////////

aeMCRuleAnimation::aeMCRuleAnimation(aeWindowMain &windowMain, aeRuleAnimation *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Animation");
	SetDescription("Rule animation properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleAnimationDifference
//////////////////////////////////////

aeMCRuleAnimationDifference::aeMCRuleAnimationDifference(aeWindowMain &windowMain, aeRuleAnimationDifference *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Animation Difference");
	SetDescription("Rule animation difference properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleAnimationSelect
//////////////////////////////////

aeMCRuleAnimationSelect::aeMCRuleAnimationSelect(aeWindowMain &windowMain, aeRuleAnimationSelect *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Animation Select");
	SetDescription("Rule animation select properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleBoneTransformator
////////////////////////////////////

aeMCRuleBoneTransformator::aeMCRuleBoneTransformator(aeWindowMain &windowMain, aeRuleBoneTransformator *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Bone Transformator");
	SetDescription("Rule bone transformator properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleForeignState
///////////////////////////////

aeMCRuleForeignState::aeMCRuleForeignState(aeWindowMain &windowMain, aeRuleForeignState *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Foreign State");
	SetDescription("Rule foreign state properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleGroup
////////////////////////

aeMCRuleGroup::aeMCRuleGroup(aeWindowMain &windowMain, aeRuleGroup *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Group");
	SetDescription("Rule group properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleInverseKinematic
///////////////////////////////////

aeMCRuleInverseKinematic::aeMCRuleInverseKinematic(aeWindowMain &windowMain, aeRuleInverseKinematic *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Inverse Kinematic");
	SetDescription("Rule inverse kinematic properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleLimit
////////////////////////

aeMCRuleLimit::aeMCRuleLimit(aeWindowMain &windowMain, aeRuleLimit *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Limit");
	SetDescription("Rule limit properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleMirror
/////////////////////////

aeMCRuleMirror::aeMCRuleMirror(aeWindowMain &windowMain, aeRuleMirror *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Mirror");
	SetDescription("Rule mirror properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleStateManipulator
///////////////////////////////////

aeMCRuleStateManipulator::aeMCRuleStateManipulator(aeWindowMain &windowMain, aeRuleStateManipulator *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule State Manipulator");
	SetDescription("Rule state manipulator properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleStateSnapshot
////////////////////////////////

aeMCRuleStateSnapshot::aeMCRuleStateSnapshot(aeWindowMain &windowMain, aeRuleStateSnapshot *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule State Snapshot");
	SetDescription("Rule state snapshot properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleSubAnimator
//////////////////////////////

aeMCRuleSubAnimator::aeMCRuleSubAnimator(aeWindowMain &windowMain, aeRuleSubAnimator *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Sub Animator");
	SetDescription("Rule sub animator properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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


// Class aeMCRuleTrackTo
//////////////////////////

aeMCRuleTrackTo::aeMCRuleTrackTo(aeWindowMain &windowMain, aeRuleTrackTo *rule) :
aeMCRule(windowMain, rule)
{
	SetLabel("Rule Track To");
	SetDescription("Rule track to properties");
	SetProperties(windowMain.GetMCAnimatorProperties().rule.metaProperties);
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

