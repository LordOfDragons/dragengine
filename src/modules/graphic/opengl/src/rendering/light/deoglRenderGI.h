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


class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglGIState;
class deoglRenderTask;
class deoglAddToRenderTaskGIMaterial;
class deoglTexture;


/**
 * GI Renderer.
 */
class deoglRenderGI : public deoglRenderLightBase{
private:
	deoglShaderProgram *pShaderResizeMaterials;
	deoglShaderProgram *pShaderTraceRays;
	deoglShaderProgram *pShaderTraceRaysCache;
	deoglShaderProgram *pShaderCopyRayCache;
	deoglShaderProgram *pShaderInitFromRayCache;
	deoglShaderProgram *pShaderCopyProbeIrradiance;
	deoglShaderProgram *pShaderUpdateProbeIrradiance;
	deoglShaderProgram *pShaderUpdateProbeDistance;
	deoglShaderProgram *pShaderClearProbeIrradiance;
	deoglShaderProgram *pShaderClearProbeDistance;
	deoglShaderProgram *pShaderMoveProbes;
	deoglShaderProgram *pShaderDynamicState;
	deoglShaderProgram *pShaderProbeOffset;
	deoglShaderProgram *pShaderProbeExtends;
	deoglShaderProgram *pShaderLight;
	deoglShaderProgram *pShaderLightGIRay;
	deoglShaderProgram *pShaderDebugProbe;
	deoglShaderProgram *pShaderDebugProbeOffset;
	deoglShaderProgram *pShaderDebugProbeUpdatePass1;
	deoglShaderProgram *pShaderDebugProbeUpdatePass2;
	
	deObjectReference pUBORenderLight;
	deObjectReference pUBORenderLightSSS;
	
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTaskGIMaterial *pAddToRenderTask;
	
	
	
	deoglDebugInformation *pDebugInfoGI;
	deoglDebugInformation *pDebugInfoGITraceRays;
	deoglDebugInformation *pDebugInfoGIRenderMaterials;
	deoglDebugInformation *pDebugInfoGIClearProbes;
	deoglDebugInformation *pDebugInfoGIUpdateProbes;
	deoglDebugInformation *pDebugInfoGIMoveProbes;
	deoglDebugInformation *pDebugInfoGIRenderLight;
	deoglDebugInformation *pDebugInfoGIRenderLightGIRay;
	
	
	
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
	inline deoglSPBlockUBO &GetUBORenderLight() const{ return ( deoglSPBlockUBO& )( deObject& )pUBORenderLight; }
	
	/** Render task for GI material. */
	inline deoglRenderTask &GetRenderTask() const{ return *pRenderTask; }
	
	/** Add to render task for GI material. */
	inline deoglAddToRenderTaskGIMaterial &GetAddToRenderTask() const{ return *pAddToRenderTask; }
	
	
	
	/** Trace rays. */
	void TraceRays( deoglRenderPlan &plan );
	
	/** Prepare render light UBO. */
	void PrepareUBORenderLight( deoglRenderPlan &plan );
	void PrepareUBORenderLight( const deoglGIState &giState, const decDVector &position );
	
	/** Render materials. */
	void RenderMaterials( deoglRenderPlan &plan );
	
	/** Resize materials. */
	void ResizeMaterials( deoglTexture &texDiffuse, deoglTexture &texReflectivity,
		deoglTexture &texEmissivity, int mapsPerRow, int rowsPerImage );
	
	/** Clear probes. */
	void ClearProbes( deoglRenderPlan &plan );
	
	/** Update probes. */
	void UpdateProbes( deoglRenderPlan &plan );
	
	/** Move probes. */
	void MoveProbes( deoglRenderPlan &plan );
	
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
	void pSharedTraceRays( deoglRenderPlan &plan );
	void pClearTraceRays();
	void pInitTraceTextures();
	void pActivateGIUBOs();
};

#endif
