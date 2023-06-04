/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#ifndef _SAEWINDOWPROPERTIES_H_
#define _SAEWINDOWPROPERTIES_H_

#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/properties/igdeWPUndoHistoryReference.h>

class saeWindowMain;
class saeSAnimation;



/**
 * Properties Panel.
 */
class saeWindowProperties : public igdeTabBook{
private:
	saeWindowMain &pWindowMain;
	
	igdeWidgetReference pPanelSAnimation;
	igdeWidgetReference pPanelView;
	igdeWPUndoHistoryReference pPanelUndoHistory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	saeWindowProperties( saeWindowMain &windowMain );
	
protected:
	/** Clean up panel. */
	virtual ~saeWindowProperties();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline saeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Set speech animation. */
	void SetSAnimation( saeSAnimation *sanimation );
	
	/** Speech animation path changed. */
	void OnSAnimationPathChanged();
	/*@}*/
};

#endif
