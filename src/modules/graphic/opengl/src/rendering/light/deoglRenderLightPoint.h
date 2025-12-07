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

#ifndef _DEOGLRENDERLIGHTPOINT_H_
#define _DEOGLRENDERLIGHTPOINT_H_

#include "deoglRenderLightBase.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../texture/cubemap/deoglCubeMap.h"

class deoglLightShader;
class deoglRenderPlan;
class deoglRenderPlanLight;
class deoglRLight;
class deoglSPBlockUBO;
class deoglShadowMapper;
class deoglRTRenderers;
class deoglRenderPlanMasked;
class deoglVAO;



/**
 * Render point lights.
 */
class deoglRenderLightPoint : public deoglRenderLightBase{
public:
	struct sShadowDepthMaps{
		deoglCubeMap *shadow1Solid;
		deoglCubeMap *shadow1Transp;
		deoglCubeMap *shadow1TranspColor;
		deoglCubeMap *shadow2Solid;
		deoglCubeMap *shadow2Transp;
		deoglCubeMap *shadow2TranspColor;
		deoglCubeMap *shadow1Ambient;
		deoglCubeMap *shadow2Ambient;
		
		sShadowDepthMaps();
	};
	
	struct sShadowParams{
		decDMatrix matrixProjection;
		bool solid;
		bool transparentStaticShadow;
		bool transparentDynamicShadow;
		const deoglCollideList *collideList1;
		const deoglCollideList *collideList2;
		int solidShadowMapSize;
		int transpShadowMapSize;
		int ambientMapSize;
		bool withTransparent;
		float shadowScale;
		float shadowOffset;
		
		/**
		 * Maximum pixel error. If the rendered mesh and the mesh used for shadow calculation
		 * differ shadow artifacts can happen. By definition lod meshes should be conservative
		 * which means they should not stick outside the silhouette if possible. This has the
		 * following consequences. If the render mesh has lower lod level than the shadow mesh
		 * shadow artifacts appear since the rendered mesh faces are inside the higher lod mesh.
		 * This also means if the shadow mesh is using a higher lod level than the rendered
		 * mesh shadow artifacts should not appear. Increasing the lod level though makes the
		 * silhouette of the cast shadow more edgy but also makes rendering the shadow maps
		 * faster. It's difficult to find a good solution for this.
		 */
		int lodMaxPixelError;
		int lodOffset;
		
		deoglCubeMap *copyShadow;
		
		sShadowParams();
	};
	
	
	
private:
	deoglCubeMap::eFaces pCubeFaces[6];
	
	deoglCollideList pCollideList;
	
	const deoglPipeline *pPipelineBoxBoundary1;
	const deoglPipeline *pPipelineBoxBoundary1Ambient;
	const deoglPipeline *pPipelineBoxBoundary2;
	const deoglPipeline *pPipelineOccMap;
	const deoglPipeline *pPipelineCopyDepth;
	
	GLuint pVBOCopyShadow;
	deoglVAO *pVAOCopyShadow;
	
	
	
	deoglDebugInformation::Ref pDebugInfoSolid;
	deoglDebugInformation::Ref pDebugInfoTransparent;
	
	deoglDebugInformation::Ref pDebugInfoSolidDetail;
	deoglDebugInformation::Ref pDebugInfoSolidShadow;
	deoglDebugInformation::Ref pDebugInfoSolidShadowClear;
	deoglDebugInformation::Ref pDebugInfoSolidShadowClearTransp;
	deoglDebugInformation::Ref pDebugInfoSolidShadowFace;
	deoglDebugInformation::Ref pDebugInfoSolidShadowFaceClear;
	deoglDebugInformation::Ref pDebugInfoSolidShadowFaceTask;
	deoglDebugInformation::Ref pDebugInfoSolidShadowFaceRender;
	deoglDebugInformation::Ref pDebugInfoSolidShadowFaceTranspClear;
	deoglDebugInformation::Ref pDebugInfoSolidShadowFaceTranspTask;
	deoglDebugInformation::Ref pDebugInfoSolidShadowFaceTranspRender;
	deoglDebugInformation::Ref pDebugInfoSolidLight;
	
	deoglDebugInformation::Ref pDebugInfoTransparentDetail;
	deoglDebugInformation::Ref pDebugInfoTransparentShadow;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowClear;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowClearTransp;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowFace;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowFaceClear;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowFaceTask;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowFaceRender;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowFaceTranspClear;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowFaceTranspTask;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowFaceTranspRender;
	deoglDebugInformation::Ref pDebugInfoTransparentLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderLightPoint(deoglRenderThread &renderThread, deoglRTRenderers &renderers);
	
	/** Clean up renderer. */
	~deoglRenderLightPoint() override;
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Calculate box boundary for a point light. */
	void CalculateBoxBoundary(deoglRenderPlanLight &planLight);
	
	/** Renders lights. */
	void RenderLights(deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask);
	
	/** Render point light. */
	void RenderLight(deoglRenderPlanLight &planLight, bool solid, const deoglRenderPlanMasked *mask);
	
	/** Activate textures for lighting. */
	void ActivateTextures(deoglRenderPlanLight &planLight, deoglLightShader &shader,
		const sShadowDepthMaps &shadowDepthMaps);
	
	/** Render shadows for a point light. */
	void RenderShadows(deoglRenderPlanLight &planLight, sShadowParams &shadowParams);
	
	/** Render shadow cube map. */
	void RenderShadowMaps(deoglRenderPlanLight &planLight,
		deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams);
	
	/** Render ambient map. */
	void RenderAmbientMap(deoglRenderPlanLight &planLight,
		deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams);
	
	/** Update light shader parameter block. */
	void UpdateLightParamBlock(deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanLight &planLight);
	
	/** Update instance shader parameter block. */
	void UpdateInstanceParamBlock(deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanLight &planLight, sShadowDepthMaps &shadowDepthMaps);
	
	
	
	/** Debug information solid lighting. */
	inline deoglDebugInformation *GetDebugInfoSolid() const{ return pDebugInfoSolid; }
	
	/** Debug information transparent lighting. */
	inline deoglDebugInformation *GetDebugInfoTransparent() const{ return pDebugInfoTransparent; }
	
	
	
	/** Reset debug information. */
	void ResetDebugInfo();
	
	/** Add top level debug information in the right order. */
	void AddTopLevelDebugInfoSolid() override;
	
	/** Add top level debug information in the right order. */
	void AddTopLevelDebugInfoTransparent() override;
	
	/** Developer mode debug information changed. */
	void DevModeDebugInfoChanged() override;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
