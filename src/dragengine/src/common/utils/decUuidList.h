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

#ifndef _DECUUIDLIST_H_
#define _DECUUIDLIST_H_

#include "decUuid.h"


/**
 * \brief Ordered list of uuids.
 * 
 * The uuids in the list are allowed the be duplicates of each other but can not be NULL.
 */
class DE_DLL_EXPORT decUuidList{
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
	decUuidList(const decUuidList &list);
	
	/** \brief Clean up uuid list. */
	~decUuidList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of uuids. */
	inline int GetCount() const{return pUuidCount;}
	
	/** \brief Uuid at the given index. */
	const decUuid &GetAt(int index) const;
	
	/** \brief Set uuid at the given index. */
	void SetAt(int index, const decUuid &uuid);
	
	/** \brief Index of the first occurance of a uuid or -1 if not found. */
	int IndexOf(const decUuid &uuid) const;
	
	/** \brief Given uuid is found one or more times in the list. */
	bool Has(const decUuid &uuid) const;
	
	/** \brief Adds a uuid. */
	void Add(const decUuid &uuid);
	
	/** \brief Inserts a uuid. */
	void InsertAt(const decUuid &uuid, int index);
	
	/** \brief Moves a uuid to a new position. */
	void Move(int from, int to);
	
	/** \brief Removes the uuid from with the given index. */
	void RemoveFrom(int index);
	
	/** \brief Removes all uuids. */
	void RemoveAll();
	
	/** \brief Get sub range of uuid list. */
	decUuidList Splice(int first) const;
	
	/** \brief Get sub range of uuid list. */
	decUuidList Splice(int first, int last) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Uuid at the given index. */
	const decUuid &operator[](int index) const;
	
	/** \brief Set this list to the content of another list. */
	decUuidList &operator=(const decUuidList &list);
	
	/** \brief Determines if this list is equal to another list. */
	bool operator==(const decUuidList &list) const;
	
	/** \brief Determines if this list is not equal to another list. */
	bool operator!=(const decUuidList &list) const;
	
	/** \brief Retrieves a new list containing the concatenation of this list and another one. */
	decUuidList operator+(const decUuidList &list) const;
	
	/** \brief Appends another list to this list. */
	decUuidList &operator+=(const decUuidList &list);
	
	/** \brief Retrieves a new list containing this list with the given uuid added to it. */
	decUuidList operator+(const decUuid &uuid) const;
	
	/** \brief Appends a uuid to this list. */
	decUuidList &operator+=(const decUuid &uuid);
	/*@}*/
};

#endif
