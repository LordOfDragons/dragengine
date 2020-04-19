/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REWINDOWPROPERTIES_H_
#define _REWINDOWPROPERTIES_H_

#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/properties/igdeWPUndoHistoryReference.h>

class reWindowMain;

class reRig;
class reRigConstraint;



/**
 * \brief Shows Properties.
 */
class reWindowProperties : public igdeTabBook{
private:
	reWindowMain &pWindowMain;
	
	igdeWidgetReference pPanelRig;
	igdeWidgetReference pPanelBone;
	igdeWidgetReference pPanelShape;
	igdeWidgetReference pPanelConstraint;
	igdeWidgetReference pPanelPush;
	igdeWidgetReference pPanelView;
	igdeWPUndoHistoryReference pPanelUndoHistory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	reWindowProperties( reWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~reWindowProperties();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline reWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	/*@}*/
};

#endif
