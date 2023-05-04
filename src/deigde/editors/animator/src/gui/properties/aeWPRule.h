/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _AEWPRULE_H_
#define _AEWPRULE_H_

#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/igdeTreeListReference.h>
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
	aeWPRuleListener *pListener;
	aeAnimator *pAnimator;
	
	igdeTreeListReference pTreeRule;
	
	igdeSwitcherReference pSwitcher;
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
	aeWPRule( aeWindowProperties &windowProperties );
	
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
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** Active rule. */
	aeRule *GetRule() const;
	
	/** Update the controller list. */
	void UpdateControllerList();
	
	/** Update the link list. */
	void UpdateLinkList();
	
	/** Update the rule tree. */
	void UpdateRuleTree();
	
	/** Update item in the rule tree. */
	void UpdateRuleTreeItem( igdeTreeItem *item, aeRule *rule );
	
	/** Select active rule. */
	void SelectActiveRule();
	
	/** Show panel matching active source. */
	void ShowActiveSourcePanel();
	
	/** Update the active rule. */
	void UpdateRule();
	
	/** Update the rule bone list. */
	void UpdateRuleBoneList();
	
	/** Update the rule move list. */
	void UpdateRuleMoveList();
	
	/** Animator path changed. */
	void OnAnimatorPathChanged();
	/*@}*/
};

#endif
