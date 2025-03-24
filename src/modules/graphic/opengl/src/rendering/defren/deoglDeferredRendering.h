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

#ifndef _DEOGLDEFERREDRENDERING_H_
#define _DEOGLDEFERREDRENDERING_H_

#include "../../deoglBasics.h"
#include "../../memory/consumption/deoglMemoryConsumptionDeferredRenderingUse.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglFramebuffer;
class deoglDRDepthMinMax;
class deoglShaderCompiled;
class deoglSPBlockUBO;
class deoglArrayTexture;
class deoglTexture;
class deoglVAO;



/**
 * OpenGL Deferred Rendering.
 * Manages the framebuffers and render buffers required for deferred rendering.
 */
class deoglDeferredRendering{
private:
	deoglRenderThread &pRenderThread;
	
	int pWidth;
	int pHeight;
	int pLayerCount;
	int pRealWidth;
	int pRealHeight;
	int pRealLayerCount;
	float pScalingU;
	float pScalingV;
	float pPixelSizeU;
	float pPixelSizeV;
	float pFSQuadOffU;
	float pFSQuadOffV;
	float pClampU;
	float pClampV;
	
	bool pUseFadeOutRange;
	
	deoglArrayTexture *pTextureDepth1;
	deoglArrayTexture *pTextureDepth2;
	deoglArrayTexture *pTextureDepth3;
	deoglArrayTexture *pTextureDepthXRay;
	deoglArrayTexture *pTextureDiffuse;
	deoglArrayTexture *pTextureNormal;
	deoglArrayTexture *pTextureReflectivity;
	deoglArrayTexture *pTextureRoughness;
	deoglArrayTexture *pTextureAOSolidity;
	deoglArrayTexture *pTextureSubSurface;
	deoglArrayTexture *pTextureColor;
	deoglArrayTexture *pTextureLuminance;
	deoglArrayTexture *pTextureTemporary1;
	deoglArrayTexture *pTextureTemporary2;
	deoglArrayTexture *pTextureTemporary3;
	
	deoglFramebuffer *pFBOs[ 38 ];
	deoglFramebuffer **pFBOMipMapDepth1;
	deoglFramebuffer **pFBOMipMapDepth2;
	deoglFramebuffer **pFBOMipMapTemporary1;
	deoglFramebuffer **pFBOMipMapTemporary2;
	int pFBOMipMapDepthCount;
	int pFBOMipMapCount;
	bool pModeDepth;
	bool pModePostProcess;
	
	deoglFramebuffer *pFBOCopyDepth[ 8 ];
	
	deoglDRDepthMinMax *pDepthMinMax;
	
// 	deoglArrayTexture *pTextureLuminance;
// 	deoglArrayTexture *pTextureLuminanceNormal;
// 	deoglArrayTexture *pTextureLuminanceDepth;
// 	deoglFramebuffer *pFBOLuminance;
// 	deoglFramebuffer *pFBOLuminanceNormal;
	
	deoglMemoryConsumptionDeferredRenderingUse pMemUse;
	
	GLuint pVBOFullScreenQuad;
	GLuint pVBOBillboard;
	deoglVAO *pVAOFullScreenQuad;
	deoglVAO *pVAOBillboard;
	
	deoglTexture *pTexRenderDocDebug;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new opengl deferred rendering object. */
	deoglDeferredRendering( deoglRenderThread &renderThread );
	/** Cleans up the opengl deferred rendering object. */
	~deoglDeferredRendering();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Width of buffers. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height of buffers. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Buffer layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Real width of buffers. */
	inline int GetRealWidth() const{ return pRealWidth; }
	
	/** Real height of buffers. */
	inline int GetRealHeight() const{ return pRealHeight; }
	
	/** Real buffer layer count. */
	inline int GetRealLayerCount() const{ return pRealLayerCount; }
	
	/** Retrieves the scaling factor in u direction. */
	inline float GetScalingU() const{ return pScalingU; }
	/** Retrieves the scaling factor in v direction. */
	inline float GetScalingV() const{ return pScalingV; }
	/** Retrieves the texture coordinate displacement in u direction for 1 pixel. */
	inline float GetPixelSizeU() const{ return pPixelSizeU; }
	/** Retrieves the texture coordinate displacement in v direction for 1 pixel. */
	inline float GetPixelSizeV() const{ return pPixelSizeV; }
	/** Resizes the buffers. */
	void Resize( int width, int height, int layerCount = 1 );
	
	
	
	/** Fade out near render range. */
	inline bool GetUseFadeOutRange() const{ return pUseFadeOutRange; }
	
	
	
	/**
	 * Create projection matrix matching depth usage mode.
	 * \details Depending on the inverse depth mode used the projection matrix is either
	 *          infinite or non-infinite.
	 */
	decDMatrix CreateProjectionDMatrix( int width, int height, float fov, float fovRatio, float znear, float zfar ) const;
	
	/**
	 * Create frustum matrix.
	 * \details This is the same as CreateProjectionDMatrix but always creates a
	 *          non-infinite projection matrix.
	 */
	decDMatrix CreateFrustumDMatrix( int width, int height, float fov, float fovRatio, float znear, float zfar ) const;
	
	
	
	/** First depth texture. */
	deoglArrayTexture *GetDepthTexture1() const;
	
	/** Second depth texture. */
	deoglArrayTexture *GetDepthTexture2() const;
	
	/** Third depth texture. */
	inline deoglArrayTexture *GetDepthTexture3() const{ return pTextureDepth3; }
	
	/** XRay depth texture. */
	inline deoglArrayTexture *GetDepthTextureXRay() const{ return pTextureDepthXRay; }
	
	/** Swap first depth texture to second. */
	void SwapDepthTextures();
	
	/**
	 * Copy first depth texture to second using framebuffer blitting.
	 * \details After call returns active framebuffer is undefined.
	 */
	void CopyFirstDepthToSecond( bool copyDepth, bool copyStencil );
	
	/**
	 * Copy first depth texture to third depth using framebuffer blitting.
	 * \details After call returns active framebuffer is undefined.
	 */
	void CopyFirstDepthToThirdDepth( bool copyDepth, bool copyStencil );
	
	/**
	 * Copy first depth texture to XRay depth using framebuffer blitting.
	 * \details After call returns active framebuffer is undefined.
	 */
	void CopyFirstDepthToXRayDepth( bool copyDepth, bool copyStencil );
	
	/** Retrieves the diffuse texture. */
	inline deoglArrayTexture *GetTextureDiffuse() const{ return pTextureDiffuse; }
	/** Retrieves the normal texture. */
	inline deoglArrayTexture *GetTextureNormal() const{ return pTextureNormal; }
	/** Retrieves the reflectivity texture. */
	inline deoglArrayTexture *GetTextureReflectivity() const{ return pTextureReflectivity; }
	/** Retrieves the roughness texture. */
	inline deoglArrayTexture *GetTextureRoughness() const{ return pTextureRoughness; }
	/** Retrieves the ao and solidity texture. */
	inline deoglArrayTexture *GetTextureAOSolidity() const{ return pTextureAOSolidity; }
	/** Retrieves the sub-surface texture. */
	inline deoglArrayTexture *GetTextureSubSurface() const{ return pTextureSubSurface; }
	/** Retrieves the color texture. */
	inline deoglArrayTexture *GetTextureColor() const{ return pTextureColor; }
	
	/** Luminance texture. */
	inline deoglArrayTexture *GetTextureLuminance() const{ return pTextureLuminance; }
	
	/** Retrieves the first temporary texture. */
	inline deoglArrayTexture *GetTextureTemporary1() const{ return pTextureTemporary1; }
	/** Retrieves the second temporary texture. */
	inline deoglArrayTexture *GetTextureTemporary2() const{ return pTextureTemporary2; }
	/** Retrieves the third temporary texture. */
	inline deoglArrayTexture *GetTextureTemporary3() const{ return pTextureTemporary3; }
	
	/** Sets the normal as the current post processing target. */
	void InitPostProcessTarget();
	/** Swaps the diffuse and normal as current post process target. */
	void SwapPostProcessTarget();
	/** Retrieves the diffuse or normal texture depending which fbo is the current post process fbo. */
	deoglArrayTexture *GetPostProcessTexture() const;
	
	
	
	/** Activate fbo for rendering to first depth texture. */
	void ActivateFBODepth();
	
	/** Activate fbo for rendering to mip-map level of current depth texture. */
	void ActivateFBODepthLevel( int level );
	
	/** Activate fbo for rendering to third depth texture. */
	void ActivateFBODepth3();
	
	/** Activate fbo for rendering to XRay depth texture. */
	void ActivateFBODepthXRay();
	
	/** Activates the current post process fbo with or without depth enabled. */
	void ActivatePostProcessFBO( bool withDepth );
	/** Activates the diffuse fbo with or without depth. */
	void ActivateFBODiffuse( bool withDepth );
	/** Activates the reflectivity fbo with or without depth. */
	void ActivateFBOReflectivity( bool withDepth );
	/** Activates the roughness fbo with or without depth. */
	void ActivateFBORoughness( bool withDepth );
	/** Activates the ao-solidity fbo with or without depth. */
	void ActivateFBOAOSolidity( bool withDepth );
	/** Activates the first temporary fbo with depth. */
	void ActivateFBOTemporary1( bool withDepth );
	/** Activates the fbo for a mip-map level of the first temporary. */
	void ActivateFBOTemporary1Level( int level );
	/** Activates the second temporary fbo with depth. */
	void ActivateFBOTemporary2( bool withDepth );
	/** Activates the fbo for a mip-map level of the second temporary. */
	void ActivateFBOTemporary2Level( int level );
	/** Activates the third temporary fbo without depth. */
	void ActivateFBOTemporary3();
	/** Activates the color fbo with or without depth. */
	void ActivateFBOColor( bool withDepth, bool withLuminance );
	/** Activates the color and temporary2 fbo with depth. */
	void ActivateFBOColorTemp2( bool withDepth, bool withLuminance );
	/** Activates the diffuse, normal, reflectivity, roughness and color fbo with depth. */
	void ActivateFBOMaterialColor();
	/** Activate luminance without depth. */
	void ActivateFBOLuminance();
	
	/** Retrieves the depth min-map texture. */
	inline deoglDRDepthMinMax &GetDepthMinMax() const{ return *pDepthMinMax; }
	
	/** Luminance texture. */
// 	inline deoglArrayTexture *GetTextureLuminance() const{ return pTextureLuminance; }
	
	/** Luminance normal texture. */
// 	inline deoglArrayTexture *GetTextureLuminanceNormal() const{ return pTextureLuminanceNormal; }
	
	/** Luminance depth texture. */
// 	inline deoglArrayTexture *GetTextureLuminanceDepth() const{ return pTextureLuminanceDepth; }
	
	/** Activate luminance fbo. */
// 	void ActivateFBOLuminance();
	
	/** Activate luminance normal fbo. */
// 	void ActivateFBOLuminanceNormal();
	
	/** Retrieves the full screen quad VAO. */
	inline deoglVAO *GetVAOFullScreenQuad() const{ return pVAOFullScreenQuad; }
	/** Retrieves the billboard VAO. */
	inline deoglVAO *GetVAOBillboard() const{ return pVAOBillboard; }
	
	/** Memory consumption. */
	inline const deoglMemoryConsumptionDeferredRenderingUse &GetMemoryConsumption() const{ return pMemUse; }
	/*@}*/
	
	/** \name Rendering */
	/*@{*/
	/**
	 * Fills the viewport parameters into the given shader parameter. The first parameter is
	 * set to the left boundary, the second to the top boundary, the third to the right
	 * boundary and the fourthed to the bottom boundary. This way the parameter can be used
	 * for clamping like this in the shaders: clamp( coordinates, viewport.xy, viewport.zw ).
	 * If the normalized parameter is set the boundaries are stored in the range from 0 to 1
	 * instead from 0 to width/height. Former is used for rectangular textures and later for
	 * non-rectangular textures.
	 */
	void SetShaderViewport( deoglShaderCompiled &shader, int parameter, bool normalized );
	void SetShaderViewport( deoglSPBlockUBO &paramBlock, int parameter, bool normalized );
	/**
	 * Sets the scaling and offset parameters in a shader required to convert from full screen
	 * quad positions to texture coordinates.
	 */
	void SetShaderParamFSQuad( deoglShaderCompiled &shader, int parameter ) const;
	void SetShaderParamFSQuad( deoglShaderCompiled &shader, int parameter, int width, int height ) const;
	void SetShaderParamFSQuad( deoglShaderCompiled &shader, int parameter, float x1, float y1, float x2, float y2 ) const;
	void SetShaderParamFSQuadUpsideDown( deoglShaderCompiled &shader, int parameter ) const;
	void SetShaderParamFSQuadUpsideDown( deoglShaderCompiled &shader, int parameter, int width, int height ) const;
	void SetShaderParamFSQuadUpsideDown( deoglShaderCompiled &shader, int parameter, float x1, float y1, float x2, float y2 ) const;
	void SetShaderParamFSQuad( deoglSPBlockUBO &paramBlock, int parameter ) const;
	/**
	 * Sets the scaling and offset parameters in a shader required to convert from screen
	 * coordinates to texture coordinates.
	 */
	void SetShaderParamSCToTC( deoglShaderCompiled &shader, int parameter ) const;
	/**
	 * Sets the scaling and offset parameters in a shader required to convert from screen
	 * coordinates to full size texture coordinates.
	 */
	void SetShaderParamSCToFSTC( deoglShaderCompiled &shader, int parameter ) const;
	/**
	 * Sets the scaling and offset parameters in a shader required to convert from screen
	 * coordinates to camera space coordinates.
	 */
	void SetShaderParamSCToCSP( deoglShaderCompiled &shader, int parameter ) const;
	
	/** Render doc debug texture. */
	inline deoglTexture *GetTexRenderDocDebug() const{ return pTexRenderDocDebug; }
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateTextures();
	void pUpdateMemoryUsage();
	void pCreateFBOs();
	void pCreateFBOTex( int index, const char *debugName, deoglArrayTexture *texture1,
		deoglArrayTexture *texture2, deoglArrayTexture *texture3, deoglArrayTexture *texture4,
		deoglArrayTexture *texture5, deoglArrayTexture *texture6, deoglArrayTexture *texture7,
		deoglArrayTexture *depth );
	void pFBOAttachColors( int index, const char *debugName, deoglArrayTexture *texture1,
		deoglArrayTexture *texture2, deoglArrayTexture *texture3, deoglArrayTexture *texture4,
		deoglArrayTexture *texture5, deoglArrayTexture *texture6, deoglArrayTexture *texture7 );
	void pDestroyFBOs();
};

#endif
