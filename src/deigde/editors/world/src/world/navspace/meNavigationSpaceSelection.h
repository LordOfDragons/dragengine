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

#ifndef _MENAVIGATIONSPACESELECTION_H_
#define _MENAVIGATIONSPACESELECTION_H_

#include "meNavigationSpaceList.h"



/**
 * @brief Navigation Space Selection.
 */
class meNavigationSpaceSelection{
private:
	meNavigationSpaceList pSelection;
	meNavigationSpace *pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meNavigationSpaceSelection();
	/** Cleans up the list. */
	~meNavigationSpaceSelection();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of selected navigation spacess. */
	inline const meNavigationSpaceList &GetSelected() const{ return pSelection; }
	/** Adds a navigation spaces to the selection if not included already. */
	void Add( meNavigationSpace *navspaces );
	/** Removes a navigation spaces from the selection if included. */
	void Remove( meNavigationSpace *navspaces );
	/** Removes all navigation spacess from the selection. */
	void RemoveAll();
	
	/** Retrieves the active navigation spaces or NULL if there is none. */
	inline meNavigationSpace *GetActive() const{ return pActive; }
	/** Determines if there is an active navigation spaces. */
	bool HasActive() const;
	/** Sets the active navigation spaces or NULL if there is none. */
	void SetActive( meNavigationSpace *navspaces );
	/** Activate the first navigation spaces in the selection. */
	void ActivateNext();
	
	/** Resets the selection removing all navigation spacess and setting the active one to NULL. */
	void Reset();
};

#endif
