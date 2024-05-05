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

#ifndef _DEOGLRENDERDEPTHPASS_H_
#define _DEOGLRENDERDEPTHPASS_H_

#include "deoglRenderBase.h"
#include "deoglRenderTranspCounting.h"


class deoglRenderPlan;
class deoglRenderPlanMasked;
class deoglOcclusionQuery;


/**
 * World renderer.
 */
class deoglRenderDepthPass : public deoglRenderBase{
private:
	const deoglPipeline *pPipelineDepthDownsample;
	const deoglPipeline *pPipelineDepthDownsampleStereo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderDepthPass( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	virtual ~deoglRenderDepthPass();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/**
	 * Render solid depth pass.
	 * 
	 * Using FBO Def-Ren Depth. Clears depth.
	 * - RenderDepth
	 * - RenderOcclusionQueryPass
	 * - count transparency
	 * Invalidates no attachments.
	 */
	void RenderSolidDepthPass( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool xray );
	
	
	
	/**
	 * Render depth.
	 * 
	 * Using FBO Def-Ren Depth. No clearing.
	 * - render geometry
	 * Invalidates no attachments.
	 */
	void RenderDepth( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask,
		bool solid, bool maskedOnly, bool reverseDepthTest, bool xray );
	
	
	
	/**
	 * Downsample depth texture.
	 * 
	 * Using FBO Def-Ren Depth-MipMap. Renders from one mip map level to the next one.
	 * Clears each level. Invalidates no buffers.
	 */
	void DownsampleDepth( deoglRenderPlan &plan );
	
	/**
	 * Render occlusion query pass.
	 * 
	 * Using FBO Def-Ren Depth set by RenderSolidGeometryPass. No clearing.
	 * Invalidates no attachments.
	 */
	void RenderOcclusionQueryPass( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	/*@}*/
};

#endif
