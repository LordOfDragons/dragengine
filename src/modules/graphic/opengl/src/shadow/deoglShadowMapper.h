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

#ifndef _DEOGLSHADOWMAPPER_H_
#define _DEOGLSHADOWMAPPER_H_

#include <dragengine/common/math/decMath.h>
#include "../deoglBasics.h"

class deoglRenderThread;
class deoglTexture;
class deoglCubeMap;
class deoglArrayTexture;
class deoglFramebuffer;
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
	
	deoglFramebuffer *pFBOTextureSolid;
	deoglFramebuffer *pFBOTextureTransp;
	deoglRenderableDepthTexture *pTextureDepthSolid;
	deoglRenderableDepthTexture *pTextureDepthTransp;
	deoglRenderableColorTexture *pTextureColorTransp;
	deoglTexture *pForeignTexDepthSolid;
	deoglTexture *pForeignTexDepthTransp;
	deoglTexture *pForeignTexColorTransp;
	deoglTexture *pUseTexDepthSolid;
	deoglTexture *pUseTexDepthTransp;
	deoglTexture *pUseTexColorTransp;
	
	deoglFramebuffer *pFBOCube;
	deoglRenderableDepthCubeMap *pCubeMapDepthSolid;
	deoglRenderableColorCubeMap *pCubeMapEncodedDepthSolid;
	deoglRenderableDepthCubeMap *pCubeMapDepthTransp;
	deoglRenderableColorCubeMap *pCubeMapEncodedDepthTransp;
	deoglRenderableColorCubeMap *pCubeMapColorTransp;
	deoglCubeMap *pForeignCubeMapDepthSolid;
	deoglCubeMap *pForeignCubeMapDepthTransp;
	deoglCubeMap *pForeignCubeMapColorTransp;
	deoglCubeMap *pUseCubeMapDepthSolid;
	deoglCubeMap *pUseCubeMapDepthTransp;
	deoglCubeMap *pUseCubeMapColorTransp;
	
	deoglFramebuffer *pFBOArrTex;
	deoglRenderableDepthArrayTexture *pArrTexSolidDepth;
	deoglRenderableDepthArrayTexture *pArrTexTranspDepth;
	deoglRenderableColorArrayTexture *pArrTexTranspColor;
	deoglArrayTexture *pForeignArrTexSolidDepth;
	deoglArrayTexture *pForeignArrTexTranspDepth;
	deoglArrayTexture *pForeignArrTexTranspColor;
	deoglArrayTexture *pUseArrTexSolidDepth;
	deoglArrayTexture *pUseArrTexTranspDepth;
	deoglArrayTexture *pUseArrTexTranspColor;
	
	deoglFramebuffer *pFBOOcclusion;
	deoglRenderableColorTexture *pTextureOcclusion;
	
	deoglFramebuffer *pFBOAmbient;
	deoglRenderableDepthTexture *pTextureAmbient;
	deoglTexture *pForeignTexAmbient;
	deoglTexture *pUseTexAmbient;
	
	deoglFramebuffer *pFBOCubeAmbient;
	deoglRenderableDepthCubeMap *pCubeMapAmbient;
	deoglCubeMap *pForeignCubeMapAmbient;
	deoglCubeMap *pUseCubeMapAmbient;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shadow mapper. */
	deoglShadowMapper( deoglRenderThread &renderThread );
	
	/** Clean up shadow mapper. */
	~deoglShadowMapper();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Solid depth texture. */
	deoglTexture *GetSolidDepthTexture() const;
	
	/** Set foreign solid depth texture. */
	void SetForeignSolidDepthTexture( deoglTexture *texture );
	
	/** Transparent depth texture. */
	deoglTexture *GetTransparentDepthTexture() const;
	
	/** Set foreign transparent depth texture. */
	void SetForeignTransparentDepthTexture( deoglTexture *texture );
	
	/** Transparent color texture. */
	deoglTexture *GetTransparentColorTexture() const;
	
	/** Set foreign transparent color texture. */
	void SetForeignTransparentColorTexture( deoglTexture *texture );
	
	/** Drop shadow textures. */
	void DropTextures();
	
	/** Drop solid shadow textures. */
	void DropTexturesSolid();
	
	/** Drop transparent shadow textures. */
	void DropTexturesTransparent();
	
	/** Drop foreign shadow textures. */
	void DropForeignTextures();
	
	/** Activate shadow texture (depth only) with the given size for solid shadow casting. */
	void ActivateSolidTexture( int size, bool useFloatDepth, bool withStencil = false );
	
	/** Activate shadow texture (depth and color) with the given size for transparent shadow casting. */
	void ActivateTransparentTexture( int size, bool useFloatDepth );
	
	
	
	/** Solid depth cubemap. */
	deoglCubeMap *GetSolidDepthCubeMap() const;
	
	/** Solid encoded depth cubemap. */
	deoglCubeMap *GetSolidEncodedDepthCubeMap() const;
	
	/** Set foreign solid depth cubemap. */
	void SetForeignSolidDepthCubeMap( deoglCubeMap *cubemap );
	
	/** Transparent depth cubemap. */
	deoglCubeMap *GetTransparentDepthCubeMap() const;
	
	/** Transparent encoded depth cubemap. */
	deoglCubeMap *GetTransparentEncodedDepthCubeMap() const;
	
	/** Set foreign transparent encoded depth cubemap. */
	void SetForeignTransparentDepthCubeMap( deoglCubeMap *cubemap );
	
	/** Transparent color cubemap. */
	deoglCubeMap *GetTransparentColorCubeMap() const;
	
	/** Set foreign transparent color cubemap. */
	void SetForeignTransparentColorCubeMap( deoglCubeMap *cubemap );
	
	/** Drop shadow cubemaps. */
	void DropCubeMaps();
	
	/** Drop solid shadow cubemaps. */
	void DropCubeMapsSolid();
	
	/** Drop transparent shadow cubemaps. */
	void DropCubeMapsTransparent();
	
	/** Drop foreign shadow cubemaps. */
	void DropForeignCubeMaps();
	
	/** Activate shadow cubemap (depth only) with the given size for solid shadow casting. */
	void ActivateSolidCubeMap( int size );
	
	/** Activate shadow cubemap face (depth only) with the given size for solid shadow casting. */
	void ActivateSolidCubeMapFace( int size, int face );
	
	/** Activate shadow cubemap (depth and color) with the given size for transparent shadow casting. */
	void ActivateTransparentCubeMap( int size );
	
	/** Activate shadow cubemap face (depth and color) with the given size for transparent shadow casting. */
	void ActivateTransparentCubeMapFace( int size, int face );
	
	
	
	/** Copy shadow map with the given size into the shadow cube face with the given size. */
	void CopyShadowMapToShadowCube( int size, int face );
	
	
	
	/** Solid depth array texture. */
	deoglArrayTexture *GetSolidDepthArrayTexture() const;
	
	/** Set foreign solid depth array texture. */
	void SetForeignSolidDepthArrayTexture( deoglArrayTexture *texture );
	
	/** Transparent depth array texture. */
	deoglArrayTexture *GetTransparentDepthArrayTexture() const;
	
	/** Set foreign transparent depth array texture. */
	void SetForeignTransparentDepthArrayTexture( deoglArrayTexture *texture );
	
	/** Transparent color array texture. */
	deoglArrayTexture *GetTransparentColorArrayTexture() const;
	
	/** Set foreign transparent color array texture. */
	void SetForeignTransparentColorArrayTexture( deoglArrayTexture *texture );
	
	/** Drop array textures. */
	void DropArrayTextures();
	
	/** Drop foreign array textures. */
	void DropForeignArrayTextures();
	
	/** Activate shadow array texture (depth) with the given size for solid shadow casting. */
	void ActivateSolidArrayTexture( int size, int layerCount, bool withStencil = false );
	
	/** Activate layer of a shadow array texture (depth) with the given size for solid shadow casting. */
	void ActivateSolidArrayTextureLayer( int size, int layerCount, int layer, bool withStencil = false );
	
	/** Activate shadow array texture (depth) with the given size for transparent shadow casting. */
	void ActivateTransparentArrayTexture( int size, int layerCount );
	
	
	
	/** Occlusion texture. */
	deoglTexture *GetOcclusionTexture() const;
	
	/** Activate occlusion texture with the given size. */
	void ActivateOcclusionTexture( int width, int height );
	
	/** Drop occlusion textures. */
	void DropOcclusionTextures();
	
	
	
	/** Ambient texture. */
	deoglTexture *GetAmbientTexture() const;
	
	/** Set foreign ambient texture. */
	void SetForeignAmbientTexture( deoglTexture *texture );
	
	/** Drop ambient textures. */
	void DropAmbientTextures();
	
	/** Drop foreign ambient textures. */
	void DropForeignAmbientTextures();
	
	/** Activate ambient texture with the given size. */
	void ActivateAmbientTexture( int size, bool useFloatDepth );
	
	
	
	
	/** Ambient cubemap. */
	deoglCubeMap *GetAmbientCubeMap() const;
	
	/** Set foreign ambient cubemap. */
	void SetForeignAmbientCubeMap( deoglCubeMap *cubemap );
	
	/** Drop ambient cubemaps. */
	void DropAmbientCubeMaps();
	
	/** Drop foreign ambient cubemaps. */
	void DropForeignAmbientCubeMaps();
	
	/** Activate ambient cubemap with size. */
	void ActivateAmbientCubeMap( int size );
	
	/** Activate ambient cubemap face with size. */
	void ActivateAmbientCubeMapFace( int size, int face );
	
	
	
	/** Shadow map size from configuration shadow quality. */
	static int ShadowMapSize( const deoglConfiguration &config );
	
	/** Shadow cube size from configuration shadow quality. */
	static int ShadowCubeSize( const deoglConfiguration &config );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
