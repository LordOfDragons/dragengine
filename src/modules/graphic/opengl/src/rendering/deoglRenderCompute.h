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

#ifndef _DEOGLRENDERCOMPUTE_H_
#define _DEOGLRENDERCOMPUTE_H_

#include "deoglRenderBase.h"

class deoglRenderPlanSkyLight;
class deoglComputeRenderTask;


/**
 * Compute stuff used by deoglRenderPlan outside of actual rendering.
 */
class deoglRenderCompute : public deoglRenderBase{
public:
	/** Counters. */
	enum eCounters{
		ecVisibleGeometries,
		ecRenderTaskSubInstanceGroups,
		ecTempCounter
	};
	
	/** Counters struct. */
	struct sCounters{
		uint32_t workGroupSize[ 3 ];
		uint32_t counter;
	};
	
	
	
private:
	const deoglPipeline *pPipelineUpdateElements;
	const deoglPipeline *pPipelineUpdateElementGeometries;
	const deoglPipeline *pPipelineClearGeometries;
	const deoglPipeline *pPipelineFindContentNode;
	const deoglPipeline *pPipelineFindContentElement;
	const deoglPipeline *pPipelineFindContentSkyLight;
	const deoglPipeline *pPipelineFindContentSkyLightGI;
	const deoglPipeline *pPipelineFindGeometries;
	const deoglPipeline *pPipelineFindGeometriesSkyShadow;
	const deoglPipeline *pPipelineUpdateCullResultSet;
	const deoglPipeline *pPipelineUpdateCullResultSetOcclusion;
	const deoglPipeline *pPipelineUpdateCullResultClear;
	const deoglPipeline *pPipelineBuildRenderTask;
	const deoglPipeline *pPipelineBuildRenderTaskOcclusion;
	const deoglPipeline *pPipelineSortRenderTask;
	const deoglPipeline *pPipelineRenderTaskSubInstGroup[ 3 ];
	
	deoglSPBlockSSBO::Ref pSSBOCounters;
	deoglSPBlockSSBO::Ref pSSBOUpdateElements;
	deoglSPBlockSSBO::Ref pSSBOUpdateElementGeometries;
	deoglSPBlockSSBO::Ref pSSBOUpdateIndices;
	deoglSPBlockSSBO::Ref pSSBOClearGeometries;
	deoglSPBlockSSBO::Ref pSSBOElementCullResult;
	deoglSPBlockSSBO::Ref pSSBOVisibleGeometries;
	deoglSPBlockSSBO::Ref pSSBORenderTaskSubInstGroups;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderCompute( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderCompute();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** SSBO counters. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOCounters() const{ return pSSBOCounters; }
	
	/** SSBO update elements. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOUpdateElements() const{ return pSSBOUpdateElements; }
	
	/** SSBO update element geometries. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOUpdateElementGeometries() const{ return pSSBOUpdateElementGeometries; }
	
	/** SSBO update element geometry index. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOUpdateIndices() const{ return pSSBOUpdateIndices; }
	
	/** SSBO clear element geometries. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOClearGeometries() const{ return pSSBOClearGeometries; }
	
	/** SSBO element cull result. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOElementCullResult() const{ return pSSBOElementCullResult; }
	
	/** SSBO visible geometries. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOVisibleGeometries() const{ return pSSBOVisibleGeometries; }
	
	/** Counters dispatch offset. */
	int CounterDispatchOffset( eCounters counter ) const;
	
	/** Update elements. */
	void UpdateElements( const deoglRenderPlan &plan );
	
	/** Update element geometries. */
	void UpdateElementGeometries( const deoglRenderPlan &plan );
	
	/** Clear element geometries. */
	void ClearGeometries( const deoglRenderPlan &plan );
	
	/** Find content. */
	void FindContent( const deoglRenderPlan &plan );
	void FindContentSkyLight( const deoglRenderPlanSkyLight &planLight );
	void FindContentSkyLightGIStatic( const deoglRenderPlanSkyLight &planLight );
	void FindContentSkyLightGIDynamic( const deoglRenderPlanSkyLight &planLight );
	
	/** Clear cull result. */
	void ClearCullResult( const deoglRenderPlan &plan );
	
	/** Update cull result. */
	void UpdateCullResult( const deoglRenderPlan &plan, const deoglSPBlockUBO &findConfig,
		const deoglSPBlockSSBO &visibleElements, const deoglSPBlockSSBO &counters, int lodLayer );
	
	void UpdateCullResultOcclusion( const deoglRenderPlan &plan, const deoglSPBlockUBO &findConfig,
		const deoglSPBlockSSBO &visibleElements, const deoglSPBlockSSBO &counters );
	
	/** Find geometries. */
	void FindGeometries( const deoglRenderPlan &plan );
	void FindGeometriesSkyShadow( const deoglRenderPlan &plan );
	
	/** Build render task. */
	void BuildRenderTask( const deoglRenderPlan &plan, deoglComputeRenderTask &renderTask );
	void BuildRenderTaskOcclusion( const deoglRenderPlan &plan, deoglComputeRenderTask &renderTask );
	void BuildRenderTaskSkyShadow( const deoglRenderPlanSkyLight &planLight, int layer );
	/*@}*/
	
	
	
protected:
	void SortRenderTask( deoglComputeRenderTask &renderTask );
	void pDebugPrintSSBOGeometries( const deoglRenderPlan &plan, const char *prefix );
};

#endif
