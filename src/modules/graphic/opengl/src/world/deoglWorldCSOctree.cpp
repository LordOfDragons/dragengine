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

#include "deoglWorldOctree.h"
#include "deoglWorldCSOctree.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decLayerMask.h>



// Class deoglWorldCSOctree
/////////////////////////////

void deoglWorldCSOctree::sCSNode::SetExtends(const decDVector &a, const decDVector &b){
	minExtendX = (GLfloat)a.x;
	minExtendY = (GLfloat)a.y;
	minExtendZ = (GLfloat)a.z;
	maxExtendX = (GLfloat)b.x;
	maxExtendY = (GLfloat)b.y;
	maxExtendZ = (GLfloat)b.z;
}

void deoglWorldCSOctree::sCSElement::SetExtends(const decDVector &a, const decDVector &b){
	minExtendX = (GLfloat)a.x;
	minExtendY = (GLfloat)a.y;
	minExtendZ = (GLfloat)a.z;
	maxExtendX = (GLfloat)b.x;
	maxExtendY = (GLfloat)b.y;
	maxExtendZ = (GLfloat)b.z;
}

void deoglWorldCSOctree::sCSElement::SetLayerMask(const decLayerMask &a){
	layerMask[0] = (uint32_t)(a.GetMask() >> 32);
	layerMask[1] = (uint32_t)a.GetMask();
}

// Constructors and Destructors
/////////////////////////////////

deoglWorldCSOctree::deoglWorldCSOctree(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pPtrNode(nullptr),
pPtrElement(nullptr),
pNodeCount(0),
pElementCount(0),
pNextNode(0),
pNextElement(0),
pElementLinks(nullptr),
pElementLinkSize(0)
{
	(void)pRenderThread;
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	pSSBONodes = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etStream);
	pSSBONodes->SetRowMajor(rowMajor);
	pSSBONodes->SetParameterCount(5);
	pSSBONodes->GetParameterAt(ecsnpMinExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1);
	pSSBONodes->GetParameterAt(ecsnpFirstNode).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBONodes->GetParameterAt(ecsnpMaxExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1);
	pSSBONodes->GetParameterAt(ecsnpChildNodeCount).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBONodes->GetParameterAt(ecsnpElementCount).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBONodes->MapToStd140();
	pSSBONodes->SetBindingPoint(1);
	
	pSSBOElements = deoglSPBlockSSBO::Ref::New(renderThread, deoglSPBlockSSBO::etStream);
	pSSBOElements->SetRowMajor(rowMajor);
	pSSBOElements->SetParameterCount(5);
	pSSBOElements->GetParameterAt(ecsepMinExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1);
	pSSBOElements->GetParameterAt(ecsepElementIndex).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElements->GetParameterAt(ecsepMaxExtend).SetAll(deoglSPBParameter::evtFloat, 3, 1, 1);
	pSSBOElements->GetParameterAt(ecsepFlags).SetAll(deoglSPBParameter::evtInt, 1, 1, 1);
	pSSBOElements->GetParameterAt(ecsepLayerMask).SetAll(deoglSPBParameter::evtInt, 2, 1, 1);
	pSSBOElements->MapToStd140();
	pSSBOElements->SetBindingPoint(0);
}

deoglWorldCSOctree::~deoglWorldCSOctree(){
	EndWriting();
	
	if(pElementLinks){
		delete [] pElementLinks;
	}
}



// Management
///////////////

void deoglWorldCSOctree::SetReferencePosition(const decDVector &position){
	pReferencePosition = position;
}

void deoglWorldCSOctree::Clear(){
	DEASSERT_NULL(pPtrNode)
	
	pNodeCount = 0;
	pElementCount = 0;
	pNextNode = 0;
	pNextElement = 0;
}

void deoglWorldCSOctree::BeginWriting(int nodeCount, int elementCount){
	DEASSERT_TRUE(nodeCount >= 0)
	DEASSERT_TRUE(elementCount >= 0)
	
	EndWriting();
	
	pNodeCount = nodeCount;
	pElementCount = elementCount;
	pNextNode = 0;
	pNextElement = 0;
	
	if(nodeCount == 0 && elementCount == 0){
		return;
	}
	
	if(nodeCount > pSSBONodes->GetElementCount()){
		pSSBONodes->SetElementCount(nodeCount);
	}
	if(elementCount > pSSBOElements->GetElementCount()){
		pSSBOElements->SetElementCount(elementCount);
	}
	
	pSSBONodes->MapBuffer();
	pSSBOElements->MapBuffer();
	
	pPtrNode = (sCSNode*)pSSBONodes->GetMappedBuffer();
	pPtrElement = (sCSElement*)pSSBOElements->GetMappedBuffer();
	
	if(elementCount > pElementLinkSize){
		if(pElementLinks){
			delete pElementLinks;
			pElementLinks = nullptr;
			pElementLinkSize = 0;
		}
		
		pElementLinks = new sElementLink[elementCount];
		pElementLinkSize = elementCount;
	}
}

void deoglWorldCSOctree::EndWriting(){
	if(pPtrElement){
		pNextElement = 0;
		pPtrElement = nullptr;
		pSSBOElements->UnmapBuffer();
	}
	if(pPtrNode){
		pNextNode = 0;
		pPtrNode = nullptr;
		pSSBONodes->UnmapBuffer();
	}
}



deoglWorldCSOctree::sCSNode &deoglWorldCSOctree::GetNodeAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pNodeCount)
	DEASSERT_NOTNULL(pPtrNode)
	return pPtrNode[index];
}

int deoglWorldCSOctree::NextNode(){
	DEASSERT_TRUE(pNextNode < pNodeCount)
	return pNextNode++;
}

deoglWorldCSOctree::sCSNode &deoglWorldCSOctree::NextNodeRef(){
	return pPtrNode[NextNode()];
}

void deoglWorldCSOctree::AdvanceNextData(int amount){
	DEASSERT_TRUE(amount >= 0)
	DEASSERT_TRUE(pNextNode + amount <= pNodeCount)
	pNextNode += amount;
}


deoglWorldCSOctree::sCSElement &deoglWorldCSOctree::GetElementAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pElementCount)
	DEASSERT_NOTNULL(pPtrElement)
	return pPtrElement[index];
}

int deoglWorldCSOctree::NextElement(eCSElementTypes type, const void *link){
	DEASSERT_TRUE(pNextElement < pElementCount)
	DEASSERT_NOTNULL(link);
	
	pElementLinks[pNextElement].type = type;
	pElementLinks[pNextElement].element = link;
	return pNextElement++;
}

deoglWorldCSOctree::sCSElement &deoglWorldCSOctree::NextElementRef(eCSElementTypes type, const void *link){
	return pPtrElement[NextElement(type, link)];
}

const deoglWorldCSOctree::sElementLink &deoglWorldCSOctree::GetLinkAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pElementCount)
	return pElementLinks[index];
}
