/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECSTRINGLIST_H_
#define _DECSTRINGLIST_H_

#include "decString.h"



/**
 * \brief Ordered list of strings.
 * \details The strings in the list are allowed the be duplicates of each other but can not be NULL.
 */
class decStringList{
private:
	decString **pStrings;
	int pStringCount;
	int pStringSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new string list. */
	decStringList();
	/** \brief Creates a new string list as a copy of another string list. */
	decStringList( const decStringList &list );
	/** \brief Cleans up the string list. */
	~decStringList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of strings. */
	inline int GetCount() const{ return pStringCount; }
	/** \brief Retrieves the string at the given index. */
	const decString &GetAt( int index ) const;
	/** \brief Sets the string at the given index. */
	void SetAt( int index, const decString &string );
	void SetAt( int index, const char *string );
	/** \brief Retrieves the index of the first occurance of a string or -1 if not found. */
	int IndexOf( const decString &string ) const;
	int IndexOf( const char *string ) const;
	/** \brief Determines if the given string is found one or more times in the list. */
	bool Has( const decString &string ) const;
	bool Has( const char *string ) const;
	/** \brief Adds a string. */
	void Add( const decString &string );
	void Add( const char *string );
	/** \brief Inserts a string. */
	void InsertAt( const decString &string, int index );
	void InsertAt( const char *string, int index );
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
	
	/** \brief Entries joined to a single string. */
	decString Join( const decString &separator ) const;
	
	/**
	 * \brief Entries joined to a single string.
	 * \throws deeInvalidParam \em separator is \em NULL.
	 */
	decString Join( const char *separator ) const;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Retrieves the string at the given index. */
	const decString &operator[]( int index ) const;
	/** \brief Set this list to the content of another list. */
	decStringList &operator=( const decStringList &list );
	/** \brief Determines if this list is equal to another list. */
	bool operator==( const decStringList &list ) const;
	/** \brief Determines if this list is not equal to another list. */
	bool operator!=( const decStringList &list ) const;
	/** \brief Retrieves a new list containing the concatenation of this list and another one. */
	decStringList operator+( const decStringList &list ) const;
	/** \brief Appends another list to this list. */
	decStringList &operator+=( const decStringList &list );
	/** \brief Retrieves a new list containing this list with the given string added to it. */
	decStringList operator+( const decString &string ) const;
	/** \brief Retrieves a new list containing this list with the given string added to it. */
	decStringList operator+( const char *string ) const;
	/** \brief Appends a string to this list. */
	decStringList &operator+=( const decString &string );
	/** \brief Appends a string to this list. */
	decStringList &operator+=( const char *string );
	/*@}*/
	
private:
	void pSortAscending( int left, int right );
	void pSortDescending( int left, int right );
};

#endif
