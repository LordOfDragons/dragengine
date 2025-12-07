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

#include "deoalRTWorldBVH.h"

#include "../../component/deoalAComponent.h"

#include <dragengine/common/exceptions.h>




// Class deoalRTWorldBVH
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTWorldBVH::deoalRTWorldBVH() :
pPosition(),
// pMaxDepth( 64 ), // 8
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
pVisitComponentSize(0){
}

deoalRTWorldBVH::~deoalRTWorldBVH(){
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

void deoalRTWorldBVH::Build(const decDVector &position){
	pVisitComponentCount = 0;
	pVisitNodeCount = 0;
	pBuildComponentCount = 0;
	pBuildNodeCount = 0;
	
	pPosition = position;
}

void deoalRTWorldBVH::AddComponent(deoalAComponent *component){
	if(! component){
		DETHROW(deeInvalidParam);
	}
	
	const int indexComponent = pAddBuildComponent();
	sBuildComponent &buildComponent = pBuildComponents[indexComponent];
	buildComponent.minExtend = component->GetMinExtend() - pPosition;
	buildComponent.maxExtend = component->GetMaxExtend() - pPosition;
	buildComponent.center = (buildComponent.minExtend + buildComponent.maxExtend) * 0.5f;
	buildComponent.halfSize = (buildComponent.maxExtend - buildComponent.minExtend) * 0.5f;
	buildComponent.component = component;
}

void deoalRTWorldBVH::Finish(){
	// if tree is empty exit early
	if(pBuildComponentCount == 0){
		pVisitComponentCount = 0;
		pVisitNodeCount = 0;
		return;
	}
	
	// build root node
	pAddBuildNode();
	sBuildNode &rootNode = pBuildNodes[0];
	
	rootNode.componentCount = pBuildComponentCount;
	rootNode.firstComponent = 0;
	rootNode.lastComponent = pBuildComponentCount - 1;
	
	int i;
	for(i=0; i<pBuildComponentCount-1; i++){
		pBuildComponents[i].next = i + 1;
	}
	pUpdateNodeExtends(rootNode);
	
	// split root node
// 	pSplitNode( 0, 0 );
	pSplitNode(0);
	
	// build visit arrays
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
	
	pIndexNode = 0;
	pIndexComponent = 0;
	pWorldMatrix.SetTranslation(pPosition);
	pBuildVisitNode(pBuildNodes[0]);
}



// Private Functions
//////////////////////

int deoalRTWorldBVH::pAddBuildNode(){
	if(pBuildNodeCount == pBuildNodeSize){
		const int size = pBuildNodeSize + 100;
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
	node.child1 = -1;
	node.child2 = -1;
	node.firstComponent = -1;
	node.lastComponent = -1;
	node.componentCount = 0;
	
	return index;
}

int deoalRTWorldBVH::pAddBuildComponent(){
	if(pBuildComponentCount == pBuildComponentSize){
		const int size = pBuildComponentSize + 1000;
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

void deoalRTWorldBVH::pUpdateNodeExtends(sBuildNode &node) const{
	if(node.firstComponent == -1){
		node.center.SetZero();
		node.halfSize.SetZero();
		return;
	}
	
	const sBuildComponent &firstComponent = pBuildComponents[node.firstComponent];
	decVector minExtend(firstComponent.minExtend);
	decVector maxExtend(firstComponent.maxExtend);
	int componentIndex = firstComponent.next;
	
	while(componentIndex != -1){
		const sBuildComponent &component = pBuildComponents[componentIndex];
		minExtend.SetSmallest(component.minExtend);
		maxExtend.SetLargest(component.maxExtend);
		componentIndex = component.next;
	}
	
	node.center = (minExtend + maxExtend) * 0.5f;
	node.halfSize = (maxExtend - minExtend) * 0.5f;
}

//void deoalRTWorldBVH::pSplitNode( int nodeIndex, int depth ){
void deoalRTWorldBVH::pSplitNode(int nodeIndex){
	//if( depth == pMaxDepth || pBuildNodes[ nodeIndex ].componentCount < 2 ){
	// measured using test scenario and 200m range (spread ~0.3)
	// max 4 components per node: 13.56
	// max 3 components per node: 13.59
	// max 2 components per node: 12.75
	// max 1 component per node: 12.44
	if(pBuildNodes[nodeIndex].componentCount < 3){
		return;
	}
	
	// find center extends. we need to use this instead of the node extends since only this
	// shows us in corner cases if a split is possible
	const sBuildComponent &firstComponent = pBuildComponents[pBuildNodes[nodeIndex].firstComponent];
	decVector centerMin(firstComponent.center);
	decVector centerMax(firstComponent.center);
	int componentIndex = firstComponent.next;
	
	while(componentIndex != -1){
		const sBuildComponent &component = pBuildComponents[componentIndex];
		centerMin.SetSmallest(component.center);
		centerMax.SetLargest(component.center);
		componentIndex = component.next;
	}
	
	// find primary axis
	const decVector centerSize(centerMax - centerMin);
	const float splitThreshold = 1.0f; //0.001f; // 0.01f
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
	
	componentIndex = node.firstComponent;
	
	switch(primaryAxis){
	case 0: // x largest
		while(componentIndex != -1){
			sBuildComponent &component = pBuildComponents[componentIndex];
			const int nextComponentIndex = component.next;
			component.next = -1;
			
			if(component.center.x < splitPosition){
				pNodeAddComponent(child1, componentIndex);
				
			}else{
				pNodeAddComponent(child2, componentIndex);
			}
			
			componentIndex = nextComponentIndex;
		}
		break;
		
	case 1: // y largest
		while(componentIndex != -1){
			sBuildComponent &component = pBuildComponents[componentIndex];
			const int nextComponentIndex = component.next;
			component.next = -1;
			
			if(component.center.y < splitPosition){
				pNodeAddComponent(child1, componentIndex);
				
			}else{
				pNodeAddComponent(child2, componentIndex);
			}
			
			componentIndex = nextComponentIndex;
		}
		break;
		
	case 2: // z largest
		while(componentIndex != -1){
			sBuildComponent &component = pBuildComponents[componentIndex];
			const int nextComponentIndex = component.next;
			component.next = -1;
			
			if(component.center.z < splitPosition){
				pNodeAddComponent(child1, componentIndex);
				
			}else{
				pNodeAddComponent(child2, componentIndex);
			}
			
			componentIndex = nextComponentIndex;
		}
		break;
	}
	
	node.componentCount = 0;
	node.firstComponent = -1;
	node.lastComponent = -1;
	node.child1 = childIndex1;
	node.child2 = childIndex2;
	
	pUpdateNodeExtends(child1);
	pUpdateNodeExtends(child2);
	
// 	pSplitNode( childIndex1, depth + 1 );
// 	pSplitNode( childIndex2, depth + 1 );
	pSplitNode(childIndex1);
	pSplitNode(childIndex2);
}

void deoalRTWorldBVH::pNodeAddComponent(sBuildNode &node, int componentIndex){
	if(node.lastComponent != -1){
		pBuildComponents[node.lastComponent].next = componentIndex;
		
	}else{
		node.firstComponent = componentIndex;
	}
	
	node.lastComponent = componentIndex;
	node.componentCount++;
}

void deoalRTWorldBVH::pBuildVisitNode(const sBuildNode &buildNode){
	sVisitNode &visitNode = pVisitNodes[pIndexNode++];
	visitNode.center = buildNode.center;
	visitNode.halfSize = buildNode.halfSize;
	
	// components
	if(buildNode.componentCount > 0){
		visitNode.firstComponent = pIndexComponent;
		visitNode.componentCount = buildNode.componentCount;
		visitNode.node1 = -1;
		visitNode.node2 = -1;
		
		int indexBuildComponent = buildNode.firstComponent;
		while(indexBuildComponent != -1){
			const sBuildComponent &buildComponent = pBuildComponents[indexBuildComponent];
			sVisitComponent &visitComponent = pVisitComponents[pIndexComponent++];
			
			visitComponent.center = buildComponent.center;
			visitComponent.halfSize = buildComponent.halfSize;
			visitComponent.component = buildComponent.component;
			visitComponent.inverseMatrix = pWorldMatrix.QuickMultiply(
				buildComponent.component->GetInverseMatrix());
			
			indexBuildComponent = buildComponent.next;
		}
		
	// child nodes
	}else if(buildNode.child1 != -1){
		visitNode.firstComponent = -1;
		visitNode.componentCount = 0;
		
		visitNode.node1 = pIndexNode;
		pBuildVisitNode(pBuildNodes[buildNode.child1]);
		
		visitNode.node2 = pIndexNode;
		pBuildVisitNode(pBuildNodes[buildNode.child2]);
		
	// this should never happen
	}else{
		DETHROW(deeInvalidParam);
	}
}
