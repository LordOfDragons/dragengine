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

#ifndef _DEOGLRENDERTASKTEXTURE_H_
#define _DEOGLRENDERTASKTEXTURE_H_

#include "../../deoglBasics.h"

class deoglRenderTaskSharedTexture;
class deoglRenderTaskVAO;
class deoglVAO;


/**
 * Render Task Texture.
 */
class deoglRenderTaskTexture{
private:
	deoglRenderTaskSharedTexture *pTexture;
	
	deoglRenderTaskVAO *pRootVAO;
	deoglRenderTaskVAO *pTailVAO;
	int pVAOCount;
	
	deoglRenderTaskTexture *pNextTexture;
	
	deoglRenderTaskTexture *pLLNext;
	
	deoglRenderTaskVAO **pHasVAO;
	int pHasVAOCount;
	int pHasVAOSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task shader. */
	deoglRenderTaskTexture();
	
	/** Clean up render task shader. */
	~deoglRenderTaskTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset. */
	void Reset();
	
	/** Total count of points. */
	int GetTotalPointCount() const;
	
	/** Total count of instances. */
	int GetTotalInstanceCount() const;
	
	/** Total count of subinstances. */
	int GetTotalSubInstanceCount() const;
	
	
	
	/** Shared texture. */
	inline deoglRenderTaskSharedTexture *GetTexture() const{ return pTexture; }
	
	/** Set shared texture. */
	void SetTexture( deoglRenderTaskSharedTexture *texture );
	
	
	
	/** Retrieves the root render task vao or NULL if there is none. */
	inline deoglRenderTaskVAO *GetRootVAO() const{ return pRootVAO; }
	/** Retrieves the number of render task vaos. */
	inline int GetVAOCount() const{ return pVAOCount; }
	/** Retrieves the render task with the given vao or NULL if not found. */
	deoglRenderTaskVAO *GetVAOWith( deoglVAO *vao );
	/** Adds a render task vao. */
	void AddVAO( deoglRenderTaskVAO *vao );
	
	/** Retrieves the vao for a vao index or NULL if not existing yet. */
	deoglRenderTaskVAO *GetVAOForIndex( int vaoIndex );
	
	
	
	/** Next texture or NULL. */
	inline deoglRenderTaskTexture *GetNextTexture() const{ return pNextTexture; }
	
	/** Set next texture or NULL. */
	void SetNextTexture( deoglRenderTaskTexture *texture );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	inline deoglRenderTaskTexture *GetLLNext() const{ return pLLNext; }
	void SetLLNext( deoglRenderTaskTexture *texture );
	/*@}*/
};

#endif
