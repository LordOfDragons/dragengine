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
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "../../../vbo/deoglVBOLayout.h"
#include "../../../utils/deoglQuickSorter.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskVAO
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskVAO::deoglPersistentRenderTaskVAO(deoglPersistentRenderTaskPool &pool) :
pPool(pool),
pLLTexture(this),

pParentTexture(NULL),
pVAO(NULL){
}

deoglPersistentRenderTaskVAO::~deoglPersistentRenderTaskVAO(){
	Clear();
}



// Management
///////////////

void deoglPersistentRenderTaskVAO::SetParentTexture(deoglPersistentRenderTaskTexture *texture){
	pParentTexture = texture;
}

void deoglPersistentRenderTaskVAO::SetVAO(const deoglVAO *vao){
	pVAO = vao;
}

int deoglPersistentRenderTaskVAO::GetTotalPointCount() const{
	decPointerLinkedList::cListEntry *iter = pInstances.GetRoot();
	int pointCount = 0;
	
	while(iter){
		const deoglPersistentRenderTaskInstance &instance = *((deoglPersistentRenderTaskInstance*)iter->GetOwner());
		pointCount += (instance.GetIndexCount() + instance.GetPointCount()) * decMath::max(instance.GetSubInstanceCount(), 1);
		iter = iter->GetNext();
	}
	
	return pointCount;
}

int deoglPersistentRenderTaskVAO::GetTotalSubInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pInstances.GetRoot();
	int subInstanceCount = 0;
	
	while(iter){
		subInstanceCount += ((deoglPersistentRenderTaskInstance*)iter->GetOwner())->GetSubInstanceCount();
		iter = iter->GetNext();
	}
	
	return subInstanceCount;
}



int deoglPersistentRenderTaskVAO::GetInstanceCount() const{
	return pInstances.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTaskVAO::GetRootInstance() const{
	return pInstances.GetRoot();
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskVAO::GetInstanceWith(
const deoglSharedSPBRTIGroup *group) const{
	if(!group){
		DETHROW(deeInvalidParam);
	}
	
	deoglPersistentRenderTaskInstance *instance;
	return pInstancesMap.GetAt(group, group->GetUniqueKey(), (void**)&instance) ? instance : NULL;
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskVAO::AddInstance(
deoglSharedSPB *spb, const deoglSharedSPBRTIGroup *group){
	deoglPersistentRenderTaskInstance * const instance = pPool.GetInstance();
	pInstances.Add(&instance->GetLLVAO());
	instance->SetParentVAO(this);
	instance->SetSubInstanceSPB(spb, group);
	if(group){
		pInstancesMap.SetAt(group, group->GetUniqueKey(), instance);
	}
	return instance;
}

void deoglPersistentRenderTaskVAO::RemoveInstance(deoglPersistentRenderTaskInstance *instance){
	if(!instance){
		DETHROW(deeInvalidParam);
	}
	
	if(instance->GetSubInstanceSPBGroup()){
		pInstancesMap.Remove(instance->GetSubInstanceSPBGroup(), instance->GetSubInstanceSPBGroup()->GetUniqueKey());
	}
	pInstances.Remove(&instance->GetLLVAO());
	pPool.ReturnInstance(instance);
}

void deoglPersistentRenderTaskVAO::RemoveAllInstances(){
	decPointerLinkedList::cListEntry *iter = pInstances.GetRoot();
	while(iter){
		pPool.ReturnInstance((deoglPersistentRenderTaskInstance*)iter->GetOwner());
		iter = iter->GetNext();
	}
	pInstances.RemoveAll();
	pInstancesMap.RemoveAll();
}



void deoglPersistentRenderTaskVAO::Clear(){
	RemoveAllInstances();
	
	pParentTexture = NULL;
	pVAO = NULL;
}

void deoglPersistentRenderTaskVAO::RemoveFromParentIfEmpty(){
	if(pInstances.GetCount() > 0){
		return;
	}
	
	deoglPersistentRenderTaskTexture * const texture = pParentTexture;
	texture->RemoveVAO(this); // clears pParentTexture
	texture->RemoveFromParentIfEmpty();
}
