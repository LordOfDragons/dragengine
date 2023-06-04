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
	const deoglPipeline *pPipelineDebugProbeUpdatePass1;
	const deoglPipeline *pPipelineDebugProbeUpdatePass2;
	
	deoglSPBSingleUse::Ref pUBORenderLightSingleUse;
	deoglSPBlockUBO::Ref pUBORenderLight;
	
	
	
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
