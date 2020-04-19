/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#ifndef _LPEWINDOWPROPERTIES_H_
#define _LPEWINDOWPROPERTIES_H_

#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/properties/igdeWPUndoHistoryReference.h>

class lpeLangPack;
class lpeWindowMain;
class lpeWPLangPack;



/**
 * \brief Properties Panel.
 */
class lpeWindowProperties : public igdeTabBook{
private:
	lpeWindowMain &pWindowMain;
	
	lpeWPLangPack *pPanelLangPack;
	igdeWPUndoHistoryReference pPanelUndoHistory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	lpeWindowProperties( lpeWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~lpeWindowProperties();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline lpeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Set language pack or NULL. */
	void SetLangPack( lpeLangPack *langpack );
	/*@}*/
};

#endif
