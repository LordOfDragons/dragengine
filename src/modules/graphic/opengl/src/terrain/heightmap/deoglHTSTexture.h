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

#ifndef _DEOGLHTSTEXTURE_H_
#define _DEOGLHTSTEXTURE_H_

#include "../../skin/deoglSkinTexture.h"

#include <dragengine/common/math/decMath.h>

class deoglRHTSector;
class deoglRSkin;
class deoglSPBlockUBO;
class deoglSkinShader;
class deoglTexUnitsConfig;


/**
 * Height Terrain Sector Texture.
 */
class deoglHTSTexture{
private:
	deoglRHTSector &pSector;
	int pIndex;
	
	decTexMatrix pMatrix;
	
	deoglRSkin *pSkin;
	
	deoglSkinTexture *pUseSkinTexture;
	
	deoglSPBlockUBO *pParamBlock;
	
	deoglTexUnitsConfig *pTUCDepth;
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCShadow;
	deoglTexUnitsConfig *pTUCEnvMap;
	deoglTexUnitsConfig *pTUCLuminance;
	
	bool pValidParamBlock;
	bool pDirtyParamBlock;
	
	bool pDirtyTUCs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create height terrain sector texture. */
	deoglHTSTexture( deoglRHTSector &sector, int index );
	
	/** Clean up height terrain sector texture. */
	~deoglHTSTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Height terrain sector. */
	inline deoglRHTSector &GetHTSector() const{ return pSector; }
	
	/** Texture index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Texture matrix. */
	inline const decTexMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Set texture matrix. */
	void SetMatrix( const decTexMatrix &matrix );
	
	/** Skin or NULL. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin or NULL. */
	void SetSkin( deoglRSkin *skin );
	
	/** Skin texture to use or NULL. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	/** Shader parameter block. */
	inline deoglSPBlockUBO *GetParamBlock() const{ return pParamBlock; }
	
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** Mark texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** Texture units configuration for shader type. */
	deoglTexUnitsConfig *GetTUCForPipelineType ( deoglSkinTexturePipelines::eTypes type ) const;
	
	/**
	 * Texture units configuration for depth type shaders or NULL if empty.
	 * Works for these shader types:
	 * - estComponentDepth
	 * - estComponentDepthClipPlane
	 * - estComponentCounter
	 * - estComponentCounterClipPlane
	 */
	inline deoglTexUnitsConfig *GetTUCDepth() const{ return pTUCDepth; }
	
	/**
	 * Texture units configuration for geometry type shaders or NULL if empty.
	 * Works for these shader types:
	 * - estComponentGeometry.
	 */
	inline deoglTexUnitsConfig *GetTUCGeometry() const{ return pTUCGeometry; }
	
	/**
	 * Texture units configuration for shadow type shaders or NULL if empty.
	 * Works for these shader types:
	 * - estComponentShadowProjection
	 * - estComponentShadowOrthogonal
	 * - estComponentShadowOrthogonalCascaded
	 * - estComponentShadowDistance
	 */
	inline deoglTexUnitsConfig *GetTUCShadow() const{ return pTUCShadow; }
	
	/**
	 * Texture units configuration for the environment map shader or NULL if empty.
	 * Works for these shader types:
	 * - estEnvMap.
	 */
	inline deoglTexUnitsConfig *GetTUCEnvMap() const{ return pTUCEnvMap; }
	
	/**
	 * Texture units configuration for luminance type shaders or NULL if empty.
	 * Works for these shader types:
	 * - estHeightMapLuminance.
	 */
	inline deoglTexUnitsConfig *GetTUCLuminance() const{ return pTUCLuminance; }
	
	/** Obtain texture units configuration for a shader type. Bare call not to be used directly. */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexturePipelines::eTypes type ) const;
	
	/** Update instance parameter shader parameter block. */
	void UpdateInstanceParamBlock( deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader );
	
	/** Prepare for render. */
	void PrepareForRender();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pPrepareParamBlock();
	void pPrepareTUCs();
};

#endif
