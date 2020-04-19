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

#ifndef _DECOBJECTSET_H_
#define _DECOBJECTSET_H_

class deObject;


/**
 * \brief Set of objects.
 * 
 * All objects including NULL are allowed and they can occure only once in the set.
 */
class decObjectSet{
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
