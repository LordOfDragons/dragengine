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

#include "deoglBVH.h"
#include "deoglBVHNode.h"

#include <dragengine/common/exceptions.h>


// Class deoglBVH
///////////////////

// Constructor, destructor
////////////////////////////

deoglBVH::deoglBVH() = default;
deoglBVH::~deoglBVH() = default;


// Management
///////////////

deoglBVHNode &deoglBVH::GetNodeAt(int index){
	return pNodes[index];
}

deoglBVHNode *deoglBVH::GetRootNode(){
	return pNodes.IsNotEmpty() ? &pNodes.First() : nullptr;
}

void deoglBVH::Clear(){
	pNodes.SetCountDiscard(0);
	pPrimitives.SetCountDiscard(0);
}

void deoglBVH::Build(const decTList<sBuildPrimitive> &primitives, int primitiveCount, int maxDepth){
	DEASSERT_TRUE(primitiveCount >= 0)
	DEASSERT_TRUE(maxDepth >= 0)
	
	Clear();
	
	if(primitiveCount == 0){
		return;
	}
	
	pInitPrimitives(primitiveCount);
	
	pNodes.Add({0, primitiveCount});
	
	pBuildNode(primitives, primitiveCount, 0, maxDepth - 1);
}



// Protected Functions
////////////////////////

void deoglBVH::pInitPrimitives(int primitiveCount){
	pPrimitives.SetCountDiscard(0);
	pPrimitives.EnlargeCapacity(primitiveCount);
	
	int i;
	for(i=0; i<primitiveCount; i++){
		pPrimitives.Add(i);
	}
}

void deoglBVH::pBuildNode(const decTList<sBuildPrimitive> &primitives, int primitiveCount, int node, int maxDepth){
	const int nodePrimitiveCount = pNodes[node].GetPrimitiveCount();
	const int nodeFirstIndex = pNodes[node].GetFirstIndex();
	
	// calculate boundaries. we need this for both cases
	const sBuildPrimitive &firstPrimitive = primitives[pPrimitives[nodeFirstIndex]];
	decVector minExtend(firstPrimitive.minExtend);
	decVector maxExtend(firstPrimitive.maxExtend);
	
	int i;
	for(i=1; i<nodePrimitiveCount; i++){
		const sBuildPrimitive &nodePrimitive = primitives[pPrimitives[nodeFirstIndex + i]];
		minExtend.SetSmallest(nodePrimitive.minExtend);
		maxExtend.SetLargest(nodePrimitive.maxExtend);
	}
	
	// make sure boundaries have at least a minimum thickness or else ray casting code
	// can fail to detect the box. slightly enlarging the box is fine enough and makes
	// hitting boxes more robust
	const float margin = 1e-5f; // 0.01mm
	const decVector enlarge(decVector().Largest(decVector(margin, margin, margin) - (maxExtend - minExtend)) * 0.5f);
	minExtend -= enlarge;
	maxExtend += enlarge;
	
	pNodes[node].SetExtends(minExtend, maxExtend);
	
	const decVector nodeSize(maxExtend - minExtend);
	
	// if more than 1 primtive is located in the node, the maximum depth has been reached
	// or the node size becomes too small keep the node a primitive only node and stop
	// building this branch of the tree
	if(nodePrimitiveCount < 2 || maxDepth == 0
	|| !(nodeSize > decVector(FLOAT_SAFE_EPSILON, FLOAT_SAFE_EPSILON, FLOAT_SAFE_EPSILON))){
		return;
	}
	
	// create two child nodes. pAddNode potentially moves memory
	const int indexLeftNode = pNodes.GetCount();
	pNodes.AddRange(2, {}); // add left and right node
	
	// distribute primitives across children
	int walkerLeft = nodeFirstIndex;
	int walkerRight = nodeFirstIndex + nodePrimitiveCount - 1;
	
	if(nodeSize.x > nodeSize.y && nodeSize.x > nodeSize.z){
		// split across X axis
		const float splitCenter = (minExtend.x + maxExtend.x) * 0.5f;
		while(walkerLeft <= walkerRight){
			const int temp = pPrimitives[walkerLeft];
			
			if(primitives[temp].center.x < splitCenter){
				walkerLeft++;
				
			}else{
				pPrimitives[walkerLeft] = pPrimitives[walkerRight];
				pPrimitives[walkerRight--] = temp;
			}
		}
		
	}else if(nodeSize.y > nodeSize.z){
		// split across Y axis
		const float splitCenter = (minExtend.y + maxExtend.y) * 0.5f;
		while(walkerLeft <= walkerRight){
			const int temp = pPrimitives[walkerLeft];
			
			if(primitives[temp].center.y < splitCenter){
				walkerLeft++;
				
			}else{
				pPrimitives[walkerLeft] = pPrimitives[walkerRight];
				pPrimitives[walkerRight--] = temp;
			}
		}
		
	}else{
		// split across Z axis
		const float splitCenter = (minExtend.z + maxExtend.z) * 0.5f;
		while(walkerLeft <= walkerRight){
			const int temp = pPrimitives[walkerLeft];
			
			if(primitives[temp].center.z < splitCenter){
				walkerLeft++;
				
			}else{
				pPrimitives[walkerLeft] = pPrimitives[walkerRight];
				pPrimitives[walkerRight--] = temp;
			}
		}
	}
	
	// update primitive indices of child nodes
	{
	deoglBVHNode &leftNode = pNodes[indexLeftNode];
	deoglBVHNode &rightNode = pNodes[indexLeftNode + 1];
	
	leftNode.SetFirstIndex(nodeFirstIndex);
	leftNode.SetPrimitiveCount(walkerLeft - nodeFirstIndex);
	rightNode.SetFirstIndex(nodeFirstIndex + leftNode.GetPrimitiveCount());
	rightNode.SetPrimitiveCount(nodePrimitiveCount - leftNode.GetPrimitiveCount());
	
	// if a node winds up empty we can not separate the primitives. drop the two child
	// nodes and keep the node a primitives only node
	if(leftNode.GetPrimitiveCount() == 0 || rightNode.GetPrimitiveCount() == 0){
		pNodes.SetCountDiscard(pNodes.GetCount() - 2);
		return;
	}
	}
	
	pNodes[node].SetFirstIndex(pNodes.GetCount() - 2);
	pNodes[node].SetPrimitiveCount(0);
	
	// otherwise continue building with the child nodes. since building nodes usually adds
	// nodes the memory potentially moves too. for this reason we can not use leftNode and
	// rightNode references anymore and have to access the nodes via their indices
	maxDepth--;
	pBuildNode(primitives, primitiveCount, indexLeftNode, maxDepth);
	pBuildNode(primitives, primitiveCount, indexLeftNode + 1, maxDepth);
}
