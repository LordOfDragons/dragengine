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

#ifndef _DEOGLRCOMPONENTCOMPONENT_H_
#define _DEOGLRCOMPONENTCOMPONENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

#include "../skin/deoglSkinTexture.h"

class deoglRComponent;
class deoglRDynamicSkin;
class deoglRSkin;
class deoglShaderParameterBlock;
class deoglShaderProgram;
class deoglSharedSPBElement;
class deoglSharedSPBRTIGroup;
class deoglSharedVideoPlayerList;
class deoglSkinShader;
class deoglSkinState;
class deoglSkinTexture;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;
class deoglComponent;
class deoglRenderPlanMasked;


/**
 * Render component texture.
 */
class deoglRComponentTexture : public deObject{
private:
	deoglRComponent &pComponent;
	const int pIndex;
	
	decTexMatrix2 pTransform;
	
	deoglRSkin *pSkin;
	deoglRDynamicSkin *pDynamicSkin;
	deoglSkinState *pSkinState;
	
	deoglRSkin *pUseSkin;
	int pUseTextureNumber;
	deoglSkinTexture *pUseSkinTexture;
	deoglSkinState *pUseSkinState;
	deoglRDynamicSkin *pUseDynamicSkin;
	bool pUseDoubleSided;
	bool pUseDecal;
	bool pIsRendered;
	int pRenderTaskFilters;
	
	deoglSharedSPBElement *pSharedSPBElement;
	decObjectList pSharedSPBRTIGroup;
	
	deoglTexUnitsConfig *pTUCDepth;
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCCounter;
	deoglTexUnitsConfig *pTUCShadow;
	deoglTexUnitsConfig *pTUCShadowCube;
	deoglTexUnitsConfig *pTUCEnvMap;
	deoglTexUnitsConfig *pTUCOutlineDepth;
	deoglTexUnitsConfig *pTUCOutlineGeometry;
	deoglTexUnitsConfig *pTUCOutlineCounter;
	deoglTexUnitsConfig *pTUCLuminance;
	deoglTexUnitsConfig *pTUCGIMaterial;
	
	bool pValidParamBlocks;
	bool pDirtyParamBlocks;
	bool pDirtyTUCs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component texture. */
	deoglRComponentTexture( deoglRComponent &component, int index );
	
	/** Clean up component texture. */
	virtual ~deoglRComponentTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent component. */
	inline deoglRComponent &GetComponent() const{ return pComponent; }
	
	/** Texture index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** Set texture coordinate transformation matrix. */
	void SetTransform( const decTexMatrix2 &matrix );
	
	
	
	/** Skin or NULL if there is none. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin or NULL if there is none. */
	void SetSkin( deoglRSkin *skin );
	
	/** Dynamic skin or NULL if there is none. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Set dynamic skin or NULL if there is none. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** Skin state or NULL if there is none. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/**
	 * Set skin state or NULL if there is none.
	 * \warning Only call from main thread during synchronization.
	 */
	void SetSkinState( deoglSkinState *skinState );
	
	/**
	 * Update skin state depending on skin and dynamic skin.
	 * \warning Only call from main thread during synchronization.
	 */
	void UpdateSkinState( deoglComponent &component );
	
	/** Skin to use. */
	inline deoglRSkin *GetUseSkin() const{ return pUseSkin; }
	
	/** Skin texture number to use. */
	inline int GetUseTextureNumber() const{ return pUseTextureNumber; }
	
	/** Skin texture to use. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	/** Skin state to use. */
	inline deoglSkinState *GetUseSkinState() const{ return pUseSkinState; }
	
	/** Dynamic skin to use. */
	inline deoglRDynamicSkin *GetUseDynamicSkin() const{ return pUseDynamicSkin; }
	
	/** Texture to use is double sided. */
	inline bool GetUseDoubleSided() const{ return pUseDoubleSided; }
	
	/** Texture has to be rendered as decal. */
	inline bool GetUseDecal() const{ return pUseDecal; }
	
	/** Render color content is rendered instead of texture mapped. */
	inline bool GetIsRendered() const{ return pIsRendered; }
	
	/** Render task filters. */
	inline int GetRenderTaskFilters() const{ return pRenderTaskFilters; }
	
	/** Updates the actual texture parameters to use. */
	void UpdateUseSkin();
	
	/** Calculate instance texture coordinate transformation matrix. */
	decTexMatrix2 CalcTexCoordMatrix() const;
	
	
	
	/** Prepare parameter blocks. */
	void PrepareParamBlocks();
	
	/** Shared shader parameter block element. */
	inline deoglSharedSPBElement *GetSharedSPBElement() const{ return pSharedSPBElement; }
	
	/** Shared SPB render task instance group. */
	deoglSharedSPBRTIGroup &GetSharedSPBRTIGroup( int lodLevel ) const;
	
	/** Update render target shared instances. */
	void UpdateRTSInstances();
	
	/** Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType ) const;
	
	/**
	 * Texture units configuration for depth type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estComponentDepth
	 * - deoglSkinTexture::estComponentDepthClipPlane
	 */
	inline deoglTexUnitsConfig *GetTUCDepth() const{ return pTUCDepth; }
	
	/**
	 * Texture units configuration for geometry type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estComponentGeometry
	 */
	inline deoglTexUnitsConfig *GetTUCGeometry() const{ return pTUCGeometry; }
	
	/**
	 * Texture units configuration for counter type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estComponentCounter
	 * - deoglSkinTexture::estComponentCounterClipPlane
	 */
	inline deoglTexUnitsConfig *GetTUCCounter() const{ return pTUCCounter; }
	
	/**
	 * Texture units configuration for shadow type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types
	 * - deoglSkinTexture::estComponentShadowProjection
	 * - deoglSkinTexture::estComponentShadowOrthogonal
	 * - deoglSkinTexture::estComponentShadowOrthogonalCascaded
	 * - deoglSkinTexture::estComponentShadowDistance
	 */
	inline deoglTexUnitsConfig *GetTUCShadow() const{ return pTUCShadow; }
	
	/**
	 * Texture units configuration for shadow cube type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types
	 * - deoglSkinTexture::estComponentShadowDistanceCube
	 */
	inline deoglTexUnitsConfig *GetTUCShadowCube() const{ return pTUCShadowCube; }
	
	/**
	 * Texture units configuration for the environment map shader or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estEnvMap
	 */
	inline deoglTexUnitsConfig *GetTUCEnvMap() const{ return pTUCEnvMap; }
	
	/**
	 * Texture units configuration for outline geometry type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estOutlineGeometry
	 */
	inline deoglTexUnitsConfig *GetTUCOutlineGeometry() const{ return pTUCOutlineGeometry; }
	
	/**
	 * Texture units configuration for outline depth type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estOutlineDepth
	 * - deoglSkinTexture::estOutlineDepthClipPlane
	 */
	inline deoglTexUnitsConfig *GetTUCOutlineDepth() const{ return pTUCOutlineDepth; }
	
	/**
	 * Texture units configuration for outline counter type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estOutlineCounter
	 * - deoglSkinTexture::estOutlineCounterClipPlane
	 */
	inline deoglTexUnitsConfig *GetTUCOutlineCounter() const{ return pTUCOutlineCounter; }
	
	/**
	 * Texture units configuration for luminance type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estComponentLuminance
	 */
	inline deoglTexUnitsConfig *GetTUCLuminance() const{ return pTUCLuminance; }
	
	/**
	 * Texture units configuration for GI material type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estComponentGIMaterial
	 */
	inline deoglTexUnitsConfig *GetTUCGIMaterial() const{ return pTUCGIMaterial; }
	
	/**
	 * Obtain texture units configuration for a shader type. Bare call not to be used directly.
	 */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ) const;
	
	/** Prepare TUCs. */
	void PrepareTUCs();
	
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/**
	 * Update instance parameter shader parameter block.
	 * 
	 * Parameter block has to be mapped while calling this method.
	 * Caller is responsible to properly unmap in case of exceptions.
	 */
	void UpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock,
		int element, const deoglSkinShader &skinShader );
	
	/** Prepare skin state renderables if dirty. */
	void PrepareSkinStateRenderables( const deoglRenderPlanMasked *renderPlanMask );
	/*@}*/
	
	
	
private:
	void pUpdateIsRendered();
	void pUpdateRenderTaskFilters();
};

#endif
