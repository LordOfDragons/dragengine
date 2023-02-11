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

#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../world/deoglWorldCSOctree.h"

class deoglRenderPlan;


/**
 * Render Plan compute.
 */
class deoglRenderPlanCompute{
public:
	/** Find config parameters. */
	enum eFindConfigParameters{
		efcpNodeCount,
		efcpElementCount,
		efcpUpdateElementCount,
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
		efcpSplitCount
	};
	
	
	
private:
	/** Counters struct. */
	struct sCounters{
		uint32_t workGroupSize[ 3 ];
		uint32_t counter;
	};
	
	
	deoglRenderPlan &pPlan;
	
	deoglWorldCSOctree::Ref pWorldCSOctree;
	
	deoglSPBlockUBO::Ref pUBOFindConfig;
	
	deoglSPBlockSSBO::Ref pSSBOSearchNodes;
	deoglSPBlockSSBO::Ref pSSBOCounters;
	
	deoglSPBlockSSBO::Ref pSSBOVisibleElements;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render plan compute. */
	deoglRenderPlanCompute( deoglRenderPlan &plan );
	
	/** Clean up render plan compute. */
	~deoglRenderPlanCompute();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** World compute shader octree. */
	inline const deoglWorldCSOctree::Ref &GetWorldCSOctree() const{ return pWorldCSOctree; }
	
	/** Prepare world compute shader octree. */
	void PrepareWorldCompute();
	
	/** Prepare buffers. */
	void PrepareBuffers();
	
	void ReadVisibleElements();
	
	inline const deoglSPBlockUBO::Ref &GetUBOFindConfig() const{ return pUBOFindConfig; }
	
	inline const deoglSPBlockSSBO::Ref &GetSSBOSearchNodes() const{ return pSSBOSearchNodes; }
	
	inline const deoglSPBlockSSBO::Ref &GetSSBOCounters() const{ return pSSBOCounters; }
	
	inline const deoglSPBlockSSBO::Ref &GetSSBOVisibleElements() const{ return pSSBOVisibleElements; }
	/*@}*/
	
	
	
protected:
	void pPrepareFindConfig();
	void pPrepareBuffer( deoglSPBlockSSBO &ssbo, int count );
	void pClearCounters();
	void pSetFrustumPlane( deoglSPBlockUBO &ubo, int index, const decDVector &normal, double distance );
	void pCalculateFrustumBoundaryBox( decDVector &frustumMinExtend, decDVector &frustumMaxExtend );
	float pCalculateErrorScaling();
	void pCullLayerMask( deoglSPBlockUBO &ubo );
};

#endif
