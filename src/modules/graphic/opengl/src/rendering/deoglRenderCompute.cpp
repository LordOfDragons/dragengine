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
	esspLaneSize,
	esspStepCount
};

enum eRenderTaskSubInstGroup1Parameters{
	ertsig1pStepCount
};

enum eRenderTaskSubInstGroup2Parameters{
	ertsig2pStage,
	ertsig2pLaneSize
};

enum eRenderTaskSubInstGroup3Parameters{
	ertsig3pStepCount
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
	
	pSSBORenderTaskSubInstGroups.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etGpu ) );
	pSSBORenderTaskSubInstGroups->SetRowMajor( rowMajor );
	pSSBORenderTaskSubInstGroups->SetParameterCount( 1 );
	pSSBORenderTaskSubInstGroups->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
	pSSBORenderTaskSubInstGroups->MapToStd140();
	
	pSSBORenderTaskSubInstGroupCounter.TakeOver( new deoglSPBlockSSBO( renderThread, deoglSPBlockSSBO::etRead ) );
	pSSBORenderTaskSubInstGroupCounter->SetRowMajor( rowMajor );
	pSSBORenderTaskSubInstGroupCounter->SetParameterCount( 2 );
	pSSBORenderTaskSubInstGroupCounter->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // uvec3
	pSSBORenderTaskSubInstGroupCounter->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pSSBORenderTaskSubInstGroupCounter->SetElementCount( 1 );
	pSSBORenderTaskSubInstGroupCounter->MapToStd140();
	pSSBORenderTaskSubInstGroupCounter->EnsureBuffer();
	
	
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
	
	
	// update cull result
	pipconf.SetShader( renderThread, "DefRen Plan Update Cull Result", defines );
	pPipelineUpdateCullResultSet = pipelineManager.GetWith( pipconf );
	
	defines.SetDefines( "CLEAR_CULL_RESULT" );
	pPipelineUpdateCullResultClear = pipelineManager.GetWith( pipconf );
	
	
	// build render task
	defines = commonDefines;
	pipconf.SetShader( renderThread, "DefRen Plan Build Render Task", defines );
	pPipelineBuildRenderTask = pipelineManager.GetWith( pipconf );
	
	
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

void deoglRenderCompute::FindContent( const deoglRenderPlan &plan ){
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	if( count == 0 ){
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
	
	planLight.GetSSBOVisibleElements()->GPUFinishedWriting();
	planLight.GetSSBOCounters()->GPUFinishedWriting();
	planLight.GetSSBOCounters()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::FindContentSkyLightGI( const deoglRenderPlanSkyLight &planLight ){
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	if( count == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindContentSkyLightGI" );
	
	pPipelineFindContentSkyLightGI->Activate();
	
	planLight.GetUBOFindConfigGI()->Activate( 0 );
	wcompute.GetSSBOElements()->Activate( 0 );
	planLight.GetSSBOVisibleElementsGI()->Activate( 1 );
	planLight.GetSSBOCountersGI()->ActivateAtomic( 0 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	
	planLight.GetSSBOVisibleElementsGI()->GPUFinishedWriting();
	planLight.GetSSBOCountersGI()->GPUFinishedWriting();
	planLight.GetSSBOCountersGI()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::ClearCullResult( const deoglRenderPlan &plan ){
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	if( count == 0 ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( GetRenderThread(), "Compute.ClearCullResult" );
	pSSBOElementCullResult->ClearDataUInt( ( count - 1 ) / 4 + 1, 0, 0, 0, 0 );
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
	const int count = wcompute.GetElementGeometryCount();
	if( count == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.FindGeometries" );
	
	pPipelineFindGeometries->Activate();
	
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	pSSBOElementCullResult->Activate( 1 );
	pSSBOVisibleGeometries->Activate( 2 );
	counters.ActivateAtomic( 0 );
	
	pPipelineFindGeometries->GetGlShader().SetParameterUInt( 0, count );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( count - 1 ) / 64 + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
}

void deoglRenderCompute::BuildRenderTask( const deoglRenderPlan &plan,
const deoglSPBlockSSBO &counters, deoglComputeRenderTask &renderTask, int dispatchOffset ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.BuildRenderTask" );
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	DEASSERT_TRUE( wcompute.GetElementGeometryCount() > 0 )
	
	pPipelineBuildRenderTask->Activate();
	
	renderTask.GetUBOConfig()->Activate( 0 );
	wcompute.GetSSBOElementGeometries()->Activate( 0 );
	renderThread.GetShader().GetSSBOSkinTextures()->Activate( 1 );
	pSSBOVisibleGeometries->Activate( 2 );
	counters.Activate( 3 );
	renderTask.GetSSBOSteps()->Activate( 4 );
	renderTask.GetSSBOCounters()->ActivateAtomic( 0 );
	
	counters.ActivateDispatchIndirect();
	
	deoglShaderCompiled &shaderBuild = pPipelineBuildRenderTask->GetGlShader();
	const int passCount = renderTask.GetPassCount();
	int i;
	
	for( i=0; i<passCount; i++ ){
		shaderBuild.SetParameterUInt( 0, i );
		OGL_CHECK( renderThread, pglDispatchComputeIndirect( dispatchOffset ) );
		OGL_CHECK( renderThread, pglMemoryBarrier( GL_ATOMIC_COUNTER_BARRIER_BIT ) );
	}
	
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT ) );
	counters.DeactivateDispatchIndirect();
	
	renderTask.GetSSBOCounters()->GPUFinishedWriting();
	renderTask.GetSSBOCounters()->GPUReadToCPU( 1 );
}

void deoglRenderCompute::SortRenderTask( deoglComputeRenderTask &renderTask ){
	const int stepCount = renderTask.GetStepCount();
	if( stepCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Compute.SortRenderTask" );
	
	
	// prepare
	const int maxSubInstGroupECount = ( ( stepCount - 1 ) / 4 ) + 1;
	if( maxSubInstGroupECount > pSSBORenderTaskSubInstGroups->GetElementCount() ){
		pSSBORenderTaskSubInstGroups->SetElementCount( maxSubInstGroupECount );
		pSSBORenderTaskSubInstGroups->EnsureBuffer();
	}
	
	pSSBORenderTaskSubInstGroupCounter->ClearDataUInt( 1, 0, 1, 1, 0 );
	
	
	// sort render task
	pPipelineSortRenderTask->Activate();
	
	renderTask.GetSSBOSteps()->Activate( 0 );
	
	const int maxLanSize = 128; // 64 work group size * 2
	const int potStepCount = oglPotOf( stepCount );
	const int workGroupCount = ( ( potStepCount - 1 ) / maxLanSize ) + 1;
	
	deoglShaderCompiled &shaderSort = pPipelineSortRenderTask->GetGlShader();
	shaderSort.SetParameterUInt( esspStepCount, stepCount );
	
	// local sorting
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
	
	
	// subinstance grouping
	
	// pass 1
	pPipelineRenderTaskSubInstGroup[ 0 ]->Activate();
	
	renderTask.GetSSBOSteps()->Activate( 0 );
	pSSBORenderTaskSubInstGroups->Activate( 1 );
	pSSBORenderTaskSubInstGroupCounter->ActivateAtomic( 0 );
	
	pPipelineRenderTaskSubInstGroup[ 0 ]->GetGlShader().SetParameterUInt( ertsig1pStepCount, stepCount );
	
	OGL_CHECK( renderThread, pglDispatchCompute( ( ( stepCount - 1 ) / 64 ) + 1, 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT ) );
	
	// pass 2
	pPipelineRenderTaskSubInstGroup[ 1 ]->Activate();
	
	pSSBORenderTaskSubInstGroupCounter->Activate( 0 );
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
	
	// pass 3
	pPipelineRenderTaskSubInstGroup[ 2 ]->Activate();
	
	pSSBORenderTaskSubInstGroups->Activate( 0 );
	pSSBORenderTaskSubInstGroupCounter->Activate( 1 );
	renderTask.GetSSBOSteps()->Activate( 2 );
	
	pPipelineRenderTaskSubInstGroup[ 2 ]->GetGlShader().SetParameterUInt( ertsig3pStepCount, stepCount );
	
	pSSBORenderTaskSubInstGroupCounter->ActivateDispatchIndirect();
	OGL_CHECK( renderThread, pglDispatchComputeIndirect( 0 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT ) );
	pSSBORenderTaskSubInstGroupCounter->DeactivateDispatchIndirect();
	
	renderTask.GetSSBOSteps()->GPUFinishedWriting();
	renderTask.GetSSBOSteps()->GPUReadToCPU( stepCount );
}



// Protected Functions
////////////////////////
