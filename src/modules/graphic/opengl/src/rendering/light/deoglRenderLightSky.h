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
	//const deoglPipeline *pPipelineAO;
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
	deoglRenderLightSky(deoglRenderThread &renderThread);
	
	/** Clean up renderer. */
	virtual ~deoglRenderLightSky();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Occlusion mesh shader. */
	inline const deoglPipeline *GetPipelineOccMesh() const{ return pPipelineOccMesh; }
	
	/** Render lights. */
	void RenderLights(deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask, bool xray);
	
	/** Render sky light ambient occlusion. */
	void RenderAO(deoglRenderPlan &plan);
	
	/** Render sky light. */
	void RenderLight(deoglRenderPlanSkyLight &plan, bool solid, const deoglRenderPlanMasked *mask, bool xray);
	
	/** Render shadows maps. */
	void RenderShadows(deoglRenderPlanSkyLight &plan, bool solid, const deoglRenderPlanMasked *mask);
	
	/** Render shadow map. */
	void RenderShadowMap(deoglRenderPlanSkyLight &plan, deoglShadowMapper &shadowMapper);
	
	/** Render GI shadows. */
	void RenderGIShadows(deoglRenderPlanSkyLight &plan, deoglShadowMapper &shadowMapper);
	
	/** Render GI shadow map. */
	void RenderGIShadowMap(deoglShadowMapper &shadowMapper, deoglRenderTask &renderTask,
		int shadowMapSize, bool clearBackFaceFragments);
	
	void RenderGIShadowMap(deoglShadowMapper &shadowMapper, deoglComputeRenderTask &renderTask,
		int shadowMapSize, bool clearBackFaceFragments);
	
	/** Update light shader parameter block. */
	void UpdateLightParamBlock(const deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanSkyLight &plan);
	
	/** Updates instance shader parameter block. */
	void UpdateInstanceParamBlock(const deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanSkyLight &plan, int shadowMapSize, int passCount);
	
	
	
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
