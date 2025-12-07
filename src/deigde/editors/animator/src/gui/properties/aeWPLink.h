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

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include "aeAnimator.h"
#include "aeWPLinkListener.h"

class aeLink;
class aeWindowProperties;
class aeEditLinkMapping;



/**
 * Link panel.
 */
class aeWPLink : public igdeContainerScroll{
private:
	aeWindowProperties &pWindowProperties;
	aeWPLinkListener::Ref pListener;
	aeAnimator::Ref pAnimator;
	
	igdeListBox::Ref pListLink;
	
	igdeTextField::Ref pEditName;
	igdeComboBox::Ref pCBController;
	igdeSpinTextField::Ref pSpinRepeat;
	igdeViewCurveBezier::Ref pEditCurve;
	igdeComboBoxFilter::Ref pCBBone;
	igdeComboBox::Ref pCBBoneParameter;
	igdeTextField::Ref pEditBoneMinimum;
	igdeTextField::Ref pEditBoneMaximum;
	igdeComboBoxFilter::Ref pCBVertexPositionSet;
	igdeTextField::Ref pEditVertexPositionSetMinimum;
	igdeTextField::Ref pEditVertexPositionSetMaximum;
	igdeCheckBox::Ref pChkWrapY;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPLink(aeWindowProperties &windowProperties);
	
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
	inline const aeAnimator::Ref &GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator(aeAnimator *animator);
	
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
