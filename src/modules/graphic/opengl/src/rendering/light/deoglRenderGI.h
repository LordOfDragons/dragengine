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

#ifndef _DEOGLRENDERGI_H_
#define _DEOGLRENDERGI_H_

#include "deoglRenderLightBase.h"

#include <dragengine/deObjectReference.h>

class deoglGIState;
class deoglGIBVH;
class deoglRenderTask;
class deoglTexture;


/**
 * GI Renderer.
 */
class deoglRenderGI : public deoglRenderLightBase{
private:
	const deoglPipeline *pPipelineResizeMaterials;
	const deoglPipeline *pPipelineClearTraceRays;
	const deoglPipeline *pPipelineTraceRays;
	const deoglPipeline *pPipelineTraceRaysCache;
	const deoglPipeline *pPipelineCopyRayCache;
	const deoglPipeline *pPipelineInitFromRayCache;
	const deoglPipeline *pPipelineUpdateProbeIrradiance;
	const deoglPipeline *pPipelineUpdateProbeDistance;
	const deoglPipeline *pPipelineClearProbeIrradiance;
	const deoglPipeline *pPipelineClearProbeDistance;
	const deoglPipeline *pPipelineProbeDynamicStates;
	const deoglPipeline *pPipelineProbeOffset;
	const deoglPipeline *pPipelineProbeExtends;
	const deoglPipeline *pPipelineLight;
	const deoglPipeline *pPipelineLightStereo;
	const deoglPipeline *pPipelineLightTransp;
	const deoglPipeline *pPipelineLightTranspStereo;
	const deoglPipeline *pPipelineLightGIRay;
	const deoglPipeline *pPipelineDebugProbe;
	const deoglPipeline *pPipelineDebugProbeXRay;
	const deoglPipeline *pPipelineDebugProbeOffset;
	const deoglPipeline *pPipelineDebugProbeOffsetXRay;
	const deoglPipeline *pPipelineDebugProbeRays;
	const deoglPipeline *pPipelineDebugProbeRaysXRay;
	const deoglPipeline *pPipelineDebugProbeUpdatePass1;
	const deoglPipeline *pPipelineDebugProbeUpdatePass2;
	
	deoglSPBSingleUse::Ref pUBORenderLightSingleUse;
	deoglSPBlockUBO::Ref pUBORenderLight;
	
	deoglSPBlockSSBO::Ref pSSBODebugRayLight;
	int pDebugRayLightIndex;
	
	
	deoglDebugInformation::Ref pDebugInfoGI;
	deoglDebugInformation::Ref pDebugInfoGITraceRays;
	deoglDebugInformation::Ref pDebugInfoGIRenderMaterials;
	deoglDebugInformation::Ref pDebugInfoGIClearProbes;
	deoglDebugInformation::Ref pDebugInfoGIUpdateProbes;
	deoglDebugInformation::Ref pDebugInfoGIMoveProbes;
	deoglDebugInformation::Ref pDebugInfoGIRenderLight;
	deoglDebugInformation::Ref pDebugInfoGIRenderLightGIRay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderGI( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	virtual ~deoglRenderGI();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Render light UBO. */
	inline deoglSPBlockUBO &GetUBORenderLight() const{ return pUBORenderLight; }
	
	
	
	/** Trace rays. */
	void TraceRays( deoglRenderPlan &plan );
	
	/** Prepare render light UBO. */
	void PrepareUBORenderLight( deoglRenderPlan &plan );
	void PrepareUBORenderLight( const deoglGIState &giState, const decDVector &position );
	
	/** Render materials. */
	void RenderMaterials( deoglRenderPlan &plan, const deoglRenderTask &renderTask );
	
	/** Resize materials. */
	void ResizeMaterials( deoglTexture &texDiffuse, deoglTexture &texReflectivity,
		deoglTexture &texEmissivity, int mapsPerRow, int rowsPerImage );
	
	/** Clear probes. */
	void ClearProbes( deoglRenderPlan &plan );
	
	/** Update probes. */
	void UpdateProbes( deoglRenderPlan &plan );
	
	/** Probe offset. */
	void ProbeOffset( deoglRenderPlan &plan );
	
	/** Probe extends. */
	void ProbeExtends( deoglRenderPlan &plan );
	
	/** Render light. */
	void RenderLight( deoglRenderPlan &plan, bool solid );
	
	/** Render light. */
	void RenderLightGIRay( deoglRenderPlan &plan );
	
	/** Debug overlay. */
	void RenderDebugOverlay( deoglRenderPlan &plan );
	
	
	
	/** Reset debug information. */
	void ResetDebugInfo();
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateUBORenderLight();
	void pClearTraceRays( const decPoint &size );
	void pInitTraceTextures( deoglGIBVH &bvh);
	void pActivateGIUBOs();
};

#endif
