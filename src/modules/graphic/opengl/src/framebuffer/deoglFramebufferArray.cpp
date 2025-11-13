/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoglFramebufferArray.h"
#include "deoglFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>


// Class deoglFramebufferArray
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglFramebufferArray::deoglFramebufferArray(deoglRenderThread &renderThread, int layerCount) :
pRenderThread(renderThread),
pLayerCount(layerCount)
{
	DEASSERT_TRUE(layerCount > 0)
	
	if(layerCount > 1 && /*renderThread.GetChoices().GetFBOArrayUseLayered()*/false){
		int i;
		for(i=0; i<layerCount; i++){
			pLayerFBOs.Add(deoglFramebuffer::Ref::NewWith(renderThread, false));
		}
		
	}else{
		pFBO.TakeOverWith(renderThread, false);
	}
}


// Management
///////////////

deoglFramebuffer *deoglFramebufferArray::GetLayerFBO(int layer) const{
	return (deoglFramebuffer*)pLayerFBOs.GetAt(layer);
}

deoglFramebuffer *deoglFramebufferArray::GetBaseFBO() const{
	return pFBO ? pFBO.Pointer() : GetLayerFBO(0);
}

void deoglFramebufferArray::Verify(){
	if(pFBO){
		pFBO->Verify();
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			GetLayerFBO(i)->Verify();
		}
	}
}


bool deoglFramebufferArray::IsColorAttached(int index) const{
	return GetBaseFBO()->IsColorAttached(index);
}

bool deoglFramebufferArray::IsColorDetached(int index) const{
	return GetBaseFBO()->IsColorDetached(index);
}

void deoglFramebufferArray::AttachColorArrayTexture(int index, deoglArrayTexture *texture){
	AttachColorArrayTextureLevel(index, texture, 0);
}

void deoglFramebufferArray::AttachColorArrayTextureLevel(int index, deoglArrayTexture *texture, int level){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->AttachColorArrayTextureLevel(index, texture, level);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->AttachColorArrayTextureLayerLevel(index, texture, i, level);
		}
	}
}

void deoglFramebufferArray::DetachColorImage(int index){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->DetachColorImage(index);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->DetachColorImage(index);
		}
	}
}

void deoglFramebufferArray::DetachColorImages(int startIndex){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->DetachColorImages(startIndex);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->DetachColorImages(startIndex);
		}
	}
}

void deoglFramebufferArray::InvalidateColor(int index){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->InvalidateColor(index);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->InvalidateColor(index);
		}
	}
}


bool deoglFramebufferArray::IsDepthAttached() const{
	return GetBaseFBO()->IsDepthAttached();
}

bool deoglFramebufferArray::IsDepthDetached() const{
	return GetBaseFBO()->IsDepthDetached();
}

void deoglFramebufferArray::AttachDepthArrayTexture(deoglArrayTexture *texture){
	AttachDepthArrayTextureLevel(texture, 0);
}

void deoglFramebufferArray::AttachDepthArrayTextureLevel(deoglArrayTexture *texture, int level){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->AttachDepthArrayTextureLevel(texture, level);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->AttachDepthArrayTextureLayerLevel(texture, i, level);
		}
	}
}

void deoglFramebufferArray::DetachDepthImage(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->DetachDepthImage();
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->DetachDepthImage();
		}
	}
}

void deoglFramebufferArray::InvalidateDepth(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->InvalidateDepth();
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->InvalidateDepth();
		}
	}
}


bool deoglFramebufferArray::IsStencilAttached() const{
	return GetBaseFBO()->IsStencilAttached();
}

bool deoglFramebufferArray::IsStencilDetached() const{
	return GetBaseFBO()->IsStencilDetached();
}

void deoglFramebufferArray::AttachStencilArrayTexture(deoglArrayTexture *texture){
	AttachStencilArrayTextureLevel(texture, 0);
}

void deoglFramebufferArray::AttachStencilArrayTextureLevel(deoglArrayTexture *texture, int level){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->AttachStencilArrayTextureLevel(texture, level);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->AttachStencilArrayTextureLayerLevel(texture, i, level);
		}
	}
}

void deoglFramebufferArray::DetachStencilImage(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->DetachStencilImage();
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->DetachStencilImage();
		}
	}
}

void deoglFramebufferArray::InvalidateStencil(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->InvalidateStencil();
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->InvalidateStencil();
		}
	}
}

void deoglFramebufferArray::InvalidateDepthStencil(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->InvalidateDepthStencil();
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->InvalidateDepthStencil();
		}
	}
}

void deoglFramebufferArray::AttachColorArrayTextures(deoglArrayTexture *texture1,
deoglArrayTexture *texture2, deoglArrayTexture *texture3, deoglArrayTexture *texture4,
deoglArrayTexture *texture5, deoglArrayTexture *texture6, deoglArrayTexture *texture7){
	AttachColorArrayTexturesLevel(0, texture1, texture2, texture3, texture4, texture5, texture6, texture7);
}

void deoglFramebufferArray::AttachColorArrayTexturesLevel(int level, deoglArrayTexture *texture1,
deoglArrayTexture *texture2, deoglArrayTexture *texture3, deoglArrayTexture *texture4,
deoglArrayTexture *texture5, deoglArrayTexture *texture6, deoglArrayTexture *texture7){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		if(texture1){
			pFBO->AttachColorArrayTextureLevel(0, texture1, level);
		}
		if(texture2){
			pFBO->AttachColorArrayTextureLevel(1, texture2, level);
		}
		if(texture3){
			pFBO->AttachColorArrayTextureLevel(2, texture3, level);
		}
		if(texture4){
			pFBO->AttachColorArrayTextureLevel(3, texture4, level);
		}
		if(texture5){
			pFBO->AttachColorArrayTextureLevel(4, texture5, level);
		}
		if(texture6){
			pFBO->AttachColorArrayTextureLevel(5, texture6, level);
		}
		if(texture7){
			pFBO->AttachColorArrayTextureLevel(6, texture7, level);
		}
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			if(texture1){
				fbo->AttachColorArrayTextureLayerLevel(0, texture1, i, level);
			}
			if(texture2){
				fbo->AttachColorArrayTextureLayerLevel(1, texture2, i, level);
			}
			if(texture3){
				fbo->AttachColorArrayTextureLayerLevel(2, texture3, i, level);
			}
			if(texture4){
				fbo->AttachColorArrayTextureLayerLevel(3, texture4, i, level);
			}
			if(texture5){
				fbo->AttachColorArrayTextureLayerLevel(4, texture5, i, level);
			}
			if(texture6){
				fbo->AttachColorArrayTextureLayerLevel(5, texture6, i, level);
			}
			if(texture7){
				fbo->AttachColorArrayTextureLayerLevel(6, texture7, i, level);
			}
		}
	}
}

void deoglFramebufferArray::AttachDepthStencilArrayTexture(deoglArrayTexture *texture){
	AttachDepthStencilArrayTextureLevel(texture, 0);
}

void deoglFramebufferArray::AttachDepthStencilArrayTextureLevel(deoglArrayTexture *texture, int level){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->AttachDepthArrayTextureLevel(texture, level);
		pFBO->AttachStencilArrayTextureLevel(texture, level);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->AttachDepthArrayTextureLayerLevel(texture, i, level);
			fbo->AttachStencilArrayTextureLayerLevel(texture, i, level);
		}
	}
}


void deoglFramebufferArray::DetachAllImages(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->DetachAllImages();
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->DetachAllImages();
		}
	}
}

void deoglFramebufferArray::InvalidateAllImages(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->InvalidateAllImages();
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			deoglFramebuffer * const fbo = GetLayerFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->InvalidateAllImages();
		}
	}
}


// Debugging
//////////////

void deoglFramebufferArray::DebugPrint(const char *prefix){
	if(pFBO){
		pFBO->DebugPrint(prefix);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			GetLayerFBO(i)->DebugPrint(prefix);
		}
	}
}

void deoglFramebufferArray::AddConfigToTrace(deErrorTracePoint &tracePoint){
	if(pFBO){
		pFBO->AddConfigToTrace(tracePoint);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			GetLayerFBO(i)->AddConfigToTrace(tracePoint);
		}
	}
}

void deoglFramebufferArray::SetDebugObjectLabel(const char *name){
	if(pFBO){
		pFBO->SetDebugObjectLabel(name);
		
	}else{
		int i;
		for(i=0; i<pLayerCount; i++){
			GetLayerFBO(i)->SetDebugObjectLabel(name);
		}
	}
}
