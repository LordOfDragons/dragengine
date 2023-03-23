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

#include <stdio.h>
#include <stdlib.h>

#include "deoglRenderCompute.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanSkyLight.h"
#include "task/deoglComputeRenderTask.h"
#include "../deoglMath.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSortStages{
	essLocalSort,
	essLocalDisperse,
	essGlobalFlip,
	essGlobalDisperse
};

enum eSortShaderParameters{
	esspStage,
	esspLaneSize
};

enum eRenderTaskSubInstGroup2Parameters{
	ertsig2pStage,
	ertsig2pLaneSize
};



// Class deoglRenderCompute
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderCompute::deoglRenderCompute( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglPipelineConfiguration pipconf;
	deoglShaderDefines defines, commonDefines;
	
	pipconf.Reset();
	pipconf.SetType( deoglPipelineConfiguration::etCompute );
	
	renderThread.GetShader().SetCommonDefines( commonDefines );
	
	
	// SSBOs
	pSSBOCounters.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etRead ) );
	pSSBOCounters->SetRowMajor( rowMajor );
	pSSBOCounters->SetParameterCount( 2 );
	pSSBOCounters->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // uvec3
	pSSBOCounters->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pSSBOCounters->SetElementCount( 3 );
	pSSBOCounters->MapToStd140();
	
	pSSBOUpdateElements.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etStream ) );
	pSSBOUpdateElements->SetRowMajor( rowMajor );
	pSSBOUpdateElements->SetParameterCount( 11 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeMinExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeFlags ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeMaxExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeUpdateIndex ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeLayerMask ).SetAll( deoglSPBParameter::evtInt, 2, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeFirstGeometry ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeGeometryCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeLodFactors ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeHighestLod ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeCullResult ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElements->GetParameterAt( deoglWorldCompute::espeLodIndex ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElements->MapToStd140();
	
	pSSBOUpdateElementGeometries.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etStream ) );
	pSSBOUpdateElementGeometries->SetRowMajor( rowMajor );
	pSSBOUpdateElementGeometries->SetParameterCount( 9 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espegElement ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espegLod ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espegRenderFilter ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espegSkinTexture ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espegPipelineBase ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espegVao ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espegInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espegSPBInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espegTUCs ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
	pSSBOUpdateElementGeometries->MapToStd140();
	
	pSSBOUpdateIndices.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etStream ) );
	pSSBOUpdateIndices->SetRowMajor( rowMajor );
	pSSBOUpdateIndices->SetParameterCount( 1 );
	pSSBOUpdateIndices->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
	pSSBOUpdateIndices->MapToStd140();
	
	pSSBOClearGeometries.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etStream ) );
	pSSBOClearGeometries->SetRowMajor( rowMajor );
	pSSBOClearGeometries->SetParameterCount( 1 );
	pSSBOClearGeometries->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOClearGeometries->MapToStd430();
	
	pSSBOElementCullResult.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etGpu ) );
	pSSBOElementCullResult->SetRowMajor( rowMajor );
	pSSBOElementCullResult->SetParameterCount( 1 );
	pSSBOElementCullResult->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
	pSSBOElementCullResult->MapToStd140();
	
	pSSBOVisibleGeometries.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etGpu ) );
	pSSBOVisibleGeometries->SetRowMajor( rowMajor );
	pSSBOVisibleGeometries->SetParameterCount( 1 );
	pSSBOVisibleGeometries->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
	pSSBOVisibleGeometries->MapToStd140();
	pSSBOVisibleGeometries->EnsureBuffer();
	
	pSSBORenderTaskSubInstGroups.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etGpu ) );
	pSSBORenderTaskSubInstGroups->SetRowMajor( rowMajor );
	pSSBORenderTaskSubInstGroups->SetParameterCount( 1 );
	pSSBORenderTaskSubInstGroups->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
	pSSBORenderTaskSubInstGroups->MapToStd140();
	pSSBORenderTaskSubInstGroups->EnsureBuffer();
	
	
	// update elements
	pipconf.SetShader( renderThread, "DefRen Plan Update Elements", commonDefines );
	pPipelineUpdateElements = pipelineManager.GetWith( pipconf );
	
	
	// update element geometries
	pipconf.SetShader( renderThread, "DefRen Plan Update Element Geometries", commonDefines );
	pPipelineUpdateElementGeometries = pipelineManager.GetWith( pipconf );
	
	
	// cleanup element geometries
	pipconf.SetShader( renderThread, "DefRen Plan Clear Geometries", commonDefines );
	pPipelineClearGeometries = pipelineManager.GetWith( pipconf );
	
	
	// find content
	defines = commonDefines;
	
	// pipconf.SetShader( renderThread, "DefRen Plan FindContent Node", defines );
	// pPipelineFindContentNode = pipelineManager.GetWith( pipconf );
	
	defines.SetDefines( "CULL_VIEW_FRUSTUM" );
	defines.SetDefines( "CULL_TOO_SMALL" );
	pipconf.SetShader( renderThread, "DefRen Plan FindContent Element", defines );
	pPipelineFindContentElement = pipelineManager.GetWith( pipconf );
	
	
	// find content sky light
	defines = commonDefines;
	defines.SetDefines( "CULL_SKY_LIGHT_FRUSTUM" );
	pipconf.SetShader( renderThread, "DefRen Plan FindContent Element", defines );
	pPipelineFindContentSkyLight = pipelineManager.GetWith( pipconf );
	
	
	// find content sky light GI
	defines = commonDefines;
	defines.SetDefines( "CULL_SKY_LIGHT_GIBOX" );
	pipconf.SetShader( renderThread, "DefRen Plan FindContent Element", defines );
	pPipelineFindContentSkyLightGI = pipelineManager.GetWith( pipconf );
	
	
	// find geometries
	defines = commonDefines;
	pipconf.SetShader( renderThread, "DefRen Plan Find Geometries", defines );
	pPipelineFindGeometries = pipelineManager.GetWith( pipconf );
	
	defines.SetDefines( "WITH_OCCLUSION" );
	pipconf.SetShader( renderThread, "DefRen Plan Find Geometries", defines );
	pPipelineFindGeometriesSkyShadow = pipelineManager.GetWith( pipconf );
	
	
	// update cull result
	defines = commonDefines;
	pipconf.SetShader( renderThread, "DefRen Plan Update Cull Result", defines );
	pPipelineUpdateCullResultSetOcclusion = pipelineManager.GetWith( pipconf );
	
	defines.SetDefines( "WITH_CALC_LOD" );
	pipconf.SetShader( renderThread, "DefRen Plan Update Cull Result", defines );
	pPipelineUpdateCullResultSet = pipelineManager.GetWith( pipconf );
	
	defines = commonDefines;
	defines.SetDefines( "CLEAR_CULL_RESULT" );
	pPipelineUpdateCullResultClear = pipelineManager.GetWith( pipconf );
	
	
	// build render task
	defines = commonDefines;
	pipconf.SetShader( renderThread, "DefRen Plan Build Render Task", defines );
	pPipelineBuildRenderTask = pipelineManager.GetWith( pipconf );
	
	pipconf.SetShader( renderThread, "DefRen Plan Build Render Task Occlusion", defines );
	pPipelineBuildRenderTaskOcclusion = pipelineManager.GetWith( pipconf );
	
	
	// sort render task
	defines = commonDefines;
	pipconf.SetShader( renderThread, "DefRen Plan Sort Render Task", defines );
	pPipelineSortRenderTask = pipelineManager.GetWith( pipconf );
	
	
	// render task sub instance group
	defines = commonDefines;
	pipconf.SetShader( renderThread, "DefRen Plan Render Task SubInstGroup Pass 1", defines );
	pPipelineRenderTaskSubInstGroup[ 0 ] = pipelineManager.GetWith( pipconf );
	
	pipconf.SetShader( renderThread, "DefRen Plan Render Task SubInstGroup Pass 2", defines );
	pPipelineRenderTaskSubInstGroup[ 1 ] = pipelineManager.GetWith( pipconf );
	
	pipconf.SetShader( renderThread, "DefRen Plan Render Task SubInstGroup Pass 3", defines );
	pPipelineRenderTaskSubInstGroup[ 2 ] = pipelineManager.GetWith( pipconf );
}

deoglRenderCompute::~deoglRenderCompute(){
}



// Rendering
//////////////

int deoglRenderCompute::CounterDispatchOffset( deoglRenderCompute::eCounters counter ) const{
	return sizeof( sCounters ) * counter;
}

void deoglRenderCompute::UpdateElements( const deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.UpdateElements" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetUpdateElementCount();
	if( count == 0 ){
		return;
	}
	
	pPipelineUpdateElements->Activate();
	
	pSSBOUpdateElements->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 1 );
	
	pPipelineUpdateElements->GetGlShader().SetParameterUInt( 0, count );
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
}

void deoglRenderCompute::UpdateElementGeometries( const deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.UpdateElementGeometries" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetUpdateElementGeometryCount();
	if( count == 0 ){
		return;
	}
	
	pPipelineUpdateElementGeometries->Activate();
	
	pSSBOUpdateElementGeometries->Activate( 0 );
	pSSBOUpdateIndices->Activate( 1 );
	wcompute.GetSSBOElementGeometries()->Activate( 2 );
	
	pPipelineUpdateElementGeometries->GetGlShader().SetParameterUInt( 0, count );
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
}

void deoglRenderCompute::ClearGeometries( const deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.ClearGeometries" );
	
	deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetClearGeometryCount();
	if( count == 0 ){
		return;
	}
	
	pPipelineClearGeometries->Activate();
	
	pSSBOClearGeometries->Activate( 0 );
	wcompute.GetSSBOElementGeometries()->Activate( 1 );
	
	pPipelineClearGeometries->GetGlShader().SetParameterUInt( 0, count );
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
}

void deoglRenderCompute::FindContent( const deoglRenderPlan &plan ){
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	if( count == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContent" );
	const deoglRenderPlanCompute &planCompute = plan.GetCompute();
	
	pPipelineFindContentElement->Activate();
	
	planCompute.GetUBOFindConfig()->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 0 );
	planCompute.GetSSBOVisibleElements()->Activate( 1 );
	planCompute.GetSSBOCounters()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	
	planCompute.GetSSBOVisibleElements()->GPUFinishedWriting();
	planCompute.GetSSBOCounters()->GPUFinishedWriting();
	planCompute.GetSSBOCounters()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::FindContentSkyLight( const deoglRenderPlanSkyLight &planLight ){
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	if( count == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContentSkyLight" );
	
	pPipelineFindContentSkyLight->Activate();
	
	planLight.GetUBOFindConfig()->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 0 );
	planLight.GetSSBOVisibleElements()->Activate( 1 );
	planLight.GetSSBOCounters()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
}

void deoglRenderCompute::FindContentSkyLightGIStatic( const deoglRenderPlanSkyLight &planLight ){
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	if( count == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContentSkyLightGIStatic" );
	
	pPipelineFindContentSkyLightGI->Activate();
	
	planLight.GetUBOFindConfigGIStatic()->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 0 );
	planLight.GetSSBOVisibleElementsGIStatic()->Activate( 1 );
	planLight.GetSSBOCountersGIStatic()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	
	planLight.GetSSBOVisibleElementsGIStatic()->GPUFinishedWriting();
	planLight.GetSSBOCountersGIStatic()->GPUFinishedWriting();
	planLight.GetSSBOCountersGIStatic()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::FindContentSkyLightGIDynamic( const deoglRenderPlanSkyLight &planLight ){
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	if( count == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContentSkyLightGIDynamic" );
	
	pPipelineFindContentSkyLightGI->Activate();
	
	planLight.GetUBOFindConfigGIDynamic()->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 0 );
	planLight.GetSSBOVisibleElementsGIDynamic()->Activate( 1 );
	planLight.GetSSBOCountersGIDynamic()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	
	planLight.GetSSBOVisibleElementsGIDynamic()->GPUFinishedWriting();
	planLight.GetSSBOCountersGIDynamic()->GPUFinishedWriting();
	planLight.GetSSBOCountersGIDynamic()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::ClearCullResult( const deoglRenderPlan &plan ){
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	if( count == 0 ){
		return;
	}
	
	if( count > pSSBOElementCullResult->GetElementCount() ){
		pSSBOElementCullResult->SetElementCount( count );
		pSSBOElementCullResult->EnsureBuffer();
	}
	
	const deoglDebugTraceGroup debugTrace( GetRenderThread(), "Compute.ClearCullResult" );
	pSSBOElementCullResult->ClearDataUInt( 0, ( count - 1 ) / 4 + 1, 0, 0, 0, 0 );
}

void deoglRenderCompute::UpdateCullResult( const deoglRenderPlan &plan, const deoglSPBlockUBO &findConfig,
const deoglSPBlockSSBO &visibleElements, const deoglSPBlockSSBO &counters, int lodLayer ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.UpdateCullResult" );
	
	pPipelineUpdateCullResultSet->Activate();
	
	pPipelineUpdateCullResultSet->GetGlShader().SetParameterUInt( 0, lodLayer );
	
	findConfig.Activate( 0 );
	plan.GetWorld()->GetCompute().GetSSBOElements()->Activate( 0 );
	visibleElements.Activate( 1 );
	counters.Activate( 2 );
	pSSBOElementCullResult->Activate( 3 );
	
	counters.ActivateDispatchIndirect();
	OGL_CHECK( renderThread, pglDispatchComputeIndirect( 0 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
	counters.DeactivateDispatchIndirect();
}

void deoglRenderCompute::UpdateCullResultOcclusion( const deoglRenderPlan &plan,
const deoglSPBlockUBO &findConfig, const deoglSPBlockSSBO &visibleElements, const deoglSPBlockSSBO &counters ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.UpdateCullResultOcclusion" );
	
	pPipelineUpdateCullResultSetOcclusion->Activate();
	
	findConfig.Activate( 0 );
	plan.GetWorld()->GetCompute().GetSSBOElements()->Activate( 0 );
	visibleElements.Activate( 1 );
	counters.Activate( 2 );
	pSSBOElementCullResult->Activate( 3 );
	
	counters.ActivateDispatchIndirect();
	OGL_CHECK( renderThread, pglDispatchComputeIndirect( 0 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
	counters.DeactivateDispatchIndirect();
}

void deoglRenderCompute::FindGeometries( const deoglRenderPlan &plan ){
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementGeometryCount();
	if( count == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindGeometries" );
	
	if( count > pSSBOVisibleGeometries->GetElementCount() ){
		pSSBOVisibleGeometries->SetElementCount( count );
		pSSBOVisibleGeometries->EnsureBuffer();
	}
	
	pSSBOCounters->ClearDataUInt( ecVisibleGeometries, 1, 0, 1, 1, 0 ); // workGroupSize.xyz, count
	
	pPipelineFindGeometries->Activate();
	
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	pSSBOElementCullResult->Activate( 1 );
	pSSBOVisibleGeometries->Activate( 2 );
	pSSBOCounters->ActivateAtomic( 0 );
	
	pPipelineFindGeometries->GetGlShader().SetParameterUInt( 0, count );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
}

void deoglRenderCompute::FindGeometriesSkyShadow( const deoglRenderPlan &plan ){
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementGeometryCount();
	if( count == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindGeometriesSkyShadow" );
	
	if( count > pSSBOVisibleGeometries->GetElementCount() ){
		pSSBOVisibleGeometries->SetElementCount( count );
		pSSBOVisibleGeometries->EnsureBuffer();
	}
	
	pSSBOCounters->ClearDataUInt( ecVisibleGeometries, 1, 0, 1, 1, 0 ); // workGroupSize.xyz, count
	
	pPipelineFindGeometriesSkyShadow->Activate();
	
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	pSSBOElementCullResult->Activate( 1 );
	pSSBOVisibleGeometries->Activate( 2 );
	pSSBOCounters->ActivateAtomic( 0 );
	
	pPipelineFindGeometries->GetGlShader().SetParameterUInt( 0, count );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
}

void deoglRenderCompute::BuildRenderTask( const deoglRenderPlan &plan, deoglComputeRenderTask &renderTask ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.BuildRenderTask" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	DEASSERT_TRUE( wcompute.GetElementGeometryCount() > 0 )
	
	pPipelineBuildRenderTask->Activate();
	
	renderTask.GetUBOConfig()->Activate( 0 );
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	renderThread.GetShader().GetSSBOSkinTextures()->Activate( 1 );
	pSSBOVisibleGeometries->Activate( 2 );
	pSSBOCounters->Activate( 3 );
	renderTask.GetSSBOSteps()->Activate( 4 );
	renderTask.GetSSBOCounters()->ActivateAtomic( 0 );
	
	pSSBOCounters->ActivateDispatchIndirect();
	
	deoglShaderCompiled &shaderBuild = pPipelineBuildRenderTask->GetGlShader();
	const int dispatchOffset = CounterDispatchOffset( ecVisibleGeometries );
	const int passCount = renderTask.GetPassCount();
	int i;
	
	for( i=0; i<passCount; i++ ){
		shaderBuild.SetParameterUInt( 0, i );
		OGL_CHECK( renderThread, pglDispatchComputeIndirect( dispatchOffset ) );
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT ) );
	}
	
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	pSSBOCounters->DeactivateDispatchIndirect();
	
	SortRenderTask( renderTask );
	
	renderTask.GetSSBOCounters()->GPUFinishedWriting();
	renderTask.GetSSBOCounters()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::BuildRenderTaskOcclusion( const deoglRenderPlan &plan, deoglComputeRenderTask &renderTask ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.BuildRenderTaskOcclusion" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementGeometryCount();
	if( count == 0 ){
		return;
	}
	
	pPipelineBuildRenderTaskOcclusion->Activate();
	
	renderTask.GetUBOConfig()->Activate( 0 );
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	pSSBOElementCullResult->Activate( 1 );
	renderTask.GetSSBOSteps()->Activate( 2 );
	renderTask.GetSSBOCounters()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	
	SortRenderTask( renderTask );
	
	renderTask.GetSSBOCounters()->GPUFinishedWriting();
	renderTask.GetSSBOCounters()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::BuildRenderTaskSkyShadow( const deoglRenderPlanSkyLight &planLight, int layer ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.BuildRenderTaskSkyShadow" );
	
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	const int count = wcompute.GetElementGeometryCount();
	if( count == 0 ){
		return;
	}
	
	const deoglRenderPlanSkyLight::sShadowLayer &sl = planLight.GetShadowLayerAt( layer );
	deoglComputeRenderTask &renderTask = sl.computeRenderTask;
	
	// pass 1, occlusion meshes
	pPipelineBuildRenderTaskOcclusion->Activate();
	
	renderTask.GetUBOConfig()->Activate( 0 );
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	pSSBOElementCullResult->Activate( 1 );
	renderTask.GetSSBOSteps()->Activate( 2 );
	renderTask.GetSSBOCounters()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT ) );
	
	// pass 2+, other geometry
	pPipelineBuildRenderTask->Activate();
	
	renderTask.GetUBOConfig()->Activate( 0 );
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	renderThread.GetShader().GetSSBOSkinTextures()->Activate( 1 );
	pSSBOVisibleGeometries->Activate( 2 );
	pSSBOCounters->Activate( 3 );
	renderTask.GetSSBOSteps()->Activate( 4 );
	renderTask.GetSSBOCounters()->ActivateAtomic( 0 );
	
	pSSBOCounters->ActivateDispatchIndirect();
	
	deoglShaderCompiled &shaderBuild = pPipelineBuildRenderTask->GetGlShader();
	const int dispatchOffset = CounterDispatchOffset( ecVisibleGeometries );
	const int passCount = renderTask.GetPassCount();
	int i;
	
	for( i=1; i<passCount; i++ ){
		shaderBuild.SetParameterUInt( 0, i );
		OGL_CHECK( renderThread, pglDispatchComputeIndirect( dispatchOffset ) );
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT ) );
	}
	
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	pSSBOCounters->DeactivateDispatchIndirect();
	
	// sort and finish
	SortRenderTask( sl.computeRenderTask );
	
	renderTask.GetSSBOCounters()->GPUFinishedWriting();
	renderTask.GetSSBOCounters()->GPUReadToCPU( 1 );
}



// Protected Functions
////////////////////////

void deoglRenderCompute::SortRenderTask( deoglComputeRenderTask &renderTask ){
	const int maxCount = renderTask.GetSSBOSteps()->GetElementCount();
	if( maxCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.SortRenderTask" );
	
	
	// sort render task
	deoglDebugTraceGroup dtSort( renderThread, "SortSteps" );
	pPipelineSortRenderTask->Activate();
	
	renderTask.GetSSBOCounters()->Activate( 0 );
	renderTask.GetSSBOSteps()->Activate( 1 );
	
	const int maxLanSize = 128; // 64 work group size * 2
	const int potStepCount = oglPotOf( maxCount );
	const int workGroupCount = ( ( potStepCount - 1 ) / maxLanSize ) + 1;
	
	// local sorting
	deoglShaderCompiled &shaderSort = pPipelineSortRenderTask->GetGlShader();
	shaderSort.SetParameterInt( esspStage, essLocalSort );
	shaderSort.SetParameterUInt( esspLaneSize, maxLanSize );
	OGL_CHECK( renderThread, pglDispatchCompute( workGroupCount, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
	
	// global sorting
	int laneSize, halfLaneSize;
	for( laneSize = maxLanSize * 2; laneSize <= potStepCount; laneSize *= 2 ){
		shaderSort.SetParameterInt( esspStage, essGlobalFlip );
		shaderSort.SetParameterUInt( esspLaneSize, laneSize );
		OGL_CHECK( renderThread, pglDispatchCompute( workGroupCount, 1, 1 ) );
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
		
		for( halfLaneSize = laneSize / 2; halfLaneSize > 1; halfLaneSize /= 2 ){
			shaderSort.SetParameterUInt( esspLaneSize, halfLaneSize );
			
			if( halfLaneSize <= maxLanSize ){
				shaderSort.SetParameterInt( esspStage, essLocalDisperse );
				OGL_CHECK( renderThread, pglDispatchCompute( workGroupCount, 1, 1 ) );
				OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
				break;
			}
			
			shaderSort.SetParameterInt( esspStage, essGlobalDisperse );
			OGL_CHECK( renderThread, pglDispatchCompute( workGroupCount, 1, 1 ) );
			OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
		}
	}
	dtSort.Close();
	
	
	
	// subinstance grouping
	deoglDebugTraceGroup dtSubInst( renderThread, "SubInstGrouping" );
	const int maxECount = ( ( maxCount - 1 ) / 4 ) + 1;
	if( maxECount > pSSBORenderTaskSubInstGroups->GetElementCount() ){
		pSSBORenderTaskSubInstGroups->SetElementCount( maxECount );
		pSSBORenderTaskSubInstGroups->EnsureBuffer();
	}
	pSSBOCounters->ClearDataUInt( ecRenderTaskSubInstanceGroups, 1, 0, 1, 1, 0 );
	
	// pass 1
	pPipelineRenderTaskSubInstGroup[ 0 ]->Activate();
	
	renderTask.GetSSBOSteps()->Activate( 0 );
	renderTask.GetSSBOCounters()->Activate( 1 );
	pSSBORenderTaskSubInstGroups->Activate( 2 );
	pSSBOCounters->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( ( maxCount - 1 ) / 64 ) + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT ) );
	
	// pass 2
	deoglDebugTraceGroup dtSubInst2( renderThread, "Pass2" );
	pPipelineRenderTaskSubInstGroup[ 1 ]->Activate();
	
	pSSBOCounters->Activate( 0 );
	pSSBORenderTaskSubInstGroups->Activate( 1 );
	
	deoglShaderCompiled &shaderSortSubInstGroup = pPipelineRenderTaskSubInstGroup[ 1 ]->GetGlShader();
	
	shaderSortSubInstGroup.SetParameterInt( ertsig2pStage, essLocalSort );
	shaderSortSubInstGroup.SetParameterUInt( ertsig2pLaneSize, maxLanSize );
	OGL_CHECK( renderThread, pglDispatchCompute( workGroupCount, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
	
	for( laneSize = maxLanSize * 2; laneSize <= potStepCount; laneSize *= 2 ){
		shaderSortSubInstGroup.SetParameterInt( ertsig2pStage, essGlobalFlip );
		shaderSortSubInstGroup.SetParameterUInt( ertsig2pLaneSize, laneSize );
		OGL_CHECK( renderThread, pglDispatchCompute( workGroupCount, 1, 1 ) );
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
		
		for( halfLaneSize = laneSize / 2; halfLaneSize > 1; halfLaneSize /= 2 ){
			shaderSortSubInstGroup.SetParameterUInt( ertsig2pLaneSize, halfLaneSize );
			
			if( halfLaneSize <= maxLanSize ){
				shaderSortSubInstGroup.SetParameterInt( ertsig2pStage, essLocalDisperse );
				OGL_CHECK( renderThread, pglDispatchCompute( workGroupCount, 1, 1 ) );
				OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
				break;
			}
			
			shaderSortSubInstGroup.SetParameterInt( ertsig2pStage, essGlobalDisperse );
			OGL_CHECK( renderThread, pglDispatchCompute( workGroupCount, 1, 1 ) );
			OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
		}
	}
	dtSubInst2.Close();
	
	// pass 3
	pPipelineRenderTaskSubInstGroup[ 2 ]->Activate();
	
	pSSBORenderTaskSubInstGroups->Activate( 0 );
	pSSBOCounters->Activate( 1 );
	renderTask.GetSSBOCounters()->Activate( 2 );
	renderTask.GetSSBOSteps()->Activate( 3 );
	
	pSSBOCounters->ActivateDispatchIndirect();
	OGL_CHECK( renderThread, pglDispatchComputeIndirect( CounterDispatchOffset( ecRenderTaskSubInstanceGroups ) ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
	pSSBOCounters->DeactivateDispatchIndirect();
	
	renderTask.GetSSBOSteps()->GPUFinishedWriting();
	renderTask.GetSSBOSteps()->GPUReadToCPU();
}

/*

// set initial value for each data point. if a data point is valid a value of 1
// is written to indicate incrementing the final output index by 1. if the data
// point is invalid (has to be skipped) a value of 0 is written. this does not
// increment the final output index and thus skips the data point later on
// 
// note: this pass does not require clearing since it writes each value

uint index = gl_GlobalInvocationID.x;
if(index >= pDataPointCount) return;

finalIndices[index] = isValid(input[index]) ? uint(1) : uint(0)



// downsample data points using sum operator. this requires a second ssbo with
// the same size as the data points ssbo. the downsampled data values are packed
// into this ssbo. this leaves the last data point unused since we need the
// downsampling only down to a size of 2 not 1.
// 
// the most simple solution is using a multipass algorithm with glMemoryBarrier()
// between each call. the data point count is halved each round (including round 0).
// furthermore two offsets are set in a shader uniform to offset reading and writing
// 
// note: for round 0 the input ssbo is the data point ssbo and the output ssbo
//       is the downsample ssbo. for all other rounds both are the downsample ssbo
// 
// note: for 100k data points this requires 16 rounds, for 200k 17 rounds

uint index = gl_GlobalInvocationID.x;
if(index >= pDataPointCount) return;

output[pWriteOffset + index] = input[pReadOffset + index]) + input[pReadOffset + index + uint(1)];



// update data points adding downsampled values. this can be done using a single
// shader run since we only read multiple values and sum them up
// 
// note: for 100k data points this requires 16 loops, for 200k 17 loops

uint index = gl_GlobalInvocationID.x;
if(index >= pDataPointCount) return;

// pDownsampleSize; // size of first down sample area
uint value = finalIndices[index];
uint blockOffset = uint(0);
uint offset = index;
uint n;

for(n=pDownsampleSize; n>1; n/=2){
	offset /= uint(2);
	value += downsample[blockOffset + offset];
	blockOffset += n;
}
finalIndices[index] = value;



// now the data points can be written in the compacted form to the final ssbo.
// this has to be a different ssbo since the processing order is unknown
// 
// note: if isValid() is more complex an alternate solution would be to use:
//       isValid = index == 0 || finalIndices[index] > finalIndices[index-1]

uint index = gl_GlobalInvocationID.x;
if(index >= pDataPointCount) return;

if(isValid(input[index])){
	result[finalIndices[index]] = input[index];
}

*/


// Protected Functions
////////////////////////
