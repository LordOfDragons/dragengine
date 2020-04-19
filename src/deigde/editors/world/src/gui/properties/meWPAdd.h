/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEWPADD_H_
#define _MEWPADD_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/event/igdeActionReference.h>

class meWorld;
class meWPAddListener;
class meWindowProperties;



/**
 * \brief Add panel.
 */
class meWPAdd : public igdeContainerScroll{
private:
	meWindowProperties &pWindowProperties;
	meWPAddListener *pListener;
	
	meWorld *pWorld;
	
	igdeActionReference pActionClassAdd;
	igdeActionReference pActionClassRemove;
	igdeActionReference pActionClassClear;
	
	igdeCheckBoxReference pChkFilterObjects;
	igdeComboBoxFilterReference pComboObjClass;
	igdeListBoxReference pListObjClasses;
	igdeCheckBoxReference pChkObjInclusive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPAdd( meWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPAdd();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Get world. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief Update object filter. */
	void UpdateObjectFilter();
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	/** \brief Actions. */
	inline igdeAction *GetActionClassAdd() const{ return pActionClassAdd; }
	inline igdeAction *GetActionClassRemove() const{ return pActionClassRemove; }
	inline igdeAction *GetActionClassClear() const{ return pActionClassClear; }
	/*@}*/
};

#endif
