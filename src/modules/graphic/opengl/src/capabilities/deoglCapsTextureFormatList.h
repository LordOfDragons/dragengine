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

#ifndef _DEOGLCAPSTEXTUREFORMATLIST_H_
#define _DEOGLCAPSTEXTUREFORMATLIST_H_

#include "../deoglBasics.h"

class deoglCapsTextureFormat;



/**
 * OpenGL capabilities texture format list.
 */
class deoglCapsTextureFormatList{
private:
	deoglCapsTextureFormat **pFormats;
	int pFormatCount;
	int pFormatSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create format list. */
	deoglCapsTextureFormatList();
	
	/** Clean up format list. */
	~deoglCapsTextureFormatList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of formats. */
	inline int GetFormatCount() const{ return pFormatCount; }
	
	/** Format at index. */
	const deoglCapsTextureFormat *GetFormatAt(int index) const;
	
	/** Format with opengl format or \em NULL if absent. */
	const deoglCapsTextureFormat *GetFormatWith(GLint format) const;
	
	/** Add format. */
	void AddFormat(GLint format, GLenum pixelFormat, GLenum pixelType, int bitsPerPixel,
		bool isDepth, bool isDepthFloat, bool isStencil, bool isCompressed, const char *name);
	/*@}*/
};

#endif
