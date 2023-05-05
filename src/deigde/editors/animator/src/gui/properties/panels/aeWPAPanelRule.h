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
 * Base rule option panel.
 */
class aeWPAPanelRule : public igdeContainerFlow{
private:
	aeWPRule &pWPRule;
	deAnimatorRuleVisitorIdentify::eRuleTypes pRequiredType;
	aeControllerTarget *pTarget;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBBlendMode;
	igdeTextFieldReference pEditBlendFactor;
	igdeCheckBoxReference pChkInvertBlendFactor;
	igdeCheckBoxReference pChkEnabled;
	
	igdeListBoxReference pListBones;
	igdeComboBoxFilterReference pCBBones;
	igdeButtonReference pBtnBoneAdd;
	igdeButtonReference pBtnBoneDel;
	
	igdeListBoxReference pListVertexPositionSets;
	igdeComboBoxFilterReference pCBVertexPositionSets;
	igdeButtonReference pBtnVertexPositionSetAdd;
	igdeButtonReference pBtnVertexPositionSetDel;
	
	igdeComboBoxReference pCBTarget;
	igdeListBoxReference pListLinks;
	igdeComboBoxFilterReference pCBLinks;
	igdeButtonReference pBtnLinkAdd;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPAPanelRule( aeWPRule &wpRule, deAnimatorRuleVisitorIdentify::eRuleTypes requiredType );
	
protected:
	/** Clean up panel. */
	virtual ~aeWPAPanelRule();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parent window. */
	inline aeWPRule &GetWPRule() const{ return pWPRule; }
	
	/** Required rule type. */
	inline deAnimatorRuleVisitorIdentify::eRuleTypes GetRequiredType() const{ return pRequiredType; }
	
	/** Main window. */
	aeWindowMain &GetWindowMain() const;
	
	
	
	/** Animator. */
	aeAnimator *GetAnimator() const;
	
	/** Selected rule or \em NULL. */
	aeRule *GetRule() const;
	
	/** Target or \em NULL. */
	inline aeControllerTarget *GetTarget() const{ return pTarget; }
	
	/** Set target or \em NULL. */
	void SetTarget( aeControllerTarget *target );
	
	
	
	/** Panel has been activated. */
	virtual void OnActivated();
	
	/** Animator changed. */
	virtual void OnAnimatorChanged();
	
	/** Animator path changed. */
	virtual void OnAnimatorPathChanged();
	
	/** Update controller list. */
	virtual void UpdateControllerList();
	
	/** Update link list. */
	virtual void UpdateLinkList();
	
	/** Update rig bone list. */
	virtual void UpdateRigBoneList();
	
	/** Update model vertex position set list. */
	virtual void UpdateModelVertexPositionSetList();
	
	/** Update animation move list. */
	virtual void UpdateAnimMoveList();
	
	/** Update rule. */
	virtual void UpdateRule();
	
	/** Update target list. */
	virtual void UpdateTargetList();
	
	/** Update target. */
	void UpdateTarget();
	
	/** Bone combo box text. */
	const decString &GetCBBoneText() const;
	
	/** Set bone combo box text. */
	void SetCBBoneText( const char *text );
	
	/** Selected bone list or NULL. */
	const char *GetListBoneSelection() const;
	
	/** Vertex position set combo box text. */
	const decString &GetCBVertexPositionSetText() const;
	
	/** Set vertex position set combo box text. */
	void SetCBVertexPositionSetText( const char *text );
	
	/** Selected vertex position set list or NULL. */
	const char *GetListVertexPositionSetSelection() const;
	
	/** Link combo box selection. */
	aeLink *GetCBLinkSelection() const;
	
	/** Set link combo box selection. */
	void SetCBLinkSelection( aeLink *selection );
	
	/** Link list selection. */
	aeLink *GetListLinkSelection() const;
	
	/** Set link list selection. */
	void SetListLinkSelection( aeLink *selection );
	
	
	
	/** Remove all targets. */
	void RemoveAllTargets();
	
	/** Add target. */
	void AddTarget( const char *name, aeControllerTarget *target );
	/*@}*/
};

#endif
