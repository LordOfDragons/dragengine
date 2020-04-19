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
 * \brief Controller Panel.
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
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	aeWPController( aeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline aeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** \brief Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** \brief Active controller. */
	aeController *GetController() const;
	
	/** \brief Update controller list. */
	void UpdateControllerList();
	
	/** \brief Select active controller. */
	void SelectActiveController();
	
	/** \brief Update controller. */
	void UpdateController();
	
	/** \brief Update controller value only. */
	void UpdateControllerValue();
	/*@}*/
};

#endif
