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

#ifndef _MEPATHFINDTESTTYPELIST_H_
#define _MEPATHFINDTESTTYPELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class mePathFindTestType;



/**
 * @brief Path Find Test Type List.
 */
class mePathFindTestTypeList{
private:
	decObjectOrderedSet pTypes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	mePathFindTestTypeList();
	/** Cleans up the list. */
	~mePathFindTestTypeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of types. */
	int GetCount() const;
	/** Retrieves the type at the given position. */
	mePathFindTestType *GetAt( int position ) const;
	/** Retrieves the type with the given type value or NULL if not found. */
	mePathFindTestType *GetWith( int typeNumber ) const;
	/** Retrieves the index of the given type or -1 if not found. */
	int IndexOf( mePathFindTestType *type ) const;
	/** Retrieves the index of the type with the given type number or -1 if not found. */
	int IndexWith( int typeNumber ) const;
	/** Determines if the given type exists. */
	bool Has( mePathFindTestType *type ) const;
	/** Determines if a type with the given type number exists. */
	bool HasWith( int typeNumber ) const;
	/** Adds an type throwing an exception if already existing. */
	void Add( mePathFindTestType *type );
	/** Adds an type if not existing already. */
	void AddIfAbsent( mePathFindTestType *type );
	/** Removes an type throwing an exception if not existing. */
	void Remove( mePathFindTestType *type );
	/** Removes an type if existing. */
	void RemoveIfPresent( mePathFindTestType *type );
	/** Removes all types. */
	void RemoveAll();
	/*@}*/
};

#endif
