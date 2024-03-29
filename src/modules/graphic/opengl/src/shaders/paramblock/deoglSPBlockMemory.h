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

#ifndef _DEOGLSPBLOCKMEMORY_H_
#define _DEOGLSPBLOCKMEMORY_H_

#include "deoglShaderParameterBlock.h"



/**
 * Shader parameter block based on client memory.
 * 
 * Stores the data for a shader parameter block in main memory. Allows to read back the
 * data in contrary to deoglSPBlockUBO and deoglSPBlockSSBO which are write-only. Upon
 * mapping the memory is grown if not large enough but not shrunk.
 */
class deoglSPBlockMemory : public deoglShaderParameterBlock{
private:
	char *pBuffer;
	int pBufferCapacity;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader parameter block. */
	deoglSPBlockMemory( deoglRenderThread &renderThread );
	
	/** Create copy of shader storage buffer object. */
	deoglSPBlockMemory( const deoglSPBlockMemory &paramBlock );
	
protected:
	/** Clean up shader parameter block. */
	virtual ~deoglSPBlockMemory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Memory buffer. */
	inline char *GetBuffer() const{ return pBuffer; }
	
	/** Always throws an exception. */
	virtual void Activate() const;
	
	/** Activate buffer overriding binding point. */
	virtual void Activate( int bindingPoint ) const;
	
	/** Always throws an exception. */
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
	
	/**
	 * Map buffer for specific elements discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element, int count );
	
	/** Has no effect. */
	virtual void UnmapBuffer();
	
	/** Create copy of shader parameter block. */
	virtual deoglShaderParameterBlock *Copy() const;
	/*@}*/
	
	
	
private:
	void pGrowBuffer();
};

#endif
