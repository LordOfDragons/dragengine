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

#include "deoglGI.h"
#include "deoglGIBVH.h"
#include "deoglGIBVHLocal.h"
#include "../model/deoglModelLOD.h"
#include "../model/face/deoglModelFace.h"
#include "../renderthread/deoglRenderThread.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../tbo/deoglDynamicTBOUInt16.h"
#include "../tbo/deoglDynamicTBOBlock.h"
#include "../tbo/deoglDynamicTBOShared.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIBVHLocal
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIBVHLocal::deoglGIBVHLocal(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pTBONodeBox(NULL),
pTBOIndex(NULL),
pTBOFace(NULL),
pTBOVertex(NULL),
pTBOTexCoord(NULL),
pBlockUsageCount(0)
{
	try{
		pTBONodeBox.TakeOverWithrenderThread, 4);
		pTBOIndex.TakeOverWithrenderThread, 2);
		pTBOFace.TakeOverWithrenderThread, 4);
		pTBOVertex.TakeOverWithrenderThread, 4);
		pTBOTexCoord.TakeOverWithrenderThread, 2);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglGIBVHLocal::~deoglGIBVHLocal(){
	pCleanUp();
}



// Management
///////////////

void deoglGIBVHLocal::Clear(){
	DropBlocks();
	pBVH.Clear();
	pTBOVertex->Clear();
	pTBOTexCoord->Clear();
	pTBOFace->Clear();
	pTBOIndex->Clear();
	pTBONodeBox->Clear();
}

void deoglGIBVHLocal::BuildBVH(const deoglBVH::sBuildPrimitive *primitives,
int primitiveCount, int maxDepth){
	pBVH.Build(primitives, primitiveCount, maxDepth);
}

void deoglGIBVHLocal::UpdateBVHExtends(){
	deoglBVHNode * const rootNode = pBVH.GetRootNode();
	if(rootNode){
		pUpdateBVHExtends(*rootNode);
	}
}

void deoglGIBVHLocal::TBOAddVertex(const decVector &position){
	pTBOVertex->AddVec4(position, 0.0f);
}

void deoglGIBVHLocal::TBOAddVertices(const oglModelPosition *positions, int count){
	int i;
	for(i=0; i<count; i++){
		TBOAddVertex(positions[i].position);
	}
}

void deoglGIBVHLocal::TBOAddFace(int vertex1, int vertex2, int vertex3, int material,
const decVector2 &texCoord1, const decVector2 &texCoord2, const decVector2 &texCoord3){
	pTBOFace->AddVec4(vertex1, vertex2, vertex3, material);
	
	pTBOTexCoord->AddVec2(texCoord1);
	pTBOTexCoord->AddVec2(texCoord2);
	pTBOTexCoord->AddVec2(texCoord3);
}

void deoglGIBVHLocal::TBOAddFaces(const deoglModelFace *faces, const oglModelVertex *vertices,
const decVector2 *texCoords){
	const int * const primitives = pBVH.GetPrimitives();
	const int primitiveCount = pBVH.GetPrimitiveCount();
	int i;
	
	for(i=0; i<primitiveCount; i++){
		const deoglModelFace &face = faces[primitives[i]];
		const oglModelVertex &v1 = vertices[face.GetVertex1()];
		const oglModelVertex &v2 = vertices[face.GetVertex2()];
		const oglModelVertex &v3 = vertices[face.GetVertex3()];
		
		TBOAddFace(v1.position, v2.position, v3.position, face.GetTexture(),
			texCoords[v1.texcoord], texCoords[v2.texcoord], texCoords[v3.texcoord]);
	}
}

void deoglGIBVHLocal::TBOAddBVH(){
	const deoglBVHNode * const nodes = pBVH.GetNodes();
	const int nodeCount = pBVH.GetNodeCount();
	int i;
	
	for(i=0; i<nodeCount; i++){
		const deoglBVHNode &node = nodes[i];
		pTBONodeBox->AddVec4(node.GetMinExtend(), 0.0f);
		pTBONodeBox->AddVec4(node.GetMaxExtend(), 0.0f);
		pTBOIndex->AddVec2(node.GetFirstIndex(), node.GetPrimitiveCount());
	}
}

void deoglGIBVHLocal::TBOBVHUpdateNodeExtends(){
	const deoglBVHNode * const nodes = pBVH.GetNodes();
	const int nodeCount = pBVH.GetNodeCount();
	float *nodeBox = pTBONodeBox->GetDataFloat();
	int i;
	
	for(i=0; i<nodeCount; i++, nodeBox+=8){
		const deoglBVHNode &node = nodes[i];
		
		const decVector &minExtend = node.GetMinExtend();
		nodeBox[0] = minExtend.x;
		nodeBox[1] = minExtend.y;
		nodeBox[2] = minExtend.z;
		
		const decVector &maxExtend = node.GetMaxExtend();
		nodeBox[4] = maxExtend.x;
		nodeBox[5] = maxExtend.y;
		nodeBox[6] = maxExtend.z;
	}
}



const deoglDynamicTBOBlock::Ref &deoglGIBVHLocal::GetBlockNode(){
	if(!pBlockNode){
		pBlockNode.TakeOver(pRenderThread.GetGI().GetBVHShared().GetSharedTBONode()
			->AddBlock(pTBOIndex, pTBONodeBox));
	}
	return pBlockNode;
}

const deoglDynamicTBOBlock::Ref &deoglGIBVHLocal::GetBlockFace(){
	if(!pBlockFace){
		pBlockFace.TakeOver(pRenderThread.GetGI().GetBVHShared().GetSharedTBOFace()
			->AddBlock(pTBOFace, pTBOTexCoord));
	}
	return pBlockFace;
}

const deoglDynamicTBOBlock::Ref &deoglGIBVHLocal::GetBlockVertex(){
	if(!pBlockVertex){
		pBlockVertex.TakeOver(pRenderThread.GetGI().GetBVHShared().GetSharedTBOVertex()
			->AddBlock(pTBOVertex));
	}
	return pBlockVertex;
}

void deoglGIBVHLocal::DropBlocks(){
	if(pBlockNode){
		pBlockNode->Drop();
		pBlockNode = NULL;
	}
	if(pBlockFace){
		pBlockFace->Drop();
		pBlockFace = NULL;
	}
	if(pBlockVertex){
		pBlockVertex->Drop();
		pBlockVertex = NULL;
	}
}

void deoglGIBVHLocal::AddBlockUsage(){
	pBlockUsageCount++;
}

void deoglGIBVHLocal::RemoveBlockUsage(){
	if(pBlockUsageCount == 0){
		return;
	}
	
	pBlockUsageCount--;
	if(pBlockUsageCount == 0){
		DropBlocks();
	}
}



// Private Functions
//////////////////////

void deoglGIBVHLocal::pCleanUp(){
	DropBlocks();
	
}

void deoglGIBVHLocal::pUpdateBVHExtends(deoglBVHNode &node){
	const int primitiveCount = node.GetPrimitiveCount();
	const int firstIndex = node.GetFirstIndex();
	
	if(primitiveCount == 0){
		deoglBVHNode &nodeLeft = pBVH.GetNodeAt(firstIndex);
		deoglBVHNode &nodeRight = pBVH.GetNodeAt(firstIndex + 1);
		
		pUpdateBVHExtends(nodeLeft);
		pUpdateBVHExtends(nodeRight);
		
		node.SetExtends(nodeLeft.GetMinExtend().Smallest(nodeRight.GetMinExtend()),
			nodeLeft.GetMaxExtend().Largest(nodeRight.GetMaxExtend()));
		
	}else{
		const uint16_t *face = pTBOFace->GetDataUInt() + firstIndex * 4;
		const float * const vertices = pTBOVertex->GetDataFloat();
		decVector minExtend, maxExtend;
		int i;
		
		for(i=0; i<primitiveCount; i++, face+=4){
			const float * const v1 = vertices + face[0] * 4;
			const decVector p1(v1[0], v1[1], v1[2]);
			if(i > 0){
				minExtend.SetSmallest(p1);
				maxExtend.SetLargest(p1);
				
			}else{
				minExtend = maxExtend = p1;
			}
			
			const float * const v2 = vertices + face[1] * 4;
			const decVector p2(v2[0], v2[1], v2[2]);
			minExtend.SetSmallest(p2);
			maxExtend.SetLargest(p2);
			
			const float * const v3 = vertices + face[2] * 4;
			const decVector p3(v3[0], v3[1], v3[2]);
			minExtend.SetSmallest(p3);
			maxExtend.SetLargest(p3);
		}
		
		node.SetExtends(minExtend, maxExtend);
	}
}
