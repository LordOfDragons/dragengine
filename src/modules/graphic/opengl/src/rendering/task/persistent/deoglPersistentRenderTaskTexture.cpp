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
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskPipeline.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskTexture
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskTexture::deoglPersistentRenderTaskTexture(deoglPersistentRenderTaskPool &pool) :
pPool(pool),
pLLTexture(this),
pParentPipeline(NULL),
pTUC(NULL),
pParamBlock(NULL){
}

deoglPersistentRenderTaskTexture::~deoglPersistentRenderTaskTexture(){
	Clear();
}



// Management
///////////////

void deoglPersistentRenderTaskTexture::SetParentPipeline(deoglPersistentRenderTaskPipeline *pipeline){
	pParentPipeline = pipeline;
}

void deoglPersistentRenderTaskTexture::SetTUC(const deoglTexUnitsConfig *tuc){
	pTUC = tuc;
}

int deoglPersistentRenderTaskTexture::GetTotalPointCount() const{
	decPointerLinkedList::cListEntry *iter = pVAOs.GetRoot();
	int pointCount = 0;
	
	while(iter){
		pointCount += ((deoglPersistentRenderTaskVAO*)iter->GetOwner())->GetTotalPointCount();
		iter = iter->GetNext();
	}
	
	return pointCount;
}

int deoglPersistentRenderTaskTexture::GetTotalInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pVAOs.GetRoot();
	int instanceCount = 0;
	
	while(iter){
		instanceCount += ((deoglPersistentRenderTaskVAO*)iter->GetOwner())->GetInstanceCount();
		iter = iter->GetNext();
	}
	
	return instanceCount;
}

int deoglPersistentRenderTaskTexture::GetTotalSubInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pVAOs.GetRoot();
	int subInstanceCount = 0;
	
	while(iter){
		subInstanceCount += ((deoglPersistentRenderTaskVAO*)iter->GetOwner())->GetTotalSubInstanceCount();
		iter = iter->GetNext();
	}
	
	return subInstanceCount;
}



void deoglPersistentRenderTaskTexture::SetParameterBlock(const deoglShaderParameterBlock *block){
	pParamBlock = block;
}



int deoglPersistentRenderTaskTexture::GetVAOCount() const{
	return pVAOs.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTaskTexture::GetRootVAO() const{
	return pVAOs.GetRoot();
}

deoglPersistentRenderTaskVAO *deoglPersistentRenderTaskTexture::GetVAOWith(const deoglVAO *vao) const{
	if(! vao){
		DETHROW(deeInvalidParam);
	}
	
	deoglPersistentRenderTaskVAO *rtvao;
	return pVAOsMap.GetAt(vao, vao->GetUniqueKey(), (void**)&rtvao) ? rtvao : NULL;
}

deoglPersistentRenderTaskVAO *deoglPersistentRenderTaskTexture::AddVAO(const deoglVAO *vao){
	if(! vao){
		DETHROW(deeInvalidParam);
	}
	
	// commented out in the name of performance
// 	if( pVAOsMap.Has( vao, vao->GetUniqueKey() ) ){
// 		DETHROW( deeInvalidParam );
// 	}
	
	deoglPersistentRenderTaskVAO * const rtvao = pPool.GetVAO();
	pVAOs.Add(&rtvao->GetLLTexture());
	rtvao->SetParentTexture(this);
	rtvao->SetVAO(vao);
	pVAOsMap.SetAt(vao, vao->GetUniqueKey(), rtvao);
	return rtvao;
}

void deoglPersistentRenderTaskTexture::RemoveVAO(deoglPersistentRenderTaskVAO *vao){
	if(! vao){
		DETHROW(deeInvalidParam);
	}
	
	pVAOsMap.Remove(vao->GetVAO(), vao->GetVAO()->GetUniqueKey());
	pVAOs.Remove(&vao->GetLLTexture());
	pPool.ReturnVAO(vao);
}

void deoglPersistentRenderTaskTexture::RemoveAllVAOs(){
	decPointerLinkedList::cListEntry *iter = pVAOs.GetRoot();
	while(iter){
		pPool.ReturnVAO((deoglPersistentRenderTaskVAO*)iter->GetOwner());
		iter = iter->GetNext();
	}
	pVAOs.RemoveAll();
	pVAOsMap.RemoveAll();
}

void deoglPersistentRenderTaskTexture::Clear(){
	RemoveAllVAOs();
	
	pParentPipeline = NULL;
	pTUC = NULL;
	pParamBlock = NULL;
}

void deoglPersistentRenderTaskTexture::RemoveFromParentIfEmpty(){
	if(pVAOs.GetCount() > 0){
		return;
	}
	
	deoglPersistentRenderTaskPipeline * const pipeline = pParentPipeline;
	pipeline->RemoveTexture(this); // clears pParentShader
	pipeline->RemoveFromParentIfEmpty();
}
