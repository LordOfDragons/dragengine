/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOGLRCOMPONENTCOMPONENT_H_
#define _DEOGLRCOMPONENTCOMPONENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

#include "../skin/deoglSkinTexture.h"
#include "../skin/pipeline/deoglSkinTexturePipelines.h"

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
	
	deoglRSkin::Ref pSkin;
	deoglRDynamicSkin::Ref pDynamicSkin;
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
	
	deoglSharedSPBElement::Ref pSharedSPBElement;
	decObjectList pSharedSPBRTIGroup;
	decObjectList pSharedSPBRTIGroupShadow;
	
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
	bool pDirtyTUCsAll;
	bool pDirtyTUCsEnvMapUse;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRComponentTexture> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component texture. */
	deoglRComponentTexture(deoglRComponent &component, int index);
	
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
	void SetTransform(const decTexMatrix2 &matrix);
	
	
	
	/** Skin or NULL if there is none. */
	inline const deoglRSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** Set skin or NULL if there is none. */
	void SetSkin(deoglRSkin *skin);
	
	/** Dynamic skin or NULL if there is none. */
	inline const deoglRDynamicSkin::Ref &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Set dynamic skin or NULL if there is none. */
	void SetDynamicSkin(deoglRDynamicSkin *dynamicSkin);
	
	/** Skin state or NULL if there is none. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/**
	 * Set skin state or NULL if there is none.
	 * \warning Only call from main thread during synchronization.
	 */
	void SetSkinState(deoglSkinState *skinState);
	
	/**
	 * Update skin state depending on skin and dynamic skin.
	 * \warning Only call from main thread during synchronization.
	 */
	void UpdateSkinState(deoglComponent &component);
	
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
	inline const deoglSharedSPBElement::Ref &GetSharedSPBElement() const{ return pSharedSPBElement; }
	
	/** Shared SPB render task instance group. */
	deoglSharedSPBRTIGroup &GetSharedSPBRTIGroup(int lodLevel) const;
	
	/** Shadow shared SPB render task instance group or NULL. */
	deoglSharedSPBRTIGroup *GetSharedSPBRTIGroupShadow(int lodLevel) const;
	
	/** Update render target shared instances. */
	void UpdateRTSInstances();
	
	/** Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForPipelineType(deoglSkinTexturePipelines::eTypes shaderType) const;
	deoglTexUnitsConfig *GetTUCForOutlinePipelineType(deoglSkinTexturePipelines::eTypes shaderType) const;
	
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
	deoglTexUnitsConfig *BareGetTUCFor(deoglSkinTexturePipelines::eTypes shaderType) const;
	deoglTexUnitsConfig *BareGetOutlineTUCFor(deoglSkinTexturePipelines::eTypes shaderType) const;
	
	/** Prepare TUCs. */
	void PrepareTUCs();
	
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** Marks env map using texture units configurations dirty. */
	void MarkTUCsDirtyEnvMapUse();
	
	/**
	 * Update instance parameter shader parameter block.
	 * 
	 * Parameter block has to be mapped while calling this method.
	 * Caller is responsible to properly unmap in case of exceptions.
	 */
	void UpdateInstanceParamBlock(deoglShaderParameterBlock &paramBlock,
		int element, const deoglSkinShader &skinShader);
	
	/** Prepare skin state renderables if dirty. */
	void PrepareSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask);
	
	/** Render skin state renderables if dirty. */
	void RenderSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask);
	
	/** Prepare skin state constructed. */
	void PrepareSkinStateConstructed();
	
	void UpdateRenderableMapping();
	/*@}*/
	
	
	
private:
	void pUpdateIsRendered();
	void pUpdateRenderTaskFilters();
	int pShadowCombineCount(int lodLevel) const;
};

#endif
