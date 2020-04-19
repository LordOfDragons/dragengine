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

#ifndef _MEDECALLIST_H_
#define _MEDECALLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class meDecal;



/**
 * @brief Object List.
 */
class meDecalList{
private:
	decObjectOrderedSet pObjects;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meDecalList();
	/** Cleans up the list. */
	~meDecalList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of decals. */
	int GetCount() const;
	/** Retrieves the decal at the given position. */
	meDecal *GetAt( int position ) const;
	/** Retrieves the index of the given decal or -1 if not found. */
	int IndexOf( meDecal *decal ) const;
	/** Determines if the given decal exists. */
	bool Has( meDecal *decal ) const;
	/** Adds an decal throwing an exception if already existing. */
	void Add( meDecal *decal );
	/** Adds an decal if not existing already. */
	void AddIfAbsent( meDecal *decal );
	/** Removes an decal throwing an exception if not existing. */
	void Remove( meDecal *decal );
	/** Removes an decal if existing. */
	void RemoveIfPresent( meDecal *decal );
	/** Removes all decals. */
	void RemoveAll();
	
	/** Sets the list to the contain the same decals as another list. */
	meDecalList &operator=( const meDecalList &list );
	/*@}*/
};

#endif
