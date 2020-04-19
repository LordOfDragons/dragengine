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

#ifndef _SEWPUNDOHISTORY_H_
#define _SEWPUNDOHISTORY_H_

#include <deigde/gui/properties/igdeWPUndoHistory.h>

class seSkin;
class seWPUndoHistoryListener;



/**
 * \brief Undo History Panel.
 */
class seWPUndoHistory : public igdeWPUndoHistory{
	seWPUndoHistoryListener *pListener;
	seSkin *pSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPUndoHistory( igdeEnvironment &environment );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPUndoHistory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Skin. */
	inline seSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( seSkin *skin );
	/*@}*/
};

#endif
