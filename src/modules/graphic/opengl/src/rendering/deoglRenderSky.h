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

#ifndef _DEOGLRENDERSKY_H_
#define _DEOGLRENDERSKY_H_

#include "deoglRenderBase.h"

#include <dragengine/common/collection/decPointerList.h>


class deoglRSkyInstance;
class deoglRenderPlan;
class deoglRenderPlanMasked;
class deoglEnvironmentMap;
class deoglRWorld;
class decLayerMask;



/**
 * OpenGL sky renderer.
 */
class deoglRenderSky : public deoglRenderBase{
private:
	deoglSPBSingleUse::Ref pRenderSkyIntoEnvMapPBSingleUse;
	deoglSPBlockUBO::Ref pRenderSkyIntoEnvMapPB;
	
	const deoglPipeline *pPipelineSkySphere;
	const deoglPipeline *pPipelineSkySphereStereo;
	
	const deoglPipeline *pPipelineSkyBox;
	const deoglPipeline *pPipelineSkyBoxStereo;
	
	const deoglPipeline *pPipelineBody;
	const deoglPipeline *pPipelineBodyStereo;
	
	decPointerList pSkyInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderSky( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	~deoglRenderSky();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/**
	 * Render sky.
	 * 
	 * Uses FBO Depth+Color. Clears color buffer. Depth used for rendering only background.
	 * - RenderSkyBox
	 * - RenderSkySphere
	 * Invalidates no attachments.
	 */
	void RenderSky( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	
	/** Render sky. */
	//void RenderSkyOld( deoglRenderPlan &plan );
	
	/** Render sky box layer. */
	bool RenderSkyBox( deoglRenderPlan &plan, deoglRSkyInstance &instance,
		int layerIndex, bool first, bool renderIntoEnvMap );
	
	/** Render sky sphere layer. */
	bool RenderSkySphere( deoglRenderPlan &plan, deoglRSkyInstance &instance,
		int layerIndex, bool first, bool renderIntoEnvMap );
	
	/** Render sky bodies layer. */
	void RenderSkyLayerBodies( deoglRenderPlan &plan, deoglRSkyInstance &instance,
		int layerIndex, bool renderIntoEnvMap );
	
	/** Render sky into an environment map. */
	void RenderSkyIntoEnvMap( deoglRWorld &world, const decLayerMask &layerMask,
		deoglEnvironmentMap &envmap );
	
	/** Render empty sky into an environment map. */
	void RenderEmptySkyIntoEnvMap( deoglRWorld &world, deoglEnvironmentMap &envmap );
	
	/** Get sky background color in linear space. */
	decColor LinearBgColor( const deoglRSkyInstance &instance, bool first ) const;
	
	/** Prepare render sky into env map parameter block. */
	void PreparepRenderSkyIntoEnvMapParamBlock( const deoglRenderPlan &plan );
	/*@}*/
};

#endif
