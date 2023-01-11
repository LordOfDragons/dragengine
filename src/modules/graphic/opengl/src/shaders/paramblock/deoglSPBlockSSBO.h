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

#ifndef _DEOGLSPBLOCKSSBO_H_
#define _DEOGLSPBLOCKSSBO_H_

#include "deoglShaderParameterBlock.h"


/**
 * OpenGL shader parameter block based on Shader Storage Buffer Object.
 */
class deoglSPBlockSSBO : public deoglShaderParameterBlock{
public:
	typedef deTObjectReference<deoglSPBlockSSBO> Ref;
	
	
	
private:
	GLuint pSSBO;
	int pBindingPoint;
	bool pAllocateBuffer;
	
	char *pWriteBuffer;
	int pWriteBufferCapacity;
	bool pWriteBufferUsed;
	
	int pMemoryGPUSSBO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader storage buffer object. */
	deoglSPBlockSSBO( deoglRenderThread &renderThread );
	
	/** Create copy of shader storage buffer object. */
	deoglSPBlockSSBO( const deoglSPBlockSSBO &paramBlock );
	
protected:
	/** Clean up shader storage buffer object. */
	virtual ~deoglSPBlockSSBO();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shader storage buffer object or 0 if not created yet. */
	inline GLuint GetSSBO() const{ return pSSBO; }
	
	/** Binding point. */
	inline int GetBindingPoint() const{ return pBindingPoint; }
	
	/** Set binding point. */
	void SetBindingPoint( int bindingPoint );
	
	/** Activate buffer. */
	virtual void Activate() const;
	
	/** Activate buffer overriding binding point. */
	virtual void Activate( int bindingPoint ) const;
	
	/** Deactivate buffer. */
	virtual void Deactivate() const;
	
	/** Deactivate buffer overriding binding point. */
	virtual void Deactivate( int bindingPoint ) const;
	
	/** Map buffer discarding content. */
	virtual void MapBuffer();
	
	/**
	 * Map buffer for specific element discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element );
	
	/** Unmap buffer uploading data to GPU. */
	virtual void UnmapBuffer();
	
	/** Get platform alignment requirements. */
	virtual int GetAlignmentRequirements() const;
	
	/** Map the parameter block definition to a shader uniform block using std430 layout. */
	void MapToStd430();
	
	
	
	/** GPU memory consumption for the VBO. */
	inline int GetMemoryConsumptionGPUVBO() const{ return pMemoryGPUSSBO; }
	
	/** Debug print configuration. */
	void DebugPrintConfig( const char *name );
	/*@}*/
	
	
	
protected:
	virtual void pUpdateBufferSize();
	
	
	
private:
	void pGrowWriteBuffer( int size );
};

#endif
