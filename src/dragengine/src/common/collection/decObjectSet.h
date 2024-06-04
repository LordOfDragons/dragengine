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

#ifndef _DECOBJECTSET_H_
#define _DECOBJECTSET_H_

#include "../../dragengine_export.h"

class deObject;


/**
 * \brief Set of objects.
 * 
 * All objects including NULL are allowed and they can occure only once in the set.
 */
class DE_DLL_EXPORT decObjectSet{
private:
	deObject **pObjects;
	int pObjectCount;
	int pObjectSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decObjectSet();
	
	/**
	 * \brief Create a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decObjectSet( int capacity );
	
	/** \brief Create copy of a set. */
	decObjectSet( const decObjectSet &set );
	
	/** \brief Clean up the set. */
	~decObjectSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of objects. */
	inline int GetCount() const{ return pObjectCount; }
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	deObject *GetAt( int index ) const;
	
	/** \brief Determine if object exists in the list. */
	bool Has( deObject *object ) const;
	
	/**
	 * \brief Add object.
	 * \throws deeInvalidParam \em object is present in the set.
	 */
	void Add( deObject *object );
	
	/** \brief Add object if absent from the set. */
	void AddIfAbsent( deObject *object );
	
	/**
	 * \brief Remove object.
	 * \throws deeInvalidParam \em object is is absent from the set.
	 */
	void Remove( deObject *object );
	
	/** \brief Remove object if present in the set. */
	void RemoveIfPresent( deObject *object );
	
	/** \brief Remove all objects. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals( const decObjectSet &set ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==( const decObjectSet &set ) const;
	
	/** \brief New set containing all objects of this set followed by all objects of another set. */
	decObjectSet operator+( const decObjectSet &set ) const;
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	deObject *operator[]( int position ) const;
	
	/** \brief Copy set to this set. */
	decObjectSet &operator=( const decObjectSet &set );
	
	/** \brief Append objects of set to this set. */
	decObjectSet &operator+=( const decObjectSet &set );
	/*@}*/
	
private:
	int pIndexOf( deObject *object ) const;
};

#endif
