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

#ifndef _DEOGLRENDERLIGHT_H_
#define _DEOGLRENDERLIGHT_H_

#include "deoglRenderLightBase.h"

class deoglAddToRenderTask;
class deoglLightProbeTexture;
class deoglRenderLightParticles;
class deoglRenderLightPoint;
class deoglRenderLightSky;
class deoglRenderLightSpot;
class deoglRenderTask;
class deoglRLight;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglRTRenderers;


/**
 * \brief Light renderer.
 */
class deoglRenderLight : public deoglRenderLightBase{
private:
	deoglShaderProgram *pShaderCopyDepth;
	
	deoglShaderProgram *pShaderAOLocal;
	deoglShaderProgram *pShaderAOBlur1;
	deoglShaderProgram *pShaderAOBlur2;
	deoglShaderProgram *pShaderDebugAO;
	
	deoglShaderProgram *pShaderSSSSS;
	
	deoglRenderLightSpot *pRenderLightSpot;
	deoglRenderLightSky *pRenderLightSky;
	deoglRenderLightPoint *pRenderLightPoint;
	deoglRenderLightParticles *pRenderLightParticles;
	
	deoglSPBlockUBO *pLightPB;
	deoglSPBlockUBO *pShadowPB;
	deoglSPBlockUBO *pShadowCubePB;
	deoglSPBlockUBO *pOccMapPB;
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTask *pAddToRenderTask;
	deoglLightProbeTexture *pLightProbesTexture;
	
	deoglDebugInformation *pDebugInfoSolid;
	deoglDebugInformation *pDebugInfoSolidCopyDepth;
	deoglDebugInformation *pDebugInfoSolidParticle;
	deoglDebugInformation *pDebugInfoSolidSSSSS;
	
	deoglDebugInformation *pDebugInfoTransparent;
	deoglDebugInformation *pDebugInfoTransparentCopyDepth;
	deoglDebugInformation *pDebugInfoTransparentSSSSS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create light renderer. */
	deoglRenderLight( deoglRenderThread &renderThread, deoglRTRenderers &renderers );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderLight();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** \brief Renderer for spot lights. */
	inline deoglRenderLightSpot &GetRenderLightSpot() const{ return *pRenderLightSpot; }
	
	/** \brief Renderer for sky lights. */
	inline deoglRenderLightSky &GetRenderLightSky() const{ return *pRenderLightSky; }
	
	/** \brief Renderer for point lights. */
	inline deoglRenderLightPoint &GetRenderLightPoint() const{ return *pRenderLightPoint; }
	
	/** \brief Renderer for particle lights. */
	inline deoglRenderLightParticles &GetRenderLightParticles() const{ return *pRenderLightParticles; }
	
	
	
	/** \brief Light render parameter block. */
	inline deoglSPBlockUBO *GetLightPB() const{ return pLightPB; }
	
	/** \brief Shadow render parameter block. */
	inline deoglSPBlockUBO *GetShadowPB() const{ return pShadowPB; }
	
	/** \brief Shadow render parameter block cubemap. */
	inline deoglSPBlockUBO *GetShadowCubePB() const{ return pShadowCubePB; }
	
	/** \brief Occmap render parameter block. */
	inline deoglSPBlockUBO *GetOccMapPB() const{ return pOccMapPB; }
	
	/** \brief Render task. */
	inline deoglRenderTask &GetRenderTask() const{ return *pRenderTask; }
	
	/** \brief Add to render task. */
	inline deoglAddToRenderTask &GetAddToRenderTask() const{ return *pAddToRenderTask; }
	
	/** \brief Light probes texture. */
	inline deoglLightProbeTexture &GetLightProbesTexture() const{ return *pLightProbesTexture; }
	
	
	
	/** \brief Render lights. */
	void RenderLights( deoglRenderPlan &plan, bool solid );
	
	/**
	 * \brief Render ambient occlusion.
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
	
	/** \brief Render screen space sub surface scattering. */
	void RenderSSSSS( deoglRenderPlan &plan, bool solid );
	
	/** \brief Prepare light render parameter shader parameter block. */
	void PrepareRenderParamBlockLight( deoglRenderPlan &plan );
	
	
	
	/** \brief Reset debug information. */
	void ResetDebugInfo();
	
	/** \brief Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** \brief Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
