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
 * \brief OpenGL capabilities texture tormat.
 */
class deoglCapsTextureFormat{
private:
	const GLint pFormat;
	const GLenum pPixelFormat;
	const GLenum pPixelType;
	const int pBitsPerPixel;
	const bool pIsDepth;
	const bool pIsDepthFloat;
	const bool pIsCompressed;
	const decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create capabilities texture format. */
	deoglCapsTextureFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
		int bitsPerPixel, bool isDepth, bool isDepthFloat, bool isCompressed,
		const char *name );
	
	/** \brief Clean up capabilities texture format. */
	~deoglCapsTextureFormat();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Format. */
	inline GLint GetFormat() const{ return pFormat; }
	
	/** \brief Pixel format. */
	inline GLenum GetPixelFormat() const{ return pPixelFormat; }
	
	/** \brief Pixel type. */
	inline GLenum GetPixelType() const{ return pPixelType; }
	
	/** \brief Bits per pixel. */
	inline int GetBitsPerPixel() const{ return pBitsPerPixel; }
	
	/** \brief Format is a depth/stencil format. */
	inline bool GetIsDepth() const{ return pIsDepth; }
	
	/** \brief Format is a float depth/stencil format. */
	inline bool GetIsDepthFloat() const{ return pIsDepthFloat; }
	
	/** \brief Format is a compressed format. */
	inline bool GetIsCompressed() const{ return pIsCompressed; }
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Print format to buffer. */
	void WriteToString( decUnicodeString &buffer ) const;
	/*@}*/
};

#endif
