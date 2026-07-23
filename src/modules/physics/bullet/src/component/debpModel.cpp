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
#include "debpShapeGenerator.h"
#include "../dePhysicsBullet.h"
#include "../coldet/octree/debpDefaultDOctree.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigBuilder.h>
#include <dragengine/resources/rig/deRigManager.h>

#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "LinearMath/btConvexHull.h"
#include "Extras/VHACD/public/VHACD.h"



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


void debpModel::PrepareModelShapes(){
	if(pModelShapesGenerated){
		return;
	}
	
	// decompose mesh into multiple convex hulls
	const deModelLOD &lod = pModel.GetLODs().First();
	const int faceCount = lod.GetFaces().GetCount();
	if(faceCount == 0){
		pModelShapesGenerated = true;
		return;
	}
	
	// collect weightless vertices and their indices
	const int vertexCount = lod.GetVertices().GetCount();
	decTList<double> vertices;
	decTList<int> indices;
	decTList<int> vertexIndexMap(vertexCount, -1);
	
	lod.GetVertices().VisitIndexed([&](int i, const deModelVertex &vertex){
		if(vertex.GetWeightSet() != -1){
			return;
		}
		
		vertexIndexMap[i] = vertices.GetCount() / 3;
		const decVector &position = vertex.GetPosition();
		vertices.Add((double)position.x);
		vertices.Add((double)position.y);
		vertices.Add((double)position.z);
	});
	
	if(vertices.GetCount() < 12){ // less than 4 vertices
		pModelShapesGenerated = true;
		return;
	}
	
	// build triangle indices
	lod.GetFaces().Visit([&](const deModelFace &face){
		int i0 = vertexIndexMap[face.GetVertex1()];
		int i1 = vertexIndexMap[face.GetVertex2()];
		int i2 = vertexIndexMap[face.GetVertex3()];
		
		// only add faces where all vertices are weightless
		if(i0 == -1 || i1 == -1 || i2 == -1){
			return;
		}
		
		indices.Add(i0);
		indices.Add(i1);
		indices.Add(i2);
	});
	
	if(indices.GetCount() < 9){ // less than 3 triangles
		pModelShapesGenerated = true;
		return;
	}
	
	// apply decomposition
	class AutoReleaseVHACD{
		VHACD::IVHACD* pVHACD;
		
	public:
		AutoReleaseVHACD(VHACD::IVHACD* pointer) : pVHACD(pointer){}
		~AutoReleaseVHACD(){
			if(pVHACD){
				pVHACD->Release();
			}
		}
		VHACD::IVHACD* operator->(){ return pVHACD; }
		operator bool() const{ return pVHACD != nullptr; }
	};
	
	auto vhacd = AutoReleaseVHACD(VHACD::CreateVHACD());
	if(!vhacd){
		pModelShapesGenerated = true;
		return;
	}
	
	// decompose
	VHACD::IVHACD::Parameters params;
	params.m_resolution = 100000;
	params.m_depth = 10;
	params.m_concavity = 0.01;
	params.m_planeDownsampling = 8; // higher=faster
	params.m_convexhullDownsampling = 8; // higher=faster
	params.m_alpha = 0.05; // axis alignment weight
	params.m_beta = 0.05; // clipping plane weight
	params.m_gamma = 0.0005; // scaling weight
	params.m_maxNumVerticesPerCH = 32; // max vertices per convex hull, prevent tiny fragments
	params.m_minVolumePerCH = 0.001; // min volume threshold, prevent tiny fragments
	params.m_mode = 0; // 0: voxel-based, 1: tetrahedron-based
	params.m_convexhullApproximation = 1; // boolean value hidden as int;
	
	if(!vhacd->Compute(vertices.GetArrayPointer(), 3, vertices.GetCount() / 3,
	indices.GetArrayPointer(), 3, indices.GetCount() / 3, params)){
		pModelShapesGenerated = true;
		return;
	}
	
	// process convex hulls
	debpShapeGenerator generator;
	unsigned int hullCount = vhacd->GetNConvexHulls();
	
	for(unsigned int h=0; h<hullCount; h++){
		VHACD::IVHACD::ConvexHull hull;
		vhacd->GetConvexHull(h, hull);
		if(hull.m_nPoints < 4){
			continue; // degenerate hull
		}
		
		debpShapeGenerator::WeightList weights;
		for(unsigned int i=0; i<hull.m_nPoints; i++){
			const double * const p = &hull.m_points[i * 3];
			weights.Add({{(float)p[0], (float)p[1], (float)p[2]}, 1.0f});
		}
		
		auto shapes = generator.Create(weights);
		if(shapes){
			shapes->Visit([&](const decShape::Ref &shape){
				if(!pModelShapes){
					pModelShapes = deTUniqueReference<decShape::List>::New();
				}
				pModelShapes->Add(std::move(shape->Copy()));
			});
		}
	}
	
	pModelShapesGenerated = true;
}

void debpModel::PrepareBoneShapes(){
	if(pBoneShapesPrepared){
		return;
	}
	
	const deModelLOD &lod = pModel.GetLODs().First();
	const deModelBone::List &bones = pModel.GetBones();
	if(bones.IsEmpty() || lod.GetVertices().IsEmpty()){
		pBoneShapesPrepared = true;
		return;
	}
	
	// collect vertices per bone with weights
	const int boneCount = bones.GetCount();
	decTList<debpShapeGenerator::WeightList> weights(boneCount, debpShapeGenerator::WeightList());
	
	lod.GetVertices().Visit([&](const deModelVertex &vertex){
		if(vertex.GetWeightSet() == -1){
			return;
		}
		
		const sWeightSet &weightSet = pWeightSets[vertex.GetWeightSet()];
		
		// the idea here is the following. weights of 50% to 100% are used for the main
		// deformation bone. weights of 25% to 50% are used for the neightbor bones to properly
		// deform joints. weights below 25% are used to smoothen the deformation. of all these
		// weights the main deformation bone is the important one for physics to avoid bone
		// collision shapes overlapping. some overlap though helps to avoid other colliders
		// slipping in and getting stuck inside joints. for this reason a threshold of 35% is
		// used to catch extend collision shapes into neighbor collision shapes without bloating
		// them up too much in size. the threshold is applied to the maximum weight since weights
		// in models are not required to be normalized
		
		float maxWeight = 0.0f;
		for(int w=0; w<weightSet.count; w++){
			maxWeight = decMath::max(maxWeight, weightSet.first[w].GetWeight());
		}
		
		const float weightThreshold = maxWeight * 0.35f;
		
		for(int w=0; w<weightSet.count; w++){
			const deModelWeight &modelWeight = weightSet.first[w];
			
			const int bone = modelWeight.GetBone();
			if(bone < 0 || bone >= boneCount){
				return; // should never happen but better safe than sorry
			}
			
			const float weight = modelWeight.GetWeight();
			if(weight < weightThreshold){
				continue;
			}
			
			weights[bone].Add({bones[bone]->GetInverseMatrix() * vertex.GetPosition(), weight});
		}
	});
	
	// create analytic shapes for each bone
	pBoneShapes.EnlargeCapacity(boneCount);
	
	debpShapeGenerator generator;
	weights.Visit([&](const debpShapeGenerator::WeightList &w){
		pBoneShapes.Add(w.GetCount() >= 4 ? generator.Create(w) : debpShapeGenerator::ShapeListRef());
	});
	
	pBoneShapesPrepared = true;
}

deRig::Ref debpModel::GenerateCollisionShapes(){
	class cRigBuilder : public deRigBuilder{
	public:
		debpModel &pModel;
		
		cRigBuilder(debpModel &model) : pModel(model){}
		
		void BuildRig(deRig *rig) override{
			const auto &bones = pModel.GetModel().GetBones();
			
			if(bones.IsNotEmpty()){
				const auto &bonesShapes = pModel.GetBoneShapes();
				int rootBoneIndex = -1;
				
				bones.VisitIndexed([&](int i, const deModelBone &modelBone){
					auto rigBone = deRigBone::Ref::New(modelBone.GetName());
					rigBone->SetPosition(modelBone.GetPosition());
					rigBone->SetRotation(modelBone.GetOrientation().GetEulerAngles());
					
					const auto &shapes = bonesShapes.GetAt(i);
					if(shapes){
						rigBone->SetShapes(shapes);
					}
					
					rigBone->SetParent(modelBone.GetParent());
					if(rigBone->GetParent() == -1 && rootBoneIndex == -1){
						rootBoneIndex = i;
					}
					
					rig->AddBone(std::move(rigBone));
				});
				
				rig->SetRootBone(rootBoneIndex);
				
			}else{
				const auto &modelShapes = pModel.GetModelShapes();
				if(modelShapes){
					rig->SetShapes(modelShapes);
				}
			}
		}
	};
	
	if(pModel.GetBones().IsNotEmpty()){
		PrepareBoneShapes();
		
	}else{
		PrepareModelShapes();
	}
	
	cRigBuilder builder(*this);
	return pBullet.GetGameEngine()->GetRigManager()->CreateRig("", builder);
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
