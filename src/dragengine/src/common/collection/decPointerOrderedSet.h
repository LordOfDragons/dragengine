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

#ifndef _DECPOINTERORDEREDSET_H_
#define _DECPOINTERORDEREDSET_H_


/**
 * \brief Ordered set of pointers.
 * 
 * All pointers including NULL are allowed. Pointers can be included
 * only once in the set.
 */
class decPointerOrderedSet{
private:
	void **pPointers;
	int pPointerCount;
	int pPointerSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decPointerOrderedSet();
	
	/**
	 * \brief Create a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decPointerOrderedSet( int capacity );
	
	/** \brief Create copy of a set. */
	decPointerOrderedSet( const decPointerOrderedSet &set );
	
	/** \brief Clean up the set. */
	~decPointerOrderedSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of pointers. */
	inline int GetCount() const{ return pPointerCount; }
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *GetAt( int index ) const;
	
	/** \brief Index of the first occurance of an pointer or -1 if not found. */
	int IndexOf( void *pointer ) const;
	
	/** \brief Determine if pointer exists in the list. */
	bool Has( void *pointer ) const;
	
	/**
	 * \brief Add pointer.
	 * \throws deeInvalidParam \em pointer is present in the set.
	 */
	void Add( void *pointer );
	
	/** \brief Add pointer if absent from the set. */
	void AddIfAbsent( void *pointer );
	
	/**
	 * \brief Insert pointer.
	 * \throws deeInvalidParam \em pointer is present in the set.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( void *pointer, int index );
	
	/**
	 * \brief Move pointer.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move( void *pointer, int to );
	
	/**
	 * \brief Remove pointer.
	 * \throws deeInvalidParam \em pointer is is absent from the set.
	 */
	void Remove( void *pointer );
	
	/** \brief Remove pointer if present in the set. */
	void RemoveIfPresent( void *pointer );
	
	/**
	 * \brief Remove pointer from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom( int index );
	
	/** \brief Remove all pointers. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals( const decPointerOrderedSet &set ) const;
	
	/**
	 * \brief New set with the pointers from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerOrderedSet GetHead( int count ) const;
	
	/**
	 * \brief Set set to pointers from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decPointerOrderedSet &set, int count ) const;
	
	/**
	 * \brief New set with pointers from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerOrderedSet GetTail( int count ) const;
	
	/**
	 * \brief Set set to pointers from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decPointerOrderedSet &set, int count ) const;
	
	/**
	 * \brief New set with pointers from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decPointerOrderedSet GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set set to pointers from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decPointerOrderedSet &set, int from, int to ) const;
	
	/**
	 * \brief New set with pointers from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decPointerOrderedSet GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set set to pointers from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decPointerOrderedSet &set, int from, int to, int step ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==( const decPointerOrderedSet &set ) const;
	
	/** \brief New set containing all pointers of this set followed by all pointers of another set. */
	decPointerOrderedSet operator+( const decPointerOrderedSet &set ) const;
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *operator[]( int index ) const;
	
	/** \brief Copy set to this set. */
	decPointerOrderedSet &operator=( const decPointerOrderedSet &set );
	
	/** \brief Append pointers of set to this set. */
	decPointerOrderedSet &operator+=( const decPointerOrderedSet &set );
	/*@}*/
};

#endif
