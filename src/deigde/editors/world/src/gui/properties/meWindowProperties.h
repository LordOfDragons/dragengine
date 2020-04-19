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

#ifndef _MEWINDOWPROPERTIES_H_
#define _MEWINDOWPROPERTIES_H_

#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/properties/igdeWPUndoHistoryReference.h>

class meWorld;
class meWindowMain;
class meWPSelection;
class meWPView;
class meWPSensors;
class meWPWorld;
class meWPHeightTerrain;
class meWPHeightTerrain;
class meWPBrowser;
class meWPAdd;



/**
 * \brief Shows Properties.
 */
class meWindowProperties : public igdeTabBook{
private:
	meWindowMain &pWindowMain;
	
	meWPSelection *pPropSelection;
	meWPView *pPropView;
	meWPWorld *pPropWorld;
	meWPSensors *pPropSensors;
	meWPHeightTerrain *pPropHT;
	igdeWPUndoHistoryReference pPanelUndoHistory;
	meWPBrowser *pPropBrowser;
	meWPAdd *pPropAdd;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	meWindowProperties( meWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~meWindowProperties();
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	/** \brief Browser panel. */
	inline meWPBrowser &GetBrowser() const{ return *pPropBrowser; }
	
	/** \brief Switch to browser panel. */
	void SwitchToBrowser();
};

#endif
