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
 * \brief Base animator rule panel.
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
	/** \brief Create panel. */
	aeWPRule( aeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPRule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline aeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** \brief Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** \brief Active rule. */
	aeRule *GetRule() const;
	
	/** \brief Update the controller list. */
	void UpdateControllerList();
	
	/** \brief Update the link list. */
	void UpdateLinkList();
	
	/** \brief Update the rule tree. */
	void UpdateRuleTree();
	
	/** \brief Update item in the rule tree. */
	void UpdateRuleTreeItem( igdeTreeItem *item, aeRule *rule );
	
	/** \brief Select active rule. */
	void SelectActiveRule();
	
	/** \brief Show panel matching active source. */
	void ShowActiveSourcePanel();
	
	/** \brief Update the active rule. */
	void UpdateRule();
	
	/** \brief Update the rule bone list. */
	void UpdateRuleBoneList();
	
	/** \brief Update the rule move list. */
	void UpdateRuleMoveList();
	
	/** \brief Animator path changed. */
	void OnAnimatorPathChanged();
	/*@}*/
};

#endif
