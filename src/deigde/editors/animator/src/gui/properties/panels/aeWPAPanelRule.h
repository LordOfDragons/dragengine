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
