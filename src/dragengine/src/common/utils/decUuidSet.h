/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECUUIDSET_H_
#define _DECUUIDSET_H_

#include "decUuid.h"


/**
 * \brief Unordered set of uuids.
 * 
 * The uuids in the set are not allowed to be duplicates of each other and can not be NULL.
 * The set is ordered.
 * 
 */
class decUuidSet{
private:
	decUuid *pUuids;
	int pUuidCount;
	int pUuidSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new uuid set. */
	decUuidSet();
	
	/** \brief Create new uuid set as a copy of another uuid list. */
	decUuidSet( const decUuidSet &set );
	
	/** \brief Clean up uuid set. */
	~decUuidSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of uuids. */
	inline int GetCount() const{ return pUuidCount; }
	
	/** \brief Uuid at the given index. */
	const decUuid &GetAt( int index ) const;
	
	/** \brief Index of the uuid or -1 if not found. */
	int IndexOf( const decUuid &uuid ) const;
	
	/** \brief Given uuid is part of the set. */
	bool Has( const decUuid &uuid ) const;
	
	/** \brief Adds a uuid if not already part of the set. */
	void Add( const decUuid &uuid );
	
	/** \brief Removes the uuid if part of the set. */
	void Remove( const decUuid &uuid );
	
	/** \brief Removes all uuids. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Uuid at the given index. */
	const decUuid &operator[]( int index ) const;
	
	/** \brief Set this list to the content of another set. */
	decUuidSet &operator=( const decUuidSet &set );
	
	/** \brief Determines if this set is equal to another set. */
	bool operator==( const decUuidSet &set ) const;
	
	/** \brief Determines if this set is not equal to another set. */
	bool operator!=( const decUuidSet &set ) const;
	
	/** \brief Retrieves a new list containing the concatenation of this set and another one. */
	decUuidSet operator+( const decUuidSet &set ) const;
	
	/** \brief Appends another list to this set. */
	decUuidSet &operator+=( const decUuidSet &set );
	
	/** \brief Retrieves a new list containing this set with the given uuid added to it. */
	decUuidSet operator+( const decUuid &uuid ) const;
	
	/** \brief Appends a uuid to this set. */
	decUuidSet &operator+=( const decUuid &uuid );
	/*@}*/
};

#endif
