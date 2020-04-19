/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWINDOWPROPERTIES_H_
#define _GDEWINDOWPROPERTIES_H_

#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/properties/igdeWPUndoHistoryReference.h>

class gdeWindowMain;
class gdeGameDefinition;
class gdeWPSelection;



/**
 * \brief Properties window.
 */
class gdeWindowProperties : public igdeTabBook{
private:
	gdeWindowMain &pWindowMain;
	
	igdeWidgetReference pPanelGameDefinition;
	igdeWidgetReference pPanelSelection;
	igdeWidgetReference pPanelView;
	igdeWPUndoHistoryReference pPanelUndoHistory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	gdeWindowProperties( gdeWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~gdeWindowProperties();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline gdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Set game definition to monitor. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	/** \brief Selection panel. */
	gdeWPSelection &GetPanelSelection() const;
	/*@}*/
};

#endif
