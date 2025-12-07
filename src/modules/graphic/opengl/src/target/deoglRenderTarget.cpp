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

#include "deoglRenderTarget.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTarget::deoglRenderTarget(deoglRenderThread &renderThread,
	const decPoint &size, int componentCount, int bitCount) :
pRenderThread(renderThread),

pSize(decPoint(1, 1).Largest(size)),
pTextureSize(pSize),
pAspectRatio((float)pSize.x / (float)pSize.y),
pBitCount(bitCount),
pComponentCount(componentCount),
pFloatTexture(bitCount != 8),

pDirtyTexture(true),

pTexture(NULL){
}

deoglRenderTarget::~deoglRenderTarget(){
	ReleaseFramebuffer();
	
	if(pTexture){
		delete pTexture;
	}
}



// Management
///////////////

void deoglRenderTarget::SetSize(const decPoint &size){
	if(! (size > decPoint())){
		DETHROW(deeInvalidParam);
	}
	
	if(size == pSize){
		return;
	}
	
	ReleaseFramebuffer();
	if(pTexture){
		delete pTexture;
		pTexture = NULL;
	}
	
	pSize = size;
	pAspectRatio = (float)pSize.x / (float)pSize.y;
	
	pTextureSize = pSize;
	
	pDirtyTexture = true;
}



void deoglRenderTarget::PrepareTexture(){
	if(pTexture){
		return;
	}
	
	pTexture = new deoglTexture(pRenderThread);
	pTexture->SetSize(pTextureSize);
	pTexture->SetFBOFormat(pComponentCount, pFloatTexture);
	pTexture->SetMipMapped(false);
	pTexture->CreateTexture(); // required or framebuffer attaching fails
}

void deoglRenderTarget::PrepareFramebuffer(){
	if(pFBO){
		return;
	}
	
	const deoglRestoreFramebuffer restoreFbo(pRenderThread);
	
	PrepareTexture();
	
	pFBO.TakeOverWith(pRenderThread, false);
	
	pRenderThread.GetFramebuffer().Activate(pFBO);
	
	pFBO->AttachColorTexture(0, pTexture);
	
	const GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
	OGL_CHECK(pRenderThread, pglDrawBuffers(1, buffers));
	OGL_CHECK(pRenderThread, glReadBuffer(GL_COLOR_ATTACHMENT0));
	
	pFBO->Verify();
}

void deoglRenderTarget::ReleaseFramebuffer(){
	pFBO = nullptr;
}


void deoglRenderTarget::SetTextureDirty(bool dirty){
	pDirtyTexture = dirty;
}

#if 0
void deoglRenderTarget::ClearTexture(){
	deoglPixelBuffer::ePixelFormats pbtype = deoglPixelBuffer::epfByte3;
	bool useFloat = false;
	
	if(pBitCount == 8){
		if(pComponentCount == 1){
			pbtype = deoglPixelBuffer::epfByte1;
			
		}else if(pComponentCount == 2){
			pbtype = deoglPixelBuffer::epfByte2;
			
		}else if(pComponentCount == 3){
			pbtype = deoglPixelBuffer::epfByte3;
			
		}else if(pComponentCount == 4){
			pbtype = deoglPixelBuffer::epfByte4;
		}
		
	}else{
		useFloat = true;
		
		if(pComponentCount == 1){
			pbtype = deoglPixelBuffer::epfFloat1;
			
		}else if(pComponentCount == 2){
			pbtype = deoglPixelBuffer::epfFloat2;
			
		}else if(pComponentCount == 3){
			pbtype = deoglPixelBuffer::epfFloat3;
			
		}else if(pComponentCount == 4){
			pbtype = deoglPixelBuffer::epfFloat4;
		}
	}
	
	deoglPixelBuffer pixelBuffer(pbtype, pTextureWidth, pTextureHeight, 1);
	const int pixelCount = pTextureWidth * pTextureHeight;
	
	if(useFloat){
		if(pComponentCount == 1){
			deoglPixelBuffer::sFloat1 * const destData = pixelBuffer.GetPointerFloat1();
			
			for(i=0; i< pixelCount; i++){
				destData[i].r = 0.0f;
			}
			
		}else if(pComponentCount == 2){
			deoglPixelBuffer::sFloat2 * const destData = pixelBuffer.GetPointerFloat2();
			
			for(i=0; i< pixelCount; i++){
				destData[i].r = 0.0f;
				destData[i].g = 1.0f;
			}
			
		}else if(pComponentCount == 3){
			deoglPixelBuffer::sFloat3 * const destData = pixelBuffer.GetPointerFloat3();
			
			for(i=0; i< pixelCount; i++){
				destData[i].r = 0.0f;
				destData[i].g = 0.0f;
				destData[i].b = 0.0f;
			}
			
		}else if(pComponentCount == 4){
			deoglPixelBuffer::sFloat4 * const destData = pixelBuffer.GetPointerFloat4();
			
			for(i=0; i< pixelCount; i++){
				destData[i].r = 0.0f;
				destData[i].g = 0.0f;
				destData[i].b = 0.0f;
				destData[i].a = 1.0f;
			}
		}
		
	}else{
		if(pComponentCount == 1){
			deoglPixelBuffer::sByte1 * const destData = pixelBuffer.GetPointerByte1();
			
			for(i=0; i< pixelCount; i++){
				destData[i].r = 0;
			}
			
		}else if(pComponentCount == 2){
			deoglPixelBuffer::sByte2 * const destData = pixelBuffer.GetPointerByte2();
			
			for(i=0; i< pixelCount; i++){
				destData[i].r = 0;
				destData[i].g = 255;
			}
			
		}else if(pComponentCount == 3){
			deoglPixelBuffer::sByte3 * const destData = pixelBuffer.GetPointerByte3();
			
			for(i=0; i< pixelCount; i++){
				destData[i].r = 0;
				destData[i].g = 0;
				destData[i].b = 0;
			}
			
		}else if(pComponentCount == 4){
			deoglPixelBuffer::sByte4 * const destData = pixelBuffer.GetPointerByte4();
			
			for(i=0; i< pixelCount; i++){
				destData[i].r = 0;
				destData[i].g = 0;
				destData[i].b = 0;
				destData[i].a = 255;
			}
		}
	}
	
	pTexture->SetPixels(pixelBuffer);
}
#endif
