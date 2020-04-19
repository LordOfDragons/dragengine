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

#ifndef _MEOBJECTSELECTION_H_
#define _MEOBJECTSELECTION_H_

#include "meObjectList.h"



/**
 * @brief Object Selection.
 */
class meObjectSelection{
private:
	meObjectList pSelection;
	meObject *pActive;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectSelection();
	/** Cleans up the list. */
	~meObjectSelection();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the list of selected objects. */
	inline const meObjectList &GetSelected() const{ return pSelection; }
	/** Adds a objects to the selection if not included already. */
	void Add( meObject *object );
	/** Removes a objects from the selection if included. */
	void Remove( meObject *object );
	/** Removes all objects from the selection. */
	void RemoveAll();
	
	/** Retrieves the active objects or NULL if there is none. */
	inline meObject *GetActive() const{ return pActive; }
	/** Determines if there is an active objects. */
	bool HasActive() const;
	/** Sets the active objects or NULL if there is none. */
	void SetActive( meObject *object );
	/** Activate the first objects in the selection. */
	void ActivateNext();
	
	/** Resets the selection removing all objects and setting the active one to NULL. */
	void Reset();
};

#endif
