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

#ifndef _DEOGLRENDERLIGHTSKY_H_
#define _DEOGLRENDERLIGHTSKY_H_

#include "deoglRenderLightBase.h"

class deoglCollideList;
class deoglLightShader;
class deoglRenderableArrayTexture;
class deoglRenderPlan;
class deoglRLight;
class deoglRSkyLayer;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglShadowMapper;
class deoglRSkyInstanceLayer;
class deoglRenderPlanSkyLight;



/**
 * \brief Render sky lights.
 */
class deoglRenderLightSky : public deoglRenderLightBase{
private:
	deoglCollideList *pColList2;
	deoglShaderProgram *pShaderAO;
	
	deoglRenderableArrayTexture *pSolidShadowMap;
	
	
	
	deoglDebugInformation *pDebugInfoSolid;
	deoglDebugInformation *pDebugInfoTransparent;
	
	deoglDebugInformation *pDebugInfoSolidDetail;
	deoglDebugInformation *pDebugInfoSolidShadow;
	deoglDebugInformation *pDebugInfoSolidShadowElements;
	deoglDebugInformation *pDebugInfoSolidShadowOcclusion;
	deoglDebugInformation *pDebugInfoSolidShadowOcclusionStart;
	deoglDebugInformation *pDebugInfoSolidShadowOcclusionVBO;
	deoglDebugInformation *pDebugInfoSolidShadowOcclusionTest;
	deoglDebugInformation *pDebugInfoSolidShadowOcclusionTracing;
	deoglDebugInformation *pDebugInfoSolidShadowVBOs;
	deoglDebugInformation *pDebugInfoSolidShadowClear;
	deoglDebugInformation *pDebugInfoSolidShadowSplit;
	deoglDebugInformation *pDebugInfoSolidShadowSplitContent;
	deoglDebugInformation *pDebugInfoSolidShadowSplitLODLevels;
	deoglDebugInformation *pDebugInfoSolidShadowSplitClear;
	deoglDebugInformation *pDebugInfoSolidShadowSplitTask;
	deoglDebugInformation *pDebugInfoSolidShadowSplitRender;
	deoglDebugInformation *pDebugInfoSolidLight;
	
	deoglDebugInformation *pDebugInfoTransparentDetail;
	deoglDebugInformation *pDebugInfoTransparentLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderLightSky( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderLightSky();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** \brief Render lights. */
	void RenderLights( deoglRenderPlan &plan, bool solid );
	
	/** \brief Render sky light ambient occlusion. */
	void RenderAO( deoglRenderPlan &plan );
	
	/** \brief Render occlusion tracing. */
	void RenderOcclusionTracing( deoglRenderPlan &plan );
	
	/** \brief Render sky light. */
	void RenderLight( deoglRenderPlan &plan, bool solid, deoglRenderPlanSkyLight &planSkyLight );
	
	/** \brief Render shadows maps. */
	void RenderShadows( deoglRenderPlan &plan, deoglRenderPlanSkyLight &planSkyLight );
	
	/** \brief Render shadow map. */
	void RenderShadowMap( deoglRenderPlan &plan, deoglRenderPlanSkyLight &planSkyLight,
		deoglShadowMapper &shadowMapper );
	
	/** \brief Update light shader parameter block. */
	void UpdateLightParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlan &plan, deoglRenderPlanSkyLight &planSkyLight );
	
	/** \brief Updates instance shader parameter block. */
	void UpdateInstanceParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlan &plan, deoglRenderPlanSkyLight &planSkyLight, int shadowMapSize, int passCount );
	
	
	
	/** \brief Debug information solid lighting. */
	inline deoglDebugInformation *GetDebugInfoSolid() const{ return pDebugInfoSolid; }
	
	/** \brief Debug information transparent lighting. */
	inline deoglDebugInformation *GetDebugInfoTransparent() const{ return pDebugInfoTransparent; }
	
	
	
	/** \brief Reset debug information. */
	void ResetDebugInfo();
	
	/** \brief Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoSolid();
	
	/** \brief Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoTransparent();
	
	/** \brief Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
