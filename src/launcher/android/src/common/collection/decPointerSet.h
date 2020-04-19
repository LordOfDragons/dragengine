/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECPOINTERSET_H_
#define _DECPOINTERSET_H_



/**
 * \brief Set of pointers.
 * 
 * All pointers including \em NULL are allowed and they can occure only once in the set.
 */
class decPointerSet{
private:
	void **pPointers;
	int pPointerCount;
	int pPointerSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decPointerSet();
	
	/**
	 * \brief Create a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decPointerSet( int capacity );
	
	/** \brief Create copy of a set. */
	decPointerSet( const decPointerSet &set );
	
	/** \brief Clean up the set. */
	~decPointerSet();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Number of objects. */
	inline int GetCount() const{ return pPointerCount; }
	
	/**
	 * \brief Pointer at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *GetAt( int position ) const;
	
	/** \brief Index of the first occurance of a pointer or -1 if not found. */
	int IndexOf( void *pointer ) const;
	
	/** \brief Determine if pointer exists in the list. */
	bool Has( void *pointer ) const;
	
	/**
	 * \brief Add pointer.
	 * \throws deeInvalidParam \em object is present in the set.
	 */
	void Add( void *pointer );
	
	/** \brief Add pointer if absent from the set. */
	void AddIfAbsent( void *pointer );
	
	/**
	 * \brief Remove pointer from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom( int position );
	
	/**
	 * \brief Remove pointer.
	 * \throws deeInvalidParam \em object is is absent from the set.
	 */
	void Remove( void *pointer );
	
	/** \brief Remove pointer if present in the set. */
	void RemoveIfPresent( void *pointer );
	
	/** \brief Remove all pointers. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals( const decPointerSet &set ) const;
	
	/**
	 * \brief New set with the values from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerSet GetHead( int count ) const;
	
	/**
	 * \brief Set set to values from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decPointerSet &set, int count ) const;
	
	/**
	 * \brief New set with values from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decPointerSet GetTail( int count ) const;
	
	/**
	 * \brief Set set to values from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decPointerSet &set, int count ) const;
	
	/**
	 * \brief New set with values from the middle of this set.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decPointerSet GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set set to values from the middle of this set.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decPointerSet &set, int from, int to ) const;
	
	/**
	 * \brief New set with values from the middle of this set using a step size.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decPointerSet GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set set to values from the middle of this set using a step size.
	 * \details \em from and \em to are clamped to the last index in the set if larger.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decPointerSet &set, int from, int to, int step ) const;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==( const decPointerSet &set ) const;
	
	/** \brief New set containing all objects of this set followed by all objects of another set. */
	decPointerSet operator+( const decPointerSet &set ) const;
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void *operator[]( int position ) const;
	
	/** \brief Copy set to this set. */
	decPointerSet &operator=( const decPointerSet &set );
	
	/** \brief Append objects of set to this set. */
	decPointerSet &operator+=( const decPointerSet &set );
	/*@}*/
};

#endif
