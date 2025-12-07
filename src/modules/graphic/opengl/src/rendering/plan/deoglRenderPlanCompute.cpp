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
#include "deoglRenderPlanCompute.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListLight.h"
#include "../../component/deoglRComponent.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../gi/deoglGICascade.h"
#include "../../gi/deoglGIState.h"
#include "../../propfield/deoglRPropField.h"
#include "../../rendering/deoglRenderCompute.h"
#include "../../rendering/deoglRenderOcclusion.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../shaders/paramblock/deoglSPBMapBufferRead.h"
#include "../../terrain/heightmap/deoglHTView.h"
#include "../../terrain/heightmap/deoglHTViewSector.h"
#include "../../terrain/heightmap/deoglHTViewSectorCluster.h"
#include "../../terrain/heightmap/deoglHTSCluster.h"
#include "../../terrain/heightmap/deoglRHTSector.h"
#include "../../world/deoglRWorld.h"
#include "../../world/deoglWorldOctree.h"

#include <dragengine/common/exceptions.h>


// Class deoglRenderPlanCompute
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanCompute::deoglRenderPlanCompute(deoglRenderPlan &plan) :
pPlan(plan)
{
	const bool rowMajor = plan.GetRenderThread().GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	pUBOFindConfig.TakeOver(new deoglSPBlockUBO(plan.GetRenderThread()));
	pUBOFindConfig->SetRowMajor(rowMajor);
	pUBOFindConfig->SetParameterCount(25);
	pUBOFindConfig->GetParameterAt(efcpNodeCount).SetAll(deoglSPBParameter::evtInt, 1, 1, 1); // uint
	pUBOFindConfig->GetParameterAt(efcpElementCount).SetAll(deoglSPBParameter::evtInt, 1, 1, 1); // uint
	pUBOFindConfig->GetParameterAt(efcpFrustumPlanes).SetAll(deoglSPBParameter::evtFloat, 4, 1, 6); // vec4[6]
	pUBOFindConfig->GetParameterAt(efcpFrustumPlanesAbs).SetAll(deoglSPBParameter::evtFloat, 3, 1, 6); // vec3[6]
	pUBOFindConfig->GetParameterAt(efcpFrustumSelect).SetAll(deoglSPBParameter::evtBool, 3, 1, 6); // bvec3[6]
	pUBOFindConfig->GetParameterAt(efcpGIMinExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1); // vec3
	pUBOFindConfig->GetParameterAt(efcpGIMaxExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1); // vec3
	pUBOFindConfig->GetParameterAt(efcpLayerMask).SetAll(deoglSPBParameter::evtInt, 2, 1, 1); // uvec2
	pUBOFindConfig->GetParameterAt(efcpCullLayerMask).SetAll(deoglSPBParameter::evtBool, 1, 1, 1); // bool
	pUBOFindConfig->GetParameterAt(efcpCullFlags).SetAll(deoglSPBParameter::evtInt, 1, 1, 1); // uint
	pUBOFindConfig->GetParameterAt(efcpCameraPosition).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1); // vec3
	pUBOFindConfig->GetParameterAt(efcpCameraView).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1); // vec3
	pUBOFindConfig->GetParameterAt(efcpErrorScaling).SetAll(deoglSPBParameter::evtFloat, 1, 1, 1); // float
	pUBOFindConfig->GetParameterAt(efcpHullEdgeNormal).SetAll(deoglSPBParameter::evtFloat, 4, 1, 5); // vec4[5]
	pUBOFindConfig->GetParameterAt(efcpHullEdgeDistance).SetAll(deoglSPBParameter::evtFloat, 1, 1, 5); // float[5]
	pUBOFindConfig->GetParameterAt(efcpMatrixLightSpace).SetAll(deoglSPBParameter::evtFloat, 4, 3, 1); // mat4x3
	pUBOFindConfig->GetParameterAt(efcpShadowAxis).SetAll(deoglSPBParameter::evtFloat, 3, 1, 3); // vec3[3]
	pUBOFindConfig->GetParameterAt(efcpLightShaftFar).SetAll(deoglSPBParameter::evtFloat, 1, 1, 1); // float
	pUBOFindConfig->GetParameterAt(efcpSplitMinExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 4); // vec3[4]
	pUBOFindConfig->GetParameterAt(efcpSplitMaxExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 4); // vec3[4]
	pUBOFindConfig->GetParameterAt(efcpSplitSizeThreshold).SetAll(deoglSPBParameter::evtFloat, 2, 1, 4); // vec2[4]
	pUBOFindConfig->GetParameterAt(efcpSplitCount).SetAll(deoglSPBParameter::evtInt, 1, 1, 1); // int
	pUBOFindConfig->GetParameterAt(efcpLodFactor).SetAll(deoglSPBParameter::evtFloat, 4, 1, 1); // vec4
	pUBOFindConfig->GetParameterAt(efcpLodOffset).SetAll(deoglSPBParameter::evtInt, 1, 1, 1); // uint
	pUBOFindConfig->GetParameterAt(efcpLodMethod).SetAll(deoglSPBParameter::evtInt, 1, 1, 1); // uint
	pUBOFindConfig->MapToStd140();
	
	pSSBOCounters.TakeOver(new deoglSPBlockSSBO(plan.GetRenderThread().GetRenderers().
		GetCompute().GetSSBOCounters(), deoglSPBlockSSBO::etRead));
	pSSBOCounters->SetElementCount(1);
	
	pSSBOVisibleElements.TakeOver(new deoglSPBlockSSBO(plan.GetRenderThread(), deoglSPBlockSSBO::etRead));
	pSSBOVisibleElements->SetRowMajor(rowMajor);
	pSSBOVisibleElements->SetParameterCount(1);
	pSSBOVisibleElements->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 4, 1, 1); // uvec4
	pSSBOVisibleElements->MapToStd140();
	pSSBOVisibleElements->EnsureBuffer();
	
	pSSBOVisibleElements2.TakeOver(new deoglSPBlockSSBO(pSSBOVisibleElements));
	pSSBOVisibleElements2->EnsureBuffer();
	
	pRTOcclusion.TakeOver(new deoglComputeRenderTask(plan.GetRenderThread()));
	pRTOcclusion->SetFilterSolid(false);
	pRTOcclusion->SetOcclusion(true);
	pRTOcclusion->EnableSkinPipelineList(deoglSkinTexturePipelinesList::eptComponent);
}

deoglRenderPlanCompute::~deoglRenderPlanCompute(){
}



// Management
///////////////

void deoglRenderPlanCompute::PrepareWorldCompute(){
	// decTimer timer;
	
	deoglWorldCompute &compute = pPlan.GetWorld()->GetCompute();
	compute.Prepare();
	// pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanCompute.PrepareWorldCompute: %dys", ( int )( timer.GetElapsedTime() * 1e6f ) );
}

void deoglRenderPlanCompute::PrepareBuffers(){
	// decTimer timer;
	
	pPrepareFindConfig();
	
	const int visElCount = ((pPlan.GetWorld()->GetCompute().GetElementCount() - 1) / 4) + 1;
	pPrepareBuffer(pSSBOVisibleElements, visElCount);
	pPrepareBuffer(pSSBOVisibleElements2, visElCount);
	
	pSSBOCounters->ClearDataUInt(0, 1, 1, 0); // workGroupSize.xyz, count
	// pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanCompute.PrepareBuffers: %dys", ( int )( timer.GetElapsedTime() * 1e6f ) );
}

void deoglRenderPlanCompute::ReadVisibleElements(){
	const deoglDebugTraceGroup debugTrace(pPlan.GetRenderThread(), "PlanCompute.ReadVisibleElements");
	const deoglWorldCompute &wcompute = pPlan.GetWorld()->GetCompute();
	if(wcompute.GetElementCount() == 0){
		return;
	}
	
		// decTimer timer;
	const deoglSPBMapBufferRead mappedCounters(pSSBOCounters, 0, 1);
	const deoglRenderCompute::sCounters &counters =
		*( deoglRenderCompute::sCounters* )pSSBOCounters->GetMappedBuffer();
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("ReadVisibleElements: counter %dys", (int)(timer.GetElapsedTime()*1e6f));
	const int indexCount = counters.counter;
	if(indexCount == 0){
		return;
	}
	
	// read written visible element indices
	deoglCollideList &collideList = pPlan.GetCollideList();
	deoglParticleEmitterInstanceList &clistParticleEmitterInstanceList = collideList.GetParticleEmitterList();
	deoglHTView * const htview = pPlan.GetHeightTerrainView();
	
	pSSBOVisibleElements->GPUReadToCPU(((indexCount - 1) / 4) + 1);
	const deoglSPBMapBufferRead mappedIndices(pSSBOVisibleElements, 0, ((indexCount - 1) / 4) + 1);
	const uint32_t * const indices = (const uint32_t *)pSSBOVisibleElements->GetMappedBuffer();
	int i;
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("ReadVisibleElements: read %dys", (int)(timer.GetElapsedTime()*1e6f));
	
	for(i=0; i<indexCount; i++){
		// NOTE indices are composed of 24-bit index and 8-bit flags. the shader does not write
		//      flags so we can use the index value directly without "& 0xffffff"
		const deoglWorldComputeElement &element = wcompute.GetElementAt(indices[i]);
		// continue;
		
		switch(element.GetType()){
		case deoglWorldComputeElement::eetComponent:
			collideList.AddComponent((deoglRComponent*)element.GetOwner());
			break;
			
		case deoglWorldComputeElement::eetBillboard:
			collideList.AddBillboard((deoglRBillboard*)element.GetOwner());
			break;
			
		case deoglWorldComputeElement::eetParticleEmitter:
			clistParticleEmitterInstanceList.Add((deoglRParticleEmitterInstance*)element.GetOwner());
			break;
			
		case deoglWorldComputeElement::eetLight:
			collideList.AddLight((deoglRLight*)element.GetOwner())->TestInside(pPlan);
			break;
			
		case deoglWorldComputeElement::eetPropFieldCluster:
			collideList.AddPropFieldCluster((deoglPropFieldCluster*)element.GetOwner());
			break;
			
		case deoglWorldComputeElement::eetHeightTerrainSectorCluster:
			if(htview){
				const deoglHTSCluster &cluster = *(deoglHTSCluster*)element.GetOwner();
				collideList.AddHTSCluster(&htview->GetSectorAt(
					cluster.GetHTSector()->GetIndex())->GetClusterAt(cluster.GetIndex()));
			}
			break;
			
		case deoglWorldComputeElement::eetDecal:
			break;
		}
	}
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("ReadVisibleElements: list %dys", (int)(timer.GetElapsedTime()*1e6f));
}

void deoglRenderPlanCompute::UpdateElementGeometries(){
	// decTimer timer;
	
	deoglWorldCompute &compute = pPlan.GetWorld()->GetCompute();
	
	compute.PrepareGeometries();
	// pPlan.GetRenderThread().GetLogger().LogInfoFormat("RenderPlanCompute.UpdateElementGeometries: Prepare %dys (%d)", (int)(timer.GetElapsedTime()*1e6f), compute.GetUpdateElementGeometryCount());
	
	if(compute.GetClearGeometryCount() > 0){
		pPlan.GetRenderThread().GetRenderers().GetCompute().ClearGeometries(pPlan);
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("RenderPlanCompute.UpdateElementGeometries: Clear %dys", (int)(timer.GetElapsedTime()*1e6f));
	}
	
	if(compute.GetUpdateElementGeometryCount() > 0){
		pPlan.GetRenderThread().GetRenderers().GetCompute().UpdateElementGeometries(pPlan);
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("RenderPlanCompute.UpdateElementGeometries: Update %dys", (int)(timer.GetElapsedTime()*1e6f));
	}
}

void deoglRenderPlanCompute::BuildRTOcclusion(const deoglRenderPlanMasked *mask){
	deoglRenderCompute &renderCompute = pPlan.GetRenderThread().GetRenderers().GetCompute();
	deoglRenderOcclusion &renderOcclusion = pPlan.GetRenderThread().GetRenderers().GetOcclusion();
	deoglWorldCompute &worldCompute = pPlan.GetWorld()->GetCompute();
	const deoglDebugTraceGroup debugTrace(pPlan.GetRenderThread(), "PlanCompute.BuildRTOcclusion");
	
	{
	const deoglComputeRenderTask::cGuard guard(pRTOcclusion, worldCompute, 1);
	pRTOcclusion->SetNoRendered(pPlan.GetNoRenderedOccMesh());
	pRTOcclusion->SetPipelineDoubleSided(renderOcclusion.GetRenderOcclusionMapRTS(pPlan, mask, true, false));
	pRTOcclusion->SetPipelineSingleSided(renderOcclusion.GetRenderOcclusionMapRTS(pPlan, mask, true, true));
	pRTOcclusion->EndPass(worldCompute);
	}
	
	renderCompute.ClearCullResult(pPlan);
	renderCompute.UpdateCullResultOcclusion(pPlan, pUBOFindConfig, pSSBOVisibleElements, pSSBOCounters);
	renderCompute.BuildRenderTaskOcclusion(pPlan, pRTOcclusion);
}

void deoglRenderPlanCompute::ReadyRTOcclusion(const deoglRenderPlanMasked *mask){
	if(pRTOcclusion->ReadBackSteps()){
		return;
	}
	
	BuildRTOcclusion(mask);
	DEASSERT_TRUE(pRTOcclusion->ReadBackSteps())
}

void deoglRenderPlanCompute::SwapVisibleElements(){
	deoglSPBlockSSBO::Ref swap = pSSBOVisibleElements;
	pSSBOVisibleElements = pSSBOVisibleElements2;
	pSSBOVisibleElements2 = swap;
}



// Protected
//////////////

void deoglRenderPlanCompute::pPrepareFindConfig(){
	deoglSPBlockUBO &ubo = pUBOFindConfig;
	const deoglSPBMapBuffer mapped(ubo);
	ubo.Clear();
	
	const decDVector &refpos = pPlan.GetWorld()->GetReferencePosition();
	const deoglWorldCompute &wcompute = pPlan.GetWorld()->GetCompute();
	
	ubo.SetParameterDataUInt(efcpNodeCount, 0);
	ubo.SetParameterDataUInt(efcpElementCount, wcompute.GetElementCount());
	
	// frustum culling
	const deoglDCollisionFrustum &frustum = *pPlan.GetUseFrustum();
	pSetFrustumPlane(ubo, 0, frustum.GetLeftNormal(), frustum.GetLeftDistance());
	pSetFrustumPlane(ubo, 1, frustum.GetRightNormal(), frustum.GetRightDistance());
	pSetFrustumPlane(ubo, 2, frustum.GetTopNormal(), frustum.GetTopDistance());
	pSetFrustumPlane(ubo, 3, frustum.GetBottomNormal(), frustum.GetBottomDistance());
	pSetFrustumPlane(ubo, 4, frustum.GetNearNormal(), frustum.GetNearDistance());
	pSetFrustumPlane(ubo, 5, frustum.GetFarNormal(), frustum.GetFarDistance());
	
	// gi cascase testing. to disable set min and max extends to the same value best far away
	const deoglGIState * const gistate = pPlan.GetUpdateGIState();
	if(gistate){
		// correctly we would have to use here the cascade position but during this
		// parallel task run at an unknown time the cascade position is updated.
		// so instead we use the camera position which is the base for the cascade
		// position update. this can cause a potential error at the boundary of the
		// GI cascade but chances for this are slim. if really a problem half the
		// probe spacing can be added to the extends
		const deoglGICascade &cascade = gistate->GetActiveCascade();
		const decDVector position(pPlan.GetCameraPosition() - refpos);
		const decDVector halfExtend(cascade.GetDetectionBox());
		
		ubo.SetParameterDataVec3(efcpGIMinExtend, position - halfExtend);
		ubo.SetParameterDataVec3(efcpGIMaxExtend, position + halfExtend);
	}
	
	ubo.SetParameterDataVec3(efcpCameraPosition, pPlan.GetCameraPosition() - refpos);
	ubo.SetParameterDataVec3(efcpCameraView, pPlan.GetInverseCameraMatrix().TransformView());
	
	ubo.SetParameterDataFloat(efcpErrorScaling, pCalculateErrorScaling());
	
	pCullLayerMask(ubo);
	
	// cull by flags
	uint32_t cullFlags = 0;
	if(pPlan.GetIgnoreDynamicComponents()){
		cullFlags |= deoglWorldCompute::eefDynamic;
	}
	ubo.SetParameterDataUInt(efcpCullFlags, cullFlags);
	
	// lod calculation
	const float fovX = pPlan.GetCameraFov();
	const float fovY = fovX * pPlan.GetCameraFovRatio();
	const float maxPixelError = (float)pPlan.GetLodMaxPixelError();
	const float lodFactorX = tanf(fovX * 0.5f) * maxPixelError / pPlan.GetViewportWidth();
	const float lodFactorY = tanf(fovY * 0.5f) * maxPixelError / pPlan.GetViewportHeight();
	const float lodFactor = decMath::min(lodFactorX, lodFactorY);
	
	ubo.SetParameterDataVec4(efcpLodFactor, lodFactor, 0.0f, 0.0f, 0.0f);
	ubo.SetParameterDataUInt(efcpLodOffset, pPlan.GetLodLevelOffset());
	ubo.SetParameterDataUInt(efcpLodMethod, elmProjection);
}

void deoglRenderPlanCompute::pPrepareBuffer(deoglSPBlockSSBO &ssbo, int count){
	if(count > ssbo.GetElementCount()){
		ssbo.SetElementCount(count);
		ssbo.EnsureBuffer();
	}
}

void deoglRenderPlanCompute::pSetFrustumPlane(deoglSPBlockUBO &ubo, int i, const decDVector& n, double d){
	ubo.SetParameterDataArrayVec4(efcpFrustumPlanes, i, n, d);
	ubo.SetParameterDataArrayVec3(efcpFrustumPlanesAbs, i, n.Absolute());
	ubo.SetParameterDataArrayBVec3(efcpFrustumSelect, i, n.x > 0.0, n.y > 0.0, n.z > 0.0);
}

void deoglRenderPlanCompute::pCalculateFrustumBoundaryBox(
decDVector &frustumMinExtend, decDVector &frustumMaxExtend){
	// calculate the corner points of the frustum in light space. only the far points and the origin
	// are transformed. correctly the near points would have to be transformed and processed too but
	// they are usually so close to the origin that processing them is a waste of time not yielding
	// any better results than using just the origin
	const decDMatrix &matrix = pPlan.GetInverseCameraMatrix();
	const double fov = (double)pPlan.GetCameraFov();
	const double fovRatio = (double)pPlan.GetCameraFovRatio();
	const double zfar = (double)pPlan.GetCameraViewDistance();
	const double xfar = tan(fov * 0.5) * zfar; // * znear, dropped since we calc x'=z'*(xnear/znear)
	const double yfar = tan(fov * 0.5 * fovRatio) * zfar; // * znear, dropped since we calc y'=z'*(ynear/znear)
	decDVector points[4];
	int i;
	
	matrix.Transform(points[0], -xfar, yfar, zfar);
	matrix.Transform(points[1], xfar, yfar, zfar);
	matrix.Transform(points[2], xfar, -yfar, zfar);
	matrix.Transform(points[3], -xfar, -yfar, zfar);
	
	// determine the box surrounding the entire frustum
	frustumMinExtend = pPlan.GetCameraPosition();
	frustumMaxExtend = frustumMinExtend;
	
	for(i=0; i<4; i++){
		if(points[i].x < frustumMinExtend.x){
			frustumMinExtend.x = points[i].x;
			
		}else if(points[i].x > frustumMaxExtend.x){
			frustumMaxExtend.x = points[i].x;
		}
		
		if(points[i].y < frustumMinExtend.y){
			frustumMinExtend.y = points[i].y;
			
		}else if(points[i].y > frustumMaxExtend.y){
			frustumMaxExtend.y = points[i].y;
		}
		
		if(points[i].z < frustumMinExtend.z){
			frustumMinExtend.z = points[i].z;
			
		}else if(points[i].z > frustumMaxExtend.z){
			frustumMaxExtend.z = points[i].z;
		}
	}
	
	frustumMinExtend -= pPlan.GetWorld()->GetReferencePosition();
	frustumMaxExtend -= pPlan.GetWorld()->GetReferencePosition();
}

float deoglRenderPlanCompute::pCalculateErrorScaling(){
	// aspectRatio = w / h
	// dx = tan(fov / 2) * (cullPixelSize / (w / 2))
	// dy = tan(fov * fovRatio / 2) * (cullPixelSize / (w / 2)) / aspectRatio
	// dz = 1
	// 
	// x = dx * z
	// y = dy * z
	
	const float cullPixelSize = 1.0f;
	const float fov = pPlan.GetCameraFov();
	const float fovRatio = pPlan.GetCameraFovRatio();
	const float halfWidth = (float)pPlan.GetViewportWidth() * 0.5f;
	const float halfHeight = (float)pPlan.GetViewportHeight() * 0.5f;
	const float aspectRatio = halfWidth / halfHeight;
	const float dx = tanf(fov * 0.5f) * (cullPixelSize / halfWidth);
	const float dy = tanf(fov * 0.5f * fovRatio) * (cullPixelSize / halfWidth) / aspectRatio;
	
	return decMath::min(dx, dy);
}

void deoglRenderPlanCompute::pCullLayerMask(deoglSPBlockUBO &ubo){
	ubo.SetParameterDataBool(efcpCullLayerMask, pPlan.GetUseLayerMask());
	
	const uint64_t layerMask = pPlan.GetLayerMask().GetMask();
	ubo.SetParameterDataUVec2(efcpLayerMask, (uint32_t)(layerMask >> 32), (uint32_t)layerMask);
}
