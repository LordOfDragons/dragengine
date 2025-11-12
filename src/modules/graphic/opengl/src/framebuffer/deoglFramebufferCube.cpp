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

#include "deoglFramebufferCube.h"
#include "deoglFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>


// Class deoglFramebufferCube
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglFramebufferCube::deoglFramebufferCube(deoglRenderThread &renderThread) :
pRenderThread(renderThread)
{
	if(renderThread.GetChoices().GetFBOCubeUseLayered()){
		int i;
		for(i=0; i<6; i++){
			pFaceFBOs.Add(deoglFramebuffer::Ref::NewWith(renderThread, false));
		}
		
	}else{
		pFBO.TakeOverWith(renderThread, false);
	}
}


// Management
///////////////

deoglFramebuffer *deoglFramebufferCube::GetFaceFBO(int face) const{
	return (deoglFramebuffer*)pFaceFBOs.GetAt(face);
}

deoglFramebuffer *deoglFramebufferCube::GetBaseFBO() const{
	return pFBO ? pFBO.Pointer() : GetFaceFBO(0);
}

void deoglFramebufferCube::Verify(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->Verify();
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->Verify();
		}
	}
}


bool deoglFramebufferCube::IsColorAttached(int index) const{
	return GetBaseFBO()->IsColorAttached(index);
}

bool deoglFramebufferCube::IsColorDetached(int index) const{
	return GetBaseFBO()->IsColorDetached(index);
}

void deoglFramebufferCube::AttachColorCubeMap(int index, deoglCubeMap *texture){
	AttachColorCubeMapLevel(index, texture, 0);
}

void deoglFramebufferCube::AttachColorCubeMapLevel(int index, deoglCubeMap *texture, int level){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->AttachColorCubeMapLevel(index, texture, level);
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->AttachColorCubeMapFaceLevel(index, texture, i, level);
		}
	}
}

void deoglFramebufferCube::DetachColorImage(int index){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->DetachColorImage(index);
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->DetachColorImage(index);
		}
	}
}

void deoglFramebufferCube::DetachColorImages(int startIndex){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->DetachColorImages(startIndex);
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->DetachColorImages(startIndex);
		}
	}
}

void deoglFramebufferCube::InvalidateColor(int index){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->InvalidateColor(index);
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->InvalidateColor(index);
		}
	}
}


bool deoglFramebufferCube::IsDepthAttached() const{
	return GetBaseFBO()->IsDepthAttached();
}

bool deoglFramebufferCube::IsDepthDetached() const{
	return GetBaseFBO()->IsDepthDetached();
}

void deoglFramebufferCube::AttachDepthCubeMap(deoglCubeMap *texture){
	AttachDepthCubeMapLevel(texture, 0);
}

void deoglFramebufferCube::AttachDepthCubeMapLevel(deoglCubeMap *texture, int level){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->AttachDepthCubeMapLevel(texture, level);
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->AttachDepthCubeMapFaceLevel(texture, i, level);
		}
	}
}

void deoglFramebufferCube::DetachDepthImage(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->DetachDepthImage();
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->DetachDepthImage();
		}
	}
}

void deoglFramebufferCube::InvalidateDepth(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->InvalidateDepth();
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->InvalidateDepth();
		}
	}
}


void deoglFramebufferCube::DetachAllImages(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->DetachAllImages();
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->DetachAllImages();
		}
	}
}

void deoglFramebufferCube::InvalidateAllImages(){
	if(pFBO){
		pRenderThread.GetFramebuffer().Activate(pFBO);
		pFBO->InvalidateAllImages();
		
	}else{
		int i;
		for(i=0; i<6; i++){
			deoglFramebuffer * const fbo = GetFaceFBO(i);
			pRenderThread.GetFramebuffer().Activate(fbo);
			fbo->InvalidateAllImages();
		}
	}
}


// Debugging
//////////////

void deoglFramebufferCube::DebugPrint(const char *prefix){
	if(pFBO){
		pFBO->DebugPrint(prefix);
		
	}else{
		int i;
		for(i=0; i<6; i++){
			GetFaceFBO(i)->DebugPrint(prefix);
		}
	}
}

void deoglFramebufferCube::AddConfigToTrace(deErrorTracePoint &tracePoint){
	if(pFBO){
		pFBO->AddConfigToTrace(tracePoint);
		
	}else{
		int i;
		for(i=0; i<6; i++){
			GetFaceFBO(i)->AddConfigToTrace(tracePoint);
		}
	}
}

void deoglFramebufferCube::SetDebugObjectLabel(const char *name){
	if(pFBO){
		pFBO->SetDebugObjectLabel(name);
		
	}else{
		int i;
		for(i=0; i<6; i++){
			GetFaceFBO(i)->SetDebugObjectLabel(name);
		}
	}
}
