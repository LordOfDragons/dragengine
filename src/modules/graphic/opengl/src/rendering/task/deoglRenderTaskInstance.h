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

#include <dragengine/common/collection/decIntList.h>

class deoglShaderParameterBlock;
class deoglRenderTaskSharedInstance;



/**
 * Render task instance.
 */
class deoglRenderTaskInstance{
private:
	const deoglRenderTaskSharedInstance *pInstance;
	
	int pSubInstanceCount;
	decIntList pSIIndexInstance;
	decIntList pSIFlags;
	deoglShaderParameterBlock *pSIIndexInstanceSPB;
	int pSIIndexInstanceFirst;
	
	deoglRenderTaskInstance *pNextInstance;
	
	deoglRenderTaskInstance *pLLNext;
	
	
	
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
	
	/** Add sub instance. */
	void AddSubInstance( int indexInstance, int flags );
	
	/** Sub instance indices for instance shared SPB. */
	inline const decIntList &GetSIIndexInstance() const{ return pSIIndexInstance; }
	
	/** Sub instance flags. */
	inline const decIntList &GetSIFlags() const{ return pSIFlags; }
	
	/** Sub instance index SPB. */
	inline deoglShaderParameterBlock *GetSIIndexInstanceSPB() const{ return pSIIndexInstanceSPB; }
	
	/** Index of first sub instance index for instance shared SPB. */
	inline int GetSIIndexInstanceFirst() const{ return pSIIndexInstanceFirst; }
	
	/** Set sub instance SPB information for later update. */
	void SetSIIndexInstanceParam( deoglShaderParameterBlock *paramBlock, int firstIndex );
	
	/** Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceInt( bool useFlags );
	
	/** Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceShort( bool useFlags );
	
	
	
	/** Clear. */
	void Clear();
	
	
	
	/** Next instance or NULL. */
	inline deoglRenderTaskInstance *GetNextInstance() const{ return pNextInstance; }
	
	/** Set next instance or NULL. */
	void SetNextInstance( deoglRenderTaskInstance *instance );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** Next instance in pool or NULL. */
	inline deoglRenderTaskInstance *GetLLNext() const{ return pLLNext; }
	
	/** Set next instance in pool or NULL. */
	void SetLLNext( deoglRenderTaskInstance *instance );
	/*@}*/
};

#endif
