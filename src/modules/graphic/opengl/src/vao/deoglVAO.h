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

#ifndef _DEOGLVAO_H_
#define _DEOGLVAO_H_

#include "../deoglBasics.h"
#include "../vbo/deoglVBOLayout.h"

class deoglRenderThread;
class deoglRenderTaskSharedVAO;


/**
 * Small wrapper for VAO objects to help with tracking VAO switches, storing indexed rendering
 * properties and speeding up render task creation.
 */
class deoglVAO{
public:
	deoglRenderThread &pRenderThread;
	
	GLuint pVAO;
	
	deoglVBOLayout::eIndexTypes pIndexType;
	int pIndexSize;
	int pIndexGLType;
	
	unsigned int pUniqueKey;
	deoglRenderTaskSharedVAO *pRTSVAO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create vao. */
	deoglVAO(deoglRenderThread &renderThread);
	
	/** Clean up vao. */
	virtual ~deoglVAO();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** VAO. */
	inline GLuint GetVAO() const{ return pVAO; }
	
	/** Index type. */
	inline deoglVBOLayout::eIndexTypes GetIndexType() const{ return pIndexType; }
	
	/** Set index type. */
	void SetIndexType(deoglVBOLayout::eIndexTypes indexType);
	
	/** Size in bytes of indices. */
	inline int GetIndexSize() const{ return pIndexSize; }
	
	/** OpenGL type of the indices. */
	inline int GetIndexGLType() const{ return pIndexGLType; }
	
	/** Unique key for use with dictionaries. */
	inline unsigned int GetUniqueKey() const{ return pUniqueKey; }
	
	/** Render task shared vao or NULL. */
	inline deoglRenderTaskSharedVAO *GetRTSVAO() const{ return pRTSVAO; }
	
	/** Ensure render task shared vao is present. */
	void EnsureRTSVAO();
	/*@}*/
};

#endif
