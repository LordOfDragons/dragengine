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

#ifndef _AEWPLINK_H_
#define _AEWPLINK_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class aeAnimator;
class aeLink;
class aeWindowProperties;
class aeWPLinkListener;
class aeEditLinkMapping;



/**
 * Link panel.
 */
class aeWPLink : public igdeContainerScroll{
private:
	aeWindowProperties &pWindowProperties;
	aeWPLinkListener *pListener;
	aeAnimator *pAnimator;
	
	igdeListBoxReference pListLink;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBController;
	igdeSpinTextFieldReference pSpinRepeat;
	igdeViewCurveBezierReference pEditCurve;
	igdeComboBoxFilterReference pCBBone;
	igdeComboBoxReference pCBBoneParameter;
	igdeTextFieldReference pEditBoneMinimum;
	igdeTextFieldReference pEditBoneMaximum;
	igdeComboBoxFilterReference pCBVertexPositionSet;
	igdeTextFieldReference pEditVertexPositionSetMinimum;
	igdeTextFieldReference pEditVertexPositionSetMaximum;
	igdeCheckBoxReference pChkWrapY;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPLink( aeWindowProperties &windowProperties );
	
protected:
	/** Clean up panel. */
	virtual ~aeWPLink();
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
	
	/** Active link. */
	aeLink *GetLink() const;
	
	/** Update link list. */
	void UpdateLinkList();
	
	/** Select active link. */
	void SelectActiveLink();
	
	/** Update link. */
	void UpdateLink();
	
	/** Update rig bone list. */
	void UpdateRigBoneList();
	
	/** Update model vertex position set list. */
	void UpdateModelVertexPositionSetList();
	
	/** Update controller list. */
	void UpdateControllerList();
	/*@}*/
};

#endif
