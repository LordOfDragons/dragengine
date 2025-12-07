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

#ifndef _DEOGLSHADOWMAPPER_H_
#define _DEOGLSHADOWMAPPER_H_

#include <dragengine/common/math/decMath.h>
#include "../deoglBasics.h"
#include "../framebuffer/deoglFramebufferManager.h"

class deoglRenderThread;
class deoglTexture;
class deoglCubeMap;
class deoglArrayTexture;
class deoglRenderableColorTexture;
class deoglRenderableDepthTexture;
class deoglRenderableColorCubeMap;
class deoglRenderableDepthCubeMap;
class deoglRenderableColorArrayTexture;
class deoglRenderableDepthArrayTexture;
class deoglConfiguration;



/**
 * Provides support for rendering shadow maps. Holds a framebuffer suitable
 * for rendering the shadow maps as well as a shadow texture and a shadow
 * cube map.
 * 
 */
class deoglShadowMapper{
private:
	deoglRenderThread &pRenderThread;
	
	deoglFramebufferManager::Usage pFBOTextureSolid;
	deoglFramebufferManager::Usage pFBOTextureTransp;
	deoglRenderableDepthTexture *pTextureDepthSolid;
	deoglRenderableDepthTexture *pTextureDepthTransp;
	deoglRenderableColorTexture *pTextureColorTransp;
	deoglTexture *pForeignTexDepthSolid;
	deoglTexture *pForeignTexDepthTransp;
	deoglTexture *pForeignTexColorTransp;
	deoglTexture *pUseTexDepthSolid;
	deoglTexture *pUseTexDepthTransp;
	deoglTexture *pUseTexColorTransp;
	
	deoglFramebufferManager::Usage pFBOCube;
	deoglRenderableDepthCubeMap *pCubeMapDepthSolid;
	deoglRenderableDepthCubeMap *pCubeMapDepthTransp;
	deoglRenderableColorCubeMap *pCubeMapColorTransp;
	deoglCubeMap *pForeignCubeMapDepthSolid;
	deoglCubeMap *pForeignCubeMapDepthTransp;
	deoglCubeMap *pForeignCubeMapColorTransp;
	deoglCubeMap *pUseCubeMapDepthSolid;
	deoglCubeMap *pUseCubeMapDepthTransp;
	deoglCubeMap *pUseCubeMapColorTransp;
	
	deoglFramebufferManager::Usage pFBOArrTex;
	deoglRenderableDepthArrayTexture *pArrTexSolidDepth;
	deoglRenderableDepthArrayTexture *pArrTexTranspDepth;
	deoglRenderableColorArrayTexture *pArrTexTranspColor;
	deoglArrayTexture *pForeignArrTexSolidDepth;
	deoglArrayTexture *pForeignArrTexTranspDepth;
	deoglArrayTexture *pForeignArrTexTranspColor;
	deoglArrayTexture *pUseArrTexSolidDepth;
	deoglArrayTexture *pUseArrTexTranspDepth;
	deoglArrayTexture *pUseArrTexTranspColor;
	
	deoglFramebufferManager::Usage pFBOOcclusion;
	deoglRenderableColorTexture *pTextureOcclusion;
	
	deoglFramebufferManager::Usage pFBOAmbient;
	deoglRenderableDepthTexture *pTextureAmbient;
	deoglTexture *pForeignTexAmbient;
	deoglTexture *pUseTexAmbient;
	
	deoglFramebufferManager::Usage pFBOCubeAmbient;
	deoglRenderableDepthCubeMap *pCubeMapAmbient;
	deoglCubeMap *pForeignCubeMapAmbient;
	deoglCubeMap *pUseCubeMapAmbient;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shadow mapper. */
	deoglShadowMapper(deoglRenderThread &renderThread);
	
	/** Clean up shadow mapper. */
	~deoglShadowMapper();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Solid depth texture. */
	deoglTexture *GetSolidDepthTexture() const;
	
	/** Set foreign solid depth texture. */
	void SetForeignSolidDepthTexture(deoglTexture *texture);
	
	/** Transparent depth texture. */
	deoglTexture *GetTransparentDepthTexture() const;
	
	/** Set foreign transparent depth texture. */
	void SetForeignTransparentDepthTexture(deoglTexture *texture);
	
	/** Transparent color texture. */
	deoglTexture *GetTransparentColorTexture() const;
	
	/** Set foreign transparent color texture. */
	void SetForeignTransparentColorTexture(deoglTexture *texture);
	
	/** Drop shadow textures. */
	void DropTextures();
	
	/** Drop solid shadow textures. */
	void DropTexturesSolid();
	
	/** Drop transparent shadow textures. */
	void DropTexturesTransparent();
	
	/** Drop foreign shadow textures. */
	void DropForeignTextures();
	
	/** Activate shadow texture (depth only) with the given size for solid shadow casting. */
	void ActivateSolidTexture(int size, bool useFloatDepth, bool withStencil = false);
	
	/** Activate shadow texture (depth and color) with the given size for transparent shadow casting. */
	void ActivateTransparentTexture(int size, bool useFloatDepth);
	
	
	
	/** Solid depth cubemap. */
	deoglCubeMap *GetSolidDepthCubeMap() const;
	
	/** Set foreign solid depth cubemap. */
	void SetForeignSolidDepthCubeMap(deoglCubeMap *cubemap);
	
	/** Transparent depth cubemap. */
	deoglCubeMap *GetTransparentDepthCubeMap() const;
	
	/** Set foreign transparent depth cubemap. */
	void SetForeignTransparentDepthCubeMap(deoglCubeMap *cubemap);
	
	/** Transparent color cubemap. */
	deoglCubeMap *GetTransparentColorCubeMap() const;
	
	/** Set foreign transparent color cubemap. */
	void SetForeignTransparentColorCubeMap(deoglCubeMap *cubemap);
	
	/** Drop shadow cubemaps. */
	void DropCubeMaps();
	
	/** Drop solid shadow cubemaps. */
	void DropCubeMapsSolid();
	
	/** Drop transparent shadow cubemaps. */
	void DropCubeMapsTransparent();
	
	/** Drop foreign shadow cubemaps. */
	void DropForeignCubeMaps();
	
	/** Activate shadow cubemap (depth only) with the given size for solid shadow casting. */
	void ActivateSolidCubeMap(int size, bool useFloatDepth);
	
	/** Activate shadow cubemap face (depth only) with the given size for solid shadow casting. */
	void ActivateSolidCubeMapFace(int size, bool useFloatDepth, int face);
	
	/** Activate shadow cubemap (depth and color) with the given size for transparent shadow casting. */
	void ActivateTransparentCubeMap(int size, bool useFloatDepth);
	
	/** Activate shadow cubemap face (depth and color) with the given size for transparent shadow casting. */
	void ActivateTransparentCubeMapFace(int size, bool useFloatDepth, int face);
	
	
	
	/** Solid depth array texture. */
	deoglArrayTexture *GetSolidDepthArrayTexture() const;
	
	/** Set foreign solid depth array texture. */
	void SetForeignSolidDepthArrayTexture(deoglArrayTexture *texture);
	
	/** Transparent depth array texture. */
	deoglArrayTexture *GetTransparentDepthArrayTexture() const;
	
	/** Set foreign transparent depth array texture. */
	void SetForeignTransparentDepthArrayTexture(deoglArrayTexture *texture);
	
	/** Transparent color array texture. */
	deoglArrayTexture *GetTransparentColorArrayTexture() const;
	
	/** Set foreign transparent color array texture. */
	void SetForeignTransparentColorArrayTexture(deoglArrayTexture *texture);
	
	/** Drop array textures. */
	void DropArrayTextures();
	
	/** Drop foreign array textures. */
	void DropForeignArrayTextures();
	
	/** Activate shadow array texture (depth) with the given size for solid shadow casting. */
	void ActivateSolidArrayTexture(int size, int layerCount, bool withStencil = false);
	
	/** Activate layer of a shadow array texture (depth) with the given size for solid shadow casting. */
	void ActivateSolidArrayTextureLayer(int size, int layerCount, int layer, bool withStencil = false);
	
	/** Activate shadow array texture (depth) with the given size for transparent shadow casting. */
	void ActivateTransparentArrayTexture(int size, int layerCount);
	
	
	
	/** Occlusion texture. */
	deoglTexture *GetOcclusionTexture() const;
	
	/** Activate occlusion texture with the given size. */
	void ActivateOcclusionTexture(int width, int height);
	
	/** Drop occlusion textures. */
	void DropOcclusionTextures();
	
	
	
	/** Ambient texture. */
	deoglTexture *GetAmbientTexture() const;
	
	/** Set foreign ambient texture. */
	void SetForeignAmbientTexture(deoglTexture *texture);
	
	/** Drop ambient textures. */
	void DropAmbientTextures();
	
	/** Drop foreign ambient textures. */
	void DropForeignAmbientTextures();
	
	/** Activate ambient texture with the given size. */
	void ActivateAmbientTexture(int size, bool useFloatDepth);
	
	
	
	
	/** Ambient cubemap. */
	deoglCubeMap *GetAmbientCubeMap() const;
	
	/** Set foreign ambient cubemap. */
	void SetForeignAmbientCubeMap(deoglCubeMap *cubemap);
	
	/** Drop ambient cubemaps. */
	void DropAmbientCubeMaps();
	
	/** Drop foreign ambient cubemaps. */
	void DropForeignAmbientCubeMaps();
	
	/** Activate ambient cubemap with size. */
	void ActivateAmbientCubeMap(int size, bool useFloatDepth);
	
	/** Activate ambient cubemap face with size. */
	void ActivateAmbientCubeMapFace(int size, bool useFloatDepth, int face);
	
	
	
	/** Shadow map size from configuration shadow quality. */
	static int ShadowMapSize(const deoglConfiguration &config);
	
	/** Shadow cube size from configuration shadow quality. */
	static int ShadowCubeSize(const deoglConfiguration &config);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
