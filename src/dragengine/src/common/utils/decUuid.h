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

#ifndef _DECUUID_H_
#define _DECUUID_H_

#include <stdint.h>
#include "../string/decString.h"


/**
 * \brief UUID.
 *
 * Universally unique identifier using randomness.
 */
class DE_DLL_EXPORT decUuid{
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
