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
private:
	const deoglPipeline *pPipelineUpdateElements;
	const deoglPipeline *pPipelineUpdateElementGeometries;
	const deoglPipeline *pPipelineFindContentNode;
	const deoglPipeline *pPipelineFindContentElement;
	const deoglPipeline *pPipelineFindContentSkyLight;
	const deoglPipeline *pPipelineFindContentSkyLightGI;
	const deoglPipeline *pPipelineFindGeometries;
	const deoglPipeline *pPipelineUpdateCullResultSet;
	const deoglPipeline *pPipelineUpdateCullResultClear;
	const deoglPipeline *pPipelineBuildRenderTask;
	const deoglPipeline *pPipelineSortRenderTask;
	
	deoglSPBlockSSBO::Ref pSSBOUpdateElements;
	deoglSPBlockSSBO::Ref pSSBOUpdateElementGeometries;
	deoglSPBlockSSBO::Ref pSSBOUpdateIndices;
	deoglSPBlockSSBO::Ref pSSBOElementCullResult;
	deoglSPBlockSSBO::Ref pSSBOVisibleGeometries;
	
	
	
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
	/** SSBO update elements. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOUpdateElements() const{ return pSSBOUpdateElements; }
	
	/** SSBO update element geometries. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOUpdateElementGeometries() const{ return pSSBOUpdateElementGeometries; }
	
	/** SSBO update element geometry index. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOUpdateIndices() const{ return pSSBOUpdateIndices; }
	
	/** SSBO element cull result. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOElementCullResult() const{ return pSSBOElementCullResult; }
	
	/** SSBO visible geometries. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOVisibleGeometries() const{ return pSSBOVisibleGeometries; }
	
	/** Update elements. */
	void UpdateElements( const deoglRenderPlan &plan );
	
	/** Update element geometries. */
	void UpdateElementGeometries( const deoglRenderPlan &plan );
	
	/** Find content. */
	void FindContent( const deoglRenderPlan &plan );
	void FindContentSkyLight( const deoglRenderPlanSkyLight &planLight );
	void FindContentSkyLightGI( const deoglRenderPlanSkyLight &planLight );
	
	/** Clear cull result. */
	void ClearCullResult( const deoglRenderPlan &plan );
	
	/** Update cull result. */
	void UpdateCullResult( const deoglRenderPlan &plan, const deoglSPBlockUBO &findConfig,
		const deoglSPBlockSSBO &visibleElements, const deoglSPBlockSSBO &counters, bool clear );
	
	/** Find geometries. */
	void FindGeometries( const deoglRenderPlan &plan, const deoglSPBlockSSBO &counters );
	
	/** Build render task. */
	void BuildRenderTask( const deoglRenderPlan &plan, const deoglSPBlockSSBO &counters,
		deoglComputeRenderTask &renderTask, int dispatchOffset );
	
	/** Sort render task. */
	void SortRenderTask( deoglComputeRenderTask &renderTask );
	/*@}*/
	
	
	
protected:
};

#endif
