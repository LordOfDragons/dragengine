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

#ifndef _DEOGLCAPSTEXTUREFORMAT_H_
#define _DEOGLCAPSTEXTUREFORMAT_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTList.h>

#include "../deoglBasics.h"

class decUnicodeString;



/**
 * OpenGL capabilities texture tormat.
 */
class deoglCapsTextureFormat{
public:
	using List = decTList<deoglCapsTextureFormat>;
	

private:
	GLint pFormat;
	GLenum pPixelFormat;
	GLenum pPixelType;
	int pBitsPerPixel;
	bool pIsDepth;
	bool pIsDepthFloat;
	bool pIsStencil;
	bool pIsCompressed;
	decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create default capabilities texture format. */
	deoglCapsTextureFormat() = default;
	
	/** Create capabilities texture format. */
	deoglCapsTextureFormat(GLint format, GLenum pixelFormat, GLenum pixelType,
		int bitsPerPixel, bool isDepth, bool isDepthFloat, bool isStencil,
		bool isCompressed, const char *name);
	
	/** Copy. */
	deoglCapsTextureFormat(const deoglCapsTextureFormat &format) = default;
	deoglCapsTextureFormat &operator=(const deoglCapsTextureFormat &format) = default;
	
	/** Move. */
	deoglCapsTextureFormat(deoglCapsTextureFormat &&format) noexcept = default;
	deoglCapsTextureFormat &operator=(deoglCapsTextureFormat &&format) noexcept = default;
	
	/** Clean up capabilities texture format. */
	~deoglCapsTextureFormat();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Format. */
	inline GLint GetFormat() const{ return pFormat; }
	
	/** Pixel format. */
	inline GLenum GetPixelFormat() const{ return pPixelFormat; }
	
	/** Pixel type. */
	inline GLenum GetPixelType() const{ return pPixelType; }
	
	/** Bits per pixel. */
	inline int GetBitsPerPixel() const{ return pBitsPerPixel; }
	
	/** Format is a depth/stencil format. */
	inline bool GetIsDepth() const{ return pIsDepth; }
	
	/** Format is a float depth/stencil format. */
	inline bool GetIsDepthFloat() const{ return pIsDepthFloat; }
	
	/** Format is a stencil format. */
	inline bool GetIsStencil() const{ return pIsStencil; }
	
	/** Format is a compressed format. */
	inline bool GetIsCompressed() const{ return pIsCompressed; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Print format to buffer. */
	void WriteToString(decUnicodeString &buffer) const;
	/*@}*/
};

#endif
