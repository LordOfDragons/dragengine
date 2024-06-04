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

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
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
	
	igdeListBoxReference pListController;
	
	igdeTextFieldReference pEditName;
	igdeTextFieldReference pEditMin;
	igdeTextFieldReference pEditMax;
	igdeButtonReference pBtnSetFromMove;
	igdeButtonReference pBtnResetValue;
	igdeEditSliderTextReference pSldValue;
	igdeEditVectorReference pEditVector;
	igdeCheckBoxReference pChkClamp;
	igdeCheckBoxReference pChkFrozen;
	igdeComboBoxReference pCBLocoAttr;
	igdeTextFieldReference pEditLocoLeg;
	igdeComboBoxReference pCBVectorSimulation;
	igdeTextFieldReference pEditDefaultValue;
	igdeEditVectorReference pEditDefaultVector;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPController( aeWindowProperties &windowProperties );
	
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
	void SetAnimator( aeAnimator *animator );
	
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
