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

#ifndef _LPEWPUNDOHISTORY_H_
#define _LPEWPUNDOHISTORY_H_

#include <deigde/gui/properties/igdeWPUndoHistory.h>

class lpeLangPack;
class lpeWPUndoHistoryListener;



/**
 * \brief Undo History Panel.
 */
class lpeWPUndoHistory : public igdeWPUndoHistory{
private:
	lpeWPUndoHistoryListener *pListener;
	lpeLangPack *pLangPack;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	lpeWPUndoHistory( igdeEnvironment &environment );
	
protected:
	/** \brief Clean up panel. */
	virtual ~lpeWPUndoHistory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Language pack or NULL. */
	inline lpeLangPack *GetLangPack() const{ return pLangPack; }
	
	/** \brief Set language pack or NULL. */
	void SetLangPack( lpeLangPack *langpack );
	/*@}*/
};

#endif
