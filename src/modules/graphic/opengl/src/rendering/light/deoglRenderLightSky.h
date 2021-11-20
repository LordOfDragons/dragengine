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
class deoglRenderableDepthArrayTexture;
class deoglRenderPlan;
class deoglRLight;
class deoglRSkyLayer;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglShadowMapper;
class deoglRSkyInstanceLayer;
class deoglRenderPlanSkyLight;
class deoglSkyLayerGICascade;


/**
 * Render sky lights.
 */
class deoglRenderLightSky : public deoglRenderLightBase{
private:
	deoglCollideList *pColList2;
	deoglShaderProgram *pShaderAO;
	deoglShaderProgram *pShaderClearDepth;
	deoglShaderProgram *pShaderOccMesh;
	
	deoglRenderableDepthArrayTexture *pSolidShadowMap;
	
	
	
	deoglDebugInformation *pDebugInfoSolid;
	deoglDebugInformation *pDebugInfoTransparent;
	
	deoglDebugInformation *pDebugInfoSolidDetail;
	deoglDebugInformation *pDebugInfoSolidShadow;
	deoglDebugInformation *pDebugInfoSolidShadowOcclusion;
	deoglDebugInformation *pDebugInfoSolidShadowSplit;
	deoglDebugInformation *pDebugInfoSolidShadowSplitContent;
	deoglDebugInformation *pDebugInfoSolidShadowSplitLODLevels;
	deoglDebugInformation *pDebugInfoSolidShadowSplitClear;
	deoglDebugInformation *pDebugInfoSolidShadowSplitTask;
	deoglDebugInformation *pDebugInfoSolidShadowSplitRender;
	deoglDebugInformation *pDebugInfoSolidShadowGI;
	deoglDebugInformation *pDebugInfoSolidLight;
	
	deoglDebugInformation *pDebugInfoTransparentDetail;
	deoglDebugInformation *pDebugInfoTransparentLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderLightSky( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	virtual ~deoglRenderLightSky();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Occlusion mesh shader. */
	inline deoglShaderProgram *GetShaderOccMesh() const{ return pShaderOccMesh; }
	
	/** Render lights. */
	void RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask );
	
	/** Render sky light ambient occlusion. */
	void RenderAO( deoglRenderPlan &plan );
	
	/** Render sky light. */
	void RenderLight( deoglRenderPlanSkyLight &plan, bool solid, const deoglRenderPlanMasked *mask );
	
	/** Render shadows maps. */
	void RenderShadows( deoglRenderPlanSkyLight &plan, bool solid, const deoglRenderPlanMasked *mask );
	
	/** Render shadow map. */
	void RenderShadowMap( deoglRenderPlanSkyLight &plan, deoglShadowMapper &shadowMapper );
	
	/** Render GI shadows. */
	void RenderGIShadows( deoglRenderPlanSkyLight &plan, deoglShadowMapper &shadowMapper );
	
	/** Render GI shadow map. */
	void RenderGIShadowMap( deoglShadowMapper &shadowMapper, deoglRenderTask &renderTask,
		int shadowMapSize, bool clearBackFaceFragments );
	
	/** Update light shader parameter block. */
	void UpdateLightParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanSkyLight &plan );
	
	/** Updates instance shader parameter block. */
	void UpdateInstanceParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanSkyLight &plan, int shadowMapSize, int passCount );
	
	
	
	/** Debug information solid lighting. */
	inline deoglDebugInformation *GetDebugInfoSolid() const{ return pDebugInfoSolid; }
	
	/** Debug information transparent lighting. */
	inline deoglDebugInformation *GetDebugInfoTransparent() const{ return pDebugInfoTransparent; }
	
	
	
	/** Reset debug information. */
	void ResetDebugInfo();
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoSolid();
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoTransparent();
	
	/** Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
