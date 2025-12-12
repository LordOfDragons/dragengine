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

#include <stdio.h>
#include <stdlib.h>

#include "deoglRenderCompute.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanSkyLight.h"
#include "task/deoglComputeRenderTask.h"
#include "task/shared/deoglRenderTaskSharedPool.h"
#include "task/shared/deoglRenderTaskSharedVAO.h"
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

deoglRenderCompute::deoglRenderCompute(deoglRenderThread &renderThread) :
deoglRenderBase(renderThread)
{
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	deoglPipelineConfiguration pipconf;
	deoglShaderDefines defines, commonDefines;
	
	pipconf.Reset();
	pipconf.SetType(deoglPipelineConfiguration::etCompute);
	
	renderThread.GetShader().SetCommonDefines(commonDefines);
	
	
	// SSBOs
	pSSBOCounters = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etRead);
	pSSBOCounters->SetRowMajor(rowMajor);
	pSSBOCounters->SetParameterCount(2);
	pSSBOCounters->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 3, 1, 1); // uvec3
	pSSBOCounters->GetParameterAt(1).SetAll(deoglSPBParameter::evtInt, 1, 1, 1); // uint
	pSSBOCounters->SetElementCount(3);
	pSSBOCounters->MapToStd140();
	
	pSSBOUpdateElements = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etStream);
	pSSBOUpdateElements->SetRowMajor(rowMajor);
	pSSBOUpdateElements->SetParameterCount(11);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeMinExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeFlags).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeMaxExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeUpdateIndex).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeLayerMask).SetAll(deoglSPBParameter::evtInt, 2, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeFirstGeometry).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeGeometryCount).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeLodFactors).SetAll(deoglSPBParameter::evtFloat, 4, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeHighestLod).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeCullResult).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElements->GetParameterAt(deoglWorldCompute::espeLodIndex).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElements->MapToStd140();
	
	pSSBOUpdateElementGeometries = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etStream);
	pSSBOUpdateElementGeometries->SetRowMajor(rowMajor);
	pSSBOUpdateElementGeometries->SetParameterCount(9);
	pSSBOUpdateElementGeometries->GetParameterAt(deoglWorldCompute::espegElement).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElementGeometries->GetParameterAt(deoglWorldCompute::espegLod).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElementGeometries->GetParameterAt(deoglWorldCompute::espegRenderFilter).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElementGeometries->GetParameterAt(deoglWorldCompute::espegSkinTexture).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElementGeometries->GetParameterAt(deoglWorldCompute::espegPipelineBase).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElementGeometries->GetParameterAt(deoglWorldCompute::espegVao).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElementGeometries->GetParameterAt(deoglWorldCompute::espegInstance).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElementGeometries->GetParameterAt(deoglWorldCompute::espegSPBInstance).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOUpdateElementGeometries->GetParameterAt(deoglWorldCompute::espegTUCs).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
	pSSBOUpdateElementGeometries->MapToStd140();
	
	pSSBOUpdateIndices = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etStream);
	pSSBOUpdateIndices->SetRowMajor(rowMajor);
	pSSBOUpdateIndices->SetParameterCount(1);
	pSSBOUpdateIndices->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
	pSSBOUpdateIndices->MapToStd140();
	
	pSSBOClearGeometries = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etStream);
	pSSBOClearGeometries->SetRowMajor(rowMajor);
	pSSBOClearGeometries->SetParameterCount(1);
	pSSBOClearGeometries->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOClearGeometries->MapToStd430();
	
	pSSBOElementCullResult = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etGpu);
	pSSBOElementCullResult->SetRowMajor(rowMajor);
	pSSBOElementCullResult->SetParameterCount(1);
	pSSBOElementCullResult->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
	pSSBOElementCullResult->MapToStd140();
	
	pSSBOVisibleGeometries = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etGpu);
	pSSBOVisibleGeometries->SetRowMajor(rowMajor);
	pSSBOVisibleGeometries->SetParameterCount(1);
	pSSBOVisibleGeometries->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
	pSSBOVisibleGeometries->MapToStd140();
	pSSBOVisibleGeometries->EnsureBuffer();
	
	pSSBORenderTaskSubInstGroups = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etGpu);
	pSSBORenderTaskSubInstGroups->SetRowMajor(rowMajor);
	pSSBORenderTaskSubInstGroups->SetParameterCount(1);
	pSSBORenderTaskSubInstGroups->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
	pSSBORenderTaskSubInstGroups->MapToStd140();
	pSSBORenderTaskSubInstGroups->EnsureBuffer();
	
	
	// update elements
	pAsyncGetPipeline(pPipelineUpdateElements, pipconf,
		"DefRen Plan Update Elements", commonDefines);
	
	
	// update element geometries
	pAsyncGetPipeline(pPipelineUpdateElementGeometries, pipconf,
		"DefRen Plan Update Element Geometries", commonDefines);
	
	
	// cleanup element geometries
	pAsyncGetPipeline(pPipelineClearGeometries, pipconf,
		"DefRen Plan Clear Geometries", commonDefines);
	
	
	// find content
	defines = commonDefines;
	
	// pAsyncGetPipeline(pPipelineFindContentNode, pipconf, "DefRen Plan FindContent Node", defines);
	
	defines.SetDefines("CULL_VIEW_FRUSTUM");
	defines.SetDefines("CULL_TOO_SMALL");
	pAsyncGetPipeline(pPipelineFindContentElement, pipconf,
		"DefRen Plan FindContent Element", defines);
	
	
	// find content sky light
	defines = commonDefines;
	defines.SetDefines("CULL_SKY_LIGHT_FRUSTUM");
	pAsyncGetPipeline(pPipelineFindContentSkyLight, pipconf,
		"DefRen Plan FindContent Element", defines);
	
	
	// find content sky light GI
	defines = commonDefines;
	defines.SetDefines("CULL_SKY_LIGHT_GIBOX");
	pAsyncGetPipeline(pPipelineFindContentSkyLightGI, pipconf,
		"DefRen Plan FindContent Element", defines);
	
	
	// find geometries
	defines = commonDefines;
	pAsyncGetPipeline(pPipelineFindGeometries, pipconf, "DefRen Plan Find Geometries", defines);
	
	defines.SetDefines("WITH_OCCLUSION");
	pAsyncGetPipeline(pPipelineFindGeometriesSkyShadow, pipconf,
		"DefRen Plan Find Geometries", defines);
	
	
	// update cull result
	defines = commonDefines;
	pAsyncGetPipeline(pPipelineUpdateCullResultSetOcclusion, pipconf,
		"DefRen Plan Update Cull Result", defines);
	
	defines.SetDefines("WITH_CALC_LOD");
	pAsyncGetPipeline(pPipelineUpdateCullResultSet, pipconf,
		"DefRen Plan Update Cull Result", defines);
	
	defines = commonDefines;
	defines.SetDefines("CLEAR_CULL_RESULT");
	pAsyncGetPipeline(pPipelineUpdateCullResultClear, pipconf,
		"DefRen Plan Update Cull Result", defines);
	
	
	// build render task
	defines = commonDefines;
	pAsyncGetPipeline(pPipelineBuildRenderTask, pipconf, "DefRen Plan Build Render Task", defines);
	
	pAsyncGetPipeline(pPipelineBuildRenderTaskOcclusion, pipconf,
		"DefRen Plan Build Render Task Occlusion", defines);
	
	
	// sort render task
	defines = commonDefines;
	pAsyncGetPipeline(pPipelineSortRenderTask, pipconf, "DefRen Plan Sort Render Task", defines);
	
	
	// render task sub instance group
	defines = commonDefines;
	pAsyncGetPipeline(pPipelineRenderTaskSubInstGroup[0], pipconf,
		"DefRen Plan Render Task SubInstGroup Pass 1", defines);
	
	pAsyncGetPipeline(pPipelineRenderTaskSubInstGroup[1], pipconf,
		"DefRen Plan Render Task SubInstGroup Pass 2", defines);
	
	pAsyncGetPipeline(pPipelineRenderTaskSubInstGroup[2], pipconf,
		"DefRen Plan Render Task SubInstGroup Pass 3", defines);
}

deoglRenderCompute::~deoglRenderCompute(){
}



// Rendering
//////////////

int deoglRenderCompute::CounterDispatchOffset(deoglRenderCompute::eCounters counter) const{
	return sizeof(sCounters) * counter;
}

void deoglRenderCompute::UpdateElements(const deoglRenderPlan &plan){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.UpdateElements");
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetUpdateElementCount();
	
	pPipelineUpdateElements->Activate();
	
	pSSBOUpdateElements->Activate(0);
	wcompute.GetSSBOElements()->Activate(1);
	
	pPipelineUpdateElements->GetShader().SetParameterUInt(0, count);
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}

void deoglRenderCompute::UpdateElementGeometries(const deoglRenderPlan &plan){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.UpdateElementGeometries");
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetUpdateElementGeometryCount();
	
	pPipelineUpdateElementGeometries->Activate();
	
	pSSBOUpdateElementGeometries->Activate(0);
	pSSBOUpdateIndices->Activate(1);
	wcompute.GetSSBOElementGeometries()->Activate(2);
	
	pPipelineUpdateElementGeometries->GetShader().SetParameterUInt(0, count);
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	// pDebugPrintSSBOGeometries( plan, "UpdateElementGeometries: " );
}

void deoglRenderCompute::ClearGeometries(const deoglRenderPlan &plan){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.ClearGeometries");
	
	deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetClearGeometryCount();
	
	pPipelineClearGeometries->Activate();
	
	pSSBOClearGeometries->Activate(0);
	wcompute.GetSSBOElementGeometries()->Activate(1);
	
	pPipelineClearGeometries->GetShader().SetParameterUInt(0, count);
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	// pDebugPrintSSBOGeometries( plan, "ClearGeometries: " );
}

void deoglRenderCompute::FindContent(const deoglRenderPlan &plan){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.FindContent");
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const deoglRenderPlanCompute &planCompute = plan.GetCompute();
	const int count = wcompute.GetElementCount();
	
	pPipelineFindContentElement->Activate();
	
	planCompute.GetUBOFindConfig()->Activate(0);
	wcompute.GetSSBOElements()->Activate(0);
	planCompute.GetSSBOVisibleElements()->Activate(1);
	planCompute.GetSSBOCounters()->ActivateAtomic(0);
	
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
	
	planCompute.GetSSBOVisibleElements()->GPUFinishedWriting();
	planCompute.GetSSBOCounters()->GPUFinishedWriting();
	planCompute.GetSSBOCounters()->GPUReadToCPU(1);
}

void deoglRenderCompute::FindContentSkyLight(const deoglRenderPlanSkyLight &planLight){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.FindContentSkyLight");
	
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	
	pPipelineFindContentSkyLight->Activate();
	
	planLight.GetUBOFindConfig()->Activate(0);
	wcompute.GetSSBOElements()->Activate(0);
	planLight.GetSSBOVisibleElements()->Activate(1);
	planLight.GetSSBOCounters()->ActivateAtomic(0);
	
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
}

void deoglRenderCompute::FindContentSkyLightGIStatic(const deoglRenderPlanSkyLight &planLight){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.FindContentSkyLightGIStatic");
	
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	
	pPipelineFindContentSkyLightGI->Activate();
	
	planLight.GetUBOFindConfigGIStatic()->Activate(0);
	wcompute.GetSSBOElements()->Activate(0);
	planLight.GetSSBOVisibleElementsGIStatic()->Activate(1);
	planLight.GetSSBOCountersGIStatic()->ActivateAtomic(0);
	
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
	
	planLight.GetSSBOVisibleElementsGIStatic()->GPUFinishedWriting();
	planLight.GetSSBOCountersGIStatic()->GPUFinishedWriting();
	planLight.GetSSBOCountersGIStatic()->GPUReadToCPU(1);
}

void deoglRenderCompute::FindContentSkyLightGIDynamic(const deoglRenderPlanSkyLight &planLight){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.FindContentSkyLightGIDynamic");
	
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	
	pPipelineFindContentSkyLightGI->Activate();
	
	planLight.GetUBOFindConfigGIDynamic()->Activate(0);
	wcompute.GetSSBOElements()->Activate(0);
	planLight.GetSSBOVisibleElementsGIDynamic()->Activate(1);
	planLight.GetSSBOCountersGIDynamic()->ActivateAtomic(0);
	
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
	
	planLight.GetSSBOVisibleElementsGIDynamic()->GPUFinishedWriting();
	planLight.GetSSBOCountersGIDynamic()->GPUFinishedWriting();
	planLight.GetSSBOCountersGIDynamic()->GPUReadToCPU(1);
}

void deoglRenderCompute::ClearCullResult(const deoglRenderPlan &plan){
	const deoglDebugTraceGroup debugTrace(GetRenderThread(), "Compute.ClearCullResult");
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementCount();
	
	if(count > pSSBOElementCullResult->GetElementCount()){
		pSSBOElementCullResult->SetElementCount(count);
		pSSBOElementCullResult->EnsureBuffer();
	}
	
	pSSBOElementCullResult->ClearDataUInt(0, (count - 1) / 4 + 1, 0, 0, 0, 0);
}

void deoglRenderCompute::UpdateCullResult(const deoglRenderPlan &plan, const deoglSPBlockUBO &findConfig,
const deoglSPBlockSSBO &visibleElements, const deoglSPBlockSSBO &counters, int lodLayer){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.UpdateCullResult");
	
	pPipelineUpdateCullResultSet->Activate();
	
	pPipelineUpdateCullResultSet->GetShader().SetParameterUInt(0, lodLayer);
	
	findConfig.Activate(0);
	plan.GetWorld()->GetCompute().GetSSBOElements()->Activate(0);
	visibleElements.Activate(1);
	counters.Activate(2);
	pSSBOElementCullResult->Activate(3);
	
	counters.ActivateDispatchIndirect();
	OGL_CHECK(renderThread, pglDispatchComputeIndirect(0));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	counters.DeactivateDispatchIndirect();
}

void deoglRenderCompute::UpdateCullResultOcclusion(const deoglRenderPlan &plan,
const deoglSPBlockUBO &findConfig, const deoglSPBlockSSBO &visibleElements, const deoglSPBlockSSBO &counters){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.UpdateCullResultOcclusion");
	
	pPipelineUpdateCullResultSetOcclusion->Activate();
	
	findConfig.Activate(0);
	plan.GetWorld()->GetCompute().GetSSBOElements()->Activate(0);
	visibleElements.Activate(1);
	counters.Activate(2);
	pSSBOElementCullResult->Activate(3);
	
	counters.ActivateDispatchIndirect();
	OGL_CHECK(renderThread, pglDispatchComputeIndirect(0));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	counters.DeactivateDispatchIndirect();
}

void deoglRenderCompute::FindGeometries(const deoglRenderPlan &plan){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.FindGeometries");
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementGeometryCount();
	
	if(count > pSSBOVisibleGeometries->GetElementCount()){
		pSSBOVisibleGeometries->SetElementCount(count);
		pSSBOVisibleGeometries->EnsureBuffer();
	}
	
	pSSBOCounters->ClearDataUInt(ecVisibleGeometries, 1, 0, 1, 1, 0); // workGroupSize.xyz, count
	
	pPipelineFindGeometries->Activate();
	
	wcompute.GetSSBOElementGeometries()->Activate(0);
	pSSBOElementCullResult->Activate(1);
	pSSBOVisibleGeometries->Activate(2);
	pSSBOCounters->ActivateAtomic(0);
	
	pPipelineFindGeometries->GetShader().SetParameterUInt(0, count);
	
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
}

void deoglRenderCompute::FindGeometriesSkyShadow(const deoglRenderPlan &plan){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.FindGeometriesSkyShadow");
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementGeometryCount();
	
	if(count > pSSBOVisibleGeometries->GetElementCount()){
		pSSBOVisibleGeometries->SetElementCount(count);
		pSSBOVisibleGeometries->EnsureBuffer();
	}
	
	pSSBOCounters->ClearDataUInt(ecVisibleGeometries, 1, 0, 1, 1, 0); // workGroupSize.xyz, count
	
	pPipelineFindGeometriesSkyShadow->Activate();
	
	wcompute.GetSSBOElementGeometries()->Activate(0);
	pSSBOElementCullResult->Activate(1);
	pSSBOVisibleGeometries->Activate(2);
	pSSBOCounters->ActivateAtomic(0);
	
	pPipelineFindGeometries->GetShader().SetParameterUInt(0, count);
	
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
}

void deoglRenderCompute::BuildRenderTask(const deoglRenderPlan &plan, deoglComputeRenderTask &renderTask){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.BuildRenderTask");
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	
	pPipelineBuildRenderTask->Activate();
	
	renderTask.GetUBOConfig()->Activate(0);
	wcompute.GetSSBOElementGeometries()->Activate(0);
	renderThread.GetShader().GetSSBOSkinTextures()->Activate(1);
	pSSBOVisibleGeometries->Activate(2);
	pSSBOCounters->Activate(3);
	renderTask.GetSSBOSteps()->Activate(4);
	renderTask.GetSSBOCounters()->ActivateAtomic(0);
	
	pSSBOCounters->ActivateDispatchIndirect();
	
	deoglShaderCompiled &shaderBuild = pPipelineBuildRenderTask->GetShader();
	const int dispatchOffset = CounterDispatchOffset(ecVisibleGeometries);
	const int passCount = renderTask.GetPassCount();
	int i;
	
	for(i=0; i<passCount; i++){
		shaderBuild.SetParameterUInt(0, i);
		OGL_CHECK(renderThread, pglDispatchComputeIndirect(dispatchOffset));
		OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
			| GL_SHADER_STORAGE_BARRIER_BIT));
	}
	
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_COMMAND_BARRIER_BIT));
	pSSBOCounters->DeactivateDispatchIndirect();
	
	SortRenderTask(renderTask);
	
	renderTask.GetSSBOCounters()->GPUFinishedWriting();
	renderTask.GetSSBOCounters()->GPUReadToCPU(1);
}

void deoglRenderCompute::BuildRenderTaskOcclusion(const deoglRenderPlan &plan, deoglComputeRenderTask &renderTask){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.BuildRenderTaskOcclusion");
	
	const deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	const int count = wcompute.GetElementGeometryCount();
	
	pPipelineBuildRenderTaskOcclusion->Activate();
	
	renderTask.GetUBOConfig()->Activate(0);
	wcompute.GetSSBOElementGeometries()->Activate(0);
	pSSBOElementCullResult->Activate(1);
	renderTask.GetSSBOSteps()->Activate(2);
	renderTask.GetSSBOCounters()->ActivateAtomic(0);
	
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
	
	SortRenderTask(renderTask);
	
	renderTask.GetSSBOCounters()->GPUFinishedWriting();
	renderTask.GetSSBOCounters()->GPUReadToCPU(1);
}

void deoglRenderCompute::BuildRenderTaskSkyShadow(const deoglRenderPlanSkyLight &planLight, int layer){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.BuildRenderTaskSkyShadow");
	
	const deoglWorldCompute &wcompute = planLight.GetPlan().GetWorld()->GetCompute();
	const int count = wcompute.GetElementGeometryCount();
	
	const deoglRenderPlanSkyLight::sShadowLayer &sl = planLight.GetShadowLayerAt(layer);
	deoglComputeRenderTask &renderTask = sl.computeRenderTask;
	
	// pass 1, occlusion meshes
	pPipelineBuildRenderTaskOcclusion->Activate();
	
	renderTask.GetUBOConfig()->Activate(0);
	wcompute.GetSSBOElementGeometries()->Activate(0);
	pSSBOElementCullResult->Activate(1);
	renderTask.GetSSBOSteps()->Activate(2);
	renderTask.GetSSBOCounters()->ActivateAtomic(0);
	
	OGL_CHECK(renderThread, pglDispatchCompute((count - 1) / 64 + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
		| GL_SHADER_STORAGE_BARRIER_BIT));
	
	// pass 2+, other geometry
	pPipelineBuildRenderTask->Activate();
	
	renderTask.GetUBOConfig()->Activate(0);
	wcompute.GetSSBOElementGeometries()->Activate(0);
	renderThread.GetShader().GetSSBOSkinTextures()->Activate(1);
	pSSBOVisibleGeometries->Activate(2);
	pSSBOCounters->Activate(3);
	renderTask.GetSSBOSteps()->Activate(4);
	renderTask.GetSSBOCounters()->ActivateAtomic(0);
	
	pSSBOCounters->ActivateDispatchIndirect();
	
	deoglShaderCompiled &shaderBuild = pPipelineBuildRenderTask->GetShader();
	const int dispatchOffset = CounterDispatchOffset(ecVisibleGeometries);
	const int passCount = renderTask.GetPassCount();
	int i;
	
	for(i=1; i<passCount; i++){
		shaderBuild.SetParameterUInt(0, i);
		OGL_CHECK(renderThread, pglDispatchComputeIndirect(dispatchOffset));
		OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
			| GL_SHADER_STORAGE_BARRIER_BIT));
	}
	
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_COMMAND_BARRIER_BIT));
	pSSBOCounters->DeactivateDispatchIndirect();
	
	// sort and finish
	SortRenderTask(sl.computeRenderTask);
	
	renderTask.GetSSBOCounters()->GPUFinishedWriting();
	renderTask.GetSSBOCounters()->GPUReadToCPU(1);
}



// Protected Functions
////////////////////////

void deoglRenderCompute::SortRenderTask(deoglComputeRenderTask &renderTask){
	const int maxCount = renderTask.GetSSBOSteps()->GetElementCount();
	if(maxCount == 0){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Compute.SortRenderTask");
	
	
	// sort render task
	deoglDebugTraceGroup dtSort(renderThread, "SortSteps");
	pPipelineSortRenderTask->Activate();
	
	renderTask.GetSSBOCounters()->Activate(0);
	renderTask.GetSSBOSteps()->Activate(1);
	
	const int maxLanSize = 128; // 64 work group size * 2
	const int potStepCount = oglPotOf(maxCount);
	const int workGroupCount = ((potStepCount - 1) / maxLanSize) + 1;
	
	// local sorting
	deoglShaderCompiled &shaderSort = pPipelineSortRenderTask->GetShader();
	shaderSort.SetParameterInt(esspStage, essLocalSort);
	shaderSort.SetParameterUInt(esspLaneSize, maxLanSize);
	OGL_CHECK(renderThread, pglDispatchCompute(workGroupCount, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	
	// global sorting
	int laneSize, halfLaneSize;
	for(laneSize = maxLanSize * 2; laneSize <= potStepCount; laneSize *= 2){
		shaderSort.SetParameterInt(esspStage, essGlobalFlip);
		shaderSort.SetParameterUInt(esspLaneSize, laneSize);
		OGL_CHECK(renderThread, pglDispatchCompute(workGroupCount, 1, 1));
		OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
		
		for(halfLaneSize = laneSize / 2; halfLaneSize > 1; halfLaneSize /= 2){
			shaderSort.SetParameterUInt(esspLaneSize, halfLaneSize);
			
			if(halfLaneSize <= maxLanSize){
				shaderSort.SetParameterInt(esspStage, essLocalDisperse);
				OGL_CHECK(renderThread, pglDispatchCompute(workGroupCount, 1, 1));
				OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
				break;
			}
			
			shaderSort.SetParameterInt(esspStage, essGlobalDisperse);
			OGL_CHECK(renderThread, pglDispatchCompute(workGroupCount, 1, 1));
			OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
		}
	}
	dtSort.Close();
	
	
	
	// subinstance grouping
	deoglDebugTraceGroup dtSubInst(renderThread, "SubInstGrouping");
	const int maxECount = ((maxCount - 1) / 4) + 1;
	if(maxECount > pSSBORenderTaskSubInstGroups->GetElementCount()){
		pSSBORenderTaskSubInstGroups->SetElementCount(maxECount);
		pSSBORenderTaskSubInstGroups->EnsureBuffer();
	}
	pSSBOCounters->ClearDataUInt(ecRenderTaskSubInstanceGroups, 1, 0, 1, 1, 0);
	
	// pass 1
	pPipelineRenderTaskSubInstGroup[0]->Activate();
	
	renderTask.GetSSBOSteps()->Activate(0);
	renderTask.GetSSBOCounters()->Activate(1);
	pSSBORenderTaskSubInstGroups->Activate(2);
	pSSBOCounters->ActivateAtomic(0);
	
	OGL_CHECK(renderThread, pglDispatchCompute(((maxCount - 1) / 64) + 1, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT
		| GL_ATOMIC_COUNTER_BARRIER_BIT));
	
	// pass 2
	deoglDebugTraceGroup dtSubInst2(renderThread, "Pass2");
	pPipelineRenderTaskSubInstGroup[1]->Activate();
	
	pSSBOCounters->Activate(0);
	pSSBORenderTaskSubInstGroups->Activate(1);
	
	deoglShaderCompiled &shaderSortSubInstGroup = pPipelineRenderTaskSubInstGroup[1]->GetShader();
	
	shaderSortSubInstGroup.SetParameterInt(ertsig2pStage, essLocalSort);
	shaderSortSubInstGroup.SetParameterUInt(ertsig2pLaneSize, maxLanSize);
	OGL_CHECK(renderThread, pglDispatchCompute(workGroupCount, 1, 1));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	
	for(laneSize = maxLanSize * 2; laneSize <= potStepCount; laneSize *= 2){
		shaderSortSubInstGroup.SetParameterInt(ertsig2pStage, essGlobalFlip);
		shaderSortSubInstGroup.SetParameterUInt(ertsig2pLaneSize, laneSize);
		OGL_CHECK(renderThread, pglDispatchCompute(workGroupCount, 1, 1));
		OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
		
		for(halfLaneSize = laneSize / 2; halfLaneSize > 1; halfLaneSize /= 2){
			shaderSortSubInstGroup.SetParameterUInt(ertsig2pLaneSize, halfLaneSize);
			
			if(halfLaneSize <= maxLanSize){
				shaderSortSubInstGroup.SetParameterInt(ertsig2pStage, essLocalDisperse);
				OGL_CHECK(renderThread, pglDispatchCompute(workGroupCount, 1, 1));
				OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
				break;
			}
			
			shaderSortSubInstGroup.SetParameterInt(ertsig2pStage, essGlobalDisperse);
			OGL_CHECK(renderThread, pglDispatchCompute(workGroupCount, 1, 1));
			OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
		}
	}
	dtSubInst2.Close();
	
	// pass 3
	pPipelineRenderTaskSubInstGroup[2]->Activate();
	
	pSSBORenderTaskSubInstGroups->Activate(0);
	pSSBOCounters->Activate(1);
	renderTask.GetSSBOCounters()->Activate(2);
	renderTask.GetSSBOSteps()->Activate(3);
	
	pSSBOCounters->ActivateDispatchIndirect();
	OGL_CHECK(renderThread, pglDispatchComputeIndirect(CounterDispatchOffset(ecRenderTaskSubInstanceGroups)));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
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

void deoglRenderCompute::pDebugPrintSSBOGeometries(const deoglRenderPlan &plan, const char *prefix){
	deoglWorldCompute &wcompute = plan.GetWorld()->GetCompute();
	deoglRenderThread &renderThread = GetRenderThread();
	
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT));
	const deoglFence::Ref fence(deoglFence::Ref::New(GetRenderThread()));
	fence->Arm();
	fence->Wait();
	
	deoglRTLogger &logger = GetRenderThread().GetLogger();
	const deoglRenderTaskSharedPool &rtsp = GetRenderThread().GetRenderTaskSharedPool();
	wcompute.GetSSBOElementGeometries()->Deactivate(1);
	deoglWorldComputeElement::sDataElementGeometry *data;
	
	OGL_CHECK(renderThread, pglBindBuffer(GL_PIXEL_PACK_BUFFER, wcompute.GetSSBOElementGeometries()->GetSSBO()));
	OGL_CHECK(renderThread, data = (deoglWorldComputeElement::sDataElementGeometry*)pglMapBufferRange(
		GL_PIXEL_PACK_BUFFER, 0, wcompute.GetSSBOElementGeometries()->GetBufferSize(), GL_MAP_READ_BIT));
	
	decStringList list;
	decString s;
	int i, cc = wcompute.GetSSBOElementGeometries()->GetElementCount();
	logger.LogInfoFormat("%sSSBO: %d", prefix, cc);
	for(i=0; i<cc; i++){
		s.Format("[%d:%d,%x,%d]", i, data[i].element, data[i].renderFilter, rtsp.GetVAOAt(data[i].vao).GetVAO() != nullptr);
		list.Add(s);
	}
	logger.LogInfo(list.Join(" "));
	
	OGL_CHECK(renderThread, pglUnmapBuffer(GL_PIXEL_PACK_BUFFER));
	OGL_CHECK(renderThread, pglBindBuffer(GL_PIXEL_PACK_BUFFER, 0));
}
