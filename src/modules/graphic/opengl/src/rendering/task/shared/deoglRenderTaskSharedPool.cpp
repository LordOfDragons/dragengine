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

#include "deoglRenderTaskSharedPool.h"
#include "deoglRenderTaskSharedTexture.h"
#include "deoglRenderTaskSharedVAO.h"
#include "deoglRenderTaskSharedInstance.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Class deoglRenderTaskSharedPool
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskSharedPool::deoglRenderTaskSharedPool(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
}

deoglRenderTaskSharedPool::~deoglRenderTaskSharedPool(){
}



// Management
///////////////

deoglRenderTaskSharedTexture *deoglRenderTaskSharedPool::GetTexture(){
	deoglRenderTaskSharedTexture *texture;
	
	const int index = pFreeTextures.GetCount() - 1;
	if(index > -1){
		texture = static_cast<deoglRenderTaskSharedTexture*>(pFreeTextures.GetAt(index));
		pFreeTextures.RemoveFrom(index);
		
	}else{
		const deoglRenderTaskSharedTexture::Ref newTexture(
			deoglRenderTaskSharedTexture::Ref::NewWith(*this, pTextures.GetCount()));
		pTextures.Add(newTexture);
		texture = newTexture;
		
		if(pTextures.GetCount() % 200 == 0){
			pRenderThread.GetLogger().LogInfoFormat(
				"RenderTaskSharedPool: Reached %d Textures", pTextures.GetCount());
		}
	}
	
	return texture;
}

deoglRenderTaskSharedVAO *deoglRenderTaskSharedPool::GetVAO(){
	deoglRenderTaskSharedVAO *vao;
	
	const int index = pFreeVAOs.GetCount() - 1;
	if(index > -1){
		vao = static_cast<deoglRenderTaskSharedVAO*>(pFreeVAOs.GetAt(index));
		pFreeVAOs.RemoveFrom(index);
		
	}else{
		const deoglRenderTaskSharedVAO::Ref newVao(
			deoglRenderTaskSharedVAO::Ref::NewWith(*this, pVAOs.GetCount()));
		pVAOs.Add(newVao);
		vao = newVao;
		
		if(pVAOs.GetCount() % 100 == 0){
			pRenderThread.GetLogger().LogInfoFormat(
				"RenderTaskSharedPool: Reached %d VAOs", pVAOs.GetCount());
		}
	}
	
	return vao;
}

deoglRenderTaskSharedInstance *deoglRenderTaskSharedPool::GetInstance(){
	deoglRenderTaskSharedInstance *instance;
	
	const int index = pFreeInstances.GetCount() - 1;
	if(index > -1){
		instance = static_cast<deoglRenderTaskSharedInstance*>(pFreeInstances.GetAt(index));
		pFreeInstances.RemoveFrom(index);
		
	}else{
		const deoglRenderTaskSharedInstance::Ref newInstance(
			deoglRenderTaskSharedInstance::Ref::NewWith(*this, pInstances.GetCount()));
		pInstances.Add(newInstance);
		instance = newInstance;
		
		if(pInstances.GetCount() % 500 == 0){
			pRenderThread.GetLogger().LogInfoFormat(
				"RenderTaskSharedPool: Reached %d Instances", pInstances.GetCount());
		}
	}
	
	return instance;
}

int deoglRenderTaskSharedPool::AssignSkinTexture(deoglSkinTexture *skinTexture){
	const deMutexGuard guard(pMutexSkinTextures);
	const int index = pFreeSkinTextures.GetCount() - 1;
	int slot;
	
	if(index > -1){
		slot = pFreeSkinTextures.GetAt(index);
		pFreeSkinTextures.RemoveFrom(index);
		pSkinTextures.SetAt(slot, skinTexture);
		
	}else{
		slot = pSkinTextures.GetCount();
		pSkinTextures.Add(skinTexture);
		
		if(pSkinTextures.GetCount() % 100 == 0){
			pRenderThread.GetLogger().LogInfoFormat(
				"RenderTaskSharedPool: Reached %d Skin Textures", pSkinTextures.GetCount());
		}
	}
	
	return slot;
}



deoglRenderTaskSharedTexture &deoglRenderTaskSharedPool::GetTextureAt(int index) const{
	return *static_cast<deoglRenderTaskSharedTexture*>(pTextures.GetAt(index));
}

deoglRenderTaskSharedVAO &deoglRenderTaskSharedPool::GetVAOAt(int index) const{
	return *static_cast<deoglRenderTaskSharedVAO*>(pVAOs.GetAt(index));
}

deoglRenderTaskSharedInstance &deoglRenderTaskSharedPool::GetInstanceAt(int index) const{
	return *static_cast<deoglRenderTaskSharedInstance*>(pInstances.GetAt(index));
}

deoglSkinTexture *deoglRenderTaskSharedPool::GetSkinTextureAt(int index) const{
	return (deoglSkinTexture*)pSkinTextures.GetAt(index);
}

int deoglRenderTaskSharedPool::GetSkinTextureCount() const{
	return pSkinTextures.GetCount();
}



void deoglRenderTaskSharedPool::ReturnTexture(deoglRenderTaskSharedTexture *texture){
	DEASSERT_NOTNULL(texture)
	
	texture->Clear();
	pFreeTextures.Add(texture);
}

void deoglRenderTaskSharedPool::ReturnVAO(deoglRenderTaskSharedVAO *vao){
	DEASSERT_NOTNULL(vao)
	
	vao->Clear();
	pFreeVAOs.Add(vao);
}

void deoglRenderTaskSharedPool::ReturnInstance(deoglRenderTaskSharedInstance *instance){
	DEASSERT_NOTNULL(instance)
	
	instance->Clear();
	pFreeInstances.Add(instance);
}

void deoglRenderTaskSharedPool::ReturnSkinTexture(int slot){
	const deMutexGuard guard(pMutexSkinTextures);
	pSkinTextures.SetAt(slot, nullptr);
	pFreeSkinTextures.Add(slot);
}
