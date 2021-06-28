/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLCAPSTEXTUREFORMAT_H_
#define _DEOGLCAPSTEXTUREFORMAT_H_

#include <dragengine/common/string/decString.h>

#include "../deoglBasics.h"

class decUnicodeString;



/**
 * OpenGL capabilities texture tormat.
 */
class deoglCapsTextureFormat{
private:
	const GLint pFormat;
	const GLenum pPixelFormat;
	const GLenum pPixelType;
	const int pBitsPerPixel;
	const bool pIsDepth;
	const bool pIsDepthFloat;
	const bool pIsStencil;
	const bool pIsCompressed;
	const decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create capabilities texture format. */
	deoglCapsTextureFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
		int bitsPerPixel, bool isDepth, bool isDepthFloat, bool isStencil,
		bool isCompressed, const char *name );
	
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
	void WriteToString( decUnicodeString &buffer ) const;
	/*@}*/
};

#endif
