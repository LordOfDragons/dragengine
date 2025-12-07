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

#ifndef _DEOGLVBOWRITERCANVASPAINT_H_
#define _DEOGLVBOWRITERCANVASPAINT_H_

#include "../../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglSharedVBOBlock;



/**
 * Canvas paint VBO Writer.
 * 
 * Helper class writing data to esvbolCanvasPaint formatted VBO blocks. Keeps track of the
 * data pointer adding one entry after the other. Call Reset with a vbo block data pointer
 * to start the writing. Each call of WritePoint appends the data and advances the pointer
 * for the next point.
 */
class deoglVBOWriterCanvasPaint{
public:
	deoglRenderThread &pRenderThread;
	char *pDataPoints;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create vbo writer. */
	deoglVBOWriterCanvasPaint(deoglRenderThread &renderThread);
	
	/** Clean up vbo writer. */
	~deoglVBOWriterCanvasPaint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set data pointer to start. */
	void Reset(deoglSharedVBOBlock *vboBlock);
	
	/** Write point and advance pointer for the next write. */
	void WritePoint(const decVector2 &position);
	/*@}*/
};

#endif
