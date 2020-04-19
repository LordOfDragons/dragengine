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

#ifndef _DECUUIDLIST_H_
#define _DECUUIDLIST_H_

#include "decUuid.h"


/**
 * \brief Ordered list of uuids.
 * 
 * The uuids in the list are allowed the be duplicates of each other but can not be NULL.
 */
class decUuidList{
private:
	decUuid *pUuids;
	int pUuidCount;
	int pUuidSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new uuid list. */
	decUuidList();
	
	/** \brief Create new uuid list as a copy of another uuid list. */
	decUuidList( const decUuidList &list );
	
	/** \brief Clean up uuid list. */
	~decUuidList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of uuids. */
	inline int GetCount() const{ return pUuidCount; }
	
	/** \brief Uuid at the given index. */
	const decUuid &GetAt( int index ) const;
	
	/** \brief Set uuid at the given index. */
	void SetAt( int index, const decUuid &uuid );
	
	/** \brief Index of the first occurance of a uuid or -1 if not found. */
	int IndexOf( const decUuid &uuid ) const;
	
	/** \brief Given uuid is found one or more times in the list. */
	bool Has( const decUuid &uuid ) const;
	
	/** \brief Adds a uuid. */
	void Add( const decUuid &uuid );
	
	/** \brief Inserts a uuid. */
	void InsertAt( const decUuid &uuid, int index );
	
	/** \brief Moves a uuid to a new position. */
	void Move( int from, int to );
	
	/** \brief Removes the uuid from with the given index. */
	void RemoveFrom( int index );
	
	/** \brief Removes all uuids. */
	void RemoveAll();
	
	/** \brief Get sub range of uuid list. */
	decUuidList Splice( int first ) const;
	
	/** \brief Get sub range of uuid list. */
	decUuidList Splice( int first, int last ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Uuid at the given index. */
	const decUuid &operator[]( int index ) const;
	
	/** \brief Set this list to the content of another list. */
	decUuidList &operator=( const decUuidList &list );
	
	/** \brief Determines if this list is equal to another list. */
	bool operator==( const decUuidList &list ) const;
	
	/** \brief Determines if this list is not equal to another list. */
	bool operator!=( const decUuidList &list ) const;
	
	/** \brief Retrieves a new list containing the concatenation of this list and another one. */
	decUuidList operator+( const decUuidList &list ) const;
	
	/** \brief Appends another list to this list. */
	decUuidList &operator+=( const decUuidList &list );
	
	/** \brief Retrieves a new list containing this list with the given uuid added to it. */
	decUuidList operator+( const decUuid &uuid ) const;
	
	/** \brief Appends a uuid to this list. */
	decUuidList &operator+=( const decUuid &uuid );
	/*@}*/
};

#endif
