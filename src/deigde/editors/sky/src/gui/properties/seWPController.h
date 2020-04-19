/* 
 * Drag[en]gine IGDE Sky Editor
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

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/event/igdeActionReference.h>
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
	seWPControllerListener *pListener;
	
	seSky *pSky;
	
	igdeActionReference pActionControllerAdd;
	igdeActionReference pActionControllerRemove;
	igdeActionReference pActionControllerUp;
	igdeActionReference pActionControllerDown;
	
	igdeListBoxReference pListController;
	
	igdeTextFieldReference pEditName;
	igdeTextFieldReference pEditMin;
	igdeTextFieldReference pEditMax;
	igdeEditSliderTextReference pSldValue;
	igdeCheckBoxReference pChkClamp;
	igdeCheckBoxReference pChkFrozen;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPController( seWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sky. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky( seSky *sky );
	
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
	inline igdeAction *GetActionControllerAdd() const{ return pActionControllerAdd; }
	inline igdeAction *GetActionControllerRemove() const{ return pActionControllerRemove; }
	inline igdeAction *GetActionControllerUp() const{ return pActionControllerUp; }
	inline igdeAction *GetActionControllerDown() const{ return pActionControllerDown; }
	/*@}*/
};

#endif
