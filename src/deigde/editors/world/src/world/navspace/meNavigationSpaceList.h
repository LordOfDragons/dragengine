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

#ifndef _MENAVIGATIONSPACELIST_H_
#define _MENAVIGATIONSPACELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class meNavigationSpace;



/**
 * @brief Navigation Space List.
 */
class meNavigationSpaceList{
private:
	decObjectOrderedSet pNavSpaces;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meNavigationSpaceList();
	/** Cleans up the list. */
	~meNavigationSpaceList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of navigation spaces. */
	int GetCount() const;
	/** Retrieves the navigation space at the given position. */
	meNavigationSpace *GetAt( int position ) const;
	/** Retrieves the index of the given navigation space or -1 if not found. */
	int IndexOf( meNavigationSpace *navspace ) const;
	/** Determines if the given navigation space exists. */
	bool Has( meNavigationSpace *navspace );
	/** Adds an navigation space throwing an exception if already existing. */
	void Add( meNavigationSpace *navspace );
	/** Adds an navigation space if not existing already. */
	void AddIfAbsent( meNavigationSpace *navspace );
	/** Removes an navigation space throwing an exception if not existing. */
	void Remove( meNavigationSpace *navspace );
	/** Removes an navigation space if existing. */
	void RemoveIfPresent( meNavigationSpace *navspace );
	/** Removes all navigation spaces. */
	void RemoveAll();
	/*@}*/
};

#endif
