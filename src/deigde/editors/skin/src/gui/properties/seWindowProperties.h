/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEWINDOWPROPERTIES_H_
#define _SEWINDOWPROPERTIES_H_

#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/properties/igdeWPUndoHistoryReference.h>

class seSkin;
class seWindowMain;



/**
 * \brief Properties panel.
 */
class seWindowProperties : public igdeTabBook{
private:
	seWindowMain &pWindowMain;
	
	igdeWidgetReference pPanelMapped;
	igdeWidgetReference pPanelTexture;
	igdeWidgetReference pPanelNode;
	igdeWidgetReference pPanelDynamicSkin;
	igdeWidgetReference pPanelView;
	igdeWPUndoHistoryReference pPanelUndoHistory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWindowProperties( seWindowMain &windowMain );
	
protected:
	/** \brief Cleanup window. */
	virtual ~seWindowProperties();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Set skin. */
	void SetSkin( seSkin *skin );
	
	/** \brief Skin path changed. */
	void OnSkinPathChanged();
	/*@}*/
};

#endif
