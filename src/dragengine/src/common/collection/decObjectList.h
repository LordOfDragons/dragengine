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

#ifndef _DECOBJECTLIST_H_
#define _DECOBJECTLIST_H_

#include "../../dragengine_export.h"

class deObject;


/**
 * \brief List of objects.
 * 
 * All objects including NULL are allowed. Objects can be included
 * multiple times in the list.
 */
class DE_DLL_EXPORT decObjectList{
private:
	deObject **pObjects;
	int pObjectCount;
	int pObjectSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new list. */
	decObjectList();
	
	/**
	 * \brief Create a new list with an initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decObjectList( int capacity );
	
	/** \brief Create a copy of a list. */
	decObjectList( const decObjectList &list );
	
	/** \brief Clean up the list. */
	~decObjectList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of objects. */
	inline int GetCount() const{ return pObjectCount; }
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	deObject *GetAt( int index ) const;
	
	/**
	 * \brief Set object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt( int index, deObject *object );
	
	/** \brief Index of the first occurance of an object or -1 if not found. */
	int IndexOf( deObject *object ) const;
	
	/**
	 * \brief Index of the first occurance of an object or -1 if not found.
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount()-1.
	 */
	int IndexOf( deObject *object, int start ) const;
	
	/** \brief Determine if object exists in the list. */
	bool Has( deObject *object ) const;
	
	/** \brief Number of times object exists in the list. */
	int CountOccurance( deObject *object ) const;
	
	/** \brief Add object. */
	void Add( deObject *object );
	
	/**
	 * \brief Insert object.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( deObject *object, int index );
	
	/**
	 * \brief Move object.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move( int from, int to );
	
	/**
	 * \brief Remove object from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom( int index );
	
	/** \brief Remove all objects. */
	void RemoveAll();
	
	/** \brief Determine if this list is equal to another list. */
	bool Equals( const decObjectList &list ) const;
	
	/**
	 * \brief New list with the values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decObjectList GetHead( int count ) const;
	
	/**
	 * \brief Set list to values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decObjectList &list, int count ) const;
	
	/**
	 * \brief New list with values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decObjectList GetTail( int count ) const;
	
	/**
	 * \brief Set list to values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decObjectList &list, int count ) const;
	
	/**
	 * \brief New list with values from the middle of this list.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decObjectList GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set list to values from the middle of this list.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decObjectList &list, int from, int to ) const;
	
	/**
	 * \brief New list with values from the middle of this list using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decObjectList GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set list to values from the middle of this list using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decObjectList &list, int from, int to, int step ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this list is equal to another list. */
	bool operator==( const decObjectList &list ) const;
	
	/** \brief New list containing all values of this list followed by all values of another list. */
	decObjectList operator+( const decObjectList &list ) const;
	
	/**
	 * \brief Object at index.
	 * 
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	deObject *operator[]( int position ) const;
	
	/** \brief Copy list to this list. */
	decObjectList &operator=( const decObjectList &list );
	
	/** \brief Append values of list to this list. */
	decObjectList &operator+=( const decObjectList &list );
	/*@}*/
};

#endif
