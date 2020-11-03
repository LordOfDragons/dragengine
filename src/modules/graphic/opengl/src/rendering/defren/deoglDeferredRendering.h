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

#ifndef _DEOGLDEFERREDRENDERING_H_
#define _DEOGLDEFERREDRENDERING_H_

#include "../../deoglGL.h"
#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglFramebuffer;
class deoglRenderbuffer;
class deoglDRDepthMinMax;
class deoglShaderCompiled;
class deoglSPBlockUBO;
class deoglTexture;
class deoglVAO;



/**
 * @brief OpenGL Deferred Rendering.
 * Manages the framebuffers and render buffers required for deferred rendering.
 */
class deoglDeferredRendering{
private:
	deoglRenderThread &pRenderThread;
	
	int pWidth;
	int pHeight;
	int pRealWidth;
	int pRealHeight;
	float pScalingU;
	float pScalingV;
	float pPixelSizeU;
	float pPixelSizeV;
	float pFSQuadOffU;
	float pFSQuadOffV;
	float pClampU;
	float pClampV;
	
	bool pUseEncodedDepth;
	bool pUseFadeOutRange;
	
	bool pUseInverseDepth;
	GLenum pDepthCompareFuncRegular;
	GLenum pDepthCompareFuncReversed;
	GLfloat pClearDepthValueRegular;
	GLfloat pClearDepthValueReversed;
	
	deoglRenderbuffer *pRenderbuffer;
	deoglTexture *pTextureDepth1;
	deoglTexture *pTextureDepth2;
	deoglTexture *pTextureDepth3;
	deoglTexture *pTextureDiffuse;
	deoglTexture *pTextureNormal;
	deoglTexture *pTextureReflectivity;
	deoglTexture *pTextureRoughness;
	deoglTexture *pTextureAOSolidity;
	deoglTexture *pTextureSubSurface;
	deoglTexture *pTextureColor;
	deoglTexture *pTextureTemporary1;
	deoglTexture *pTextureTemporary2;
	deoglTexture *pTextureTemporary3;
	
	deoglFramebuffer *pFBOs[ 30 ];
	deoglFramebuffer **pFBOMipMapDepth1;
	deoglFramebuffer **pFBOMipMapDepth2;
	deoglFramebuffer **pFBOMipMapTemporary1;
	deoglFramebuffer **pFBOMipMapTemporary2;
	int pFBOMipMapDepthCount;
	int pFBOMipMapCount;
	bool pModeDepth;
	bool pModePostProcess;
	
	deoglDRDepthMinMax *pDepthMinMax;
	
	deoglTexture *pTextureLuminance;
	deoglTexture *pTextureLuminanceNormal;
	deoglTexture *pTextureLuminanceDepth;
	deoglFramebuffer *pFBOLuminance;
	deoglFramebuffer *pFBOLuminanceNormal;
	
	int pMemoryUsageGPU;
	int pMemoryUsageGPUTexture;
	int pMemoryUsageGPURenBuf;
	
	GLuint pVBOFullScreenQuad;
	GLuint pVBOBillboard;
	deoglVAO *pVAOFullScreenQuad;
	deoglVAO *pVAOBillboard;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new opengl deferred rendering object. */
	deoglDeferredRendering( deoglRenderThread &renderThread );
	/** Cleans up the opengl deferred rendering object. */
	~deoglDeferredRendering();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the width of the buffers. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height of the buffers. */
	inline int GetHeight() const{ return pHeight; }
	/** Retrieves the real width of the buffers. */
	inline int GetRealWidth() const{ return pRealWidth; }
	/** Retrieves the real height of the buffers. */
	inline int GetRealHeight() const{ return pRealHeight; }
	/** Retrieves the scaling factor in u direction. */
	inline float GetScalingU() const{ return pScalingU; }
	/** Retrieves the scaling factor in v direction. */
	inline float GetScalingV() const{ return pScalingV; }
	/** Retrieves the texture coordinate displacement in u direction for 1 pixel. */
	inline float GetPixelSizeU() const{ return pPixelSizeU; }
	/** Retrieves the texture coordinate displacement in v direction for 1 pixel. */
	inline float GetPixelSizeV() const{ return pPixelSizeV; }
	/** Resizes the buffers. */
	void Resize( int width, int height );
	/** Force resize of textures. */
	void ForceResize();
	
	
	
	/** Determines if encoded depth is used. */
	inline bool GetUseEncodedDepth() const{ return pUseEncodedDepth; }
	/** Sets if encoded depth is used. */
	void SetUseEncodedDepth( bool useEncodedDepth );
	
	/** \brief Fade out near render range. */
	inline bool GetUseFadeOutRange() const{ return pUseFadeOutRange; }
	
	
	
	/** \brief Enable inverse depth using floating point depth buffer if supported. */
	inline bool GetUseInverseDepth() const{ return pUseInverseDepth; }
	
	/** \brief Regular depth compare function. */
	inline GLenum GetDepthCompareFuncRegular() const{ return pDepthCompareFuncRegular; }
	
	/** \brief Reversed depth compare function. */
	inline GLenum GetDepthCompareFuncReversed() const{ return pDepthCompareFuncReversed; }
	
	/** \brief Regular clear depth value. */
	inline GLfloat GetClearDepthValueRegular() const{ return pClearDepthValueRegular; }
	
	/** \brief Reversed clear depth value. */
	inline GLfloat GetClearDepthValueReversed() const{ return pClearDepthValueReversed; }
	
	/**
	 * \brief Create projection matrix matching depth usage mode.
	 * \details Depending on the inverse depth mode used the projection matrix is either
	 *          infinite or non-infinite.
	 */
	decDMatrix CreateProjectionDMatrix( int width, int height, float fov, float fovRatio, float znear, float zfar ) const;
	
	/**
	 * \brief Create frustum matrix.
	 * \details This is the same as CreateProjectionDMatrix but always creates a
	 *          non-infinite projection matrix.
	 */
	decDMatrix CreateFrustumDMatrix( int width, int height, float fov, float fovRatio, float znear, float zfar ) const;
	
	
	
	/** \brief Render buffer. */
	inline deoglRenderbuffer *GetRenderbuffer() const{ return pRenderbuffer; }
	
	/** \brief First depth texture. */
	deoglTexture *GetDepthTexture1() const;
	
	/** \brief Second depth texture. */
	deoglTexture *GetDepthTexture2() const;
	
	/** \brief Third depth texture. */
	inline deoglTexture *GetDepthTexture3() const{ return pTextureDepth3; }
	
	/** \brief Swap first depth texture to second. */
	void SwapDepthTextures();
	
	/**
	 * \brief Copy first depth texture to second using framebuffer blitting.
	 * \details After call returns active framebuffer is undefined.
	 */
	void CopyFirstDepthToSecond( bool copyDepth, bool copyStencil );
	
	/**
	 * \brief Copy first depth texture to third depth using framebuffer blitting.
	 * \details After call returns active framebuffer is undefined.
	 */
	void CopyFirstDepthToThirdDepth( bool copyDepth, bool copyStencil );
	
	/** Retrieves the diffuse texture. */
	inline deoglTexture *GetTextureDiffuse() const{ return pTextureDiffuse; }
	/** Retrieves the normal texture. */
	inline deoglTexture *GetTextureNormal() const{ return pTextureNormal; }
	/** Retrieves the reflectivity texture. */
	inline deoglTexture *GetTextureReflectivity() const{ return pTextureReflectivity; }
	/** Retrieves the roughness texture. */
	inline deoglTexture *GetTextureRoughness() const{ return pTextureRoughness; }
	/** Retrieves the ao and solidity texture. */
	inline deoglTexture *GetTextureAOSolidity() const{ return pTextureAOSolidity; }
	/** Retrieves the sub-surface texture. */
	inline deoglTexture *GetTextureSubSurface() const{ return pTextureSubSurface; }
	/** Retrieves the color texture. */
	inline deoglTexture *GetTextureColor() const{ return pTextureColor; }
	/** Retrieves the first temporary texture. */
	inline deoglTexture *GetTextureTemporary1() const{ return pTextureTemporary1; }
	/** Retrieves the second temporary texture. */
	inline deoglTexture *GetTextureTemporary2() const{ return pTextureTemporary2; }
	/** Retrieves the third temporary texture. */
	inline deoglTexture *GetTextureTemporary3() const{ return pTextureTemporary3; }
	
	/** Sets the normal as the current post processing target. */
	void InitPostProcessTarget();
	/** Swaps the diffuse and normal as current post process target. */
	void SwapPostProcessTarget();
	/** Retrieves the diffuse or normal texture depending which fbo is the current post process fbo. */
	deoglTexture *GetPostProcessTexture() const;
	
	
	
	/** \brief Activate fbo for rendering to first depth texture. */
	void ActivateFBODepth();
	
	/** \brief Activate fbo for rendering to mip-map level of current depth texture. */
	void ActivateFBODepthLevel( int level );
	
	/** \brief Activate fbo for rendering to third depth texture. */
	void ActivateFBODepth3();
	
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
	void ActivateFBOColor( bool withDepth );
	/** Activates the depth and diffuse fbo with depth. */
	void ActivateFBODepthDiffuse();
	/** Activates the depth and first temporary fbo with depth. */
	void ActivateFBODepthTemp1();
	/** Activates the color fbo with depth. */
	void ActivateFBOColorDepth();
	/** Activates the color and temporary2 fbo with depth. */
	void ActivateFBOColorTemp2( bool withDepth );
	/** Activates the diffuse, normal, reflectivity, roughness and color fbo with depth. */
	void ActivateFBOMaterialColor();
	
	/** Retrieves the depth min-map texture. */
	inline deoglDRDepthMinMax &GetDepthMinMax() const{ return *pDepthMinMax; }
	
	/** Luminance texture. */
	inline deoglTexture *GetTextureLuminance() const{ return pTextureLuminance; }
	
	/** Luminance normal texture. */
	inline deoglTexture *GetTextureLuminanceNormal() const{ return pTextureLuminanceNormal; }
	
	/** Luminance depth texture. */
	inline deoglTexture *GetTextureLuminanceDepth() const{ return pTextureLuminanceDepth; }
	
	/** Activate luminance fbo. */
	void ActivateFBOLuminance();
	
	/** Activate luminance normal fbo. */
	void ActivateFBOLuminanceNormal();
	
	/** Retrieves the full screen quad VAO. */
	inline deoglVAO *GetVAOFullScreenQuad() const{ return pVAOFullScreenQuad; }
	/** Retrieves the billboard VAO. */
	inline deoglVAO *GetVAOBillboard() const{ return pVAOBillboard; }
	
	/** Retrieves the GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	/** Retrieves the texture GPU memory usage. */
	inline int GetMemoryUsageGPUTexture() const{ return pMemoryUsageGPUTexture; }
	/** Retrieves the renderbuffer GPU memory usage. */
	inline int GetMemoryUsageGPURenderbuffer() const{ return pMemoryUsageGPURenBuf; }
	/*@}*/
	
	/** @name Rendering */
	/*@{*/
	/** Renders a full screen quad using VAO. */
	void RenderFSQuadVAO();
	
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
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateTextures();
	void pUpdateMemoryUsage();
	void pCreateFBOs();
	void pCreateFBOTex( int index, deoglTexture *texture1, deoglTexture *texture2, deoglTexture *texture3,
		deoglTexture *texture4, deoglTexture *texture5, deoglTexture *texture6, deoglTexture *texture7,
		deoglTexture *depth );
	void pCreateFBORenBuf( int index, deoglTexture *texture1, deoglTexture *texture2, deoglTexture *texture3,
		deoglTexture *texture4, deoglTexture *texture5, deoglTexture *texture6, deoglTexture *texture7,
		bool depth );
	void pFBOAttachColors( int index, deoglTexture *texture1, deoglTexture *texture2, deoglTexture *texture3,
		deoglTexture *texture4, deoglTexture *texture5, deoglTexture *texture6, deoglTexture *texture7 );
	void pDestroyFBOs();
};

#endif
