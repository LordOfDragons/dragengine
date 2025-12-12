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
#include <string.h>

#include "deoglRenderPlan.h"
#include "deoglRenderPlanMasked.h"
#include "deoglRenderPlanTasks.h"
#include "parallel/deoglRPTBuildRTsDepth.h"
#include "parallel/deoglRPTBuildRTsGeometry.h"
#include "../../deGraphicOpenGl.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCompute.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>


// Class deoglRenderPlanTasks
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanTasks::deoglRenderPlanTasks(deoglRenderPlan &plan) :
pPlan(plan),

pSolidDepthTask(nullptr),
pSolidDepthOutlineTask(nullptr),
pSolidGeometryTask(nullptr),
pSolidGeometryHeight1Task(nullptr),
pSolidGeometryHeight2Task(nullptr),
pSolidGeometryOutlineTask(nullptr),
pSolidDecalsTask(nullptr),

pSolidDepthXRayTask(nullptr),
pSolidDepthOutlineXRayTask(nullptr),
pSolidGeometryXRayTask(nullptr),
pSolidGeometryHeight1XRayTask(nullptr),
pSolidGeometryHeight2XRayTask(nullptr),
pSolidGeometryOutlineXRayTask(nullptr),
pSolidDecalsXRayTask(nullptr)
{
	deoglRenderThread &renderthread = plan.GetRenderThread();
	
	pSolidDepthTask = new deoglRenderTask(renderthread);
	pSolidDepthOutlineTask = new deoglRenderTask(renderthread);
	pSolidGeometryTask = new deoglRenderTask(renderthread);
	pSolidGeometryHeight1Task = new deoglRenderTask(renderthread);
	pSolidGeometryHeight2Task = new deoglRenderTask(renderthread);
	pSolidGeometryOutlineTask = new deoglRenderTask(renderthread);
	pSolidDecalsTask = new deoglRenderTask(renderthread);
	
	pSolidDepthXRayTask = new deoglRenderTask(renderthread);
	pSolidDepthOutlineXRayTask = new deoglRenderTask(renderthread);
	pSolidGeometryXRayTask = new deoglRenderTask(renderthread);
	pSolidGeometryHeight1XRayTask = new deoglRenderTask(renderthread);
	pSolidGeometryHeight2XRayTask = new deoglRenderTask(renderthread);
	pSolidGeometryOutlineXRayTask = new deoglRenderTask(renderthread);
	pSolidDecalsXRayTask = new deoglRenderTask(renderthread);
	
	pCRTSolidDepth = deoglComputeRenderTask::Ref::New(renderthread);
	pCRTSolidGeometry = deoglComputeRenderTask::Ref::New(renderthread);
	
	pCRTSolidDepthXRay = deoglComputeRenderTask::Ref::New(renderthread);
	pCRTSolidGeometryXRay = deoglComputeRenderTask::Ref::New(renderthread);
}

deoglRenderPlanTasks::~deoglRenderPlanTasks(){
	CleanUp();
	
	if(pSolidDepthTask){
		delete pSolidDepthTask;
	}
	if(pSolidDepthOutlineTask){
		delete pSolidDepthOutlineTask;
	}
	if(pSolidGeometryTask){
		delete pSolidGeometryTask;
	}
	if(pSolidGeometryHeight1Task){
		delete pSolidGeometryHeight1Task;
	}
	if(pSolidGeometryHeight2Task){
		delete pSolidGeometryHeight2Task;
	}
	if(pSolidGeometryOutlineTask){
		delete pSolidGeometryOutlineTask;
	}
	if(pSolidDecalsTask){
		delete pSolidDecalsTask;
	}
	
	if(pSolidDepthXRayTask){
		delete pSolidDepthXRayTask;
	}
	if(pSolidDepthOutlineXRayTask){
		delete pSolidDepthOutlineXRayTask;
	}
	if(pSolidGeometryXRayTask){
		delete pSolidGeometryXRayTask;
	}
	if(pSolidGeometryHeight1XRayTask){
		delete pSolidGeometryHeight1XRayTask;
	}
	if(pSolidGeometryHeight2XRayTask){
		delete pSolidGeometryHeight2XRayTask;
	}
	if(pSolidGeometryOutlineXRayTask){
		delete pSolidGeometryOutlineXRayTask;
	}
	if(pSolidDecalsXRayTask){
		delete pSolidDecalsXRayTask;
	}
}



// Management
///////////////

void deoglRenderPlanTasks::BuildComputeRenderTasks(const deoglRenderPlanMasked *mask, bool rebuild){
	deoglRenderCompute &renderCompute = pPlan.GetRenderThread().GetRenderers().GetCompute();
	const deoglDebugTraceGroup dt(pPlan.GetRenderThread(), "PlanTasks.BuildRenderTasks");
	const deoglRenderPlanCompute &compute = pPlan.GetCompute();
	
	renderCompute.ClearCullResult(pPlan);
	renderCompute.UpdateCullResult(pPlan, compute.GetUBOFindConfig(),
		compute.GetSSBOVisibleElements(), compute.GetSSBOCounters(), 0);
	renderCompute.FindGeometries(pPlan);
	
	// we check state here since this could be a rebuild due to too small SSBO size.
	// rebuilding is not required for all render tasks so check first
	if(!(rebuild && pCRTSolidDepth->GetState() == deoglComputeRenderTask::esReady)){
		const deoglDebugTraceGroup dt2(pPlan.GetRenderThread(), "SolidDepth");
		pBuildCRTSolidDepth(pCRTSolidDepth, mask, false);
		renderCompute.BuildRenderTask(pPlan, pCRTSolidDepth);
	}
	
	if(!(rebuild && pCRTSolidGeometry->GetState() == deoglComputeRenderTask::esReady)){
		const deoglDebugTraceGroup dt2(pPlan.GetRenderThread(), "SolidGeometry");
		if(pCRTSolidGeometry->GetState() != deoglComputeRenderTask::esBuilding){
			pBuildCRTSolidGeometry(pCRTSolidGeometry, mask, false);
		}
		renderCompute.BuildRenderTask(pPlan, pCRTSolidGeometry);
	}
	
	if(!(rebuild && pCRTSolidDepthXRay->GetState() == deoglComputeRenderTask::esReady)){
		const deoglDebugTraceGroup dt2(pPlan.GetRenderThread(), "XRay.SolidDepth");
		if(pCRTSolidDepthXRay->GetState() != deoglComputeRenderTask::esBuilding){
			pBuildCRTSolidDepth(pCRTSolidDepthXRay, mask, true);
		}
		renderCompute.BuildRenderTask(pPlan, pCRTSolidDepthXRay);
	}
	
	if(!(rebuild && pCRTSolidGeometryXRay->GetState() == deoglComputeRenderTask::esReady)){
		const deoglDebugTraceGroup dt2(pPlan.GetRenderThread(), "XRay.SolidGeometry");
		if(pCRTSolidGeometryXRay->GetState() != deoglComputeRenderTask::esBuilding){
			pBuildCRTSolidGeometry(pCRTSolidGeometryXRay, mask, true);
		}
		renderCompute.BuildRenderTask(pPlan, pCRTSolidGeometryXRay);
	}
}

void deoglRenderPlanTasks::FinishReadBackComputeRenderTasks(const deoglRenderPlanMasked *mask){
	bool ssbosLargeEnough = true;
	ssbosLargeEnough &= pCRTSolidDepth->ReadBackSteps();
	ssbosLargeEnough &= pCRTSolidGeometry->ReadBackSteps();
	ssbosLargeEnough &= pCRTSolidDepthXRay->ReadBackSteps();
	ssbosLargeEnough &= pCRTSolidGeometryXRay->ReadBackSteps();
	if(ssbosLargeEnough){
		return;
	}
	
	BuildComputeRenderTasks(mask, true);
	
	if(pCRTSolidDepth->GetState() != deoglComputeRenderTask::esReady){
		DEASSERT_TRUE(pCRTSolidDepth->ReadBackSteps())
	}
	if(pCRTSolidGeometry->GetState() != deoglComputeRenderTask::esReady){
		DEASSERT_TRUE(pCRTSolidGeometry->ReadBackSteps())
	}
	if(pCRTSolidDepthXRay->GetState() != deoglComputeRenderTask::esReady){
		DEASSERT_TRUE(pCRTSolidDepthXRay->ReadBackSteps())
	}
	if(pCRTSolidGeometryXRay->GetState() != deoglComputeRenderTask::esReady){
		DEASSERT_TRUE(pCRTSolidGeometryXRay->ReadBackSteps())
	}
}

void deoglRenderPlanTasks::StartBuildTasks(const deoglRenderPlanMasked *mask){
	if(pTaskDepth || pTaskGeometry){
		DETHROW(deeInvalidParam);
	}
	
	deParallelProcessing &pp = pPlan.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing();
	
	pTaskDepth = deoglRPTBuildRTsDepth::Ref::New(*this, mask);
	pp.AddTaskAsync(pTaskDepth);
	
	pTaskGeometry = deoglRPTBuildRTsGeometry::Ref::New(*this, mask);
	pp.AddTaskAsync(pTaskGeometry);
}

void deoglRenderPlanTasks::WaitFinishBuildingTasksDepth(){
	if(!pTaskDepth){
		return;
	}
	
	pTaskDepth->GetSemaphore().Wait();
	
	deoglRenderCanvas &rc = pPlan.GetRenderThread().GetRenderers().GetCanvas();
	rc.SampleDebugInfoPlanPrepareBuildRTs(pPlan, pTaskDepth->GetElapsedTime());
	pTaskDepth = NULL;
	
	// this call does modify a shader parameter block and can thus not be parallel
	pSolidDepthTask->PrepareForRender();
	pSolidDepthOutlineTask->PrepareForRender();
	
	pSolidDepthXRayTask->PrepareForRender();
	pSolidDepthOutlineXRayTask->PrepareForRender();
}

// DEBUG
#if 0

#include "../task/deoglRenderTaskPipeline.h"
#include "../task/deoglRenderTaskTexture.h"
#include "../task/deoglRenderTaskVAO.h"
#include "../task/deoglRenderTaskInstance.h"
#include "../task/shared/deoglRenderTaskSharedTexture.h"
#include "../task/shared/deoglRenderTaskSharedVAO.h"
#include "../task/shared/deoglRenderTaskSharedInstance.h"
static void LogRT(deoglRTLogger &l, const char *name, int pass, const deoglRenderTask &rt){
	l.LogInfoFormat("%s: %d", name, rt.GetTotalSubInstanceCount());
	int p, t, v, i, pc = rt.GetPipelineCount();
	for(p=0; p<pc; p++){
		const deoglRenderTaskPipeline &pp = *rt.GetPipelineAt(p);
		const int tc = pp.GetTextureCount();
		for(t=0; t<tc; t++){
			const deoglRenderTaskTexture &tt = *pp.GetTextureAt(t);
			const int vc = tt.GetVAOCount();
			for(v=0; v<vc; v++){
				const deoglRenderTaskVAO &vv = *tt.GetVAOAt(v);
				const int ic = vv.GetInstanceCount();
				for(i=0; i<ic; i++){
					const deoglRenderTaskInstance &ii = *vv.GetInstanceAt(i);
					const int sc = ii.GetSubInstanceCount();
					/*
					for(s=0; s<sc; s++){
						const deoglRenderTaskInstance::sSubInstance &ss = ii.GetSubInstanceAt(s);
						l.LogInfoFormat("pass=%d pipeline=%d tuc=%d vao=%d instance=%d spbInst=%d specFlags=%x",
							pass, pp.GetPipeline()->GetRTSIndex(), tt.GetTexture()->GetIndex(),
							vv.GetVAO()->GetIndex(), ii.GetInstance()->GetIndex(), ss.instance + 1, ss.flags);
					}
					*/
					l.LogInfoFormat("pass=%d pipeline=%d tuc=%d vao=%d instance=%d spbInst=%d specFlags=%x subInstCount=%d",
						pass, pp.GetPipeline()->GetRTSIndex(), tt.GetTexture()->GetIndex(), vv.GetVAO()->GetIndex(),
						ii.GetInstance()->GetIndex(), sc > 0 ? ii.GetSubInstanceAt(0).instance + 1 : -1,
						sc > 0 ? ii.GetSubInstanceAt(0).flags : 0, sc);
				}
			}
		}
	}
}
#endif
// DEBUG

void deoglRenderPlanTasks::WaitFinishBuildingTasksGeometry(){
	if(!pTaskGeometry){
		return;
	}
	
	pTaskGeometry->GetSemaphore().Wait();
	
	deoglRenderCanvas &rc = pPlan.GetRenderThread().GetRenderers().GetCanvas();
	rc.SampleDebugInfoPlanPrepareBuildRTs(pPlan, pTaskGeometry->GetElapsedTime());
	pTaskGeometry = NULL;
	
	// this call does modify a shader parameter block and can thus not be parallel
	pSolidGeometryTask->PrepareForRender();
	pSolidGeometryHeight1Task->PrepareForRender();
	pSolidGeometryHeight2Task->PrepareForRender();
	pSolidGeometryOutlineTask->PrepareForRender();
	pSolidDecalsTask->PrepareForRender();
	
	pSolidGeometryXRayTask->PrepareForRender();
	pSolidGeometryHeight1XRayTask->PrepareForRender();
	pSolidGeometryHeight2XRayTask->PrepareForRender();
	pSolidGeometryOutlineXRayTask->PrepareForRender();
	pSolidDecalsXRayTask->PrepareForRender();
	
	// DEBUG
#if 0
	deoglRTLogger &l = pPlan.GetRenderThread().GetLogger();
	LogRT(l, "SolidGeometryTask", 0, *pSolidGeometryTask);
	LogRT(l, "SolidDecalsTask", 3, *pSolidDecalsTask);
#endif
}

void deoglRenderPlanTasks::CleanUp(){
	WaitFinishBuildingTasksDepth();
	WaitFinishBuildingTasksGeometry();
	
	pSolidDepthTask->Clear();
	pSolidDepthOutlineTask->Clear();
	
	pSolidGeometryTask->Clear();
	pSolidGeometryHeight1Task->Clear();
	pSolidGeometryHeight2Task->Clear();
	pSolidGeometryOutlineTask->Clear();
	pSolidDecalsTask->Clear();
	
	pSolidDepthXRayTask->Clear();
	pSolidDepthOutlineXRayTask->Clear();
	
	pSolidGeometryXRayTask->Clear();
	pSolidGeometryHeight1XRayTask->Clear();
	pSolidGeometryHeight2XRayTask->Clear();
	pSolidGeometryOutlineXRayTask->Clear();
	pSolidDecalsXRayTask->Clear();
}



// Private Functions
//////////////////////

void deoglRenderPlanTasks::pBuildCRTSolidDepth(deoglComputeRenderTask &renderTask,
const deoglRenderPlanMasked *mask, bool xray){
	const deoglWorldCompute &worldCompute = pPlan.GetWorld()->GetCompute();
	const deoglComputeRenderTask::cGuard guard(renderTask, worldCompute, 2);
	
	renderTask.Clear();
	
	renderTask.SetSolid(true);
	renderTask.SetNoNotReflected(pPlan.GetNoReflections());
	renderTask.SetNoRendered(mask);
	if(xray){
		renderTask.SetFilterXRay(true);
		renderTask.SetXRay(true);
	}
	renderTask.SetRenderVSStereo(pPlan.GetRenderStereo()
		&& pPlan.GetRenderThread().GetChoices().GetRenderStereoVSLayer());
	
	if(mask && mask->GetUseClipPlane()){
		renderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etDepthClipPlane);
		
	}else{
		renderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etDepth);
	}
	
	int pipelineModifier = 0;
	if(pPlan.GetFlipCulling()){
		pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if(pPlan.GetRenderStereo()){
		pipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
	renderTask.SetSkinPipelineModifier(pipelineModifier);
	
	
	// pass 1: regular depth
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptComponent);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptBillboard);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptPropField);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptPropFieldImposter);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptHeightMap1);
	
	if(pPlan.GetRenderThread().GetChoices().GetRealTransparentParticles()){
		// addToRenderTask.AddParticles( collideList );
	}
	
	renderTask.EndPass(worldCompute);
	
	
	// pass 2: outline depth
	renderTask.SetOutline(true);
	renderTask.SetFilterDecal(true);
	renderTask.SetDecal(false);
	
	renderTask.SetSkinPipelineLists(0);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptOutline);
	
	renderTask.EndPass(worldCompute);
	
	
	// pass 3: model decals depth, decals depth but only if parent is fully solid
	/*
	renderTask.SetOutline(false);
	renderTask.SetDecal(true);
	renderTask.SetFilterSolid(false);
	
	renderTask.SetSkinPipelineLists(0);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptComponent);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptDecal);
	
	renderTask.EndPass(worldCompute);
	*/
}

void deoglRenderPlanTasks::pBuildCRTSolidGeometry(deoglComputeRenderTask &renderTask,
const deoglRenderPlanMasked*, bool xray){
	const deoglWorldCompute &worldCompute = pPlan.GetWorld()->GetCompute();
	const deoglComputeRenderTask::cGuard guard(renderTask, worldCompute, 4);
	
	renderTask.Clear();
	
	renderTask.SetSolid(true);
	renderTask.SetNoNotReflected(pPlan.GetNoReflections());
	renderTask.SetNoRendered(true);
	renderTask.SetFilterXRay(true);
	renderTask.SetXRay(xray);
	renderTask.SetRenderVSStereo(pPlan.GetRenderStereo()
		&& pPlan.GetRenderThread().GetChoices().GetRenderStereoVSLayer());
	
	renderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etGeometry);
	
	int pipelineModifier = 0;
	if(pPlan.GetFlipCulling()){
		pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if(pPlan.GetRenderStereo()){
		pipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
	renderTask.SetSkinPipelineModifier(pipelineModifier);
	
	
	// pass 1: regular, no decals, no overdraw, no outline
	renderTask.SetFilterDecal(true);
	renderTask.SetDecal(false);
	
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptComponent);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptBillboard);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptPropField);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptPropFieldImposter);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptHeightMap1);
	
	if(pPlan.GetRenderThread().GetChoices().GetRealTransparentParticles()){
		// addToRenderTask.AddParticles( collideList );
	}
	
	renderTask.EndPass(worldCompute);
	
	
	// pass 2: height terrain overdraw, no outline
	renderTask.SetSkinPipelineLists(0);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptHeightMap2);
	
	renderTask.EndPass(worldCompute);
	
	
	// pass 3: outline
	renderTask.SetOutline(true);
	
	renderTask.SetSkinPipelineLists(0);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptOutline);
	
	renderTask.EndPass(worldCompute);
	
	
	// pass 4: model decals, decals but only if not on potentially non-solid geometry
	renderTask.SetOutline(false);
	renderTask.SetDecal(true);
	// renderTask.SetFilterSolid(false);
	
	renderTask.SetSkinPipelineLists(0);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptComponent);
	renderTask.EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptDecal);
	
	renderTask.EndPass(worldCompute);
}
