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

#include "deoalModelRTOctree.h"
#include "deoalModelOctree.h"
#include "deoalModelOctreeVisitor.h"
#include "../deoalModelFace.h"

#include <dragengine/common/exceptions.h>



// Visitors
/////////////

class deoalModelRTOctreeCalcCounts : public deoalModelOctreeVisitor {
public:
	int pNodeCount;
	int pFaceCount;
	
	deoalModelRTOctreeCalcCounts() :
	pNodeCount(0),
	pFaceCount(0){
	}
	
	inline int GetNodeCount() const{ return pNodeCount; }
	inline int GetFaceCount() const{ return pFaceCount; }
	inline bool IsEmpty() const{ return pNodeCount == 0 || pFaceCount == 0; }
	
	virtual void VisitNode(deoalOctree *node, int /*intersection*/){
		pNodeCount++;
		pFaceCount += ((deoalModelOctree*)node)->GetFaceCount();
	}
};


class deoalModelRTOctreeBuild{
private:
	deoalModelRTOctree::sFace *pFaces;
	deoalModelRTOctree::sNode *pNodes;
	int pIndexNode;
	int pIndexChild;
	int pIndexFace;
	
public:
	deoalModelRTOctreeBuild(deoalModelRTOctree::sFace *faces, deoalModelRTOctree::sNode *nodes) :
	pFaces(faces),
	pNodes(nodes),
	pIndexNode(0),
	pIndexChild(1),
	pIndexFace(0){
	}
	
	void VisitNode(deoalModelOctree &node){
		deoalModelRTOctree::sNode &curNode = pNodes[pIndexNode];
		int i;
		
		curNode.center = node.GetCenter();
		curNode.halfSize = node.GetHalfSize();
		
		// pFaces
		const int faceCount = node.GetFaceCount();
		curNode.firstFace = pIndexFace;
		curNode.faceCount = faceCount;
		
		for(i=0; i<faceCount; i++){
			const deoalModelFace &modelFace = *node.GetFaceAt(i);
			deoalModelRTOctree::sFace &face = pFaces[pIndexFace++];
			
			face.normal = modelFace.GetNormal();
			face.baseVertex = modelFace.GetVertex1();
			face.edgeNormal[0] = modelFace.GetEdge1Normal();
			face.edgeNormal[1] = modelFace.GetEdge2Normal();
			face.edgeNormal[2] = modelFace.GetEdge3Normal();
			face.edgeDistance[0] = modelFace.GetEdge1DistanceSafe();
			face.edgeDistance[1] = modelFace.GetEdge2DistanceSafe();
			face.edgeDistance[2] = modelFace.GetEdge3DistanceSafe();
			face.indexFace = modelFace.GetIndex();
			face.indexTexture = modelFace.GetTexture();
		}
		
		// child pNodes
		curNode.firstNode = pIndexChild;
		curNode.nodeCount = 0;
		for(i=0; i<8; i++){
			if(node.GetNodeAt(i)){
				curNode.nodeCount++;
			}
		}
		pIndexChild += curNode.nodeCount;
		
		int iterIndex = curNode.firstNode;
		for(i=0; i<8; i++){
			deoalOctree * const child = node.GetNodeAt(i);
			if(child){
				pIndexNode = iterIndex++;
				VisitNode(*((deoalModelOctree*)child));
			}
		}
	}
};



// Class deoalModelRTOctree
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalModelRTOctree::deoalModelRTOctree(deoalModelOctree &octree) :
pFaces(NULL),
pFaceCount(0),
pNodes(NULL),
pNodeCount(0)
{
	try{
		deoalModelRTOctreeCalcCounts visitorCounts;
		octree.VisitNodes(&visitorCounts);
		if(visitorCounts.IsEmpty()){
			return;
		}
		
		pFaceCount = visitorCounts.GetFaceCount();
		pFaces = new sFace[pFaceCount];
		
		pNodeCount = visitorCounts.GetNodeCount();
		pNodes = new sNode[pNodeCount];
		
		deoalModelRTOctreeBuild(pFaces, pNodes).VisitNode(octree);
		
	}catch(const deException &e){
		e.PrintError();
		pCleanUp();
		throw;
	}
}

deoalModelRTOctree::~deoalModelRTOctree(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoalModelRTOctree::pCleanUp(){
	if(pNodes){
		delete [] pNodes;
	}
	if(pFaces){
		delete [] pFaces;
	}
}
