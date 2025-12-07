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

#ifndef _DEOGLDYNAMICTBOBLOCK_H_
#define _DEOGLDYNAMICTBOBLOCK_H_

#include "../tbo/deoglDynamicTBO.h"
#include <dragengine/deObject.h>
class deoglDynamicTBOShared;


/**
 * Dynamic TBO block. Blocks can be either in use or empty as well as valid or invalid.
 * A block is valid if the data it contains has been copied into the TBO and is ready
 * to be used. If invalid the block data has first to be written to the TBO before it
 * can be used. If the block is not empty a data buffer is provided. In this case the
 * owner has to fill the data into the buffer and invalidate the block using
 * SetValid(false). The offset and size of the block are measured in data units.
 */
class deoglDynamicTBOBlock : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglDynamicTBOBlock> Ref;
	
	
	deoglDynamicTBOShared *pSharedTBO;
	int pOffset;
	int pSize;
	deoglDynamicTBO::Ref pData;
	deoglDynamicTBO::Ref pData2;
	bool pEmpty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create empty dynamic tbo block. */
	deoglDynamicTBOBlock(deoglDynamicTBOShared *tbo);
	
	/** Create empty dynamic tbo block. */
	deoglDynamicTBOBlock(deoglDynamicTBOShared *tbo, int offset, int size);
	
	/** Clean up dynamic tbo block. */
	virtual ~deoglDynamicTBOBlock();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Shared TBO containing block or NULL. */
	inline deoglDynamicTBOShared *GetSharedTBO() const{ return pSharedTBO; }
	
	/** Drop shared TBO. */
	void DropSharedTBO();
	
	
	
	/**
	 * Offset to block measured in units.
	 * \warning Can potentially change during block lifetime.
	 */
	inline int GetOffset() const{ return pOffset; }
	
	/** Set offset to block measured in units. */
	void SetOffset(int offset);
	
	/** Size of block in units. */
	inline int GetSize() const{ return pSize; }
	
	/** Set size of block in units. Also sets data TBO to NULL. */
	void SetSize(int size);
	
	/** TBO containing the data to use for the block or NULL if empty. */
	inline const deoglDynamicTBO::Ref &GetData() const{ return pData; }
	inline const deoglDynamicTBO::Ref &GetData2() const{ return pData2; }
	
	/** Set TBO containing the data to use for the block or NULL if empty. */
	void SetData(deoglDynamicTBO *tbo, deoglDynamicTBO *tbo2 = NULL);
	
	/** Block is empty. */
	inline bool GetEmpty() const{ return pEmpty; }
	
	/** Set if block is empty. */
	void SetEmpty(bool empty);
	
	/**
	 * Drop block removing it from owner shared TBO returning it to free memory pool.
	 * This call is only valid for non-empty blocks. For empty blocks nothing is done.
	 */
	void Drop();
	
	/** Update shared TBO data with TBO data. Call this after altering data in TBO. */
	void WriteToTBO();
	/*@}*/
};

#endif
