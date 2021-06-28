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

#ifndef _DEOGLCAPSTEXTUREFORMATLIST_H_
#define _DEOGLCAPSTEXTUREFORMATLIST_H_

#include "../deoglBasics.h"

class deoglCapsTextureFormat;



/**
 * \brief OpenGL capabilities texture format list.
 */
class deoglCapsTextureFormatList{
private:
	deoglCapsTextureFormat **pFormats;
	int pFormatCount;
	int pFormatSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create format list. */
	deoglCapsTextureFormatList();
	
	/** \brief Clean up format list. */
	~deoglCapsTextureFormatList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of formats. */
	inline int GetFormatCount() const{ return pFormatCount; }
	
	/** \brief Format at index. */
	const deoglCapsTextureFormat *GetFormatAt( int index ) const;
	
	/** \brief Format with opengl format or \em NULL if absent. */
	const deoglCapsTextureFormat *GetFormatWith( GLint format ) const;
	
	/** \brief Add format. */
	void AddFormat( GLint format, GLenum pixelFormat, GLenum pixelType, int bitsPerPixel,
		bool isDepth, bool isDepthFloat, bool isStencil, bool isCompressed, const char *name );
	/*@}*/
};

#endif
