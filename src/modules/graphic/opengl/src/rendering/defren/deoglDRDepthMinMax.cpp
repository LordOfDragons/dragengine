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

#include "deoglDRDepthMinMax.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglRestoreFramebuffer.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../texture/arraytexture/deoglArrayTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

// WARNING DO NOT ENABLE FOR THE TIME BEING!!!
int deoglDRDepthMinMax::USAGE_VERSION = -1; // 0=2pixel, 1=2texture, 2=splitTexture, -1=disabled

deoglDRDepthMinMax::deoglDRDepthMinMax(deoglRenderThread &renderThread,
int width, int height, int layerCount, int maxLevelCount) :
pRenderThread(renderThread){
	if(width < 1 || height < 1 || layerCount < 1 || maxLevelCount < 0){
		DETHROW(deeInvalidParam);
	}
	
	pTexture = nullptr;
	pTextureMin = nullptr;
	pTextureMax = nullptr;
	
	pWidth = width;
	pHeight = height;
	pLayerCount = layerCount;
	pLevelCount = 1;
	pMaxLevelCount = maxLevelCount;
	
	try{
		pCreateTextures();
		pCreateFBOs();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDRDepthMinMax::~deoglDRDepthMinMax(){
	pCleanUp();
}



// Management
///////////////

deoglFramebuffer *deoglDRDepthMinMax::GetFBOAt(int level){
	return (deoglFramebuffer*)pFBOs[level];
}

deoglFramebuffer *deoglDRDepthMinMax::GetFBOMinAt(int level){
	return (deoglFramebuffer*)pFBOMin[level];
}

deoglFramebuffer *deoglDRDepthMinMax::GetFBOMaxAt(int level){
	return (deoglFramebuffer*)pFBOMax[level];
}



// Private Functions
//////////////////////

void deoglDRDepthMinMax::pCleanUp(){
	pFBOMax.RemoveAll();
	if(pTextureMax){
		delete pTextureMax;
	}
	
	pFBOMin.RemoveAll();
	if(pTextureMin){
		delete pTextureMin;
	}
	
	pFBOs.RemoveAll();
	if(pTexture){
		delete pTexture;
	}
}

void deoglDRDepthMinMax::pCreateTextures(){
	int largestSize;
	pLevelCount = 1;
	for(largestSize=decMath::max(pWidth, pHeight); largestSize>2; largestSize>>=1){
		pLevelCount++;
	}
	
	if(pLevelCount > pMaxLevelCount){
		//pLevelCount = pMaxLevelCount;
	}
	
	if(USAGE_VERSION == 0){
		pTexture = new deoglArrayTexture(pRenderThread);
		pTexture->SetSize(pWidth, pHeight, pLayerCount);
		//pTexture->SetFBOFormat( 2, true );
		pTexture->SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRG32F); // on the laptop eutfRG16 is faster
		pTexture->SetMipMapped(true);
		pTexture->SetMipMapLevelCount(pLevelCount);
		pTexture->CreateTexture();
		
	}else if(USAGE_VERSION == 1){
		pTextureMin = new deoglArrayTexture(pRenderThread);
		pTextureMin->SetSize(pWidth, pHeight, pLayerCount);
		pTextureMin->SetDepthFormat(false, false);
		pTextureMin->SetMipMapped(true);
		pTextureMin->SetMipMapLevelCount(pLevelCount - 1);
		pTextureMin->CreateTexture();
		
		pTextureMax = new deoglArrayTexture(pRenderThread);
		pTextureMax->SetSize(pWidth, pHeight, pLayerCount);
		pTextureMax->SetDepthFormat(false, false);
		pTextureMax->SetMipMapped(true);
		pTextureMax->SetMipMapLevelCount(pLevelCount - 1);
		pTextureMax->CreateTexture();
		
	}else if(USAGE_VERSION == 2){
		pTexture = new deoglArrayTexture(pRenderThread);
		pTexture->SetSize(pWidth << 1, pHeight, pLayerCount);
		pTexture->SetDepthFormat(false, false);
		pTexture->SetMipMapped(true);
		pTexture->SetMipMapLevelCount(pLevelCount - 1);
		pTexture->CreateTexture();
		
	}else{
		DETHROW_INFO(deeInvalidParam, "Disabled");
	}
}

void deoglDRDepthMinMax::pCreateFBOs(){
	const deoglRestoreFramebuffer restoreFbo(pRenderThread);
	int i;
	
	if(pLevelCount > 0){
		if(USAGE_VERSION == 0){
			const GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
			
			for(i=0; i<pLevelCount; i++){
				const deoglFramebuffer::Ref fbo(deoglFramebuffer::Ref::New(pRenderThread, false));
				pRenderThread.GetFramebuffer().Activate(fbo);
				
				fbo->AttachColorArrayTextureLevel(0, pTexture, i);
				
				OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
				OGL_CHECK(pRenderThread, glReadBuffer(GL_COLOR_ATTACHMENT0));
				
				fbo->Verify();
				pFBOs.Add(fbo);
			}
			
		}else if(USAGE_VERSION == 1){
			const GLenum buffers[1] = {GL_NONE};
			
			// min
			for(i=0; i<pLevelCount; i++){
				const deoglFramebuffer::Ref fbo(deoglFramebuffer::Ref::New(pRenderThread, false));
				pRenderThread.GetFramebuffer().Activate(fbo);
				
				fbo->AttachDepthArrayTextureLevel(pTextureMin, i);
				
				OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
				OGL_CHECK(pRenderThread, glReadBuffer(GL_NONE));
				
				fbo->Verify();
				pFBOMin.Add(fbo);
			}
			
			// max
			for(i=0; i<pLevelCount; i++){
				const deoglFramebuffer::Ref fbo(deoglFramebuffer::Ref::New(pRenderThread, false));
				pRenderThread.GetFramebuffer().Activate(fbo);
				
				fbo->AttachDepthArrayTextureLevel(pTextureMax, i);
				
				OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
				OGL_CHECK(pRenderThread, glReadBuffer(GL_NONE));
				
				fbo->Verify();
				pFBOMax.Add(fbo);
			}
			
		}else if(USAGE_VERSION == 2){
			const GLenum buffers[1] = {GL_NONE};
			
			for(i=0; i<pLevelCount; i++){
				const deoglFramebuffer::Ref fbo(deoglFramebuffer::Ref::New(pRenderThread, false));
				pRenderThread.GetFramebuffer().Activate(fbo);
				
				fbo->AttachDepthArrayTextureLevel(pTexture, i);
				
				OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
				OGL_CHECK(pRenderThread, glReadBuffer(GL_NONE));
				
				fbo->Verify();
				pFBOs.Add(fbo);
			}
			
		}else{
			DETHROW_INFO(deeInvalidParam, "Disabled");
		}
	}
}
