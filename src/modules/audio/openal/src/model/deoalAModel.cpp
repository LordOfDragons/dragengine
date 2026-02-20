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

#include "deoalAModel.h"
#include "deoalModelFace.h"
#include "octree/deoalModelOctree.h"
#include "octree/deoalModelRTBVH.h"
#include "octree/deoalModelRTOctree.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../utils/cache/deoalRayCache.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/model/deModelFace.h>



// Class deoalAModel
/////////////////////

// Constructor, destructor
////////////////////////////

deoalAModel::deoalAModel(deoalAudioThread &audioThread, const deModel &model) :
pAudioThread(audioThread),
pFilename(model.GetFilename()),
// pRTSphereRadiusSquared( 0.0f ),
pOctree(nullptr),
pOctreeOverlap(nullptr),
// pRTOctree( NULL ),
pRTBVH(nullptr),
pRayCache(nullptr)
{
	// use the highest LOD level available for performance reasons
	const int lodCount = model.GetLODCount();
	if(lodCount < 1){
		return;
	}
	
	const deModelLOD &lod = model.GetLODAt(lodCount - 1);
	
	try{
		pInitBoneNames(model);
		pInitTextureNames(model);
		pBuildWeights(lod);
		pBuildFaces(lod);
// 		pInitRTSphere( lod );
// 		pRayCache = new deoalRayCache( pMinExtend, pMaxExtend );
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	LEAK_CHECK_CREATE(audioThread, Model);
}

deoalAModel::~deoalAModel(){
	LEAK_CHECK_FREE(pAudioThread, Model);
	
	pCleanUp();
}



// Management
///////////////

const deoalModelFace &deoalAModel::GetFaceAt(int index) const{
	return pFaces[index];
}

void deoalAModel::PrepareOctree(){
	if(pOctree){
		return;
	}
	
	pBuildOctree();
}



// Private Functions
//////////////////////

void deoalAModel::pCleanUp(){
	if(pRayCache){
		delete pRayCache;
	}
	if(pRTBVH){
		delete pRTBVH;
	}
// 	if( pRTOctree ){
// 		delete pRTOctree;
// 	}
	if(pOctree){
		delete pOctree;
	}
	if(pOctreeOverlap){
		delete pOctreeOverlap;
	}
}

void deoalAModel::pInitBoneNames(const deModel &model){
	const int count = model.GetBoneCount();
	int i;
	
	for(i=0; i<count; i++){
		pBoneNames.Add(model.GetBoneAt(i)->GetName());
	}
}

void deoalAModel::pInitTextureNames(const deModel &model){
	const int count = model.GetTextureCount();
	int i;
	
	for(i=0; i<count; i++){
		pTextureNames.Add(model.GetTextureAt(i)->GetName());
	}
}

void deoalAModel::pBuildWeights(const deModelLOD &lod){
	const decTList<int> &weightGroups = lod.GetWeightGroups();
	const decTList<deModelWeight> &weights = lod.GetWeights();
	const int weightGroupCount = weightGroups.GetCount();
	const int weightCount = weights.GetCount();
	
	if(weightCount > 0){
		pWeights.EnlargeCapacity(weightCount);
		int i;
		for(i=0; i<weightCount; i++){
			pWeights.Add({weights[i].GetBone(), weights[i].GetWeight()});
		}
	}
	
	if(weightGroupCount > 0){
		int weightSetCount = 0;
		int i;
		for(i=0; i<weightGroupCount; i++){
			weightSetCount += weightGroups[i];
		}
		
		if(weightSetCount){
			pWeightSets.EnlargeCapacity(weightSetCount);
			
			int weightIndex = 0;
			int j;
			
			for(i=0; i<weightGroupCount; i++){
				const int setsInGroupCount = weightGroups[i];
				const int weightsInSetCount = i + 1;
				
				for(j=0; j<setsInGroupCount; j++){
					pWeightSets.Add({weightIndex, weightsInSetCount});
					weightIndex += weightsInSetCount;
				}
			}
		}
	}
}

void deoalAModel::pBuildFaces(const deModelLOD &lod){
	const int faceCount = lod.GetFaces().GetCount();
	if(faceCount == 0){
		return;
	}
	
	pFaces.AddRange(faceCount, {});
	pFaces.VisitIndexed([&](int i, deoalModelFace &face){
		face.Init(lod, i, lod.GetFaces()[i]);
		
		if(i > 0){
			pMinExtend.SetSmallest(face.GetMinExtend());
			pMaxExtend.SetLargest(face.GetMaxExtend());
			
		}else{
			pMinExtend = face.GetMinExtend();
			pMaxExtend = face.GetMaxExtend();
		}
	});
	
	pSize = pMaxExtend - pMinExtend;
}

void deoalAModel::pBuildOctree(){
	const decVector size(pMaxExtend - pMinExtend);
	
// 	pAudioThread.GetLogger().LogInfoFormat(
// 		"ModelOctree.BuildOctree(%s) extends=(%.3f,%.3f,%.3f)-(%.3f,%.3f,%.3f)"
// 		" size=(%.3f,%.3f,%.3f) faces=%d", pFilename.GetString(), pMinExtend.x, pMinExtend.y,
// 		pMinExtend.z, pMaxExtend.x, pMaxExtend.y, pMaxExtend.z, size.x, size.y, size.z, pFaceCount );
	
	pOctree = new deoalModelOctree((pMinExtend + pMaxExtend) * 0.5f, size * 0.5f);
	
// 	const float maxHalfSize = decMath::max( size.x, size.y, size.z ) * 0.5f;
// 	pOctree = new deoalModelOctree( ( pMinExtend + pMaxExtend ) * 0.5f,
// 		decVector( maxHalfSize, maxHalfSize, maxHalfSize ) );
	
	// what maximum depth to use? for environment models like rooms large triangles are typically
	// present so large depths are not helping much. for props the density of triangles can be
	// high in which case a larger depth could help. using 5 for the time being until it is
	// clearer what value works best
	const int maxDepth = 5;
	int i;
	for(i=0; i<pFaces.GetCount(); i++){
		pOctree->InsertFaceIntoTree(&pFaces[i], maxDepth);
	}
	
	// overlapping octree for faster ray tracing
#if 0
	const float maxSize = decMath::max(size.x, size.y, size.z);
	const float maxHalfSize = maxSize * 0.5f;
	pOctreeOverlap = new deoalModelOctree((pMinExtend + pMaxExtend) * 0.5f,
		decVector(maxHalfSize, maxHalfSize, maxHalfSize));
	
	const float overlapNodeSize = 0.1;
	const int minOverlapDepth = 4;
	const int maxOverlapDepth = 8;
	const int overlapDepth = 3 + 0*decMath::clamp((int)((maxSize / overlapNodeSize) + 0.5f),
		minOverlapDepth, maxOverlapDepth);
	pAudioThread.GetLogger().LogInfoFormat("ModelOctree.BuildOctree(%s) Overlap depth=%d",
		pFilename.GetString(), overlapDepth);
	
	for(i=0; i<pFaceCount; i++){
		pOctreeOverlap->InsertFaceOverlap(pFaces + i, overlapDepth);
	}
#endif
	
	// ray-tracing optimized octree
// 	pRTOctree = new deoalModelRTOctree( *pOctree );
	pRTBVH = new deoalModelRTBVH;
	pRTBVH->Build(pFaces.GetArrayPointer(), pFaces.GetCount());
	pRTBVH->DropBuildData();
	
	// debug
// 	pDebugLogOctreePerfMetrics( *pOctree );
// 	pDebugLogOctreePerfMetrics( *pOctreeOverlap );
	// allocated. then the original octree is visited to fill in data.
}

/*
void deoalAModel::pInitRTSphere(const deModelLOD &lod){
	const int vertexCount = lod.GetVertexCount();
	if(vertexCount == 0){
		return;
	}
	
	int i;
	
	pRTSphereCenter.SetZero();
	for(i=0; i<vertexCount; i++){
		pRTSphereCenter += lod.GetVertexAt(i).GetPosition();
	}
	pRTSphereCenter /= (float)vertexCount;
	
	pRTSphereRadiusSquared = 0.0f;
	for(i=0; i<vertexCount; i++){
		const float distSquared = (lod.GetVertexAt(i).GetPosition() - pRTSphereCenter).LengthSquared();
		if(distSquared > pRTSphereRadiusSquared){
			pRTSphereRadiusSquared = distSquared;
		}
	}
}
*/



void deoalAModel::pDebugLogOctreePerfMetrics(const deoalModelOctree &octree){
	int faces[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i;
	for(i=0; i<9; i++){
		pDebugLogOctreePerfMetrics(octree, i, faces[i]);
	}
	pAudioThread.GetLogger().LogInfoFormat(
		"ModelOctree.PerfMetrics(%s) faces=(%d, %d, %d, %d, %d, %d, %d, %d, %d)",
		pFilename.GetString(), faces[0], faces[1], faces[2], faces[3], faces[4],
		faces[5], faces[6], faces[7], faces[8]);
}

void deoalAModel::pDebugLogOctreePerfMetrics(const deoalModelOctree &node, int level, int &count){
	if(level == 0){
		count += node.GetFaceCount();
		return;
	}
	
	int i;
	for(i=0; i<8; i++){
		if(node.GetNodeAt(i)){
			pDebugLogOctreePerfMetrics((const deoalModelOctree &)*node.GetNodeAt(i), level - 1, count);
		}
	}
}
