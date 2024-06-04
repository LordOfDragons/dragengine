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

#ifndef _DEOGLSPBSINGLEUSE_H_
#define _DEOGLSPBSINGLEUSE_H_

#include <stdint.h>

#include "deoglShaderParameterBlock.h"

#include <dragengine/common/collection/decObjectList.h>


/**
 * Single use shader parameter block. Stores one or more parameter blocks in a ring list.
 * Each time Next() is called the next block from the list is returned. Upon render thread
 * frame counter change the start index is set to the current list position. If Next()
 * would end up at the start index after a full loop a new parameter block is inserted
 * as a clone of the first parameter block in the list.
 */
class deoglSPBSingleUse : public deObject{
public:
	typedef deTObjectReference<deoglSPBSingleUse> Ref;
	
	
	
private:
	const deoglRenderThread &pRenderThread;
	deoglShaderParameterBlock *pInitialBlock;
	decObjectList pBlocks;
	uint32_t pFrameCounter;
	int pStartIndex;
	int pNextIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Map block. */
	deoglSPBSingleUse( const deoglRenderThread &renderThread, deoglShaderParameterBlock *block );
	
protected:
	/** Unmap block if mapped. */
	virtual ~deoglSPBSingleUse();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Get next parameter block. Caller does not take over reference */
	deoglShaderParameterBlock *Next();
	/*@}*/
};

#endif
