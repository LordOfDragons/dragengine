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

#ifndef _DEOGLRRTCHOICES_H_
#define _DEOGLRRTCHOICES_H_

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
	
	
	
private:
	bool pSharedVBOUseBaseVertex;
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
	/*@}*/
};

#endif
