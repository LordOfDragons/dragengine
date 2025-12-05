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

#ifndef _DEOGLRENDERERPLANCOMPUTE_H_
#define _DEOGLRENDERERPLANCOMPUTE_H_

#include "../task/deoglComputeRenderTask.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"

class deoglRenderPlan;


/**
 * Render Plan compute.
 */
class deoglRenderPlanCompute : public deObject{
public:
	typedef deTObjectReference<deoglRenderPlanCompute> Ref;
	
	
	/** Find config parameters. */
	enum eFindConfigParameters{
		efcpNodeCount,
		efcpElementCount,
		efcpFrustumPlanes,
		efcpFrustumPlanesAbs,
		efcpFrustumSelect,
		efcpGIMinExtend,
		efcpGIMaxExtend,
		efcpLayerMask,
		efcpCullLayerMask,
		efcpCullFlags,
		efcpCameraPosition,
		efcpCameraView,
		efcpErrorScaling,
		efcpHullEdgeNormal,
		efcpHullEdgeDistance,
		efcpMatrixLightSpace,
		efcpShadowAxis,
		efcpLightShaftFar,
		efcpSplitMinExtend,
		efcpSplitMaxExtend,
		efcpSplitSizeThreshold,
		efcpSplitCount,
		efcpLodFactor,
		efcpLodOffset,
		efcpLodMethod
	};
	
	/** Lod method. */
	enum eLodMethod{
		elmLowest,
		elmHighest,
		elmProjection,
		elmOrthogonal
	};
	
	
	
private:
	deoglRenderPlan &pPlan;
	
	deoglSPBlockUBO::Ref pUBOFindConfig;
	deoglSPBlockSSBO::Ref pSSBOCounters;
	deoglSPBlockSSBO::Ref pSSBOVisibleElements;
	deoglSPBlockSSBO::Ref pSSBOVisibleElements2;
	
	deoglComputeRenderTask::Ref pRTOcclusion;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render plan compute. */
	deoglRenderPlanCompute( deoglRenderPlan &plan );
	
protected:
	/** Clean up render plan compute. */
	~deoglRenderPlanCompute();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Prepare world compute shader octree. */
	void PrepareWorldCompute();
	
	/** Prepare buffers. */
	void PrepareBuffers();
	
	/** Read visible elements. */
	void ReadVisibleElements();
	
	/** Update element geometries. */
	void UpdateElementGeometries();
	
	/** Build occlusion render task. */
	void BuildRTOcclusion( const deoglRenderPlanMasked *mask );
	
	/** Ready occlusion render task. */
	void ReadyRTOcclusion( const deoglRenderPlanMasked *mask );
	
	inline const deoglSPBlockUBO::Ref &GetUBOFindConfig() const{ return pUBOFindConfig; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOCounters() const{ return pSSBOCounters; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOVisibleElements() const{ return pSSBOVisibleElements; }
	inline const deoglSPBlockSSBO::Ref &GetSSBOVisibleElements2() const{ return pSSBOVisibleElements2; }
	
	void SwapVisibleElements();
	
	inline const deoglComputeRenderTask::Ref &GetRTOcclusion() const{ return pRTOcclusion; }
	/*@}*/
	
	
	
protected:
	void pPrepareFindConfig();
	void pPrepareBuffer( deoglSPBlockSSBO &ssbo, int count );
	void pSetFrustumPlane( deoglSPBlockUBO &ubo, int index, const decDVector &normal, double distance );
	void pCalculateFrustumBoundaryBox( decDVector &frustumMinExtend, decDVector &frustumMaxExtend );
	float pCalculateErrorScaling();
	void pCullLayerMask( deoglSPBlockUBO &ubo );
};

#endif
