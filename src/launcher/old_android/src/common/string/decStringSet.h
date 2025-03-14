/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DECSTRINGSET_H_
#define _DECSTRINGSET_H_

#include "decString.h"



/**
 * \brief Unordered set of strings.
 * 
 * The strings in the set are not allowed to be duplicates of each other and can not be \em NULL.
 * The set is ordered.
 * 
 */
class decStringSet{
private:
	decString **pStrings;
	int pStringCount;
	int pStringSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new string set. */
	decStringSet();
	/** \brief Creates a new string set as a copy of another string list. */
	decStringSet( const decStringSet &set );
	/** \brief Cleans up the string set. */
	~decStringSet();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of strings. */
	inline int GetCount() const{ return pStringCount; }
	/** \brief Retrieves the string at the given index. */
	const decString &GetAt( int index ) const;
	/** \brief Retrieves the index of the string or -1 if not found. */
	int IndexOf( const decString &string ) const;
	int IndexOf( const char *string ) const;
	/** \brief Determines if the given string is part of the set. */
	bool Has( const decString &string ) const;
	bool Has( const char *string ) const;
	/** \brief Adds a string if not already part of the set. */
	void Add( const decString &string );
	void Add( const char *string );
	/** \brief Removes the string if part of the set. */
	void Remove( const decString &string );
	void Remove( const char *string );
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
	const decString &operator[]( int index ) const;
	/** \brief Set this list to the content of another set. */
	decStringSet &operator=( const decStringSet &set );
	/** \brief Determines if this set is equal to another set. */
	bool operator==( const decStringSet &set ) const;
	/** \brief Determines if this set is not equal to another set. */
	bool operator!=( const decStringSet &set ) const;
	/** \brief Retrieves a new list containing the concatenation of this set and another one. */
	decStringSet operator+( const decStringSet &set ) const;
	/** \brief Appends another list to this set. */
	decStringSet &operator+=( const decStringSet &set );
	/** \brief Retrieves a new list containing this set with the given string added to it. */
	decStringSet operator+( const decString &string ) const;
	/** \brief Retrieves a new list containing this set with the given string added to it. */
	decStringSet operator+( const char *string ) const;
	/** \brief Appends a string to this set. */
	decStringSet &operator+=( const decString &string );
	/** \brief Appends a string to this set. */
	decStringSet &operator+=( const char *string );
	/*@}*/
	
private:
	void pSortAscending( int left, int right );
	void pSortDescending( int left, int right );
};

#endif
