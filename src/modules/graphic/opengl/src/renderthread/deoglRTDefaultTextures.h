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
	deoglTexture *pNonPbrMetalness;
	deoglTexture *pShadowMap;
	deoglTexture *pShadowMapInverseDepth;
	deoglTexture *pShadowMapColor;
	deoglCubeMap *pShadowCube;
	deoglCubeMap *pShadowCubeInverseDepth;
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
	deoglArrayTexture *pNonPbrMetalnessArray;
	
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
	
	inline deoglTexture *GetNonPbrMetalness() const{ return pNonPbrMetalness; }
	
	/** Default shadow map. */
	inline deoglTexture *GetShadowMap() const{ return pShadowMap; }
	
	/** Default inverse depth shadow map. */
	inline deoglTexture *GetShadowMapInverseDepth() const{ return pShadowMapInverseDepth; }
	
	/** Default shadow map color. */
	inline deoglTexture *GetShadowMapColor() const{ return pShadowMapColor; }
	
	/** Default shadow cube. */
	inline deoglCubeMap *GetShadowCube() const{ return pShadowCube; }
	
	/** Default shadow cube inverse depth. */
	inline deoglCubeMap *GetShadowCubeInverseDepth() const{ return pShadowCubeInverseDepth; }
	
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
	
	inline deoglArrayTexture *GetNonPbrMetalnessArray() const{ return pNonPbrMetalnessArray; }
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
