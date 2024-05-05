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

#ifndef _DEOGLSHAREDVBOLIST_H_
#define _DEOGLSHAREDVBOLIST_H_

#include <dragengine/common/collection/decObjectList.h>

#include "../deoglBasics.h"
#include "deoglVBOLayout.h"

class deoglRenderThread;
class deoglSharedVBO;
class deoglSharedVBOBlock;



/**
 * A shared VBO list contains a list of VBOs all having the same layout. Each VBO contains
 * one or more blocks of data belonging to different resources. Each VBO has one VAO assigned
 * to reduce the VAO switching. Shared VBOs can be used in a static or dynamic way but all
 * data has to fit into one VBO.
 */
class deoglSharedVBOList{
public:
	deoglRenderThread &pRenderThread;
	deoglVBOLayout pLayout;
	GLenum pDrawType;
	int pMaxSize;
	int pMaxPointCount;
	int pMaxIndexSize;
	int pMaxIndexCount;
	decObjectList pVBOs;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared vbo list. */
	deoglSharedVBOList( deoglRenderThread &renderThread, const deoglVBOLayout &layout,
		GLenum drawType, int maxSize, int maxIndexSize );
	
	/** Clean up shared vbo list. */
	~deoglSharedVBOList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Layout. */
	inline const deoglVBOLayout &GetLayout() const{ return pLayout; }
	
	/** Draw type. */
	inline GLenum GetDrawType() const{ return pDrawType; }
	
	/** Maximum VBO size. */
	inline int GetMaxSize() const{ return pMaxSize; }
	
	/** Maximum count of points. */
	inline int GetMaxPointCount() const{ return pMaxPointCount; }
	
	/** Maximum IBO size. */
	inline int GetMaxIndexSize() const{ return pMaxIndexSize; }
	
	/** Maximum count of indices. */
	inline int GetMaxIndexCount() const{ return pMaxIndexCount; }
	
	/** VBO list matches the layout and draw type. */
	bool Matches( const deoglVBOLayout &layout, GLenum drawType ) const;
	
	/** Count of VBOs. */
	int GetCount() const;
	
	/** VBO at index. */
	deoglSharedVBO *GetAt( int index ) const;
	
	/** Add block of data to a matching VBO returning the resulting block. */
	deoglSharedVBOBlock *AddData( int size, int indexCount = 0 );
	
	/** Prepare all VBOs not yet prepared. */
	void PrepareVBOs();
	/*@}*/
};

#endif
