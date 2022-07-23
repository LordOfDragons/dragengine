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

#ifndef _DEOGLRENDERDEPTHPASS_H_
#define _DEOGLRENDERDEPTHPASS_H_

#include "deoglRenderBase.h"
#include "deoglRenderTranspCounting.h"


class deoglRenderPlan;
class deoglRenderPlanMasked;
class deoglOcclusionQuery;


/**
 * \brief World renderer.
 */
class deoglRenderDepthPass : public deoglRenderBase{
private:
	deoglShaderProgramUsage pShaderDepthDownsample;
	deoglShaderProgramUsage pShaderDepthDownsampleStereo;
	
	deoglShaderProgramUsage pShaderDepthSolid;
	deoglShaderProgramUsage pShaderDepthClipSolid;
	
	deoglShaderProgramUsage pShaderParticleDepthSolid;
	deoglShaderProgramUsage pShaderParticleDepthSolidCD;
	deoglShaderProgramUsage pShaderParticleDepthHoles;
	deoglShaderProgramUsage pShaderParticleDepthHolesCD;
	deoglShaderProgramUsage pShaderParticleDepthClipSolid;
	deoglShaderProgramUsage pShaderParticleDepthClipSolidCD;
	deoglShaderProgramUsage pShaderParticleDepthClipHoles;
	deoglShaderProgramUsage pShaderParticleDepthClipHolesCD;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderDepthPass( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderDepthPass();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/**
	 * \brief Render solid depth pass.
	 * 
	 * Using FBO Def-Ren Depth. Clears depth.
	 * - RenderDepth
	 * - RenderOcclusionQueryPass
	 * - count transparency
	 * Invalidates no attachments.
	 */
	void RenderSolidDepthPass( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	
	
	
	/**
	 * \brief Render depth.
	 * 
	 * Using FBO Def-Ren Depth. No clearing.
	 * - render geometry
	 * Invalidates no attachments.
	 */
	void RenderDepth( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask,
		bool solid, bool maskedOnly, bool reverseDepthTest );
	
	
	
	/**
	 * \brief Downsample depth texture.
	 * 
	 * Using FBO Def-Ren Depth-MipMap. Renders from one mip map level to the next one.
	 * Clears each level. Invalidates no buffers.
	 */
	void DownsampleDepth( deoglRenderPlan &plan );
	
	/**
	 * \brief Render occlusion query pass.
	 * 
	 * Using FBO Def-Ren Depth set by RenderSolidGeometryPass. No clearing.
	 * Invalidates no attachments.
	 */
	void RenderOcclusionQueryPass( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	/*@}*/
};

#endif
