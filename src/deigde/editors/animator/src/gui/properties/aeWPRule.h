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

#ifndef _AEWPRULE_H_
#define _AEWPRULE_H_

#include "aeWPRuleListener.h"
#include "panels/aeWPAPanelRuleAnimation.h"
#include "panels/aeWPAPanelRuleAnimationDifference.h"
#include "panels/aeWPAPanelRuleAnimationSelect.h"
#include "panels/aeWPAPanelRuleBoneTransformator.h"
#include "panels/aeWPAPanelRuleForeignState.h"
#include "panels/aeWPAPanelRuleGroup.h"
#include "panels/aeWPAPanelRuleInverseKinematic.h"
#include "panels/aeWPAPanelRuleLimit.h"
#include "panels/aeWPAPanelRuleStateManipulator.h"
#include "panels/aeWPAPanelRuleStateSnapshot.h"
#include "panels/aeWPAPanelRuleSubAnimator.h"
#include "panels/aeWPAPanelRuleTrackTo.h"
#include "panels/aeWPAPanelRuleMirror.h"
#include "../../animator/aeAnimator.h"

#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class aeRule;
class aeWindowProperties;

class igdeTreeItem;


/**
 * Base animator rule panel.
 */
class aeWPRule : public igdeContainerScroll{
public:
	typedef deTObjectReference<aeWPRule> Ref;
	
	
private:
	aeWindowProperties &pWindowProperties;
	aeWPRuleListener::Ref pListener;
	aeAnimator::Ref pAnimator;
	
	igdeTreeList::Ref pTreeRule;
	
	igdeSwitcher::Ref pSwitcher;
	aeWPAPanelRuleAnimation::Ref pPanelAnim;
	aeWPAPanelRuleAnimationDifference::Ref pPanelAnimDiff;
	aeWPAPanelRuleAnimationSelect::Ref pPanelAnimSelect;
	aeWPAPanelRuleBoneTransformator::Ref pPanelBoneRot;
	aeWPAPanelRuleForeignState::Ref pPanelFState;
	aeWPAPanelRuleGroup::Ref pPanelGroup;
	aeWPAPanelRuleInverseKinematic::Ref pPanelIK;
	aeWPAPanelRuleStateManipulator::Ref pPanelSManip;
	aeWPAPanelRuleStateSnapshot::Ref pPanelSSnapshot;
	aeWPAPanelRuleSubAnimator::Ref pPanelSubAnimator;
	aeWPAPanelRuleTrackTo::Ref pPanelTrackTo;
	aeWPAPanelRuleLimit::Ref pPanelLimit;
	aeWPAPanelRuleMirror::Ref pPanelMirror;
	aeWPAPanelRule::Ref pActivePanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPRule(aeWindowProperties &windowProperties);
	
protected:
	/** Clean up panel. */
	virtual ~aeWPRule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline aeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Animator. */
	inline const aeAnimator::Ref &GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Active rule. */
	aeRule *GetRule() const;
	
	/** Update the controller list. */
	void UpdateControllerList();
	
	/** Update the link list. */
	void UpdateLinkList();
	
	/** Update the rule tree. */
	void UpdateRuleTree();
	
	/** Update item in the rule tree. */
	void UpdateRuleTreeItem(igdeTreeItem *item, aeRule *rule);
	
	/** Select active rule. */
	void SelectActiveRule();
	
	/** Show panel matching active source. */
	void ShowActiveSourcePanel();
	
	/** Update the active rule. */
	void UpdateRule();
	
	/** Update the rule bone list. */
	void UpdateRuleBoneList();
	
	/** Update the rule vertex position set list. */
	void UpdateRuleVertexPositionSetList();
	
	/** Update the rule move list. */
	void UpdateRuleMoveList();
	
	/** Animator path changed. */
	void OnAnimatorPathChanged();
	/*@}*/
};

#endif
