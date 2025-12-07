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

#include "deoalRTWorldOctree.h"

#include "../../component/deoalAComponent.h"

#include <dragengine/common/exceptions.h>




// Class deoalRTWorldOctree
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTWorldOctree::deoalRTWorldOctree() :
pPosition(),
pHalfExtends(),
pMaxDepth(8), //4
pBuildNodes(NULL),
pBuildNodeCount(0),
pBuildNodeSize(0),
pBuildComponents(NULL),
pBuildComponentCount(0),
pBuildComponentSize(0),
pVisitNodes(NULL),
pVisitNodeCount(0),
pVisitNodeSize(0),
pVisitComponents(NULL),
pVisitComponentCount(0),
pVisitComponentSize(0)
{
}

deoalRTWorldOctree::~deoalRTWorldOctree(){
	if(pVisitComponents){
		delete [] pVisitComponents;
	}
	if(pVisitNodes){
		delete [] pVisitNodes;
	}
	if(pBuildComponents){
		delete [] pBuildComponents;
	}
	if(pBuildNodes){
		delete [] pBuildNodes;
	}
}



// Building
/////////////

void deoalRTWorldOctree::Build(const decDVector &position, const decVector &halfExtends){
	pVisitComponentCount = 0;
	pVisitNodeCount = 0;
	pBuildComponentCount = 0;
	pBuildNodeCount = 0;
	
	pPosition = position;
	pHalfExtends = halfExtends;
	
	pAddBuildNode();
	pBuildNodes[0].center.SetZero();
	pBuildNodes[0].halfSize = halfExtends;
}

void deoalRTWorldOctree::Build(const decDVector &position, double radius){
	Build(position, decDVector(radius, radius, radius));
}

void deoalRTWorldOctree::AddComponent(deoalAComponent *component){
	if(! component){
		DETHROW(deeInvalidParam);
	}
	
	const decVector minExtend(component->GetMinExtend() - pPosition);
	const decVector maxExtend(component->GetMaxExtend() - pPosition);
	
	const int indexComponent = pAddBuildComponent();
	sBuildComponent &buildComponent = pBuildComponents[indexComponent];
	buildComponent.center = (minExtend + maxExtend) * 0.5f;
	buildComponent.halfSize = (maxExtend - minExtend) * 0.5f;
// 	buildComponent.sphereCenter = component->GetRTSphereCenter() - pPosition;
// 	buildComponent.sphereRadiusSquared = component->GetRTSphereRadiusSquared();
	buildComponent.component = component;
	
	const int nodeIndex = pGetBuildNodeFor(buildComponent.center, buildComponent.halfSize);
		// WARNING do not combine this line with the line below! the compiler does not know
		//         that pBuildNodes can be modified inside pGetBuildNodeFor. due to compiler
		//         optimizations the value of pBuildNodes is stored away before evaluating
		//         the array index. if pBuildNodes then changes inside pGetBuildNodeFor
		//         then the compiler uses the old array pointer stored away for optimization
		//         purpose and causes segfaults down the line
	sBuildNode &node = pBuildNodes[nodeIndex];
	
	if(node.lastComponent != -1){
		pBuildComponents[node.lastComponent].next = indexComponent;
		
	}else{
		node.firstComponent = indexComponent;
	}
	
	node.lastComponent = indexComponent;
	node.componentCount++;
}

void deoalRTWorldOctree::Finish(){
	if(pBuildNodeCount == 0){
		DETHROW(deeInvalidParam);
	}
	
// 	pGatherNodeCount = 1;
// 	pGatherComponentCount = 0;
// 	pGatherCounts( pBuildNodes[ 0 ] );
// 	if( pGatherNodeCount != pBuildNodeCount || pGatherComponentCount != pBuildComponentCount ){
// 		DETHROW( deeInvalidParam );
// 	}
	
	if(pBuildNodeCount > pVisitNodeSize){
		sVisitNode * const nodes = new sVisitNode[pBuildNodeCount];
		if(pVisitNodes){
			delete [] pVisitNodes;
		}
		pVisitNodes = nodes;
		pVisitNodeSize = pBuildNodeCount;
	}
	
	if(pBuildComponentCount > pVisitComponentSize){
		sVisitComponent * const components = new sVisitComponent[pBuildComponentCount];
		if(pVisitComponents){
			delete [] pVisitComponents;
		}
		pVisitComponents = components;
		pVisitComponentSize = pBuildComponentCount;
	}
	
	pVisitComponentCount = pBuildComponentCount;
	pVisitNodeCount = pBuildNodeCount;
	
	pIndexChild = 1;
	pIndexComponent = 0;
	pWorldMatrix.SetTranslation(pPosition);
	pBuildVisitNode(pBuildNodes[0], pVisitNodes[0]);
}



// Private Functions
//////////////////////

int deoalRTWorldOctree::pGetBuildNodeFor(const decVector &center, const decVector &halfSize){
	int i, node = 0;
	
	for(i=0; i<pMaxDepth; i++){
		const int next = pGetBuildNodeFor(node, center, halfSize);
		if(next == -1){
			break;
		}
		node = next;
	}
	
	return node;
}

int deoalRTWorldOctree::pGetBuildNodeFor(int nodeIndex, const decVector &center,
const decVector &halfSize){
	const int octant = pGetBuildOctantFor(pBuildNodes[nodeIndex], center, halfSize);
	if(octant == -1){
		return -1;
	}
	
	const int childIndex = pBuildNodes[nodeIndex].child[octant];
	if(childIndex != -1){
		return childIndex;
	}
	
	const int newNodeIndex = pAddBuildNode();
		// WARNING pAddBuildNode potentially reallocates nodes array. node references
		//         beyond this point will be invalid!
	
	sBuildNode &node = pBuildNodes[nodeIndex];
	node.child[octant] = newNodeIndex;
	node.childCount++;
	
	sBuildNode &child = pBuildNodes[newNodeIndex];
	child.halfSize = node.halfSize * 0.5f;
	
	if((octant & 4) == 4){
		child.center.x = node.center.x + child.halfSize.x;
		
	}else{
		child.center.x = node.center.x - child.halfSize.x;
	}
	
	if((octant & 2) == 2){
		child.center.y = node.center.y + child.halfSize.y;
		
	}else{
		child.center.y = node.center.y - child.halfSize.y;
	}
	
	if((octant & 1) == 1){
		child.center.z = node.center.z + child.halfSize.z;
		
	}else{
		child.center.z = node.center.z - child.halfSize.z;
	}
	
	return newNodeIndex;
}

int deoalRTWorldOctree::pGetBuildOctantFor(sBuildNode &node,
const decVector &center, const decVector &halfSize) const{
	int octant = 0;
	
	if(center.x - halfSize.x >= node.center.x){
		octant |= 4;
		
	}else if(center.x + halfSize.x >= node.center.x){
		return -1;
	}
	
	if(center.y - halfSize.y >= node.center.y){
		octant |= 2;
		
	}else if(center.y + halfSize.y >= node.center.y){
		return -1;
	}
	
	if(center.z - halfSize.z >= node.center.z){
		octant |= 1;
		
	}else if(center.z + halfSize.z >= node.center.z){
		return -1;
	}
	
	return octant;
}

int deoalRTWorldOctree::pAddBuildNode(){
	if(pBuildNodeCount == pBuildNodeSize){
		const int size = pBuildNodeSize + 20;
		sBuildNode * const nodes = new sBuildNode[size];
		if(pBuildNodes){
			memcpy(nodes, pBuildNodes, sizeof(sBuildNode) * pBuildNodeSize);
			delete [] pBuildNodes;
		}
		pBuildNodes = nodes;
		pBuildNodeSize = size;
	}
	
	const int index = pBuildNodeCount++;
	sBuildNode &node = pBuildNodes[index];
	int i;
	for(i=0; i<8; i++){
		node.child[i] = -1;
	}
	node.childCount = 0;
	node.firstComponent = -1;
	node.lastComponent = -1;
	node.componentCount = 0;
	
	return index;
}

int deoalRTWorldOctree::pAddBuildComponent(){
	if(pBuildComponentCount == pBuildComponentSize){
		const int size = pBuildComponentSize + 20;
		sBuildComponent * const components = new sBuildComponent[size];
		if(pBuildComponents){
			memcpy(components, pBuildComponents, sizeof(sBuildComponent) * pBuildComponentSize);
			delete [] pBuildComponents;
		}
		pBuildComponents = components;
		pBuildComponentSize = size;
	}
	
	const int index = pBuildComponentCount++;
	pBuildComponents[index].next = -1;
	
	return index;
}

void deoalRTWorldOctree::pGatherCounts(const sBuildNode &node){
	pGatherNodeCount += node.childCount;
	pGatherComponentCount += node.componentCount;
	
	int i;
	for(i=0; i<8; i++){
		if(node.child[i] != -1){
			pGatherCounts(pBuildNodes[node.child[i]]);
		}
	}
}

void deoalRTWorldOctree::pBuildVisitNode(const sBuildNode &buildNode, sVisitNode &visitNode){
	visitNode.center = buildNode.center;
	visitNode.halfSize = buildNode.halfSize;
	
	// components
	visitNode.firstComponent = pIndexComponent;
	visitNode.componentCount = buildNode.componentCount;
	
	int indexBuildComponent = buildNode.firstComponent;
	while(indexBuildComponent != -1){
		const sBuildComponent &buildComponent = pBuildComponents[indexBuildComponent];
		sVisitComponent &visitComponent = pVisitComponents[pIndexComponent++];
		
		visitComponent.center = buildComponent.center;
		visitComponent.halfSize = buildComponent.halfSize;
// 		visitComponent.sphereCenter = buildComponent.sphereCenter;
// 		visitComponent.sphereRadiusSquared = buildComponent.sphereRadiusSquared;
		visitComponent.component = buildComponent.component;
		visitComponent.inverseMatrix = pWorldMatrix.QuickMultiply(
			buildComponent.component->GetInverseMatrix());
		
		indexBuildComponent = buildComponent.next;
	}
	
	// child nodes
	visitNode.firstNode = pIndexChild;
	visitNode.nodeCount = buildNode.childCount;
	
	pIndexChild += buildNode.childCount;
	
	int i, iterIndex = visitNode.firstNode;
	for(i=0; i<8; i++){
		if(buildNode.child[i] != -1){
			pBuildVisitNode(pBuildNodes[buildNode.child[i]], pVisitNodes[iterIndex++]);
		}
	}
}
