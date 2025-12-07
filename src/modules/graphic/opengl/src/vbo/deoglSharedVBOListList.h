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

#ifndef _DEOGLSHAREDVBOLISTLIST_H_
#define _DEOGLSHAREDVBOLISTLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglVBOLayout;
class deoglRenderThread;
class deoglSharedVBOList;



/**
 * List of shared VBO lists. Convenience class to store various types of shared VBO lists.
 * The list is gready so lists are created if not existing and stay around for as long as
 * the graphic module runs.
 */
class deoglSharedVBOListList{
public:
	deoglRenderThread &pRenderThread;
	int pMaxSize;
	int pMaxIndexSize;
	decPointerList pLists;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared vbo list list. */
	deoglSharedVBOListList(deoglRenderThread &renderThread, int maxSize, int maxIndexSize);
	
	/** Clean up shared vbo list list. */
	~deoglSharedVBOListList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Maximum VBO size. */
	inline int GetMaxSize() const{ return pMaxSize; }
	
	/** Maximum IBO size. */
	inline int GetMaxIndexSize() const{ return pMaxIndexSize; }
	
	/** Count of lists. */
	int GetCount() const;
	
	/** List at index. */
	deoglSharedVBOList *GetAt(int index) const;
	
	/** Matching list is present. */
	bool HasWith(const deoglVBOLayout &layout, GLenum drawType) const;
	
	/** Matching list creating it if absent. */
	deoglSharedVBOList *GetWith(const deoglVBOLayout &layout, GLenum drawType);
	
	/** Prepare all lists. */
	void PrepareAllLists();
	/*@}*/
};

#endif
