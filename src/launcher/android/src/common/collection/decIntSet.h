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

#ifndef _DECINTSET_H_
#define _DECINTSET_H_



/**
 * @brief Int Set.
 * Set of int values.
 */
class decIntSet{
private:
	int *pValues;
	int pValueCount;
	int pValueSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decIntSet();
	
	/**
	 * \brief Creates a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	decIntSet( int capacity );
	
	/** \brief Creates a copy of a set. */
	decIntSet( const decIntSet &set );
	
	/** \brief Clean up the set. */
	~decIntSet();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Number of values. */
	inline int GetCount() const{ return pValueCount; }
	
	/**
	 * \brief Value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	int GetAt( int index ) const;
	
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
	 * \brief Remove value.
	 * \throws deeInvalidParam \em value is absent from the set.
	 */
	void Remove( int value );
	
	/** \brief Remove value if present. */
	void RemoveIfPresent( int value );
	
	/** \brief Removes all values. */
	void RemoveAll();
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals( const decIntSet &set ) const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==( const decIntSet &set ) const;
	
	/** \brief New set with all values from this set followed by all values of another set. */
	decIntSet operator+( const decIntSet &set ) const;
	
	/**
	 * \brief Value at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	int operator[]( int position ) const;
	
	/** \brief Copy set to this set. */
	decIntSet &operator=( const decIntSet &set );
	
	/** \brief Append absent values of set to this set. */
	decIntSet &operator+=( const decIntSet &set );
	/*@}*/
	
private:
	int pIndexOf( int value ) const;
};

#endif
