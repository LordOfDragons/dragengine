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

#ifndef _DECTHREADSAFEOBJECTORDEREDSET_H_
#define _DECTHREADSAFEOBJECTORDEREDSET_H_

#include "../../dragengine_export.h"

class deThreadSafeObject;


/**
 * \brief Ordered set of objects.
 * 
 * All objects including NULL are allowed. Objects can be included
 * only once in the set.
 */
class DE_DLL_EXPORT decThreadSafeObjectOrderedSet{
private:
	deThreadSafeObject **pObjects;
	int pObjectCount;
	int pObjectSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decThreadSafeObjectOrderedSet();
	
	/**
	 * \brief Create a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decThreadSafeObjectOrderedSet( int capacity );
	
	/** \brief Create copy of a set. */
	decThreadSafeObjectOrderedSet( const decThreadSafeObjectOrderedSet &set );
	
	/** \brief Clean up the set. */
	~decThreadSafeObjectOrderedSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of objects. */
	inline int GetCount() const{ return pObjectCount; }
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	deThreadSafeObject *GetAt( int index ) const;
	
	/** \brief Index of the first occurance of an object or -1 if not found. */
	int IndexOf( deThreadSafeObject *object ) const;
	
	/** \brief Determine if object exists in the list. */
	bool Has( deThreadSafeObject *object ) const;
	
	/**
	 * \brief Set object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt( int index, deThreadSafeObject *object );
	
	/**
	 * \brief Add object.
	 * \throws deeInvalidParam \em object is present in the set.
	 */
	void Add( deThreadSafeObject *object );
	
	/** \brief Add object if absent from the set. */
	void AddIfAbsent( deThreadSafeObject *object );
	
	/**
	 * \brief Insert object.
	 * \throws deeInvalidParam \em object is present in the set.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( deThreadSafeObject *object, int index );
	
	/**
	 * \brief Move object.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move( deThreadSafeObject *object, int to );
	
	/**
	 * \brief Remove object.
	 * \throws deeInvalidParam \em object is is absent from the set.
	 */
	void Remove( deThreadSafeObject *object );
	
	/** \brief Remove object if present in the set. */
	void RemoveIfPresent( deThreadSafeObject *object );
	
	/**
	 * \brief Remove object from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom( int index );
	
	/** \brief Remove all objects. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals( const decThreadSafeObjectOrderedSet &set ) const;
	
	/**
	 * \brief New set with the objects from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decThreadSafeObjectOrderedSet GetHead( int count ) const;
	
	/**
	 * \brief Set set to objects from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decThreadSafeObjectOrderedSet &set, int count ) const;
	
	/**
	 * \brief New set with objects from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decThreadSafeObjectOrderedSet GetTail( int count ) const;
	
	/**
	 * \brief Set set to objects from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decThreadSafeObjectOrderedSet &set, int count ) const;
	
	/**
	 * \brief New set with objects from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decThreadSafeObjectOrderedSet GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set set to objects from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decThreadSafeObjectOrderedSet &set, int from, int to ) const;
	
	/**
	 * \brief New set with objects from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decThreadSafeObjectOrderedSet GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set set to objects from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decThreadSafeObjectOrderedSet &set, int from, int to, int step ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==( const decThreadSafeObjectOrderedSet &set ) const;
	
	/** \brief New set containing all objects of this set followed by all objects of another set. */
	decThreadSafeObjectOrderedSet operator+( const decThreadSafeObjectOrderedSet &set ) const;
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	deThreadSafeObject *operator[]( int index ) const;
	
	/** \brief Copy set to this set. */
	decThreadSafeObjectOrderedSet &operator=( const decThreadSafeObjectOrderedSet &set );
	
	/** \brief Append objects of set to this set. */
	decThreadSafeObjectOrderedSet &operator+=( const decThreadSafeObjectOrderedSet &set );
	/*@}*/
};

#endif
