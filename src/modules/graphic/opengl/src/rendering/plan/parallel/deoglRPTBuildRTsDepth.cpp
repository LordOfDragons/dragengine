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

#include "deoglRPTBuildRTsDepth.h"
#include "../deoglRenderPlan.h"
#include "../deoglRenderPlanTasks.h"
#include "../deoglRenderPlanMasked.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../rendering/task/deoglAddToRenderTask.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTChoices.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>


// Class deoglRPTBuildRTsDepth
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTBuildRTsDepth::deoglRPTBuildRTsDepth(deoglRenderPlanTasks &plan, const deoglRenderPlanMasked *mask) :
deParallelTask(&plan.GetPlan().GetRenderThread().GetOgl()),
pPlan(plan),
pMask(mask),
pElapsedTime(0.0f)
{
	SetMarkFinishedAfterRun(true);
}

deoglRPTBuildRTsDepth::~deoglRPTBuildRTsDepth(){
}



// Management
///////////////

void deoglRPTBuildRTsDepth::Run(){
	if(IsCancelled()){
		return;
	}
	
	decTimer timer;
	try{
		pSolid(false);
		pSolidOutline(false);
		
		pSolid(true);
		pSolidOutline(true);
		
	}catch(const deException &e){
		pPlan.GetPlan().GetRenderThread().GetLogger().LogException(e);
		pSemaphore.Signal();
		throw;
	}
	
	pElapsedTime = timer.GetElapsedTime();
	pSemaphore.Signal();
}

void deoglRPTBuildRTsDepth::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTBuildRTsDepth::GetDebugName() const{
	return "RPTBuildRTsDepth";
}



// Private Functions
//////////////////////
 
void deoglRPTBuildRTsDepth::pSolid(bool xray){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidDepthXRayTask() : pPlan.GetSolidDepthTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask(pPlan.GetPlan().GetRenderThread(), renderTask);
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo(pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer());
	
	addToRenderTask.SetSolid(true);
	addToRenderTask.SetNoNotReflected(pPlan.GetPlan().GetNoReflections());
	addToRenderTask.SetNoRendered(pMask);
	if(xray){
		addToRenderTask.SetFilterXRay(true);
		addToRenderTask.SetXRay(true);
	}
	
	if(pMask && pMask->GetUseClipPlane()){
		addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etDepthClipPlane);
		
	}else{
		addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etDepth);
	}
	
	int pipelineModifier = 0;
	if(pPlan.GetPlan().GetFlipCulling()){
		pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if(pPlan.GetPlan().GetRenderStereo()){
		pipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
	addToRenderTask.SetSkinPipelineModifier(pipelineModifier);
	
	addToRenderTask.AddComponents(collideList);
	
	addToRenderTask.AddBillboards(collideList);
	
	addToRenderTask.AddPropFields(collideList, false);
	addToRenderTask.AddPropFields(collideList, true);
	addToRenderTask.AddPropFieldClusters(collideList, false);
	addToRenderTask.AddPropFieldClusters(collideList, true);
	
	addToRenderTask.AddHeightTerrains(collideList, true);
	addToRenderTask.AddHeightTerrainSectorClusters(collideList, true);
	
	if(pPlan.GetPlan().GetRenderThread().GetChoices().GetRealTransparentParticles()){
		addToRenderTask.AddParticles(collideList);
	}
}

void deoglRPTBuildRTsDepth::pSolidOutline(bool xray){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidDepthOutlineXRayTask() : pPlan.GetSolidDepthOutlineTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask(pPlan.GetPlan().GetRenderThread(), renderTask);
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo(pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer());
	
	addToRenderTask.SetOutline(true);
	addToRenderTask.SetFilterDecal(true);
	addToRenderTask.SetDecal(false);
	addToRenderTask.SetSolid(true);
	addToRenderTask.SetNoNotReflected(pPlan.GetPlan().GetNoReflections());
	addToRenderTask.SetNoRendered(pMask);
	if(xray){
		addToRenderTask.SetFilterXRay(true);
		addToRenderTask.SetXRay(true);
	}
	
	if(pMask && pMask->GetUseClipPlane()){
		addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etDepthClipPlane);
		
	}else{
		addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etDepth);
	}
	
	int pipelineModifier = 0;
	if(pPlan.GetPlan().GetFlipCulling()){
		pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if(pPlan.GetPlan().GetRenderStereo()){
		pipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
	addToRenderTask.SetSkinPipelineModifier(pipelineModifier);
	
	if(pMask && pMask->GetUseClipPlane()){
		addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etDepthClipPlane);
		
	}else{
		addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etDepth);
	}
	
	addToRenderTask.AddComponents(collideList);
}
