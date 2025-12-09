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

#include "deoglComponent.h"
#include "deoglComponentListener.h"
#include "deoglComponentTestForTouch.h"
#include "deoglRComponent.h"
#include "deoglRComponentTexture.h"
#include "deoglRComponentLOD.h"
#include "deoglRComponentWCElement.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListManager.h"
#include "../configuration/deoglConfiguration.h"
#include "../decal/deoglDecal.h"
#include "../decal/deoglRDecal.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../extensions/deoglExtensions.h"
#include "../light/deoglRLight.h"
#include "../model/deoglModelLOD.h"
#include "../model/face/deoglModelFace.h"
#include "../model/texture/deoglModelTexture.h"
#include "../occlusiontest/mesh/deoglDynamicOcclusionMesh.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../rendering/deoglRenderWorld.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../rendering/plan/deoglRenderPlanMasked.h"
#include "../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTUniqueKey.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRenderThread.h"
#include "../shaders/paramblock/deoglSPBParameter.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElementMapBuffer.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinRenderable.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../sky/deoglRSkyInstance.h"
#include "../target/deoglRenderTarget.h"
#include "../utils/deoglCubeHelper.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vao/deoglVAO.h"
#include "../visitors/deoglFindBestEnvMap.h"
#include "../world/deoglRCamera.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionDetection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>

#ifdef OS_W32
#undef near
#undef far
#endif



// Class deoglRComponent
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRComponent::deoglRComponent(deoglRenderThread &renderThread) :
pRenderThread(renderThread),

pParentWorld(NULL),
pOctreeNode(NULL),
pWorldComputeElement(deoglWorldComputeElement::Ref::New(new deoglRComponentWCElement(*this))),
pHasEnteredWorld(false),

pVisible(true),
pMovementHint(deComponent::emhStationary),
pGIImportance(5),

pStaticTextures(true),
pDirtyModelVBOs(true),

pDirtyOccMeshVBO(true),
pDirtyOccMeshSharedSPBElement(true),

pDirtyLODVBOs(true),
pDirtyLODRenderTaskConfigs(true),

pSkinRendered(renderThread, *this),

pDirtyTextureTUCs(true),
pDirtyTextureParamBlocks(true),
pOutlineTextureCount(0),
pDirtyDecals(true),
pDirtyDecalsRenderRenderables(true),

pVertexPositionSetWeights(nullptr),
pVertexPositionSetCount(0),

pCSOctreeIndex(0),

pWorldMarkedRemove(false),
pLLWorldPrev(NULL),
pLLWorldNext(NULL),

pLLPrepareForRenderWorld(this)
{
	pLODErrorScaling = 1.0f;
	
	pDynamicOcclusionMesh = NULL;
	pDynOccMeshRequiresPrepareForRender = true;
	
	pSkinState = NULL;
	pDirtyPrepareSkinStateRenderables = true;
	pDirtyRenderSkinStateRenderables = true;
	
	pFirstRender = true;
	pRenderStatic = true;
	pStaticSince = 0.0f;
	
	pBoneMatrices = NULL;
	pBoneMatrixCount = 0;
	
	pRenderMode = ermStatic;
	
	pDirtyModelRigMappings = true;
	
	pSolid = true;
	pOutlineSolid = true;
	pXRaySolid = true;
	pDirtySolid = true;
	
	pMarked = false;
	
	pDirtyCulling = true;
	
	pRenderEnvMapFadePerTime = 1.0f;
	pRenderEnvMapFadeFactor = 1.0f;
	pDirtyRenderEnvMap = true;
	
	pListenerIndex = 0;
	
	try{
		pUniqueKey = renderThread.GetUniqueKey().Get();
		pSkinState = new deoglSkinState(renderThread, *this);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE(renderThread, Component);
}

deoglRComponent::~deoglRComponent(){
	LEAK_CHECK_FREE(pRenderThread, Component);
	pCleanUp();
}



// Management
///////////////

void deoglRComponent::SetParentWorld(deoglRWorld *parentWorld){
	if(parentWorld == pParentWorld){
		return;
	}
	
	if(pParentWorld && /*pRenderStatic*/ pMovementHint == deComponent::emhStationary && pRenderMode == ermStatic){
		NotifySkiesUpdateStatic();
	}
	
	pHasEnteredWorld = false;
	InvalidateRenderEnvMap();
	pSkinRendered.DropDelayedDeletionObjects();
	
	/*if( pEnvMap ){
		pEnvMap->SetWorld(parentWorld);
	}*/
	
	const int decalCount = pDecals.GetCount();
	
	if(pParentWorld){
		int i;
		for(i=0; i<decalCount; i++){
			((deoglRDecal*)pDecals.GetAt(i))->RemoveFromWorldCompute();
		}
		pWorldComputeElement->RemoveFromCompute();
		
		// make sure we are unregistered from the old world. this is required since the
		// calls above can potentially smuggle in an AddPrepareForRenderComponent() call
		// along complicated ways. if this happens any further prepare for render is not
		// possible anymore and the object vanishes and even throws exceptions
		pParentWorld->RemovePrepareForRenderComponent(this);
	}
	
	pParentWorld = parentWorld;
	
	pRemoveFromAllLights();
	
	if(pOctreeNode){
		pOctreeNode->RemoveComponent(this);
	}
	
	pDirtyRenderEnvMap = true;
	pFirstRender = true;
	NotifyParentWorldChanged();
	
	// no NotifySkiesUpdateStatic on entering worls since we start out dynamic
	
	pRequiresPrepareForRender();
}

void deoglRComponent::HasEnteredWorld(){
	pHasEnteredWorld = true;
}



void deoglRComponent::SetOctreeNode(deoglWorldOctree *octreeNode){
	pOctreeNode = octreeNode;
}

// extern int hackCSSpecialCount;
// extern float hackCSSpecialTime;

void deoglRComponent::UpdateOctreeNode(){
	// WARNING Called during synchronization from main thread.
	
	if(!pParentWorld){
		return;
	}
	
	NotifyLightsDirtyLightVolume();
	pRemoveFromAllLights();
	
	// NotifySkiesUpdateStatic(); // called after ResetRenderStatic() so not required
	if(pMovementHint == deComponent::emhStationary && pRenderMode == ermStatic){
		NotifySkiesUpdateStatic();
	}
	
	// insert into parent world octree
	const int decalCount = pDecals.GetCount();
	int i;
	
	if(pVisible && pModel){
		pParentWorld->GetOctree().InsertComponentIntoTree(this);
		
		if(pWorldComputeElement->GetWorldCompute()){
			pWorldComputeElement->ComputeUpdateElement();
			for(i=0; i<decalCount; i++){
				((deoglRDecal*)pDecals.GetAt(i))->UpdateWorldCompute();
			}
			
		}else{
			deoglWorldCompute &worldCompute = pParentWorld->GetCompute();
			worldCompute.AddElement(pWorldComputeElement);
			for(i=0; i<decalCount; i++){
				((deoglRDecal*)pDecals.GetAt(i))->AddToWorldCompute(worldCompute);
			}
		}
		
		// visit the world for touching lights
// 			decTimer timer;
		deoglComponentTestForTouch testForTouching(this);
		pParentWorld->VisitRegion(pMinExtend, pMaxExtend, testForTouching);
// 			hackCSSpecialCount++;
// 			hackCSSpecialTime += timer.GetElapsedTime();
		
	}else{
		for(i=0; i<decalCount; i++){
			((deoglRDecal*)pDecals.GetAt(i))->RemoveFromWorldCompute();
		}
		pWorldComputeElement->RemoveFromCompute();
		
		if(pOctreeNode){
			pOctreeNode->RemoveComponent(this);
		}
	}
}



void deoglRComponent::SetVisible(bool visible){
	if(visible == pVisible){
		return;
	}
	
	pVisible = visible;
	
	if(visible && pParentWorld && pHasEnteredWorld){
		pParentWorld->GIStatesNotifyComponentBecameVisible(this);
	}
	NotifyVisibilityChanged();
	
	if(visible){
		pRequiresPrepareForRender();
	}
}

void deoglRComponent::SetMovementHint(deComponent::eMovementHints hint){
	if(hint == pMovementHint){
		return;
	}
	
	pMovementHint = hint;
	NotifyMovementHintChanged();
	
	if(pRenderMode == ermStatic){
		NotifySkiesUpdateStatic();
	}
}

void deoglRComponent::SetGIImportance(int importance){
	if(importance == pGIImportance){
		return;
	}
	
	pGIImportance = importance;
	
	if(pParentWorld && pHasEnteredWorld){
		pParentWorld->GIStatesNotifyComponentChangedGIImportance(this);
	}
	
	NotifyGIImportanceChanged();
}

void deoglRComponent::SetLayerMask(const decLayerMask &layerMask){
	if(layerMask == pLayerMask){
		return;
	}
	
	pLayerMask = layerMask;
	
	// light shadow matching potentially changed
	const deoglLightList list(pLightList);
	int i, count = list.GetCount();
	for(i=0; i<count; i++){
		list.GetAt(i)->TestComponent(this);
	}
	
	if(/*pRenderStatic*/ pMovementHint == deComponent::emhStationary && pRenderMode == ermStatic){
		NotifySkiesUpdateStatic();
	}
	
	if(pParentWorld && pHasEnteredWorld){
		pParentWorld->GIStatesNotifyComponentChangedLayerMask(this);
	}
	
	NotifyLayerMaskChanged();
}



void deoglRComponent::SetModel(deoglRModel *model){
	if(pModel == model){
		return;
	}
	
	// required to be guarded until listeners are notified to avoid memory corruptions
	const deoglRModel::Ref guardModel(pModel);
	pModel = model;
	
	InvalidateVAO();
	
	pDirtyModelRigMappings = true;
	pDirtyModelVBOs = true;
	
	pDirtyTextureParamBlocks = true;
	pDirtyLODRenderTaskConfigs = true;
	InvalidateAllTexturesParamBlocks(); // required only if not using world shared spb
	
	pResizeModelSkinMappings();
	pUpdateModelSkinMappings();
	pResizeModelRigMappings();
	pResizeBoneMatrices();
	pUpdateRenderMode();
	
	NotifyModelChanged();
	NotifyBoundariesChanged();
	
	pRequiresPrepareForRender();
}

void deoglRComponent::SetSkin(deoglRSkin *skin){
	if(skin == pSkin){
		return;
	}
	pSkin = skin;
	pUpdateModelSkinMappings();
	
	pSkinRendered.SetDirty();
}

void deoglRComponent::RigChanged(){
	InvalidateVAO();
	
	pDirtyModelRigMappings = true;
	pDirtyLODRenderTaskConfigs = true;
	
	pUpdateRenderMode();
	pResizeBoneMatrices();
	pResizeModelRigMappings();
	NotifyBoundariesChanged();
	MarkAllTexturesParamBlocksDirty();
	MarkAllDecalTexturesParamBlocksDirty();
	MarkOccMeshParamBlockDirty();
}

void deoglRComponent::SetDynamicSkin(deoglComponent &component, deoglRDynamicSkin *dynamicSkin){
	// NOTE this is called from the main thread during synchronization
	if(dynamicSkin == pDynamicSkin){
		return;
	}
	pDynamicSkin = dynamicSkin;
	// texture can use the dynamic skin we had so far for their skin state.
	// force an update to make sure everything matches up again
	const int textureCount = pTextures.GetCount();
	int i;
	for(i=0; i<textureCount; i++){
		deoglRComponentTexture &texture = *((deoglRComponentTexture*)pTextures.GetAt(i));
		texture.SetSkinState(NULL); // required since UpdateSkinState can not figure out dynamic skin changed
		texture.UpdateSkinState(component);
	}
	
	// child decals can use the dynamic skin we had so far for their skin state.
	// force an update to make sure everything matches up again
	const int decalCount = pDecals.GetCount();
	for(i=0; i<decalCount; i++){
		deoglRDecal &decal = *((deoglRDecal*)pDecals.GetAt(i));
		decal.SetSkinState(NULL); // required since UpdateSkinState can not figure out dynamic skin changed
		decal.UpdateSkinState();
	}
	
	pSkinRendered.SetDirty();
}

void deoglRComponent::SetOcclusionMesh(deoglROcclusionMesh *occlusionMesh){
	if(occlusionMesh == pOcclusionMesh){
		return;
	}
	
	if(pDynamicOcclusionMesh){
		delete pDynamicOcclusionMesh;
		pDynamicOcclusionMesh = NULL;
	}
	pOccMeshSharedSPBDoubleSided = nullptr;
	pOccMeshSharedSPBSingleSided = nullptr;
	if(pOccMeshSharedSPBElement){
		pOccMeshSharedSPBElement = NULL;
	}
	pOcclusionMesh = occlusionMesh;
	if(occlusionMesh){
		if(occlusionMesh->GetWeightsCount() > 0){
			pDynamicOcclusionMesh = new deoglDynamicOcclusionMesh(pRenderThread, pOcclusionMesh, this);
			
			// update all touching lights since their light volume potentially changes. this works even with
			// lights storing only components in their reduced extends. if a component has a new occlusion
			// mesh (or a larger one) that did not have it before the light volume can at best reduce but
			// not expand. if on the other hand the component has now no occlusion mesh (or a smaller one)
			// than before the light volumes of the affected lights can potentially expand. this works since
			// lights visit the entire full extends upon rebuilding the light volume. this catches then also
			// component occlusion meshes not included yet in their list
		//	pDirtyOctree = true;
		//	ResetStatic();
		}
	}
	
	pDirtyOccMeshVBO = true;
	pDynOccMeshRequiresPrepareForRender = true;
	
	pRequiresPrepareForRender();
	
	InvalidateOccMeshSharedSPBRTIGroup();
	NotifyOcclusionMeshChanged();
}

deoglSharedSPBRTIGroup &deoglRComponent::GetOccMeshSharedSPBRTIGroup(bool doubleSided) const{
	return doubleSided ? pOccMeshSharedSPBDoubleSided : pOccMeshSharedSPBSingleSided;
}

void deoglRComponent::DynOccMeshRequiresPrepareForRender(){
	pDynOccMeshRequiresPrepareForRender = true;
	pRequiresPrepareForRender();
}

void deoglRComponent::InvalidateOccMeshSharedSPBRTIGroup(){
	pValidOccMeshSharedSPBElement = false;
	pRequiresPrepareForRender();
	
	pWorldComputeElement->ComputeUpdateElementAndGeometries();
}

void deoglRComponent::MeshChanged(){
	pInvalidateLODVBOs();
	
	if(pDynamicOcclusionMesh){
		pDynamicOcclusionMesh->ComponentStateChanged();
		DynOccMeshRequiresPrepareForRender();
	}
}



void deoglRComponent::InitSkinStateStates(const deComponent &component){
	if(pSkinState){
		pSkinState->InitAll();
		pSkinState->MapBonesAll(component);
	}
	
	const int textureCount = pTextures.GetCount();
	int i;
	for(i=0; i<textureCount; i++){
		deoglRComponentTexture &texture = *((deoglRComponentTexture*)pTextures.GetAt(i));
		if(texture.GetSkinState()){
			texture.GetSkinState()->InitAll();
			texture.GetSkinState()->MapBonesAll(component);
		}
	}
}

void deoglRComponent::UpdateSkinStateBones(const deComponent &component){
	if(pSkinState){
		pSkinState->UpdateBonesAll(component);
	}
	
	const int textureCount = pTextures.GetCount();
	int i;
	for(i=0; i<textureCount; i++){
		deoglRComponentTexture &texture = *((deoglRComponentTexture*)pTextures.GetAt(i));
		if(texture.GetSkinState()){
			texture.GetSkinState()->UpdateBonesAll(component);
		}
	}
}

void deoglRComponent::UpdateSkinStateStates(){
	if(pSkinState){
		pSkinState->UpdateAll();
	}
	
	const int textureCount = pTextures.GetCount();
	int i;
	for(i=0; i<textureCount; i++){
		deoglRComponentTexture &texture = *((deoglRComponentTexture*)pTextures.GetAt(i));
		if(texture.GetSkinState()){
			texture.GetSkinState()->UpdateAll();
		}
	}
}



void deoglRComponent::DirtyPrepareSkinStateRenderables(){
	pDirtyPrepareSkinStateRenderables = true;
	pDirtyRenderSkinStateRenderables = true;
	pRequiresPrepareForRender();
}



void deoglRComponent::MarkOccMeshParamBlockDirty(){
	pDirtyOccMeshSharedSPBElement = true;
	pValidOccMeshSharedSPBElement = false;
	pRequiresPrepareForRender();
}

void deoglRComponent::UpdateOccmeshInstanceParamBlock(deoglShaderParameterBlock &paramBlock,
int element){
	if(!pParentWorld){
		DETHROW(deeInvalidParam);
	}
	
	const decDVector &referencePosition = pParentWorld->GetReferencePosition();
	decDMatrix matrix(pMatrix);
	matrix.a14 -= referencePosition.x;
	matrix.a24 -= referencePosition.y;
	matrix.a34 -= referencePosition.z;
	
	paramBlock.SetParameterDataMat4x3(0, element, matrix); // pMatrixModel
}



void deoglRComponent::UpdateBoneMatrices(const deComponent &component){
	const deRig * const rig = component.GetRig();
	int i;
	
	pUpdateModelRigMappings(component);
	
	for(i=0; i<pBoneMatrixCount; i++){
		oglMatrix3x4 &boneMatrix = pBoneMatrices[i];
		const int bone = pModelRigMappings[i];
		
		if(bone == -1){
			boneMatrix.a11 = 1.0f;
			boneMatrix.a12 = 0.0f;
			boneMatrix.a13 = 0.0f;
			boneMatrix.a14 = 0.0f;
			boneMatrix.a21 = 0.0f;
			boneMatrix.a22 = 1.0f;
			boneMatrix.a23 = 0.0f;
			boneMatrix.a24 = 0.0f;
			boneMatrix.a31 = 0.0f;
			boneMatrix.a32 = 0.0f;
			boneMatrix.a33 = 1.0f;
			boneMatrix.a34 = 0.0f;
			
		}else{
			const decMatrix matrix(rig->GetBoneAt(bone).GetInverseMatrix()
				.QuickMultiply(component.GetBoneAt(bone).GetMatrix()));
			
			boneMatrix.a11 = matrix.a11;
			boneMatrix.a12 = matrix.a12;
			boneMatrix.a13 = matrix.a13;
			boneMatrix.a14 = matrix.a14;
			boneMatrix.a21 = matrix.a21;
			boneMatrix.a22 = matrix.a22;
			boneMatrix.a23 = matrix.a23;
			boneMatrix.a24 = matrix.a24;
			boneMatrix.a31 = matrix.a31;
			boneMatrix.a32 = matrix.a32;
			boneMatrix.a33 = matrix.a33;
			boneMatrix.a34 = matrix.a34;
		}
	}
}

void deoglRComponent::UpdateVertexPositionSets(const deComponent &component){
	const int count = component.GetVertexPositionSetCount();
	int i;
	
	if(count != pVertexPositionSetCount){
		if(pVertexPositionSetWeights){
			delete [] pVertexPositionSetWeights;
			pVertexPositionSetWeights = nullptr;
			pVertexPositionSetCount = 0;
		}
		
		if(count > 0){
			pVertexPositionSetWeights = new float[count];
			pVertexPositionSetCount = count;
		}
	}
	
	for(i=0; i<count; i++){
		pVertexPositionSetWeights[i] = component.GetVertexPositionSetWeightAt(i);
	}
}



int deoglRComponent::GetPointOffset(int lodLevel) const{
	deoglRComponentLOD &lod = GetLODAt(lodLevel);
	
	if(lod.GetVAO()){
		return lod.GetPointOffset();
	}
	
	if(!pModel){
		return 0;
	}
	deoglSharedVBOBlock * const block = pModel->GetLODAt(lodLevel).GetVBOBlock();
	if(block){
		return block->GetOffset();
	}
	return 0;
}

int deoglRComponent::GetIndexOffset(int lodLevel) const{
	deoglRComponentLOD &lod = GetLODAt(lodLevel);
	
	if(lod.GetVAO()){
		return lod.GetIndexOffset();
	}
	
	if(!pModel){
		return 0;
	}
	deoglSharedVBOBlock * const block = pModel->GetLODAt(lodLevel).GetVBOBlock();
	if(block){
		return block->GetIndexOffset();
	}
	return 0;
}

deoglVAO *deoglRComponent::GetVAO(int lodLevel) const{
	const deoglRComponentLOD &lod = GetLODAt(lodLevel);
	if(lod.GetVAO()){
		return lod.GetVAO();
	}
	deoglSharedVBOBlock * const block = GetModel()->GetLODAt(lodLevel).GetVBOBlock();
	if(block){
		return block->GetVBO()->GetVAO();
	}
	return nullptr;
}

void deoglRComponent::InvalidateVAO(){
	const int count = pLODs.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRComponentLOD*)pLODs.GetAt(i))->InvalidateVAO();
	}
	
	pWorldComputeElement->ComputeUpdateElementAndGeometries();
}



void deoglRComponent::UpdateSkin(float elapsed){
	// WARNING called from main thread during synchronization time
	
	const int count = pTextures.GetCount();
	int i;
	
	// advance time and mark tucs/parameters dirty if dynamic channels are present
	pSkinState->AdvanceTime(elapsed);
	
	for(i=0; i<count; i++){
		//deoglRComponentTexture &texture = *( ( deoglRComponentTexture* )pTextures.GetAt( i ) );
		deoglRComponentTexture &texture = GetTextureAt(i);
		
		if(texture.GetSkinState()){
			texture.GetSkinState()->AdvanceTime(elapsed);
		}
		
		if(texture.GetUseSkinTexture()){
			if(texture.GetUseSkinTexture()->GetDynamicChannels()){
				texture.MarkParamBlocksDirty();
				texture.MarkTUCsDirty();
				
			}else if(texture.GetUseSkinTexture()->GetCalculatedProperties()
			|| texture.GetUseSkinTexture()->GetConstructedProperties()
			|| texture.GetUseSkinTexture()->GetBoneProperties()){
				texture.MarkParamBlocksDirty();
			}
		}
	}
	
	// update environment map fading
	if(pRenderEnvMapFade){
		pRenderEnvMapFadeFactor += pRenderEnvMapFadePerTime * elapsed;
		
		if(pRenderEnvMapFadeFactor >= 1.0f){
			SetRenderEnvMapFade(NULL);
			pRenderEnvMapFadeFactor = 1.0f;
		}
	}
	
	// update skin state in all attached decals
	const int decalCount = pDecals.GetCount();
	for(i=0; i<decalCount; i++){
		((deoglRDecal*)pDecals.GetAt(i))->UpdateSkin(elapsed);
	}
}



//#define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer timer;

float elapsedCompCalcVBO;
float elapsedCompBuildVBO;
float elapsedCompWeights;
float elapsedCompTVert;
float elapsedCompNorTan;
float elapsedCompNormalize;

void debugCompReset(){
	elapsedCompCalcVBO = 0.0f;
	elapsedCompBuildVBO = 0.0f;
	elapsedCompWeights = 0.0f;
	elapsedCompTVert = 0.0f;
	elapsedCompNorTan = 0.0f;
	elapsedCompNormalize = 0.0f;
}
#endif



//#include <dragengine/common/utils/decTimer.h>
//static decTimer timer;

void deoglRComponent::Update(float elapsed){
	if(pFirstRender){
		pFirstRender = false;
		pStaticSince = 0.0f;
		SetRenderStatic(pMovementHint == deComponent::emhStationary);
	}
	
	pStaticSince += elapsed;
	if(pStaticSince > 1.0f){
		SetRenderStatic(true);
	}
}



void deoglRComponent::AddSkinStateRenderPlans(deoglRenderPlan &plan){
	pSkinState->AddRenderPlans(plan);
	
	int i;
	for(i=0; i<pTextures.GetCount(); i++){
		deoglRComponentTexture &texture = *((deoglRComponentTexture*)pTextures.GetAt(i));
		if(texture.GetSkinState()){
			texture.GetSkinState()->AddRenderPlans(plan);
		}
	}
	
	pSkinRendered.AddRenderPlans(plan);
}



void deoglRComponent::SetMatrix(const decDMatrix &matrix){
	pMatrix = matrix;
	pInverseMatrix = matrix.QuickInvert();
	
	MarkAllTexturesParamBlocksDirty();
	MarkAllDecalTexturesParamBlocksDirty();
	MarkOccMeshParamBlockDirty();
	
	NotifyBoundariesChanged();
	
	/*if( pEnvMap ){
		pEnvMap->SetPosition(pComponent->GetPosition());
	}*/
}

void deoglRComponent::UpdateExtends(deComponent &component){
	if(pModel){
		const int boneCount = component.GetBoneCount();
		const deoglRModel &model = pModel;
		decDVector corners[8];
		
		if(boneCount > 0){
			component.PrepareBones();
			const decDMatrix &matrix = component.GetMatrix();
			bool extendsSet = false;
			
			if(model.GetHasWeightlessExtends()){
				corners[7].Set(model.GetWeightlessExtends().minimum);
				corners[1].Set(model.GetWeightlessExtends().maximum);
				corners[0].Set(corners[7].x, corners[1].y, corners[1].z);
				corners[2].Set(corners[1].x, corners[7].y, corners[1].z);
				corners[3].Set(corners[7].x, corners[7].y, corners[1].z);
				corners[4].Set(corners[7].x, corners[1].y, corners[7].z);
				corners[5].Set(corners[1].x, corners[1].y, corners[7].z);
				corners[6].Set(corners[1].x, corners[7].y, corners[7].z);
				int j;
				for(j=0; j<8; j++){
					corners[j] = matrix * corners[j];
					
					if(extendsSet){
						pMinExtend.SetSmallest(corners[j]);
						pMaxExtend.SetLargest(corners[j]);
						
					}else{
						pMinExtend = pMaxExtend = corners[j];
						extendsSet = true;
					}
				}
			}
			
			int i;
			for(i=0; i<pModelRigMappings.GetCount(); i++){
				if(pModelRigMappings.GetAt(i) == -1){
					continue;
				}
				
				const deoglRModel::sExtends &boneExtends = model.GetBoneExtends()[i];
				const decDMatrix boneMatrix(decDMatrix(component.GetBoneAt(
					pModelRigMappings.GetAt(i)).GetMatrix()) * matrix);
				
				corners[7].Set(boneExtends.minimum);
				corners[1].Set(boneExtends.maximum);
				corners[0].Set(corners[7].x, corners[1].y, corners[1].z);
				corners[2].Set(corners[1].x, corners[7].y, corners[1].z);
				corners[3].Set(corners[7].x, corners[7].y, corners[1].z);
				corners[4].Set(corners[7].x, corners[1].y, corners[7].z);
				corners[5].Set(corners[1].x, corners[1].y, corners[7].z);
				corners[6].Set(corners[1].x, corners[7].y, corners[7].z);
				
				int j;
				for(j=0; j<8; j++){
					corners[j] = boneMatrix * corners[j];
					
					if(extendsSet){
						pMinExtend.SetSmallest(corners[j]);
						pMaxExtend.SetLargest(corners[j]);
						
					}else{
						pMinExtend = pMaxExtend = corners[j];
						extendsSet = true;
					}
				}
			}
			
		}else{
			const decDMatrix &matrix = component.GetMatrix();
			corners[7] = decDVector(model.GetExtends().minimum);
			corners[1] = decDVector(model.GetExtends().maximum);
			corners[0].Set(corners[7].x, corners[1].y, corners[1].z);
			corners[2].Set(corners[1].x, corners[7].y, corners[1].z);
			corners[3].Set(corners[7].x, corners[7].y, corners[1].z);
			corners[4].Set(corners[7].x, corners[1].y, corners[7].z);
			corners[5].Set(corners[1].x, corners[1].y, corners[7].z);
			corners[6].Set(corners[1].x, corners[7].y, corners[7].z);
			int j;
			for(j=0; j<8; j++){
				corners[j] = matrix * corners[j];
			}
			
			pMinExtend = pMaxExtend = corners[0];
			for(j=1; j<8; j++){
				pMinExtend.SetSmallest(corners[j]);
				pMaxExtend.SetLargest(corners[j]);
			}
		}
	}
	
	const decDVector position(pMatrix.GetPosition());
	pLocalMinExtend = (pMinExtend - position).ToVector();
	pLocalMaxExtend = (pMaxExtend - position).ToVector();
	
	NotifyBoundariesChanged();
}



void deoglRComponent::SetSortDistance(float distance){
	pSortDistance = distance;
}



void deoglRComponent::SetLit(bool lit){
	pLit = lit;
}

void deoglRComponent::SetOccluded(bool occluded){
	pOccluded = occluded;
}

// void deoglRComponent::TestCameraInside( const decDVector &position ){
// 	const decVector relPos( position - pMatrix.GetPosition() );
// 	const decVector minExtend( pLocalMinExtend - decVector( 0.1f, 0.1f, 0.1f ) );
// 	const decVector maxExtend( pLocalMaxExtend + decVector( 0.1f, 0.1f, 0.1f ) );
// 	pCameraInside = ( relPos >= minExtend && relPos <= maxExtend );
// }

void deoglRComponent::SetRenderMode(eRenderModes renderMode){
	if(renderMode == pRenderMode){
		return;
	}
	
	pRenderMode = renderMode;
	pDirtyLODVBOs = true;
	pRequiresPrepareForRender();
	
	if(pMovementHint == deComponent::emhStationary){
		NotifySkiesUpdateStatic();
	}
}



void deoglRComponent::DirtySolid(){
	pDirtySolid = true;
	pRequiresPrepareForRender();
}

void deoglRComponent::SetRenderStatic(bool isStatic){
	if(isStatic == pRenderStatic){
		return;
	}
	
	pRenderStatic = isStatic;
	
	const int lightCount = pLightList.GetCount();
	int i;
	for(i=0; i<lightCount; i++){
		deoglRLight &light = *pLightList.GetAt(i);
		light.RemoveComponent(this);
		light.AddComponent(this);
	}
	
	NotifyRenderStaticChanged();
// 	NotifySkiesUpdateStatic();
	
	pWorldComputeElement->ComputeUpdateElement();
	
	const int decalCount = pDecals.GetCount();
	for(i=0; i<decalCount; i++){
		((deoglRDecal*)pDecals.GetAt(i))->UpdateWorldCompute();
	}
}

void deoglRComponent::ResetRenderStatic(){
	pStaticSince = 0.0f;
	SetRenderStatic(false);
}

bool deoglRComponent::IsGIStatic() const{
	return pMovementHint == deComponent::emhStationary && pRenderMode == deoglRComponent::ermStatic;
}



void deoglRComponent::SetRenderEnvMap(deoglEnvironmentMap *envmap){
	// note about the switch process. we have to wait setting the fading environment map until the
	// new environment map has been set. if this is not done the SetRenderEnvMapFade function tries
	// to add the component to the component list of the same environment map as the current one
	// resulting in an exception. to avoid this the fade environment map is first cleared, then
	// the environment set and then the fade environment map is set again
	// 
	// another note. if a new environment map is set while the fading is still in progress a hard
	// switch may still occur. a possible solution would be to delay the switch until the fading
	// is finished. for this we would have to keep the dirty flag set, which is currently set
	// outside somewhere
	if(envmap == pRenderEnvMap){
		return;
	}
	
	const deoglEnvironmentMap::Ref guard(pRenderEnvMap);
	
	if(pRenderEnvMap){
		pRenderEnvMap->GetComponentList().RemoveIfExisting(this);
	}
	
	pRenderEnvMap = envmap;
	
	if(envmap){
		envmap->GetComponentList().Add(this);
	}
	
	// now it is safe to set the fade env map
	SetRenderEnvMapFade(guard);
	pRenderEnvMapFadeFactor = 0.0f;
	
	if(!guard){ // in case SetRenderEnvMapFade did not mark all textures dirty yet
		MarkAllTexturesTUCsDirtyEnvMapUse();
	}
}

void deoglRComponent::SetRenderEnvMapFade(deoglEnvironmentMap *envmap){
	if(envmap == pRenderEnvMapFade){
		return;
	}
	
	if(pRenderEnvMapFade){
		pRenderEnvMapFade->GetComponentList().RemoveIfExisting(this);
	}
	
	pRenderEnvMapFade = envmap;
	
	if(envmap){
		envmap->GetComponentList().Add(this);
	}
	
	MarkAllTexturesTUCsDirtyEnvMapUse();
}

void deoglRComponent::SetRenderEnvMapFadePerTime(float fadePerTime){
	if(fadePerTime < 0.1f){
		pRenderEnvMapFadePerTime = 0.1f;
		
	}else{
		pRenderEnvMapFadePerTime = fadePerTime;
	}
}

void deoglRComponent::SetRenderEnvMapFadeFactor(float factor){
	if(factor < 0.0f){
		factor = 0.0f;
		
	}else if(factor > 1.0f){
		factor = 1.0f;
	}
	
	if(fabsf(factor - pRenderEnvMapFadeFactor) > 0.001f){
		pRenderEnvMapFadeFactor = factor;
		
	}
}

void deoglRComponent::WorldEnvMapLayoutChanged(){
	pDirtyRenderEnvMap = true;
	pRequiresPrepareForRender();
}

void deoglRComponent::InvalidateRenderEnvMap(){
	if(!pRenderEnvMap && !pRenderEnvMapFade){
		return;
	}
	
	SetRenderEnvMap(nullptr);
	SetRenderEnvMapFade(nullptr);
	pDirtyRenderEnvMap = true;
	
	pRequiresPrepareForRender();
}

void deoglRComponent::InvalidateRenderEnvMapIf(deoglEnvironmentMap *envmap){
	if(pRenderEnvMap == envmap || pRenderEnvMapFade == envmap){
		InvalidateRenderEnvMap();
	}
}



void deoglRComponent::WorldReferencePointChanged(){
	MarkAllTexturesParamBlocksDirty();
	MarkAllDecalTexturesParamBlocksDirty();
	MarkOccMeshParamBlockDirty();
}



// #define DO_PFR_TIMING

#ifdef DO_PFR_TIMING
#define PFRT_INIT decTimer timer;
#define PFRT_SAMPLE(n) const float time##n = timer.GetElapsedTime();
#define PFRT_FINAL(m,...) if(pModel) printf("PrepareForRender %s: " m "\n", pModel->GetFilename().GetString(), __VA_ARGS__);
#else
#define PFRT_INIT
#define PFRT_SAMPLE(n)
#define PFRT_FINAL(...)
#endif

void deoglRComponent::PrepareForRender(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask){
	/*
	if(pDirtyModelVBOs|pDirtyOccMeshVBO|pDirtyOccMeshSharedSPBElement|pDirtyLODVBOs
	|pDirtyLODRenderTaskConfigs|pDirtyModelRigMappings|pDirtySolid|pDirtyPrepareSkinStateRenderables
	|pDirtyTextureTUCs|pDirtyTextureParamBlocks|pDirtyDecals|pDirtyCulling|pDirtyRenderEnvMap){
		printf("RComponent.PrepareForRender %s:%s%s%s%s%s%s%s%s%s%s%s%s%s\n", pModel->GetFilename().GetString(),
		pDirtyModelVBOs?" dirtyModelVBOs":"", pDirtyOccMeshVBO?" dirtyOccMeshVBO":"",
		pDirtyOccMeshSharedSPBElement?" dirtyOccMeshSharedSPBElement":"", pDirtyLODVBOs?" dirtyLODVBOs":"",
		pDirtyLODRenderTaskConfigs?" dirtyLODRenderTaskConfigs":"",
		pDirtyModelRigMappings?" pDirtyModelRigMappings":"", pDirtySolid?" dirtySolid":"",
		pDirtyPrepareSkinStateRenderables?" dirtyPrepareSkinStateRenderables":"",
		pDirtyTextureTUCs?" dirtyTextureTUCs":"", pDirtyTextureParamBlocks?" dirtyTextureParamBlocks":"",
		pDirtyDecals?" dirtyDecals":"", pDirtyCulling?" dirtyCulling":"", pDirtyRenderEnvMap?" dirtyRenderEnvMap":"");
	}
	*/
	
	PFRT_INIT
	
	pPrepareModelVBOs(); PFRT_SAMPLE(ModelVBOs)
	pPrepareLODVBOs(); PFRT_SAMPLE(LODVBOs)
	pPrepareRenderEnvMap(); PFRT_SAMPLE(RenderEnvMap)
	
	pCheckRenderModifier(plan.GetCamera());
	pPrepareSkinStateConstructed(); PFRT_SAMPLE(SkinStateConstructed)
	pPrepareSkinStateRenderables(mask); PFRT_SAMPLE(SkinStateRenderables)
	pPrepareSolidity(); PFRT_SAMPLE(Solidity)
	
	pPrepareParamBlocks(); PFRT_SAMPLE(ParamBlocks)
	pPrepareTextureTUCs(); PFRT_SAMPLE(TextureTUCs)
	pPrepareTextureParamBlocks(); PFRT_SAMPLE(TextureParamBlocks) // has to come after pPrepareTextureTUCs
	
	pPrepareOccMeshVBO(); PFRT_SAMPLE(OccMeshVBO)
	pPrepareDynOccMesh(); PFRT_SAMPLE(DynOccMesh)
	pPrepareOccMeshRTSInstances(); PFRT_SAMPLE(OccMeshRTSInstances) // requires (dyn) occmesh VBOs to be prepared
	
	pPrepareLODRenderTaskConfigs(); PFRT_SAMPLE(LODRenderTaskConfigs)
	
	pPrepareDecals(plan, mask); PFRT_SAMPLE(Decals)
	
	PFRT_FINAL("mv=%dys lv=%dys rem=%dys ssr=%dys s=%dys pb=%dys tt=%dys tpb=%dys omv=%dys dom=%dys omri=%dys lrtc=%dys d=%dys ssc=%dys",
		(int)(timeModelVBOs*1e6f), (int)(timeLODVBOs*1e6f), (int)(timeRenderEnvMap*1e6f),
		(int)(timeSkinStateRenderables*1e6f), (int)(timeSolidity*1e6f), (int)(timeParamBlocks*1e6f),
		(int)(timeTextureTUCs*1e6f), (int)(timeTextureParamBlocks*1e6f), (int)(timeOccMeshVBO*1e6f),
		(int)(timeDynOccMesh*1e6f), (int)(timeOccMeshRTSInstances*1e6f),
		(int)(timeLODRenderTaskConfigs*1e6f), (int)(timeDecals*1e6f), (int)(timeSkinStateConstructed*1e6f))
// 	if(pModel) printf("RComponent.PrepareForRender %s %dys\n", pModel->GetFilename().GetString(), (int)(timer.GetElapsedTime()*1e6f));
}

void deoglRComponent::PrepareForRenderRender(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask){
	pRenderSkinStateRenderables(mask);
	pPrepareDecalsRenderRenderables(plan, mask);
}



void deoglRComponent::PrepareQuickDispose(){
	NotifyComponentDestroyed(); // important, otherwise listeners segfault
	pListeners.RemoveAll();
	
	pParentWorld = NULL;
	pOctreeNode = NULL;
	
	pLightList.RemoveAll();
	
	const int count = pDecals.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglRDecal*)pDecals.GetAt(i))->PrepareQuickDispose();
	}
}



// LODs
/////////

int deoglRComponent::GetLODCount() const{
	return pLODs.GetCount();
}

deoglRComponentLOD &deoglRComponent::GetLODAt(int index) const{
	return *((deoglRComponentLOD*)pLODs.GetAt(index < 0 ? pLODs.GetCount() + index : index));
}

void deoglRComponent::RemoveAllLODs(){
	pLODs.RemoveAll();
}

void deoglRComponent::AddLOD(deoglRComponentLOD *lod){
	pLODs.Add(lod);
}

void deoglRComponent::SetLODErrorScaling(float errorScaling){
	pLODErrorScaling = errorScaling;
}

void deoglRComponent::DirtyLODVBOs(){
	pDirtyLODVBOs = true;
	pRequiresPrepareForRender();
}

void deoglRComponent::DirtyLODRenderTaskConfigs(){
	pDirtyLODRenderTaskConfigs = true;
	pRequiresPrepareForRender();
}

void deoglRComponent::UpdateRTSInstances(){
	const int count = pTextures.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->UpdateRTSInstances();
	}
}



// Textures
/////////////

int deoglRComponent::GetTextureCount() const{
	return pTextures.GetCount();
}

deoglRComponentTexture &deoglRComponent::GetTextureAt(int index) const{
	return *((deoglRComponentTexture*)pTextures.GetAt(index));
}

void deoglRComponent::RemoveAllTextures(){
	pTextures.RemoveAll();
	pOutlineTextureCount = 0;
}

void deoglRComponent::AddTexture(deoglRComponentTexture *texture){
	pTextures.Add(texture);
}

void deoglRComponent::InvalidateAllTexturesParamBlocks(){
	const int count = pTextures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->InvalidateParamBlocks();
	}
}

void deoglRComponent::MarkAllTexturesParamBlocksDirty(){
	const int count = pTextures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->MarkParamBlocksDirty();
	}
}

void deoglRComponent::MarkAllTexturesTUCsDirty(){
	const int count = pTextures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->MarkTUCsDirty();
	}
}

void deoglRComponent::MarkAllTexturesTUCsDirtyEnvMapUse(){
	const int count = pTextures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->MarkTUCsDirtyEnvMapUse();
	}
}

void deoglRComponent::UpdateStaticTextures(){
	pStaticTextures = true;
	
	if(!pModel){
		return;
	}
	
	const int count = pTextures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglRComponentTexture &texture = *((deoglRComponentTexture*)pTextures.GetAt(i));
		deoglSkinState * const skinState = texture.GetUseSkinState();
		if(!skinState){
			continue;
		}
		
		if(skinState->GetVideoPlayerCount() > 0
		|| skinState->GetCalculatedPropertyCount() > 0
		|| skinState->GetConstructedPropertyCount() > 0){
			pStaticTextures = false;
			break;
		}
	}
}

void deoglRComponent::DynamicSkinRenderablesChanged(){
	if(!pDynamicSkin || !pSkin || !pSkin->GetHasRenderables()){
		return;
	}
	
	// mark all textures dirty that could be affected by the update. textures are safe if they have a
	// custom texture assigned or if the corresponding skin texture (if existing) is not marked as dynamic.
	// later on this will be made better by usin notifications so this check is automatic since only
	// textures listening for dynamic changes are affected.
	const int textureCount = pTextures.GetCount();
	int i;
	
	for(i=0; i<textureCount; i++){
		deoglRComponentTexture &texture = *((deoglRComponentTexture*)pTextures.GetAt(i));
		if(texture.GetSkin() || !pSkin){
			continue;
		}
		
		const int mapping = pModelSkinMappings.GetAt(i);
		if(mapping == -1){
			continue;
		}
		
		const deoglSkinTexture &skinTexture = pSkin->GetTextureAt(mapping);
		if(!skinTexture.GetDynamicChannels()){
			continue;
		}
		
		texture.MarkParamBlocksDirty();
		texture.MarkTUCsDirty();
	}
}

void deoglRComponent::TextureDynamicSkinRenderablesChanged(deoglRComponentTexture &texture){
	if(!texture.GetUseSkinState() || !texture.GetUseDynamicSkin()){
		return;
	}
	
	texture.MarkParamBlocksDirty();
	texture.MarkTUCsDirty();
}

void deoglRComponent::UpdateRenderableMapping(){
	// udpate mappings of dynamic skin of component itself
	pSkinState->RemoveAllRenderables();
	if(pSkin && pDynamicSkin){
		pSkinState->AddRenderables(*pSkin, *pDynamicSkin);
	}
	
	// update mappings of dynamic skins of component textures is done in the texture
	// SyncToRender call since otherwise the mapping is updated before the skin and
	// dynamic skin in the texture have been potentially updated
	
	// setting skin rendered dirty here is fine
	pSkinRendered.SetDirty();
	
	// textures will mark their param blocks and tucs dirty if they need to update renderable mappings
}

void deoglRComponent::UpdateTexturesUseSkin(){
	const int count = pTextures.GetCount();
	int i;
	
	pOutlineTextureCount = 0;
	
	for(i=0; i<count; i++){
		deoglRComponentTexture &texture = *(deoglRComponentTexture*)pTextures.GetAt(i);
		texture.UpdateUseSkin();
		if(texture.GetUseSkinTexture() && texture.GetUseSkinTexture()->GetHasOutline()){
			pOutlineTextureCount++;
		}
	}
	
	pWorldComputeElement->ComputeUpdateElementAndGeometries();
}

void deoglRComponent::DirtyTextureTUCs(){
	if(pDirtyTextureTUCs){
		return;
	}
	
	pDirtyTextureTUCs = true;
	pRequiresPrepareForRender();
	
	pWorldComputeElement->ComputeUpdateElementGeometries();
}

void deoglRComponent::DirtyTextureParamBlocks(){
	if(pDirtyTextureParamBlocks){
		return;
	}
	
	pDirtyTextureParamBlocks = true;
	pRequiresPrepareForRender();
}



// Decals
///////////

int deoglRComponent::GetDecalCount() const{
	return pDecals.GetCount();
}

deoglRDecal *deoglRComponent::GetDecalAt(int index) const{
	return (deoglRDecal*)pDecals.GetAt(index);
}

void deoglRComponent::SyncDecalReferences(const deComponent &engComponent){
	// clear parent component on all decals marked removed. this invalidates decals and
	// forces rebuilding the decal should they be added somewhere else
	const int count = pDecals.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglRDecal &decal = *((deoglRDecal*)pDecals.GetAt(i));
		if(decal.GetComponentMarkedRemove()){
			decal.SetParentComponent(NULL);
		}
	}
	
	// clear the list and add decals from component. sets the parent component on all decals.
	// this will trigger a rebuild of the decal only if the parent component changed
	pDecals.RemoveAll();
	
	deDecal *engDecal = engComponent.GetRootDecal();
	while(engDecal){
		deoglRDecal * const decal = ((deoglDecal*)engDecal->GetPeerGraphic())->GetRDecal();
		pDecals.Add(decal);
		decal->SetComponentMarkedRemove(false);
		decal->SetParentComponent(this);
		engDecal = engDecal->GetLLComponentNext();
	}
	
	if(pDecals.GetCount() > 0){
		DecalRequiresPrepareForRender();
	}
}

void deoglRComponent::MarkAllDecalTexturesParamBlocksDirty(){
	const int count = pDecals.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRDecal*)pDecals.GetAt(i))->MarkParamBlocksDirty();
	}
}

void deoglRComponent::DecalRequiresPrepareForRender(){
	pDirtyDecals = true;
	pDirtyDecalsRenderRenderables = true;
	pRequiresPrepareForRender();
}



// Culling
////////////

const decDVector &deoglRComponent::GetCullSphereCenter(){
	if(pDirtyCulling){
		pUpdateCullSphere();
		pDirtyCulling = false;
	}
	
	return pCullSphereCenter;
}

float deoglRComponent::GetCullSphereRadius(){
	if(pDirtyCulling){
		pUpdateCullSphere();
		pDirtyCulling = false;
	}
	
	return pCullSphereRadius;
}

void deoglRComponent::SetDirtyCulling(){
	pDirtyCulling = true;
}



// Notifiers
//////////////

void deoglRComponent::AddListener(deoglComponentListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void deoglRComponent::RemoveListener(deoglComponentListener *listener){
	const int index = pListeners.IndexOf(listener);
	if(index == -1){
		return;
	}
	
	pListeners.Remove(listener);
	
	if(pListenerIndex >= index){
		pListenerIndex--;
	}
}

void deoglRComponent::NotifyBoundariesChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->BoundariesChanged(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyComponentDestroyed(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->ComponentDestroyed(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyParentWorldChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		try{ // temp hack
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->ParentWorldChanged(*this);
		}catch(const deException &e){ // temp hack
			pRenderThread.GetLogger().LogException(e);
		}
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyLayerMaskChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->LayerMaskChanged(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyRenderStaticChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->RenderStaticChanged(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyOcclusionMeshChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->OcclusionMeshChanged(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyTexturesChanged(){
	// TODO works in games but not in the editor since it changes textures after adding it
	//      to the game world. maybe add a static timer like for render static?
// 	pStaticTextures = false;
	
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->TexturesChanged(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyTUCChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->TUCChanged(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyMovementHintChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->MovementHintChanged(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyGIImportanceChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->GIImportanceChanged(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyLightsDirtyLightVolume(){
	if(!pRenderStatic || !pOcclusionMesh){
		return;
	}
	
	const int lightCount = pLightList.GetCount();
	int i;
	
	for(i=0; i<lightCount; i++){
		pLightList.GetAt(i)->SetLightVolumeDirty();
	}
}

void deoglRComponent::NotifySkiesUpdateStatic(){
	if(pParentWorld && !pFirstRender){
		pParentWorld->SkiesNotifyUpdateStaticComponent(this);
	}
}

void deoglRComponent::NotifyVisibilityChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->VisibilityChanged(*this);
		pListenerIndex++;
	}
}

void deoglRComponent::NotifyModelChanged(){
	pListenerIndex = 0;
	while(pListenerIndex < pListeners.GetCount()){
		((deoglComponentListener*)pListeners.GetAt(pListenerIndex))->ModelChanged(*this);
		pListenerIndex++;
	}
}


// Render world usage
///////////////////////

void deoglRComponent::SetWorldMarkedRemove(bool marked){
	pWorldMarkedRemove = marked;
}

void deoglRComponent::SetLLWorldPrev(deoglRComponent *component){
	pLLWorldPrev = component;
}

void deoglRComponent::SetLLWorldNext(deoglRComponent *component){
	pLLWorldNext = component;
}



// Private Functions
//////////////////////

void deoglRComponent::pCleanUp(){
	NotifyComponentDestroyed();
	
	SetParentWorld(NULL);
	
	RemoveAllTextures();
	RemoveAllLODs();
	
	pRemoveFromAllLights();
	
	pListeners.RemoveAll();
	
	if(pDynamicOcclusionMesh){
		delete pDynamicOcclusionMesh;
	}
	
	if(pBoneMatrices){
		delete [] pBoneMatrices;
	}
	
	pOccMeshSharedSPBElement = nullptr; // has to be done before pOcclusionMesh mesh is released
	if(pSkinState){
		delete pSkinState;
	}
	if(pVertexPositionSetWeights){
		delete [] pVertexPositionSetWeights;
	}
	
	pRenderThread.GetUniqueKey().Return(pUniqueKey);
}



void deoglRComponent::pInvalidateLODVBOs(){
	const int count = pLODs.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglRComponentLOD*)pLODs.GetAt(i))->InvalidateVBO();
	}
}



void deoglRComponent::pUpdateModelSkinMappings(){
	if(!pModel || !pSkin){
		const int count = pModelSkinMappings.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pModelSkinMappings.SetAt(i, -1);
		}
		return;
	}
	
	// determine mappings of textures from model to skin
	const decStringList &textureNames = pModel->GetTextureNames();
	const int count = pModelSkinMappings.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		pModelSkinMappings.SetAt(i, -1);
		if(!pSkin){
			continue;
		}
		
		const decString &name = textureNames.GetAt(i);
		for(j=0; j<pSkin->GetTextureCount(); j++){
			if(pSkin->GetTextureAt(j).GetName() == name){
				pModelSkinMappings.SetAt(i, j);
				break;
			}
		}
	}
	
	// mark all textures dirty
	pSkinRendered.SetDirty();
	InvalidateAllTexturesParamBlocks();
	MarkAllTexturesTUCsDirty();
	
	// hack
	/*if( pSkin ){
		if(pEnvMap){
			if(pParentWorld){
				pParentWorld->RemoveEnvMap(pEnvMap);
			}
			delete pEnvMap;
			pEnvMap = NULL;
		}
		
		pDirtyRenderEnvMap = true;
	}*/
}

void deoglRComponent::pResizeModelSkinMappings(){
	int count = 0;
	if(pModel){
		count = pModel->GetTextureNames().GetCount();
	}
	
	if(count == pModelSkinMappings.GetCount()){
		return;
	}
	
	pModelSkinMappings.RemoveAll();
	int i;
	for(i=0; i<count; i++){
		pModelSkinMappings.Add(-1);
	}
}

void deoglRComponent::pUpdateModelRigMappings(const deComponent &component){
	if(!pDirtyModelRigMappings){
		return;
	}
	
	pDirtyModelRigMappings = false;
	
	const deRig * const rig = component.GetRig();
	const int count = pModelRigMappings.GetCount();
	int i;
	
	if(!pModel || !rig){
		for(i=0; i<count; i++){
			pModelRigMappings.SetAt(i, -1);
		}
		return;
	}
	
	for(i=0; i<count; i++){
		pModelRigMappings.SetAt(i, rig->IndexOfBoneNamed(pModel->GetBoneNames().GetAt(i)));
	}
}

void deoglRComponent::pResizeModelRigMappings(){
	int count = 0;
	if(pModel){
		count = pModel->GetBoneNames().GetCount();
	}
	
	if(count == pModelRigMappings.GetCount()){
		return;
	}
	
	pModelRigMappings.RemoveAll();
	int i;
	for(i=0; i<count; i++){
		pModelRigMappings.Add(-1);
	}
	
	pDirtyModelRigMappings = true;
}

void deoglRComponent::pCheckRenderModifier(deoglRCamera *rcamera){
	// TODO implement modifiers in a proper way
#if 0
	int i, count = pComponent->GetModifierCount();
	int renModIndex, renderModifierCount = 0;
	deComponentModifier *modifier;
	deoglRSkin *skin;
	deSkin *finalSkin = pComponent->GetSkin();
	
	// determine the skin to use
	if(rcamera){
		const deCamera &camera = rcamera->GetCamera()->GetCamera();
		renderModifierCount = camera.GetRenderModifierCount();
		
		for(i=0; i<count; i++){
			modifier = pComponent->GetModifierAt(i);
			renModIndex = modifier->GetModifierIndex();
			if(renModIndex < renderModifierCount && camera.GetRenderModifierEnabledAt(renModIndex)){
				if(modifier->GetSkin()){
					finalSkin = modifier->GetSkin();
				}
			}
		}
	}
	
	// if the skin changed update the mappings
	skin = ((deoglSkin*)finalSkin->GetPeerGraphic())->GetRSkin();
	
	if(pSkin != skin){
		pSkin = skin;
		pUpdateModelSkinMappings();
	}
#endif
}

void deoglRComponent::pUpdateRenderMode(){
	if(!pModel){
		SetRenderMode(ermStatic);
		return;
	}
	
	const deoglModelLOD &modelLOD = pModel->GetLODAt(0);
	
	if(pModel->GetVPSNames().GetCount() > 0){
		SetRenderMode(ermDynamic);
		
	}else if(modelLOD.GetWeightsEntryCount() > 0 && pModel->GetBoneCount() > 0){
		if(pBoneMatrixCount == 0){
			SetRenderMode(ermStatic);
			
		}else{
			SetRenderMode(ermDynamic);
		}
		
	}else{
		SetRenderMode(ermStatic);
	}
}

void deoglRComponent::pUpdateCullSphere(){
	deoglDCollisionSphere sphere;
	deoglDCollisionBox box;
	
	box.SetFromExtends(pMinExtend, pMaxExtend);
	box.GetEnclosingSphere(&sphere);
	
	pCullSphereCenter = sphere.GetCenter();
	pCullSphereRadius = (float)sphere.GetRadius();
}



void deoglRComponent::pPrepareSolidity(){
	if(!pDirtySolid){
		return;
	}
	pDirtySolid = false;
	
	pSolid = true;
	pOutlineSolid = true;
	pXRaySolid = true;
	
	if(!pModel){
		return;
	}
	
	const int textureCount = pTextures.GetCount();
	int i;
	
	if(pSkin){
		// if a skin is assigned check each texture using the components for transparency
		for(i=0; i<textureCount; i++){
			deoglRComponentTexture &texture = *((deoglRComponentTexture*)pTextures.GetAt(i));
			deoglRSkin *skin = texture.GetSkin();
			int textureNumber = 0;
			
			if(skin && skin->GetTextureCount() == 0){
				skin = NULL;
			}
			
			if(!skin){
				const int mapping = pModelSkinMappings.GetAt(i);
				if(mapping != -1){
					textureNumber = mapping;
					skin = pSkin;
				}
			}
			
			if(!skin){
				continue;
			}
			
			const deoglSkinTexture &skinTexture = skin->GetTextureAt(textureNumber);
			pSolid &= skinTexture.GetSolid();
			pOutlineSolid &= skinTexture.GetIsOutlineSolid();
			pXRaySolid &= !skinTexture.GetXRay() || skinTexture.GetSolid();
		}
		
	}else{
		// if no skin is assigned the component can only be transparent
		// if one or more assigned texture skins are transparent
		for(i =0; i <textureCount; i++){
			deoglRComponentTexture &texture = *((deoglRComponentTexture*)pTextures.GetAt(i));
			const deoglRSkin *skin = texture.GetSkin();
			if(!skin || skin->GetTextureCount() == 0){
				continue;
			}
			
			const deoglSkinTexture &skinTexture = skin->GetTextureAt(0);
			pSolid &= skinTexture.GetSolid();
			pOutlineSolid &= skinTexture.GetIsOutlineSolid();
			pXRaySolid &= !skinTexture.GetXRay() || skinTexture.GetSolid();
		}
	}
}

void deoglRComponent::pPrepareModelVBOs(){
	if(!pDirtyModelVBOs){
		return;
	}
	pDirtyModelVBOs = false;
	
	if(!pModel){
		return;
	}
	
	const deoglRModel &model = pModel;
	const int count = model.GetLODCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglModelLOD &modelLOD = model.GetLODAt(i);
		
		modelLOD.PrepareVBOBlock();
		modelLOD.PrepareVBOBlockVertPosSet();
		
		switch(pRenderThread.GetChoices().GetGPUTransformVertices()){
		case deoglRTChoices::egputvAccurate:
			
		case deoglRTChoices::egputvApproximate:
			modelLOD.PrepareVBOBlockWithWeight();
			break;
			
		default:
			break;
		}
	}
}

void deoglRComponent::pPrepareLODVBOs(){
	if(!pDirtyLODVBOs){
		return;
	}
	pDirtyLODVBOs = false;
	
	const int count = pLODs.GetCount();
	int i;
	
	if(pRenderMode == ermDynamic){
		for(i=0; i<count; i++){
			((deoglRComponentLOD*)pLODs.GetAt(i))->UpdateVBO();
		}
		
	}else{
		for(i=0; i<count; i++){
			((deoglRComponentLOD*)pLODs.GetAt(i))->FreeVBO();
		}
	}
}

void deoglRComponent::pPrepareLODRenderTaskConfigs(){
	if(!pDirtyLODRenderTaskConfigs){
		return;
	}
	pDirtyLODRenderTaskConfigs = false;
	
	const int count = pLODs.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglRComponentLOD*)pLODs.GetAt(i))->UpdateRenderTaskConfigurations();
	}
}

void deoglRComponent::pPrepareRenderEnvMap(){
	if(!pDirtyRenderEnvMap){
		return;
	}
	pDirtyRenderEnvMap = false;
	
	if(deoglSkinShader::REFLECTION_TEST_MODE == deoglSkinShader::ertmSingleBlenderEnvMap){
		return;
	}
	
	DEASSERT_NOTNULL(pParentWorld)
	
	// for the time being we simply pick the environment map that is closest to the component position.
	// this can lead to wrong picks and harshly switching environment maps but this is enough for the
	// first test.
	// 
	// for the time being we choose no region but visit all environment maps existing in the world.
	// a more optimal solution would be to search in an area somewhat larger than the distance to the
	// currently used environment map. the new environment map has to be closer than the current one
	// to be better. if no environment map exists yet a full search could be done.
	// 
	// better solutions could be using grid like connection between environment maps. this way the next
	// best environment map can be searched by following the grid.
	// 
	// for the time being the center of the bounding box is used as the reference point. for large
	// components we've got the problem of what environment map to use. this though has to be solved
	// later on most probably by splitting up the component into smaller components.
	deoglFindBestEnvMap visitor;
	decDVector position;
	
	position = (pMinExtend + pMaxExtend) * 0.5;
	
	visitor.SetPosition(position);
	//pParentWorld->VisitRegion( pMinExtend, pMaxExtend, visitor );
	visitor.VisitList(pParentWorld->GetEnvMapList());
	
	if(visitor.GetEnvMap()){
		SetRenderEnvMap(visitor.GetEnvMap());
		
	}else if(pParentWorld->GetSkyEnvironmentMap()){
		SetRenderEnvMap(pParentWorld->GetSkyEnvironmentMap());
		
	}else{
		SetRenderEnvMap(nullptr);
		SetRenderEnvMapFade(nullptr);
		pRenderEnvMapFadeFactor = 1.0f;
	}
	//pOgl->LogInfoFormat( "update component %p render env map %p\n", pComponent, pRenderEnvMap );
}

void deoglRComponent::pPrepareSkinStateRenderables(const deoglRenderPlanMasked *plan){
	if(!pDirtyPrepareSkinStateRenderables){
		return;
	}
	pDirtyPrepareSkinStateRenderables = false;
	pDirtyRenderSkinStateRenderables = true;
	
	if(pSkinState){
		pSkinState->PrepareRenderables(pSkin, pDynamicSkin, plan);
	}
	
	const int textureCount = pTextures.GetCount();
	int i;
	for(i=0; i<textureCount; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->PrepareSkinStateRenderables(plan);
	}
}

void deoglRComponent::pRenderSkinStateRenderables(const deoglRenderPlanMasked *plan){
	if(!pDirtyRenderSkinStateRenderables){
		return;
	}
	pDirtyRenderSkinStateRenderables = false;
	
	if(pSkinState){
		pSkinState->RenderRenderables(pSkin, pDynamicSkin, plan);
	}
	
	const int textureCount = pTextures.GetCount();
	int i;
	for(i=0; i<textureCount; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->RenderSkinStateRenderables(plan);
	}
}

void deoglRComponent::pPrepareSkinStateConstructed(){
	if(pSkinState){
		pSkinState->PrepareConstructedProperties();
	}
	
	const int textureCount = pTextures.GetCount();
	int i;
	for(i=0; i<textureCount; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->PrepareSkinStateConstructed();
	}
}

void deoglRComponent::pPrepareTextureTUCs(){
	if(!pDirtyTextureTUCs){
		return;
	}
	
	pDirtyTextureTUCs = false;
	
	const int count = pTextures.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->PrepareTUCs();
	}
}

void deoglRComponent::pPrepareParamBlocks(){
	if(!pValidOccMeshSharedSPBElement){
		// shared spb
		if(pOccMeshSharedSPBElement){
			pOccMeshSharedSPBElement = NULL;
		}
		
		if(pRenderThread.GetChoices().GetSharedSPBUseSSBO()){
			pOccMeshSharedSPBElement = pRenderThread.GetBufferObject()
				.GetSharedSPBList(deoglRTBufferObject::esspblOccMeshInstanceSSBO).AddElement();
			
		}else{
			if(pRenderThread.GetChoices().GetGlobalSharedSPBLists()){
				pOccMeshSharedSPBElement = pRenderThread.GetBufferObject()
					.GetSharedSPBList(deoglRTBufferObject::esspblOccMeshInstanceUBO).AddElement();
				
			}else if(pOcclusionMesh){
				pOccMeshSharedSPBElement = pOcclusionMesh->GetSharedSPBListUBO().AddElement();
			}
		}
		
		// shared spb render task instance group
		pOccMeshSharedSPBDoubleSided = nullptr;
		pOccMeshSharedSPBSingleSided = nullptr;
		
		if(pOccMeshSharedSPBElement && pOcclusionMesh){
			deoglSharedSPBRTIGroupList &listDouble = pOcclusionMesh->GetRTIGroupDouble();
			pOccMeshSharedSPBDoubleSided = listDouble.GetWith(pOccMeshSharedSPBElement->GetSPB());
			if(!pOccMeshSharedSPBDoubleSided){
				pOccMeshSharedSPBDoubleSided.TakeOver(listDouble.AddWith(pOccMeshSharedSPBElement->GetSPB()));
				pOccMeshSharedSPBDoubleSided->GetRTSInstance()->SetSubInstanceSPB(&pOccMeshSharedSPBElement->GetSPB());
			}
			
			deoglSharedSPBRTIGroupList &listSingle = pOcclusionMesh->GetRTIGroupsSingle();
			pOccMeshSharedSPBSingleSided = listSingle.GetWith(pOccMeshSharedSPBElement->GetSPB());
			if(!pOccMeshSharedSPBSingleSided){
				pOccMeshSharedSPBSingleSided.TakeOver(listSingle.AddWith(pOccMeshSharedSPBElement->GetSPB()));
				pOccMeshSharedSPBSingleSided->GetRTSInstance()->SetSubInstanceSPB(&pOccMeshSharedSPBElement->GetSPB());
			}
			
			UpdateRTSInstances();
		}
		
		pDirtyOccMeshSharedSPBElement = true;
		pValidOccMeshSharedSPBElement = true;
	}
	
	if(pDirtyOccMeshSharedSPBElement){
		if(pOccMeshSharedSPBElement){
			UpdateOccmeshInstanceParamBlock(deoglSharedSPBElementMapBuffer(*pOccMeshSharedSPBElement),
				pOccMeshSharedSPBElement->GetIndex());
		}
		
		pDirtyOccMeshSharedSPBElement = false;
	}
}

void deoglRComponent::pPrepareTextureParamBlocks(){
	if(!pDirtyTextureParamBlocks){
		return;
	}
	
	pDirtyTextureParamBlocks = false;
	
	const int count = pTextures.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglRComponentTexture*)pTextures.GetAt(i))->PrepareParamBlocks();
	}
}

void deoglRComponent::pPrepareDecals(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask){
	if(!pDirtyDecals){
		return;
	}
	pDirtyDecals = false;
	pDirtyDecalsRenderRenderables = true;
	
	const int count = pDecals.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglRDecal*)pDecals.GetAt(i))->PrepareForRender(plan, mask);
	}
}

void deoglRComponent::pPrepareDecalsRenderRenderables(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask){
	if(!pDirtyDecalsRenderRenderables){
		return;
	}
	pDirtyDecalsRenderRenderables = false;
	
	const int count = pDecals.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglRDecal*)pDecals.GetAt(i))->PrepareForRenderRender(plan, mask);
	}
}

void deoglRComponent::pPrepareOccMeshVBO(){
	if(!pDirtyOccMeshVBO){
		return;
	}
	pDirtyOccMeshVBO = false;
	
	if(pOcclusionMesh){
		pOcclusionMesh->PrepareVBOBlock();
	}
}

void deoglRComponent::pPrepareOccMeshRTSInstances(){
	if(pOccMeshSharedSPBDoubleSided || pOccMeshSharedSPBSingleSided){
		const deoglSharedVBOBlock &block = *pOcclusionMesh->GetVBOBlock();
		const int pointOffset = pDynamicOcclusionMesh ? 0 : block.GetOffset();
		
		if(pOccMeshSharedSPBDoubleSided){
			deoglRenderTaskSharedInstance &rtsi = *pOccMeshSharedSPBDoubleSided->GetRTSInstance();
			rtsi.SetFirstPoint(pointOffset);
			rtsi.SetFirstIndex(block.GetIndexOffset() + pOcclusionMesh->GetSingleSidedFaceCount() * 3);
			rtsi.SetIndexCount(pOcclusionMesh->GetDoubleSidedFaceCount() * 3);
		}
		
		if(pOccMeshSharedSPBSingleSided){
			deoglRenderTaskSharedInstance &rtsi = *pOccMeshSharedSPBSingleSided->GetRTSInstance();
			rtsi.SetFirstPoint(pointOffset);
			rtsi.SetFirstIndex(block.GetIndexOffset());
			rtsi.SetIndexCount(pOcclusionMesh->GetSingleSidedFaceCount() * 3);
		}
	}
}

void deoglRComponent::pPrepareDynOccMesh(){
	if(!pDynOccMeshRequiresPrepareForRender){
		return;
	}
	pDynOccMeshRequiresPrepareForRender = false;
	
	if(pDynamicOcclusionMesh){
		pDynamicOcclusionMesh->PrepareForRender();
	}
}



void deoglRComponent::pResizeBoneMatrices(){
	const int boneCount = pModel ? pModel->GetBoneCount() : 0;
	oglMatrix3x4 *matrices = NULL;
	
	if(boneCount > 0){
		matrices = new oglMatrix3x4[boneCount];
	}
	
	if(pBoneMatrices){
		delete [] pBoneMatrices;
	}
	pBoneMatrices = matrices;
	pBoneMatrixCount = boneCount;
	
	if(pDynamicOcclusionMesh){
		pDynamicOcclusionMesh->ComponentStateChanged();
		pDynOccMeshRequiresPrepareForRender = true;
	}
}



void deoglRComponent::pRemoveFromAllLights(){
	const int count = pLightList.GetCount();
	int i;
	
	for(i=0; i< count; i++){
		pLightList.GetAt(i)->RemoveComponent(this);
	}
	
	pLightList.RemoveAll();
}

void deoglRComponent::pRequiresPrepareForRender(){
	if(!pLLPrepareForRenderWorld.GetList() && pParentWorld){
		pParentWorld->AddPrepareForRenderComponent(this);
	}
}
