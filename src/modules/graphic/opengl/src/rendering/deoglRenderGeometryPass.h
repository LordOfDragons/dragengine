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

#ifndef _DEOGLRENDERGEOMETRYPASS_H_
#define _DEOGLRENDERGEOMETRYPASS_H_

#include "deoglRenderBase.h"

class deoglRenderPlan;
class deoglRenderPlanMasked;


/**
 * Geometry pass renderer.
 */
class deoglRenderGeometryPass : public deoglRenderBase{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderGeometryPass( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	virtual ~deoglRenderGeometryPass();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/**
	 * Render solid geometry pass.
	 * 
	 * Using FBO Def-Ren Depth. Clears depth. Depth buffer only is used to take advantage of
	 * double write optimization. Prevents use of Depth+Color to avoid FBO switch in RenderSky.
	 * - RenderDepthPass
	 * - RenderOcclusionQueryPass
	 * - RenderTransparencyCounterPass
	 * Invalidates no attachments.
	 * 
	 * Using FBO Def-Ren Material. Clears color attachments.
	 * - Sky.RenderSky (clears color buffer)
	 * - render geometry (clears colors except color buffer)
	 * - RenderDecals
	 * - DownsampleDepth (if SSAO and not No-Reflections)
	 * - Light.RenderAO (if not No-Reflections)
	 * Invalidates no attachments.
	 */
	void RenderSolidGeometryPass( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool xray );
	
	/**
	 * Render luminance only pass.
	 */
// 	void RenderLuminanceOnly( deoglRenderPlan &plan );
	
	
	
	/**
	 * Render decals.
	 * 
	 * Using FBO Def-Ren Material buffers set by RenderSolidGeometryPass. No clearing.
	 * Invalidating no buffers.
	 */
	void RenderDecals( deoglRenderPlan &plan, bool xray );
	
	/** Render volumetric pass. */
	void RenderVolumetricPass( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool inbetween );
	/*@}*/
};

#endif
