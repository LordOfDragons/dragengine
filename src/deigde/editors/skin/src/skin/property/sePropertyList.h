/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEPROPERTYLIST_H_
#define _SEPROPERTYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seProperty;



/**
 * @brief Property List.
 */
class sePropertyList{
private:
	decObjectOrderedSet pProperties;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new property list. */
	sePropertyList();
	/** Cleans up the property list. */
	~sePropertyList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of properties. */
	int GetCount() const;
	/** Retrieves the property at the given position. */
	seProperty *GetAt( int index ) const;
	/** Retrieves the property with the given name or NULL if not found. */
	seProperty *GetNamed( const char *name ) const;
	/** Retrieves the index of the given property or -1 if not found. */
	int IndexOf( seProperty *property ) const;
	/** Retrieves the index of the property with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a property exists. */
	bool Has( seProperty *property ) const;
	/** Determines if a property with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a property. */
	void Add( seProperty *property );
	/** Inserts a new property. */
	void InsertAt( seProperty *property, int index );
	/** Moves a property to a new position. */
	void MoveTo( seProperty *property, int index );
	/** Removes a property. */
	void Remove( seProperty *property );
	/** Removes all properties. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	sePropertyList &operator=( const sePropertyList &list );
	/*@}*/
};

#endif
