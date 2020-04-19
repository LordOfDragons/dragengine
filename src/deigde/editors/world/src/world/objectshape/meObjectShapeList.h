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

#ifndef _MEOBJECTSHAPELIST_H_
#define _MEOBJECTSHAPELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>

class meObjectShape;
class decShapeList;



/**
 * @brief Object Shape List.
 */
class meObjectShapeList{
private:
	decObjectOrderedSet pObjectShapes;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectShapeList();
	/** Cleans up the list. */
	~meObjectShapeList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of object shapes. */
	int GetCount() const;
	/** Retrieves the object shape at the given position. */
	meObjectShape *GetAt( int position ) const;
	/** Retrieves the index of the given object shape or -1 if not found. */
	int IndexOf( meObjectShape *objectShape ) const;
	/** Determines if the given object shape exists. */
	bool Has( meObjectShape *objectShape ) const;
	/** Adds an object shape throwing an exception if already existing. */
	void Add( meObjectShape *objectShape );
	/** Adds an object shape if not existing already. */
	void AddIfAbsent( meObjectShape *objectShape );
	/** Removes an object shape throwing an exception if not existing. */
	void Remove( meObjectShape *objectShape );
	/** Removes an object shape if existing. */
	void RemoveIfPresent( meObjectShape *objectShape );
	/** Removes all object shapes. */
	void RemoveAll();
	
	/** Creates a shape list from this list. */
	void CreateShapeList( decShapeList &list ) const;
	/** Create property string from shape list. */
	void CreatePropertyString( decString &string ) const;
	
	/** Sets the list to the contain the same object shapes as another list. */
	meObjectShapeList &operator=( const meObjectShapeList &list );
	/*@}*/
};

#endif
