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

#include "deoalModelRTBVH.h"
#include "../deoalAModel.h"
#include "../deoalModelFace.h"

#include <dragengine/common/exceptions.h>


// Class deoalModelRTBVH
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalModelRTBVH::deoalModelRTBVH() :
pIndexNode(0),
pIndexFace(0){
}

deoalModelRTBVH::~deoalModelRTBVH() = default;



// Management
///////////////

void deoalModelRTBVH::Build(const deoalModelFace *faces, int faceCount){
	pNodes.RemoveAll();
	pFaces.RemoveAll();
	pBuildFaces.RemoveAll();
	pBuildNodes.RemoveAll();
	if(faceCount == 0){
		return;
	}
	
	pFaces.AddRange(faceCount, {});
	
	pBuildFaces.SetCountDiscard(faceCount);
	pBuildFaces.VisitIndexed([&](int i, sBuildFace &buildFace){
		const deoalModelFace &modelFace = faces[i];
		buildFace.minExtend = modelFace.GetMinExtend();
		buildFace.maxExtend = modelFace.GetMaxExtend();
		buildFace.center = (buildFace.minExtend + buildFace.maxExtend) * 0.5f;
		buildFace.modelFace = &modelFace;
		buildFace.next = i + 1;
	});
	pBuildFaces.Last().next = -1;
	
	// build root node
	pAddBuildNode();
	sBuildNode &rootNode = pBuildNodes.First();
	rootNode.faceCount = pBuildFaces.GetCount();
	rootNode.firstFace = 0;
	rootNode.lastFace = pBuildFaces.GetCount() - 1;
	pUpdateNodeExtends(rootNode);
	
	// split root node
	pSplitNode(0);
	
	// build visit arrays
	pNodes.AddRange(pBuildNodes.GetCount(), {});
	pFaces.AddRange(pBuildFaces.GetCount(), {});
	
	pIndexNode = 0;
	pIndexFace = 0;
	pBuildVisitNode(pBuildNodes[0]);
}

void deoalModelRTBVH::DropBuildData(){
	pBuildNodes.RemoveAll();
	pBuildFaces.RemoveAll();
}



// Private Functions
//////////////////////

int deoalModelRTBVH::pAddBuildNode(){
	pBuildNodes.Add({});
	sBuildNode &node = pBuildNodes.Last();
	node.child1 = -1;
	node.child2 = -1;
	node.firstFace = -1;
	node.lastFace = -1;
	node.faceCount = 0;
	return pBuildNodes.GetCount() - 1;
}

void deoalModelRTBVH::pUpdateNodeExtends(sBuildNode &node) const{
	if(node.firstFace == -1){
		node.center.SetZero();
		node.halfSize.SetZero();
		return;
	}
	
	const sBuildFace &firstFace = pBuildFaces[node.firstFace];
	decVector minExtend(firstFace.minExtend);
	decVector maxExtend(firstFace.maxExtend);
	int faceIndex = firstFace.next;
	
	while(faceIndex != -1){
		const sBuildFace &face = pBuildFaces[faceIndex];
		minExtend.SetSmallest(face.minExtend);
		maxExtend.SetLargest(face.maxExtend);
		faceIndex = face.next;
	}
	
	node.center = (minExtend + maxExtend) * 0.5f;
	node.halfSize = (maxExtend - minExtend) * 0.5f;
}

void deoalModelRTBVH::pSplitNode(int nodeIndex){
	if(pBuildNodes[nodeIndex].faceCount < 3){
		return;
	}
	
	// find center extends. we need to use this instead of the node extends since only this
	// shows us in corner cases if a split is possible
	const sBuildFace &firstFace = pBuildFaces[pBuildNodes[nodeIndex].firstFace];
	decVector centerMin(firstFace.center);
	decVector centerMax(firstFace.center);
	int faceIndex = firstFace.next;
	
	while(faceIndex != -1){
		const sBuildFace &face = pBuildFaces[faceIndex];
		centerMin.SetSmallest(face.center);
		centerMax.SetLargest(face.center);
		faceIndex = face.next;
	}
	
	// find primary axis
	const decVector centerSize(centerMax - centerMin);
	const float splitThreshold = 0.1f; //0.001f; // 0.01f
	float splitPosition;
	int primaryAxis;
	
	if(centerSize.x >= centerSize.y){
		if(centerSize.x >= centerSize.z){
			if(centerSize.x < splitThreshold){
				return;
			}
			splitPosition = (centerMin.x + centerMax.x) * 0.5f;
			primaryAxis = 0;
			
		}else{
			if(centerSize.z < splitThreshold){
				return;
			}
			splitPosition = (centerMin.z + centerMax.z) * 0.5f;
			primaryAxis = 2;
		}
		
	}else if(centerSize.y >= centerSize.z){
		if(centerSize.y < splitThreshold){
			return;
		}
		splitPosition = (centerMin.y + centerMax.y) * 0.5f;
		primaryAxis = 1;
		
	}else{
		if(centerSize.z < splitThreshold){
			return;
		}
		splitPosition = (centerMin.z + centerMax.z) * 0.5f;
		primaryAxis = 2;
	}
	
	// create child nodes
	const int childIndex1 = pAddBuildNode();
	const int childIndex2 = pAddBuildNode();
	
	sBuildNode &node = pBuildNodes[nodeIndex];
	sBuildNode &child1 = pBuildNodes[childIndex1];
	sBuildNode &child2 = pBuildNodes[childIndex2];
	
	faceIndex = node.firstFace;
	
	switch(primaryAxis){
	case 0: // x largest
		while(faceIndex != -1){
			sBuildFace &face = pBuildFaces[faceIndex];
			const int nextFaceIndex = face.next;
			face.next = -1;
			
			if(face.center.x < splitPosition){
				pNodeAddFace(child1, faceIndex);
				
			}else{
				pNodeAddFace(child2, faceIndex);
			}
			
			faceIndex = nextFaceIndex;
		}
		break;
		
	case 1: // y largest
		while(faceIndex != -1){
			sBuildFace &face = pBuildFaces[faceIndex];
			const int nextFaceIndex = face.next;
			face.next = -1;
			
			if(face.center.y < splitPosition){
				pNodeAddFace(child1, faceIndex);
				
			}else{
				pNodeAddFace(child2, faceIndex);
			}
			
			faceIndex = nextFaceIndex;
		}
		break;
		
	case 2: // z largest
		while(faceIndex != -1){
			sBuildFace &face = pBuildFaces[faceIndex];
			const int nextFaceIndex = face.next;
			face.next = -1;
			
			if(face.center.z < splitPosition){
				pNodeAddFace(child1, faceIndex);
				
			}else{
				pNodeAddFace(child2, faceIndex);
			}
			
			faceIndex = nextFaceIndex;
		}
		break;
	}
	
	node.faceCount = 0;
	node.firstFace = -1;
	node.lastFace = -1;
	node.child1 = childIndex1;
	node.child2 = childIndex2;
	
	pUpdateNodeExtends(child1);
	pUpdateNodeExtends(child2);
	
	pSplitNode(childIndex1);
	pSplitNode(childIndex2);
}

void deoalModelRTBVH::pNodeAddFace(sBuildNode &node, int faceIndex){
	if(node.lastFace != -1){
		pBuildFaces[node.lastFace].next = faceIndex;
		
	}else{
		node.firstFace = faceIndex;
	}
	
	node.lastFace = faceIndex;
	node.faceCount++;
}

void deoalModelRTBVH::pBuildVisitNode(const sBuildNode &buildNode){
	sNode &visitNode = pNodes[pIndexNode++];
	visitNode.center = buildNode.center;
	visitNode.halfSize = buildNode.halfSize;
	
	// components
	if(buildNode.faceCount > 0){
		visitNode.firstFace = pIndexFace;
		visitNode.faceCount = buildNode.faceCount;
		visitNode.node1 = -1;
		visitNode.node2 = -1;
		
		int indexBuildFace = buildNode.firstFace;
		while(indexBuildFace != -1){
			const sBuildFace &buildFace = pBuildFaces[indexBuildFace];
			sFace &visitFace = pFaces[pIndexFace++];
			const deoalModelFace &modelFace = *buildFace.modelFace;
			
			visitFace.normal = modelFace.GetNormal();
			visitFace.baseVertex = modelFace.GetVertex1();
			visitFace.edgeNormal[0] = modelFace.GetEdge1Normal();
			visitFace.edgeNormal[1] = modelFace.GetEdge2Normal();
			visitFace.edgeNormal[2] = modelFace.GetEdge3Normal();
			visitFace.edgeDistance[0] = modelFace.GetEdge1DistanceSafe();
			visitFace.edgeDistance[1] = modelFace.GetEdge2DistanceSafe();
			visitFace.edgeDistance[2] = modelFace.GetEdge3DistanceSafe();
			visitFace.indexFace = modelFace.GetIndex();
			visitFace.indexTexture = modelFace.GetTexture();
			
			indexBuildFace = buildFace.next;
		}
		
	// child nodes
	}else if(buildNode.child1 != -1){
		visitNode.firstFace = -1;
		visitNode.faceCount = 0;
		
		visitNode.node1 = pIndexNode;
		pBuildVisitNode(pBuildNodes[buildNode.child1]);
		
		visitNode.node2 = pIndexNode;
		pBuildVisitNode(pBuildNodes[buildNode.child2]);
		
	// this should never happen
	}else{
		DETHROW(deeInvalidParam);
	}
}
