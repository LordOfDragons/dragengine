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

#ifndef _DECSTRINGLIST_H_
#define _DECSTRINGLIST_H_

#include "decString.h"


/**
 * \brief Ordered list of strings.
 * 
 * The strings in the list are allowed the be duplicates of each other but can not be NULL.
 */
class DE_DLL_EXPORT decStringList{
private:
	decString **pStrings;
	int pStringCount;
	int pStringSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new string list. */
	decStringList();
	
	/** \brief Create new string list as a copy of another string list. */
	decStringList(const decStringList &list);
	
	/** \brief Clean up string list. */
	~decStringList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of strings. */
	inline int GetCount() const{return pStringCount;}
	
	/** \brief String at the given index. */
	const decString &GetAt(int index) const;
	
	/** \brief Set string at the given index. */
	void SetAt(int index, const decString &string);
	void SetAt(int index, const char *string);
	
	/** \brief Index of the first occurance of a string or -1 if not found. */
	int IndexOf(const decString &string) const;
	int IndexOf(const char *string) const;
	
	/** \brief Given string is found one or more times in the list. */
	bool Has(const decString &string) const;
	bool Has(const char *string) const;
	
	/** \brief Adds a string. */
	void Add(const decString &string);
	void Add(const char *string);
	
	/** \brief Inserts a string. */
	void InsertAt(const decString &string, int index);
	void InsertAt(const char *string, int index);
	
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
	
	/** \brief Get sub range of string list. */
	decStringList Splice(int first) const;
	
	/** \brief Get sub range of string list. */
	decStringList Splice(int first, int last) const;
	
	/** \brief Entries joined to a single string. */
	decString Join(const decString &separator) const;
	
	/**
	 * \brief Entries joined to a single string.
	 * \throws deeInvalidParam \em separator is NULL.
	 */
	decString Join(const char *separator) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief String at the given index. */
	const decString &operator[](int index) const;
	
	/** \brief Set this list to the content of another list. */
	decStringList &operator=(const decStringList &list);
	
	/** \brief Determines if this list is equal to another list. */
	bool operator==(const decStringList &list) const;
	
	/** \brief Determines if this list is not equal to another list. */
	bool operator!=(const decStringList &list) const;
	
	/** \brief Retrieves a new list containing the concatenation of this list and another one. */
	decStringList operator+(const decStringList &list) const;
	
	/** \brief Appends another list to this list. */
	decStringList &operator+=(const decStringList &list);
	
	/** \brief Retrieves a new list containing this list with the given string added to it. */
	decStringList operator+(const decString &string) const;
	
	/** \brief Retrieves a new list containing this list with the given string added to it. */
	decStringList operator+(const char *string) const;
	
	/** \brief Appends a string to this list. */
	decStringList &operator+=(const decString &string);
	
	/** \brief Appends a string to this list. */
	decStringList &operator+=(const char *string);
	/*@}*/
	
	
	
private:
	void pSortAscending(int left, int right);
	void pSortDescending(int left, int right);
};

#endif
