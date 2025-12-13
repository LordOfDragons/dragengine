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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRLight.h"
#include "deoglLightTestForTouch.h"
#include "deoglLightGatherOcclusionMeshes.h"
#include "pipeline/deoglLightPipelinesPoint.h"
#include "pipeline/deoglLightPipelinesSpot.h"
#include "shader/deoglLightShader.h"
#include "shader/deoglLightShaderManager.h"
#include "volume/deoglLightVolume.h"
#include "volume/deoglLightVolumeBuilder.h"
#include "deoglNotifyEnvMapLightChanged.h"
#include "../canvas/render/deoglRCanvasView.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideList.h"
#include "../component/deoglRComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../occquery/deoglOcclusionQuery.h"
#include "../optimizer/deoglOptimizerLight.h"
#include "../optimizer/deoglOptimizerManager.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTChoices.h"
#include "../shadow/deoglSCSolid.h"
#include "../shadow/deoglSCTransparent.h"
#include "../shadow/deoglShadowCaster.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/deoglSkinRenderable.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../visitors/deoglTransformVolume.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionTriangle.h"
#include "../utils/collision/deoglDCollisionVolume.h"
#include "../utils/collision/deoglDCollisionDetection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/world/deWorld.h>



#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/shape/decShapeBox.h>
static decTimer timer;
#define DEBUG_RESET_TIMERS				timer.Reset();
#define DEBUG_PRINT_TIMER(what)			pOgl->LogInfoFormat("%s = %iys", what, (int)(timer.GetElapsedTime() * 1000000.0))



// Class deoglRLight::WorldComputeElement
///////////////////////////////////////////

deoglRLight::WorldComputeElement::WorldComputeElement(deoglRLight &light) :
deoglWorldComputeElement(eetLight, &light),
pLight(light){
}

void deoglRLight::WorldComputeElement::UpdateData(sDataElement &data) const {
	const decDVector &refpos = GetReferencePosition();
	data.SetExtends(pLight.GetMinimumExtend() - refpos, pLight.GetMaximumExtend() - refpos);
	data.SetLayerMask(pLight.GetLayerMask());
	data.flags = (uint32_t)deoglWorldCompute::eefLight;
}



// Class deoglRLight
/////////////////////

// Constructor, destructor
////////////////////////////

deoglRLight::deoglRLight(deoglRenderThread &renderThread) :
pRenderThread(renderThread),

pParentWorld(NULL),
pOctreeNode(NULL),
pWorldComputeElement(WorldComputeElement::Ref::New(*this)),

pActive(false),
pLightType(deLight::eltPoint),
pCastShadows(false),
pSpotAngle(DEG2RAD * 30.0f),
pSpotRatio(1.0f),
pSpotSmoothness(1.0f),
pSpotExponent(1.0f),
pHintMovement(deLight::emhStationary),
pHintShadowImportance(100),
pIntensity(0.0f),
pAmbientRatio(0.0f),
pColor(1.0f, 1.0f, 1.0f),
pSkinState(NULL),
pUseSkinTexture(NULL),
pDirtyPrepareSkinStateRenderables(true),
pDirtyRenderSkinStateRenderables(true),
pDirtyPrepareLightCanvas(true),

pCSOctreeIndex(0),

pWorldMarkedRemove(false),
pLLWorldPrev(NULL),
pLLWorldNext(NULL),

pLLPrepareForRenderWorld(this)
{
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyFullExtends = true;
	pDirtyExtends = true;
	pConvexVolumeList = NULL;
	pOptimizer = NULL;
	pColVol = NULL;
	pShadowCaster = NULL;
	pDirtyStaticShadows = true;
	pDirtyDynamicShadows = true;
	pDirtyCollideLists = true;
	pStaticCollideList = NULL;
	pDynamicCollideList = NULL;
	
	pLightVolume = NULL;
	pLightVolumeCropBox = NULL;
	
	pDirtyTouching = true;
	pMarked = false;
	pUpdateOnRemoveComponent = true;
	
	try{
		pConvexVolumeList = new decConvexVolumeList;
		pShadowCaster = new deoglShadowCaster(renderThread);
		pLightVolume = new deoglLightVolume(renderThread);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE(renderThread, Light);
}

deoglRLight::~deoglRLight(){
	LEAK_CHECK_FREE(pRenderThread, Light);
	pCleanUp();
}



// Management
///////////////

void deoglRLight::SetParentWorld(deoglRWorld *parentWorld){
	if(parentWorld == pParentWorld){
		return;
	}
	
	pWorldComputeElement->RemoveFromCompute();
	
	pParentWorld = parentWorld;
	
	RemoveAllComponents();
	
	if(pOctreeNode){
		pOctreeNode->RemoveLight(this);
		pOctreeNode = NULL;
	}
	
	pDirtyTouching = true;
}



void deoglRLight::SetOctreeNode(deoglWorldOctree *octreeNode){
	pOctreeNode = octreeNode;
}

void deoglRLight::UpdateOctreeNode(){
	// WARNING called from main thread during synchronization
	
	if(!pParentWorld){
		return;
	}
	
	if(pActive){
		pUpdateExtends(); // required or we might end up in the wrong octree
		pParentWorld->GetOctree().InsertLightIntoTree(this);
		
		if(pWorldComputeElement->GetWorldCompute()){
			pWorldComputeElement->ComputeUpdateElement();
			
		}else{
			pParentWorld->GetCompute().AddElement(pWorldComputeElement);
		}
		
	}else{
		pWorldComputeElement->RemoveFromCompute();
		if(pOctreeNode){
			pOctreeNode->RemoveLight(this);
			pOctreeNode = NULL;
		}
	}
}

void deoglRLight::UpdateSkin(float elapsed){
	if(pSkinState){
		pSkinState->AdvanceTime(elapsed);
	}
}



void deoglRLight::SetActive(bool active){
	if(active == pActive){
		return;
	}
	
	pActive = active;
	
	if(active){
		pRequiresPrepareForRender();
	}
}

void deoglRLight::SetLightType(deLight::eLightTypes type){
	pLightType = type;
}

void deoglRLight::SetCastShadows(bool castShadows){
	pCastShadows = castShadows;
}

void deoglRLight::SetSpotAngle(float angle){
	pSpotAngle = angle;
}

void deoglRLight::SetSpotRatio(float ratio){
	pSpotRatio = ratio;
}

void deoglRLight::SetSpotSmoothness(float smoothness){
	pSpotSmoothness = smoothness;
}

void deoglRLight::SetSpotExponent(float exponent){
	pSpotExponent = exponent;
}

void deoglRLight::SetHintMovement(deLight::eMovementHints movement){
	pHintMovement = movement;
}

void deoglRLight::SetHintShadowImportance(int importance){
	pHintShadowImportance = importance;
}

void deoglRLight::SetIntensity(float intensity){
	pIntensity = intensity;
}

void deoglRLight::SetAmbientRatio(float ratio){
	pAmbientRatio = ratio;
}



void deoglRLight::SetLightSkin(deoglRSkin *skin){
	if(skin == pLightSkin){
		return;
	}
	
	pUseSkinTexture = NULL;
	pLightSkin = skin;
	
	if(!skin){
		return;
	}
	if(skin->GetTextureCount() > 0){
		pUseSkinTexture = &skin->GetTextureAt(0);
	}
	
	if(!pSkinState){
		pSkinState = new deoglSkinState(pRenderThread, *this);
	}
}

void deoglRLight::SetLightCanvas(deoglRCanvasView *canvas){
	if(canvas == pLightCanvas){
		return;
	}
	pLightCanvas = canvas;
	   pRequiresPrepareForRender();
}

void deoglRLight::SetDynamicSkin(deoglRDynamicSkin *dynamicSkin){
	if(dynamicSkin == pDynamicSkin){
		return;
	}
	pDynamicSkin = dynamicSkin;
}

void deoglRLight::SetTransform(const decTexMatrix2 &matrix){
	pTransform = matrix;
}



void deoglRLight::SetColor(const decColor &color){
	pColor = color;
}

void deoglRLight::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
}

void deoglRLight::SetLayerMaskShadow(const decLayerMask &layerMask){
	pLayerMaskShadow = layerMask;
}

void deoglRLight::RemoveAllShadowIgnoreComponents(){
	if(pShadowIgnoreComponents.GetCount() == 0){
		return;
	}
	
	const decObjectSet components(pShadowIgnoreComponents);
	
	pShadowIgnoreComponents.RemoveAll();
	
	// test all components since they can now be potentially in shadow maps
	const int count = components.GetCount();
	int i;
	for(i=0; i<count; i++){
		TestComponent((deoglRComponent*)components.GetAt(i));
	}
}

void deoglRLight::AddShadowIgnoreComponent(deoglRComponent *component){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	pShadowIgnoreComponents.Add(component);
	
	// remove component in case it is enlisted for the static or dynamic shadow map
	RemoveComponent(component);
}

bool deoglRLight::HasShadowIgnoreComponent(deoglRComponent *component) const{
	return pShadowIgnoreComponents.Has(component);
}

bool deoglRLight::StaticMatchesCamera(const decLayerMask &layerMask) const{
	// if layer mask restriction is used dynamic only shadows have to be used to filter properly.
	// the logic is this. lights filter scene elements to be included in their shadow maps by
	// matching the element "layer mask" against the "shadow layer mask". if the camera restricts
	// the layer mask this filtering stays correct if all bits of the "shadow layer mask" are
	// covered by the bits of the "camery layer mask".
	// 
	// as a side note it would be also possible for this rule to not apply if not all bits of
	// the "shadow layer mask" match the "camera layer mask". this requires or combining all
	// layer masks of all filtered scene elements. if this combined layer mask does match in
	// all bits the "camera layer mask" then this would be enough to still fullfil the
	// requirement to use the static shadow maps.
	// TODO check if this special filter check should be added or not
	if(pLayerMaskShadow.IsEmpty() || pLayerMaskShadow.IsFull()){
		return layerMask.IsEmpty() || layerMask.IsFull();
		
	}else{
		return layerMask.IsEmpty() || (pLayerMaskShadow & layerMask) == pLayerMaskShadow;
	}
}



void deoglRLight::InitSkinStateStates(){
	if(pSkinState){
		pSkinState->InitAll();
	}
}

void deoglRLight::UpdateSkinStateStates(){
	if(pSkinState){
		pSkinState->UpdateAll();
	}
}

void deoglRLight::DirtyPrepareSkinStateRenderables(){
	pDirtyPrepareSkinStateRenderables = true;
	pDirtyRenderSkinStateRenderables = true;
	pRequiresPrepareForRender();
}

void deoglRLight::PrepareSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask){
	if(pSkinState){
		pSkinState->PrepareRenderables(pLightSkin, pDynamicSkin, renderPlanMask);
	}
}

void deoglRLight::RenderSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask){
	if(pSkinState){
		pSkinState->RenderRenderables(pLightSkin, pDynamicSkin, renderPlanMask);
	}
}

void deoglRLight::PrepareSkinStateConstructed(){
	if(pSkinState){
		pSkinState->PrepareConstructedProperties();
	}
}

void deoglRLight::DynamicSkinRenderablesChanged(){
// 	if( ! pDynamicSkin || ! pLightSkin || ! pLightSkin->GetHasRenderables() ){
// 		return;
// 	}
// 	
// 	MarkParamBlocksDirty();
// 	MarkTUCsDirty();
}

void deoglRLight::UpdateRenderableMapping(){
	if(!pSkinState){
		return;
	}
	
	// udpate mappings of dynamic skin of component itself
	pSkinState->RemoveAllRenderables();
	if(pLightSkin && pDynamicSkin){
		pSkinState->AddRenderables(*pLightSkin, *pDynamicSkin);
	}
	
// 	MarkParamBlocksDirty();
// 	MarkTUCsDirty();
// 	MarkUseSkinDirty(); // required?
}

void deoglRLight::DirtyPrepareLightCanvas(){
	pDirtyPrepareLightCanvas = true;
	pRequiresPrepareForRender();
}



void deoglRLight::SetMatrix(const decDMatrix &matrix){
	pMatrix = matrix;
	pInverseMatrix = matrix.QuickInvert();
}



bool deoglRLight::HasExtends() const{
	/*if( pLight->GetType() == deLight::eltDirectional ){
		return false;
	}*/
	return true;
}

void deoglRLight::SetDirtyFullExtends(){
	pDirtyFullExtends = true;
}

void deoglRLight::SetDirtyExtends(){
	pDirtyExtends = true;
	pRequiresPrepareForRender();
}



void deoglRLight::SetDirtyCollisionVolume(){
	pDirtyColVol = true;
	pRequiresPrepareForRender();
}



const deoglCollideList *deoglRLight::GetStaticCollideList(){
	pUpdateCollideLists();
	return pStaticCollideList;
}

const deoglCollideList *deoglRLight::GetDynamicCollideList(){
	pUpdateCollideLists();
	return pDynamicCollideList;
}

void deoglRLight::SetDirtyCollideLists(){
	pDirtyCollideLists = true;
}



void deoglRLight::SetLightVolumeDirty(){
	pDirtyConvexVolumeList = true; // HACK until the problem is solved
	
	if(pOptimizer){
		pOptimizer->ResetAllOptimizations();
	}else{
		// we have to create one... TODO
	}
	
	pRequiresPrepareForRender();
}



void deoglRLight::SetLightVolumeCropBox(decShapeBox *box){
	if(box == pLightVolumeCropBox){
		return;
	}
	
	if(pLightVolumeCropBox){
		delete pLightVolumeCropBox;
	}
	
	pLightVolumeCropBox = box;
	
	SetLightVolumeDirty();
}



deoglShadowCaster *deoglRLight::GetShadowCaster(){
	if(pDirtyStaticShadows || pDirtyDynamicShadows){
		if(pCastShadows){
			if(pHintMovement == deLight::emhStationary){
				if(pHintShadowImportance >= 75){
					if(pDynamicComponentList.GetCount() > 0){
						if(pStaticComponentList.GetCount() > 0){
							pShadowCaster->SetShadowType(deoglShadowCaster::estStaticAndDynamic);
							
						}else{
							pShadowCaster->SetShadowType(deoglShadowCaster::estDynamicOnly);
						}
						
					}else if(pStaticComponentList.GetCount() > 0){
						pShadowCaster->SetShadowType(deoglShadowCaster::estStaticOnly);
						
					}else{
						pShadowCaster->SetShadowType(deoglShadowCaster::estNoShadows);
					}
					
				}else{
					if(pStaticComponentList.GetCount() > 0){
						pShadowCaster->SetShadowType(deoglShadowCaster::estStaticOnly);
						
					}else{
						pShadowCaster->SetShadowType(deoglShadowCaster::estNoShadows);
					}
				}
				
			}else{
				if(pDynamicComponentList.GetCount() > 0 || pStaticComponentList.GetCount() > 0){
					pShadowCaster->SetShadowType(deoglShadowCaster::estDynamicOnly);
					
				}else{
					pShadowCaster->SetShadowType(deoglShadowCaster::estNoShadows);
				}
			}
			
		}else{
			pShadowCaster->SetShadowType(deoglShadowCaster::estNoShadows);
		}
	}
	
	if(pDirtyStaticShadows){
		pShadowCaster->Clear();
		pShadowCaster->SetStaticParams(0.01f, pRange);
		pDirtyStaticShadows = false;
	}
	
	if(pDirtyDynamicShadows){
		pShadowCaster->SetDynamicParams(0.01f, pRange);
		pDirtyDynamicShadows = false;
	}
	
	return pShadowCaster;
}

void deoglRLight::SetDirtyShadows(){
	pDirtyStaticShadows = true;
	pDirtyDynamicShadows = true;
}

void deoglRLight::ShadowCasterRequiresPrepare(){
	pRequiresPrepareForRender();
}



void deoglRLight::UpdateAttenuation(float range, float halfIntensityDistance){
	if(range < 0.001f){
		range = 0.001f;
	}
	
	pAttenCoeff = (1.0f / (halfIntensityDistance * halfIntensityDistance) - 2.0f) / (range * range);
	pRange = range;
	
	const float rangeNormInt = 1.0f / (1.0f + pAttenCoeff * range * range);
	
	pDampCoeff = 1.0f / (1.0f - rangeNormInt);
	pDampThreshold = -rangeNormInt * pDampCoeff;
}



void deoglRLight::ClearOptimizer(){
	pOptimizer = NULL;
}



void deoglRLight::EarlyPrepareForRender(){
	pUpdateFullExtends();
	pUpdateExtends();
	pUpdateLightVolume();
	pUpdateCollisionVolume();
}

void deoglRLight::PrepareForRender(const deoglRenderPlanMasked *renderPlanMask){
	if(pDirtyPrepareLightCanvas){
		if(pLightCanvas){
			pLightCanvas->PrepareForRender(renderPlanMask);
		}
		pDirtyPrepareLightCanvas = false;
	}
	
	PrepareSkinStateConstructed();
	
	if(pDirtyPrepareSkinStateRenderables){
		PrepareSkinStateRenderables(renderPlanMask);
		pDirtyPrepareSkinStateRenderables = false;
		pDirtyRenderSkinStateRenderables = true;
	}
	
	pCheckTouching();
	
	pShadowCaster->Update();
	
	// make sure the shadow caster is cleared of off dyamic shadow maps
	//pShadowCaster->GetSolid().DropDynamic();
	
	// update light volume vbo if existing. has to be done here. see UpdateLightVolume()
	if(pLightVolume){
		pLightVolume->UpdateVBO();
	}
	
	// force update next frame if required
	if(pShadowCaster->RequiresUpdate()){
		pRequiresPrepareForRender();
	}
}

void deoglRLight::PrepareForRenderRender(const deoglRenderPlanMasked *renderPlanMask){
	if(pDirtyRenderSkinStateRenderables){
		RenderSkinStateRenderables(renderPlanMask);
		pDirtyRenderSkinStateRenderables = false;
	}
}



deoglLightPipelines &deoglRLight::GetPipelines(){
	if(pPipelines){
		return pPipelines;
	}
	
	if(pLightType == deLight::eltPoint){
		pPipelines = deoglLightPipelinesPoint::Ref::New(*this);
		
	}else{
		pPipelines = deoglLightPipelinesSpot::Ref::New(*this);
	}
	
	deoglBatchedShaderLoading batched(pRenderThread, 1000.0f, true);
	try{
		pPipelines->Prepare(batched);
		
	}catch(const deException &){
		batched.WaitAllCompileFinished();
		throw;
	}
	
	batched.WaitAllCompileFinished();
	return pPipelines;
}

const deoglSPBlockUBO::Ref &deoglRLight::GetLightParameterBlock(){
	if(!pParamBlockLight){
		pParamBlockLight = GetPipelines().GetWithRef(
			deoglLightPipelines::etNoShadow, 0).GetShader()->CreateSPBLightParam();
	}
	return pParamBlockLight;
}

const deoglSPBlockUBO::Ref &deoglRLight::GetInstanceParameterBlock(){
	if(!pParamBlockInstance){
		pParamBlockInstance = GetPipelines().GetWithRef(
			deoglLightPipelines::etNoShadow, 0).GetShader()->CreateSPBInstParam();
	}
	return pParamBlockInstance;
}

const deoglSPBlockUBO::Ref &deoglRLight::GetOccQueryParameterBlock(){
	if(!pParamBlockOccQuery){
		pParamBlockOccQuery = deoglLightShader::CreateSPBOccQueryParam(pRenderThread);
	}
	return pParamBlockOccQuery;
}

void deoglRLight::DropPipelines(){
	pParamBlockOccQuery = nullptr;
	pParamBlockInstance = nullptr;
	pParamBlockLight = nullptr;
	pPipelines = nullptr;
}



void deoglRLight::SetDirtyTouching(){
	pDirtyTouching = true;
	pRequiresPrepareForRender();
}

void deoglRLight::EnvMapNotifyLightChanged(){
	if(!pParentWorld){
		return;
	}
	
	pUpdateExtends();
	
	deoglNotifyEnvMapLightChanged visitor(*this);
	pParentWorld->VisitRegion(pMinExtend, pMaxExtend, visitor);
}



void deoglRLight::PrepareQuickDispose(){
	pParentWorld = NULL;
	pOctreeNode = NULL;
	pStaticComponentList.RemoveAll();
	pDynamicComponentList.RemoveAll();
}



// Components
///////////////

void deoglRLight::AddComponent(deoglRComponent *component){
	if(component->GetRenderStatic() && pHintMovement == deLight::emhStationary){
		if(pStaticComponentList.AddIfMissing(component)){
			pDirtyStaticShadows = true;
			pDirtyCollideLists = true;
			SetLightVolumeCropBox(NULL);
			
			if(component->GetOcclusionMesh()){
				SetLightVolumeDirty();
			}
		}
		
	}else{
		if(pDynamicComponentList.AddIfMissing(component)){
			pDirtyDynamicShadows = true;
			pDirtyCollideLists = true;
		}
	}
}

void deoglRLight::RemoveComponent(deoglRComponent *component){
	if(pStaticComponentList.RemoveIfExisting(component)){
		pDirtyCollideLists = true;
		
		if(pUpdateOnRemoveComponent){
			pDirtyStaticShadows = true;
			SetLightVolumeCropBox(NULL);
			
			if(component->GetOcclusionMesh()){
				SetLightVolumeDirty();
			}
		}
	}
	
	if(pDynamicComponentList.RemoveIfExisting(component)){
		pDirtyDynamicShadows = true;
		pDirtyCollideLists = true;
	}
}

void deoglRLight::RemoveAllComponents(){
	int i, count;
	
	count = pStaticComponentList.GetCount();
	if(count > 0){
		for(i=0; i<count; i++){
			pStaticComponentList.GetAt(i)->GetLightList().RemoveIfExisting(this);
		}
		pStaticComponentList.RemoveAll();
		pDirtyStaticShadows = true;
		pDirtyCollideLists = true;
		SetLightVolumeCropBox(NULL);
	}
	
	count = pDynamicComponentList.GetCount();
	if(count > 0){
		for(i=0; i<count; i++){
			pDynamicComponentList.GetAt(i)->GetLightList().RemoveIfExisting(this);
		}
		pDynamicComponentList.RemoveAll();
		pDirtyDynamicShadows = true;
		pDirtyCollideLists = true;
	}
	
	//DirtyLightVolume();
}

void deoglRLight::TestComponent(deoglRComponent *component){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	if(pShadowIgnoreComponents.Has(component)){
		return;
	}
	
	if(pLayerMaskShadow.IsNotEmpty() && component->GetLayerMask().IsNotEmpty()
	&& pLayerMaskShadow.MatchesNot(component->GetLayerMask())){
		return;
	}
	
	bool touchesLight = false;
	
	pUpdateExtends();
	
	if(component->GetMaximumExtend() >= pMinExtend && component->GetMinimumExtend() <= pMaxExtend){
		touchesLight = true;
		
		switch(pLightType){
		case deLight::eltSpot:{
			// cone-sphere collision test after geometrictools.com . the problem here is that
			// the cone can be squashed. to solve this the radius used for the final test is
			// adjusted depending on where the sphere center is located before being ratio
			// corrected. this is a good enough
			decVector sphereCenter(pInverseMatrix * component->GetCullSphereCenter());
			
			
			const float sphereRadiusX = component->GetCullSphereRadius();
			const float sphereRadiusY = sphereRadiusX * pSpotRatio;
			
			const float srAngle = atan2f(sphereCenter.y, sphereCenter.x);
			const float srConeSin = sinf(srAngle);
			const float srConeCos = cosf(srAngle);
			
			const float sphereRadius = sphereRadiusX * sphereRadiusY / sqrtf(
				sphereRadiusX * sphereRadiusX * srConeCos * srConeCos
				+ sphereRadiusY * sphereRadiusY * srConeSin * srConeSin);
			const float sphereRadiusSquared = sphereRadius * sphereRadius;
			
			sphereCenter.y *= pSpotRatio;
			
			
			//const float sphereRadius = component->GetCullSphereRadius();
			//const float sphereRadiusSquared = sphereRadius * sphereRadius;
			
			// from here on it is the geometrictools.com version simply modified with V=zero
			// and D=(0,0,1)
			const float coneSin = sinf(pSpotAngle * 0.5f);
			const float coneCos = cosf(pSpotAngle * 0.5f);
			const float coneSinSquared = coneSin * coneSin;
			const float coneCosSquared = coneCos * coneCos;
			const float coneSinReci = 1.0f / coneSin;
			
			touchesLight = false;
			
			const decVector d(sphereCenter.x, sphereCenter.y,
				sphereCenter.z + sphereRadius * coneSinReci);
			
			if(d.z <= 0.0 || d.z * d.z < d.LengthSquared() * coneCosSquared){
				break;
			}
			
			const float dsquared = sphereCenter.LengthSquared();
			const float e = -sphereCenter.z;
			
			if(e > 0.0 && e * e >= dsquared * coneSinSquared){
				touchesLight = (dsquared <= sphereRadiusSquared);
				
			}else{
				touchesLight = true;
			}
			}break;
			
			/*
			// previous code
			
			// cone-sphere collision test after geometrictools.com
			const decDVector &sphereCenter = component->GetCullSphereCenter();
			const float sphereRadius = component->GetCullSphereRadius();
			const float sphereRadiusSquared = sphereRadius * sphereRadius;
			const decDVector coneCenter(pMatrix.GetPosition());
			const decDVector coneAxis(pMatrix.TransformView());
			const float coneSin = sinf(pSpotAngles.y * 0.5f);
			const float coneCos = cosf(pSpotAngles.y * 0.5f);
			const float coneCosSquared = coneCos * coneCos;
			const float coneSinSquared = coneSin * coneSin;
			const float coneSinReci = 1.0f / coneSin;
			double dsquared, e;
			decDVector u, d;
			
			touchesLight = false;
			
			u = coneCenter - coneAxis * (double)(sphereRadius * coneSinReci);
			d = sphereCenter - u;
			dsquared = d * d;
			e = coneAxis * d;
			
			if(e > 0.0 && e * e >= dsquared * (double)coneCosSquared){
				d = sphereCenter - coneCenter;
				dsquared = d * d;
				e = -(coneAxis * d);
				
				if(e > 0.0 && e * e >= dsquared * (double)coneSinSquared){
					touchesLight = (dsquared <= (double)sphereRadiusSquared);
					
				}else{
					touchesLight = true;
				}
			}
			*/
			
		case deLight::eltProjector:{
			// TODO this needs a frustum-aabb check
			}break;
			
		case deLight::eltPoint:
			break;
		}
	}
	
	if(touchesLight){
		AddComponent(component);
		component->GetLightList().AddIfMissing(this);
		
	}else{
		RemoveComponent(component);
		component->GetLightList().RemoveIfExisting(this);
	}
}



// Optimizations
//////////////////

void deoglRLight::LightVolumeImproved(){
	pDirtyColVol = true;
	pDirtyCollideLists = true;
	pDirtyExtends = true;
	pDirtyStaticShadows = true;
	pDirtyDynamicShadows = true;
	pDirtyTouching = true;
	pRequiresPrepareForRender();
}

void deoglRLight::ReplaceLightVolume(decConvexVolumeList *list){
	if(!list) DETHROW(deeInvalidParam);
	if(pConvexVolumeList) delete pConvexVolumeList;
	pConvexVolumeList = list;
	LightVolumeImproved();
}

void deoglRLight::ReplaceShadowCaster(deoglShadowCaster *shadowCaster){
	if(!shadowCaster) DETHROW(deeInvalidParam);
	if(pShadowCaster) delete pShadowCaster;
	pShadowCaster = shadowCaster;
	//pCalcShadowCasterParams();
	pDirtyStaticShadows = true;
	pDirtyDynamicShadows = true;
	
	// some stats
//	pOgl->LogInfoFormat( "Updated Shadow Caster for %p:", this );
//	pOgl->LogInfoFormat( "   Solid: %i points", pShadowCaster->GetSolid()->GetPointCount() );
//	pOgl->LogInfoFormat( "   Transp: %i textures, %i points",
//		pShadowCaster->GetTransparent()->GetTextureCount(),
//		pShadowCaster->GetTransparent()->GetPointCount() );
}



// Render world usage
///////////////////////

void deoglRLight::SetWorldMarkedRemove(bool marked){
	pWorldMarkedRemove = marked;
}

void deoglRLight::SetLLWorldPrev(deoglRLight *light){
	pLLWorldPrev = light;
}

void deoglRLight::SetLLWorldNext(deoglRLight *light){
	pLLWorldNext = light;
}



// Private Functions
//////////////////////

void deoglRLight::pCleanUp(){
	SetParentWorld(NULL);
	
	if(pDynamicCollideList){
		delete pDynamicCollideList;
	}
	if(pStaticCollideList){
		delete pStaticCollideList;
	}
	
	pShadowIgnoreComponents.RemoveAll();
	
	RemoveAllComponents();
	
	if(pOptimizer){
		pRenderThread.GetOptimizerManager().RemoveOptimizer(pOptimizer);
	}
	if(pLightVolumeCropBox){
		delete pLightVolumeCropBox;
	}
	if(pConvexVolumeList){
		delete pConvexVolumeList;
	}
	if(pColVol){
		delete pColVol;
	}
	if(pLightVolume){
		delete pLightVolume;
	}
	if(pShadowCaster){
		delete pShadowCaster;
	}
	if(pSkinState){
		delete pSkinState;
	}
}

void deoglRLight::pUpdateFullExtends(){
	if(!pDirtyFullExtends){
		return;
	}
	
	const decDVector lightPosition(pMatrix.GetPosition());
	
	switch(pLightType){
	case deLight::eltPoint:
		// set extends from a box around the light sphere
		pFullMinExtend.x = lightPosition.x - pRange;
		pFullMinExtend.y = lightPosition.y - pRange;
		pFullMinExtend.z = lightPosition.z - pRange;
		
		pFullMaxExtend.x = lightPosition.x + pRange;
		pFullMaxExtend.y = lightPosition.y + pRange;
		pFullMaxExtend.z = lightPosition.z + pRange;
		break;
		
	case deLight::eltSpot:{
		// the calculation of the boundary box around the light cone boils down to the
		// calculation of the boundary box around the circular base of the cone using
		// the cone tip as the starting value of the boundary. box. the boundary box
		// around a circle in 3d is calculated like this:
		// 
		//   he_x = radius * sin( acos( cos( normal dot axis ) ) )
		// 
		// he_x is the half extends. normal dot axis returns cos( a ) since both are
		// normalized quantities. so we are looking actually for sin( a ). this can
		// be written more easily using only a square root instead of trigonometric
		// functions like this:
		//   
		//   he_x = radius * sqrt( 1 - squared( normal dot axis ) )
		// 
		// the resulting boundary box is then (O-HE)-(O+HE) for O as the center of the
		// circle and HE = (he_x, he_y, he_z).
		// 
		// in this case some shortcuts are used. the normal is actually the negation of
		// the circle normal. hence -nx is used instead of nx in the above calculation.
		// for squaring this is though identical
		// 
		// TODO fix this since now spot cone can be squashed. we need something better
		pFullMinExtend.x = lightPosition.x - pRange;
		pFullMinExtend.y = lightPosition.y - pRange;
		pFullMinExtend.z = lightPosition.z - pRange;
		
		pFullMaxExtend.x = lightPosition.x + pRange;
		pFullMaxExtend.y = lightPosition.y + pRange;
		pFullMaxExtend.z = lightPosition.z + pRange;
		
		/*
		const decDVector normal(pMatrix.TransformView());
		const decDVector center = lightPosition + normal * (double)pRange;
		const int resolution = 12;
		const float circleAngleStep = PI * 2.0f / (float)resolution;
		const float circleRadius = pRange * tanf(pSpotAngles.y * 0.5f) / cosf(circleAngleStep * 0.5f);
		decDVector halfExtend;
		
		halfExtend.x = sqrt(1.0 - normal.x * normal.x) * (double)circleRadius;
		halfExtend.y = sqrt(1.0 - normal.y * normal.y) * (double)circleRadius;
		halfExtend.z = sqrt(1.0 - normal.z * normal.z) * (double)circleRadius;
		
		pFullMinExtend = center - halfExtend;
		pFullMaxExtend = center + halfExtend;
		
		// add the cone base to the boundary box
		if(lightPosition.x < pFullMinExtend.x){
			pFullMinExtend.x = lightPosition.x;
			
		}else if(lightPosition.x > pFullMaxExtend.x){
			pFullMaxExtend.x = lightPosition.x;
		}
		
		if(lightPosition.y < pFullMinExtend.y){
			pFullMinExtend.y = lightPosition.y;
			
		}else if(lightPosition.y > pFullMaxExtend.y){
			pFullMaxExtend.y = lightPosition.y;
		}
		
		if(lightPosition.z < pFullMinExtend.z){
			pFullMinExtend.z = lightPosition.z;
			
		}else if(lightPosition.z > pFullMaxExtend.z){
			pFullMaxExtend.z = lightPosition.z;
		}
		*/
		}break;
		
	case deLight::eltProjector:
		// TODO: same code as for the spot light case just with a box around a frustum.
		// this only requires finding the box around the 5 frustum points (if the near
		// vertices all fall together)
		
		// set extends from a box around the light sphere. a frustum would be the best
		// solution but currently the frustum does not contain enclosing box code
		pFullMinExtend.x = lightPosition.x - pRange;
		pFullMinExtend.y = lightPosition.y - pRange;
		pFullMinExtend.z = lightPosition.z - pRange;
		
		pFullMaxExtend.x = lightPosition.x + pRange;
		pFullMaxExtend.y = lightPosition.y + pRange;
		pFullMaxExtend.z = lightPosition.z + pRange;
	}
	
	pDirtyFullExtends = false;
}

void deoglRLight::pUpdateExtends(){
	if(!pDirtyExtends){
		return;
	}
	
	pUpdateFullExtends();
	
	pMinExtend = pFullMinExtend;
	pMaxExtend = pFullMaxExtend;
	
	pUpdateLightVolume();
	
	// if the light casts shadows determine a matching box using the light volume if existing
	if(pCastShadows){
		//if( pLight->GetType() == deLight::eltSpot )
		deoglLightVolumeBuilder builder;
		builder.GetTransformedVolumeExtends(*pConvexVolumeList, pMatrix, pMinExtend, pMaxExtend);
	}
	
	pDirtyExtends = false;
}

void deoglRLight::pUpdateCollideLists(){
	if(!pDirtyCollideLists) return;
	
	int i, count;
	
	// create collide lists if not existng or clear them if existing
	if(pStaticCollideList){
		pStaticCollideList->Clear();
		
	}else{
		pStaticCollideList = new deoglCollideList;
	}
	
	if(pDynamicCollideList){
		pDynamicCollideList->Clear();
		
	}else{
		pDynamicCollideList = new deoglCollideList;
	}
	
	// update static collide list
	count = pStaticComponentList.GetCount();
	for(i=0; i<count; i++){
		pStaticCollideList->AddComponent(pStaticComponentList.GetAt(i));
	}
	
	// update dynamic collide list
	count = pDynamicComponentList.GetCount();
	for(i=0; i<count; i++){
		pDynamicCollideList->AddComponent(pDynamicComponentList.GetAt(i));
	}
	
	// no more dirty
	pDirtyCollideLists = false;
}

void deoglRLight::pCheckTouching(){
	if(!pDirtyTouching){
		return;
	}
	
	pDirtyTouching = false; // at the beginning to avoid potential dead-loops
	
	//if( pLight->GetHintMovement() == deLight::emhStationary ){
	
	pStaticComponentList.MarkAll(true);
	pDynamicComponentList.MarkAll(true);
	
	if(pActive && pParentWorld){
		pUpdateExtends();
		pDirtyTouching = false; // pUpdateExtends potentially sets pDirtyTouching to true
		
		deoglLightTestForTouch visitor(*this);
		pUpdateOnRemoveComponent = false;
		pParentWorld->VisitRegion(pMinExtend, pMaxExtend, visitor);
		pUpdateOnRemoveComponent = true;
	}
	
	// remove light from all non-marked components. act like pUpdateOnRemoveComponent is false.
	// this is required or else components still hold to this light pointer although it is
	// freed some time later. RemoveAllMarked only removes the components but does not remove
	// the light from them.
	const int dynamicComponentCount = pDynamicComponentList.GetCount();
	const int staticComponentCount = pStaticComponentList.GetCount();
	int i;
	
	for(i=0; i<staticComponentCount; i++){
		deoglRComponent &component = *pStaticComponentList.GetAt(i);
		if(component.GetMarked()){
			component.GetLightList().RemoveIfExisting(this);
		}
	}
	
	for(i=0; i<dynamicComponentCount; i++){
		deoglRComponent &component = *pDynamicComponentList.GetAt(i);
		if(component.GetMarked()){
			component.GetLightList().RemoveIfExisting(this);
		}
	}
	
	// now the components can be safely removed in an efficient way
	pStaticComponentList.RemoveAllMarked(true);
	pDynamicComponentList.RemoveAllMarked(true);
}

void deoglRLight::pUpdateCollisionVolume(){
	if(!pDirtyColVol){
		return;
	}
	
	const decDVector lightPosition(pMatrix.GetPosition());
	deoglDCollisionVolume *newColVol = NULL;
	
	try{
		// if there exists no light volume yet create a default collision volume
		if(pConvexVolumeList->GetVolumeCount() == 0){
			switch(pLightType){
			case deLight::eltPoint:
				// create a sphere enclosing the point light
				newColVol = new deoglDCollisionSphere(lightPosition, pRange);
				break;
				
			default:
				// create a sphere around the spot light. a frustum would be the best
				// solution but currently the frustum is not able to test for collisions
				// with another frustum
				newColVol = new deoglDCollisionSphere(lightPosition, pRange);
			}
			
		// otherwise calculate a box around the light volume list
		}else{
			pUpdateExtends();
			const decDVector halfSize((pMaxExtend - pMinExtend) * 0.5f);
			newColVol = new deoglDCollisionBox(pMinExtend + halfSize, halfSize);
		}
		
		// replace old collision volumes
		if(pColVol){
			delete pColVol;
		}
		pColVol = newColVol;
		
	}catch(const deException &){
		if(newColVol){
			delete newColVol;
		}
		throw;
	}
	
	pDirtyColVol = false;
}

void deoglRLight::pUpdateLightVolume(){
	// NOTE Can be called indirectly from main thread during synchronization.
	
	if(!pDirtyConvexVolumeList){
		return;
	}
	
	deoglLightVolumeBuilder builder;
	
	// cropping produces unfortunately a concave volume which we can't use. to solve this
	// a convex hull has to be calculated from the volume points. this is correctly done
	// using a gift-wrap algorithm. for the time being though we simply calculate an axis
	// aligned bounding box and clip the volume against it. this produces a result that
	// is not as optimal as the gift-wrap but it's better than nothing for the time being
//	decDVector boundingBoxMinExtend, boundingBoxMaxExtend;
	
//	builder.GetTransformedVolumeExtends( *pConvexVolumeList, pMatrix, boundingBoxMinExtend, boundingBoxMaxExtend );
	
	switch(pLightType){
	case deLight::eltPoint:
		builder.BuildSphere(*pConvexVolumeList, decVector(), pRange);
		break;
		
	case deLight::eltSpot:
		builder.BuildCone(*pConvexVolumeList, decMatrix(), pRange,
			pSpotAngle * 0.5f, pSpotAngle * pSpotRatio * 0.5f, 12);
		break;
		
	case deLight::eltProjector:
		builder.BuildFrustum(*pConvexVolumeList, decMatrix(), pRange,
			pSpotAngle * 0.5f, pSpotAngle * pSpotRatio * 0.5f);
		break;
	}
	
	if(pLightVolumeCropBox){
		const decDVector minExtend(pLightVolumeCropBox->GetPosition()
			- pLightVolumeCropBox->GetHalfExtends());
		const decDVector maxExtend(pLightVolumeCropBox->GetPosition()
			+ pLightVolumeCropBox->GetHalfExtends());
		
		builder.CropByBoundingBox(*pConvexVolumeList,
			pInverseMatrix.GetRotationMatrix(), minExtend, maxExtend);
	}
	
//	builder.CropByBoundingBox( *pConvexVolumeList, invLightMatrix, boundingBoxMinExtend, boundingBoxMaxExtend );
	
	// optimizer stuff
#if 0
	if(!pOptimizer){
		try{
			pOptimizer = new deoglOptimizerLight(this, oglWorld);
			
			if(lightType == deLight::eltPoint){
				pOptimizer->SetOptimizeShadowCaster(false);
			}else{
				pOptimizer->SetOptimizeShadowCaster(true);
			}
			
			if(pLight->GetHintMovement() == deLight::emhStationary){
				pOptimizer->SetInitialWarmUpTime(100);
				if(lightType == deLight::eltPoint){
					pOptimizer->SetMaximalVolumeCount(200);
				}else{
					pOptimizer->SetMaximalVolumeCount(50);
				}
				
			}else{
				pOptimizer->SetInitialWarmUpTime(500);
				if(lightType == deLight::eltPoint){
					pOptimizer->SetMaximalVolumeCount(100);
				}else{
					pOptimizer->SetMaximalVolumeCount(25);
				}
			}
			
			pOptimizer->ResetAllOptimizations();
			
			pOgl->GetOptimizerManager()->AddOptimizer(pOptimizer);
			
		}catch(const deException &){
			if(pOptimizer) delete pOptimizer;
			throw;
		}
	}
#endif
	
	// sanity check. if there is not a single volume in the list the chance is high the light volume
	// becomes concave. we only write a warning here for the case we experiment during debugging
	if(pConvexVolumeList->GetVolumeCount() > 1){
		const decDVector position(pMatrix.GetPosition());
		pRenderThread.GetLogger().LogWarnFormat("Light at (%g,%g,%g) has a light volume with %i volumes (potentially concave)",
			position.x, position.y, position.z, pConvexVolumeList->GetVolumeCount());
	}
	
	// rebuild the light volume using the convex volume list
	pLightVolume->CreateFrom(*pConvexVolumeList);
	// we can not update the VBO here because this metho can be potentially called by the
	// main thread during synchronization over detours
	
	// extends are usually dirty now
	pDirtyExtends = true;
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyTouching = true;
	pRequiresPrepareForRender();
	
	// no more dirty
	pDirtyConvexVolumeList = false;
}

void deoglRLight::pRequiresPrepareForRender(){
	if(!pLLPrepareForRenderWorld.GetList() && pParentWorld){
		pParentWorld->AddPrepareForRenderLight(this);
	}
}
