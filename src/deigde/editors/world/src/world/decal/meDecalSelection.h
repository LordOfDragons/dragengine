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

#ifndef _MEDECALSELECTION_H_
#define _MEDECALSELECTION_H_

#include "meDecalList.h"



/**
 * @brief Decal Selection.
 */
class meDecalSelection{
private:
	meDecalList pSelection;
	meDecal *pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meDecalSelection();
	/** Cleans up the list. */
	~meDecalSelection();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of selected decals. */
	inline const meDecalList &GetSelected() const{ return pSelection; }
	/** Adds a decals to the selection if not included already. */
	void Add( meDecal *decal );
	/** Removes a decals from the selection if included. */
	void Remove( meDecal *decal );
	/** Removes all decals from the selection. */
	void RemoveAll();
	
	/** Retrieves the active decals or NULL if there is none. */
	inline meDecal *GetActive() const{ return pActive; }
	/** Determines if there is an active decals. */
	bool HasActive() const;
	/** Sets the active decals or NULL if there is none. */
	void SetActive( meDecal *decal );
	/** Activate the first decals in the selection. */
	void ActivateNext();
	
	/** Resets the selection removing all decals and setting the active one to NULL. */
	void Reset();
};

#endif
