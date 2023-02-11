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
	int pBindingPointUBO;
	int pBindingPointAtomic;
	bool pCompact;
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
	
	/** UBO Binding point. */
	inline int GetBindingPointUBO() const{ return pBindingPointUBO; }
	
	/** Set UBO binding point. */
	void SetBindingPointUBO( int bindingPoint );
	
	/** Atmomic counter binding point. */
	inline int GetBindingPointAtomic() const{ return pBindingPointAtomic; }
	
	/** Set atomic counter binding point. */
	void SetBindingPointAtomic( int bindingPoint );
	
	/** Compact elements. If true mapping individual elements is prohibited. */
	inline bool GetCompact() const{ return pCompact; }
	
	/** Set if elements are compact. If true mapping individual elements is prohibited. */
	void SetCompact( bool compact );
	
	/** Activate buffer. */
	virtual void Activate() const;
	
	/** Activate buffer overriding binding point. */
	virtual void Activate( int bindingPoint ) const;
	
	/** Deactivate buffer. */
	virtual void Deactivate() const;
	
	/** Deactivate buffer overriding binding point. */
	virtual void Deactivate( int bindingPoint ) const;
	
	/** Activate buffer as UBO. */
	void ActivateUBO() const;
	
	/** Deactivate buffer as UBO. */
	void DeactivateUBO() const;
	
	/** Activate buffer as atomic buffer. */
	void ActivateAtomic() const;
	
	/** Deactivate buffer as atomic buffer. */
	void DeactivateAtomic() const;
	
	/** Activate buffer as dispatch indirect. */
	void ActivateDispatchIndirect() const;
	
	/** Deactivate buffer as dispatch indirect. */
	void DeactivateDispatchIndirect() const;
	
	/** Map buffer discarding content. */
	virtual void MapBuffer();
	
	/**
	 * Map buffer for specific element discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element );
	
	/**
	 * Map buffer for specific elements discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element, int count );
	
	/** Unmap buffer uploading data to GPU. */
	virtual void UnmapBuffer();
	
	/** Get platform alignment requirements. */
	virtual int GetAlignmentRequirements() const;
	
	/** Map the parameter block definition to a shader uniform block using std430 layout. */
	void MapToStd430();
	
	/** Read data from SSBO into write buffer. Returns write buffer memory pointer. */
	char *ReadBuffer();
	
	/** Read partial data from SSBO into write buffer. Returns write buffer memory pointer. */
	char *ReadBuffer( int elementCount );
	
	/** Direct access to write buffer. Use with care. Can be nullptr. */
	inline char *GetWriteBuffer() const{ return pWriteBuffer; }
	
	
	
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
