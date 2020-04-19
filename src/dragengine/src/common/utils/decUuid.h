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

#ifndef _DECUUID_H_
#define _DECUUID_H_

#include <stdint.h>
#include "../string/decString.h"


/**
 * \brief UUID.
 *
 * Universally unique identifier using randomness.
 */
class decUuid{
private:
	uint8_t pValues[ 16 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create 0 uuid. */
	decUuid();
	
	/** \brief Create uuid from integer value. */
	decUuid( const uint8_t values[ 16 ] );
	
	/**
	 * \brief Create uuid from hex string.
	 * 
	 * If \em condensed is true the string has to contain 16 bytes of hex encoded values.
	 * If \em condensed is false the string has to contain packets of 16 hex encoded values
	 * separated by dashes. The form is "4-2-2-2-6" which indicates the number of hex
	 * encoded bytes in each packet.
	 */
	decUuid( const char *string, bool condensed );
	
	/** \brief Create copy of uuid. */
	decUuid( const decUuid &id );
	
	/** \brief Clean up uuid. */
	~decUuid();
	
	/** \brief Create random uuid. */
	static decUuid Random();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Value at index. */
	uint8_t GetValueAt( int index ) const;
	
	/** \brief Set value at index. */
	void SetValueAt( int index, uint8_t value );
	
	/**
	 * \brief Convert to hex encoded string.
	 * 
	 * If \em condensed is true all bytes are output as hex encoded values in a single row
	 * without dashes in between. If \em condensed is false the value is written in the
	 * form "4-2-2-2-6" which indicates the number of hex encoded bytes in each packet.
	 */
	decString ToHexString( bool condensed ) const;
	
	/**
	 * \brief Set from hex encoded string.
	 * 
	 * If \em condensed is true the string has to contain 16 bytes of hex encoded values.
	 * If \em condensed is false the string has to contain packets of 16 hex encoded values
	 * separated by dashes. The form is "4-2-2-2-6" which indicates the number of hex
	 * encoded bytes in each packet.
	 */
	void SetFromHexString( const char *string, bool condensed );
	
	/** Set to empty. */
	void Clear();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Assign. */
	bool operator=( const decUuid &id );
	
	/** \brief UUIDs are equal. */
	bool operator==( const decUuid &id ) const;
	
	/** \brief UUIDs are not equal. */
	bool operator!=( const decUuid &id ) const;
	
	/** \brief UUID is 0. */
	bool operator!() const;
	
	/** \brief UUID is not null 0. */
	operator bool() const;
	/*@}*/
};

#endif
