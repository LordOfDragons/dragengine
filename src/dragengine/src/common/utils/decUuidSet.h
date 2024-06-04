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

#ifndef _DECUUIDSET_H_
#define _DECUUIDSET_H_

#include "decUuid.h"


/**
 * \brief Unordered set of uuids.
 * 
 * The uuids in the set are not allowed to be duplicates of each other and can not be NULL.
 * The set is ordered.
 * 
 */
class DE_DLL_EXPORT decUuidSet{
private:
	decUuid *pUuids;
	int pUuidCount;
	int pUuidSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new uuid set. */
	decUuidSet();
	
	/** \brief Create new uuid set as a copy of another uuid list. */
	decUuidSet( const decUuidSet &set );
	
	/** \brief Clean up uuid set. */
	~decUuidSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of uuids. */
	inline int GetCount() const{ return pUuidCount; }
	
	/** \brief Uuid at the given index. */
	const decUuid &GetAt( int index ) const;
	
	/** \brief Index of the uuid or -1 if not found. */
	int IndexOf( const decUuid &uuid ) const;
	
	/** \brief Given uuid is part of the set. */
	bool Has( const decUuid &uuid ) const;
	
	/** \brief Adds a uuid if not already part of the set. */
	void Add( const decUuid &uuid );
	
	/** \brief Removes the uuid if part of the set. */
	void Remove( const decUuid &uuid );
	
	/** \brief Removes all uuids. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Uuid at the given index. */
	const decUuid &operator[]( int index ) const;
	
	/** \brief Set this list to the content of another set. */
	decUuidSet &operator=( const decUuidSet &set );
	
	/** \brief Determines if this set is equal to another set. */
	bool operator==( const decUuidSet &set ) const;
	
	/** \brief Determines if this set is not equal to another set. */
	bool operator!=( const decUuidSet &set ) const;
	
	/** \brief Retrieves a new list containing the concatenation of this set and another one. */
	decUuidSet operator+( const decUuidSet &set ) const;
	
	/** \brief Appends another list to this set. */
	decUuidSet &operator+=( const decUuidSet &set );
	
	/** \brief Retrieves a new list containing this set with the given uuid added to it. */
	decUuidSet operator+( const decUuid &uuid ) const;
	
	/** \brief Appends a uuid to this set. */
	decUuidSet &operator+=( const decUuid &uuid );
	/*@}*/
};

#endif
