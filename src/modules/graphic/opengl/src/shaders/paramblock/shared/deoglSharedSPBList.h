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

#ifndef _DEOGLSHAREDSPBLIST_H_
#define _DEOGLSHAREDSPBLIST_H_

#include <dragengine/common/collection/decObjectList.h>

#include "../deoglShaderParameterBlock.h"

class deoglRenderThread;
class deoglSharedSPB;
class deoglSharedSPBElement;



/**
 * OpenGL shared shader parameter block List.
 */
class deoglSharedSPBList{
private:
	deoglRenderThread &pRenderThread;
	const deoglShaderParameterBlock::Ref pLayout;
	int pSize;
	decObjectList pSPBs;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared shader parameter block list. */
	deoglSharedSPBList(deoglRenderThread &renderThread, deoglShaderParameterBlock *layout);
	
public:
	/** Clean up shared shader parameter block list. */
	virtual ~deoglSharedSPBList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** Layout reference. */
	inline const deoglShaderParameterBlock::Ref &GetLayout() const{return pLayout;}
	
	/** Maximum number of elements per block. */
	int GetSize() const;
	
	/** Number of blocks. */
	int GetCount() const;
	
	/** Block at index. */
	deoglSharedSPB *GetAt(int index) const;
	
	/**
	 * Add element.
	 * 
	 * Caller obtains reference to the element. Release reference if not used anymore.
	 * Element is removed from the shared parameter block once all references are released.
	 */
	deoglSharedSPBElement *AddElement();
	/*@}*/
	
	
	
protected:
	virtual deoglShaderParameterBlock::Ref pCreateBlock() const = 0;
};

#endif
