/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECOBJECTORDEREDSET_H_
#define _DECOBJECTORDEREDSET_H_

class deObject;


/**
 * \brief Ordered set of objects.
 * 
 * All objects including NULL are allowed. Objects can be included
 * only once in the set.
 */
class decObjectOrderedSet{
private:
	deObject **pObjects;
	int pObjectCount;
	int pObjectSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decObjectOrderedSet();
	
	/**
	 * \brief Create a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decObjectOrderedSet( int capacity );
	
	/** \brief Create copy of a set. */
	decObjectOrderedSet( const decObjectOrderedSet &set );
	
	/** \brief Clean up the set. */
	~decObjectOrderedSet();
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
	
	/** \brief Index of the first occurance of an object or -1 if not found. */
	int IndexOf( deObject *object ) const;
	
	/** \brief Determine if object exists in the list. */
	bool Has( deObject *object ) const;
	
	/**
	 * \brief Set object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt( int index, deObject *object );
	
	/**
	 * \brief Add object.
	 * \throws deeInvalidParam \em object is present in the set.
	 */
	void Add( deObject *object );
	
	/** \brief Add object if absent from the set. */
	void AddIfAbsent( deObject *object );
	
	/**
	 * \brief Insert object.
	 * \throws deeInvalidParam \em object is present in the set.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( deObject *object, int index );
	
	/**
	 * \brief Move object.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move( deObject *object, int to );
	
	/**
	 * \brief Remove object.
	 * \throws deeInvalidParam \em object is is absent from the set.
	 */
	void Remove( deObject *object );
	
	/** \brief Remove object if present in the set. */
	void RemoveIfPresent( deObject *object );
	
	/**
	 * \brief Remove object from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom( int index );
	
	/** \brief Remove all objects. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals( const decObjectOrderedSet &set ) const;
	
	/**
	 * \brief New set with the objects from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decObjectOrderedSet GetHead( int count ) const;
	
	/**
	 * \brief Set set to objects from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decObjectOrderedSet &set, int count ) const;
	
	/**
	 * \brief New set with objects from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decObjectOrderedSet GetTail( int count ) const;
	
	/**
	 * \brief Set set to objects from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decObjectOrderedSet &set, int count ) const;
	
	/**
	 * \brief New set with objects from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decObjectOrderedSet GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set set to objects from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decObjectOrderedSet &set, int from, int to ) const;
	
	/**
	 * \brief New set with objects from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decObjectOrderedSet GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set set to objects from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decObjectOrderedSet &set, int from, int to, int step ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==( const decObjectOrderedSet &set ) const;
	
	/** \brief New set containing all objects of this set followed by all objects of another set. */
	decObjectOrderedSet operator+( const decObjectOrderedSet &set ) const;
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	deObject *operator[]( int index ) const;
	
	/** \brief Copy set to this set. */
	decObjectOrderedSet &operator=( const decObjectOrderedSet &set );
	
	/** \brief Append objects of set to this set. */
	decObjectOrderedSet &operator+=( const decObjectOrderedSet &set );
	/*@}*/
};

#endif
