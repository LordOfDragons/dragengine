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

#include "deoglLightBoundaryMap.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightBoundaryMap::deoglLightBoundaryMap(deoglRenderThread &renderThread, int size) :
pRenderThread(renderThread),

pTextureMin(NULL),
pTextureMax(NULL),

pPixBufBoundaryMin(deoglPixelBuffer::Ref::New(deoglPixelBuffer::epfFloat3, 1, 1, 1)),
pPixBufBoundaryMax(deoglPixelBuffer::Ref::New(deoglPixelBuffer::epfFloat3, 1, 1, 1)),

pSize(size),
pLevelCount(1)
{
	DEASSERT_TRUE(pSize >= 2)
	
	try{
		pCreateTextures();
		pCreateFBOs();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglLightBoundaryMap::~deoglLightBoundaryMap(){
	pCleanUp();
}



// Management
///////////////

int deoglLightBoundaryMap::GetBaseLevel(int baseSize){
	int tempSize = pSize;
	int baseLevel = 0;
	
	while(tempSize > baseSize){
		tempSize >>= 1;
		baseLevel++;
	}
	
	return baseLevel;
}



deoglFramebuffer *deoglLightBoundaryMap::GetFBOAt(int level){
	return (deoglFramebuffer*)pFBOs.GetAt(level);
}



void deoglLightBoundaryMap::GetResult(decVector &boundaryMin, decVector &boundaryMax){
	pTextureMin->GetPixelsLevel(pLevelCount - 1, pPixBufBoundaryMin);
	pTextureMax->GetPixelsLevel(pLevelCount - 1, pPixBufBoundaryMax);
	
	const deoglPixelBuffer::sFloat3 &resultMin = *pPixBufBoundaryMin->GetPointerFloat3();
	const deoglPixelBuffer::sFloat3 &resultMax = *pPixBufBoundaryMax->GetPointerFloat3();
	
	boundaryMin.x = (float)resultMin.r;
	boundaryMin.y = (float)resultMin.g;
	boundaryMin.z = (float)resultMin.b;
	
	boundaryMax.x = (float)resultMax.r;
	boundaryMax.y = (float)resultMax.g;
	boundaryMax.z = (float)resultMax.b;
}



// Private Functions
//////////////////////

void deoglLightBoundaryMap::pCleanUp(){
	pFBOs.RemoveAll();
	if(pTextureMax){
		delete pTextureMax;
	}
	if(pTextureMin){
		delete pTextureMin;
	}
}

void deoglLightBoundaryMap::pCreateTextures(){
	pLevelCount = (int)(ceilf(log2f((float)pSize))) + 1;
	
	pTextureMin = new deoglTexture(pRenderThread);
	pTextureMin->SetSize(pSize, pSize);
	pTextureMin->SetFBOFormat(3, true);
	pTextureMin->SetMipMapped(true);
	pTextureMin->SetMipMapLevelCount(pLevelCount - 1);
	pTextureMin->CreateTexture();
	
	pTextureMax = new deoglTexture(pRenderThread);
	pTextureMax->SetSize(pSize, pSize);
	pTextureMax->SetFBOFormat(3, true);
	pTextureMax->SetMipMapped(true);
	pTextureMax->SetMipMapLevelCount(pLevelCount - 1);
	pTextureMax->CreateTexture();
}

void deoglLightBoundaryMap::pCreateFBOs(){
	const GLenum buffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	const deoglRestoreFramebuffer restoreFbo(pRenderThread);
	int i;
	
	for(i=0; i<pLevelCount; i++){
		const deoglFramebuffer::Ref fbo(deoglFramebuffer::Ref::New(pRenderThread, false));
		
		pRenderThread.GetFramebuffer().Activate(fbo);
		
		fbo->AttachColorTextureLevel(0, pTextureMin, i);
		fbo->AttachColorTextureLevel(1, pTextureMax, i);
		
		OGL_CHECK(pRenderThread, pglDrawBuffers(2, &buffers[0]));
		
		fbo->Verify();
		pFBOs.Add(fbo);
	}
}
