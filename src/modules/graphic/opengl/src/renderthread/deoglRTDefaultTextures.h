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

#ifndef _DEOGLRRTDEFAULTTEXTURES_H_
#define _DEOGLRRTDEFAULTTEXTURES_H_

class deoglArrayTexture;
class deoglCubeMap;
class deoglRenderThread;
class deoglTexture;



/**
 * Render thread default textures.
 */
class deoglRTDefaultTextures{
private:
	deoglTexture *pColor;
	deoglTexture *pTransparency;
	deoglTexture *pNormal;
	deoglTexture *pHeight;
	deoglTexture *pReflectivity;
	deoglTexture *pRoughness;
	deoglTexture *pEmissivity;
	deoglTexture *pAO;
	deoglTexture *pSolidity;
	deoglTexture *pEnvRoomMask;
	deoglTexture *pShadowMap;
	deoglTexture *pShadowMapInverseDepth;
	deoglTexture *pShadowMapColor;
	deoglCubeMap *pShadowCube;
	deoglCubeMap *pShadowCubeColor;
	
	deoglTexture *pWeights;
	deoglTexture *pMaskOpaque;
	deoglTexture *pNoise2D;
	deoglCubeMap *pEnvMap;
	
	deoglArrayTexture *pColorArray;
	deoglArrayTexture *pTransparencyArray;
	deoglArrayTexture *pNormalArray;
	deoglArrayTexture *pHeightArray;
	deoglArrayTexture *pReflectivityArray;
	deoglArrayTexture *pRoughnessArray;
	deoglArrayTexture *pEmissivityArray;
	deoglArrayTexture *pAOArray;
	deoglArrayTexture *pSolidityArray;
	deoglArrayTexture *pEnvRoomMaskArray;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread default textures. */
	deoglRTDefaultTextures( deoglRenderThread &renderThread );
	
	/** Clean up render thread default textures. */
	~deoglRTDefaultTextures();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Default color channel texture. */
	inline deoglTexture *GetColor() const{ return pColor; }
	
	/** Default transparency channel texture. */
	inline deoglTexture *GetTransparency() const{ return pTransparency; }
	
	/** Default normal channel texture. */
	inline deoglTexture *GetNormal() const{ return pNormal; }
	
	/** Default height channel texture. */
	inline deoglTexture *GetHeight() const{ return pHeight; }
	
	/** Default reflectivity channel texture. */
	inline deoglTexture *GetReflectivity() const{ return pReflectivity; }
	
	/** Default roughness channel texture. */
	inline deoglTexture *GetRoughness() const{ return pRoughness; }
	
	/** Default emissivity channel texture. */
	inline deoglTexture *GetEmissivity() const{ return pEmissivity; }
	
	/** Default solidity channel texture. */
	inline deoglTexture *GetSolidity() const{ return pSolidity; }
	
	/** Default ambient occlusion channel texture. */
	inline deoglTexture *GetAO() const{ return pAO; }
	
	/** Default environment room mask texture. */
	inline deoglTexture *GetEnvRoomMask() const{ return pEnvRoomMask; }
	
	/** Default shadow map. */
	inline deoglTexture *GetShadowMap() const{ return pShadowMap; }
	
	/** Default inverse depth shadow map. */
	inline deoglTexture *GetShadowMapInverseDepth() const{ return pShadowMapInverseDepth; }
	
	/** Default shadow map color. */
	inline deoglTexture *GetShadowMapColor() const{ return pShadowMapColor; }
	
	/** Default shadow cube. */
	inline deoglCubeMap *GetShadowCube() const{ return pShadowCube; }
	
	/** Default shadow cube color. */
	inline deoglCubeMap *GetShadowCubeColor() const{ return pShadowCubeColor; }
	
	/** Default envirnoment map. */
	inline deoglCubeMap *GetEnvMap() const{ return pEnvMap; }
	
	
	
	/** Default opaque mask texture. */
	inline deoglTexture *GetMaskOpaque() const{ return pMaskOpaque; }
	
	/** Default 2d noise texture. */
	inline deoglTexture *GetNoise2D() const{ return pNoise2D; }
	
	/** Default weights texture. */
	inline deoglTexture *GetWeightsTexture() const{ return pWeights; }
	
	
	
	/** Default color channel array texture. */
	inline deoglArrayTexture *GetColorArray() const{ return pColorArray; }
	
	/** Default transparency channel array texture. */
	inline deoglArrayTexture *GetTransparencyArray() const{ return pTransparencyArray; }
	
	/** Default normal channel array texture. */
	inline deoglArrayTexture *GetNormalArray() const{ return pNormalArray; }
	
	/** Default height channel array texture. */
	inline deoglArrayTexture *GetHeightArray() const{ return pHeightArray; }
	
	/** Default reflectivity channel array texture. */
	inline deoglArrayTexture *GetReflectivityArray() const{ return pReflectivityArray; }
	
	/** Default roughness channel array texture. */
	inline deoglArrayTexture *GetRoughnessArray() const{ return pRoughnessArray; }
	
	/** Default emissivity channel array texture. */
	inline deoglArrayTexture *GetEmissivityArray() const{ return pEmissivityArray; }
	
	/** Default solidity channel array texture. */
	inline deoglArrayTexture *GetSolidityArray() const{ return pSolidityArray; }
	
	/** Default ambient occlusion channel array texture. */
	inline deoglArrayTexture *GetAOArray() const{ return pAOArray; }
	
	/** Default environment room mask array texture. */
	inline deoglArrayTexture *GetEnvRoomMaskArray() const{ return pEnvRoomMaskArray; }
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateDefaultTextures( deoglRenderThread &renderThread );
	void pCreateDefaultTexturesArray( deoglRenderThread &renderThread );
	void pCreateWeightsTexture( deoglRenderThread &renderThread );
	void pCreateTextureMaskOpaque( deoglRenderThread &renderThread );
	void pCreateTextureNoise2D( deoglRenderThread &renderThread );
	void pCreateShadowTextures( deoglRenderThread &renderThread );
};

#endif
