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

#ifndef _DEOGLSPBLOCKMEMORY_H_
#define _DEOGLSPBLOCKMEMORY_H_

#include "deoglShaderParameterBlock.h"



/**
 * Shader parameter block based on client memory.
 * 
 * Stores the data for a shader parameter block in main memory. Allows to read back the
 * data in contrary to deoglSPBlockUBO and deoglSPBlockSSBO which are write-only. Upon
 * mapping the memory is grown if not large enough but not shrunk.
 */
class deoglSPBlockMemory : public deoglShaderParameterBlock{
private:
	char *pBuffer;
	int pBufferCapacity;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader parameter block. */
	deoglSPBlockMemory( deoglRenderThread &renderThread );
	
	/** Create copy of shader storage buffer object. */
	deoglSPBlockMemory( const deoglSPBlockMemory &paramBlock );
	
protected:
	/** Clean up shader parameter block. */
	virtual ~deoglSPBlockMemory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Memory buffer. */
	inline char *GetBuffer() const{ return pBuffer; }
	
	/** Always throws an exception. */
	virtual void Activate() const;
	
	/** Activate buffer overriding binding point. */
	virtual void Activate( int bindingPoint ) const;
	
	/** Always throws an exception. */
	virtual void Deactivate() const;
	
	/** Deactivate buffer overriding binding point. */
	virtual void Deactivate( int bindingPoint ) const;
	
	/** Map buffer discarding content. */
	virtual void MapBuffer();
	
	/**
	 * Map buffer for specific element discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element );
	
	/**
	 * Map buffer for specific elements discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element, int count );
	
	/** Has no effect. */
	virtual void UnmapBuffer();
	
	/** Create copy of shader parameter block. */
	virtual deoglShaderParameterBlock *Copy() const;
	/*@}*/
	
	
	
private:
	void pGrowBuffer();
};

#endif
