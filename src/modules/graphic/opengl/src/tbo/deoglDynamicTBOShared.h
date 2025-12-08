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

#ifndef _DEOGLDYNAMICTBOSHARED_H_
#define _DEOGLDYNAMICTBOSHARED_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/deObject.h>
#include "../deoglBasics.h"
#include "../tbo/deoglDynamicTBO.h"

class deoglDynamicTBO;
class deoglDynamicTBOBlock;
class deoglRTLogger;


/**
 * Shared TBO containing one or more blocks.
 */
class deoglDynamicTBOShared : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglDynamicTBOShared> Ref;


public:
	const deoglDynamicTBO::Ref pTBO;
	const deoglDynamicTBO::Ref pTBO2;
	const int pStride;
	const int pStride2;
	decObjectList pBlocks;
	int pUsedSize;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared dynamic TBO. */
	deoglDynamicTBOShared(deoglDynamicTBO *tbo, int stride, deoglDynamicTBO *tbo2 = NULL, int stride2 = 1);
	
	/** Cleans up shared dynamic TBO. */
	virtual ~deoglDynamicTBOShared();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** TBO. */
	inline const deoglDynamicTBO::Ref &GetTBO() const{ return pTBO; }
	inline const deoglDynamicTBO::Ref &GetTBO2() const{ return pTBO2; }
	
	/** Stride in pixel for one unit. */
	inline int GetStride() const{ return pStride; }
	inline int GetStride2() const{ return pStride2; }
	
	/** Prepare TBO data if required. This does not upload the TBO data just prepare it. */
	void Prepare();
	
	/** Used size in units. */
	inline int GetUsedSize() const{ return pUsedSize; }
	
	/** Update used sizes in units. */
	void UpdateUsedSizes();
	
	/** Mark TBO dirty. */
	void MarkDirty();
	
	
	
	/** Count of blocks. */
	int GetBlockCount() const;
	
	/** Block at index. */
	deoglDynamicTBOBlock::Ref GetBlockAt(int index) const;
	
	/** Add block. Returns block or NULL if not enough space. */
	deoglDynamicTBOBlock *AddBlock(deoglDynamicTBO *tbo, deoglDynamicTBO *tbo2 = NULL);
	
	/** Remove block returning the space to the pool of free space. */
	void RemoveBlock(deoglDynamicTBOBlock *block);
	
	/**
	 * Index of first empty block TBO fits in or NULL if not found. If the only empty block
	 * is the last block the TBO is considered to always fit.
	 */
	int FirstMatchingBlock(deoglDynamicTBO *tbo);
	
	
	
	/** Debug print. */
	void DebugPrint(deoglRTLogger &logger) const;
	/*@}*/
	
	
	
private:
	deoglDynamicTBOBlock *pAddEmptyBlock();
	void pEnsureTBOSize();
};

#endif
