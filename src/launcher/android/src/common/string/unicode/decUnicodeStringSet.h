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

#ifndef _DECUNICODESTRINGSET_H_
#define _DECUNICODESTRINGSET_H_

#include "decUnicodeString.h"



/**
 * \brief Unordered set of unicode strings.
 * \details The strings in the set are not allowed the be duplicates of each other and can not be NULL.
 */
class decUnicodeStringSet{
private:
	decUnicodeString **pStrings;
	int pStringCount;
	int pStringSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new string set. */
	decUnicodeStringSet();
	/** \brief Creates a new string set as a copy of another string list. */
	decUnicodeStringSet( const decUnicodeStringSet &set );
	/** \brief Cleans up the string set. */
	~decUnicodeStringSet();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of strings. */
	inline int GetCount() const{ return pStringCount; }
	/** \brief Retrieves the string at the given index. */
	const decUnicodeString &GetAt( int index ) const;
	/** \brief Retrieves the index of the string or -1 if not found. */
	int IndexOf( const decUnicodeString &string ) const;
	/** \brief Determines if the given string is part of the set. */
	bool Has( const decUnicodeString &string ) const;
	/** \brief Adds a string if not already part of the set. */
	void Add( const decUnicodeString &string );
	/** \brief Removes the string if part of the set. */
	void Remove( const decUnicodeString &string );
	/** \brief Removes all strings. */
	void RemoveAll();
	/** \brief Sorts strings in ascending order. */
	void SortAscending();
	/** \brief Sorts strings in descending order. */
	void SortDescending();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Retrieves the string at the given index. */
	const decUnicodeString &operator[]( int index ) const;
	/** \brief Set this list to the content of another set. */
	decUnicodeStringSet &operator=( const decUnicodeStringSet &set );
	/** \brief Determines if this set is equal to another set. */
	bool operator==( const decUnicodeStringSet &set );
	/** \brief Determines if this set is not equal to another set. */
	bool operator!=( const decUnicodeStringSet &set );
	/** \brief Retrieves a new list containing the concatenation of this set and another one. */
	decUnicodeStringSet operator+( const decUnicodeStringSet &set ) const;
	/** \brief Appends another list to this set. */
	decUnicodeStringSet &operator+=( const decUnicodeStringSet &set );
	/** \brief Retrieves a new list containing this set with the given string added to it. */
	decUnicodeStringSet operator+( const decUnicodeString &string ) const;
	/** \brief Appends a string to this set. */
	decUnicodeStringSet &operator+=( const decUnicodeString &string );
	/*@}*/
	
private:
	void pSortAscending( int left, int right );
	void pSortDescending( int left, int right );
};

#endif
