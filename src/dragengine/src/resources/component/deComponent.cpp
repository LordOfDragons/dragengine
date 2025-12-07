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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deComponent.h"
#include "deComponentBone.h"
#include "deComponentManager.h"
#include "deComponentTexture.h"
#include "../animator/deAnimator.h"
#include "../decal/deDecal.h"
#include "../model/deModelBone.h"
#include "../model/deModelFace.h"
#include "../model/deModel.h"
#include "../model/deModelLOD.h"
#include "../model/deModelManager.h"
#include "../model/deModelTexture.h"
#include "../model/deModelVertex.h"
#include "../model/deModelWeight.h"
#include "../occlusionmesh/deOcclusionMesh.h"
#include "../rig/deRigBone.h"
#include "../rig/deRig.h"
#include "../skin/deSkin.h"
#include "../skin/deSkinManager.h"
#include "../skin/deSkinTexture.h"
#include "../skin/dynamic/deDynamicSkin.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"
#include "../../parallel/deParallelProcessing.h"
#include "../../parallel/deParallelTask.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/modules/animator/deBaseAnimatorComponent.h"
#include "../../systems/modules/audio/deBaseAudioComponent.h"
#include "../../systems/modules/graphic/deBaseGraphicComponent.h"
#include "../../systems/modules/physics/deBasePhysicsComponent.h"



// Class deComponent
//////////////////////

// Constructor, destructor
////////////////////////////

deComponent::deComponent(deComponentManager *manager, deModel *model, deSkin *skin) :
deResource(manager),

pSkin(skin),

pScale(1.0f, 1.0f, 1.0f),
pVisible(true),
pHintMovement(emhStationary),
pEnableGI(true),
pHintGIImportance(4),

pBones(NULL),
pBoneCount(0),

pVertexPositionSetWeights(nullptr),
pVertexPositionSetCount(0),

pBonesDirty(true),
pMatrixDirty(false),

pTextures(NULL),
pTextureCount(0),

pDecalRoot(NULL),
pDecalTail(NULL),
pDecalCount(0),

pPeerGraphic(NULL),
pPeerPhysics(NULL),
pPeerAudio(NULL),
pPeerAnimator(NULL),

pParentWorld(NULL),
pLLWorldPrev(NULL),
pLLWorldNext(NULL)
{
	pLayerMask.SetBit(0);
	
	try{
		pChangeModel(model);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}
deComponent::~deComponent(){
	pCleanUp();
}



// Management
///////////////

void deComponent::SetPosition(const decDVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	pMatrixDirty = true;
	
	if(pPeerGraphic){
		pPeerGraphic->PositionChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->PositionChanged();
	}
	if(pPeerAudio){
		pPeerAudio->PositionChanged();
	}
}

void deComponent::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	pMatrixDirty = true;
	
	if(pPeerGraphic){
		pPeerGraphic->OrientationChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->OrientationChanged();
	}
	if(pPeerAudio){
		pPeerAudio->OrientationChanged();
	}
	
	InvalidateExtends();
}

void deComponent::SetScaling(const decVector &scale){
	if(pScale.IsEqualTo(scale)){
		return;
	}
	
	pScale = scale;
	pMatrixDirty = true;
	
	if(pPeerGraphic){
		pPeerGraphic->ScalingChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->ScalingChanged();
	}
	if(pPeerAudio){
		pPeerAudio->ScalingChanged();
	}
	
	InvalidateExtends();
}



void deComponent::SetModel(deModel *model){
	if(pModel == model){
		return;
	}
	
	pChangeModel(model);
	
	if(pPeerGraphic){
		pPeerGraphic->ModelChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->ModelChanged();
	}
	if(pPeerAudio){
		pPeerAudio->ModelChanged();
	}
	if(pPeerAnimator){
		pPeerAnimator->ModelChanged();
	}
	
	InvalidateMesh();
	InvalidateExtends();
}

void deComponent::SetModelKeepTextures(deModel* model){
	if(!pModel || !model || pModel->GetTextureCount() == 0 || model->GetTextureCount() == 0){
		SetModel(model);
		return;
	}
	
	deComponentTexture *textures = NULL;
	const int textureCount = pTextureCount;
	const deModel::Ref oldModel(pModel); // guard reference
	int i;
	
	try{
		textures = new deComponentTexture[textureCount];
		for(i=0; i<textureCount; i++){
			textures[i].SetDynamicSkin(pTextures[i].GetDynamicSkin());
			textures[i].SetSkin(pTextures[i].GetSkin());
			textures[i].SetTexture(pTextures[i].GetTexture());
			textures[i].SetTransform(pTextures[i].GetTransform());
		}
		
		SetModel(model);
		
		for(i=0; i<pTextureCount; i++){
			const int index = oldModel->IndexOfTextureNamed(model->GetTextureAt(i)->GetName());
			if(index == -1){
				continue;
			}
			
			pTextures[i].SetDynamicSkin(textures[index].GetDynamicSkin());
			pTextures[i].SetSkin(textures[index].GetSkin());
			pTextures[i].SetTexture(textures[index].GetTexture());
			pTextures[i].SetTransform(textures[index].GetTransform());
			NotifyTextureChanged(i);
		}
		
		delete [] textures;
		
	}catch(const deException &){
		if(textures){
			delete [] textures;
		}
		throw;
	}
}

void deComponent::SetSkin(deSkin *skin){
	if(pSkin == skin){
		return;
	}
	
	pSkin = skin;
	
	if(pPeerGraphic){
		pPeerGraphic->SkinChanged();
	}
	if(pPeerAudio){
		pPeerAudio->SkinChanged();
	}
}

void deComponent::SetRig(deRig *rig){
	if(pRig == rig){
		return;
	}
	
	pChangeRig(rig);
	
	if(pPeerGraphic){
		pPeerGraphic->RigChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->RigChanged();
	}
	if(pPeerAudio){
		pPeerAudio->RigChanged();
	}
	if(pPeerAnimator){
		pPeerAnimator->RigChanged();
	}
	
	InvalidateBones();
}

void deComponent::SetModelAndSkin(deModel *model, deSkin *skin){
	const bool modelChanged = model != pModel;
	const bool skinChanged = skin != pSkin;
	if(!modelChanged && !skinChanged){
		return;
	}
	
	if(modelChanged){
		pChangeModel(model);
	}
	if(skinChanged){
		pSkin = skin;
	}
	if(modelChanged){
		InvalidateBones();
	}
	
	if(modelChanged && skinChanged){
		if(pPeerGraphic){
			pPeerGraphic->ModelAndSkinChanged();
		}
		if(pPeerPhysics){
			pPeerPhysics->ModelChanged();
		}
		if(pPeerAudio){
			pPeerAudio->ModelAndSkinChanged();
		}
		if(pPeerAnimator){
			pPeerAnimator->ModelChanged();
		}
		
	}else if(modelChanged){
		if(pPeerGraphic){
			pPeerGraphic->ModelChanged();
		}
		if(pPeerPhysics){
			pPeerPhysics->ModelChanged();
		}
		if(pPeerAudio){
			pPeerAudio->ModelChanged();
		}
		if(pPeerAnimator){
			pPeerAnimator->ModelChanged();
		}
		
	}else if(skinChanged){
		if(pPeerGraphic){
			pPeerGraphic->SkinChanged();
		}
		if(pPeerAudio){
			pPeerAudio->SkinChanged();
		}
	}
}

void deComponent::SetOcclusionMesh(deOcclusionMesh *occmesh){
	if(occmesh == pOcclusionMesh){
		return;
	}
	
	pOcclusionMesh = occmesh;
	
	if(pPeerGraphic){
		pPeerGraphic->OcclusionMeshChanged();
	}
}

void deComponent::SetAudioModel(deModel *model){
	if(pAudioModel == model){
		return;
	}
	
	pAudioModel = model;
	
	if(pPeerAudio){
		pPeerAudio->AudioModelChanged();
	}
}



const decDMatrix &deComponent::GetMatrix(){
	PrepareMatrix();
	return pMatrix;
}

const decDMatrix &deComponent::GetInverseMatrix(){
	PrepareMatrix();
	return pInverseMatrix;
}

void deComponent::SetVisible(bool visible){
	if(visible == pVisible){
		return;
	}
	
	pVisible = visible;
	
	if(pPeerGraphic){
		pPeerGraphic->VisibilityChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->VisibilityChanged();
	}
	if(pPeerAudio){
		pPeerAudio->VisibilityChanged();
	}
}

void deComponent::SetHintMovement(eMovementHints hint){
	if(hint == pHintMovement){
		return;
	}
	
	pHintMovement = hint;
	
	if(pPeerGraphic){
		pPeerGraphic->ParametersChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->ParametersChanged();
	}
	if(pPeerAudio){
		pPeerAudio->ParametersChanged();
	}
}

void deComponent::SetEnableGI(bool enable){
	if(enable == pEnableGI){
		return;
	}
	
	pEnableGI = enable;
	
	if(pPeerGraphic){
		pPeerGraphic->ParametersChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->ParametersChanged();
	}
	if(pPeerAudio){
		pPeerAudio->ParametersChanged();
	}
}

void deComponent::SetHintGIImportance(int importance){
	DEASSERT_TRUE(importance >= 0)
	DEASSERT_TRUE(importance <= 4)
	
	if(importance == pHintGIImportance){
		return;
	}
	
	pHintGIImportance = importance;
	
	if(pPeerGraphic){
		pPeerGraphic->ParametersChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->ParametersChanged();
	}
	if(pPeerAudio){
		pPeerAudio->ParametersChanged();
	}
}

void deComponent::SetAnimator(deAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	pAnimator = animator;
	
	if(pPeerGraphic){
		pPeerGraphic->AnimatorChanged();
	}
}



void deComponent::SetLayerMask(const decLayerMask &layerMask){
	if(layerMask == pLayerMask){
		return;
	}
	
	pLayerMask = layerMask;
	
	if(pPeerGraphic){
		pPeerGraphic->LayerMaskChanged();
	}
	if(pPeerAudio){
		pPeerAudio->LayerMaskChanged();
	}
}



// Textures
/////////////

deComponentTexture &deComponent::GetTextureAt(int index) const{
	if(index < 0 || index >= pTextureCount){
		DETHROW(deeInvalidParam);
	}
	return pTextures[index];
}

int deComponent::IndexOfTextureClosestTo(const decVector &position, float radius) const{
	if(pPeerGraphic){
		return pPeerGraphic->IndexOfTextureClosestTo(position, radius);
	}
	return -1;
}

void deComponent::NotifyTextureChanged(int index) const{
	if(index < 0 || index >= pTextureCount){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerGraphic){
		pPeerGraphic->TextureChanged(index, pTextures[index]);
	}
	if(pPeerAudio){
		pPeerAudio->TextureChanged(index, pTextures[index]);
	}
}


deComponentBone &deComponent::GetBoneAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pBoneCount)
	return pBones[index];
}

void deComponent::UpdateBoneAt(int bone){
	DEASSERT_TRUE(bone >= 0)
	DEASSERT_TRUE(bone < pBoneCount)
	pUpdateBoneAt(bone);
}

void deComponent::InvalidateBones(){
	pBonesDirty = true;
	
	InvalidateMesh();
	InvalidateExtends();
}

void deComponent::ValidateBones(){
	pBonesDirty = false;
}

float deComponent::GetVertexPositionSetWeightAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pVertexPositionSetCount)
	return pVertexPositionSetWeights[index];
}

void deComponent::SetVertexPositionSetWeightAt(int index, float weight){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pVertexPositionSetCount)
	pVertexPositionSetWeights[index] = weight;
}

void deComponent::InvalidateMesh(){
	if(pPeerGraphic){
		pPeerGraphic->MeshDirty();
	}
	if(pPeerPhysics){
		pPeerPhysics->MeshDirty();
	}
	if(pPeerAudio){
		pPeerAudio->MeshDirty();
	}
}

void deComponent::InvalidateExtends(){
	if(pPeerGraphic){
		pPeerGraphic->ExtendsChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->ExtendsChanged();
	}
	if(pPeerAudio){
		pPeerAudio->ExtendsChanged();
	}
}

void deComponent::CopyBonesToComponent(deComponent &component){
	if(pBoneCount == 0 || !pRig || component.pBoneCount == 0 || !component.pRig){
		return;
	}
	
	PrepareBones();
	component.PrepareBones();
	
	int i;
	for(i=0; i<pBoneCount; i++){
		const int boneIndex = component.pRig->IndexOfBoneNamed(pRig->GetBoneAt(i).GetName());
		if(boneIndex == -1){
			continue;
		}
		
		deComponentBone &otherBone = component.pBones[boneIndex];
		const deComponentBone &bone = pBones[i];
		
		if(otherBone.GetParentBone() == -1){
			// root bone requires component space copy
			const decMatrix matrix(bone.GetMatrix().QuickMultiply(otherBone.GetInverseOriginalMatrix()));
			otherBone.SetPosition(matrix.GetPosition());
			otherBone.SetRotation(matrix.ToQuaternion());
			otherBone.SetScale(bone.GetScale());
			
		}else{
			// child bones require only bone local copy
			otherBone.SetPosition(bone.GetPosition());
			otherBone.SetRotation(bone.GetRotation());
			otherBone.SetScale(bone.GetScale());
		}
	}
	
	component.InvalidateBones();
}



void deComponent::SetDynamicSkin(deDynamicSkin *dynamicSkin){
	if(dynamicSkin == pDynamicSkin){
		return;
	}
	
	pDynamicSkin = dynamicSkin;
	
	if(pPeerGraphic){
		pPeerGraphic->DynamicSkinChanged();
	}
	if(pPeerAudio){
		pPeerAudio->DynamicSkinChanged();
	}
}



// Decals
////////////

void deComponent::AddDecal(deDecal *decal){
	if(!decal || decal->GetParentComponent() || decal->GetParentHeightTerrainSector()){
		DETHROW(deeInvalidParam);
	}
	
	if(pDecalTail){
		pDecalTail->SetLLComponentNext(decal);
		decal->SetLLComponentPrev(pDecalTail);
		decal->SetLLComponentNext(NULL); // not required by definition, just to make sure...
		
	}else{
		decal->SetLLComponentPrev(NULL); // not required by definition, just to make sure...
		decal->SetLLComponentNext(NULL); // not required by definition, just to make sure...
		pDecalRoot = decal;
	}
	
	pDecalTail = decal;
	pDecalCount++;
	decal->SetParentComponent(this);
	decal->AddReference();
	
	if(pPeerGraphic){
		pPeerGraphic->DecalAdded(decal);
	}
	if(pPeerPhysics){
		pPeerPhysics->DecalAdded(decal);
	}
	if(pPeerAudio){
		pPeerAudio->DecalAdded(decal);
	}
}

void deComponent::RemoveDecal(deDecal *decal){
	if(!decal || decal->GetParentComponent() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(decal->GetLLComponentPrev()){
		decal->GetLLComponentPrev()->SetLLComponentNext(decal->GetLLComponentNext());
	}
	if(decal->GetLLComponentNext()){
		decal->GetLLComponentNext()->SetLLComponentPrev(decal->GetLLComponentPrev());
	}
	if(decal == pDecalRoot){
		pDecalRoot = decal->GetLLComponentNext();
	}
	if(decal == pDecalTail){
		pDecalTail = decal->GetLLComponentPrev();
	}
	pDecalCount--;
	
	decal->SetParentComponent(NULL);
	decal->SetLLComponentPrev(NULL);
	decal->SetLLComponentNext(NULL);
	
	if(pPeerGraphic){
		pPeerGraphic->DecalRemoved(decal);
	}
	if(pPeerPhysics){
		pPeerPhysics->DecalRemoved(decal);
	}
	if(pPeerAudio){
		pPeerAudio->DecalRemoved(decal);
	}
	
	decal->FreeReference();
}

void deComponent::RemoveAllDecals(){
	while(pDecalTail){
		deDecal * const next = pDecalTail->GetLLComponentPrev();
		pDecalTail->SetParentComponent(NULL);
		pDecalTail->SetLLComponentPrev(NULL);
		pDecalTail->SetLLComponentNext(NULL);
		pDecalTail->FreeReference();
		pDecalTail = next;
		pDecalCount--;
	}
	pDecalRoot = NULL;
	
	if(pPeerGraphic){
		pPeerGraphic->AllDecalsRemoved();
	}
	if(pPeerPhysics){
		pPeerPhysics->AllDecalsRemoved();
	}
	if(pPeerAudio){
		pPeerAudio->AllDecalsRemoved();
	}
}



void deComponent::PrepareMatrix(){
	if(!pMatrixDirty){
		return;
	}
	
	pMatrix = decDMatrix::CreateScale(decDVector(pScale))
		* decDMatrix::CreateFromQuaternion( pOrientation )
		* decDMatrix::CreateTranslation( pPosition );
	
	pInverseMatrix = pMatrix.Invert();
	pMatrixDirty = false;
}

void deComponent::PrepareBones(){
	WaitAnimatorTaskFinished();
	
	if(!pBonesDirty){
		return;
	}
	
	int i;
	for(i=0; i<pBoneCount; i++){
		pUpdateBoneAt(i);
	}
	
	pBonesDirty = false;
}



void deComponent::SetAnimatorTask(deParallelTask *task){
	if(task == pAnimatorTask){
		return;
	}
	
	pAnimatorTask = task;
	InvalidateBones();
}

void deComponent::WaitAnimatorTaskFinished(){
	if(pAnimatorTask){
		GetEngine()->GetParallelProcessing().WaitForTask(pAnimatorTask);
	}
}





// Collision Detection
////////////////////////

void deComponent::FindDecalsAt(const decVector &point, deDecalList &list) const{
	if(pPeerPhysics){
		pPeerPhysics->FindDecalsAt(point, list);
	}
}

void deComponent::FindDecalsTouching(decShape *shape, deDecalList &list) const{
	if(pPeerPhysics){
		pPeerPhysics->FindDecalsTouching(shape, list);
	}
}



// System Peers
/////////////////

void deComponent::SetPeerGraphic(deBaseGraphicComponent *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	deBaseGraphicComponent * const safeDeletePeer = pPeerGraphic;
	
	pPeerGraphic = peer;
	
	if(safeDeletePeer){
		delete safeDeletePeer;
	}
}

void deComponent::SetPeerPhysics(deBasePhysicsComponent *peer){
	if(peer == pPeerPhysics){
		return;
	}
	
	deBasePhysicsComponent * const safeDeletePeer = pPeerPhysics;
	
	pPeerPhysics = peer;
	
	if(safeDeletePeer){
		delete safeDeletePeer;
	}
}

void deComponent::SetPeerAudio(deBaseAudioComponent *peer){
	if(peer == pPeerAudio){
		return;
	}
	
	deBaseAudioComponent * const safeDeletePeer = pPeerAudio;
	
	pPeerAudio = peer;
	
	if(safeDeletePeer){
		delete safeDeletePeer;
	}
}

void deComponent::SetPeerAnimator(deBaseAnimatorComponent *peer){
	if(peer == pPeerAnimator){
		return;
	}
	
	deBaseAnimatorComponent * const safeDeletePeer = pPeerAnimator;
	
	pPeerAnimator = peer;
	
	if(safeDeletePeer){
		delete safeDeletePeer;
	}
}



// Linked List
////////////////

void deComponent::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deComponent::SetLLWorldPrev(deComponent *component){
	pLLWorldPrev = component;
}

void deComponent::SetLLWorldNext(deComponent *component){
	pLLWorldNext = component;
}



// Private Functions
//////////////////////

void deComponent::pCleanUp(){
	if(pAnimatorTask){
		pAnimatorTask->Cancel();
		pAnimatorTask = NULL;
	}
	
	SetPeerAudio(NULL);
	SetPeerPhysics(NULL);
	SetPeerGraphic(NULL);
	SetPeerAnimator(NULL);
	
	RemoveAllDecals();
	
	if(pTextures){
		delete [] pTextures;
	}
	if(pVertexPositionSetWeights){
		delete [] pVertexPositionSetWeights;
	}
	if(pBones){
		delete [] pBones;
	}
}

void deComponent::pUpdateBoneAt(int bone){
	deComponentBone &cbone = pBones[bone];
	
	decMatrix matrix;
	matrix.SetWorld(cbone.GetPosition(), cbone.GetRotation(), cbone.GetScale());
	matrix = matrix.QuickMultiply(cbone.GetOriginalMatrix());
	
	if(cbone.GetParentBone() != -1){
		matrix = matrix.QuickMultiply(pBones[cbone.GetParentBone()].GetMatrix());
	}
	
	cbone.SetMatrix(matrix);
}

void deComponent::pChangeModel(deModel *model){
	float *vertexPositionSetWeights = nullptr;
	deComponentTexture *textures = nullptr;
	int vertexPositionSetCount = 0;
	int textureCount = 0;
	
	if(model){
		try{
			textureCount = model->GetTextureCount();
			if(textureCount > 0){
				textures = new deComponentTexture[textureCount];
			}
			
			vertexPositionSetCount = model->GetVertexPositionSetCount();
			if(vertexPositionSetCount > 0){
				vertexPositionSetWeights = new float[vertexPositionSetCount];
				memset(vertexPositionSetWeights, 0, sizeof(float) * vertexPositionSetCount);
			}
			
		}catch(const deException &){
			if(vertexPositionSetWeights){
				delete [] vertexPositionSetWeights;
			}
			if(textures){
				delete [] textures;
			}
			throw;
		}
	}
	
	if(pTextures){
		delete [] pTextures;
	}
	pTextures = textures;
	pTextureCount = textureCount;
	
	if(pVertexPositionSetWeights){
		delete [] pVertexPositionSetWeights;
	}
	pVertexPositionSetWeights = vertexPositionSetWeights;
	pVertexPositionSetCount = vertexPositionSetCount;
	
	pModel = model;
}

void deComponent::pChangeRig(deRig *rig){
	// if an animator task is pending cancel it since the result would be wrong anyways
	if(pAnimatorTask){
		pAnimatorTask->Cancel();
		pAnimatorTask = NULL;
	}
	
	// create new bones array
	deComponentBone *bones = NULL;
	int i, boneCount = 0;
	
	if(rig){
		try{
			boneCount = rig->GetBoneCount();
			if(boneCount > 0){
				bones = new deComponentBone[boneCount];
				for(i=0; i<boneCount; i++){
					const deRigBone &bone = rig->GetBoneAt(i);
					bones[i].SetParentBone(bone.GetParent());
					bones[i].SetOriginalMatrix(bone.GetPosition(), bone.GetRotation());
				}
			}
			
		}catch(const deException &){
			if(bones){
				delete [] bones;
			}
			throw;
		}
	}
	
	if(pBones){
		delete [] pBones;
	}
	pBones = bones;
	pBoneCount = boneCount;
	
	pRig = rig;
}
