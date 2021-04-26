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
 * @brief Height Terrain Sector Texture.
 */
class deoglHTSTexture{
private:
	deoglRHTSector &pSector;
	int pIndex;
	
	decTexMatrix pMatrix;
	
	deoglRSkin *pSkin;
	
	deoglSkinTexture *pUseSkinTexture;
	
	deoglSPBlockUBO *pParamBlockDepth;
	deoglSPBlockUBO *pParamBlockGeometry;
	deoglSPBlockUBO *pParamBlockEnvMap;
	
	deoglTexUnitsConfig *pTUCDepth;
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCShadow;
	deoglTexUnitsConfig *pTUCEnvMap;
	deoglTexUnitsConfig *pTUCLuminance;
	
	bool pValidParamBlockDepth;
	bool pValidParamBlockGeometry;
	bool pValidParamBlockEnvMap;
	bool pDirtyParamBlockDepth;
	bool pDirtyParamBlockGeometry;
	bool pDirtyParamBlockEnvMap;
	
	bool pDirtyTUCDepth;
	bool pDirtyTUCGeometry;
	bool pDirtyTUCShadow;
	bool pDirtyTUCEnvMap;
	bool pDirtyTUCLuminance;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new height terrain sector texture. */
	deoglHTSTexture( deoglRHTSector &sector, int index );
	/** Cleans up the height terrain sector texture. */
	~deoglHTSTexture();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Height terrain sector. */
	inline deoglRHTSector &GetHTSector() const{ return pSector; }
	
	/** Retrieves the texture index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Retrieves the texture matrix. */
	inline const decTexMatrix &GetMatrix() const{ return pMatrix; }
	/** Sets the texture matrix. */
	void SetMatrix( const decTexMatrix &matrix );
	
	/** Retrieves the opengl skin or NULL if not set. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	/** Sets the skin or NULL if not set. */
	void SetSkin( deoglRSkin *skin );
	
	/** Retrieves the skin texture to use or NULL if not valid. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	/** Retrieves the shader parameter block for a shader type. */
	deoglSPBlockUBO *GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType );
	/**
	 * Retrieves the depth shader parameter block or NULL if there is no valid skin texture.
	 * This texture units configuration works for these shader types:
	 * - estComponentDepth
	 * - estComponentDepthClipPlane
	 * - estComponentCounter
	 * - estComponentCounterClipPlane
	 * - estComponentShadowProjection
	 * - estComponentShadowOrthogonal
	 * - estComponentShadowOrthogonalCascaded
	 * - estComponentShadowDistance
	 */
	deoglSPBlockUBO *GetParamBlockDepth();
	/**
	 * Retrieves the geometry shader parameter block or NULL if there is no valid skin texture.
	 * This texture units configuration works for the shader type estComponentGeometry.
	 */
	deoglSPBlockUBO *GetParamBlockGeometry();
	/**
	 * Retrieves the environment map shader parameter block or NULL if there is no valid skin texture.
	 * This texture units configuration works for the shader type estEnvMap.
	 */
	deoglSPBlockUBO *GetParamBlockEnvMap();
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** Retrieves the texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType );
	/**
	 * Retrieves the texture units configuration for depth type shaders or NULL if empty.
	 * This texture units configuration works for the shader types estComponentDepth, estComponentDepthClipPlane,
	 * estComponentCounter and estComponentCounterClipPlane.
	 */
	deoglTexUnitsConfig *GetTUCDepth();
	/**
	 * Retrieves the texture units configuration for geometry type shaders or NULL if empty.
	 * This texture units configuration works for the shader type estComponentGeometry.
	 */
	deoglTexUnitsConfig *GetTUCGeometry();
	/**
	 * Retrieves the texture units configuration for shadow type shaders or NULL if empty.
	 * This texture units configuration works for these shader types:
	 * - estComponentShadowProjection
	 * - estComponentShadowOrthogonal
	 * - estComponentShadowOrthogonalCascaded
	 * - estComponentShadowDistance
	 */
	deoglTexUnitsConfig *GetTUCShadow();
	/**
	 * Retrieves the texture units configuration for the environment map shader or NULL if empty.
	 * This texture units configuration works for the shader type estEnvMap.
	 */
	deoglTexUnitsConfig *GetTUCEnvMap();
	
	/**
	 * Texture units configuration for luminance type shaders or NULL if empty.
	 * Works for the shader type estHeightMapLuminance.
	 */
	deoglTexUnitsConfig *GetTUCLuminance();
	
	/** Obtain texture units configuration for a shader type. Bare call not to be used directly. */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ) const;
	
	/** Update instance parameter shader parameter block. */
	void UpdateInstanceParamBlock( deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
