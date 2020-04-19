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

#ifndef _DEOGLSHAREDSPBELEMENT_H_
#define _DEOGLSHAREDSPBELEMENT_H_

#include <dragengine/deObject.h>

class deoglSharedSPB;
class deoglShaderParameterBlock;



/**
 * \brief OpenGL shared shader parameter block element.
 */
class deoglSharedSPBElement : public deObject{
public:
	deoglSharedSPB &pSPB;
	int pIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared shader parameter block element. */
	deoglSharedSPBElement( deoglSharedSPB &spb, int index );
	
	/** \brief Clean up shared shader parameter block element. */
	virtual ~deoglSharedSPBElement();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Shader parameter block this element belongs to. */
	inline deoglSharedSPB &GetSPB() const{ return pSPB; }
	
	/** \brief Element index in the shader parameter block. */
	inline int GetIndex() const{ return pIndex; }
	
	/**
	 * \brief Map buffer for element.
	 * 
	 * Call UnmapBuffer() on the shader parameter block once finished. You can call
	 * all methods without explicit element index to write into the mapped element
	 * while the buffer is mapped.
	 */
	deoglShaderParameterBlock &MapBuffer() const;
	/*@}*/
};

#endif
