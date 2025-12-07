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

#include "deoglPersistentRenderTaskPool.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "deoglPersistentRenderTaskSubInstance.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "../../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../../shaders/paramblock/deoglSPBlockMemory.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBElement.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskInstance
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskInstance::deoglPersistentRenderTaskInstance(deoglPersistentRenderTaskPool &pool) :
pPool(pool),
pLLVAO(this),

pParentVAO(NULL),
pParamBlock(NULL),
pParamBlockSpecial(NULL),

pFirstPoint(0),
pPointCount(0),
pFirstIndex(0),
pIndexCount(0),
pPrimitiveType(GL_TRIANGLES),
pTessPatchVertexCount(3),

pSubInstanceSPB(NULL),
pSubInstanceSPBGroup(NULL),
pSIIndexInstanceSPB(NULL),
pSIIndexInstanceFirst(0){
}

deoglPersistentRenderTaskInstance::~deoglPersistentRenderTaskInstance(){
	Clear();
}



// Management
///////////////

void deoglPersistentRenderTaskInstance::SetParentVAO(deoglPersistentRenderTaskVAO *vao){
	pParentVAO = vao;
}

void deoglPersistentRenderTaskInstance::SetParameterBlock(const deoglShaderParameterBlock *block){
	pParamBlock = block;
}

void deoglPersistentRenderTaskInstance::SetParameterBlockSpecial(const deoglShaderParameterBlock *block){
	pParamBlockSpecial = block;
}

void deoglPersistentRenderTaskInstance::SetFirstPoint(int firstPoint){
	pFirstPoint = firstPoint;
}

void deoglPersistentRenderTaskInstance::SetPointCount(int pointCount){
	pPointCount = pointCount;
}

void deoglPersistentRenderTaskInstance::SetFirstIndex(int firstIndex){
	pFirstIndex = firstIndex;
}

void deoglPersistentRenderTaskInstance::SetIndexCount(int indexCount){
	pIndexCount = indexCount;
}

void deoglPersistentRenderTaskInstance::SetPrimitiveType(GLenum primitiveType){
	pPrimitiveType = primitiveType;
}

void deoglPersistentRenderTaskInstance::SetTessPatchVertexCount(int count){
	pTessPatchVertexCount = count;
}



void deoglPersistentRenderTaskInstance::SetSubInstanceSPB(deoglSharedSPB *spb,
const deoglSharedSPBRTIGroup *group){
	pSubInstanceSPB = spb;
	pSubInstanceSPBGroup = group;
}

int deoglPersistentRenderTaskInstance::GetSubInstanceCount() const{
	return pSubInstances.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTaskInstance::GetRootSubInstance() const{
	return pSubInstances.GetRoot();
}

deoglPersistentRenderTaskSubInstance *deoglPersistentRenderTaskInstance::AddSubInstance(int indexInstance, int flags){
	deoglPersistentRenderTaskSubInstance * const subInstance = pPool.GetSubInstance();
	pSubInstances.Add(&subInstance->GetLLInstance());
	
	subInstance->SetParentInstance(this);
	subInstance->SetIndexInstance(indexInstance);
	subInstance->SetFlags(flags);
	return subInstance;
}

void deoglPersistentRenderTaskInstance::RemoveSubInstance(deoglPersistentRenderTaskSubInstance *subInstance){
	if(!subInstance){
		DETHROW(deeInvalidParam);
	}
	
	pSubInstances.Remove(&subInstance->GetLLInstance());
	pPool.ReturnSubInstance(subInstance);
}

void deoglPersistentRenderTaskInstance::RemoveAllSubInstances(){
	decPointerLinkedList::cListEntry *iter = pSubInstances.GetRoot();
	while(iter){
		pPool.ReturnSubInstance((deoglPersistentRenderTaskSubInstance*)iter->GetOwner());
		iter = iter->GetNext();
	}
	pSubInstances.RemoveAll();
}

void deoglPersistentRenderTaskInstance::SetSIIndexInstanceParam(
deoglShaderParameterBlock *paramBlock, int firstIndex){
	pSIIndexInstanceSPB = paramBlock;
	pSIIndexInstanceFirst = firstIndex;
}

void deoglPersistentRenderTaskInstance::WriteSIIndexInstanceInt(bool useFlags){
	if(!pSIIndexInstanceSPB){
		DETHROW(deeInvalidParam);
	}
	
	if(useFlags){
		struct sIndexFlags{
			GLuint index;
			GLuint flags;
		} *data = (sIndexFlags*)pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		
		decPointerLinkedList::cListEntry *iterSubInstance = pSubInstances.GetRoot();
		while(iterSubInstance){
			const deoglPersistentRenderTaskSubInstance &subInstance =
				*( ( deoglPersistentRenderTaskSubInstance* )iterSubInstance->GetOwner() );
			data->index = (GLuint)subInstance.GetIndexInstance();
			data->flags = (GLuint)subInstance.GetFlags();
			data++;
			iterSubInstance = iterSubInstance->GetNext();
		}
		
	}else{
		struct sIndex{
			GLuint index;
		} *data = (sIndex*)pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		
		decPointerLinkedList::cListEntry *iterSubInstance = pSubInstances.GetRoot();
		while(iterSubInstance){
			const deoglPersistentRenderTaskSubInstance &subInstance =
				*( ( deoglPersistentRenderTaskSubInstance* )iterSubInstance->GetOwner() );
			data->index = (GLuint)subInstance.GetIndexInstance();
			data++;
			iterSubInstance = iterSubInstance->GetNext();
		}
	}
}

void deoglPersistentRenderTaskInstance::WriteSIIndexInstanceShort(bool useFlags){
	if(!pSIIndexInstanceSPB){
		DETHROW(deeInvalidParam);
	}
	
	if(useFlags){
		struct sIndexFlags{
			GLushort index;
			GLushort flags;
		} *data = (sIndexFlags*)pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		
		decPointerLinkedList::cListEntry *iterSubInstance = pSubInstances.GetRoot();
		while(iterSubInstance){
			const deoglPersistentRenderTaskSubInstance &subInstance =
				*( ( deoglPersistentRenderTaskSubInstance* )iterSubInstance->GetOwner() );
			data->index = (GLushort)subInstance.GetIndexInstance();
			data->flags = (GLushort)subInstance.GetFlags();
			data++;
			iterSubInstance = iterSubInstance->GetNext();
		}
		
	}else{
		struct sIndex{
			GLuint index;
		} *data = (sIndex*)pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		
		decPointerLinkedList::cListEntry *iterSubInstance = pSubInstances.GetRoot();
		while(iterSubInstance){
			const deoglPersistentRenderTaskSubInstance &subInstance =
				*( ( deoglPersistentRenderTaskSubInstance* )iterSubInstance->GetOwner() );
			data->index = (GLushort)subInstance.GetIndexInstance();
			data++;
			iterSubInstance = iterSubInstance->GetNext();
		}
	}
}



void deoglPersistentRenderTaskInstance::Clear(){
	RemoveAllSubInstances();
	
	pParentVAO = NULL;
	pParamBlock = NULL;
	pParamBlockSpecial = NULL;
	
	pFirstPoint = 0;
	pPointCount = 0;
	pFirstIndex = 0;
	pIndexCount = 0;
	pPrimitiveType = GL_TRIANGLES;
	pTessPatchVertexCount = 3;
	
	pSubInstanceSPB = NULL;
	pSubInstanceSPBGroup = NULL;
	pSIIndexInstanceSPB = NULL;
	pSIIndexInstanceFirst = 0;
}

void deoglPersistentRenderTaskInstance::RemoveFromParentIfEmpty(){
	if(pSubInstances.GetCount() > 0){
		return;
	}
	
	deoglPersistentRenderTaskVAO * const vao = pParentVAO;
	vao->RemoveInstance(this); // clears pParentVAO
	vao->RemoveFromParentIfEmpty();
}
