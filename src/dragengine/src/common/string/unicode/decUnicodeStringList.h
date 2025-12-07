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
	decUnicodeStringList(const decUnicodeStringList &list);
	
	/** \brief Clean up string list. */
	~decUnicodeStringList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of strings. */
	inline int GetCount() const{return pStringCount;}
	
	/** \brief String at the given index. */
	const decUnicodeString &GetAt(int index) const;
	
	/** \brief Set string at the given index. */
	void SetAt(int index, const decUnicodeString &string);
	
	/** \brief Index of the first occurance of a string or -1 if not found. */
	int IndexOf(const decUnicodeString &string) const;
	
	/** \brief Given string is found one or more times in the list. */
	bool Has(const decUnicodeString &string) const;
	
	/** \brief Adds a string. */
	void Add(const decUnicodeString &string);
	
	/** \brief Inserts a string. */
	void InsertAt(const decUnicodeString &string, int index);
	
	/** \brief Moves a string to a new position. */
	void Move(int from, int to);
	
	/** \brief Removes the string from with the given index. */
	void RemoveFrom(int index);
	
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
	const decUnicodeString &operator[](int index) const;
	
	/** \brief Set this list to the content of another list. */
	decUnicodeStringList &operator=(const decUnicodeStringList &list);
	
	/** \brief Determines if this list is equal to another list. */
	bool operator==(const decUnicodeStringList &list);
	
	/** \brief Determines if this list is not equal to another list. */
	bool operator!=(const decUnicodeStringList &list);
	
	/** \brief Retrieves a new list containing the concatenation of this list and another one. */
	decUnicodeStringList operator+(const decUnicodeStringList &list) const;
	
	/** \brief Appends another list to this list. */
	decUnicodeStringList &operator+=(const decUnicodeStringList &list);
	
	/** \brief Retrieves a new list containing this list with the given string added to it. */
	decUnicodeStringList operator+(const decUnicodeString &string) const;
	
	/** \brief Appends a string to this list. */
	decUnicodeStringList &operator+=(const decUnicodeString &string);
	/*@}*/
	
	
	
private:
	void pSortAscending(int left, int right);
	void pSortDescending(int left, int right);
};

#endif
