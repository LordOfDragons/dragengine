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

#ifndef _SEWPCONTROLLER_H_
#define _SEWPCONTROLLER_H_

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSky;
class seController;
class seWindowProperties;
class seWPControllerListener;


/**
 * \brief Controller panel.
 */
class seWPController : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPControllerListener::Ref pListener;
	
	seSky::Ref pSky;
	
	igdeAction::Ref pActionControllerAdd;
	igdeAction::Ref pActionControllerRemove;
	igdeAction::Ref pActionControllerUp;
	igdeAction::Ref pActionControllerDown;
	
	igdeListBox::Ref pListController;
	
	igdeTextField::Ref pEditName;
	igdeTextField::Ref pEditMin;
	igdeTextField::Ref pEditMax;
	igdeEditSliderText::Ref pSldValue;
	igdeCheckBox::Ref pChkClamp;
	igdeCheckBox::Ref pChkFrozen;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPController(seWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sky. */
	inline const seSky::Ref &GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky(seSky *sky);
	
	/** \brief Active controller or \em NULL. */
	seController *GetController() const;
	
	/** \brief Update controller list. */
	void UpdateControllerList();
	
	/** \brief Change selection to active controller. */
	void SelectActiveController();
	
	/** \brief Update controller. */
	void UpdateController();
	
	/** \brief Update controller value only. */
	void UpdateControllerValue();
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionControllerAdd() const{ return pActionControllerAdd; }
	inline const igdeAction::Ref &GetActionControllerRemove() const{ return pActionControllerRemove; }
	inline const igdeAction::Ref &GetActionControllerUp() const{ return pActionControllerUp; }
	inline const igdeAction::Ref &GetActionControllerDown() const{ return pActionControllerDown; }
	/*@}*/
};

#endif
