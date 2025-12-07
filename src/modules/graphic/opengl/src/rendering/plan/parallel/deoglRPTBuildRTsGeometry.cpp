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

#include "deoglRPTBuildRTsGeometry.h"
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


// Class deoglRPTBuildRTsGeometry
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTBuildRTsGeometry::deoglRPTBuildRTsGeometry(deoglRenderPlanTasks &plan, const deoglRenderPlanMasked *mask) :
deParallelTask(&plan.GetPlan().GetRenderThread().GetOgl()),
pPlan(plan),
pMask(mask),
pElapsedTime(0.0f)
{
	(void)pMask; // silence compiler warning
	SetMarkFinishedAfterRun(true);
}

deoglRPTBuildRTsGeometry::~deoglRPTBuildRTsGeometry(){
}



// Management
///////////////

void deoglRPTBuildRTsGeometry::Run(){
	if(IsCancelled()){
		return;
	}
	
	const bool useComputeRenderTask = pPlan.GetPlan().GetRenderThread().GetChoices().GetUseComputeRenderTask(); // TEMP! removed once properly working in compute render task
	decTimer timer;
	try{
		pInitPipelineModifier();
		
		if(!useComputeRenderTask) pSolid(false);
		pSolidTerrain(false);
		if(!useComputeRenderTask) pSolidOutline(false);
		if(!useComputeRenderTask) pSolidDecals(false);
		
		if(!useComputeRenderTask) pSolid(true);
		pSolidTerrain(true);
		if(!useComputeRenderTask) pSolidOutline(true);
		if(!useComputeRenderTask) pSolidDecals(true);
		
	}catch(const deException &e){
		pPlan.GetPlan().GetRenderThread().GetLogger().LogException(e);
		pSemaphore.Signal();
		throw;
	}
	
	pElapsedTime = timer.GetElapsedTime();
	pSemaphore.Signal();
}

void deoglRPTBuildRTsGeometry::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTBuildRTsGeometry::GetDebugName() const{
	return "RPTBuildRTsGeometry";
}



// Private Functions
//////////////////////

void deoglRPTBuildRTsGeometry::pInitPipelineModifier(){
	pPipelineModifier = 0;
	if(pPlan.GetPlan().GetFlipCulling()){
		pPipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if(pPlan.GetPlan().GetRenderStereo()){
		pPipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
}

void deoglRPTBuildRTsGeometry::pSolid(bool xray){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidGeometryXRayTask() : pPlan.GetSolidGeometryTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask(pPlan.GetPlan().GetRenderThread(), renderTask);
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo(pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer());
	
	addToRenderTask.SetSolid(true);
	addToRenderTask.SetNoRendered(true);
	addToRenderTask.SetNoNotReflected(pPlan.GetPlan().GetNoReflections());
	addToRenderTask.SetFilterXRay(true);
	addToRenderTask.SetXRay(xray);
	addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etGeometry);
	addToRenderTask.SetSkinPipelineModifier(pPipelineModifier);
	
	// components without decals
	addToRenderTask.SetFilterDecal(true);
	addToRenderTask.SetDecal(false);
	addToRenderTask.AddComponents(collideList);
	addToRenderTask.SetFilterDecal(false);
	
	addToRenderTask.AddBillboards(collideList);
	
	addToRenderTask.AddPropFields(collideList, false);
	addToRenderTask.AddPropFields(collideList, true);
	addToRenderTask.AddPropFieldClusters(collideList, false);
	addToRenderTask.AddPropFieldClusters(collideList, true);
	
	if(pPlan.GetPlan().GetRenderThread().GetChoices().GetRealTransparentParticles()){
		addToRenderTask.AddParticles(collideList);
	}
}

void deoglRPTBuildRTsGeometry::pSolidTerrain(bool xray){
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	if(collideList.GetHTSectorCount() == 0 && collideList.GetHTSClusterCount() == 0){
		return;
	}
	
	{// pass 1
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidGeometryHeight1XRayTask() : pPlan.GetSolidGeometryHeight1Task();
	deoglAddToRenderTask addToRenderTask(pPlan.GetPlan().GetRenderThread(), renderTask);
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo(pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer());
	
	addToRenderTask.SetSolid(true);
	addToRenderTask.SetNoRendered(true);
	addToRenderTask.SetNoNotReflected(pPlan.GetPlan().GetNoReflections());
	addToRenderTask.SetFilterXRay(true);
	addToRenderTask.SetXRay(xray);
	addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etGeometry);
	addToRenderTask.SetSkinPipelineModifier(pPipelineModifier);
	
	addToRenderTask.AddHeightTerrains(collideList, true);
	addToRenderTask.AddHeightTerrainSectorClusters(collideList, true);
	}
	
	{// pass 2
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidGeometryHeight2XRayTask() : pPlan.GetSolidGeometryHeight2Task();
	deoglAddToRenderTask addToRenderTask(pPlan.GetPlan().GetRenderThread(), renderTask);
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo(pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer());
	
	addToRenderTask.SetSolid(true);
	addToRenderTask.SetNoRendered(true);
	addToRenderTask.SetNoNotReflected(pPlan.GetPlan().GetNoReflections());
	addToRenderTask.SetFilterXRay(true);
	addToRenderTask.SetXRay(xray);
	addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etGeometry);
	addToRenderTask.SetSkinPipelineModifier(pPipelineModifier);
	
	addToRenderTask.AddHeightTerrains(collideList, false);
	addToRenderTask.AddHeightTerrainSectorClusters(collideList, false);
	}
}

void deoglRPTBuildRTsGeometry::pSolidOutline(bool xray){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidGeometryOutlineXRayTask() : pPlan.GetSolidGeometryOutlineTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask(pPlan.GetPlan().GetRenderThread(), renderTask);
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo(pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer());
	
	addToRenderTask.SetOutline(true);
	addToRenderTask.SetFilterDecal(true);
	addToRenderTask.SetDecal(false);
	addToRenderTask.SetSolid(true);
	addToRenderTask.SetNoRendered(true);
	addToRenderTask.SetNoNotReflected(pPlan.GetPlan().GetNoReflections());
	addToRenderTask.SetFilterXRay(true);
	addToRenderTask.SetXRay(xray);
	addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etGeometry);
	addToRenderTask.SetSkinPipelineModifier(pPipelineModifier);
	
	addToRenderTask.AddComponents(collideList);
}

void deoglRPTBuildRTsGeometry::pSolidDecals(bool xray){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidDecalsXRayTask() : pPlan.GetSolidDecalsTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask(pPlan.GetPlan().GetRenderThread(), renderTask);
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo(pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer());
	
	addToRenderTask.SetNoRendered(true);
	addToRenderTask.SetNoNotReflected(pPlan.GetPlan().GetNoReflections());
	addToRenderTask.SetFilterXRay(true);
	addToRenderTask.SetXRay(xray);
	addToRenderTask.SetSkinPipelineType(deoglSkinTexturePipelines::etGeometry);
	addToRenderTask.SetSkinPipelineModifier(pPipelineModifier);
	
	// component model decals
	addToRenderTask.SetFilterDecal(true);
	addToRenderTask.SetDecal(true);
	
	addToRenderTask.SetSolid(true);
	addToRenderTask.AddComponents(collideList);
	
	addToRenderTask.SetSolid(false);
	addToRenderTask.AddComponents(collideList);
	addToRenderTask.SetFilterDecal(false);
	
	// component attached decals
	addToRenderTask.SetSolid(true);
	addToRenderTask.AddDecals(collideList);
}
