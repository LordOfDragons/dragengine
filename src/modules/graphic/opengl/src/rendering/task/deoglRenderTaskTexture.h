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

class deoglTexUnitsConfig;
class deoglSkinTexture;
class deoglRenderTaskVAO;
class deoglSPBlockUBO;
class deoglVAO;


/**
 * \brief Render Task Texture.
 */
class deoglRenderTaskTexture{
private:
	deoglTexUnitsConfig *pTUC;
	deoglSkinTexture *pTexture;
	deoglSPBlockUBO *pParamBlock;
	
	deoglRenderTaskVAO *pRootVAO;
	deoglRenderTaskVAO *pTailVAO;
	int pVAOCount;
	
	deoglRenderTaskTexture *pNextTexture;
	
	deoglRenderTaskTexture *pLLNext;
	
	deoglRenderTaskVAO **pHasVAO;
	int pHasVAOCount;
	int pHasVAOSize;
	
	int pMaterialIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new render task shader. */
	deoglRenderTaskTexture();
	/** Cleans up the render task shader. */
	~deoglRenderTaskTexture();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Reset the texture. */
	void Reset();
	
	/** Retrieves the total amount of points in this texture. */
	int GetTotalPointCount() const;
	/** Retrieves the total amount of instances in this texture. */
	int GetTotalInstanceCount() const;
	/** Retrieves the total amount of subinstances in this texture. */
	int GetTotalSubInstanceCount() const;
	
	/** Retrieves the texture units configuration. */
	inline deoglTexUnitsConfig *GetTUC() const{ return pTUC; }
	/** Sets the texture units configuration. */
	void SetTUC( deoglTexUnitsConfig *tuc );
	/** Retrieves the skin texture. */
	inline deoglSkinTexture *GetTexture() const{ return pTexture; }
	/** Sets the skin texture. */
	void SetTexture( deoglSkinTexture *texture );
	/** Retrieves the shader parameter block or NULL if not used. */
	inline deoglSPBlockUBO *GetParameterBlock() const{ return pParamBlock; }
	/** Sets the shader parameter block or NULL if not used. */
	void SetParameterBlock( deoglSPBlockUBO *block );
	
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
	
	/** Retrieves the next texture to render or NULL if this is the last one. */
	inline deoglRenderTaskTexture *GetNextTexture() const{ return pNextTexture; }
	/** Sets the next texture to render or NULL if this is the last one. */
	void SetNextTexture( deoglRenderTaskTexture *texture );
	
	/** \brief Material index. */
	inline int GetMaterialIndex() const{ return pMaterialIndex; }
	
	/** \brief Set material index. */
	void SetMaterialIndex( int materialIndex );
	/*@}*/
	
	/** \name Linked List */
	/*@{*/
	/** Retrieves the next texture in the pool in the parent render task or NULL if there is none. */
	inline deoglRenderTaskTexture *GetLLNext() const{ return pLLNext; }
	/** Sets the next texture in the pool in the parent render task or NULL if there is none. */
	void SetLLNext( deoglRenderTaskTexture *texture );
	/*@}*/
};

#endif
