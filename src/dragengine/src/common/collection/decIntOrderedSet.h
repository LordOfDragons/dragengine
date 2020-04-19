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

#ifndef _DECINTORDEREDSET_H_
#define _DECINTORDEREDSET_H_


/**
 * \brief Ordered set of integer values. Each value can exists only once.
 */
class decIntOrderedSet{
private:
	int *pValues;
	int pValueCount;
	int pValueSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decIntOrderedSet();
	
	/**
	 * \brief Create new set with initial capacity.
	 * 
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decIntOrderedSet( int capacity );
	
	/** \brief Create copy of a set. */
	decIntOrderedSet( const decIntOrderedSet &set );
	
	/** \brief Clean up the set. */
	~decIntOrderedSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of values. */
	inline int GetCount() const{ return pValueCount; }
	
	/**
	 * \brief Value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	int GetAt( int index ) const;
	
	/** \brief Index of the a value or -1 if not found. */
	int IndexOf( int value ) const;
	
	/** \brief Determine if a value exists. */
	bool Has( int value ) const;
	
	/**
	 * \brief Add value.
	 * \throws deeInvalidParam \em value is contained in the set.
	 */
	void Add( int value );
	
	/** \brief Add value if not existing. */
	void AddIfAbsent( int value );
	
	/**
	 * \brief Insert value.
	 * \throws deeInvalidParam \em value is contained in the set.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert( int value, int index );
	
	/**
	 * \brief Move value.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move( int value, int to );
	
	/**
	 * \brief Remove value.
	 * \throws deeInvalidParam \em value is absent from the set.
	 */
	void Remove( int value );
	
	/** \brief Remove value if present. */
	void RemoveIfPresent( int value );
	
	/** \brief Removes all values. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals( const decIntOrderedSet &set ) const;
	
	/**
	 * \brief New set with values from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decIntOrderedSet GetHead( int count ) const;
	
	/**
	 * \brief Set set to values from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead( decIntOrderedSet &list, int count ) const;
	
	/**
	 * \brief New set with values from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decIntOrderedSet GetTail( int count ) const;
	
	/**
	 * \brief Set set to values from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail( decIntOrderedSet &list, int count ) const;
	
	/**
	 * \brief New set with values from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decIntOrderedSet GetMiddle( int from, int to ) const;
	
	/**
	 * \brief Set set to values from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle( decIntOrderedSet &list, int from, int to ) const;
	
	/**
	 * \brief New set with values from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decIntOrderedSet GetSliced( int from, int to, int step ) const;
	
	/**
	 * \brief Set set to values from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced( decIntOrderedSet &list, int from, int to, int step ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==( const decIntOrderedSet &set ) const;
	
	/** \brief New set with all values from this set followed by all values of another set. */
	decIntOrderedSet operator+( const decIntOrderedSet &set ) const;
	
	/**
	 * \brief Value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	int operator[]( int index ) const;
	
	/** \brief Copy set to this set. */
	decIntOrderedSet &operator=( const decIntOrderedSet &set );
	
	/** \brief Append absent values of set to this set. */
	decIntOrderedSet &operator+=( const decIntOrderedSet &set );
	/*@}*/
};

#endif
