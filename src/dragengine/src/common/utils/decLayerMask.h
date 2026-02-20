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

#ifndef _DECLAYERMASK_H_
#define _DECLAYERMASK_H_

#include <stdint.h>

#include "../../dragengine_export.h"

class decString;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Simple mask to organize objects into layers.
 * 
 * Objects can be located on multiple layers depending on the bits set.
 * A maximum of 64 layers can be used.
 */
class DE_DLL_EXPORT decLayerMask{
private:
	uint64_t pMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new layer mask. */
	decLayerMask();
	
	/** \brief Create new layer mask as a copy of another layer mask. */
	decLayerMask(const decLayerMask &layerMask);
	
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
	void SetBit(int bit);
	
	/** \brief Clears a bit in the range from 0 to 63 inclusive. */
	void ClearBit(int bit);
	
	/** \brief Determines if a bit is set in the range from 0 to 63 inclusive. */
	bool IsBitSet(int bit) const;
	
	/** \brief Determines if a bit is not set in the range from 0 to 63 inclusive. */
	bool IsBitCleared(int bit) const;
	
	/** \brief Layer mask has no bits set. */
	bool IsEmpty() const;
	
	/** \brief Layer mask has at least one bit set. */
	bool IsNotEmpty() const;
	
	/** \brief Layer mask has all bits set. */
	bool IsFull() const;
	
	/** \brief Determines if this mask shares at least one bit with another mask. */
	bool Matches(const decLayerMask &layerMask) const;
	
	/** \brief Determines if this mask shares no bits with another mask. */
	bool MatchesNot(const decLayerMask &layerMask) const;
	
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
	void SetFromBitString(const char *string);
	
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
	void SetFromHexString(const char *string);
	
	/** \brief Read layer mask from file reader. */
	static decLayerMask ReadFromFile(decBaseFileReader &reader);
	
	/** \brief Write layer mask to file writer. */
	void WriteToFile(decBaseFileWriter &writer) const;
	
	/** \brief Mask value. */
	inline uint64_t GetMask() const{ return pMask; }
	
	/** \brief Set mask value. */
	void SetMask(uint64_t mask);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Layer masks are equal. */
	bool operator==(const decLayerMask &layerMask) const;
	
	/** \brief Layer masks  are not equal. */
	bool operator!=(const decLayerMask &layerMask) const;
	
	/** \brief Sets this layer mask to another one. */
	decLayerMask &operator=(const decLayerMask &layerMask);
	
	/** \brief Sets all bits that are set in another one. */
	decLayerMask &operator|=(const decLayerMask &layerMask);
	
	/** \brief Clears all bits that are not set in another one. */
	decLayerMask &operator&=(const decLayerMask &layerMask);
	
	/** \brief Sets bits that are different between this layer mask and another one. */
	decLayerMask &operator^=(const decLayerMask &layerMask);
	
	/** \brief Retrieves a layermask with all bits set that are set in either this layer mask or another one. */
	decLayerMask operator|(const decLayerMask &layerMask) const;
	
	/** \brief Retrieves a layermask with all bits set that are set in both this layer mask and another one. */
	decLayerMask operator&(const decLayerMask &layerMask) const;
	
	/** \brief Retrieves a layermask with all bits set that are different between this layer mask and another one. */
	decLayerMask operator^(const decLayerMask &layerMask) const;
	
	/** \brief Inverse layermask with all bits inverted. */
	decLayerMask operator~() const;
	/*@}*/
};

#endif
