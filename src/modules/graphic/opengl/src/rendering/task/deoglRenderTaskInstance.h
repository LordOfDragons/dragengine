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

#ifndef _DEOGLRENDERTASKINSTANCE_H_
#define _DEOGLRENDERTASKINSTANCE_H_

#include "../../deoglBasics.h"

class deoglShaderParameterBlock;
class deoglRenderTaskSharedInstance;



/**
 * Render task instance.
 */
class deoglRenderTaskInstance{
public:
	/** Sub instance. */
	struct sSubInstance{
		int instance;
		int flags;
	};
	
	
	
private:
	const deoglRenderTaskSharedInstance *pInstance;
	
	sSubInstance *pSubInstances;
	int pSubInstanceCount;
	int pSubInstanceSize;
	deoglShaderParameterBlock *pSIIndexInstanceSPB;
	int pSIIndexInstanceFirst;
	int pDrawIndirectIndex;
	int pDrawIndirectCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task instance. */
	deoglRenderTaskInstance();
	
	/** Clean up render task instance. */
	~deoglRenderTaskInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Shared render task instance. */
	inline const deoglRenderTaskSharedInstance *GetInstance() const{ return pInstance; }
	
	/** Set shared render task instance. */
	void SetInstance( const deoglRenderTaskSharedInstance *instance );
	
	
	
	/** Count of sub instances to render. */
	inline int GetSubInstanceCount() const{ return pSubInstanceCount; }
	
	/** Sub instance at index. */
	const sSubInstance &GetSubInstanceAt( int index ) const;
	
	/** Add sub instance. */
	void AddSubInstance( int indexInstance, int flags );
	
	/** Sub instance index SPB. */
	inline deoglShaderParameterBlock *GetSIIndexInstanceSPB() const{ return pSIIndexInstanceSPB; }
	
	/** Index of first sub instance index for instance shared SPB. */
	inline int GetSIIndexInstanceFirst() const{ return pSIIndexInstanceFirst; }
	
	/** Set sub instance SPB information for later update. */
	void SetSIIndexInstanceParam( deoglShaderParameterBlock *paramBlock, int firstIndex );
	
	/** Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceInt( bool useFlags );
	void WriteSIIndexInstanceCompute();
	
	/** Draw indirect index. */
	inline int GetDrawIndirectIndex() const{ return pDrawIndirectIndex; }
	
	/** Set draw indirect index. */
	void SetDrawIndirectIndex( int index );
	
	/** Draw indirect count. */
	inline int GetDrawIndirectCount() const{ return pDrawIndirectCount; }
	
	/** Set draw indirect count. */
	void SetDrawIndirectCount( int count );
	
	
	
	/** Reset. */
	void Reset();
	/*@}*/
};

#endif
