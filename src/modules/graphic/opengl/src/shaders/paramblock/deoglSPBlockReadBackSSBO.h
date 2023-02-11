/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSPBLOCKREADBACKSSBO_H_
#define _DEOGLSPBLOCKREADBACKSSBO_H_

#include "deoglShaderParameterBlock.h"

class deoglSPBlockSSBO;


/**
 * OpenGL shader parameter block based on Shader Storage Buffer Object for read-back use.
 */
class deoglSPBlockReadBackSSBO : public deoglShaderParameterBlock{
public:
	typedef deTObjectReference<deoglSPBlockReadBackSSBO> Ref;
	
	
	
private:
	GLuint pSSBO;
	bool pCompact;
	bool pAllocateBuffer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader storage buffer object. */
	deoglSPBlockReadBackSSBO( deoglRenderThread &renderThread );
	
	/** Create copy of shader storage buffer object. */
	deoglSPBlockReadBackSSBO( const deoglSPBlockReadBackSSBO &paramBlock );
	
	/** Create copy of shader storage buffer object. */
	deoglSPBlockReadBackSSBO( const deoglSPBlockSSBO &paramBlock );
	
protected:
	/** Clean up shader storage buffer object. */
	virtual ~deoglSPBlockReadBackSSBO();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shader storage buffer object or 0 if not created yet. */
	inline GLuint GetSSBO() const{ return pSSBO; }
	
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
	
	/** Map buffer for reading. Get buffer pointer with GetMappedBuffer(). */
	virtual void MapBuffer();
	
	/** Map buffer for reading specific element. Get buffer pointer with GetMappedBuffer(). */
	virtual void MapBuffer( int element );
	
	/** Map buffer for specific elements discarding content. */
	virtual void MapBuffer( int element, int count );
	
	/** Unmap buffer. */
	virtual void UnmapBuffer();
	
	/** Ensure buffer exists. Calls MapBuffer() then UnmapBuffer(). */
	virtual void EnsureBuffer();
	
	/** Get platform alignment requirements. */
	virtual int GetAlignmentRequirements() const;
	
	/** Start transfering data from SSBO to PBO asynchronously. */
	void TransferFrom( const deoglSPBlockSSBO &ssbo );
	void TransferFrom( const deoglSPBlockSSBO &ssbo, int elementCount );
	
	/** Debug print configuration. */
	void DebugPrintConfig( const char *name );
	/*@}*/
	
	
	
protected:
	virtual void pUpdateBufferSize();
};

#endif
