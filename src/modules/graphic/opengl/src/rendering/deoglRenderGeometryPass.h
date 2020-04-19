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

#ifndef _DEOGLRENDERGEOMETRYPASS_H_
#define _DEOGLRENDERGEOMETRYPASS_H_

#include "deoglRenderBase.h"
#include "../shaders/deoglShaderProgramUsage.h"


class deoglRenderPlan;
class deoglRenderPlanMasked;


/**
 * \brief Geometry pass renderer.
 */
class deoglRenderGeometryPass : public deoglRenderBase{
private:
	deoglShaderProgramUsage pShaderDEBUG;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderGeometryPass( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderGeometryPass();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/**
	 * \brief Render solid geometry pass.
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
	void RenderSolidGeometryPass( deoglRenderPlan &plan, deoglRenderPlanMasked *mask );
	
	
	
	/**
	 * \brief Render decals.
	 * 
	 * Using FBO Def-Ren Material buffers set by RenderSolidGeometryPass. No clearing.
	 * Invalidating no buffers.
	 */
	void RenderDecals( deoglRenderPlan &plan );
	
	/** \brief Render volumetric pass. */
	void RenderVolumetricPass( deoglRenderPlan &plan, deoglRenderPlanMasked *mask, bool inbetween );
	/*@}*/
};

#endif
