/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SEWPCONTROLLER_H_
#define _SEWPCONTROLLER_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierReference.h>
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
	
	igdeListBoxReference pListController;
	igdeTextFieldReference pEditName;
	igdeTextFieldReference pEditMin;
	igdeTextFieldReference pEditMax;
	igdeViewCurveBezierReference pEditCurve;
	igdeCheckBoxReference pChkClamp;
	
	igdeTextFieldReference pEditCurveSetConstValue;
	igdeButtonReference pBtnCurveSetConstant;
	igdeButtonReference pBtnCurveSetLinear;
	igdeButtonReference pBtnCurveSetLinearInverse;
	igdeButtonReference pBtnCurveSetBezier;
	igdeButtonReference pBtnCurveSetBezierInverse;
	
	
	
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
