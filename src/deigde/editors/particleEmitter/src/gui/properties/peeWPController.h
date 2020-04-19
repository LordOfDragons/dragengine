/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEEWPCONTROLLER_H_
#define _PEEWPCONTROLLER_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class peeEmitter;
class peeController;
class peeWindowProperties;
class peeWPControllerListener;


/**
 * \brief Controller panel.
 */
class peeWPController : public igdeContainerScroll{
private:
	peeWindowProperties &pWindowProperties;
	peeWPControllerListener *pListener;
	
	peeEmitter *pEmitter;
	
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
	peeWPController( peeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~peeWPController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Emitter. */
	inline peeEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter. */
	void SetEmitter( peeEmitter *emitter );
	
	/** \brief Active controller or \em NULL. */
	peeController *GetController() const;
	
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
