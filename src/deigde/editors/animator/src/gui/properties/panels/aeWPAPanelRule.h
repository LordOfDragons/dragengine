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

#ifndef _AEWPAPANELRULE_H_
#define _AEWPAPANELRULE_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>


class aeAnimator;
class aeRule;
class aeLink;
class aeWPRule;
class aeWindowMain;
class aeControllerTarget;



/**
 * \brief Base rule option panel.
 */
class aeWPAPanelRule : public igdeContainerFlow{
private:
	aeWPRule &pWPRule;
	deAnimatorRuleVisitorIdentify::eRuleTypes pRequiredType;
	aeControllerTarget *pTarget;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBBlendMode;
	igdeTextFieldReference pEditBlendFactor;
	igdeCheckBoxReference pChkEnabled;
	
	igdeListBoxReference pListBones;
	igdeComboBoxFilterReference pCBBones;
	igdeButtonReference pBtnBoneAdd;
	igdeButtonReference pBtnBoneDel;
	
	igdeComboBoxReference pCBTarget;
	igdeListBoxReference pListLinks;
	igdeComboBoxFilterReference pCBLinks;
	igdeButtonReference pBtnLinkAdd;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	aeWPAPanelRule( aeWPRule &wpRule, deAnimatorRuleVisitorIdentify::eRuleTypes requiredType );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPAPanelRule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent window. */
	inline aeWPRule &GetWPRule() const{ return pWPRule; }
	
	/** \brief Required rule type. */
	inline deAnimatorRuleVisitorIdentify::eRuleTypes GetRequiredType() const{ return pRequiredType; }
	
	/** \brief Main window. */
	aeWindowMain &GetWindowMain() const;
	
	
	
	/** \brief Animator. */
	aeAnimator *GetAnimator() const;
	
	/** \brief Selected rule or \em NULL. */
	aeRule *GetRule() const;
	
	/** \brief Target or \em NULL. */
	inline aeControllerTarget *GetTarget() const{ return pTarget; }
	
	/** \brief Set target or \em NULL. */
	void SetTarget( aeControllerTarget *target );
	
	
	
	/** \brief Panel has been activated. */
	virtual void OnActivated();
	
	/** \brief Animator changed. */
	virtual void OnAnimatorChanged();
	
	/** \brief Update controller list. */
	virtual void UpdateControllerList();
	
	/** \brief Update link list. */
	virtual void UpdateLinkList();
	
	/** \brief Update rig bone list. */
	virtual void UpdateRigBoneList();
	
	/** \brief Update animation move list. */
	virtual void UpdateAnimMoveList();
	
	/** \brief Update rule. */
	virtual void UpdateRule();
	
	/** \brief Update target list. */
	virtual void UpdateTargetList();
	
	/** \brief Update target. */
	void UpdateTarget();
	
	/** \brief Bone combo box text. */
	const decString &GetCBBoneText() const;
	
	/** \brief Set bone combo box text. */
	void SetCBBoneText( const char *text );
	
	/** \brief Link combo box selection. */
	aeLink *GetCBLinkSelection() const;
	
	/** \brief Set link combo box selection. */
	void SetCBLinkSelection( aeLink *selection );
	
	/** \brief Link list selection. */
	aeLink *GetListLinkSelection() const;
	
	/** \brief Set link list selection. */
	void SetListLinkSelection( aeLink *selection );
	
	
	
	/** \brief Remove all targets. */
	void RemoveAllTargets();
	
	/** \brief Add target. */
	void AddTarget( const char *name, aeControllerTarget *target );
	/*@}*/
};

#endif
