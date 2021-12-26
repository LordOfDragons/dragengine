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

#ifndef _DECUNICODESTRINGLIST_H_
#define _DECUNICODESTRINGLIST_H_

#include "decUnicodeString.h"


/**
 * \brief Ordered list of unicode strings.
 * 
 * The unicode strings in the list are allowed the be duplicates of each other but can not be NULL.
 */
class DE_DLL_EXPORT decUnicodeStringList{
private:
	decUnicodeString **pStrings;
	int pStringCount;
	int pStringSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief \brief Creates a new string list. */
	decUnicodeStringList();
	
	/** \brief Create new string list as a copy of another string list. */
	decUnicodeStringList( const decUnicodeStringList &list );
	
	/** \brief Clean up string list. */
	~decUnicodeStringList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of strings. */
	inline int GetCount() const{ return pStringCount; }
	
	/** \brief String at the given index. */
	const decUnicodeString &GetAt( int index ) const;
	
	/** \brief Set string at the given index. */
	void SetAt( int index, const decUnicodeString &string );
	
	/** \brief Index of the first occurance of a string or -1 if not found. */
	int IndexOf( const decUnicodeString &string ) const;
	
	/** \brief Given string is found one or more times in the list. */
	bool Has( const decUnicodeString &string ) const;
	
	/** \brief Adds a string. */
	void Add( const decUnicodeString &string );
	
	/** \brief Inserts a string. */
	void InsertAt( const decUnicodeString &string, int index );
	
	/** \brief Moves a string to a new position. */
	void Move( int from, int to );
	
	/** \brief Removes the string from with the given index. */
	void RemoveFrom( int index );
	
	/** \brief Removes all strings. */
	void RemoveAll();
	
	/** \brief Sorts strings in ascending order. */
	void SortAscending();
	
	/** \brief Sorts strings in descending order. */
	void SortDescending();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief String at the given index. */
	const decUnicodeString &operator[]( int index ) const;
	
	/** \brief Set this list to the content of another list. */
	decUnicodeStringList &operator=( const decUnicodeStringList &list );
	
	/** \brief Determines if this list is equal to another list. */
	bool operator==( const decUnicodeStringList &list );
	
	/** \brief Determines if this list is not equal to another list. */
	bool operator!=( const decUnicodeStringList &list );
	
	/** \brief Retrieves a new list containing the concatenation of this list and another one. */
	decUnicodeStringList operator+( const decUnicodeStringList &list ) const;
	
	/** \brief Appends another list to this list. */
	decUnicodeStringList &operator+=( const decUnicodeStringList &list );
	
	/** \brief Retrieves a new list containing this list with the given string added to it. */
	decUnicodeStringList operator+( const decUnicodeString &string ) const;
	
	/** \brief Appends a string to this list. */
	decUnicodeStringList &operator+=( const decUnicodeString &string );
	/*@}*/
	
	
	
private:
	void pSortAscending( int left, int right );
	void pSortDescending( int left, int right );
};

#endif
