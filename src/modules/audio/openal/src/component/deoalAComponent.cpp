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
#include <string.h>

#include "deoalAComponent.h"
#include "deoalAComponentBone.h"
#include "deoalAComponentTexture.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../model/deoalAModel.h"
#include "../model/deoalModelFace.h"
#include "../model/octree/deoalModelOctree.h"
#include "../model/octree/deoalModelRTBVH.h"
#include "../skin/deoalASkin.h"
#include "../skin/deoalSkinTexture.h"
#include "../world/deoalAWorld.h"
#include "../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>


// Class deoalAComponent
//////////////////////////

// Constructor, destructor
////////////////////////////

deoalAComponent::deoalAComponent(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pParentWorld(NULL),
pOctreeNode(NULL),
pTextureNames(NULL),
pAffectsSound(false),
pHasScaling(false),
pDirtyMatrices(true),
pDirtyWeightMatrices(true),
pDirtyExtends(true),
// pDirtyRTSphere( true ),
pDirtyTextureUseSkin(true),

pBones(NULL),
pBoneCount(0),
pWeightMatrices(NULL),
pFaces(NULL),
pFaceCount(0),
// pOctree( NULL ),
pBVH(NULL),

pWorldMarkedRemove(false),
pLLWorldPrev(NULL),
pLLWorldNext(NULL)
{
	LEAK_CHECK_CREATE(audioThread, Component);
}

deoalAComponent::~deoalAComponent(){
	LEAK_CHECK_FREE(pAudioThread, Component);
	
	pCleanUp();
}



// Management
///////////////

void deoalAComponent::SetParentWorld(deoalAWorld *world){
	if(world == pParentWorld){
		return;
	}
	
	if(pOctreeNode){
		pOctreeNode->RemoveComponent(this);
	}
	
	pParentWorld = world;
}



void deoalAComponent::SetOctreeNode(deoalWorldOctree *node){
	pOctreeNode = node;
}

void deoalAComponent::UpdateOctreeNode(){
	if(pParentWorld && pAffectsSound){
		pPrepareExtends();
		pParentWorld->GetOctree()->InsertComponentIntoTree(this, 8);
		pParentWorld->InvalidateEnvProbes(pMinExtend, pMaxExtend, pLayerMask);
		
	}else if(pOctreeNode){
		pOctreeNode->RemoveComponent(this);
		if(pParentWorld){
			pParentWorld->InvalidateEnvProbes(pMinExtend, pMaxExtend, pLayerMask);
		}
	}
}

void deoalAComponent::PrepareQuickDispose(){
	pParentWorld = NULL;
	pOctreeNode = NULL;
}



void deoalAComponent::SetModel(deoalAModel::Ref model, const decStringList *textureNames){
	if(model == pModel && textureNames == pTextureNames){
		return;
	}
	
	pDropWeightMatrices();
	pDropFaces();
	pModel = model;
	pTextureNames = textureNames;
	pDirtyTextureUseSkin = true;
	pDirtyExtends = true;
// 	pDirtyRTSphere = true;
	
	pResizeModelTextureMappings();
	pUpdateModelTextureMappings();
	
	pResizeTextureSkinMappings();
	pUpdateTextureSkinMappings();
	
	pResizeModelRigMappings();
	pUpdateModelRigMappings();
}

void deoalAComponent::SetSkin(deoalASkin::Ref skin){
	if(skin == pSkin){
		return;
	}
	pSkin = skin;
	pUpdateTextureSkinMappings();
}



int deoalAComponent::GetTextureCount() const{
	return pTextures.GetCount();
}

deoalAComponentTexture &deoalAComponent::GetTextureAt(int index) const{
	// we can not do pDirtyTextureUseSkin checking here because this method is potentially
	// called from concurrent threads. the update has to be done during an explicit
	// PrepareOctree() call. if missed the result is out of date and bad things can happen
	/*
	if(pDirtyTextureUseSkin){
		const int count = pTextures.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			deoalAComponentTexture &texture = *((deoalAComponentTexture*)pTextures.GetAt(i));
			texture.UpdateUseTexture();
			texture.UpdateSoundParameters();
		}
		
		pDirtyTextureUseSkin = false;
	}
	*/
	
	return *((deoalAComponentTexture*)pTextures.GetAt(index));
}

deoalAComponentTexture &deoalAComponent::GetModelTextureAt(int index) const{
	return *((deoalAComponentTexture*)pTextures.GetAt(pModelTextureMappings.GetAt(index)));
}

void deoalAComponent::AddTexture(deoalAComponentTexture *texture){
	pTextures.Add(texture);
	pUpdateModelTextureMappings();
}

void deoalAComponent::RemoveAllTextures(){
	pTextures.RemoveAll();
	pUpdateModelTextureMappings();
}



void deoalAComponent::SetGeometry(const decDVector &position,
const decQuaternion &orientation, const decVector &scaling){
	pPosition = position;
	pOrientation = orientation;
	pScaling = scaling;
	
	pDirtyMatrices = true;
	pDirtyExtends = true;
// 	pDirtyRTSphere = true;
}

void deoalAComponent::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
}



void deoalAComponent::UpdateAffectsSound(){
	pAffectsSound = false;
	
	if(!pModel){
		return;
	}
	
	if(pParentWorld && pParentWorld->GetAllMicLayerMask().MatchesNot(pLayerMask)){
		return;
	}
	
	const int count = pTextures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalAComponentTexture &texture = *((deoalAComponentTexture*)pTextures.GetAt(i));
		if(texture.GetUseTexture() && texture.GetUseTexture()->GetAffectsSound()){
			pAffectsSound = true;
			break;
		}
	}
	
	// for performance reasons ignore small objects
	// 
	// NOTE not done anymore since user has to explicitly use audio models so trust him
// 	if( pAffectsSound && pIgnoreBySize( pModel->GetSize() ) ){
// 		pAffectsSound = false;
// 	}
}



void deoalAComponent::InitBones(const deComponent &component){
	const int boneCount = component.GetBoneCount();
	if(boneCount > 0 && !component.GetRig()){
		DETHROW(deeInvalidParam); // better safe than sorry
	}
	
	pDropFaces();
	
	if(boneCount != pBoneCount){
		if(pBones){
			delete [] pBones;
			pBones = NULL;
			pBoneCount = 0;
		}
		
		if(boneCount > 0){
			pBones = new deoalAComponentBone[boneCount];
		}
	}
	
	if(boneCount > 0){
		// this if-check is used to silence address sanitizers. the potential null pointer
		// dereferencing has no affect since this can only happen if boneCount is 0
		const deRig &rig = *component.GetRig();
		
		for(pBoneCount=0; pBoneCount<boneCount; pBoneCount++){
			const deComponentBone &engBone = component.GetBoneAt(pBoneCount);
			const deRigBone &rigBone = rig.GetBoneAt(pBoneCount);
			deoalAComponentBone &bone = pBones[pBoneCount];
			
			bone.SetIndex(pBoneCount);
			bone.SetName(rigBone.GetName());
			
			if(engBone.GetParentBone() != -1){
				bone.SetParent(pBones + engBone.GetParentBone());
				
			}else{
				bone.SetParent(NULL);
			}
			
			bone.SetOriginalMatrix(engBone.GetOriginalMatrix());
			bone.SetRigInverseMatrix(rigBone.GetInverseMatrix());
		}
	}
	
	pResizeModelRigMappings();
	pUpdateModelRigMappings();
	
	pDirtyWeightMatrices = true;
}

void deoalAComponent::UpdateBoneGeometry(const deComponent &component){
	// update bone geometry and track changes
	// 
	// TODO figure out which bones potentially affect audible geometry.
	//      this way changes to unrelated bones do not trigger updates.
	bool nothingChanged = true;
	int i;
	
	for(i=0; i<pBoneCount; i++){
		const deComponentBone &engBone = component.GetBoneAt(i);
		deoalAComponentBone &bone = pBones[i];
		
		if(nothingChanged){
			nothingChanged = bone.GetPosition().IsEqualTo(engBone.GetPosition(), 1e-4f)
				&& bone.GetRotation().IsEqualTo(engBone.GetRotation(), 1e-4f)
				&& bone.GetScale().IsEqualTo(engBone.GetScale(), 1e-4f);
		}
		
		bone.SetGeometry(engBone);
	}
	
	// invalidate matrices and env-probes only if any bone changed. this avoids recalculations
	// in situations where animators are applied but the result is the same as last frame
	if(nothingChanged){
		return;
	}
	
	pDirtyWeightMatrices = true;
	
	pParentWorld->InvalidateEnvProbes(pMinExtend, pMaxExtend, pLayerMask);
	// NOTE it is possible the invalidate is done here and in an UpdateOctreeNode. check out
	//      later if this is a performance problem or not
}

void deoalAComponent::PrepareOctree(){
	// prepare textures. this has to be done here because GetTextureAt() is potentially called
	// by concurrent threads
	if(pDirtyTextureUseSkin){
		const int count = pTextures.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			deoalAComponentTexture &texture = *((deoalAComponentTexture*)pTextures.GetAt(i));
			texture.UpdateUseTexture();
			texture.UpdateSoundParameters();
		}
		
		pDirtyTextureUseSkin = false;
	}
	
	// prepare octrees
	if(pBoneCount == 0){
		if(pModel){
			pModel->PrepareOctree();
		}
		
	}else{
		pUpdateFaces();
		pBuildOctree();
	}
	
	// prepare ray-trace sphere for accelerated octrees
// 	pPrepareRTSphere();
}

const deoalModelFace &deoalAComponent::GetFaceAt(int index) const{
	if(index < 0 || index >= pFaceCount){
		DETHROW(deeInvalidParam);
	}
	return pFaces[index];
}



// Render world usage
///////////////////////

void deoalAComponent::SetWorldMarkedRemove(bool marked){
	pWorldMarkedRemove = marked;
}

void deoalAComponent::SetLLWorldPrev(deoalAComponent *component){
	pLLWorldPrev = component;
}

void deoalAComponent::SetLLWorldNext(deoalAComponent *component){
	pLLWorldNext = component;
}



// Private Functions
//////////////////////

void deoalAComponent::pCleanUp(){
	if(pBVH){
		delete pBVH;
	}
// 	if( pOctree ){
// 		delete pOctree;
// 	}
	if(pFaces){
		delete [] pFaces;
	}
	if(pWeightMatrices){
		delete [] pWeightMatrices;
	}
	if(pBones){
		delete [] pBones;
	}
	
	pTextures.RemoveAll();
}



void deoalAComponent::pUpdateModelTextureMappings(){
	if(!pTextureNames || !pModel){
		const int count = pModelTextureMappings.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pModelTextureMappings.SetAt(i, -1);
		}
		return;
	}
	
	const decStringList &modelTexturesNames = pModel->GetTextureNames();
	const int count = pModelTextureMappings.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pModelTextureMappings.SetAt(i, pTextureNames->IndexOf(modelTexturesNames.GetAt(i)));
	}
}

void deoalAComponent::pResizeModelTextureMappings(){
	const int count = pModel ? pModel->GetTextureNames().GetCount() : 0;
	if(count == pModelTextureMappings.GetCount()){
		return;
	}
	
	pModelTextureMappings.RemoveAll();
	int i;
	for(i=0; i<count; i++){
		pModelTextureMappings.Add(-1);
	}
}

void deoalAComponent::pUpdateTextureSkinMappings(){
	if(!pTextureNames || !pSkin){
		const int count = pTextureSkinMappings.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pTextureSkinMappings.SetAt(i, -1);
		}
		return;
	}
	
	// determine mappings of textures from model to skin
	const int count = pTextureSkinMappings.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		pTextureSkinMappings.SetAt(i, -1);
		if(!pSkin){
			continue;
		}
		
		const decString &name = pTextureNames->GetAt(i);
		for(j=0; j<pSkin->GetTextureCount(); j++){
			if(pSkin->GetTextureAt(j).GetName() == name){
				pTextureSkinMappings.SetAt(i, j);
				break;
			}
		}
	}
	
	// mark all textures dirty
	pDirtyTextureUseSkin = true;
}

void deoalAComponent::pResizeTextureSkinMappings(){
	int count = 0;
	if(pTextureNames){
		count = pTextureNames->GetCount();
	}
	
	if(count == pTextureSkinMappings.GetCount()){
		return;
	}
	
	pTextureSkinMappings.RemoveAll();
	int i;
	for(i=0; i<count; i++){
		pTextureSkinMappings.Add(-1);
	}
}

void deoalAComponent::pUpdateModelRigMappings(){
	if(!pModel || pBoneCount == 0){
		const int count = pModelRigMappings.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pModelRigMappings.SetAt(i, -1);
		}
		return;
	}
	
	// determine mappings of bones from model to rig
	const decStringList &boneNames = pModel->GetBoneNames();
	const int count = pModelRigMappings.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		pModelRigMappings.SetAt(i, -1);
		
		const decString &name = boneNames.GetAt(i);
		for(j=0; j<pBoneCount; j++){
			if(pBones[j].GetName() == name){
				pModelRigMappings.SetAt(i, j);
				break;
			}
		}
	}
}

void deoalAComponent::pResizeModelRigMappings(){
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
}



void deoalAComponent::pPrepareMatrices(){
	if(!pDirtyMatrices){
		return;
	}
	pDirtyMatrices = false;
	
	pHasScaling = !pScaling.IsEqualTo(decVector(1.0f, 1.0f, 1.0f));
	
	if(pHasScaling){
		pMatrix.SetWorld(pPosition, pOrientation, pScaling);
		
	}else{
		pMatrix.SetWorld(pPosition, pOrientation);
	}
	
	pInvMatrix = pMatrix.QuickInvert();
}

void deoalAComponent::pPrepareExtends(){
	if(!pDirtyExtends){
		return;
	}
	pDirtyExtends = false;
	
	if(!pModel){
		pMinExtend = pMaxExtend = pPosition;
		return;
	}
	
	pPrepareMatrices();
	
	const decDVector modelMin(pBoneCount == 0 ? pModel->GetMinExtend() : pDynamicMinExtend);
	const decDVector modelMax(pBoneCount == 0 ? pModel->GetMaxExtend() : pDynamicMaxExtend);
	const decDVector corners[8] = {
		pMatrix.Transform(modelMin.x, modelMin.y, modelMin.z),
		pMatrix.Transform(modelMax.x, modelMin.y, modelMin.z),
		pMatrix.Transform(modelMax.x, modelMax.y, modelMin.z),
		pMatrix.Transform(modelMin.x, modelMax.y, modelMin.z),
		pMatrix.Transform(modelMin.x, modelMin.y, modelMax.z),
		pMatrix.Transform(modelMax.x, modelMin.y, modelMax.z),
		pMatrix.Transform(modelMax.x, modelMax.y, modelMax.z),
		pMatrix.Transform(modelMin.x, modelMax.y, modelMax.z)};
	
	pMinExtend = pMaxExtend = corners[0];
	int i;
	for(i=1; i<8; i++){
		pMinExtend.SetSmallest(corners[i]);
		pMaxExtend.SetLargest(corners[i]);
	}
	
	pBoxCenter = (pMinExtend + pMaxExtend) * 0.5;
	pBoxHalfExtends = (pMaxExtend - pMinExtend) * 0.5;
}

/*
void deoalAComponent::pPrepareRTSphere(){
	if(!pDirtyRTSphere){
		return;
	}
	pDirtyRTSphere = false;
	
	if(!pModel){
		pRTSphereCenter = pPosition;
		pRTSphereRadiusSquared = 0.0;
		return;
	}
	
	pPrepareMatrices();
	
	if(pBoneCount == 0){
		pRTSphereCenter = pMatrix * pModel->GetRTSphereCenter();
		pRTSphereRadiusSquared = pModel->GetRTSphereRadiusSquared();
		return;
	}
	
	if(pFaceCount == 0){
		pRTSphereCenter = pPosition;
		pRTSphereRadiusSquared = 0.0;
		return;
	}
	
	int i;
	
	pRTSphereCenter.SetZero();
	for(i=0; i<pFaceCount; i++){
		pRTSphereCenter += pFaces[i].GetVertex1();
		pRTSphereCenter += pFaces[i].GetVertex2();
		pRTSphereCenter += pFaces[i].GetVertex3();
	}
	pRTSphereCenter /= (float)(pFaceCount * 3);
	
	pRTSphereRadiusSquared = 0.0;
	for(i=0; i<pFaceCount; i++){
		const double distSquared = decMath::max(
			(pFaces[i].GetVertex1() - pRTSphereCenter).LengthSquared(),
			(pFaces[i].GetVertex2() - pRTSphereCenter).LengthSquared(),
			(pFaces[i].GetVertex3() - pRTSphereCenter).LengthSquared());
		if(distSquared > pRTSphereRadiusSquared){
			pRTSphereRadiusSquared = distSquared;
		}
	}
	
	pRTSphereCenter = pMatrix * pRTSphereCenter;
}
*/



void deoalAComponent::pPrepareBoneMatrices(){
	if(pBoneCount == 0){
		return;
	}
	
	int i;
	for(i=0; i<pBoneCount; i++){
		pBones[i].UpdateMatrix();
	}
}

void deoalAComponent::pPrepareWeightMatrices(){
	if(!pModel || !pDirtyWeightMatrices){
		return;
	}
	
	pPrepareBoneMatrices();
	
	const int weightSetCount = pModel->GetWeightSetCount();
	if(weightSetCount == 0){
		pDirtyWeightMatrices = false;
		return;
	}
	
	if(!pWeightMatrices){
		pWeightMatrices = new decMatrix[weightSetCount];
	}
	
	const deoalAModel::sWeightSet * const weightSets = pModel->GetWeightSets();
	const deoalAModel::sWeight *weight = pModel->GetWeights();
	int i, j;
	
	for(i=0; i<weightSetCount; i++){
		const deoalAModel::sWeightSet &weightSet = weightSets[i];
		decMatrix &matrix = pWeightMatrices[i];
		
		if(weightSet.weightCount == 0){
			matrix.SetIdentity();
			
		}else if(weightSet.weightCount == 1){
			const int bone = pModelRigMappings.GetAt(weight->bone);
			
			if(bone != -1){
				matrix = pBones[bone].GetWeightMatrix();
				
			}else{
				matrix.SetIdentity();
			}
			
			weight++;
			
		}else{
			const int bone0 = pModelRigMappings.GetAt(weight->bone);
			const float value0 = weight->weight;
			
			if(bone0 != -1){
				matrix = pBones[bone0].GetWeightMatrix().QuickMultiply(value0);
				
			}else{
				matrix.SetScale(value0, value0, value0);
			}
			weight++;
			
			for(j=1; j<weightSet.weightCount; j++){
				const int boneJ = pModelRigMappings.GetAt(weight->bone);
				const float valueJ = weight->weight;
				
				if(boneJ != -1){
					matrix.QuickAddTo(pBones[boneJ].GetWeightMatrix().QuickMultiply(valueJ));
					
				}else{
					matrix.a11 += valueJ;
					matrix.a22 += valueJ;
					matrix.a33 += valueJ;
				}
				
				weight++;
			}
		}
	}
	
	pDirtyWeightMatrices = false;
}

void deoalAComponent::pDropWeightMatrices(){
	if(pWeightMatrices){
		delete [] pWeightMatrices;
		pWeightMatrices = NULL;
		
		pDirtyWeightMatrices = true;
	}
	
	if(pBones){
		delete [] pBones;
		pBones = NULL;
		pBoneCount = 0;
	}
}

void deoalAComponent::pUpdateFaces(){
	if(!pModel){
		pDropFaces();
		return;
	}
	
	const int count = pModel->GetFaceCount();
	if(count != pFaceCount){
		pDropFaces();
	}
	
	if(count == 0){
		return;
	}
	
	pPrepareWeightMatrices();
	
	if(!pFaces){
		pFaces = new deoalModelFace[count];
		for(pFaceCount=0; pFaceCount<count; pFaceCount++){
			pFaces[pFaceCount].InitFromStatic(pModel->GetFaceAt(pFaceCount));
		}
	}
	
	int i;
	for(i=0; i<pFaceCount; i++){
		const deoalModelFace &modelFace = pModel->GetFaceAt(i);
		deoalModelFace &face = pFaces[i];
		
		if(face.GetWeightSet1() != -1){
			face.SetVertex1(pWeightMatrices[face.GetWeightSet1()] * modelFace.GetVertex1());
		}
		if(face.GetWeightSet2() != -1){
			face.SetVertex2(pWeightMatrices[face.GetWeightSet2()] * modelFace.GetVertex2());
		}
		if(face.GetWeightSet3() != -1){
			face.SetVertex3(pWeightMatrices[face.GetWeightSet3()] * modelFace.GetVertex3());
		}
		
		face.UpdateNormalAndEdges();
	}
	
	decVector minExtend, maxExtend;
	
	for(i=0; i<pFaceCount; i++){
		const deoalModelFace &face = pFaces[i];
		
		minExtend.SetSmallest(face.GetVertex1());
		minExtend.SetSmallest(face.GetVertex2());
		minExtend.SetSmallest(face.GetVertex3());
		
		maxExtend.SetLargest(face.GetVertex1());
		maxExtend.SetLargest(face.GetVertex2());
		maxExtend.SetLargest(face.GetVertex3());
	}
	
	if(!(minExtend >= pDynamicMinExtend && maxExtend <= pDynamicMaxExtend)){
		pDirtyExtends = true;
// 		pDirtyRTSphere = true;
		pDynamicMinExtend.SetSmallest(minExtend);
		pDynamicMaxExtend.SetLargest(maxExtend);
		pPrepareExtends();
		UpdateOctreeNode();
	}
}

void deoalAComponent::pDropFaces(){
	pDropOctree();
	if(pFaces){
		delete [] pFaces;
		pFaces = NULL;
	}
	pFaceCount = 0;
}

void deoalAComponent::pBuildOctree(){
	/*
	if(pOctree){
		pOctree->ClearFaces();
		
	}else{
		pOctree = new deoalModelOctree(
			(pDynamicMinExtend + pDynamicMaxExtend) * 0.5f,
			(pDynamicMaxExtend - pDynamicMinExtend) * 0.5f);
	}
	
	const int maxDepth = 5;
	int i;
	for(i=0; i<pFaceCount; i++){
		pOctree->InsertFaceIntoTree(pFaces + i, maxDepth);
	}
	*/
	
	if(!pBVH){
		pBVH = new deoalModelRTBVH;
	}
	pBVH->Build(pFaces, pFaceCount);
}

void deoalAComponent::pDropOctree(){
	if(pBVH){
		delete pBVH;
		pBVH = NULL;
	}
// 	if( pOctree ){
// 		delete pOctree;
// 		pOctree = NULL;
// 	}
}

bool deoalAComponent::pIgnoreBySize(const decVector &size) const{
	// for performance reasons ignore small objects. as a reference here some wave-length:
	// - 125Hz: 2.7m
	// - 1kHz: 0.34m
	// - 2kHz: 0.17m
	// - 4kHz: 0.085m
	// 
	// using here 15cm. this could be made a configuration parameter or something similar.
	// to be ignore the object needs to have at least 2 extends equal or smaller than the
	// threshold value. this captures all kinds of small objects as well as elongated
	// pole like objects
	const float threshold = 0.15f;
	
	int count = 0;
	if(size.x <= threshold){
		count++;
	}
	if(size.y <= threshold){
		count++;
	}
	if(size.z <= threshold){
		count++;
	}
	
	if(count >= 2){
		return true;
	}
	
	return false;
}
