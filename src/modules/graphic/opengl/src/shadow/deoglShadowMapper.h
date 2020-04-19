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
class deoglRenderableArrayTexture;



/**
 * \brief Shadow mapper.
 *
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
	deoglRenderableArrayTexture *pArrTexSolidDepth;
	deoglRenderableArrayTexture *pArrTexTranspDepth;
	deoglRenderableArrayTexture *pArrTexTranspColor;
	deoglArrayTexture *pForeignArrTexSolidDepth;
	deoglArrayTexture *pForeignArrTexTranspDepth;
	deoglArrayTexture *pForeignArrTexTranspColor;
	deoglArrayTexture *pUseArrTexSolidDepth;
	deoglArrayTexture *pUseArrTexTranspDepth;
	deoglArrayTexture *pUseArrTexTranspColor;
	
	deoglFramebuffer *pFBOOcclusion;
	deoglRenderableColorTexture *pTextureOcclusion;
	
	deoglFramebuffer *pFBOAmbient;
	deoglRenderableColorTexture *pTextureAmbient;
	deoglTexture *pForeignTexAmbient;
	deoglTexture *pUseTexAmbient;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shadow mapper. */
	deoglShadowMapper( deoglRenderThread &renderThread );
	
	/** \brief Clean up shadow mapper. */
	~deoglShadowMapper();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Solid depth texture. */
	deoglTexture *GetSolidDepthTexture() const;
	
	/** \brief Set foreign solid depth texture. */
	void SetForeignSolidDepthTexture( deoglTexture *texture );
	
	/** \brief Transparent depth texture. */
	deoglTexture *GetTransparentDepthTexture() const;
	
	/** \brief Set foreign transparent depth texture. */
	void SetForeignTransparentDepthTexture( deoglTexture *texture );
	
	/** \brief Transparent color texture. */
	deoglTexture *GetTransparentColorTexture() const;
	
	/** \brief Set foreign transparent color texture. */
	void SetForeignTransparentColorTexture( deoglTexture *texture );
	
	/** \brief Drop shadow textures. */
	void DropTextures();
	
	/** \brief Drop solid shadow textures. */
	void DropTexturesSolid();
	
	/** \brief Drop transparent shadow textures. */
	void DropTexturesTransparent();
	
	/** \brief Drop foreign shadow textures. */
	void DropForeignTextures();
	
	/** \brief Activate shadow texture (depth only) with the given size for solid shadow casting. */
	void ActivateSolidTexture( int size, bool useFloatDepth );
	
	/** \brief Activate shadow texture (depth and color) with the given size for transparent shadow casting. */
	void ActivateTransparentTexture( int size, bool useFloatDepth );
	
	
	
	/** \brief Solid depth cubemap. */
	deoglCubeMap *GetSolidDepthCubeMap() const;
	
	/** \brief Solid encoded depth cubemap. */
	deoglCubeMap *GetSolidEncodedDepthCubeMap() const;
	
	/** \brief Set foreign solid depth cubemap. */
	void SetForeignSolidDepthCubeMap( deoglCubeMap *cubemap );
	
	/** \brief Transparent depth cubemap. */
	deoglCubeMap *GetTransparentDepthCubeMap() const;
	
	/** \brief Transparent encoded depth cubemap. */
	deoglCubeMap *GetTransparentEncodedDepthCubeMap() const;
	
	/** \brief Set foreign transparent encoded depth cubemap. */
	void SetForeignTransparentDepthCubeMap( deoglCubeMap *cubemap );
	
	/** \brief Transparent color cubemap. */
	deoglCubeMap *GetTransparentColorCubeMap() const;
	
	/** \brief Set foreign transparent color cubemap. */
	void SetForeignTransparentColorCubeMap( deoglCubeMap *cubemap );
	
	/** \brief Drop shadow cubemaps. */
	void DropCubeMaps();
	
	/** \brief Drop foreign shadow cubemaps. */
	void DropForeignCubeMaps();
	
	/** \brief Activate shadow cubemap (depth only) with the given size for solid shadow casting. */
	void ActivateSolidCubeMap( int size );
	
	/** \brief Activate shadow cubemap face (depth only) with the given size for solid shadow casting. */
	void ActivateSolidCubeMapFace( int size, int face );
	
	/** \brief Activate shadow cubemap (depth and color) with the given size for transparent shadow casting. */
	void ActivateTransparentCubeMap( int size );
	
	/** \brief Activate shadow cubemap face (depth and color) with the given size for transparent shadow casting. */
	void ActivateTransparentCubeMapFace( int size, int face );
	
	
	
	/** \brief Copy shadow map with the given size into the shadow cube face with the given size. */
	void CopyShadowMapToShadowCube( int size, int face );
	
	
	
	/** \brief Solid depth array texture. */
	deoglArrayTexture *GetSolidDepthArrayTexture() const;
	
	/** \brief Set foreign solid depth array texture. */
	void SetForeignSolidDepthArrayTexture( deoglArrayTexture *texture );
	
	/** \brief Transparent depth array texture. */
	deoglArrayTexture *GetTransparentDepthArrayTexture() const;
	
	/** \brief Set foreign transparent depth array texture. */
	void SetForeignTransparentDepthArrayTexture( deoglArrayTexture *texture );
	
	/** \brief Transparent color array texture. */
	deoglArrayTexture *GetTransparentColorArrayTexture() const;
	
	/** \brief Set foreign transparent color array texture. */
	void SetForeignTransparentColorArrayTexture( deoglArrayTexture *texture );
	
	/** \brief Drop array textures. */
	void DropArrayTextures();
	
	/** \brief Drop foreign array textures. */
	void DropForeignArrayTextures();
	
	/** \brief Activate shadow array texture (depth) with the given size for solid shadow casting. */
	void ActivateSolidArrayTexture( int size, int layerCount );
	
	/** \brief Activate layer of a shadow array texture (depth) with the given size for solid shadow casting. */
	void ActivateSolidArrayTextureLayer( int size, int layerCount, int layer );
	
	/** \brief Activate shadow array texture (depth) with the given size for transparent shadow casting. */
	void ActivateTransparentArrayTexture( int size, int layerCount );
	
	
	
	/** \brief Occlusion texture. */
	deoglTexture *GetOcclusionTexture() const;
	
	/** \brief Activate occlusion texture with the given size. */
	void ActivateOcclusionTexture( int width, int height );
	
	/** \brief Drop occlusion textures. */
	void DropOcclusionTextures();
	
	
	
	/** \brief Ambient texture. */
	deoglTexture *GetAmbientTexture() const;
	
	/** \brief Set foreign ambient texture. */
	void SetForeignAmbientTexture( deoglTexture *texture );
	
	/** \brief Drop ambient textures. */
	void DropAmbientTextures();
	
	/** \brief Drop foreign ambient textures. */
	void DropForeignAmbientTextures();
	
	/** \brief Activate ambient texture with the given size. */
	void ActivateAmbientTexture( int size );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
