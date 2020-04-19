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

#ifndef _MEOBJECTLIST_H_
#define _MEOBJECTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class meObject;



/**
 * @brief Object List.
 */
class meObjectList{
private:
	decObjectOrderedSet pObjects;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectList();
	/** Cleans up the list. */
	~meObjectList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of objects. */
	int GetCount() const;
	/** Retrieves the object at the given position. */
	meObject *GetAt( int position ) const;
	/** Retrieves the index of the given object or -1 if not found. */
	int IndexOf( meObject *object ) const;
	/** Determines if the given object exists. */
	bool Has( meObject *object ) const;
	/** Adds an object throwing an exception if already existing. */
	void Add( meObject *object );
	/** Adds an object if not existing already. */
	void AddIfAbsent( meObject *object );
	/** Removes an object throwing an exception if not existing. */
	void Remove( meObject *object );
	/** Removes an object if existing. */
	void RemoveIfPresent( meObject *object );
	/** Removes all objects. */
	void RemoveAll();
	
	/** Sets the list to the contain the same objects as another list. */
	meObjectList &operator=( const meObjectList &list );
	/*@}*/
};

#endif
