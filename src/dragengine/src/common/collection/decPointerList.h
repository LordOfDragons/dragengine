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

#ifndef _DECPOINTERLIST_H_
#define _DECPOINTERLIST_H_

#include "../../dragengine_export.h"


/**
 * \brief List of pointers.
 * 
 * All pointers including NULL are allowed. Pointers can be included
 * multiple times in the list.
 */
class DE_DLL_EXPORT decPointerList{
private:
	void **pPointers;
	int pPointerCount;
	int pPointerSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new list. */
	decPointerList();
	
	/**
	 * \brief Create a new list with an initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decPointerList( int capacity );
	
	/** \brief Create a copy of a list. */
	decPointerList( const decPointerList &list );
	
	/** \brief Clean up the list. */
	~decPointerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of objects. */
	inline int GetCount() const{ return pPointerCount; }
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *GetAt( int position ) const;
	
	/**
	 * \brief Set pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt( int position, void *pointer );
	
	/** \brief Index of the first occurance of a pointer or -1 if not found. */
	int IndexOf( void *pointer ) const;
	
	/**
	 * \brief Index of the first occurance of a pointer or -1 if not found.
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount()-1.
	 */
	int IndexOf( void *pointer, int start ) const;
	
	/** \brief Determine if pointer exists in the list. */
	bool Has( void *pointer ) const;
	
	/** \brief Number of times pointer exists in the list. */
	int CountOccurance( void *pointer ) const;
	
	/** \brief Add pointer. */
	void Add( void *pointer );
	
	/**
	 * \brief Insert pointer.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( void *pointer, int index );
	
	/**
	 * \brief Move pointer.
	 * \throws deeInvalidParam \em pointer is not part of the list.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Move( void *pointer, int index );
	
	/**
	 * \brief Move pointer.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount()-1.
	 */
	void Move( int from, int to );
	
	/**
	 * \brief Remove pointer from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom( int position );
	
	/** \brief Remove all pointers. */
	void RemoveAll();
	
	/** \brief Determine if this list is equal to another list. */
	bool Equals( const decPointerList &list ) const;
	
	/**
	 * \brief New list with the values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerList GetHead( int count ) const;
	
	/**
	 * \brief Set list to values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decPointerList &list, int count ) const;
	
	/**
	 * \brief New list with values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerList GetTail( int count ) const;
	
	/**
	 * \brief Set list to values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decPointerList &list, int count ) const;
	
	/**
	 * \brief New list with values from the middle of this list.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decPointerList GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set list to values from the middle of this list.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decPointerList &list, int from, int to ) const;
	
	/**
	 * \brief New list with values from the middle of this list using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decPointerList GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set list to values from the middle of this list using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decPointerList &list, int from, int to, int step ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this list is equal to another list. */
	bool operator==( const decPointerList &list ) const;
	
	/** \brief New list containing all values of this list followed by all values of another list. */
	decPointerList operator+( const decPointerList &list ) const;
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *operator[]( int position ) const;
	
	/**
	 * \brief Set pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *&operator[]( int position );
	
	/** \brief Copy list to this list. */
	decPointerList &operator=( const decPointerList &list );
	
	/** \brief Append values of list to this list. */
	decPointerList &operator+=( const decPointerList &list );
	/*@}*/
};

#endif
