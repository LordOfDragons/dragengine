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

#ifndef _GDEWPUNDOHISTORY_H_
#define _GDEWPUNDOHISTORY_H_

#include <deigde/gui/properties/igdeWPUndoHistory.h>

class gdeGameDefinition;
class gdeWPUndoHistoryListener;



/**
 * \brief Undo history panel.
 */
class gdeWPUndoHistory : public igdeWPUndoHistory{
private:
	gdeWPUndoHistoryListener *pListener;
	gdeGameDefinition *pGameDefinition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rig. */
	gdeWPUndoHistory( igdeEnvironment &environment );
	
	/** \brief Clean up rig. */
	virtual ~gdeWPUndoHistory();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Monitored game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition to monitor or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
