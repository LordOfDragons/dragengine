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

#ifndef _DEOGLSHAREDBLOCKSPB_H_
#define _DEOGLSHAREDBLOCKSPB_H_

#include "../deoglShaderParameterBlock.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerList.h>

class deoglSharedBlockSPBElement;
class deoglShaderParameterBlock;
class deoglRTLogger;


/**
 * OpenGL Shared Shader Parameter Block.
 * 
 * Shared shader parameter blocks contain one or more elements sharing all the same
 * data layout. Each element can be updated individually in the parameter block.
 * Each shared parameter block can have a maximum number of elements. Elements are
 * added to open slots without moving. Hence the index of an element is not changed
 * after it has been created.
 * 
 * Main purpose of shared parameter blocks is to reduce the state changes and to allow
 * rendering multiple instances with different parameter blocks more efficiently.
 * 
 * This is the basic class. Subclasses define the actual type of backing storage used.
 */
class deoglSharedBlockSPB : public deObject{
public:
	typedef deTObjectReference<deoglSharedBlockSPB> Ref;
	
	
private:
	const deoglShaderParameterBlock::Ref pParameterBlock;
	decObjectList pElements;
	decPointerList pEmptyElements;
	int pSize;
	int pUsedElementCount;
	int pFreeElementCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * Create shared shader parameter block.
	 * 
	 * \warning Do not modify the parameter block after creating the shared object.
	 */
	deoglSharedBlockSPB( deoglShaderParameterBlock *parameterBlock );
	
protected:
	/** Clean up shared shader parameter block. */
	virtual ~deoglSharedBlockSPB();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parameter block storing the elements. */
	inline const deoglShaderParameterBlock::Ref &GetParameterBlock() const{ return pParameterBlock; }
	
	/** Obtain element. Call deoglSharedBlockSPBElement::Return to return element. */
	deoglSharedBlockSPBElement *GetElement( int count );
	
	/** Return element. */
	void ReturnElement( deoglSharedBlockSPBElement *element );
	
	/** Maximal count of elements the buffer can store. */
	inline int GetSize() const{ return pSize; }
	
	/** Count of used elements. */
	inline int GetUsedElementCount() const{ return pUsedElementCount; }
	
	/** Count of free elements. */
	inline int GetFreeElementCount() const{ return pFreeElementCount; }
	
	/** Count of free elements at the end of the buffer. */
	int GetFreeElementCountAtEnd() const;
	
	/** Debug print blocks. */
	void DebugPrint( deoglRTLogger &logger ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	int pIndexOfEmptyElementWithMinCount( int count );
	void pCheckSize();
};

#endif
