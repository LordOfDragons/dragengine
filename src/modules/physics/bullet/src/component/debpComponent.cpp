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

#include "debpModel.h"
#include "debpComponent.h"
#include "../dePhysicsBullet.h"
#include "../coldet/collision/debpDCollisionVolume.h"
#include "../decal/debpDecal.h"
#include "../visitors/debpShapeToVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../world/debpWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Class debpComponent
////////////////////////

// Constructor, destructor
////////////////////////////

debpComponent::debpComponent(dePhysicsBullet &bullet, deComponent *component) :
pBullet(bullet),

pComponent(component),
pIndex(-1),
pDirtyModel(true),
pDirtyMesh(true),
pDirtyExtends(true),
pDirtyBoneWeights(true),
pDirtyWeights(true),
pEnabled(false),
pModel(nullptr),
pLinkedCollider(nullptr)
{
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	ModelChanged();
}

debpComponent::~debpComponent(){
	pCleanUp();
}



// Management
///////////////

void debpComponent::SetIndex(int index){
	pIndex = index;
}

void debpComponent::SetEnabled(bool enabled){
	pEnabled = enabled;
}

void debpComponent::SetLinkedCollider(debpColliderComponent *collider){
	pLinkedCollider = collider;
}



debpComponent::sBone &debpComponent::GetBoneAt(int index){
	return pBones[index];
}

const debpComponent::sBone &debpComponent::GetBoneAt(int index) const{
	return pBones[index];
}

void debpComponent::PrepareBone(int index){
	pComponent->WaitAnimatorTaskFinished();
	pPrepareBone(index);
}

void debpComponent::SetAllBoneDirty(){
	pBones.Visit([&](sBone &bone){
		bone.dirty = true;
	});
}

void debpComponent::ClearAllBoneDirty(){
	pBones.Visit([&](sBone &bone){
		bone.dirty = false;
	});
}

int debpComponent::GetModelRigMapping(int bone) const{
	return pModelRigMappings[bone];
}



// Notifications
//////////////////


void debpComponent::PositionChanged(){
}

void debpComponent::ScalingChanged(){
	// does send ExtendsChanged
	
	if(pLinkedCollider){
		pLinkedCollider->ComponentShapeChanged();
	}
}

void debpComponent::OrientationChanged(){
	// does send ExtendsChanged
}

void debpComponent::ModelChanged(){
	SetAllBoneDirty();
	
	pDirtyModel = true;
	pDirtyMesh = true;
	pDirtyBoneWeights = true;
	pDirtyWeights = true;
	
	pModel = nullptr;
	if(pComponent->GetModel()){
		pModel = (debpModel*)pComponent->GetModel()->GetPeerPhysics();
	}
	
	pChangeModel();
	pUpdateModelRigMappings();
	
	if(pLinkedCollider){
		pLinkedCollider->ComponentShapeChanged();
	}
}

void debpComponent::RigChanged(){
	pRebuildBoneArrays();
	pDirtyMesh = true;
	
	pUpdateModelRigMappings();
	
	if(pLinkedCollider){
		pLinkedCollider->ComponentRigChanged();
	}
}

void debpComponent::VisibilityChanged(){
}

void debpComponent::ExtendsChanged(){
	pDirtyExtends = true;
}

void debpComponent::MeshDirty(){
	SetAllBoneDirty();
	
	pDirtyMesh = true;
	
	if(pLinkedCollider){
		pLinkedCollider->ComponentMeshDirty();
	}
}



// Collision Detection
////////////////////////

void debpComponent::FindDecalsAt(const decVector &point, deDecal::List &list){
	const decVector localPoint(pComponent->GetInverseMatrix() * point);
	pComponent->GetDecals().Visit([&](deDecal *engDecal){
		const debpDecal &decal = *((debpDecal*)engDecal->GetPeerPhysics());
		if(decal.TouchesPoint(localPoint)){
			list.Add(engDecal);
		}
	});
}

void debpComponent::FindDecalsTouching(decShape *shape, deDecal::List &list){
	debpShapeToVolume shapeToVolume;
	shapeToVolume.SetMatrix(pComponent->GetInverseMatrix());
	debpDCollisionVolume * const volume = shapeToVolume.GetVolumeFor(shape);
	
	pComponent->GetDecals().Visit([&](deDecal *engDecal){
		const debpDecal &decal = *((debpDecal*)engDecal->GetPeerPhysics());
		if(decal.TouchesVolume(volume)){
			list.Add(engDecal);
		}
	});
}



void debpComponent::PrepareMesh(){
	if(!pDirtyMesh){
		return;
	}
	if(!pComponent->GetModel() || !pComponent->GetRig()){
		pDirtyMesh = false;
		return;
	}
	
	PrepareWeights();
	
	const deModelLOD &lod = pComponent->GetModel()->GetLODAt(0);
	pVertices.VisitIndexed([&](int i, decVector &vertex){
		const deModelVertex &modelVertex = lod.GetVertices()[i];
		const int weightSet = modelVertex.GetWeightSet();
		
		if(weightSet == -1){
			vertex = modelVertex.GetPosition();
			
		}else{
			vertex = pWeights[weightSet] * modelVertex.GetPosition();
		}
	});
	
	pDirtyMesh = false;
}

void debpComponent::PrepareExtends(){
	if(!pDirtyExtends){
		return;
	}
	
	if(!pModel){
		pMinExtend.SetZero();
		pMaxExtend.SetZero();
		pDirtyExtends = false;
		return;
	}
	
	decDVector corners[8];
	bool extendsSet = false;
	int i, j;
	
	if(pBones.IsNotEmpty()){
		pComponent->PrepareBones();
		const decDMatrix &matrix = pComponent->GetMatrix();
		const decDVector position(matrix.GetPosition());
		
		if(pModel->GetHasWeightlessExtends()){
			corners[7].Set(pModel->GetWeightlessExtends().minimum);
			corners[1].Set(pModel->GetWeightlessExtends().maximum);
			corners[0].Set(corners[7].x, corners[1].y, corners[1].z);
			corners[2].Set(corners[1].x, corners[7].y, corners[1].z);
			corners[3].Set(corners[7].x, corners[7].y, corners[1].z);
			corners[4].Set(corners[7].x, corners[1].y, corners[7].z);
			corners[5].Set(corners[1].x, corners[1].y, corners[7].z);
			corners[6].Set(corners[1].x, corners[7].y, corners[7].z);
			for(j=0; j<8; j++){
				corners[j] = (matrix * corners[j]) - position;
				
				if(extendsSet){
					pMinExtend.SetSmallest(corners[j]);
					pMaxExtend.SetLargest(corners[j]);
					
				}else{
					pMinExtend = pMaxExtend = corners[j];
					extendsSet = true;
				}
			}
		}
		
		for(i=0; i<pModelRigMappings.GetCount(); i++){
			if(pModelRigMappings.GetAt(i) == -1){
				continue;
			}
			
			const debpModel::sExtends &boneExtends = pModel->GetBoneExtends()[i];
			const decDMatrix boneMatrix(decDMatrix(pComponent->GetBoneAt(
				pModelRigMappings.GetAt(i)).GetMatrix()) * matrix);
			
			corners[7].Set(boneExtends.minimum);
			corners[1].Set(boneExtends.maximum);
			corners[0].Set(corners[7].x, corners[1].y, corners[1].z);
			corners[2].Set(corners[1].x, corners[7].y, corners[1].z);
			corners[3].Set(corners[7].x, corners[7].y, corners[1].z);
			corners[4].Set(corners[7].x, corners[1].y, corners[7].z);
			corners[5].Set(corners[1].x, corners[1].y, corners[7].z);
			corners[6].Set(corners[1].x, corners[7].y, corners[7].z);
			
			for(j=0; j<8; j++){
				corners[j] = (boneMatrix * corners[j]) - position;
				
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
		const decDMatrix matrix = pComponent->GetMatrix();
		const decDVector position(matrix.GetPosition());
		
		corners[7] = decDVector(pModel->GetExtends().minimum);
		corners[1] = decDVector(pModel->GetExtends().maximum);
		corners[0].Set(corners[7].x, corners[1].y, corners[1].z);
		corners[2].Set(corners[1].x, corners[7].y, corners[1].z);
		corners[3].Set(corners[7].x, corners[7].y, corners[1].z);
		corners[4].Set(corners[7].x, corners[1].y, corners[7].z);
		corners[5].Set(corners[1].x, corners[1].y, corners[7].z);
		corners[6].Set(corners[1].x, corners[7].y, corners[7].z);
		for(j=0; j<8; j++){
			corners[j] = (matrix * corners[j]) - position;
		}
		
		pMinExtend = pMaxExtend = corners[0];
		for(j=1; j<8; j++){
			pMinExtend.SetSmallest(corners[j]);
			pMaxExtend.SetLargest(corners[j]);
		}
	}
	
	pDirtyExtends = false;
}

void debpComponent::PrepareWeights(){
	if(!pDirtyWeights){
		return;
	}
	
	const deRig * const rig = pComponent->GetRig();
	if(!rig){
		pDirtyWeights = false;
		return;
	}
	
	PrepareBoneWeights();
	
	// calculate weights matrices
	const debpModel::sWeightSet * const groups = pModel->GetWeightSets();
	const int groupCount = pModel->GetWeightSetCount();
	int i;
	
	for(i=0; i<groupCount; i++){
		const int count = groups[i].count;
		
		if(count == 1){
			const int bone = pModelRigMappings.GetAt(groups[i].first[0].GetBone());
			
			if(bone == -1){
				pWeights[i].SetIdentity();
				
			}else{
				pWeights[i] = pBones[bone].weightMatrix;
			}
			
		}else{
			const int bone = pModelRigMappings.GetAt(groups[i].first[0].GetBone());
			const float value = groups[i].first[0].GetWeight();
			int b;
			
			if(bone == -1){
				pWeights[i].SetScale(value, value, value);
				
			}else{
				pWeights[i] = pBones[bone].weightMatrix.QuickMultiply(value);
			}
			
			for(b=1; b<count; b++){
				const deModelWeight &weight = groups[i].first[b];
				const int bone2 = pModelRigMappings.GetAt(weight.GetBone());
				const float value2 = weight.GetWeight();
				
				if(bone2 == -1){
					pWeights[i].a11 += value2;
					pWeights[i].a22 += value2;
					pWeights[i].a33 += value2;
					
				}else{
					pWeights[i].QuickAddTo(pBones[bone2].weightMatrix.QuickMultiply(value2));
				}
			}
		}
	}
	
	pDirtyWeights = false;
}

void debpComponent::PrepareBoneWeights(){
	if(!pDirtyBoneWeights){
		return;
	}
	
	const deRig * const rig = pComponent->GetRig();
	if(!rig){
		pDirtyBoneWeights = false;
		return;
	}
	
	pComponent->PrepareBones();
	
	pBones.VisitIndexed([&](int i, sBone &bone){
		bone.weightMatrix = rig->GetBoneAt(i)->GetInverseMatrix()
			.QuickMultiply(pComponent->GetBoneAt(i).GetMatrix());
	});
	
	pDirtyBoneWeights = false;
}



const decMatrix &debpComponent::GetWeights(int index) const{
	return pWeights[index];
}

const decVector &debpComponent::GetVertex(int index) const{
	return pVertices[index];
}



// Private functions
//////////////////////

void debpComponent::pCleanUp(){
	if(pLinkedCollider){
		pLinkedCollider->SetLinkedComponent(nullptr);
	}
}

void debpComponent::pRebuildBoneArrays(){
	pBones.RemoveAll();
	
	pBones.AddRange(pComponent->GetBones().GetCount(), {});
	pBones.Visit([&](sBone &bone){
		bone.dirty = true;
	});
}

void debpComponent::pChangeModel(){
	pVertices.RemoveAll();
	pWeights.RemoveAll();
	pModelRigMappings.RemoveAll();
	
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	
	const deModel * const model = pComponent->GetModel();
	if(!model){
		return;
	}
	
	pVertices.AddRange(model->GetLODAt(0)->GetVertices().GetCount(), {});
	pWeights.AddRange(pModel->GetWeightSetCount(), {});
	pModelRigMappings.AddRange(model->GetBoneCount(), -1);
	
	pMinExtend = pModel->GetExtends().minimum;
	pMaxExtend = pModel->GetExtends().maximum;
}

void debpComponent::pUpdateModelRigMappings(){
	const deModel * const model = pComponent->GetModel();
	const deRig * const rig = pComponent->GetRig();
	const int count = pModelRigMappings.GetCount();
	int i;
	
	if(!rig || !model){
		for(i=0; i<count; i++){
			pModelRigMappings.SetAt(i, -1);
		}
		return;
	}
	
	for(i=0; i<count; i++){
		pModelRigMappings.SetAt(i, rig->IndexOfBoneNamed(model->GetBoneAt(i)->GetName()));
	}
}

void debpComponent::pPrepareBone(int index){
	if(!pBones[index].dirty){
		return;
	}
	
	const int parent = pComponent->GetBoneAt(index).GetParentBone();
	if(parent != -1){
		pPrepareBone(parent);
	}
	
	pComponent->UpdateBoneAt(index);
	
	pBones[index].dirty = false;
}
