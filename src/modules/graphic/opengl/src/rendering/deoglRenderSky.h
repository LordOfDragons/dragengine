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
