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
#include "deoglRenderPlanDebug.h"
#include "deoglRenderPlanLight.h"
#include "deoglRenderPlanSkyLight.h"
#include "deoglRenderPlanMasked.h"
#include "deoglRenderPlanEnvMap.h"
#include "parallel/deoglRPTFindContent.h"
#include "../deoglRenderCompute.h"
#include "../deoglRenderOcclusion.h"
#include "../deoglRenderReflection.h"
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../light/deoglRenderLight.h"
#include "../lod/deoglLODCalculator.h"
#include "../../deGraphicOpenGl.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListLight.h"
#include "../../collidelist/deoglCollideListManager.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../light/deoglRLight.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../occlusiontest/deoglOcclusionTest.h"
#include "../../occlusiontest/deoglOcclusionMapPool.h"
#include "../../occlusiontest/deoglOcclusionTestPool.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIState.h"
#include "../../particle/deoglRParticleEmitter.h"
#include "../../particle/deoglRParticleEmitterInstance.h"
#include "../../particle/deoglRParticleEmitterInstanceType.h"
#include "../../particle/deoglRParticleEmitterType.h"
#include "../../propfield/deoglRPropField.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/light/deoglRenderGI.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../shadow/deoglShadowMapper.h"
#include "../../shadow/deoglShadowCaster.h"
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../sky/deoglRSky.h"
#include "../../sky/deoglRSkyInstance.h"
#include "../../sky/deoglRSkyInstanceLayer.h"
#include "../../terrain/heightmap/deoglHTSCluster.h"
#include "../../terrain/heightmap/deoglHTView.h"
#include "../../terrain/heightmap/deoglHTViewSector.h"
#include "../../terrain/heightmap/deoglRHeightTerrain.h"
#include "../../texture/texture2d/deoglRenderableColorTexture.h"
#include "../../utils/collision/deoglDCollisionFrustum.h"
#include "../../utils/collision/deoglDCollisionSphere.h"
#include "../../vr/deoglVR.h"
#include "../../world/deoglRCamera.h"
#include "../../world/deoglRWorld.h"
#include "../../world/deoglWorldOctree.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>

#ifdef OS_W32
#undef near
#undef far
#endif



// Class deoglRenderPlan
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlan::deoglRenderPlan(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pWorld(nullptr),

pIsRendering(false),

pLevel(0),

pUseGIState(false),
pRenderStereo(false),
pUseConstGIState(NULL),
pRenderVR(ervrNone),
pSkyLightCount(0),
pLodMaxPixelError(0),
pLodLevelOffset(0),
pOcclusionMap(NULL),
pOcclusionTest(NULL),
pGIState(NULL),
pTaskFindContent(NULL)
{
	pCamera = NULL;
	pCameraFov = DEG2RAD * 90.0f;
	pCameraFovRatio = 1.0f;
	pCameraImageDistance = 0.01f;
	pCameraViewDistance = 200.0f;
	pCameraAdaptedIntensity = 1.0f;
	pViewportWidth = 100;
	pViewportHeight = 100;
	pAspectRatio = (float)pViewportWidth / (float)pViewportHeight;
	
	pUpscaleWidth = 0;
	pUpscaleHeight = 0;
	pUseUpscaling = false;
	pUpsideDown = false;
	pUseToneMap = true;
	pIgnoreDynamicComponents = false;
	pRenderDebugPass = true;
	pNoReflections = false;
	pNoAmbientLight = false;
	
	pUseLayerMask = false;
	
	pUseCustomFrustum = false;
	
	pFBOTarget = NULL;
	pFBOMaterial = NULL;
	
	pDirtyProjMat = true;
	
	pNoRenderedOccMesh = false;
	pFlipCulling = false;
	
	pDisableLights = false;
	pShadowMapSize = 128;
	pShadowCubeSize = 128;
	pShadowSkySize = 128;
	
	pEnvMaps = new deoglRenderPlanEnvMap[16];
	pEnvMapCount = 8; //4;
	
	pDirectEnvMapFader.SetFadePerTime(1.0f);
	
	pLights = NULL;
	pLightCount = 0;
	pLightSize = 0;
	
	pMaskedPlans = NULL;
	pMaskedPlanCount = 0;
	pMaskedPlanSize = 0;
	
	pEmptyPass = false;
	pClearStencilPassBits = false;
	pClearColor = true;
	
	pTransparencyLayerCount = 0;
	pCurTransparencyLayer = 0;
	pRenderPassNumber = 0;
	pStencilRefValue = 0;
	pStencilPrevRefValue = 0;
	pStencilWriteMask = 0xf;
	
	pDebug = NULL;
	pDebugTiming = false;
}

deoglRenderPlan::~deoglRenderPlan(){
	CleanUp();
	SetWorld(NULL);
	
	int i, count;
	
	if(pMaskedPlans){
		while(pMaskedPlanSize > 0){
			pMaskedPlanSize--;
			if(pMaskedPlans[pMaskedPlanSize]){
				delete pMaskedPlans[pMaskedPlanSize];
			}
		}
		
		delete [] pMaskedPlans;
	}
	
	count = pSkyLights.GetCount();
	for(i=0; i<count; i++){
		delete (deoglRenderPlanSkyLight*)pSkyLights.GetAt(i);
	}
	pSkyLights.RemoveAll();
	
	//RemoveAllLights();
	if(pLights){
		while(pLightSize > 0){
			pLightSize--;
			if(pLights[pLightSize]){
				delete pLights[pLightSize];
			}
		}
		
		delete [] pLights;
	}
	
	pHTView = nullptr;
	
	if(pDebug){
		delete pDebug;
	}
	
	pDirectEnvMapFader.DropAll();
	
	if(pEnvMaps){
		for(i=0; i<pEnvMapCount; i++){
			if(pEnvMaps[i].GetEnvMap()){
				pEnvMaps[i].GetEnvMap()->RemovePlanUsage();
				pEnvMaps[i].GetEnvMap()->GetRenderPlanList().Remove(this);
			}
			pEnvMaps[i].SetEnvMap(nullptr);
		}
		delete [] pEnvMaps;
	}
	
	if(pGIState){
		delete pGIState;
	}
}



// Management
///////////////

void deoglRenderPlan::SetWorld(deoglRWorld *world){
	if(pWorld == world){
		return;
	}
	
	if(pWorld && pGIState){
		pWorld->RemoveGICascade(pGIState);
	}
	
	pWorld = world;
	
	if(pGIState){
		pGIState->Invalidate();
		
		if(world){
			world->AddGICascade(pGIState);
		}
	}
	
	pHTView = nullptr;
}

void deoglRenderPlan::SetLevel(int level){
	pLevel = level;
}



void deoglRenderPlan::PrepareRender(const deoglRenderPlanMasked *mask){
	if(pIsRendering){
		return; // re-entrant rendering causes exceptions. ignore rendering in this case
	}
	
	const deoglDebugTraceGroup debugTrace(pRenderThread, "Plan.PrepareRender");
	pIsRendering = true;
	
	try{
		if(pRenderVR == ervrRightEye){
			pBarePrepareRenderRightEye();
			
		}else{
			pBarePrepareRender(mask);
		}
		
	}catch(const deException &){
		pIsRendering = false;
		throw;
	}
	
	pIsRendering = false;
}

void deoglRenderPlan::PrepareRenderSkyOnly(){
	UpdateRefPosCameraMatrix(); // this is required for rendering but not updating world ref pos
	pPlanCameraProjectionMatrix();
}

// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) if(pDebugTiming) pRenderThread.GetLogger().LogInfoFormat("RenderPlan.Prepare: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRenderPlan::pBarePrepareRender(const deoglRenderPlanMasked *mask){
	if(!pWorld){
		return;
	}
	
	deoglRenderCanvas &renderCanvas = pRenderThread.GetRenderers().GetCanvas();
	renderCanvas.ClearAllDebugInfoPlanPrepare(*this);
	INIT_SPECIAL_TIMING
	
	CleanUp(); // just to make sure everything is clean
	
	// we can not create these objects during construction time since they can create OpenGL
	// objects and render plan objects are potentially created from inside main thread
	if(!pCompute){
		pCompute.TakeOverWith*this);
	}
	if(!pTasks){
		pTasks.TakeOverWith*this);
	}
	
	// the rest is safe
	pDebugPrepare();
	pPlanCamera();
	SPECIAL_TIMER_PRINT("PrepareCamera")
	
	pDisableLights = pWorld->GetDisableLights();
	pWorld->EarlyPrepareForRender(*this);
	renderCanvas.SampleDebugInfoPlanPrepareEarlyWorld(*this);
	SPECIAL_TIMER_PRINT("EarlyPrepareWorld")
	
	pUpdateHTView();
	SPECIAL_TIMER_PRINT("PrepareHTV")
	
	// plan what we can plan already
	pPlanDominance();
	pPlanGI();
	pPlanSky();
	pPlanSkyLight();
	pPlanShadowCasting();
	
	pStartFindContent(mask); // starts parallel tasks
	SPECIAL_TIMER_PRINT("Planning")
	
	// these calls run in parallel with above started tasks
	pWorld->PrepareForRender(*this, mask);
	pRenderThread.GetShader().UpdateSSBOSkinTextures();
	if(pRenderThread.GetChoices().GetUseComputeRenderTask()){
		pCompute->UpdateElementGeometries();
		pCompute->BuildRTOcclusion(mask);
	}
	renderCanvas.SampleDebugInfoPlanPrepareWorld(*this);
	SPECIAL_TIMER_PRINT("PrepareWorld")
	
	if(!pNoReflections){
		// NOTE requires world prepare to be fully done first.
		// 
		// NOTE this can trigger rendering in the world and thus can trigger
		//      deoglRWorld::EarlyPrepareForRender() and deoglRWorld::PrepareForRender()
		//      calls. if this would happen the EarlyPrepareForRender call can potentially
		//      run in parallel to find content tasks which can cause data race. a dirty
		//      flag in deoglRWorld prevents this from happening
		pPlanEnvMaps();
		SPECIAL_TIMER_PRINT("PrepareEnv")
	}
	
	pUpdateGI();
	SPECIAL_TIMER_PRINT("UpdateGI")
	
	// NOTE indirectly accesses projection matrix. renders though a separate occlusion map
	//      so the regular projection matrix could be used. occlusion though can be testes
	//      for the first rendered eye only and reused for the other eye
	pRenderOcclusionTests(mask);
	renderCanvas.SampleDebugInfoPlanPrepareCulling(*this);
	SPECIAL_TIMER_PRINT("RenderOcclusionTests")
	
	// update the blended environment map to use for rendering
	if(deoglSkinShader::REFLECTION_TEST_MODE == deoglSkinShader::ertmSingleBlenderEnvMap){
		pRenderThread.GetRenderers().GetReflection().UpdateEnvMap(*this);
	}
	//PlanEnvMaps(); // doing this here kills the occlusion map causing all kinds of problems
	renderCanvas.SampleDebugInfoPlanPrepareEnvMaps(*this);
	SPECIAL_TIMER_PRINT("UpdateEnvMap")
	
	// update lod for visible elements
	pPlanLODLevels();
	SPECIAL_TIMER_PRINT("PlanLODLevels")
	pUpdateHTViewRTSInstances();
	SPECIAL_TIMER_PRINT("UpdateHTViewRTSInstances")
	
	// prepare particles for rendering
	const deoglParticleEmitterInstanceList &particleEmitterList = pCollideList.GetParticleEmitterList();
	const int particleEmitterCount = particleEmitterList.GetCount();
	int i;
	
	for(i=0; i<particleEmitterCount; i++){
		particleEmitterList.GetAt(i)->PrepareForRender();
	}
	SPECIAL_TIMER_PRINT("PrepareForRenderParticle")
	
	// finish occlusion testing. we can not do this later since this usually removes a large
	// quantity of elements. processing those below just to drop them later on is not helping
	pFinishOcclusionTests(mask);
	if(pRenderThread.GetChoices().GetUseComputeRenderTask()){
		pTasks->BuildComputeRenderTasks(mask);
	}
	SPECIAL_TIMER_PRINT("FinishOcclusionTests")
	
	// update dynamic skins and masked rendering if required
	// NOTE these calls indirectly access projection matrix. this requires per-eye updating
	const int componentCount = pCollideList.GetComponentCount();
	for(i=0; i<componentCount; i++){
		pCollideList.GetComponentAt(i)->GetComponent()->AddSkinStateRenderPlans(*this);
	}
	
	const int billboardCount = pCollideList.GetBillboardCount();
	for(i=0; i<billboardCount; i++){
		deoglRBillboard &billboard = *pCollideList.GetBillboardAt(i);
		//billboard.TestCameraInside( pCameraPosition );
		//renderCanvas.SampleDebugInfoPlanPrepareBillboardsRenderables( *this );
		billboard.AddSkinStateRenderPlans(*this);
	}
	SPECIAL_TIMER_PRINT("Components")
	renderCanvas.SampleDebugInfoPlanPreparePrepareContent(*this);
	
	// now we are ready to produce a render plan
	pBuildRenderPlan();
	renderCanvas.SampleDebugInfoPlanPrepareBuildPlan(*this);
	SPECIAL_TIMER_PRINT("PrepareRenderPlan")
	
	// finish preparations
	for(i=0; i<pSkyLightCount; i++){
		((deoglRenderPlanSkyLight*)pSkyLights.GetAt(i))->FinishPrepare();
	}
	renderCanvas.SampleDebugInfoPlanPrepareFinish(*this);
	SPECIAL_TIMER_PRINT("Finish")
	
	// determine the stencil properties for pass and mask rendering. right now the
	// mask is always 1 bit and the rest is available to the render pass number
	pStencilRefValue = 0;
	pStencilPrevRefValue = 0;
	pStencilWriteMask = 0xe;
	
	// reset the transparency layer stuff just for the case somebody reads it too early
	pClearStencilPassBits = false;
	pClearColor = true;
	pTransparencyLayerCount = 0;
	pCurTransparencyLayer = 0;
	
	renderCanvas.SampleDebugInfoPlanPrepare(*this);
}

void deoglRenderPlan::pBarePrepareRenderRightEye(){
	if(!pWorld){
		return;
	}
	
	deoglRenderCanvas &renderCanvas = pRenderThread.GetRenderers().GetCanvas();
	int i;
	INIT_SPECIAL_TIMING
	
	UpdateRefPosCameraMatrix(); // this is required for rendering but not updating world ref pos
	pPlanCameraProjectionMatrix();
	
	// and masked rendering if required
	const int componentCount = pCollideList.GetComponentCount();
	for(i=0; i<componentCount; i++){
		pCollideList.GetComponentAt(i)->GetComponent()->AddSkinStateRenderPlans(*this);
	}
	
	const int billboardCount = pCollideList.GetBillboardCount();
	for(i=0; i<billboardCount; i++){
		pCollideList.GetBillboardAt(i)->AddSkinStateRenderPlans(*this);
	}
	SPECIAL_TIMER_PRINT("Components")
	renderCanvas.SampleDebugInfoPlanPreparePrepareContent(*this);
	
	// determine the stencil properties for pass and mask rendering. right now the
	// mask is always 1 bit and the rest is available to the render pass number
	pStencilRefValue = 0;
	pStencilPrevRefValue = 0;
	pStencilWriteMask = 0xe;
	
	// reset the transparency layer stuff just for the case somebody reads it too early
	pClearStencilPassBits = false;
	pClearColor = true;
	pTransparencyLayerCount = 0;
	pCurTransparencyLayer = 0;
	
	renderCanvas.SampleDebugInfoPlanPrepare(*this);
}

void deoglRenderPlan::pPlanCamera(){
	if(pCamera){
		pCamera->PrepareForRender();
	}
	
	pWorld->CheckReferencePosition(pCameraPosition);
	UpdateRefPosCameraMatrix();
	
	pPlanCameraProjectionMatrix();
}

void deoglRenderPlan::pPlanCameraProjectionMatrix(){
	// prepare the projection matrix if dirty
	if(pDirtyProjMat){
		const deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
		
		pProjectionMatrix = defren.CreateProjectionDMatrix(pViewportWidth, pViewportHeight,
			pCameraFov, pCameraFovRatio, pCameraImageDistance, pCameraViewDistance);
		pFrustumMatrix = defren.CreateFrustumDMatrix(pViewportWidth, pViewportHeight,
			pCameraFov, pCameraFovRatio, pCameraImageDistance, pCameraViewDistance);
		
		if(pRenderThread.GetChoices().GetUseInverseDepth()){
			pDepthToPosition.x = -pCameraImageDistance;
			pDepthToPosition.y = 0.0f;
			
		}else{
			pDepthToPosition.x = pCameraImageDistance;
			pDepthToPosition.y = 1.0f;
		}
		
		pDepthToPosition.z = 1.0f / (float)pProjectionMatrix.a11;
		pDepthToPosition.w = 1.0f / (float)pProjectionMatrix.a22;
		pDepthToPosition2.Set(0.0f, 0.0f);
		
		// depth sample offset is required to reconstruct depth from nearby depth samples.
		// offset is relative to 1 fragment texel step
		pDepthSampleOffset.x = 2.0f / (float)pViewportWidth;
		pDepthSampleOffset.y = 2.0f / (float)pViewportHeight;
		
		/* non-infinite projection matrix
		const int q = pCameraViewDistance / (pCameraViewDistance - pCameraImageDistance);
		pDepthToPosition.x = q * pCameraImageDistance;
		pDepthToPosition.y = q;
		*/
		
		pProjectionMatrixStereo = pProjectionMatrix;
		pFrustumMatrixStereo = pFrustumMatrix;
		pDepthToPositionStereo = pDepthToPosition;
		pDepthToPositionStereo2 = pDepthToPosition2;
		
		pDirtyProjMat = false;
	}
	
	// VR modifies the matrices
	if(pCamera && pCamera->GetVR() && pRenderVR != ervrNone){
		const deoglVR &vr = *pCamera->GetVR();
		const deoglVREye &vreye = pRenderVR == ervrRightEye ? vr.GetRightEye() : vr.GetLeftEye();
		
		pCameraFov = vr.GetCameraFov();
		pCameraFovRatio = vr.GetCameraFovRatio();
		
		// left eye
		pProjectionMatrix = vreye.CreateProjectionDMatrix(pCameraImageDistance, pCameraViewDistance);
		pFrustumMatrix = vreye.CreateFrustumDMatrix(pCameraImageDistance, pCameraViewDistance);
		
		pDepthToPosition.z = 1.0f / (float)pProjectionMatrix.a11;
		pDepthToPosition.w = 1.0f / (float)pProjectionMatrix.a22;
		pDepthToPosition2.Set((float)-pProjectionMatrix.a13, (float)-pProjectionMatrix.a23);
		
		// right eye
		pProjectionMatrixStereo = vr.GetRightEye().CreateProjectionDMatrix(pCameraImageDistance, pCameraViewDistance);
		pFrustumMatrixStereo = vr.GetRightEye().CreateFrustumDMatrix(pCameraImageDistance, pCameraViewDistance);
		
		pDepthToPositionStereo.z = 1.0f / (float)pProjectionMatrixStereo.a11;
		pDepthToPositionStereo.w = 1.0f / (float)pProjectionMatrixStereo.a22;
		pDepthToPositionStereo2.Set((float)-pProjectionMatrixStereo.a13, (float)-pProjectionMatrixStereo.a23);
	}
	
	// determine frustum to use
	if(pUseCustomFrustum){
		pUseFrustum = &pCustomFrustum;
		
	}else{
		pCameraFrustum.SetFrustum(pCameraMatrix * pFrustumMatrix);
		pUseFrustum = &pCameraFrustum;
	}
}

void deoglRenderPlan::pPlanSky(){
	const int count = pWorld->GetSkyCount();
	int i;
	
	pSkyInstances.RemoveAll();
	pSkyBgColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	
	for(i=0; i<count; i++){
		deoglRSkyInstance * const instance = pWorld->GetSkyAt(i);
		if(!instance->GetRSky()){
			continue;
		}
		if(pUseLayerMask && instance->GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot(instance->GetLayerMask())){
			continue;
		}
		
		if(pSkyInstances.GetCount() == 0){
			pSkyBgColor = instance->GetRSky()->GetBgColor();
		}
		
		pSkyInstances.Add(instance);
	}
}

void deoglRenderPlan::pPlanSkyLight(){
	const int skyCount = GetSkyInstanceCount();
	int i, j, k;
	
	for(i=0; i<pSkyLightCount; i++){
		((deoglRenderPlanSkyLight*)pSkyLights.GetAt(i))->ClearPlanned();
	}
	
	for(i=0; i<skyCount; i++){
		deoglRSkyInstance &instance = *GetSkyInstanceAt(i);
		
		const int layerCount = instance.GetLayerCount();
		for(j=0; j<layerCount; j++){
			deoglRSkyInstanceLayer &skyLayer = instance.GetLayerAt(j);
			if(!skyLayer.GetHasLightDirect() && !skyLayer.GetHasLightAmbient()){
				continue;
			}
			
			deoglRenderPlanSkyLight *planSkyLight = NULL;
			
			for(k=0; k<pSkyLightCount; k++){
				deoglRenderPlanSkyLight * const check = (deoglRenderPlanSkyLight*)pSkyLights.GetAt(k);
				if(check->GetLayer() == &skyLayer){
					planSkyLight = check;
					break;
				}
			}
			
			if(!planSkyLight){
				if(pSkyLightCount < pSkyLights.GetCount()){
					planSkyLight = (deoglRenderPlanSkyLight*)pSkyLights.GetAt(pSkyLightCount);
					
				}else{
					planSkyLight = new deoglRenderPlanSkyLight(*this);
					pSkyLights.Add(planSkyLight);
				}
				
				planSkyLight->SetLayer(&instance, &skyLayer);
				pSkyLightCount++;
			}
			
			planSkyLight->Plan();
		}
	}
	
	for(i=0; i<pSkyLightCount; i++){
		deoglRenderPlanSkyLight * const planSkyLight = (deoglRenderPlanSkyLight*)pSkyLights.GetAt(i);
		if(planSkyLight->GetPlanned()){
			continue;
		}
		
		planSkyLight->Clear();
		pSkyLights.Move(i--, --pSkyLightCount);
	}
}

void deoglRenderPlan::pPlanDominance(){
	//const deoglGraphicContext &gc = pOgl->GetPrimaryGraphicContext();
	
	// determines the dominance of the render. this calcuates the relative space this render
	// occupies compared to the primary render target size. this is used to down-tune if the
	// render is rather small to speed up more important renders
	pDominance = 1.0f; //(float)(pViewportWidth * pViewportHeight) / (float)(gc.GetRenderTargetWidth() * gc.GetRenderTargetHeight());
}

void deoglRenderPlan::pPlanShadowCasting(){
	const deoglConfiguration &config = pRenderThread.GetConfiguration();
	
#if 0
	// largest screen size
	int renderSize = pViewportWidth;
	
	if(pViewportHeight > pViewportWidth){
		renderSize = pViewportHeight;
	}
	
	// calculate the shadow map size to use. the shadow map size is the smalltest power of
	// two value equal or larger than the render size. hence the shadow map size for a
	// 800x600 screen would be 1024. for a large 1650x1050 screen this would be 2048 if the
	// system supports this size. for smaller screens this value drops quickly speeding
	// rendering up. hence a 400x300 screen would require only 512 shadow maps to look good
	// enough. with smaller screens like 200x150 as might be used for small remote views
	// 256 would be selected. reducing the shadow map size by factor 2 already reduces fill
	// rate and per-pixel workload by factor 4.
	
	// TODO replace the shadow map size with a scale factor which indicates the relation
	// between the shadow map size and the render viewport size. the base value is the
	// first value of two size greater or equal to the largest render viewport size. the
	// shadow map is then this base size multiplied by the shadow map factor. this allows
	// to increase or decrease the shadow map size while keeping the rescaling behavior
	// for different render viewport dimensions
	//int unclampedSize = 0;
	const int shadowMapSize = deoglShadowMapper::ShadowMapSize(config);
	int shiftSize = 0;
	
	// disabled since shadow maps are now reused across rendered frames. reducing the size
	// in one render situation is not helping much and is often nullified by the last frame
	// size limitation
	if(pRescaleShadowMaps){
		// TODO review this code. it looks wrong from one end to the other. no idea
		//      what the original idea has been behind this code
		
		if(shadowMapSize > 1024){
			shiftSize = (shadowMapSize / 1024) - 1;
			
		}else if(shadowMapSize < 1024){
			shiftSize = 1 - (1024 / shadowMapSize);
		}
		
		for(pShadowMapSize=16; pShadowMapSize<renderSize; pShadowMapSize<<=1);
		
		if(shiftSize > 0){
			pShadowMapSize <<= shiftSize;
			
		}else if(shiftSize < 0){
			pShadowMapSize >>= -shiftSize;
		}
		//unclampedSize = pShadowMapSize;
		
		pShadowMapSize = decMath::clamp(pShadowMapSize, 16, shadowMapSize);
		pShadowCubeSize = pShadowMapSize;
		pShadowSkySize = pShadowMapSize;
		
		/*
		if(shiftSize > 0){
			pShadowCubeSize = config.GetShadowCubeSize() << shiftSize;
			
		}else if(shiftSize < 0){
			pShadowCubeSize = config.GetShadowCubeSize() >> shiftSize;
			
		}else{
			pShadowCubeSize = config.GetShadowCubeSize();
		}
		*/
		
	}else{
		pShadowMapSize = shadowMapSize;
		pShadowCubeSize = shadowMapSize; //config.GetShadowCubeSize();
		pShadowSkySize = shadowMapSize;
		//unclampedSize = pShadowMapSize;
	}
#endif
	pShadowMapSize = deoglShadowMapper::ShadowMapSize(config);
	pShadowCubeSize = deoglShadowMapper::ShadowCubeSize(config);
	pShadowSkySize = pShadowMapSize;
	
	//printf( "shadow map size: rendersize=%i forced=%i shift=%i size=%i cube=%i sky=%i config=%i\n", renderSize, pForceShadowMapSize, shiftSize, pShadowMapSize, pShadowCubeSize, pShadowSkySize, shadowMapSize );
}

void deoglRenderPlan::pStartFindContent(const deoglRenderPlanMasked *mask){
	INIT_SPECIAL_TIMING
	
	// camera view
	DEASSERT_NULL(pTaskFindContent)
	
	if(pRenderThread.GetChoices().GetUseComputeRenderTask()){
		pCompute->PrepareWorldCompute();
		pCompute->PrepareBuffers();
		
		deoglRenderCompute &renderCompute = pRenderThread.GetRenderers().GetCompute();
		
		if(pWorld->GetCompute().GetUpdateElementCount() > 0){
			renderCompute.UpdateElements(*this);
		}
		
		renderCompute.FindContent(*this);
	}
	
	SetOcclusionMap(pRenderThread.GetTexture().GetOcclusionMapPool().Get(256, 256, pRenderStereo ? 2 : 1)); // 512
	SetOcclusionTest(pRenderThread.GetOcclusionTestPool().Get());
	pOcclusionMapBaseLevel = 0; // logic to choose this comes later
	pOcclusionTest->RemoveAllInputData();
	
	if(!pRenderThread.GetChoices().GetUseComputeRenderTask()){
		pTaskFindContent.TakeOverWith*this);
		pRenderThread.GetOgl().GetGameEngine()->GetParallelProcessing().AddTaskAsync(pTaskFindContent);
	}
	
	// sky lights
	int i;
	for(i=0; i<pSkyLightCount; i++){
		((deoglRenderPlanSkyLight*)pSkyLights.GetAt(i))->StartFindContent();
	}
}

void deoglRenderPlan::pWaitFinishedFindContent(const deoglRenderPlanMasked *mask){
	if(pRenderThread.GetChoices().GetUseComputeRenderTask()){
		pRenderThread.GetRenderers().GetCanvas().SampleDebugInfoPlanPrepareFindContent(*this);
		return;
	}
	
	if(!pTaskFindContent){
		return;
	}
	
// 	pRenderThread.GetLogger().LogInfoFormat( "RenderPlan(%p) WaitFinishedFindContent(%p)", this, pTaskFindContent );
	pTaskFindContent->GetSemaphore().Wait();
	
	pRenderThread.GetRenderers().GetCanvas().SampleDebugInfoPlanPrepareFindContent(
		*this, pTaskFindContent->GetElapsedTime() );
	
	pTaskFindContent = NULL;
}

void deoglRenderPlan::pPlanGI(){
	if(pUseConstGIState || !pUseGIState || pDisableLights
	|| pRenderThread.GetConfiguration().GetGIQuality() == deoglConfiguration::egiqOff){
		return;
	}
	
	if(!pGIState){
		// GI state uses probes of 32x8x32 grid size. this is a default ratio of 4 times as width
		// than high. we use the view distance as the width and thus 1/4 as the height. for a view
		// distance of 500m this would yield a height of 125m. for most games this is enough.
		// the camera parameters like field of view are not used. if probes fall outside the
		// camera the closest GI probe is used. at the far end of the view this is good enough
		const float length = pCameraViewDistance * 2.0f;
		const decVector size(length, length / 4.0f, length);
		
		pGIState = new deoglGIState(pRenderThread, size);
		
		if(pWorld){
			pWorld->AddGICascade(pGIState);
		}
	}
	
	// activate next cascade to use for the upcoming pUpdateGI() call. it is important to do
	// this here and not inside pUpdateGI() since deoglRenderPlanSkyLight needs to know the
	// active cascade used for this frame update. and this happens before pUpdateGI() is called
	pGIState->ActivateNextCascade();
	
	// while doing the first loop of cascade updates force tone mapping adaption on camera
	// to avoid bad starting values due t non-lit geometry
	if(pGIState->CameraForceToneMapAdaption() && pCamera){
		pCamera->SetForceToneMapAdaption(true);
	}
}

void deoglRenderPlan::pUpdateGI(){
	deoglGIState * const giState = GetUpdateGIState();
	if(!giState){
		return;
	}
	
	giState->SetWorld(pWorld);
	giState->SetLayerMask(pUseLayerMask ? pLayerMask : ~decLayerMask());
	giState->Update(pCameraPosition, *pUseFrustum);
	pRenderThread.GetRenderers().GetCanvas().SampleDebugInfoPlanPrepareGIUpdate(*this);
}

void deoglRenderPlan::pPlanLODLevels(){
	if(pHTView){
		pHTView->UpdateLODLevels(pCameraPosition.ToVector());
	}
	
	deoglLODCalculator lodCalculator;
	lodCalculator.SetMaxPixelError(pLodMaxPixelError);
	lodCalculator.SetLodOffset(pLodLevelOffset);
	
	lodCalculator.SetComponentLODProjection(pCollideList, pCameraPosition,
		pCameraInverseMatrix.TransformView(), pCameraFov, pCameraFov * pCameraFovRatio,
		pViewportWidth, pViewportHeight);
}

void deoglRenderPlan::pPlanEnvMaps(){
	const deoglEnvironmentMapList &envmapList = pWorld->GetEnvMapList();
	const int envmapCount = envmapList.GetCount();
	deoglEnvironmentMap *envmap;
	float distance;
	int i, j, k, cap;
	
	// determine the 4 closest environment maps around the camera. the code is for the time
	// being a bit special. first all env maps are moved from indices 0 to 3 to indices 4 to 7.
	// this allows to fill in the 4 closest environment maps in the indices 0 to 3 without
	// affecting the environment maps from the last frame. important is to add the environment
	// maps first in the new location and then remove them from the old one so the usage count
	// does not drop to 0 freeing them although we might reuse them right after. after the 4
	// closest environment maps are found the old environment maps in the indices 4 to 7 are
	// cleared. this way reused environment maps are kept while now unused free their resources
	// if possible
	for(i=0; i<pEnvMapCount; i++){
		pEnvMaps[pEnvMapCount + i].SetEnvMap(pEnvMaps[i].GetEnvMap());
		pEnvMaps[i].SetEnvMap(NULL);
	}
	
	// first find the environment maps containing the camera
	for(i=0, j=0; i<pEnvMapCount && j<envmapCount; j++){
		envmap = envmapList.GetAt(j);
		
		for(k=0; k<i; k++){
			if(pEnvMaps[k].GetEnvMap() == envmap){
				break;
			}
			if(envmap->GetPosition().IsEqualTo(pEnvMaps[k].GetEnvMap()->GetPosition())){ // temporary hack required
				break;
			}
		}
		
		if(k == i && !envmap->GetSkyOnly() && envmap->GetHasInfluenceBox()
		&& envmap->GetInfluenceCollisionBox().IsPointInside(pCameraPosition)){
			pEnvMaps[i].SetEnvMap(envmap);
			pEnvMaps[i].SetDistance(0.0f);
			
			for(k=pEnvMapCount; k<pEnvMapCount*2; k++){
				if(pEnvMaps[k].GetEnvMap() == envmap){
					pEnvMaps[k].SetEnvMap(NULL);
					break;
				}
			}
			if(k == pEnvMapCount*2){
				envmap->GetRenderPlanList().Add(this);
				envmap->AddPlanUsage();
			}
			
			envmap->PrepareForRender();
			i++;
		}
	}
	
	// now fill up with environment maps overlapping the found ones
	while(i < pEnvMapCount){
		cap = i;
		
		for(j=0; i<pEnvMapCount && j<envmapCount; j++){
			envmap = envmapList.GetAt(j);
			
			for(k=0; k<i; k++){
				if(pEnvMaps[k].GetEnvMap() == envmap){
					break;
				}
				if(envmap->GetPosition().IsEqualTo(pEnvMaps[k].GetEnvMap()->GetPosition())){ // temporary hack required
					break;
				}
			}
			
			if(k == i && !envmap->GetSkyOnly() && envmap->GetHasInfluenceBox()){
				for(k=0; k<cap; k++){
					if(envmap->GetInfluenceCollisionBox().BoxHitsBox(
					&pEnvMaps[k].GetEnvMap()->GetInfluenceCollisionBox())){
						break;
					}
				}
				
				if(k < cap){
					pEnvMaps[i].SetEnvMap(envmap);
					pEnvMaps[i].SetDistance(0.0f);
					
					for(k=pEnvMapCount; k<pEnvMapCount*2; k++){
						if(pEnvMaps[k].GetEnvMap() == envmap){
							pEnvMaps[k].SetEnvMap(NULL);
							break;
						}
					}
					if(k == pEnvMapCount*2){
						envmap->GetRenderPlanList().Add(this);
						envmap->AddPlanUsage();
					}
					
					envmap->PrepareForRender();
					i++;
				}
			}
		}
		
		if(cap == i){
			break;
		}
	}
	
	// eventually fill up the remaining slots with environment maps nearby
	for(; i<pEnvMapCount; i++){
		for(j=0; j<envmapCount; j++){
			envmap = envmapList.GetAt(j);
			
			for(k=0; k<i; k++){
				if(envmap->GetPosition().IsEqualTo(pEnvMaps[k].GetEnvMap()->GetPosition())){
					break;
				}
			}
			
			if(k == i && !envmap->GetSkyOnly()){
				distance = (float)((envmap->GetPosition() - pCameraPosition).Length());
				
				if(!pEnvMaps[i].GetEnvMap() || distance < pEnvMaps[i].GetDistance()){
					pEnvMaps[i].SetEnvMap(envmap);
					pEnvMaps[i].SetDistance(distance);
				}
			}
		}
		
		envmap = pEnvMaps[i].GetEnvMap();
		if(envmap){
			for(j=pEnvMapCount; j<pEnvMapCount*2; j++){
				if(pEnvMaps[j].GetEnvMap() == envmap){
					pEnvMaps[j].SetEnvMap(NULL);
					break;
				}
			}
			if(j == pEnvMapCount*2){
				envmap->GetRenderPlanList().Add(this);
				envmap->AddPlanUsage();
			}
			
			envmap->PrepareForRender();
			
		}else{
			break;
		}
	}
	
	// if no environment maps have been found add positionless environment maps
	if(i == 0){
		for(j=0; i<pEnvMapCount && j<envmapCount; j++){
			envmap = envmapList.GetAt(j);
			
			if(envmap->GetSkyOnly()){ // hack, requires a GetIsPositionless function
				pEnvMaps[i].SetEnvMap(envmap);
				pEnvMaps[i].SetDistance(0.0f);
				
				for(k=pEnvMapCount; k<pEnvMapCount*2; k++){
					if(pEnvMaps[k].GetEnvMap() == envmap){
						pEnvMaps[k].SetEnvMap(NULL);
						break;
					}
				}
				if(k == pEnvMapCount*2){
					envmap->GetRenderPlanList().Add(this);
					envmap->AddPlanUsage();
				}
				
				envmap->PrepareForRender();
				i++;
			}
		}
	}
	
	// sort the found environment maps by priority
	for(j=1; j<i; j++){
		if(pEnvMaps[j].GetEnvMap()->GetInfluencePriority() <=
		pEnvMaps[j - 1].GetEnvMap()->GetInfluencePriority()){
			continue;
		}
		
		envmap = pEnvMaps[j - 1].GetEnvMap();
		distance = pEnvMaps[j - 1].GetDistance();
		
		pEnvMaps[j - 1].SetEnvMap(pEnvMaps[j].GetEnvMap());
		pEnvMaps[j - 1].SetDistance(pEnvMaps[j].GetDistance());
		
		pEnvMaps[j].SetEnvMap(envmap);
		pEnvMaps[j].SetDistance(distance);
		
		if(j > 1){
			j -= 2;
		}
	}
	
	// for the direct environment map fader switch to the first environment map we found
	if(pEnvMapCount == 0){
		pDirectEnvMapFader.FadeTo(NULL);
		
	}else{
		pDirectEnvMapFader.FadeTo(pEnvMaps[0].GetEnvMap());
	}
	
	// clean up
	for(i=pEnvMapCount; i<pEnvMapCount*2; i++){
		if(pEnvMaps[i].GetEnvMap()){
			pDirectEnvMapFader.Drop(pEnvMaps[i].GetEnvMap());
			
			pEnvMaps[i].GetEnvMap()->RemovePlanUsage();
			pEnvMaps[i].GetEnvMap()->GetRenderPlanList().Remove(this);
			pEnvMaps[i].SetEnvMap(NULL);
		}
	}
	
	/*
	printf("envmap count %i\n", pEnvMapCount);
	for(i=0; i<pEnvMapCount; i++){
		if(pEnvMaps[i].GetEnvMap()){
			printf("envmap %i priority %i position (%g,%g,%g)\n", i, pEnvMaps[i].GetEnvMap()->GetInfluencePriority(), pEnvMaps[i].GetEnvMap()->GetPosition().x, pEnvMaps[i].GetEnvMap()->GetPosition().y, pEnvMaps[i].GetEnvMap()->GetPosition().z);
		}else{
			printf("envmap %i NULL\n", i);
		}
	}
	*/
	
	// update hard limits
	/*
	for(i=0; i<envmapCount; i++){
		envmapList.GetAt(i)->PrepareCubeMapHardLimit();
	}
	*/
	
	// update soft limits
	/*
	for(i=0; i<envmapCount; i++){
		envmapList.GetAt(i)->PrepareCubeMapSoftLimit();
	}
	*/
	
	// update env map if required
	if(pWorld->GetEnvMapUpdateCount() > 0){
		deoglEnvironmentMap *updateEnvmap = NULL;
		
		// check if one of the 4 closest environment maps is in need of updating. if so choose this one
		for(i=0; i<pEnvMapCount; i++){
			if(pEnvMaps[i].GetEnvMap() && pEnvMaps[i].GetEnvMap()->GetDirty()){
				updateEnvmap = pEnvMaps[i].GetEnvMap();
				break;
			}
		}
		
		// if no environment map has been chosen to update check if the sky environment map needs updating. if so choose this one
		if(!updateEnvmap && pWorld->GetSkyEnvironmentMap()->GetDirty()){
			updateEnvmap = pWorld->GetSkyEnvironmentMap();
		}
		
		// if the sky environment map is not updated yet do this first
		/*
		if(pWorld->GetSky() && pWorld->GetSky()->GetEnvironmentMap()->GetDirty()){
			pWorld->GetSky()->GetEnvironmentMap()->Update();
			
			// rendering the environment map potentially alters the reference position. ensure the reference
			// position is back to a proper value and that the reference position camera matrix is correct
			pWorld->CheckReferencePosition(pCameraPosition);
			UpdateRefPosCameraMatrix();
			
			return;
		}
		*/
		
#if 0
		double envmapDistSquared, updateEnvmapDistSquared = 0.0;
		deoglEnvironmentMap *updateEnvmap = NULL;
		decDVector positionDifference;
		
		// unmark all environment maps
		for(i=0; i<envmapCount; i++){
			envmapList.GetAt(i)->SetMarked(false);
		}
		
		// find the next environment map to update. for the time being we choose the environment
		// map closest to the camera position that is dirty.
		
		// first check all environment maps assigned to visible components
#if 0
		count = pCollideList.GetComponentCount();
		for(i=0; i<count; i++){
			envmap = pCollideList.GetComponentAt(i)->GetComponent()->GetRenderEnvMap();
			
			if(envmap && !envmap->GetMarked()){
				envmap->SetMarked(true);
				
				if(envmap->GetDirty()){
					positionDifference = envmap->GetPosition() - pCameraPosition;
					envmapDistSquared = positionDifference * positionDifference;
					
					if(!updateEnvmap || envmapDistSquared < updateEnvmapDistSquared){
						updateEnvmap = envmap;
						updateEnvmapDistSquared = envmapDistSquared;
					}
				}
			}
		}
#endif
		
		// if no environment map has been found yet look in the entire list of environment maps
		// for a match. this includes environment maps that are not yet visible but potentially
		// are in the next frame
		for(i=0; i<envmapCount; i++){
			envmap = envmapList.GetAt(i);
			
			if(!envmap->GetMarked()){
				envmap->SetMarked(true);
				
				if(envmap->GetDirty()){
					positionDifference = envmap->GetPosition() - pCameraPosition;
					envmapDistSquared = positionDifference * positionDifference;
					
					if(!updateEnvmap || envmapDistSquared < updateEnvmapDistSquared){
						updateEnvmap = envmap;
						updateEnvmapDistSquared = envmapDistSquared;
					}
				}
			}
		}
#endif
		
		// if we found an environment map update it
		if(updateEnvmap){
			const deoglEnvironmentMap::Ref guard(updateEnvmap);
			updateEnvmap->Update(*this);
			
			// rendering the environment map potentially alters the reference position. ensure the reference
			// position is back to a proper value and that the reference position camera matrix is correct
			pWorld->CheckReferencePosition(pCameraPosition);
			UpdateRefPosCameraMatrix();
		}
	}
}

void deoglRenderPlan::pRenderOcclusionTests(const deoglRenderPlanMasked *mask){
	INIT_SPECIAL_TIMING
	pWaitFinishedFindContent(mask);
	SPECIAL_TIMER_PRINT("> WaitFinishFindContent")
	
	// debug information if demanded
	pDebugVisibleNoCull();
	
	if(pRenderThread.GetConfiguration().GetDebugNoCulling()){
		pSkyVisible = true;
		return;
	}
	
	// determine if height terrain and sky are visible
	pCheckOutsideVisibility();
	
	// render occlusion tests if there is input data. results are read back in
	// pFinishOcclusionTests to avoid stalling
	if(pRenderThread.GetChoices().GetUseComputeRenderTask()){
		pCompute->ReadyRTOcclusion(mask);
		SPECIAL_TIMER_PRINT("> ReadyRTOcclusion")
		
		pRenderThread.GetRenderers().GetOcclusion().RenderTestsCamera(*this, mask);
		SPECIAL_TIMER_PRINT("> RenderTestsCamera")
		
		int i;
		for(i=0; i<pSkyLightCount; i++){
			((deoglRenderPlanSkyLight*)pSkyLights.GetAt(i))->RenderOcclusionTests();
		}
		SPECIAL_TIMER_PRINT("> SkyLightsRenderTests")
		
		for(i=0; i<pSkyLightCount; i++){
			((deoglRenderPlanSkyLight*)pSkyLights.GetAt(i))->BuildComputeRenderTasks();
		}
		SPECIAL_TIMER_PRINT("> SkyLightsBuildComputeRenderTasks")
		
		pCompute->ReadVisibleElements();
		SPECIAL_TIMER_PRINT("> ReadVisibleElements")
		
		pRenderThread.GetRenderers().GetOcclusion().RenderOcclusionQueries(*this, mask, true);
		SPECIAL_TIMER_PRINT("> RenderOcclusionQueries")
		
	}else if(pOcclusionTest->GetInputDataCount() > 0){
		pOcclusionTest->UpdateSSBO();
		if(pDebug){
			pDebug->IncrementOccTestCount(pOcclusionTest->GetInputDataCount());
		}
		SPECIAL_TIMER_PRINT("> UpdateVBO")
		
		pRenderThread.GetRenderers().GetOcclusion().RenderTestsCamera(*this, mask);
		SPECIAL_TIMER_PRINT("> Render")
		
		pRenderThread.GetRenderers().GetOcclusion().RenderOcclusionQueries(*this, mask, true);
		SPECIAL_TIMER_PRINT("> RenderOcclusionQueries")
	}
}

void deoglRenderPlan::pFinishOcclusionTests(const deoglRenderPlanMasked *mask){
	if(pRenderThread.GetConfiguration().GetDebugNoCulling()){
		pTasks->StartBuildTasks(mask);
		return;
	}
	INIT_SPECIAL_TIMING
	
	// occlusion tests have been rendered in pRenderOcclusionTests to avoid stalling
	// NOTE if compute render tasks are used this will be always 0 and skipped
	if(pOcclusionTest->GetInputDataCount() > 0){
		pOcclusionTest->UpdateResults();
		SPECIAL_TIMER_PRINT("> UpdateResults")
		
		if(GetUpdateGIState()){
			// we can not use RemoveCulledElements in this situation since lights outside
			// the frustum have to be kept if inside GI cascade detection box. the light
			// renderers will take care of not rendering a light for camera view content
			// if the light is culled
			pCollideList.RemoveCulledComponents();
			pCollideList.RemoveCulledBillboards();
			
		}else{
			pCollideList.RemoveCulledElements();
		}
		SPECIAL_TIMER_PRINT("> RemoveCulledElements")
	}
	
	pDebugVisibleCulled();
	
	pTasks->StartBuildTasks(mask);
}

void deoglRenderPlan::pDebugPrepare(){
	if(pRenderThread.GetDebug().GetDeveloperMode().GetDebugRenderPlan()){
		if(pDebug){
			pDebug->Reset();
			
		}else{
			pDebug = new deoglRenderPlanDebug;
		}
		
	}else{
		if(pDebug){
			delete pDebug;
			pDebug = NULL;
		}
	}
}

void deoglRenderPlan::pDebugVisibleNoCull(){
	/*
	if(pUseLayerMask){
		const int componentCount = pCollideList.GetComponentCount();
		int i, triangleCount = 0;
		for(i=0; i<componentCount; i++){
			const deoglCollideListComponent &clistComponent = *pCollideList.GetComponentAt(i);
			const deoglRComponent &component = *clistComponent.GetComponent();
			if(!component.GetModel()){
				continue;
			}
			
			const deoglModelLOD &modelLOD = *component.GetModel()->GetLODAt(clistComponent.GetLODLevel());
			triangleCount += modelLOD.GetFaceCount();
		}
		pRenderThread.GetLogger().LogInfoFormat("colliderlist no occlusion test;%i;%i;%i\n", pCollideList.GetComponentCount(), pCollideList.GetLightCount(), triangleCount);
	}
	*/
	if(pDebug){
		const int componentCount = pCollideList.GetComponentCount();
		const int lightCount = pCollideList.GetLightCount();
		deoglEnvironmentMapList envMapList;
		int i;
		
		pDebug->IncrementViewObjects(componentCount);
		pDebug->IncrementViewLights(lightCount);
		
		for(i=0; i<componentCount; i++){
			const deoglCollideListComponent &clistComponent = *pCollideList.GetComponentAt(i);
			const deoglRComponent &component = *clistComponent.GetComponent();
			
			if(component.GetModel()){
				const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt(clistComponent.GetLODLevel());
				pDebug->IncrementViewTriangles(modelLOD.GetFaceCount());
			}
			/*
			if(component.GetRenderEnvMap()){
				envMapList.AddIfMissing(component.GetRenderEnvMap());
			}
			if(component.GetRenderEnvMapFade()){
				envMapList.AddIfMissing(component.GetRenderEnvMapFade());
			}
			*/
		}
		
		pDebug->IncrementViewEnvMaps(envMapList.GetCount());
	}
}

void deoglRenderPlan::pDebugVisibleCulled(){
	/*
	if(pUseLayerMask){
		const int componentCount = pCollideList.GetComponentCount();
		int i, triangleCount = 0;
		for(i=0; i<componentCount; i++){
			const deoglCollideListComponent &clistComponent = *pCollideList.GetComponentAt(i);
			const deoglRComponent &component = *clistComponent.GetComponent();
			if(!component.GetModel()){
				continue;
			}
			
			const deoglModelLOD &modelLOD = *component.GetModel()->GetLODAt(clistComponent.GetLODLevel());
			triangleCount += modelLOD.GetFaceCount();
		}
		pRenderThread.GetLogger().LogInfoFormat("colliderlist occlusion tested;%i;%i;%i\n", pCollideList.GetComponentCount(), pCollideList.GetLightCount(), triangleCount);
	}
	*/
	if(pDebug){
		const int componentCount = pCollideList.GetComponentCount();
		const int lightCount = pCollideList.GetLightCount();
		int i;
		
		pDebug->IncrementCullPSObjects(componentCount);
		pDebug->IncrementCullPSLights(lightCount);
		
		for(i=0; i<componentCount; i++){
			const deoglCollideListComponent &clistComponent = *pCollideList.GetComponentAt(i);
			const deoglRComponent &component = *clistComponent.GetComponent();
			
			if(!component.GetModel()){
				continue;
			}
			
			const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt(clistComponent.GetLODLevel());
			pDebug->IncrementCullPSTriangles(modelLOD.GetFaceCount());
		}
	}
}

void deoglRenderPlan::PrepareRenderOneTurn(){
	/*
	deWorld *world = pWorld->GetWorld();
	int p, propFieldCount = world->GetPropFieldCount();
	deoglPropField *oglPropField;
	
	// prop fields can be prepared for one render turn
	for(p=0; p<propFieldCount; p++){
		oglPropField = (deoglPropField*)world->GetPropFieldAt(p)->GetPeerGraphic();
		
		if(oglPropField){
			oglPropField->PrepareInstances(pCameraPosition, decDMatrix(pCameraMatrix));
		}
	}
	*/
}

void deoglRenderPlan::PlanTransparency(int layerCount){
	pTransparencyLayerCount = layerCount;
	pCurTransparencyLayer = 0;
	
	if(!pHasTransparency){
		// set light shadow parameters to default
		return;
	}
	
	//printf( "RenderPlan: Transparency Layer Count = %d\n", layerCount );
	
	// plan transparency handling for lights
// 	if( pDisableLights ){ // what?
// 		int i;
// 		for( i=0; i<pLightCount; i++ ){
// 			deoglRenderPlanLight &planLight = *pLights[ i ];
// 		}
// 	}
}

void deoglRenderPlan::Render(){
	const deoglDebugTraceGroup debugTrace(pRenderThread, "Plan.Render");
	if(pIsRendering){
		// re-entrant rendering causes exceptions. render instead a black screen and do not clean up
		pRenderThread.GetRenderers().GetWorld().RenderBlackScreen(*this);
		return;
	}
	
	// to make sure we clean up everyting even after an exception try this all
	try{
		pRenderThread.GetRenderers().GetWorld().RenderWorld(*this, NULL);
		
	}catch(const deException &e){
		e.PrintError();
		// add to exception trace
		if(pRenderVR != ervrLeftEye){
			CleanUp();
		}
		throw;
	}
	
	if(pRenderVR != ervrLeftEye){
		CleanUp();
	}
}

void deoglRenderPlan::CleanUp(){
	int i;
	for(i=0; i<pSkyLightCount; i++){
		((deoglRenderPlanSkyLight*)pSkyLights.GetAt(i))->CleanUp();
	}
	
	if(pTasks){
		pTasks->CleanUp();
	}
	if(!pRenderThread.GetChoices().GetUseComputeRenderTask()){
		pWaitFinishedFindContent(nullptr);
	}
	
	RemoveAllSkyInstances();
	RemoveAllMaskedPlans();
	RemoveAllLights();
	pDropLightsTemporary();
	pCollideList.Clear();
	pComponentsOccMap.RemoveAll();
	SetOcclusionTest(NULL);
	SetOcclusionMap(NULL);
}



void deoglRenderPlan::SetCamera(deoglRCamera *camera){
	pCamera = camera;
	
	if(camera){
		pCameraPosition = camera->GetPosition();
		
	}else{
		pCameraPosition.SetZero();
	}
}

void deoglRenderPlan::SetCameraMatrix(const decDMatrix &matrix){
	pCameraMatrix = matrix;
	pCameraInverseMatrix = matrix.QuickInvert();
	pCameraMatrixNonMirrored = matrix;
	
	// NOTE has to be this way. the camera can be the same but the matrix can be
	// something else for example in a mirror situation. the position from the
	// inverse matrix is correct. still though an additional parameter containing
	// the position specifically would be better and less error prone.
	
//	if( pCamera ){
//		pCameraPosition = pCamera->GetCamera()->GetPosition();
//		
//	}else{
		pCameraPosition = pCameraInverseMatrix.GetPosition();
//	}
	
	pCameraStereoMatrix.SetIdentity();
	pCameraStereoInverseMatrix.SetIdentity();
}

void deoglRenderPlan::SetCameraMatrixNonMirrored(const decDMatrix &matrix){
	pCameraMatrixNonMirrored = matrix;
}

void deoglRenderPlan::SetCameraStereoMatrix(const decMatrix &matrix){
	pCameraStereoMatrix = matrix;
	pCameraStereoInverseMatrix = matrix.QuickInvert();
}

void deoglRenderPlan::SetCameraParameters(float fov, float fovRatio, float imageDistance, float viewDistance){
	if(fov <= 0.0f || fov >= PI || fovRatio <= 0.0f || imageDistance <= 0.0f || imageDistance >= viewDistance){
		DETHROW(deeInvalidParam);
	}
	pCameraFov = fov;
	pCameraFovRatio = fovRatio;
	pCameraImageDistance = imageDistance;
	pCameraViewDistance = viewDistance;
	
	pDirtyProjMat = true;
}

void deoglRenderPlan::SetCameraAdaptedIntensity(float intensity){
	pCameraAdaptedIntensity = intensity;
}

void deoglRenderPlan::SetViewport(int width, int height){
	if(width < 1 || height < 1) DETHROW(deeInvalidParam);
	
	pViewportWidth = width;
	pViewportHeight = height;
	pAspectRatio = (float)pViewportWidth / (float)pViewportHeight;
	
	pDirtyProjMat = true;
}

void deoglRenderPlan::CopyCameraParametersFrom(const deoglRenderPlan &plan){
	pViewportWidth = plan.pViewportWidth;
	pViewportHeight = plan.pViewportHeight;
	pAspectRatio = plan.pAspectRatio;
	pCameraFov = plan.pCameraFov;
	pCameraFovRatio = plan.pCameraFovRatio;
	pCameraImageDistance = plan.pCameraImageDistance;
	pCameraViewDistance = plan.pCameraViewDistance;
	pCameraAdaptedIntensity = plan.pCameraAdaptedIntensity;
	
	pProjectionMatrix = plan.pProjectionMatrix;
	pProjectionMatrixStereo = plan.pProjectionMatrixStereo;
	pFrustumMatrix = plan.pFrustumMatrix;
	pFrustumMatrixStereo = plan.pFrustumMatrixStereo;
	pDepthToPosition = plan.pDepthToPosition;
	pDepthToPosition2 = plan.pDepthToPosition2;
	pDepthToPositionStereo = plan.pDepthToPositionStereo;
	pDepthToPositionStereo2 = plan.pDepthToPositionStereo2;
	pDepthSampleOffset = plan.pDepthSampleOffset;
	
	pDirtyProjMat = false;
}

void deoglRenderPlan::UpdateRefPosCameraMatrix(){
	// -( campos - refpos )
	const decVector refPosCam(pWorld->GetReferencePosition() - pCameraPosition);
	
	pRefPosCameraMatrix.a11 = (float)pCameraMatrix.a11;
	pRefPosCameraMatrix.a12 = (float)pCameraMatrix.a12;
	pRefPosCameraMatrix.a13 = (float)pCameraMatrix.a13;
	pRefPosCameraMatrix.a14 = refPosCam.x * pRefPosCameraMatrix.a11
		+ refPosCam.y * pRefPosCameraMatrix.a12 + refPosCam.z * pRefPosCameraMatrix.a13;
	
	pRefPosCameraMatrix.a21 = (float)pCameraMatrix.a21;
	pRefPosCameraMatrix.a22 = (float)pCameraMatrix.a22;
	pRefPosCameraMatrix.a23 = (float)pCameraMatrix.a23;
	pRefPosCameraMatrix.a24 = refPosCam.x * pRefPosCameraMatrix.a21
		+ refPosCam.y * pRefPosCameraMatrix.a22 + refPosCam.z * pRefPosCameraMatrix.a23;
	
	pRefPosCameraMatrix.a31 = (float)pCameraMatrix.a31;
	pRefPosCameraMatrix.a32 = (float)pCameraMatrix.a32;
	pRefPosCameraMatrix.a33 = (float)pCameraMatrix.a33;
	pRefPosCameraMatrix.a34 = refPosCam.x * pRefPosCameraMatrix.a31
		+ refPosCam.y * pRefPosCameraMatrix.a32 + refPosCam.z * pRefPosCameraMatrix.a33;
	
	// mirror free
	
	pRefPosCameraMatrixNonMirrored.a11 = (float)pCameraMatrixNonMirrored.a11;
	pRefPosCameraMatrixNonMirrored.a12 = (float)pCameraMatrixNonMirrored.a12;
	pRefPosCameraMatrixNonMirrored.a13 = (float)pCameraMatrixNonMirrored.a13;
	pRefPosCameraMatrixNonMirrored.a14 = refPosCam.x * pRefPosCameraMatrixNonMirrored.a11
		+ refPosCam.y * pRefPosCameraMatrixNonMirrored.a12
		+ refPosCam.z * pRefPosCameraMatrixNonMirrored.a13;
	
	pRefPosCameraMatrixNonMirrored.a21 = (float)pCameraMatrixNonMirrored.a21;
	pRefPosCameraMatrixNonMirrored.a22 = (float)pCameraMatrixNonMirrored.a22;
	pRefPosCameraMatrixNonMirrored.a23 = (float)pCameraMatrixNonMirrored.a23;
	pRefPosCameraMatrixNonMirrored.a24 = refPosCam.x * pRefPosCameraMatrixNonMirrored.a21
		+ refPosCam.y * pRefPosCameraMatrixNonMirrored.a22
		+ refPosCam.z * pRefPosCameraMatrixNonMirrored.a23;
	
	pRefPosCameraMatrixNonMirrored.a31 = (float)pCameraMatrixNonMirrored.a31;
	pRefPosCameraMatrixNonMirrored.a32 = (float)pCameraMatrixNonMirrored.a32;
	pRefPosCameraMatrixNonMirrored.a33 = (float)pCameraMatrixNonMirrored.a33;
	pRefPosCameraMatrixNonMirrored.a34 = refPosCam.x * pRefPosCameraMatrixNonMirrored.a31
		+ refPosCam.y * pRefPosCameraMatrixNonMirrored.a32
		+ refPosCam.z * pRefPosCameraMatrixNonMirrored.a33;
}



void deoglRenderPlan::SetFBOTarget(deoglFramebuffer *fbo){
	pFBOTarget = fbo;
}

void deoglRenderPlan::SetFBOMaterial(deoglFramebuffer *fbo){
	pFBOMaterial = fbo;
}

void deoglRenderPlan::SetFBOMaterialMatrix(const decMatrix &matrix){
	pFBOMaterialMatrix = matrix;
}



void deoglRenderPlan::SetUpscaleSize(int width, int height){
	if(width < 0 || height < 0) DETHROW(deeInvalidParam);
	
	pUpscaleWidth = width;
	pUpscaleHeight = height;
}

void deoglRenderPlan::SetUseUpscaling(bool useUpscaling){
	pUseUpscaling = useUpscaling;
}

void deoglRenderPlan::SetUpsideDown(bool upsideDown){
	pUpsideDown = upsideDown;
}

void deoglRenderPlan::SetUseToneMap(bool useToneMap){
	pUseToneMap = useToneMap;
}

void deoglRenderPlan::SetIgnoreDynamicComponents(bool ignoreStaticComponents){
	pIgnoreDynamicComponents = ignoreStaticComponents;
}

void deoglRenderPlan::SetRenderDebugPass(bool render){
	pRenderDebugPass = render;
}

void deoglRenderPlan::SetNoReflections(bool noReflections){
	pNoReflections = noReflections;
}

void deoglRenderPlan::SetNoAmbientLight(bool noAmbientLight){
	pNoAmbientLight = noAmbientLight;
}

void deoglRenderPlan::SetUseGIState(bool useGIState){
	pUseGIState = useGIState;
}

void deoglRenderPlan::SetUseConstGIState(deoglGIState *giState){
	pUseConstGIState = giState;
}

void deoglRenderPlan::SetRenderStereo (bool stereoRender){
	pRenderStereo = stereoRender;
}

void deoglRenderPlan::SetRenderVR(eRenderVR renderVR){
	pRenderVR = renderVR;
}



void deoglRenderPlan::SetUseLayerMask(bool useLayerMask){
	pUseLayerMask = useLayerMask;
}

void deoglRenderPlan::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
}



void deoglRenderPlan::SetCustomFrustumBoundaries(
const decDVector &position, const decDVector &topLeft, const decDVector &topRight,
const decDVector &bottomLeft, const decDVector &bottomRight, double near, double far){
	decDVector normal;
	
	// left plane
	normal = (topLeft - bottomLeft) % (bottomLeft - position);
	normal.Normalize();
	pCustomFrustum.SetLeftPlane(normal, -(normal * position));
	
	// top plane
	normal = (topRight - topLeft) % (topLeft - position);
	normal.Normalize();
	pCustomFrustum.SetTopPlane(normal, -(normal * position));
	
	// right plane
	normal = (bottomRight - topRight) % (topRight - position);
	normal.Normalize();
	pCustomFrustum.SetRightPlane(normal, -(normal * position));
	
	// bottom plane
	normal = (bottomLeft - bottomRight) % (bottomRight - position);
	normal.Normalize();
	pCustomFrustum.SetBottomPlane(normal, -(normal * position));
	
	// near and far plane
	//normal = ( topLeft + topRight + bottomLeft + bottomRight ) * 0.25 - position;
	normal = (topLeft + bottomRight) * 0.5 - position; // if this is a real box the center of each diagonal is the same point
	normal.Normalize();
	pCustomFrustum.SetNearPlane(normal, -(normal * position + near)); // n*(p+n*near) = n*p+(n*n)*near = n*p+near
	pCustomFrustum.SetFarPlane(-normal, normal * position + far); // -n*(p+n*far) = -n*p+(-n*n)*far = -n*p-far = -(n*p+far)
}

void deoglRenderPlan::SetUseCustomFrustum(bool useCustomFrustum){
	pUseCustomFrustum = useCustomFrustum;
}

void deoglRenderPlan::SetCustomFrustumScreenArea(const decBoundary &area){
	pCustomFrustumScreenArea = area;
}



void deoglRenderPlan::SetNoRenderedOccMesh(bool noRenderedOccMesh){
	pNoRenderedOccMesh = noRenderedOccMesh;
}

void deoglRenderPlan::SetFlipCulling(bool flipCulling){
	pFlipCulling = flipCulling;
}



void deoglRenderPlan::SetEmptyPass(bool emptyPass){
	pEmptyPass = emptyPass;
}

void deoglRenderPlan::SetClearStencilPassBits(bool clear){
	pClearStencilPassBits = clear;
}

void deoglRenderPlan::SetClearColor(bool clear){
	pClearColor = clear;
}



void deoglRenderPlan::SetCurrentTransparencyLayer(int layer){
	pCurTransparencyLayer = layer;
}

void deoglRenderPlan::SetTransparencyLayerCount(int count){
	pTransparencyLayerCount = count;
}

void deoglRenderPlan::SetRenderPassNumber(int number){
	pRenderPassNumber = number;
}

void deoglRenderPlan::SetStencilRefValue(int refValue){
	pStencilRefValue = refValue;
}

void deoglRenderPlan::SetStencilPrevRefValue(int refValue){
	pStencilPrevRefValue = refValue;
}

void deoglRenderPlan::SetStencilWriteMask(int writeMask){
	pStencilWriteMask = writeMask;
}

void deoglRenderPlan::SetLodMaxPixelError(int error){
	pLodMaxPixelError = decMath::max(error, 0);
}

void deoglRenderPlan::SetLodLevelOffset(int offset){
	pLodLevelOffset = offset;
}

void deoglRenderPlan::SetOcclusionMap(deoglOcclusionMap *occlusionMap){
	if(pOcclusionMap == occlusionMap){
		return;
	}
	
	if(pOcclusionMap){
		pRenderThread.GetTexture().GetOcclusionMapPool().Return(pOcclusionMap);
	}
	
	pOcclusionMap = occlusionMap;
}

void deoglRenderPlan::SetOcclusionTest(deoglOcclusionTest *occlusionTest){
	if(pOcclusionTest == occlusionTest){
		return;
	}
	
	if(pOcclusionTest){
		pRenderThread.GetOcclusionTestPool().Return(pOcclusionTest);
	}
	
	pOcclusionTest = occlusionTest;
}

void deoglRenderPlan::SetOcclusionMapBaseLevel(int level){
	pOcclusionMapBaseLevel = level;
}

void deoglRenderPlan::SetOcclusionTestMatrix(const decMatrix &matrix){
	pOcclusionTestMatrix = matrix;
}

void deoglRenderPlan::SetOcclusionTestMatrixStereo(const decMatrix &matrix){
	pOcclusionTestMatrixStereo = matrix;
}



deoglGIState *deoglRenderPlan::GetUpdateGIState() const{
	if(pUseGIState && !pUseConstGIState && !pDisableLights
	&& pRenderThread.GetConfiguration().GetGIQuality() != deoglConfiguration::egiqOff
	&& pRenderVR != ervrRightEye){
		return pGIState;
	}
	return NULL;
}

deoglGIState *deoglRenderPlan::GetRenderGIState() const{
	if(!pUseGIState || pDisableLights
	|| pRenderThread.GetConfiguration().GetGIQuality() == deoglConfiguration::egiqOff){
		return NULL;
		
	}else if(pUseConstGIState){
		return pUseConstGIState;
		
	}else{
		return pGIState;
	}
}

void deoglRenderPlan::DropGIState(){
	// WARNING called from main thread during synchronization
	
	if(pGIState){
		delete pGIState;
	}
}



void deoglRenderPlan::SetDebugTiming(bool debugTiming){
	pDebugTiming = debugTiming;
}



// Environment Maps
/////////////////////

deoglRenderPlanEnvMap &deoglRenderPlan::GetEnvMapAt(int index) const{
	if(index < 0 || index >= pEnvMapCount){
		DETHROW(deeInvalidParam);
	}
	
	return pEnvMaps[index];
}

void deoglRenderPlan::RemoveEnvMap(deoglEnvironmentMap *envmap){
	if(!envmap){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	pDirectEnvMapFader.Drop(envmap);
	
	for(i=0; i<pEnvMapCount; i++){
		if(pEnvMaps[i].GetEnvMap() == envmap){
			pEnvMaps[i].SetEnvMap(NULL);
		}
	}
}



// Lights
///////////

deoglRenderPlanLight *deoglRenderPlan::GetLightAt(int index) const{
	if(index < 0 || index >= pLightCount) DETHROW(deeInvalidParam);
	
	return pLights[index];
}

deoglRenderPlanLight *deoglRenderPlan::GetLightFor(deoglCollideListLight *light){
	if(!light){
		DETHROW(deeInvalidParam);
	}
	
	int index = pIndexOfLightWith(light);
	if(index == -1){
		if(pLightCount == pLightSize){
			const int newSize = pLightSize * 3 / 2 + 1;
			deoglRenderPlanLight ** const newArray = new deoglRenderPlanLight*[newSize];
			
			memset(newArray, 0, sizeof(deoglRenderPlanLight*) * newSize);
			if(pLights){
				memcpy(newArray, pLights, sizeof(deoglRenderPlanLight*) * pLightSize);
				delete [] pLights;
			}
			
			pLights = newArray;
			pLightSize = newSize;
		}
		
		if(!pLights[pLightCount]){
			pLights[pLightCount] = new deoglRenderPlanLight(*this);
		}
		
		index = pLightCount;
		pLights[pLightCount]->SetLight(light);
		pLightCount++;
	}
	
	return pLights[index];
}

void deoglRenderPlan::RemoveAllLights(){
	while(pLightCount > 0){
		pLightCount--;
		pLights[pLightCount]->SetLight(NULL);
	}
}



// Sky lights
///////////////

deoglRenderPlanSkyLight *deoglRenderPlan::GetSkyLightAt(int index) const{
	return (deoglRenderPlanSkyLight*)pSkyLights.GetAt(index);
}

void deoglRenderPlan::RemoveAllSkyLights(){
	while(pSkyLightCount > 0){
		((deoglRenderPlanSkyLight*)pSkyLights.GetAt(--pSkyLightCount))->Clear();
	}
}

void deoglRenderPlan::SkyLightsStartBuildRT(){
	int i;
	for(i=0; i<pSkyLightCount; i++){
		((deoglRenderPlanSkyLight*)pSkyLights.GetAt(i))->StartBuildRT();
	}
}



// Sky instances
//////////////////

int deoglRenderPlan::GetSkyInstanceCount() const{
	return pSkyInstances.GetCount();
}

deoglRSkyInstance *deoglRenderPlan::GetSkyInstanceAt(int index) const{
	return (deoglRSkyInstance*)pSkyInstances.GetAt(index);
}

void deoglRenderPlan::RemoveAllSkyInstances(){
	pSkyInstances.RemoveAll();
}

void deoglRenderPlan::SetSkyBgColor(const decColor& color){
	pSkyBgColor = color;
}



// Sub Plans
//////////////

deoglRenderPlanMasked *deoglRenderPlan::GetMaskedPlanAt(int index) const{
	if(index < 0 || index >= pMaskedPlanCount) DETHROW(deeInvalidParam);
	
	return pMaskedPlans[index];
}

deoglRenderPlanMasked *deoglRenderPlan::AddMaskedPlanFor(deoglRenderPlan *plan){
	if(!plan) DETHROW(deeInvalidParam);
	
	if(pMaskedPlanCount == pMaskedPlanSize){
		int newSize = pMaskedPlanSize * 3 / 2 + 1;
		deoglRenderPlanMasked **newArray = new deoglRenderPlanMasked*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		
		memset(newArray, '\0', sizeof(deoglRenderPlanMasked*) * newSize);
		if(pMaskedPlans){
			memcpy(newArray, pMaskedPlans, sizeof(deoglRenderPlanMasked*) * pMaskedPlanSize);
			delete [] pMaskedPlans;
		}
		
		pMaskedPlans = newArray;
		pMaskedPlanSize = newSize;
	}
	
	if(!pMaskedPlans[pMaskedPlanCount]){
		pMaskedPlans[pMaskedPlanCount] = new deoglRenderPlanMasked;
	}
	
	pMaskedPlans[pMaskedPlanCount]->SetPlan(plan);
	pMaskedPlans[pMaskedPlanCount]->SetComponent(NULL, 0);
	pMaskedPlanCount++;
	
	plan->SetLevel(pLevel + 1); // it is deeper one level than us
	
	return pMaskedPlans[pMaskedPlanCount - 1];
}

void deoglRenderPlan::RemoveAllMaskedPlans(){
	while(pMaskedPlanCount > 0){
		pMaskedPlanCount--;
		pMaskedPlans[pMaskedPlanCount]->SetPlan(NULL);
		pMaskedPlans[pMaskedPlanCount]->SetComponent(NULL, 0);
	}
}



// Private Functions
//////////////////////

int deoglRenderPlan::pIndexOfLightWith(deoglCollideListLight *light) const{
	int i;
	
	for(i=0; i<pLightCount; i++){
		if(light == pLights[i]->GetLight()){
			return i;
		}
	}
	
	return -1;
}



void deoglRenderPlan::pCheckTransparency(){
	pHasTransparency = false;
	pHasXRayTransparency = false;
	
	// components
	const int componentCount = pCollideList.GetComponentCount();
	int i;
	for(i=0; i<componentCount; i++){
		const deoglRComponent &component = *pCollideList.GetComponentAt(i)->GetComponent();
		if(!component.GetSolid() || !component.GetOutlineSolid()){
			pHasTransparency = true;
		}
		if(!component.GetXRaySolid()){
			pHasXRayTransparency = true;
		}
	}
	if(pHasTransparency && pHasXRayTransparency){
		return;
	}
	
	// billboards
	const int billboardCount = pCollideList.GetBillboardCount();
	for(i=0; i<billboardCount; i++){
		const deoglRBillboard &billboard = *pCollideList.GetBillboardAt(i);
		if(billboard.GetUseSkinTexture()){
			if(!billboard.GetUseSkinTexture()->GetSolid()){
				pHasTransparency = true;
				if(billboard.GetUseSkinTexture()->GetXRay()){
					pHasXRayTransparency = true;
				}
			}
		}
	}
	if(pHasTransparency && pHasXRayTransparency){
		return;
	}
	
	// particles
	if(pRenderThread.GetChoices().GetRealTransparentParticles()){
		const deoglParticleEmitterInstanceList &peinstList = pCollideList.GetParticleEmitterList();
		const int peinstCount = peinstList.GetCount();
		int j;
		
		for(i=0; i< peinstCount; i++){
			const deoglRParticleEmitterInstance &instance = *peinstList.GetAt(i);
			if(instance.GetParticleCount() == 0){
				continue;
			}
			
			const int typeCount = instance.GetTypeCount();
			for(j=0; j<typeCount; j++){
				const deoglRSkin * const skin = instance.GetTypeAt(j).GetUseSkin();
				if(skin){
					if(!skin->GetIsSolid()){
						pHasTransparency = true;
						if(skin->GetHasXRay()){
							pHasXRayTransparency = true;
						}
					}
				}
				/*
				if(instance.GetEmitter()->GetTypeAt(j).GetHasTransparency()){
					pHasTransparency = true;
					return;
				}
				*/
			}
		}
		
	}else{
		// NOTE this check below only catches the case of a particle using a non-solid
		//      skin. it is though possible a solid skin is used with transparency curve
		//      in which case the skin becomes non-solid
		const deoglParticleEmitterInstanceList &peinstList = pCollideList.GetParticleEmitterList();
		const int peinstCount = peinstList.GetCount();
		int j;
		
		for(i=0; i< peinstCount; i++){
			const deoglRParticleEmitterInstance &instance = *peinstList.GetAt(i);
			if(instance.GetParticleCount() == 0){
				continue;
			}
			
			const int typeCount = instance.GetTypeCount();
			for(j=0; j<typeCount; j++){
				const deoglRSkin * const skin = instance.GetTypeAt(j).GetUseSkin();
				if(skin){
					if(!skin->GetIsSolid()){
						pHasTransparency = true;
						if(skin->GetHasXRay()){
							pHasXRayTransparency = true;
						}
					}
				}
			}
		}
	}
}

void deoglRenderPlan::pBuildRenderPlan(){
	// currently we simply assign a shared texture to all lights.
	// this is not very economic and will be replaced later on with
	// a better plan builder taking the on-screen size and location
	// of lights into consideration to down-tune shadow maps where
	// there is less chance of being noticed by the player to
	// conserve memory. also require is to add a memory checked to
	// avoid going beyond the memory capacity of the graphic card.
	// this test has to be only rough and should down-tune more
	// aggressively if the memory becomes scarce. for this to work
	// we need though first a framework to keep track of statically
	// spend GPU memory.
	INIT_SPECIAL_TIMING
	
	// determine if we need transparency
	pCheckTransparency();
	
	// assing the stencil mask to all masked plans
	int i;
	for(i=0; i<pMaskedPlanCount; i++){
		pMaskedPlans[i]->SetStencilMask(i + 1);
	}
	SPECIAL_TIMER_PRINT(">Misc")
	
	// first let's simply print out the number of lights and what
	// a conservative assignment would cause
	pBuildLightPlan();
	SPECIAL_TIMER_PRINT(">Light")
}

void deoglRenderPlan::pBuildLightPlan(){
	const int count = pCollideList.GetLightCount();
	int i;
	
	RemoveAllLights(); // better safe than sorry
	
	for(i=0; i<count; i++){
		// we have to add lights here. earlier is not possible since the elements in the
		// collide list are potentially removed due to culling
		deoglRenderPlanLight &planLight = *GetLightFor(pCollideList.GetLightAt(i)) ;
		
		planLight.Init();
		planLight.PlanShadowCasting();
	}
}



void deoglRenderPlan::pUpdateHTView(){
	if(pHTView && pWorld && &pHTView->GetHeightTerrain() == pWorld->GetHeightTerrain()){
		pHTView->Prepare();
		return;
	}
	
	pHTView = nullptr;
	
	if(pWorld && pWorld->GetHeightTerrain()){
		pHTView.TakeOverWithpWorld->GetHeightTerrain());
		pHTView->Prepare();
	}
}

void deoglRenderPlan::pUpdateHTViewRTSInstances(){
	if(!pHTView){
		return;
	}
	
	// we have to wait until the LOD level is calculated. now we can update the render task
	// shared instances of clusters in the collide list or all clusters. updating only the
	// clusters in the collide list is faster but other render code can potentially use it
	// too so it is better to update all clusters to avoid having to manage additional
	// height terrain views
	pHTView->UpdateAllRTSInstances();
}

void deoglRenderPlan::pCheckOutsideVisibility(){
	bool outsideWorldVisible = true;
	
	pSkyVisible = outsideWorldVisible;
	if(!outsideWorldVisible){
		pCollideList.RemoveAllHTSectors();
	}
}

void deoglRenderPlan::pDropLightsTemporary(){
	const int count = pCollideList.GetLightCount();
	int i;
	for(i=0; i<count; i++){
		pCollideList.GetLightAt(i)->GetLight()->GetShadowCaster()->DropTemporary();
	}
}
