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
class deoglComputeRenderTask;
class deoglLightShader;
class deoglRenderableDepthArrayTexture;
class deoglRenderPlan;
class deoglRLight;
class deoglRSkyLayer;
class deoglSPBlockUBO;
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
	const deoglPipeline *pPipelineAO;
	const deoglPipeline *pPipelineClearDepth;
	const deoglPipeline *pPipelineOccMesh;
	
	deoglRenderableDepthArrayTexture *pSolidShadowMap;
	
	
	
	deoglDebugInformation::Ref pDebugInfoSolid;
	deoglDebugInformation::Ref pDebugInfoTransparent;
	
	deoglDebugInformation::Ref pDebugInfoSolidDetail;
	deoglDebugInformation::Ref pDebugInfoSolidShadow;
	deoglDebugInformation::Ref pDebugInfoSolidShadowOcclusion;
	deoglDebugInformation::Ref pDebugInfoSolidShadowSplit;
	deoglDebugInformation::Ref pDebugInfoSolidShadowSplitContent;
	deoglDebugInformation::Ref pDebugInfoSolidShadowSplitLODLevels;
	deoglDebugInformation::Ref pDebugInfoSolidShadowSplitClear;
	deoglDebugInformation::Ref pDebugInfoSolidShadowSplitTask;
	deoglDebugInformation::Ref pDebugInfoSolidShadowSplitRender;
	deoglDebugInformation::Ref pDebugInfoSolidShadowGI;
	deoglDebugInformation::Ref pDebugInfoSolidLight;
	
	deoglDebugInformation::Ref pDebugInfoTransparentDetail;
	deoglDebugInformation::Ref pDebugInfoTransparentLight;
	
	
	
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
	inline const deoglPipeline *GetPipelineOccMesh() const{ return pPipelineOccMesh; }
	
	/** Render lights. */
	void RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask, bool xray );
	
	/** Render sky light ambient occlusion. */
	void RenderAO( deoglRenderPlan &plan );
	
	/** Render sky light. */
	void RenderLight( deoglRenderPlanSkyLight &plan, bool solid, const deoglRenderPlanMasked *mask, bool xray );
	
	/** Render shadows maps. */
	void RenderShadows( deoglRenderPlanSkyLight &plan, bool solid, const deoglRenderPlanMasked *mask );
	
	/** Render shadow map. */
	void RenderShadowMap( deoglRenderPlanSkyLight &plan, deoglShadowMapper &shadowMapper );
	
	/** Render GI shadows. */
	void RenderGIShadows( deoglRenderPlanSkyLight &plan, deoglShadowMapper &shadowMapper );
	
	/** Render GI shadow map. */
	void RenderGIShadowMap( deoglShadowMapper &shadowMapper, deoglRenderTask &renderTask,
		int shadowMapSize, bool clearBackFaceFragments );
	
	void RenderGIShadowMap( deoglShadowMapper &shadowMapper, deoglComputeRenderTask &renderTask,
		int shadowMapSize, bool clearBackFaceFragments );
	
	/** Update light shader parameter block. */
	void UpdateLightParamBlock( const deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanSkyLight &plan );
	
	/** Updates instance shader parameter block. */
	void UpdateInstanceParamBlock( const deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
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
