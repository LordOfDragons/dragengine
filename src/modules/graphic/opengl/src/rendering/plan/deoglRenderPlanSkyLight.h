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

#ifndef _DEOGLRENDERPLANSKYLIGHT_H_
#define _DEOGLRENDERPLANSKYLIGHT_H_

#include "../task/deoglAddToRenderTask.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglComputeRenderTask.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>


class deoglRenderThread;
class deoglRSkyInstance;
class deoglRSkyInstanceLayer;
#include "parallel/deoglRPTSkyLightFindContent.h"
#include "parallel/deoglRPTSkyLightBuildRT.h"
#include "parallel/deoglRPTSkyLightGIFindContent.h"
#include "parallel/deoglRPTSkyLightGIUpdateRT.h"
class deoglRenderPlan;
class deoglOcclusionTest;


/**
 * Render plan sky light.
 */
class deoglRenderPlanSkyLight{
public:
	struct sShadowLayer{
		float frustumNear;
		float frustumFar;
		float layerBorder;
		float zscale;
		float zoffset;
		decVector minExtend;
		decVector maxExtend;
		decVector position;
		decVector scale;
		decMatrix matrix;
		deoglRenderTask *renderTask;
		deoglAddToRenderTask *addToRenderTask;
		deoglComputeRenderTask::Ref computeRenderTask;
	};
	
	
	
private:
	deoglRenderPlan &pPlan;
	deoglRSkyInstance *pSky;
	deoglRSkyInstanceLayer *pLayer;
	
	deoglCollideList pCollideList;
	decVector pFrustumBoxMinExtend;
	decVector pFrustumBoxMaxExtend;
	
	deoglOcclusionTest *pOcclusionTest;
	
	bool pPlanned;
	bool pUseLight;
	bool pUseShadow;
	int pShadowLayerCount;
	sShadowLayer pShadowLayers[4];
	deoglCollideList pSLCollideList1;
	deoglCollideList pSLCollideList2;
	
	deoglCollideList pGICollideList;
// 	decVector pGIBoxMinExtend;
// 	decVector pGIBoxMaxExtend;
	int pGIShadowSize;
	sShadowLayer pGIShadowLayer;
	bool pGIShadowUpdateStatic;
	deoglRenderTask pGIRenderTaskStatic;
	deoglRenderTask pGIRenderTaskDynamic;
	deoglAddToRenderTask pGIRenderTaskAddStatic;
	deoglAddToRenderTask pGIRenderTaskAddDynamic;
	
	deoglRPTSkyLightFindContent::Ref pTaskFindContent;
	deoglRPTSkyLightBuildRT::Ref pTaskBuildRT1;
	deoglRPTSkyLightBuildRT::Ref pTaskBuildRT2;
	deoglRPTSkyLightGIFindContent::Ref pTaskGIFindContent;
	deoglRPTSkyLightGIUpdateRT::Ref pTaskGIUpdateRT;
	
	deoglSPBlockUBO::Ref pUBOFindConfig;
	deoglSPBlockSSBO::Ref pSSBOCounters;
	deoglSPBlockSSBO::Ref pSSBOVisibleElements;
	deoglSPBlockSSBO::Ref pSSBOVisibleElements2;
	
	deoglSPBlockUBO::Ref pUBOFindConfigGIStatic;
	deoglSPBlockSSBO::Ref pSSBOCountersGIStatic;
	deoglSPBlockSSBO::Ref pSSBOVisibleElementsGIStatic;
	deoglComputeRenderTask::Ref pCRTShadowGIStatic;
	
	deoglSPBlockUBO::Ref pUBOFindConfigGIDynamic;
	deoglSPBlockSSBO::Ref pSSBOCountersGIDynamic;
	deoglSPBlockSSBO::Ref pSSBOVisibleElementsGIDynamic;
	deoglComputeRenderTask::Ref pCRTShadowGIDynamic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render plan light. */
	deoglRenderPlanSkyLight(deoglRenderPlan &plan);
	
	/** Clean up render plan light. */
	~deoglRenderPlanSkyLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render plan. */
	inline deoglRenderPlan &GetPlan() const{ return pPlan; }
	
	/** Sky instance. */
	inline deoglRSkyInstance *GetSky() const{ return pSky; }
	
	/** Sky layer. */
	inline deoglRSkyInstanceLayer *GetLayer() const{ return pLayer; }
	
	/** Set sky layer. */
	void SetLayer(deoglRSkyInstance *sky, deoglRSkyInstanceLayer *layer);
	
	
	
	/** Collide list with potentially shadow casting elements. */
	inline deoglCollideList &GetCollideList(){ return pCollideList; }
	inline const deoglCollideList &GetCollideList() const{ return pCollideList; }
	
	/** Frustum box min extend. */
	inline const decVector &GetFrustumBoxMinExtend() const{ return pFrustumBoxMinExtend; }
	
	/** Frustum box max extend. */
	inline const decVector &GetFrustumBoxMaxExtend() const{ return pFrustumBoxMaxExtend; }
	
	/** Set frustom box extends. */
	void SetFrustumBoxExtend(const decVector &minExtend, const decVector &maxExtend);
	
	
	
	/** Occlusion test. */
	inline deoglOcclusionTest *GetOcclusionTest() const{ return pOcclusionTest; }
	
	/** Set occlusion test. */
	void SetOcclusionTest(deoglOcclusionTest *occlusionTest);
	
	
	
	/** Plan has been planed. */
	inline bool GetPlanned() const{ return pPlanned; }
	
	/** Clear plan is planned. */
	void ClearPlanned();
	
	/** Use light. */
	inline bool GetUseLight() const{ return pUseLight; }
	
	/** Use shadow casting. */
	inline bool GetUseShadow() const{ return pUseShadow; }
	
	/** Shadow layer count. */
	inline int GetShadowLayerCount() const{ return pShadowLayerCount; }
	
	/** Shadow layer at index. */
	sShadowLayer &GetShadowLayerAt(int index);
	const sShadowLayer &GetShadowLayerAt(int index) const;
	
	
	
	/** GI collide list with potentially shadow casting elements. */
	inline deoglCollideList &GetGICollideList(){ return pGICollideList; }
	inline const deoglCollideList &GetGICollideList() const{ return pGICollideList; }
	
	/** GI box min extend. */
// 	inline const decVector &GetGIBoxMinExtend() const{ return pGIBoxMinExtend; }
	
	/** GI box max extend. */
// 	inline const decVector &GetGIBoxMaxExtend() const{ return pGIBoxMaxExtend; }
	
	/** GI shadow size. */
	inline int GetGIShadowSize() const{ return pGIShadowSize; }
	
	/** GI shadow layer. */
	inline sShadowLayer &GetGIShadowLayer(){ return pGIShadowLayer; }
	inline const sShadowLayer &GetGIShadowLayer() const{ return pGIShadowLayer; }
	
	/** Update static GI shadow map. */
	inline bool GetGIShadowUpdateStatic() const{ return pGIShadowUpdateStatic; }
	
	/** GI render task. */
	inline deoglRenderTask &GetGIRenderTaskStatic(){ return pGIRenderTaskStatic; }
	inline const deoglRenderTask &GetGIRenderTaskStatic() const{ return pGIRenderTaskStatic; }
	
	inline deoglRenderTask &GetGIRenderTaskDynamic(){ return pGIRenderTaskDynamic; }
	inline const deoglRenderTask &GetGIRenderTaskDynamic() const{ return pGIRenderTaskDynamic; }
	
	/** GI add to render task. */
	inline deoglAddToRenderTask &GetGIRenderTaskAddStatic(){ return pGIRenderTaskAddStatic; }
	inline const deoglAddToRenderTask &GetGIRenderTaskAddStatic() const{ return pGIRenderTaskAddStatic; }
	
	inline deoglAddToRenderTask &GetGIRenderTaskAddDynamic(){ return pGIRenderTaskAddDynamic; }
	inline const deoglAddToRenderTask &GetGIRenderTaskAddDynamic() const{ return pGIRenderTaskAddDynamic; }
	
	
	
	inline const deoglSPBlockUBO::Ref &GetUBOFindConfig() const{ return pUBOFindConfig; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOCounters() const{ return pSSBOCounters; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOVisibleElements() const{ return pSSBOVisibleElements; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOVisibleElements2() const{ return pSSBOVisibleElements2; }
	
	void SwapVisibleElements();
	
	inline const deoglSPBlockUBO::Ref &GetUBOFindConfigGIStatic() const{ return pUBOFindConfigGIStatic; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOCountersGIStatic() const{ return pSSBOCountersGIStatic; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOVisibleElementsGIStatic() const{ return pSSBOVisibleElementsGIStatic; }
	inline const deoglComputeRenderTask::Ref &GetCRTShadowGIStatic() const{ return pCRTShadowGIStatic; }
	
	inline const deoglSPBlockUBO::Ref &GetUBOFindConfigGIDynamic() const{ return pUBOFindConfigGIDynamic; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOCountersGIDynamic() const{ return pSSBOCountersGIDynamic; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOVisibleElementsGIDynamic() const{ return pSSBOVisibleElementsGIDynamic; }
	inline const deoglComputeRenderTask::Ref &GetCRTShadowGIDynamic() const{ return pCRTShadowGIDynamic; }
	
	
	
	/** Clear. */
	void Clear();
	
	/** Plan sky light. */
	void Plan();
	
	/** Start find content. */
	void StartFindContent();
	
	/** Render occlusion tests. */
	void RenderOcclusionTests();
	
	/** Finish preparations. */
	void FinishPrepare();
	
	/** Start building render task. */
	void StartBuildRT();
	
	/** Wait for GI Update Render Task parallel task to finish. */
	void WaitFinishedGIUpdateRT();
	
	/** Wait for Build Render Task parallel task to finish. */
	void WaitFinishedBuildRT1();
	void WaitFinishedBuildRT2();
	
	void PrepareBuffers();
	void ReadVisibleElements();
	void BuildComputeRenderTasks(bool rebuild = false);
	void FinishReadBackComputeRenderTasks();
	
	/** Clean up after rendering. */
	void CleanUp();
	
	
	
private:
	void pPrepareFindConfig();
	void pPrepareFindConfigGIStatic();
	void pPrepareFindConfigGIDynamic();
	void pPrepareFindConfigGI(deoglSPBlockUBO &ubo);
	void pPrepareBuffer(deoglSPBlockSSBO &ssbo, int count);
	void pClearCounters();
	void pSetWorldComputeParams(deoglSPBlockUBO &ubo);
	void pSetFrustumPlane(deoglSPBlockUBO &ubo, int index, const decDVector &normal, double distance);
	void pSetLightFrustumPlanes(deoglSPBlockUBO &ubo,
		const decDMatrix &frustumMatrix, const decDMatrix &matrixLightSpace);
	void pSetLightFrustumPlane(deoglSPBlockUBO &ubo, const decDMatrix &matrix,
		int &index, const decDVector &normal, double distance);
	void pCalcLightFrustum(const decDMatrix &matrix, decDVector (&frustumPoints)[5]);
	void pLightFrustumBox(const decDVector (&frustumPoints)[5], decDVector &minExtend, decDVector &maxExtend);
	void pFrustumHull(deoglSPBlockUBO &ubo, const decDVector (&frustumPoints)[5]);
	void pCullLayerMask(deoglSPBlockUBO &ubo);
	void pSetSplits(deoglSPBlockUBO &ubo, float backtrack);
	void pSetGISplits(deoglSPBlockUBO &ubo);
	
	void pDetermineShadowParameters();
	void pCalcShadowLayerParams();
	void pWaitFinishedFindContent();
	void pWaitFinishedGIFindContent();
	void pGICalcShadowLayerParams();
	
	void pBuildCRTShadow(int  layer);
	void pBuildCRTShadowGI(deoglComputeRenderTask &renderTask);
	/*@}*/
};

#endif
