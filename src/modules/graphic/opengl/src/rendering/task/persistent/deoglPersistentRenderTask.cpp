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
#include "deoglPersistentRenderTask.h"
#include "deoglPersistentRenderTaskPipeline.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "deoglPersistentRenderTaskSubInstance.h"
#include "deoglPersistentRenderTaskOwner.h"
#include "../../../capabilities/deoglCapabilities.h"
#include "../../../pipeline/deoglPipeline.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../renderthread/deoglRTBufferObject.h"
#include "../../../skin/shader/deoglSkinShader.h"
#include "../../../shaders/deoglShaderProgram.h"
#include "../../../shaders/deoglShaderUnitSourceCode.h"
#include "../../../shaders/paramblock/deoglSPBParameter.h"
#include "../../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../../texture/cubemap/deoglCubeMap.h"
#include "../../../texture/texture2d/deoglTexture.h"
#include "../../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglPersistentRenderTask
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTask::deoglPersistentRenderTask(deoglPersistentRenderTaskPool &pool) :
pPool(pool),
pRenderParamBlock(NULL),
pTBOInstances(0),
pSPBInstanceMaxEntries(0),
pUseSPBInstanceFlags(false){
}

deoglPersistentRenderTask::~deoglPersistentRenderTask(){
	RemoveAllPipelines();
	RemoveAllOwners();
}



// Management
///////////////

void deoglPersistentRenderTask::PrepareForRender(deoglRenderThread &renderThread){
	if(pPipelines.GetCount() == 0){
		return;
	}
	
	pCalcSPBInstancesMaxEntries(renderThread);
	pAssignSPBInstances(renderThread);
	pUpdateSPBInstances();
}

void deoglPersistentRenderTask::SetRenderParamBlock(deoglSPBlockUBO *paramBlock){
	pRenderParamBlock = paramBlock;
}

void deoglPersistentRenderTask::SetTBOInstances(GLuint tbo){
	pTBOInstances = tbo;
}

void deoglPersistentRenderTask::SetUseSPBInstanceFlags(bool useFlags){
	pUseSPBInstanceFlags = useFlags;
}



int deoglPersistentRenderTask::GetOwnerCount() const{
	return pOwners.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTask::GetRootOwner() const{
	return pOwners.GetRoot();
}

deoglPersistentRenderTaskOwner *deoglPersistentRenderTask::GetOwnerWith(deObject *owner, unsigned int hash) const{
	deoglPersistentRenderTaskOwner *rtowner;
	return pOwnersMap.GetAt(owner, hash, (void**)&rtowner) ? rtowner : NULL;
}

deoglPersistentRenderTaskOwner *deoglPersistentRenderTask::AddOwner(deObject *owner, unsigned int hash){
	// commented out in the name of performance
// 	if( pOwnersMap.Has( owner, hash ) ){
// 		DETHROW( deeInvalidParam );
// 	}
	
	deoglPersistentRenderTaskOwner * const rtowner = pPool.GetOwner();
	rtowner->SetOwner(owner, hash);
	pOwners.Add(&rtowner->GetLLTask());
	pOwnersMap.SetAt(owner, hash, rtowner);
	return rtowner;
}

void deoglPersistentRenderTask::RemoveOwner(deoglPersistentRenderTaskOwner *owner){
	if(!owner){
		DETHROW(deeInvalidParam);
	}
	
	pOwnersMap.Remove(owner->GetOwner(), owner->GetHash());
	pOwners.Remove(&owner->GetLLTask());
	pPool.ReturnOwner(owner);
}

void deoglPersistentRenderTask::RemoveAllOwners(){
	decPointerLinkedList::cListEntry *iter = pOwners.GetRoot();
	while(iter){
		pPool.ReturnOwner((deoglPersistentRenderTaskOwner*)iter->GetOwner());
		iter = iter->GetNext();
	}
	pOwners.RemoveAll();
	pOwnersMap.RemoveAll();
}



int deoglPersistentRenderTask::GetPipelineCount() const{
	return pPipelines.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTask::GetRootPipeline() const{
	return pPipelines.GetRoot();
}

deoglPersistentRenderTaskPipeline *deoglPersistentRenderTask::GetPipelineWith(const deoglPipeline *pipeline) const{
	DEASSERT_NOTNULL(pipeline)
	
	deoglPersistentRenderTaskPipeline *rtpipeline;
	return pPipelinesMap.GetAt(pipeline, pipeline->GetRTSIndex(), (void**)&rtpipeline) ? rtpipeline : nullptr;
}

deoglPersistentRenderTaskPipeline *deoglPersistentRenderTask::AddPipeline(const deoglPipeline *pipeline){
	DEASSERT_NOTNULL(pipeline)
	
	// commented out in the name of performance
// 	if( pPipelinesMap.Has( pipeline, pipeline->GetUniqueKey() ) ){
// 		DETHROW( deeInvalidParam );
// 	}
	
	deoglPersistentRenderTaskPipeline * const rtpipeline = pPool.GetPipeline();
	pPipelines.Add(&rtpipeline->GetLLTask());
	rtpipeline->SetParentTask(this);
	rtpipeline->SetPipeline(pipeline);
	pPipelinesMap.SetAt(pipeline, pipeline->GetRTSIndex(), rtpipeline);
	return rtpipeline;
}

void deoglPersistentRenderTask::RemovePipeline(deoglPersistentRenderTaskPipeline *pipeline){
	DEASSERT_NOTNULL(pipeline)
	
	pPipelinesMap.Remove(pipeline->GetPipeline(), pipeline->GetPipeline()->GetRTSIndex());
	pPipelines.Remove(&pipeline->GetLLTask());
	pPool.ReturnPipeline(pipeline);
}

void deoglPersistentRenderTask::RemoveAllPipelines(){
	decPointerLinkedList::cListEntry *iter = pPipelines.GetRoot();
	while(iter){
		pPool.ReturnPipeline((deoglPersistentRenderTaskPipeline*)iter->GetOwner());
		iter = iter->GetNext();
	}
	pPipelines.RemoveAll();
	pPipelinesMap.RemoveAll();
}



int deoglPersistentRenderTask::GetTotalPointCount() const{
	decPointerLinkedList::cListEntry *iter = pPipelines.GetRoot();
	int totalPointCount = 0;
	
	while(iter){
		totalPointCount += ((deoglPersistentRenderTaskPipeline*)iter->GetOwner())->GetTotalPointCount();
		iter = iter->GetNext();
	}
	
	return totalPointCount;
}

int deoglPersistentRenderTask::GetTotalTextureCount() const{
	decPointerLinkedList::cListEntry *iter = pPipelines.GetRoot();
	int totalTextureCount = 0;
	
	while(iter){
		totalTextureCount += ((deoglPersistentRenderTaskPipeline*)iter->GetOwner())->GetTextureCount();
		iter = iter->GetNext();
	}
	
	return totalTextureCount;
}

int deoglPersistentRenderTask::GetTotalVAOCount() const{
	decPointerLinkedList::cListEntry *iter = pPipelines.GetRoot();
	int totalVAOCount = 0;
	
	while(iter){
		totalVAOCount += ((deoglPersistentRenderTaskPipeline*)iter->GetOwner())->GetTotalVAOCount();
		iter = iter->GetNext();
	}
	
	return totalVAOCount;
}

int deoglPersistentRenderTask::GetTotalInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pPipelines.GetRoot();
	int totalInstanceCount = 0;
	
	while(iter){
		totalInstanceCount += ((deoglPersistentRenderTaskPipeline*)iter->GetOwner())->GetTotalInstanceCount();
		iter = iter->GetNext();
	}
	
	return totalInstanceCount;
}

int deoglPersistentRenderTask::GetTotalSubInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pPipelines.GetRoot();
	int totalSubInstanceCount = 0;
	
	while(iter){
		totalSubInstanceCount += ((deoglPersistentRenderTaskPipeline*)iter->GetOwner())->GetTotalSubInstanceCount();
		iter = iter->GetNext();
	}
	
	return totalSubInstanceCount;
}

void deoglPersistentRenderTask::Clear(){
	RemoveAllPipelines();
	RemoveAllOwners();
}

void deoglPersistentRenderTask::RemoveOwnedBy(deoglPersistentRenderTaskOwner &owner){
	// remove sub instances
	int i, count = owner.GetSubInstanceCount();
	for(i=0; i<count; i++){
		deoglPersistentRenderTaskSubInstance * const subInstance = owner.GetSubInstanceAt(i);
		deoglPersistentRenderTaskInstance * const instance = subInstance->GetParentInstance();
		instance->RemoveSubInstance(subInstance);
		instance->RemoveFromParentIfEmpty();
	}
	owner.RemoveAllSubInstances();
	
	// remove instances
	count = owner.GetInstanceCount();
	for(i=0; i<count; i++){
		deoglPersistentRenderTaskInstance * const instance = owner.GetInstanceAt(i);
		deoglPersistentRenderTaskVAO * const vao = instance->GetParentVAO();
		vao->RemoveInstance(instance);
		vao->RemoveFromParentIfEmpty();
	}
	owner.RemoveAllInstances();
}



// Private Functions
//////////////////////

void deoglPersistentRenderTask::pCalcSPBInstancesMaxEntries(deoglRenderThread &renderThread){
	// since std140 layout adds a lot of padding between array elements we use ivec4.
	// this groups indices in blocks of four so the final index is pSPB[i/4][i%4].
	//pSPBInstanceMaxEntries = ( renderThread.GetCapabilities().GetUBOMaxSize() / 16 ) * 4;
	pSPBInstanceMaxEntries = renderThread.GetBufferObject().GetInstanceArraySizeUBO();
	
	if(pUseSPBInstanceFlags){
		// if instance flags are used the vector <instanceIndex, instanceFlags> is
		// used instead. this reduces the max entries count by factor 2 but allows
		// to transport a scratch flags value in a fast way
		pSPBInstanceMaxEntries /= 2;
	}
}

void deoglPersistentRenderTask::pAssignSPBInstances(deoglRenderThread &renderThread){
	decPointerLinkedList::cListEntry *iterPipeline = pPipelines.GetRoot();
	const int componentsPerIndex = pUseSPBInstanceFlags ? 2 : 1;
	deoglShaderParameterBlock *paramBlock = NULL;
	int paramBlockCount = 0;
	int firstIndex = 0;
	
	while(iterPipeline){
		const deoglPersistentRenderTaskPipeline &pipeline = *((deoglPersistentRenderTaskPipeline*)iterPipeline->GetOwner());
		decPointerLinkedList::cListEntry *iterTexture = pipeline.GetRootTexture();
		
		while(iterTexture){
			deoglPersistentRenderTaskTexture &texture = *((deoglPersistentRenderTaskTexture*)iterTexture->GetOwner());
			decPointerLinkedList::cListEntry *iterVAO = texture.GetRootVAO();
			
			while(iterVAO){
				deoglPersistentRenderTaskVAO &vao = *((deoglPersistentRenderTaskVAO*)iterVAO->GetOwner());
				decPointerLinkedList::cListEntry *iterInstance = vao.GetRootInstance();
				
				while(iterInstance){
					deoglPersistentRenderTaskInstance &instance = *((deoglPersistentRenderTaskInstance*)iterInstance->GetOwner());
					
					if(!paramBlock || firstIndex + instance.GetSubInstanceCount() > pSPBInstanceMaxEntries){
						if(paramBlock){
							const int ecount = componentsPerIndex * decMath::max(((firstIndex - 1) / 4) + 1, 1);
							if(ecount > paramBlock->GetElementCount()){
								paramBlock->SetElementCount(ecount);
							}
						}
						
						if(paramBlockCount == pSPBInstances.GetCount()){
							pCreateSPBInstanceParamBlock(renderThread);
						}
						paramBlock = pSPBInstances.GetAt(paramBlockCount++);
						firstIndex = 0;
					}
					
					instance.SetSIIndexInstanceParam(paramBlock, firstIndex);
					firstIndex += instance.GetSubInstanceCount();
					
					iterInstance = iterInstance->GetNext();
				}
				iterVAO = iterVAO->GetNext();
			}
			iterTexture = iterTexture->GetNext();
		}
		iterPipeline = iterPipeline->GetNext();
	}
	
	if(paramBlock){
		const int ecount = componentsPerIndex * decMath::max(((firstIndex - 1) / 4) + 1, 1);
		if(ecount > paramBlock->GetElementCount()){
			paramBlock->SetElementCount(ecount);
		}
	}
}

void deoglPersistentRenderTask::pUpdateSPBInstances(){
	deoglShaderParameterBlock *paramBlock = NULL;
	
	try{
		decPointerLinkedList::cListEntry *iterPipeline = pPipelines.GetRoot();
		
		while(iterPipeline){
			const deoglPersistentRenderTaskPipeline &pipeline = *((deoglPersistentRenderTaskPipeline*)iterPipeline->GetOwner());
			decPointerLinkedList::cListEntry *iterTexture = pipeline.GetRootTexture();
			
			while(iterTexture){
				deoglPersistentRenderTaskTexture &texture = *((deoglPersistentRenderTaskTexture*)iterTexture->GetOwner());
				decPointerLinkedList::cListEntry *iterVAO = texture.GetRootVAO();
				
				while(iterVAO){
					deoglPersistentRenderTaskVAO &vao = *((deoglPersistentRenderTaskVAO*)iterVAO->GetOwner());
					decPointerLinkedList::cListEntry *iterInstance = vao.GetRootInstance();
					
					while(iterInstance){
						deoglPersistentRenderTaskInstance &instance = *((deoglPersistentRenderTaskInstance*)iterInstance->GetOwner());
						
						if(instance.GetSIIndexInstanceSPB() != paramBlock){
							if(paramBlock){
								paramBlock->UnmapBuffer();
								paramBlock = NULL;
							}
							
							instance.GetSIIndexInstanceSPB()->MapBuffer();
							paramBlock = instance.GetSIIndexInstanceSPB();
						}
						
						instance.WriteSIIndexInstanceInt(pUseSPBInstanceFlags);
						
						iterInstance = iterInstance->GetNext();
					}
					iterVAO = iterVAO->GetNext();
				}
				iterTexture = iterTexture->GetNext();
			}
			iterPipeline = iterPipeline->GetNext();
		}
		
		if(paramBlock){
			paramBlock->UnmapBuffer();
		}
		
	}catch(const deException &){
		if(paramBlock){
			paramBlock->UnmapBuffer();
		}
		throw;
	}
}

void deoglPersistentRenderTask::pCreateSPBInstanceParamBlock(deoglRenderThread &renderThread){
	// since std140 layout adds a lot of padding between array elements we use ivec4.
	// this groups indices in blocks of four so the final index is pSPB[i/4][i%4]
	const deoglSPBlockUBO::Ref ubo(deoglSPBlockUBO::Ref::NewWith(renderThread));
	ubo->SetRowMajor(renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
	ubo->SetParameterCount(1);
	ubo->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
	ubo->MapToStd140();
	ubo->SetBindingPoint(deoglSkinShader::eubInstanceIndex);
	pSPBInstances.Add(ubo);
}
