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
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espetElement ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espetLod ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espetRenderFilter ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espetSkinTexture ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espetPipelineBase ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espetVao ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espetInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espetSPBInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOUpdateElementGeometries->GetParameterAt( deoglWorldCompute::espetTUCs ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
	pSSBOUpdateElementGeometries->MapToStd140();
	
	pSSBOUpdateIndices.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etStream ) );
	pSSBOUpdateIndices->SetRowMajor( rowMajor );
	pSSBOUpdateIndices->SetParameterCount( 1 );
	pSSBOUpdateIndices->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
	pSSBOUpdateIndices->MapToStd140();
	
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
	
	
	// update elements
	pipconf.SetShader( renderThread, "DefRen Plan Update Elements", commonDefines );
	pPipelineUpdateElements = pipelineManager.GetWith( pipconf );
	
	
	// update element geometries
	pipconf.SetShader( renderThread, "DefRen Plan Update Element Geometries", commonDefines );
	pPipelineUpdateElementGeometries = pipelineManager.GetWith( pipconf );
	
	
	// find content
	defines = commonDefines;
	
	// pipconf.SetShader( renderThread, "DefRen Plan FindContent Node", defines );
	// pPipelineFindContentNode = pipelineManager.GetWith( pipconf );
	
	defines.SetDefines( "DIRECT_ELEMENTS" );
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
	
	
	// clear cull result
	defines = commonDefines;
	pipconf.SetShader( renderThread, "DefRen Plan Clear Cull Result", defines );
	pPipelineClearCullResult = pipelineManager.GetWith( pipconf );
	
	// update cull result
	pipconf.SetShader( renderThread, "DefRen Plan Update Cull Result", defines );
	pPipelineUpdateCullResultSet = pipelineManager.GetWith( pipconf );
	
	defines.SetDefines( "CLEAR_CULL_RESULT" );
	pPipelineUpdateCullResultClear = pipelineManager.GetWith( pipconf );
	
	
	// build render task
	defines = commonDefines;
	pipconf.SetShader( renderThread, "DefRen Plan Build Render Task", defines );
	pPipelineBuildRenderTask = pipelineManager.GetWith( pipconf );
}

deoglRenderCompute::~deoglRenderCompute(){
}



// Rendering
//////////////

void deoglRenderCompute::UpdateElements( const deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.UpdateElements" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	if( wcompute.GetUpdateElementCount() == 0 ){
		return;
	}
	
	pPipelineUpdateElements->Activate();
	
	pSSBOUpdateElements->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 1 );
	
	pPipelineUpdateElements->GetGlShader().SetParameterUInt( 0, wcompute.GetUpdateElementCount() );
	OGL_CHECK( renderThread, pglDispatchCompute(
		( wcompute.GetUpdateElementCount() - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
}

void deoglRenderCompute::UpdateElementGeometries( const deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.UpdateElementGeometries" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	if( wcompute.GetUpdateElementGeometryCount() == 0 ){
		return;
	}
	
	pPipelineUpdateElementGeometries->Activate();
	
	pSSBOUpdateElementGeometries->Activate( 0 );
	pSSBOUpdateIndices->Activate( 1 );
	wcompute.GetSSBOElementGeometries()->Activate( 2 );
	
	pPipelineUpdateElementGeometries->GetGlShader().SetParameterUInt( 0, wcompute.GetUpdateElementCount() );
	OGL_CHECK( renderThread, pglDispatchCompute(
		( wcompute.GetUpdateElementGeometryCount() - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
}

void deoglRenderCompute::FindContent( const deoglRenderPlan &plan ){
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	if( wcompute.GetElementCount() == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContent" );
	const deoglRenderPlanCompute &planCompute = plan.GetCompute();
	
	// find content
	pPipelineFindContentElement->Activate();
	
	planCompute.GetUBOFindConfig()->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 0 );
	planCompute.GetSSBOVisibleElements()->Activate( 1 );
	planCompute.GetSSBOCounters()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( wcompute.GetElementCount() - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	
	planCompute.GetSSBOVisibleElements()->GPUFinishedWriting();
	planCompute.GetSSBOCounters()->GPUFinishedWriting();
	planCompute.GetSSBOCounters()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::FindContentSkyLight( const deoglRenderPlanSkyLight &planLight ){
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	if( wcompute.GetElementCount() == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContentSkyLight" );
	
	pPipelineFindContentSkyLight->Activate();
	
	planLight.GetUBOFindConfig()->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 0 );
	planLight.GetSSBOVisibleElements()->Activate( 1 );
	planLight.GetSSBOCounters()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( wcompute.GetElementCount() - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	
	planLight.GetSSBOVisibleElements()->GPUFinishedWriting();
	planLight.GetSSBOCounters()->GPUFinishedWriting();
	planLight.GetSSBOCounters()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::FindContentSkyLightGI( const deoglRenderPlanSkyLight &planLight ){
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	if( wcompute.GetElementCount() == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContentSkyLightGI" );
	
	pPipelineFindContentSkyLightGI->Activate();
	
	planLight.GetUBOFindConfigGI()->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 0 );
	planLight.GetSSBOVisibleElementsGI()->Activate( 1 );
	planLight.GetSSBOCountersGI()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( wcompute.GetElementCount() - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	
	planLight.GetSSBOVisibleElementsGI()->GPUFinishedWriting();
	planLight.GetSSBOCountersGI()->GPUFinishedWriting();
	planLight.GetSSBOCountersGI()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::ClearCullResult( const deoglRenderPlan &plan ){
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int elementCount = wcompute.GetElementCount();
	if( elementCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.ClearCullResult" );
	
	/*
	pPipelineClearCullResult->Activate();
	
	const int count = ( elementCount - 1 ) / 4 + 1;
	pPipelineClearCullResult->GetGlShader().SetParameterUInt( 0, count );
	pSSBOElementCullResult->Activate( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
	*/
	
	pSSBOElementCullResult->ClearDataUInt( ( elementCount - 1 ) / 4 + 1, 0, 0, 0, 0 );
}

void deoglRenderCompute::UpdateCullResult( const deoglRenderPlan &plan, const deoglSPBlockUBO &findConfig,
const deoglSPBlockSSBO &visibleElements, const deoglSPBlockSSBO &counters, bool clear ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.UpdateCullResult" );
	
	( clear ? pPipelineUpdateCullResultClear : pPipelineUpdateCullResultSet )->Activate();
	
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

void deoglRenderCompute::FindGeometries( const deoglRenderPlan &plan, const deoglSPBlockSSBO &counters ){
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int geometryCount = wcompute.GetElementGeometryCount();
	if( geometryCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindGeometries" );
	
	pPipelineFindGeometries->Activate();
	
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	pSSBOElementCullResult->Activate( 1 );
	pSSBOVisibleGeometries->Activate( 2 );
	counters.ActivateAtomic( 0 );
	
	pPipelineFindGeometries->GetGlShader().SetParameterUInt( 0, geometryCount );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( geometryCount - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
}

void deoglRenderCompute::BuildRenderTask( const deoglRenderPlan &plan,
const deoglSPBlockSSBO &counters, deoglComputeRenderTask &renderTask, int dispatchOffset ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.BuildRenderTask" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	DEASSERT_TRUE( wcompute.GetElementGeometryCount() > 0 )
	
	// build render task
	pPipelineBuildRenderTask->Activate();
	
	renderTask.GetUBOConfig()->Activate( 0 );
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	renderThread.GetShader().GetSSBOSkinTextures()->Activate( 1 );
	pSSBOVisibleGeometries->Activate( 2 );
	counters.Activate( 3 );
	renderTask.GetSSBOSteps()->Activate( 4 );
	renderTask.GetSSBOCounters()->ActivateAtomic( 0 );
	
	counters.ActivateDispatchIndirect();
	
	deoglShaderCompiled &shader = pPipelineBuildRenderTask->GetGlShader();
	const int passCount = renderTask.GetPassCount();
	int i;
	
	for( i=0; i<passCount; i++ ){
		shader.SetParameterUInt( 0, i );
		OGL_CHECK( renderThread, pglDispatchComputeIndirect( dispatchOffset ) );
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT ) );
	}
	
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	counters.DeactivateDispatchIndirect();
	
	// sort render task
	
	
	renderTask.GetSSBOSteps()->GPUFinishedWriting();
	renderTask.GetSSBOCounters()->GPUFinishedWriting();
	renderTask.GetSSBOCounters()->GPUReadToCPU( 1 );
}



// Protected Functions
////////////////////////
