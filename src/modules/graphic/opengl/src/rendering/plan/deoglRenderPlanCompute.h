/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
