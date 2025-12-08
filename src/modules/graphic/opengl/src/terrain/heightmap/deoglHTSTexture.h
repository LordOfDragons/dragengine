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

#ifndef _DEOGLHTSTEXTURE_H_
#define _DEOGLHTSTEXTURE_H_

#include "../../skin/deoglSkinTexture.h"

#include <dragengine/common/math/decMath.h>

class deoglRHTSector;
class deoglRSkin;
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
	
	deoglRSkin::Ref pSkin;
	
	deoglSkinTexture *pUseSkinTexture;
	
	deoglSPBlockUBO::Ref pParamBlock;
	
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
	deoglHTSTexture(deoglRHTSector &sector, int index);
	
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
	void SetMatrix(const decTexMatrix &matrix);
	
	/** Skin or NULL. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin or NULL. */
	void SetSkin(deoglRSkin::Ref skin);
	
	/** Skin texture to use or NULL. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	/** Shader parameter block. */
	inline const deoglSPBlockUBO::Ref &GetParamBlock() const{ return pParamBlock; }
	
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** Mark texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** Texture units configuration for shader type. */
	deoglTexUnitsConfig *GetTUCForPipelineType (deoglSkinTexturePipelines::eTypes type) const;
	
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
	deoglTexUnitsConfig *BareGetTUCFor(deoglSkinTexturePipelines::eTypes type) const;
	
	/** Update instance parameter shader parameter block. */
	void UpdateInstanceParamBlock(deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader);
	
	/** Prepare for render. */
	void PrepareForRender();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pPrepareParamBlock();
	void pPrepareTUCs();
};

#endif
