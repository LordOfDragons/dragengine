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

#ifndef _DECUNICODESTRINGSET_H_
#define _DECUNICODESTRINGSET_H_

#include "decUnicodeString.h"


/**
 * \brief Unordered set of unicode strings.
 * 
 * The strings in the set are not allowed the be duplicates of each other and can not be NULL.
 */
class DE_DLL_EXPORT decUnicodeStringSet{
private:
	decUnicodeString **pStrings;
	int pStringCount;
	int pStringSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new string set. */
	decUnicodeStringSet();
	
	/** \brief Create new string set as a copy of another string list. */
	decUnicodeStringSet( const decUnicodeStringSet &set );
	
	/** \brief Clean up string set. */
	~decUnicodeStringSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of strings. */
	inline int GetCount() const{ return pStringCount; }
	
	/** \brief String at the given index. */
	const decUnicodeString &GetAt( int index ) const;
	
	/** \brief Index of the string or -1 if not found. */
	int IndexOf( const decUnicodeString &string ) const;
	
	/** \brief Given string is part of the set. */
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
	/** \brief String at the given index. */
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
