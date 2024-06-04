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

#ifndef _DEOGLSHAREDSPBELEMENTMAPBUFFER_H_
#define _DEOGLSHAREDSPBELEMENTMAPBUFFER_H_

class deoglSharedSPBElement;
class deoglShaderParameterBlock;


/**
 * Map shader parameter block while in scope. Allows manual mapping and unmapping.
 */
class deoglSharedSPBElementMapBuffer{
private:
	deoglSharedSPBElement &pElement;
	deoglShaderParameterBlock *pBlock;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Map block. */
	deoglSharedSPBElementMapBuffer( deoglSharedSPBElement &element );
	
	/** Unmap block if mapped. */
	~deoglSharedSPBElementMapBuffer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Map block if not mapped. */
	void Map();
	
	/** Block or nullptr if not mapped. */
	inline deoglShaderParameterBlock *GetBlock() const{ return pBlock; }
	
	/** Block or throws exception if not mapped. */
	deoglShaderParameterBlock &GetBlockRef() const;
	
	/** Block or nullptr if not mapped. */
	inline operator deoglShaderParameterBlock*() const{ return GetBlock(); }
	
	/** Block or throws exception if not mapped. */
	inline operator deoglShaderParameterBlock&() const{ return GetBlockRef(); }
	
	/** Unmap block if mapped. */
	void Unmap();
	/*@}*/
};

#endif
