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

#ifndef _MEWPSELECTION_H_
#define _MEWPSELECTION_H_

#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeWidgetReference.h>

class meWPSelectionListener;
class meWindowProperties;
class meWorld;



/**
 * \brief Selection panel.
 */
class meWPSelection : public igdeSwitcher{
private:
	meWindowProperties &pWindowProperties;
	meWPSelectionListener *pListener;
	
	meWorld *pWorld;
	
	igdeWidgetReference pPanelObject;
	igdeWidgetReference pPanelObjectShape;
	igdeWidgetReference pPanelDecal;
	igdeWidgetReference pPanelNavSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSelection( meWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPSelection();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline meWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief Element mode changed. */
	void ElementModeChanged();
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	/*@}*/
};

#endif
