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

pWeightSets(NULL),
pWeightSetCount(0),
pHasWeightlessExtends(false),
pBoneExtends(NULL),
pBoneCount(0),
pNormals(NULL),
pFaceProbabilities(NULL),

pBulletShape(NULL)
{
	(void)pBullet;
	
	try{
		pCheckCanDeform();
		pCalculateExtends();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

debpModel::~debpModel(){
	pCleanUp();
}



// Management
///////////////

void debpModel::PrepareOctree(){
	if(pOctree){
		return;
	}
	
	// NOTE if model data has been released RetainModelData() is required to be called first
	
	const deModelLOD &lod = *pModel.GetLODAt(0);
	const decVector offset(0.01f, 0.01f, 0.01f);
	const decVector minExtend(pExtends.minimum - offset);
	const decVector maxExtend(pExtends.maximum + offset);
	const int faceCount = lod.GetFaceCount();
	int i;
	
	pOctree = new debpModelOctree((minExtend + maxExtend) * 0.5f, (maxExtend - minExtend) * 0.5f);
	
	for(i=0; i<faceCount; i++){
		const deModelFace &face = lod.GetFaceAt(i);
		
		const deModelVertex &v1 = lod.GetVertexAt(face.GetVertex1());
		const deModelVertex &v2 = lod.GetVertexAt(face.GetVertex2());
		const deModelVertex &v3 = lod.GetVertexAt(face.GetVertex3());
		
		const decVector &posV1 = v1.GetPosition();
		const decVector &posV2 = v2.GetPosition();
		const decVector &posV3 = v3.GetPosition();
		
		const decVector faceMinExtend(posV1.Smallest(posV2).Smallest(posV3));
		const decVector faceMaxExtend(posV1.Largest(posV2).Largest(posV3));
		
		pOctree->InsertFaceIntoTree(i, (faceMinExtend + faceMaxExtend) * 0.5f, (faceMaxExtend - faceMinExtend) * 0.5f);
	}
}

void debpModel::PrepareNormals(){
	if(pNormals){
		return;
	}
	
	// NOTE if model data has been released RetainModelData() is required to be called first
	
	const deModelLOD &lod = *pModel.GetLODAt(0);
	const int normalCount = lod.GetVertexCount();
	if(normalCount == 0){
		return;
	}
	
	const int faceCount = lod.GetFaceCount();
	int i;
	
	pNormals = new decVector[normalCount];
	
	for(i=0; i<faceCount; i++){
		const deModelFace &face = lod.GetFaceAt(i);
		const decVector &faceNormal = face.GetFaceNormal();
		
		pNormals[face.GetVertex1()] += faceNormal;
		pNormals[face.GetVertex2()] += faceNormal;
		pNormals[face.GetVertex3()] += faceNormal;
	}
	
	for(i=0; i<normalCount; i++){
		const float len = pNormals[i].Length();
		
		if(len < FLOAT_SAFE_EPSILON){
			pNormals[i].Set(0.0f, 1.0f, 0.0f);
			
		}else{
			pNormals[i] /= len;
		}
	}
}

int debpModel::IndexOfFaceWithProbability(float probability) const{
	if(!pFaceProbabilities){
		DETHROW(deeInvalidParam); // somebody forgot to retain the data first
	}
	
	// to get it working a hack version first. later on this will be replaced by a binary
	// search which is a lot faster
	const int count = pModel.GetLODAt(0)->GetFaceCount();
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
	if(pFaceProbabilities){
		return;
	}
	
	// NOTE if model data has been released RetainModelData() is required to be called first
	
	const deModelLOD &lod = *pModel.GetLODAt(0);
	const int faceCount = lod.GetFaceCount();
	if(faceCount == 0){
		return;
	}
	
	float sumArea = 0.0f;
	int i;
	
	pFaceProbabilities = new float[faceCount];
	
	// calculate the area of the face as well as the sum of all face areas
	for(i=0; i<faceCount; i++){
		const deModelFace &face = lod.GetFaceAt(i);
		const decVector &v1 = lod.GetVertexAt(face.GetVertex1()).GetPosition();
		const decVector &v2 = lod.GetVertexAt(face.GetVertex2()).GetPosition();
		const decVector &v3 = lod.GetVertexAt(face.GetVertex3()).GetPosition();
		
		pFaceProbabilities[i] = ((v2 - v1) % (v3 - v1)).Length() * 0.5f;
		sumArea += pFaceProbabilities[i];
	}
	
	// the probability for a face is now the sum of all areas up to and including the face itself
	// devided by the total area sum. the entire array is thus a proability distribution in the end
	if(sumArea > FLOAT_SAFE_EPSILON){
		const float factor = 1.0f / sumArea;
		
		sumArea = 0.0f;
		for(i=0; i<faceCount; i++){
			sumArea += pFaceProbabilities[i];
			pFaceProbabilities[i] = sumArea * factor;
		}
		
	}else{
		for(i=0; i<faceCount; i++){
			pFaceProbabilities[i] = 1.0f;
		}
	}
}

void debpModel::PrepareShape(){
	if(pBulletShape){
		return;
	}
	
	// NOTE if model data has been released RetainModelData() is required to be called first
	
	const deModelLOD &lod = *pModel.GetLODAt(0);
	int i, j;
	
	// create vertex and face array suitable for the triangle mesh
	const int vertexCount = lod.GetVertexCount();
	btScalar *vertices = NULL;
	
	if(vertexCount > 0){
		vertices = new btScalar[vertexCount * 3];
		for(j=0, i=0; i<vertexCount; i++){
			const decVector &position = lod.GetVertexAt(i).GetPosition();
			vertices[j++] = position.x;
			vertices[j++] = position.y;
			vertices[j++] = position.z;
		}
	}
	
	const int faceCount = lod.GetFaceCount();
	int *faces = NULL;
	
	if(faceCount > 0){
		faces = new int[faceCount * 3];
		for(j=0, i=0; i<faceCount; i++){
			const deModelFace &face = lod.GetFaceAt(i);
			faces[j++] = face.GetVertex1();
			faces[j++] = face.GetVertex2();
			faces[j++] = face.GetVertex3();
		}
	}
	
	// create triangle mesh
	btTriangleIndexVertexArray * const ivarray = new btTriangleIndexVertexArray(
		faceCount, faces, sizeof(int) * 3, vertexCount, vertices, sizeof(btScalar) * 3);
	
	btBvhTriangleMeshShape * const meshShape = new btBvhTriangleMeshShape(ivarray, true, true);
	meshShape->setUserPointer(0); // means -1 => no shape index set
	
	pBulletShape.TakeOverWith(meshShape, ivarray, vertices, faces, vertexCount, faceCount);
}



// private functions
//////////////////////

void debpModel::pCleanUp(){
	
	if(pFaceProbabilities){
		delete [] pFaceProbabilities;
	}
	if(pBoneExtends){
		delete [] pBoneExtends;
		pBoneExtends = NULL;
	}
	if(pNormals){
		delete [] pNormals;
	}
	if(pWeightSets){
		delete [] pWeightSets;
	}
	if(pOctree){
		delete pOctree;
	}
}

void debpModel::pCheckCanDeform(){
	int weightCount = pModel.GetLODAt(0)->GetWeightCount();
	
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
	const deModelLOD &lod = *pModel.GetLODAt(0);
	const deModelVertex * const vertices = lod.GetVertices();
	const int vertexCount = lod.GetVertexCount();
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
	
	pBoneExtends = new sExtends[boneCount];
	pBoneCount = boneCount;
	
	const int weightGroupCount = lod.GetWeightGroupCount();
	if(weightGroupCount == 0){
		return;
	}
	
	const int * const weightGroups = lod.GetWeightGroups();
	
	int weightSetCount = 0;
	for(i=0; i<weightGroupCount; i++){
		weightSetCount += weightGroups[i];
	}
	
	if(weightSetCount == 0){
		return;
	}
	
	pWeightSets = new sWeightSet[weightSetCount];
	pWeightSetCount = weightSetCount;
	
	const deModelWeight *weightEntries = lod.GetWeights();
	int j;
	
	sWeightSet *weightPointer = pWeightSets;
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
	
	int * const dominatingBones = new int[pWeightSetCount];
	bool * const boneHasExtends = new bool[boneCount];
	for(i=0; i<boneCount; i++){
		boneHasExtends[i] = false;
	}
	
	for(i=0; i<pWeightSetCount; i++){
		dominatingBones[i] = -1;
		if(pWeightSets[i].first->GetWeight() > 0.001f){ // 0.499f){
			dominatingBones[i] = pWeightSets[i].first->GetBone();
		}
	}
	
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
	
	delete [] boneHasExtends;
	delete [] dominatingBones;
}
