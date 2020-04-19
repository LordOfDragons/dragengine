/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEETYPELIST_H_
#define _PEETYPELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class peeType;



/**
 * \brief Emitter Type List.
 */
class peeTypeList{
private:
	decObjectOrderedSet pTypes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new type list. */
	peeTypeList();
	/** \brief Clean up the type list. */
	~peeTypeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of types. */
	int GetCount() const;
	/** Retrieves the type at the given position. */
	peeType *GetAt( int index ) const;
	/** Retrieves the type with the given name or NULL if not found. */
	peeType *GetNamed( const char *name ) const;
	/** Retrieves the index of the given type or -1 if not found. */
	int IndexOf( peeType *type ) const;
	/** Retrieves the index of the type with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a type exists. */
	bool Has( peeType *type ) const;
	/** Determines if a type with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a type. */
	void Add( peeType *type );
	/** Inserts a new type. */
	void InsertAt( peeType *type, int index );
	/** Moves a type to a new position. */
	void MoveTo( peeType *type, int index );
	/** Removes a type. */
	void Remove( peeType *type );
	/** Removes all types. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	peeTypeList &operator=( const peeTypeList &list );
	/*@}*/
};

#endif
