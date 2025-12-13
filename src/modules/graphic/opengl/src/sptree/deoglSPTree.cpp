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
#include "deoglSPTree.h"
#include "deoglSPTreeNode.h"
#include <dragengine/common/exceptions.h>



// Class deoglSPTree
//////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglSPTree::deoglSPTree(const decVector &minExtend, const decVector &maxExtend, const decPoint3 &nodeCount){
	decVector size = maxExtend - minExtend;
	int i;
	
	pNodeSize.x = size.x / (float)nodeCount.x;
	pNodeSize.y = size.y / (float)nodeCount.y;
	pNodeSize.z = size.z / (float)nodeCount.z;
	
	pNodeCount = nodeCount;
	
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
	pNodes = NULL;
	
	pTotalNodeCount = nodeCount.x * nodeCount.y * nodeCount.z;
	pStride = nodeCount.x * nodeCount.y;
	
	pNodes = new deoglSPTreeNode*[pTotalNodeCount];
	if(!pNodes) DETHROW(deeOutOfMemory);
	for(i=0; i<pTotalNodeCount; i++) pNodes[i] = NULL;
}

deoglSPTree::~deoglSPTree(){
	ClearAllNodes();
	if(pNodes) delete [] pNodes;
}



// Management
///////////////

bool deoglSPTree::IsIndexValid(const decPoint3 &index) const{
	if(index.x < 0 || index.x >= pNodeCount.x) return false;
	if(index.y < 0 || index.y >= pNodeCount.y) return false;
	if(index.z < 0 || index.z >= pNodeCount.z) return false;
	
	return true;
}

void deoglSPTree::IndexOfNodeAt(decPoint3 &index, const decVector &position) const{
	index.x = (int)((position.x - pMinExtend.x) / pNodeSize.x);
	if(index.x >= 0 && index.x < pNodeCount.x){
		index.y = (int)((position.y - pMinExtend.y) / pNodeSize.y);
		if(index.y >= 0 && index.y < pNodeCount.y){
			index.z = (int)((position.z - pMinExtend.z) / pNodeSize.z);
			if(index.z >= 0 && index.z < pNodeCount.z){
				return;
			}
		}
	}
	
	index.x = -1;
	index.y = -1;
	index.z = -1;
}

deoglSPTreeNode *deoglSPTree::GetNodeAt(const decPoint3 &index) const{
	if(!IsIndexValid(index)) DETHROW(deeInvalidParam);
	
	return pNodes[pStride * index.z + pNodeCount.x * index.y + index.x];
}

void deoglSPTree::SetNodeAt(const decPoint3 &index, deoglSPTreeNode *node){
	if(!IsIndexValid(index)) DETHROW(deeInvalidParam);
	int realIndex = pStride * index.z + pNodeCount.x * index.y + index.x;
	
	if(pNodes[realIndex]){
		delete pNodes[realIndex];
	}
	
	pNodes[realIndex] = node;
}

void deoglSPTree::ClearAllNodes(){
	int i, count = pNodeCount.x * pNodeCount.y * pNodeCount.z;
	
	for(i=0; i<count; i++){
		if(pNodes[i]){
			delete pNodes[i];
			pNodes[i] = NULL;
		}
	}
}
