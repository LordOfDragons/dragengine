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
#include "deoglPersistentRenderTaskPipeline.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "deoglPersistentRenderTaskSubInstance.h"
#include "deoglPersistentRenderTaskOwner.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskPool
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskPool::deoglPersistentRenderTaskPool()
/*:
pOwners(20000),
pPipelines(100),
pTextures(200),
pVAOs(2000),
pInstances(5000)
*/
{
	try{
		pCreateInitial();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglPersistentRenderTaskPool::~deoglPersistentRenderTaskPool(){
	pCleanUp();
}



// Management
///////////////

deoglPersistentRenderTaskOwner *deoglPersistentRenderTaskPool::GetOwner(){
	deoglPersistentRenderTaskOwner *owner;
	
	const int index = pOwners.GetCount() - 1;
	if(index > -1){
		owner = (deoglPersistentRenderTaskOwner*)pOwners.GetAt(index);
		pOwners.RemoveFrom(index);
		
	}else{
		owner = new deoglPersistentRenderTaskOwner;
	}
	
	return owner;
}

deoglPersistentRenderTaskPipeline *deoglPersistentRenderTaskPool::GetPipeline(){
	deoglPersistentRenderTaskPipeline *pipeline;
	
	const int index = pPipelines.GetCount() - 1;
	if(index > -1){
		pipeline = (deoglPersistentRenderTaskPipeline*)pPipelines.GetAt(index);
		pPipelines.RemoveFrom(index);
		
	}else{
		pipeline = new deoglPersistentRenderTaskPipeline(*this);
	}
	
	return pipeline;
}

deoglPersistentRenderTaskTexture *deoglPersistentRenderTaskPool::GetTexture(){
	deoglPersistentRenderTaskTexture *texture;
	
	const int index = pTextures.GetCount() - 1;
	if(index > -1){
		texture = (deoglPersistentRenderTaskTexture*)pTextures.GetAt(index);
		pTextures.RemoveFrom(index);
		
	}else{
		texture = new deoglPersistentRenderTaskTexture(*this);
	}
	
	return texture;
}

deoglPersistentRenderTaskVAO *deoglPersistentRenderTaskPool::GetVAO(){
	deoglPersistentRenderTaskVAO *vao;
	
	const int index = pVAOs.GetCount() - 1;
	if(index > -1){
		vao = (deoglPersistentRenderTaskVAO*)pVAOs.GetAt(index);
		pVAOs.RemoveFrom(index);
		
	}else{
		vao = new deoglPersistentRenderTaskVAO(*this);
	}
	
	return vao;
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskPool::GetInstance(){
	deoglPersistentRenderTaskInstance *instance;
	
	const int index = pInstances.GetCount() - 1;
	if(index > -1){
		instance = (deoglPersistentRenderTaskInstance*)pInstances.GetAt(index);
		pInstances.RemoveFrom(index);
		
	}else{
		instance = new deoglPersistentRenderTaskInstance(*this);
	}
	
	return instance;
}

deoglPersistentRenderTaskSubInstance *deoglPersistentRenderTaskPool::GetSubInstance(){
	deoglPersistentRenderTaskSubInstance *subInstance;
	
	const int index = pSubInstances.GetCount() - 1;
	if(index > -1){
		subInstance = (deoglPersistentRenderTaskSubInstance*)pSubInstances.GetAt(index);
		pSubInstances.RemoveFrom(index);
		
	}else{
		subInstance = new deoglPersistentRenderTaskSubInstance(*this);
	}
	
	return subInstance;
}



void deoglPersistentRenderTaskPool::ReturnOwner(deoglPersistentRenderTaskOwner *owner){
	if(! owner){
		DETHROW(deeInvalidParam);
	}
	
	owner->Clear();
	pOwners.Add(owner);
}

void deoglPersistentRenderTaskPool::ReturnPipeline(deoglPersistentRenderTaskPipeline *pipeline){
	if(! pipeline){
		DETHROW(deeInvalidParam);
	}
	
	pipeline->Clear();
	pPipelines.Add(pipeline);
}

void deoglPersistentRenderTaskPool::ReturnTexture(deoglPersistentRenderTaskTexture *texture){
	if(! texture){
		DETHROW(deeInvalidParam);
	}
	
	texture->Clear();
	pTextures.Add(texture);
}

void deoglPersistentRenderTaskPool::ReturnVAO(deoglPersistentRenderTaskVAO *vao){
	if(! vao){
		DETHROW(deeInvalidParam);
	}
	
	vao->Clear();
	pVAOs.Add(vao);
}

void deoglPersistentRenderTaskPool::ReturnInstance(deoglPersistentRenderTaskInstance *instance){
	if(! instance){
		DETHROW(deeInvalidParam);
	}
	
	instance->Clear();
	pInstances.Add(instance);
}

void deoglPersistentRenderTaskPool::ReturnSubInstance(deoglPersistentRenderTaskSubInstance *subInstance){
	if(! subInstance){
		DETHROW(deeInvalidParam);
	}
	
	subInstance->Clear();
	pSubInstances.Add(subInstance);
}



// Private Functions
//////////////////////

void deoglPersistentRenderTaskPool::pCleanUp(){
	int i, count = pSubInstances.GetCount();
	for(i=0; i<count; i++){
		delete (deoglPersistentRenderTaskSubInstance*)pSubInstances.GetAt(i);
	}
	
	count = pInstances.GetCount();
	for(i=0; i<count; i++){
		delete (deoglPersistentRenderTaskInstance*)pInstances.GetAt(i);
	}
	
	count = pVAOs.GetCount();
	for(i=0; i<count; i++){
		delete (deoglPersistentRenderTaskVAO*)pVAOs.GetAt(i);
	}
	
	count = pTextures.GetCount();
	for(i=0; i<count; i++){
		delete (deoglPersistentRenderTaskTexture*)pTextures.GetAt(i);
	}
	
	count = pPipelines.GetCount();
	for(i=0; i<count; i++){
		delete (deoglPersistentRenderTaskPipeline*)pPipelines.GetAt(i);
	}
	
	count = pOwners.GetCount();
	for(i=0; i<count; i++){
		delete (deoglPersistentRenderTaskOwner*)pOwners.GetAt(i);
	}
}

void deoglPersistentRenderTaskPool::pCreateInitial(){
	// create some initial elements. this ensures the typical amount of elements created
	// are located close together in memory to improve performance.
	// 
	// typical values for sky shadow render plan:
	// - owners: 8000
	// - pipelines: 2
	// - textures: 20
	// - vaos: 600
	// - instances: 2500
	// - subinstances: 50000
	// 
	// camera render plans are similar but have less since they can cull better.
	// we allocate here elements for roughly 2 sky shadow maps. that should be
	// a good starting value.
	// 
	// the arrays use the amounts also as capacities in the constructors. this allocates
	// the memory ahead of time so it is faster to build the initial elements
	/*
	int i;
	
	for(i=0; i<20000; i++){
		pOwners.Add(new deoglPersistentRenderTaskOwner);
	}
	
	for(i=0; i<100; i++){
		pPipelines.Add(new deoglPersistentRenderTaskPipeline(*this));
	}
	
	for(i=0; i<200; i++){
		pTextures.Add(new deoglPersistentRenderTaskTexture(*this));
	}
	
	for(i=0; i<2000; i++){
		pVAOs.Add(new deoglPersistentRenderTaskVAO(*this));
	}
	
	for(i=0; i<5000; i++){
		pInstances.Add(new deoglPersistentRenderTaskInstance(*this));
	}
	*/
}
