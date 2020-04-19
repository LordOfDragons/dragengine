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

#ifndef _MEWINDOWCHANGELOG_H_
#define _MEWINDOWCHANGELOG_H_

#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/igdeIconListBoxReference.h>

class meWindowMain;
class meWindowChangelogListener;
class meWCEntry;
class meWorld;



/**
 * \brief Changelog Window.
 */
class meWindowChangelog : public igdeContainerBorder{
private:
	meWindowMain &pWindowMain;
	meWindowChangelogListener *pListener;
	
	meWorld *pWorld;
	
	igdeIconListBoxReference pListChanges;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	meWindowChangelog( meWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~meWindowChangelog();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief Update changelog list. */
	void UpdateChangelog();
	
	/** \brief Save entry. */
	void SaveEntry( meWCEntry *entry );
	
	/** \brief Save all entries. */
	void SaveAllEntries();
	/*@}*/
};

#endif
