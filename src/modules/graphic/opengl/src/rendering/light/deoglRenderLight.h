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

#ifndef _DEOGLRENDERLIGHT_H_
#define _DEOGLRENDERLIGHT_H_

#include "deoglRenderLightBase.h"

class deoglAddToRenderTask;
class deoglRenderLightParticles;
class deoglRenderLightPoint;
class deoglRenderLightSky;
class deoglRenderLightSpot;
class deoglRenderGI;
class deoglRenderTask;
class deoglRLight;
class deoglRTRenderers;
class deoglRenderPlanMasked;


/**
 * Light renderer.
 */
class deoglRenderLight : public deoglRenderLightBase{
private:
	const deoglPipeline *pPipelineAOLocal;
	const deoglPipeline *pPipelineAOLocalStereo;
	const deoglPipeline *pPipelineAOBlur1;
	const deoglPipeline *pPipelineAOBlur1Stereo;
	const deoglPipeline *pPipelineAOBlur2;
	const deoglPipeline *pPipelineAOBlur2Stereo;
	const deoglPipeline *pPipelineDebugAO;
	
	// const deoglPipeline *pPipelineSSAO;
	const deoglPipeline *pPipelineSSAOBlur1;
	const deoglPipeline *pPipelineSSAOBlur2;
	const deoglPipeline *pPipelineSSAOUpscale;
	const deoglPipeline *pPipelineSSAOUpscaleStereo;
	
	const deoglPipeline *pPipelineSSSSS;
	const deoglPipeline *pPipelineSSSSSStereo;
	
	const deoglPipeline *pPipelineCopyDepth;
	const deoglPipeline *pPipelineCopyDepthStereo;
	
	deoglRenderLightSpot *pRenderLightSpot;
	deoglRenderLightSky *pRenderLightSky;
	deoglRenderLightPoint *pRenderLightPoint;
	deoglRenderLightParticles *pRenderLightParticles;
	deoglRenderGI *pRenderGI;
	
	deoglSPBSingleUse::Ref pShadowPBSingleUse;
	deoglSPBlockUBO::Ref pShadowPB;
	deoglSPBSingleUse::Ref pOccMapPBSingleUse;
	deoglSPBlockUBO::Ref pOccMapPB;
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTask *pAddToRenderTask;
	
	deoglDebugInformation::Ref pDebugInfoSolid;
	deoglDebugInformation::Ref pDebugInfoSolidCopyDepth;
	deoglDebugInformation::Ref pDebugInfoSolidParticle;
	deoglDebugInformation::Ref pDebugInfoSolidSSSSS;
	
	deoglDebugInformation::Ref pDebugInfoTransparent;
	deoglDebugInformation::Ref pDebugInfoTransparentCopyDepth;
	deoglDebugInformation::Ref pDebugInfoTransparentSSSSS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light renderer. */
	deoglRenderLight( deoglRenderThread &renderThread, deoglRTRenderers &renderers );
	
	/** Clean up renderer. */
	virtual ~deoglRenderLight();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Renderer for spot lights. */
	inline deoglRenderLightSpot &GetRenderLightSpot() const{ return *pRenderLightSpot; }
	
	/** Renderer for sky lights. */
	inline deoglRenderLightSky &GetRenderLightSky() const{ return *pRenderLightSky; }
	
	/** Renderer for point lights. */
	inline deoglRenderLightPoint &GetRenderLightPoint() const{ return *pRenderLightPoint; }
	
	/** Renderer for particle lights. */
	inline deoglRenderLightParticles &GetRenderLightParticles() const{ return *pRenderLightParticles; }
	
	/** Renderer for global illumination. */
	inline deoglRenderGI &GetRenderGI() const{ return *pRenderGI; }
	
	
	
	/** Get a new shadow render parameter block. */
	const deoglSPBlockUBO::Ref &NextShadowPB();
	
	/** Shadow render parameter block. */
	inline const deoglSPBlockUBO::Ref &GetShadowPB() const{ return pShadowPB; }
	
	/** Get a new occmap render parameter block. */
	const deoglSPBlockUBO::Ref &NextOccMapPB();
	
	/** Occmap render parameter block. */
	inline const deoglSPBlockUBO::Ref &GetOccMapPB() const{ return pOccMapPB; }
	
	/** Render task. */
	inline deoglRenderTask &GetRenderTask() const{ return *pRenderTask; }
	
	/** Add to render task. */
	inline deoglAddToRenderTask &GetAddToRenderTask() const{ return *pAddToRenderTask; }
	
	
	
	/** Render lights. */
	void RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask, bool xray );
	
	/**
	 * Render ambient occlusion.
	 * 
	 * SSAO Pass. Using FBO Def-Ren AO-Solidity buffer. No clearing.
	 * Input textures: 0=depth, 1=diffuse, 2=normal.
	 * Invalidating no buffers. Result in Green Channel.
	 * 
	 * Gauss Blur Pass 1. Using FBO Def-Ren Temporary3. Clear color buffer.
	 * Input textures: 0=aoSolidity, 1=depth.
	 * Invalidating no buffers.
	 * 
	 * Gauss Blur Pass 2. Using FBO Def-Ren AO-Solidity buffer. No clearing.
	 * Input textures: 0=temporary3, 1=depth.
	 * Invalidating no buffers. Result in Green Channel.
	 * 
	 * Result of this pass is AO-Solidity updated with SSAO: (ao, SSAO, solidity, blend)
	 * 
	 * TODO Check if it would be possible to improve the FBO handling to avoid loading
	 *      GPU data on switches. Temporary1 and Temporary2 are already in use. Maybe
	 *      introduce a Temporary4 (with 1-channel type). This would allow to use
	 *      Temporary4 instead of AO-Solidity in the Local AO pass. Doing so would avoid
	 *      the GPU Data Load. In the Gauss Blur Pass 2 the Temporary4 would then be
	 *      read and AO-Solidity Red/Green Channel written in one go. Still requires
	 *      GPU Data Load there but it's only once not twice.
	 */
	void RenderAO( deoglRenderPlan &plan, bool solid );
	
	/** Render screen space sub surface scattering. */
	void RenderSSSSS( deoglRenderPlan &plan, bool solid );
	
	/** Copy first depth to third depth. */
	void CopyDepth1ToDepth3( deoglRenderPlan &plan );
	
	
	
	/** Reset debug information. */
	void ResetDebugInfo();
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
