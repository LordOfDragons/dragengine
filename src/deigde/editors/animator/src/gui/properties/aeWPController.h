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

#ifndef _AEWPCONTROLLER_H_
#define _AEWPCONTROLLER_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class aeAnimator;
class aeController;
class aeWindowProperties;
class aeWPControllerListener;


/**
 * Controller Panel.
 */
class aeWPController : public igdeContainerScroll{
private:
	aeWindowProperties &pWindowProperties;
	aeWPControllerListener *pListener;
	aeAnimator *pAnimator;
	
	igdeListBox::Ref pListController;
	
	igdeTextField::Ref pEditName;
	igdeTextField::Ref pEditMin;
	igdeTextField::Ref pEditMax;
	igdeButton::Ref pBtnSetFromMove;
	igdeButton::Ref pBtnResetValue;
	igdeEditSliderText::Ref pSldValue;
	igdeEditVector::Ref pEditVector;
	igdeCheckBox::Ref pChkClamp;
	igdeCheckBox::Ref pChkFrozen;
	igdeComboBox::Ref pCBLocoAttr;
	igdeTextField::Ref pEditLocoLeg;
	igdeComboBox::Ref pCBVectorSimulation;
	igdeTextField::Ref pEditDefaultValue;
	igdeEditVector::Ref pEditDefaultVector;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPController(aeWindowProperties &windowProperties);
	
protected:
	/** Clean up panel. */
	virtual ~aeWPController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline aeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Active controller. */
	aeController *GetController() const;
	
	/** Update controller list. */
	void UpdateControllerList();
	
	/** Select active controller. */
	void SelectActiveController();
	
	/** Update controller. */
	void UpdateController();
	
	/** Update controller value only. */
	void UpdateControllerValue();
	/*@}*/
};

#endif
