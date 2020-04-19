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

#ifndef _LPELANGPACKENTRYSELECTION_H_
#define _LPELANGPACKENTRYSELECTION_H_

#include "lpeLangPackEntryList.h"



/**
 * \brief Language pack entry selection.
 */
class lpeLangPackEntrySelection{
private:
	lpeLangPackEntryList pSelection;
	lpeLangPackEntry *pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new list. */
	lpeLangPackEntrySelection();
	/** \brief Cleans up the list. */
	~lpeLangPackEntrySelection();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the list of selected entries. */
	inline const lpeLangPackEntryList &GetSelected() const{ return pSelection; }
	/** \brief Adds a entries to the selection if not included already. */
	void Add( lpeLangPackEntry *entry );
	/** \brief Removes a entries from the selection if included. */
	void Remove( lpeLangPackEntry *entry );
	/** \brief Removes all entries from the selection. */
	void RemoveAll();
	
	/** \brief Retrieves the active entries or NULL if there is none. */
	inline lpeLangPackEntry *GetActive() const{ return pActive; }
	/** \brief Determines if there is an active entries. */
	bool HasActive() const;
	/** \brief Sets the active entries or NULL if there is none. */
	void SetActive( lpeLangPackEntry *entry );
	/** \brief Activate the first entries in the selection. */
	void ActivateNext();
	
	/** \brief Resets the selection removing all entries and setting the active one to NULL. */
	void Reset();
};

#endif
