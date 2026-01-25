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

#include "debpBulletShapeModel.h"
#include "debpModel.h"
#include "debpModelOctree.h"
#include "../dePhysicsBullet.h"
#include "../coldet/octree/debpDefaultDOctree.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>

#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"



// Class debpModel
////////////////////

// Constructor, Destructor
////////////////////////////

debpModel::debpModel(dePhysicsBullet &bullet, deModel &model) :
pBullet(bullet),
pModel(model),
pOctree(NULL),
pCanDeform(false),
pHasWeightlessExtends(false)
{
	(void)pBullet;
	
	pCheckCanDeform();
	pCalculateExtends();
}

debpModel::~debpModel(){
	if(pOctree){
		delete pOctree;
	}
}



// Management
///////////////

void debpModel::PrepareOctree(){
	if(pOctree){
		return;
	}
	
	// NOTE if model data has been released RetainModelData() is required to be called first
	
	const deModelLOD &lod = pModel.GetLODs().First();
	const decVector offset(0.01f, 0.01f, 0.01f);
	const decVector minExtend(pExtends.minimum - offset);
	const decVector maxExtend(pExtends.maximum + offset);
	const int faceCount = lod.GetFaces().GetCount();
	int i;
	
	pOctree = new debpModelOctree((minExtend + maxExtend) * 0.5f, (maxExtend - minExtend) * 0.5f);
	
	for(i=0; i<faceCount; i++){
		const deModelFace &face = lod.GetFaces()[i];
		
		const deModelVertex &v1 = lod.GetVertices()[face.GetVertex1()];
		const deModelVertex &v2 = lod.GetVertices()[face.GetVertex2()];
		const deModelVertex &v3 = lod.GetVertices()[face.GetVertex3()];
		
		const decVector &posV1 = v1.GetPosition();
		const decVector &posV2 = v2.GetPosition();
		const decVector &posV3 = v3.GetPosition();
		
		const decVector faceMinExtend(posV1.Smallest(posV2).Smallest(posV3));
		const decVector faceMaxExtend(posV1.Largest(posV2).Largest(posV3));
		
		pOctree->InsertFaceIntoTree(i, (faceMinExtend + faceMaxExtend) * 0.5f, (faceMaxExtend - faceMinExtend) * 0.5f);
	}
}

void debpModel::PrepareNormals(){
	if(pNormals.IsNotEmpty()){
		return;
	}
	
	// NOTE if model data has been released RetainModelData() is required to be called first
	
	const deModelLOD &lod = pModel.GetLODs().First();
	const int normalCount = lod.GetVertices().GetCount();
	if(normalCount == 0){
		return;
	}
	
	const int faceCount = lod.GetFaces().GetCount();
	int i;
	
	pNormals.AddRange(normalCount, {});
	
	for(i=0; i<faceCount; i++){
		const deModelFace &face = lod.GetFaces()[i];
		const decVector &faceNormal = face.GetFaceNormal();
		
		pNormals[face.GetVertex1()] += faceNormal;
		pNormals[face.GetVertex2()] += faceNormal;
		pNormals[face.GetVertex3()] += faceNormal;
	}
	
	pNormals.Visit([](decVector &normal){
		const float len = normal.Length();
		
		if(len < FLOAT_SAFE_EPSILON){
			normal.Set(0.0f, 1.0f, 0.0f);
			
		}else{
			normal /= len;
		}
	});
}

int debpModel::IndexOfFaceWithProbability(float probability) const{
	if(pFaceProbabilities.IsEmpty()){
		DETHROW(deeInvalidParam); // somebody forgot to retain the data first
	}
	
	// to get it working a hack version first. later on this will be replaced by a binary
	// search which is a lot faster
	const int count = pModel.GetLODs().First()->GetFaces().GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(probability < pFaceProbabilities[i]){
			break;
		}
	}
	
	if(i < count){
		return i;
		
	}else{
		return count - 1;
	}
}

void debpModel::PrepareFaceProbabilities(){
	if(pFaceProbabilities.IsNotEmpty()){
		return;
	}
	
	// NOTE if model data has been released RetainModelData() is required to be called first
	
	const deModelLOD &lod = pModel.GetLODs().First();
	const int faceCount = lod.GetFaces().GetCount();
	if(faceCount == 0){
		return;
	}
	
	float sumArea = 0.0f;
	int i;
	
	pFaceProbabilities.EnlargeCapacity(faceCount);
	
	// calculate the area of the face as well as the sum of all face areas
	for(i=0; i<faceCount; i++){
		const deModelFace &face = lod.GetFaces()[i];
		const decVector &v1 = lod.GetVertices()[face.GetVertex1()].GetPosition();
		const decVector &v2 = lod.GetVertices()[face.GetVertex2()].GetPosition();
		const decVector &v3 = lod.GetVertices()[face.GetVertex3()].GetPosition();
		
		const float probability = ((v2 - v1) % (v3 - v1)).Length() * 0.5f;
		pFaceProbabilities.Add(probability);
		sumArea += probability;
	}
	
	// the probability for a face is now the sum of all areas up to and including the face itself
	// devided by the total area sum. the entire array is thus a proability distribution in the end
	if(sumArea > FLOAT_SAFE_EPSILON){
		const float factor = 1.0f / sumArea;
		
		sumArea = 0.0f;
		pFaceProbabilities.Visit([&](float &faceProbability){
			sumArea += faceProbability;
			faceProbability = sumArea * factor;
		});
		
	}else{
		pFaceProbabilities.SetRangeAt(0, faceCount, 1.0f);
	}
}

void debpModel::PrepareShape(){
	if(pBulletShape){
		return;
	}
	
	// NOTE if model data has been released RetainModelData() is required to be called first
	
	const deModelLOD &lod = pModel.GetLODs().First();
	int i;
	
	// create vertex and face array suitable for the triangle mesh
	const int vertexCount = lod.GetVertices().GetCount();
	decTList<btScalar> vertices;
	
	if(vertexCount > 0){
		vertices.EnlargeCapacity(vertexCount * 3);
		for(i=0; i<vertexCount; i++){
			const decVector &position = lod.GetVertices()[i].GetPosition();
			vertices.Add(position.x);
			vertices.Add(position.y);
			vertices.Add(position.z);
		}
	}
	
	const int faceCount = lod.GetFaces().GetCount();
	decTList<int> faces;
	
	if(faceCount > 0){
		faces.EnlargeCapacity(faceCount * 3);
		for(i=0; i<faceCount; i++){
			const deModelFace &face = lod.GetFaces()[i];
			faces.Add(face.GetVertex1());
			faces.Add(face.GetVertex2());
			faces.Add(face.GetVertex3());
		}
	}
	
	// create triangle mesh
	btTriangleIndexVertexArray * const ivarray = new btTriangleIndexVertexArray(
		faceCount, faces.GetArrayPointer(), sizeof(int) * 3, vertexCount,
		vertices.GetArrayPointer(), sizeof(btScalar) * 3);
	
	btBvhTriangleMeshShape * const meshShape = new btBvhTriangleMeshShape(ivarray, true, true);
	meshShape->setUserPointer(0); // means -1 => no shape index set
	
	pBulletShape = debpBulletShapeModel::Ref::New(
		meshShape, ivarray, std::move(vertices), std::move(faces));
}



// private functions
//////////////////////

void debpModel::pCheckCanDeform(){
	int weightCount = pModel.GetLODs().First()->GetWeights().GetCount();
	
	// sanity check
	if(weightCount > 0 && pModel.GetBoneCount() == 0){
		//pBullet.LogWarnFormat( "Model '%s' has weights but no bones! Using 0 weights instead.", pModel.GetFilename() );
		weightCount = 0;
	}
	
	// deform check
	pCanDeform = (weightCount > 0);
	//pBullet.LogInfoFormat( "Model %s can deform: %s", pModel.GetFilename(), pCanDeform ? "yes" : "no" );
}

void debpModel::pCalculateExtends(){
	// extends of all vertices
	const deModelLOD &lod = pModel.GetLODs().First();
	const deModelVertex * const vertices = lod.GetVertices().GetArrayPointer();
	const int vertexCount = lod.GetVertices().GetCount();
	int i;
	
	if(vertexCount > 0){
		pExtends.minimum = pExtends.maximum = vertices[0].GetPosition();
		for(i=1; i<vertexCount; i++){
			const decVector &position = vertices[i].GetPosition();
			pExtends.minimum.SetSmallest(position);
			pExtends.maximum.SetLargest(position);
		}
		
	}else{
		pExtends.minimum.SetZero();
		pExtends.maximum.SetZero();
	}
	
	// extends of all points without weights and bone extends
	pWeightlessExtends.minimum.SetZero();
	pWeightlessExtends.maximum.SetZero();
	
	const int boneCount = pModel.GetBoneCount();
	if(boneCount == 0){
		return;
	}
	
	pBoneExtends.AddRange(boneCount, {});
	
	const int weightGroupCount = lod.GetWeightGroups().GetCount();
	if(weightGroupCount == 0){
		return;
	}
	
	const int * const weightGroups = lod.GetWeightGroups().GetArrayPointer();
	
	int weightSetCount = 0;
	for(i=0; i<weightGroupCount; i++){
		weightSetCount += weightGroups[i];
	}
	
	if(weightSetCount == 0){
		return;
	}
	
	pWeightSets.AddRange(weightSetCount, {});
	
	const deModelWeight *weightEntries = lod.GetWeights().GetArrayPointer();
	int j;
	
	sWeightSet *weightPointer = pWeightSets.GetArrayPointer();
	int weightGroupSize = 1;
	for(i=0; i<weightGroupCount; i++){
		const int count = weightGroups[i];
		for(j=0; j<count; j++){
			weightPointer->first = weightEntries;
			weightPointer->count = weightGroupSize;
			weightEntries += weightGroupSize;
			weightPointer++;
		}
		weightGroupSize++;
	}
	
	decTList<int> dominatingBones;
	dominatingBones.AddRange(pWeightSets.GetCount(), -1);
	
	decTList<bool> boneHasExtends;
	boneHasExtends.AddRange(boneCount, false);
	
	pWeightSets.VisitIndexed([&](int i, const sWeightSet &ws){
		if(ws.first->GetWeight() > 0.001f){ // 0.499f){
			dominatingBones[i] = ws.first->GetBone();
		}
	});
	
	for(i=0; i<vertexCount; i++){
		const decVector &position = vertices[i].GetPosition();
		
		int dominatingBone = -1;
		if(vertices[i].GetWeightSet() != -1){
			dominatingBone = dominatingBones[vertices[i].GetWeightSet()];
		}
		
		if(dominatingBone != -1){
			sExtends &boneExtends = pBoneExtends[dominatingBone];
			const decVector p(pModel.GetBoneAt(dominatingBone)->GetInverseMatrix() * position);
			
			if(boneHasExtends[dominatingBone]){
				boneExtends.minimum.SetSmallest(p);
				boneExtends.maximum.SetLargest(p);
				
			}else{
				boneExtends.minimum = boneExtends.maximum = p;
				boneHasExtends[dominatingBone] = true;
			}
			
		}else{
			if(pHasWeightlessExtends){
				pWeightlessExtends.minimum.SetSmallest(position);
				pWeightlessExtends.maximum.SetLargest(position);
				
			}else{
				pWeightlessExtends.minimum = pWeightlessExtends.maximum = position;
				pHasWeightlessExtends = true;
			}
		}
	}
}
