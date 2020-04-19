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

#ifndef _DEOGLSPBLOCKVBO_H_
#define _DEOGLSPBLOCKVBO_H_

#include "deoglShaderParameterBlock.h"



/**
 * \brief Shader parameter block based on VBO.
 * 
 * Stores the data for a shader parameter block. This is used to set the value of a GLSL
 * uniform block in shader programs. Each block is defined as a byte array with members
 * in the uniform block mapped areas inde the data block.
 */
class deoglSPBlockUBO : public deoglShaderParameterBlock{
private:
	GLuint pUBO;
	int pBindingPoint;
	bool pAllocateBuffer;
	
	char *pWriteBuffer;
	int pWriteBufferCapacity;
	bool pWriteBufferUsed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shader parameter block. */
	deoglSPBlockUBO( deoglRenderThread &renderThread );
	
	/** \brief Create copy of shader storage buffer object. */
	deoglSPBlockUBO( const deoglSPBlockUBO &paramBlock );
	
protected:
	/** \brief Clean up shader parameter block. */
	virtual ~deoglSPBlockUBO();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Uniform buffer object or 0 if not created yet. */
	inline GLuint GetUBO() const{ return pUBO; }
	
	/** \brief Binding point. */
	inline int GetBindingPoint() const{ return pBindingPoint; }
	
	/** \brief Set binding point. */
	void SetBindingPoint( int bindingPoint );
	
	/** \brief Activate buffer. */
	virtual void Activate();
	
	/** \brief Deactivate buffer. */
	virtual void Deactivate();
	
	/** \brief Map buffer discarding content. */
	virtual void MapBuffer();
	
	/**
	 * \brief Map buffer for specific element discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element );
	
	/** \brief Unmap buffer uploading data to GPU. */
	virtual void UnmapBuffer();
	
	/** \brief Debug print configuration. */
	void DebugPrintConfig( const char *name );
	/*@}*/
	
	
	
protected:
	virtual void pUpdateBufferSize();
	
	
	
private:
	void pGrowWriteBuffer( int size );
};

#endif
