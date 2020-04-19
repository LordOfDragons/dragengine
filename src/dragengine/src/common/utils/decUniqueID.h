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

#ifndef _DECUNIQUEID_H_
#define _DECUNIQUEID_H_

#include "../string/decString.h"


/**
 * \brief Unique ID.
 *
 * Unique id useful for identifying objects across the network or on persistent
 * storage. The ID has a variable bit count which has to be a multiple of 8.
 * Therefore you do not have to worry about id overflow or using too many data
 * for few ids. The id can be incremented. This is useful if a counter id is
 * kept that is used to initialize new ids with. Unique ids are stored as an
 * array of bytes. You can retrieve and alter the bytes individually.
 */
class decUniqueID{
private:
	unsigned char *pBytes;
	int pByteCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create unique id with value 0. */
	decUniqueID();
	
	/** \brief Create unique id from integer value. */
	decUniqueID( int value );
	
	/** \brief Create unique id from hex string. */
	decUniqueID( const char *string );
	
	/** \brief Create copy of unique id. */
	decUniqueID( const decUniqueID &id );
	
	/** \brief Clean up unique id. */
	~decUniqueID();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bit count. */
	inline int GetBitCount() const{ return pByteCount << 3; }
	
	/**
	 * \brief Set bit count.
	 * 
	 * ID is reset to 0 if bit count changes.
	 */
	void SetBitCount( int bitCount );
	
	/** \brief Byte at the position in LSB. */
	int GetByteAt( int position ) const;
	
	/** \brief Set byte at position in LSB. */
	void SetByteAt( int position, int value );
	
	/**
	 * \brief Increment ID by 1.
	 * 
	 * Increases bit count by 8 if overflowing.
	 */
	void Increment();
	
	/**
	 * \brief Increment ID by positive amount.
	 * 
	 * Increases bit count by multiples of 8 if overflowing.
	 */
	void IncrementBy( int amount );
	
	/**
	 * \brief Increment ID by another ID.
	 * 
	 * Increases bit count by multiples of 8 if overflowing.
	 */
	void IncrementBy( const decUniqueID &id );
	
	/** \brief Decrement ID by 1. */
	void Decrement();
	
	/** \brief Decrement ID by positive amount. */
	void DecrementBy( int amount );
	
	/** \brief Decrement ID by another ID. */
	void DecrementBy( const decUniqueID &id );
	
	/** \brief Reset id to 0 and bit count to 8. */
	void Reset();
	
	/** \brief Convert to hex encoded string. */
	decString ToHexString() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set ID. */
	decUniqueID &operator=( const decUniqueID &id );
	
	/** \brief IDs are equal. */
	bool operator==( const decUniqueID &id ) const;
	
	/** \brief IDs are not equal. */
	bool operator!=( const decUniqueID &id ) const;
	
	/** \brief ID is less than another ID. */
	bool operator<( const decUniqueID &id ) const;
	
	/** \brief ID is less than or equal to another ID. */
	bool operator<=( const decUniqueID &id ) const;
	
	/** \brief ID is greater than another ID. */
	bool operator>( const decUniqueID &id ) const;
	
	/** \brief ID is greater than or equal to another ID. */
	bool operator>=( const decUniqueID &id ) const;
	/*@}*/
};

#endif
