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

#ifndef _DEOGLRRTCHOICES_H_
#define _DEOGLRRTCHOICES_H_

#include "../deoglBasics.h"

class deoglRenderThread;


/**
 * Render thread choices.
 * 
 * Stores choices so they can be easily retrieved by interested parties.
 */
class deoglRTChoices{
public:
	enum eGPUTransformVertices{
		egputvNone,
		egputvAccurate,
		egputvApproximate
	};
	
	static const int RenderDocDebugFlagGI = 0x1;
	
	
	
private:
	bool pSharedVBOUseBaseVertex;
	bool pUseSSBORender;
	bool pSharedSPBUseSSBO;
	bool pGlobalSharedSPBLists;
	bool pRealTransparentParticles;
	eGPUTransformVertices pGPUTransformVertices;
	bool pGIMoveUsingCache;
	bool pVRRenderStereo;
	bool pRenderStereoVSLayer;
	bool pRenderFSQuadStereoVSLayer;
	bool pUseInverseDepth;
	GLenum pDepthCompareFuncRegular;
	GLenum pDepthCompareFuncReversed;
	GLfloat pClearDepthValueRegular;
	GLfloat pClearDepthValueReversed;
	bool pUseComputeRenderTask;
	bool pUseDirectStateAccess;
	int pRenderDocDebugFlags;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread choices. */
	deoglRTChoices( deoglRenderThread &renderThread );
	
	/** Clean up render thread choices. */
	~deoglRTChoices();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Use base vertex for shared VBOs.
	 * 
	 * Requires gl*BaseVertex to be usable and rendering does not required pre-shifted points.
	 */
	inline bool GetSharedVBOUseBaseVertex() const{ return pSharedVBOUseBaseVertex; }
	
	/** Use SSBO for rendering. */
	inline bool GetUseSSBORender() const{ return pUseSSBORender; }
	
	/**
	 * Use SSBO for shared SPB.
	 * 
	 * Requires SSBO to be supported by hardware.
	 */
	inline bool GetSharedSPBUseSSBO() const{ return pSharedSPBUseSSBO; }
	
	/** Use global shared SPB lists. */
	inline bool GetGlobalSharedSPBLists() const{ return pGlobalSharedSPBLists; }
	
	/** Render real transparent particles. */
	inline bool GetRealTransparentParticles() const{ return pRealTransparentParticles; }
	
	/** Transform component vertices on the GPU. */
	inline eGPUTransformVertices GetGPUTransformVertices() const{ return pGPUTransformVertices; }
	
	/** GI move probes using ray cache instead of all rays. */
	inline bool GetGIMoveUsingCache() const{ return pGIMoveUsingCache; }
	
	/** Use render stereo rendering for VR. */
	inline bool GetVRRenderStereo() const{ return pVRRenderStereo; }
	
	/** Use layer in vertex shaders for stereo rendering. */
	inline bool GetRenderStereoVSLayer() const{ return pRenderStereoVSLayer; }
	
	/** Use layer in vertex shaders for fullscreen quad stereo rendering. */
	inline bool GetRenderFSQuadStereoVSLayer() const{ return pRenderFSQuadStereoVSLayer; }
	
	/** Use inverse depth. */
	inline bool GetUseInverseDepth() const{ return pUseInverseDepth; }
	
	/** Regular depth compare function. */
	inline GLenum GetDepthCompareFuncRegular() const{ return pDepthCompareFuncRegular; }
	
	/** Reversed depth compare function. */
	inline GLenum GetDepthCompareFuncReversed() const{ return pDepthCompareFuncReversed; }
	
	/** Regular clear depth value. */
	inline GLfloat GetClearDepthValueRegular() const{ return pClearDepthValueRegular; }
	
	/** Reversed clear depth value. */
	inline GLfloat GetClearDepthValueReversed() const{ return pClearDepthValueReversed; }
	
	/** Use compute shader render task. */
	inline bool GetUseComputeRenderTask() const{ return pUseComputeRenderTask; }
	
	/** Use direct state access. */
	inline bool GetUseDirectStateAccess() const{ return pUseDirectStateAccess; }
	
	/** Render doc debug flags. */
	inline int GetRenderDocDebugFlags() const{ return pRenderDocDebugFlags; }
	/*@}*/
};

#endif
