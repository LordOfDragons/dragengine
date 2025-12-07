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

#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class aeWPAPanelRuleLimit;
class aeAnimator;
class aeRule;
class aeWPAPanelRule;
class aeWPAPanelRuleAnimation;
class aeWPAPanelRuleAnimationDifference;
class aeWPAPanelRuleAnimationSelect;
class aeWPAPanelRuleBoneTransformator;
class aeWPAPanelRuleForeignState;
class aeWPAPanelRuleGroup;
class aeWPAPanelRuleInverseKinematic;
class aeWPAPanelRuleStateManipulator;
class aeWPAPanelRuleStateSnapshot;
class aeWPAPanelRuleSubAnimator;
class aeWPAPanelRuleTrackTo;
class aeWPAPanelRuleMirror;
class aeWindowProperties;
class aeWPRuleListener;

class igdeTreeItem;


/**
 * Base animator rule panel.
 */
class aeWPRule : public igdeContainerScroll{
private:
	aeWindowProperties &pWindowProperties;
	aeWPRuleListener::Ref pListener;
	aeAnimator::Ref pAnimator;
	
	igdeTreeList::Ref pTreeRule;
	
	igdeSwitcher::Ref pSwitcher;
	aeWPAPanelRuleAnimation *pPanelAnim;
	aeWPAPanelRuleAnimationDifference *pPanelAnimDiff;
	aeWPAPanelRuleAnimationSelect *pPanelAnimSelect;
	aeWPAPanelRuleBoneTransformator *pPanelBoneRot;
	aeWPAPanelRuleForeignState *pPanelFState;
	aeWPAPanelRuleGroup *pPanelGroup;
	aeWPAPanelRuleInverseKinematic *pPanelIK;
	aeWPAPanelRuleStateManipulator *pPanelSManip;
	aeWPAPanelRuleStateSnapshot *pPanelSSnapshot;
	aeWPAPanelRuleSubAnimator *pPanelSubAnimator;
	aeWPAPanelRuleTrackTo *pPanelTrackTo;
	aeWPAPanelRuleLimit *pPanelLimit;
	aeWPAPanelRuleMirror *pPanelMirror;
	aeWPAPanelRule *pActivePanel;
	
	
	
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
