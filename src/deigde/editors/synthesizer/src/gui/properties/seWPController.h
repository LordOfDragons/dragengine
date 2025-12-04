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

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSynthesizer;
class seController;
class seViewSynthesizer;
class seWPControllerListener;



/**
 * \brief Controller properties panel.
 */
class seWPController : public igdeContainerScroll{
private:
	seViewSynthesizer &pViewSynthesizer;
	seWPControllerListener *pListener;
	
	seSynthesizer *pSynthesizer;
	
	igdeListBox::Ref pListController;
	igdeTextField::Ref pEditName;
	igdeTextField::Ref pEditMin;
	igdeTextField::Ref pEditMax;
	igdeViewCurveBezier::Ref pEditCurve;
	igdeCheckBox::Ref pChkClamp;
	
	igdeTextField::Ref pEditCurveSetConstValue;
	igdeButton::Ref pBtnCurveSetConstant;
	igdeButton::Ref pBtnCurveSetLinear;
	igdeButton::Ref pBtnCurveSetLinearInverse;
	igdeButton::Ref pBtnCurveSetBezier;
	igdeButton::Ref pBtnCurveSetBezierInverse;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create properties panel. */
	seWPController( seViewSynthesizer &viewSynthesizer );
	
protected:
	/** \brief Clean up properties panel. */
	virtual ~seWPController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View. */
	inline seViewSynthesizer &GetViewSynthesizer() const{ return pViewSynthesizer; }
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	/** \brief Controller or \em NULL if not set. */
	seController *GetController() const;
	
	
	
	/** \brief Update controller list. */
	void UpdateControllerList();
	
	/** \brief Select active controller. */
	void SelectActiveController();
	
	/** \brief Update controller. */
	void UpdateController();
	
	/** \brief Update controller range. */
	void UpdateControllerRange();
	
	/** \brief Update controller curve. */
	void UpdateControllerCurve();
	/*@}*/
};

#endif
