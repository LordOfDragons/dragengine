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

#ifndef _DECLAYERMASK_H_
#define _DECLAYERMASK_H_

#include <stdint.h>

class decString;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Simple mask to organize objects into layers.
 * 
 * Objects can be located on multiple layers depending on the bits set.
 * A maximum of 64 layers can be used.
 */
class decLayerMask{
private:
	uint64_t pMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new layer mask. */
	decLayerMask();
	
	/** \brief Create new layer mask as a copy of another layer mask. */
	decLayerMask( const decLayerMask &layerMask );
	
	/** \brief Cleans up layer mask. */
	~decLayerMask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Clears the mask. */
	void ClearMask();
	
	/** \brief Sets all bits in the mask. */
	void FillMask();
	
	/** \brief Sets a bit in the range from 0 to 63 inclusive. */
	void SetBit( int bit );
	
	/** \brief Clears a bit in the range from 0 to 63 inclusive. */
	void ClearBit( int bit );
	
	/** \brief Determines if a bit is set in the range from 0 to 63 inclusive. */
	bool IsBitSet( int bit ) const;
	
	/** \brief Determines if a bit is not set in the range from 0 to 63 inclusive. */
	bool IsBitCleared( int bit ) const;
	
	/** \brief Layer mask has no bits set. */
	bool IsEmpty() const;
	
	/** \brief Layer mask has at least one bit set. */
	bool IsNotEmpty() const;
	
	/** \brief Determines if this mask shares at least one bit with another mask. */
	bool Matches( const decLayerMask &layerMask ) const;
	
	/** \brief Determines if this mask shares no bits with another mask. */
	bool MatchesNot( const decLayerMask &layerMask ) const;
	
	/**
	 * \brief Retrieves a bit string representation.
	 * 
	 * String contains 1 for set bits and 0 for cleared bits. The first bit is located
	 * on the front of the string with bits following up to the bit 63.
	 */
	decString ToBitString() const;
	
	/**
	 * \brief Set layer mask from a bit string representation.
	 * 
	 * String contains 1 for set bits and 0 for cleared bits. The first bit is located
	 * on the front of the string with bits following up to the bit 63. If the string
	 * is less than 64 characters long all bits outside the range are set to cleared.
	 */
	void SetFromBitString( const char *string );
	
	/**
	 * \brief Retrieves a hexadecimal string representation.
	 * 
	 * String contains groups of 4 bits converted to readable hexadecimal in the form
	 * 0 to f. The first 4 bits are located in the hex digit at the front of the string
	 * with each group of 4 bits following up to bit 63. Inside each hex digit the first
	 * bit in the group is located at the high bit with each following bit progressing
	 * down to the lowest bit.
	 */
	decString ToHexString() const;
	
	/**
	 * \brief Set layer mask from a hexadecimal string representation.
	 * 
	 * String contains groups of 4 bits converted to readable hexadecimal in the form
	 * 0 to f. The first 4 bits are located in the hex digit at the front of the string
	 * with each group of 4 bits following up to bit 63. Inside each hex digit the first
	 * bit in the group is located at the high bit with each following bit progressing
	 * down to the lowest bit. If the string is less than 16 characters long all bits
	 * outside the range are set to cleared.
	 */
	void SetFromHexString( const char *string );
	
	/** \brief Read layer mask from file reader. */
	static decLayerMask ReadFromFile( decBaseFileReader &reader );
	
	/** \brief Write layer mask to file writer. */
	void WriteToFile( decBaseFileWriter &writer ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Layer masks are equal. */
	bool operator==( const decLayerMask &layerMask ) const;
	
	/** \brief Layer masks  are not equal. */
	bool operator!=( const decLayerMask &layerMask ) const;
	
	/** \brief Sets this layer mask to another one. */
	decLayerMask &operator=( const decLayerMask &layerMask );
	
	/** \brief Sets all bits that are set in another one. */
	decLayerMask &operator|=( const decLayerMask &layerMask );
	
	/** \brief Clears all bits that are not set in another one. */
	decLayerMask &operator&=( const decLayerMask &layerMask );
	
	/** \brief Sets bits that are different between this layer mask and another one. */
	decLayerMask &operator^=( const decLayerMask &layerMask );
	
	/** \brief Retrieves a layermask with all bits set that are set in either this layer mask or another one. */
	decLayerMask operator|( const decLayerMask &layerMask ) const;
	
	/** \brief Retrieves a layermask with all bits set that are set in both this layer mask and another one. */
	decLayerMask operator&( const decLayerMask &layerMask ) const;
	
	/** \brief Retrieves a layermask with all bits set that are different between this layer mask and another one. */
	decLayerMask operator^( const decLayerMask &layerMask ) const;
	
	/** \brief Inverse layermask with all bits inverted. */
	decLayerMask operator~() const;
	/*@}*/
};

#endif
