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

#include "deoglImage.h"
#include "../deoglBasics.h"
#include "../deGraphicOpenGl.h"
#include "../canvas/deoglCanvasImage.h"
#include "../renderthread/deoglRenderThread.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/threading/deMutexGuard.h>



// Class deoglImage
/////////////////////

// Constructor, destructor
////////////////////////////

deoglImage::deoglImage(deGraphicOpenGl &ogl, deImage &image) :
pOgl(ogl),
pImage(image),
pRImage(deoglRImage::Ref::NewWith(ogl.GetRenderThread(), image)),
pPixelBufferUseCount(0),
pPixelBufferRImageTexture(pCreatePixelBuffer()),
pDirtyTexture(true){
}

deoglImage::~deoglImage(){
	if(pPixelBufferUseCount > 0){
		pOgl.LogErrorFormat("Image(%s): pPixelBufferUseCount > 0 (%d)",
			pImage.GetFilename().GetString(), pPixelBufferUseCount);
	}
	
	// notify owners we are about to be deleted. required since owners hold only a weak pointer
	// to the image and are notified only after switching to a new image. in this case they can
	// not use the old pointer to remove themselves from the image
	int i, count = pNotifyCanvas.GetCount();
	for(i=0; i<count; i++){
		((deoglCanvasImage*)pNotifyCanvas.GetAt(i))->DropImage();
	}
}



// Management
///////////////

void deoglImage::SyncToRender(){
	if(!pDirtyTexture){
		return;
	}
	
	if(!pPixelBufferRImageTexture){
		if(pPixelBuffer){
			// if there is a pixel buffer held already copy this one which is faster
			pPixelBufferRImageTexture.TakeOverWith(pPixelBuffer);
			
		}else{
			// if there is no pixel buffer held create a new one which can cause retaining
			pPixelBufferRImageTexture.TakeOver(pCreatePixelBuffer());
		}
	}
	
	pRImage->SetPixelBuffer(pPixelBufferRImageTexture);
	pPixelBufferRImageTexture = nullptr;
	
	pDirtyTexture = false;
}

void deoglImage::MarkTextureDirty(){
	pDirtyTexture = true;
	pRequiresSync();
}



void deoglImage::CreatePixelBuffer(){
	// NOTE Can be called from non-main threads
	
	deMutexGuard lock(pMutex);
	
	pPixelBufferUseCount++;
	if(pPixelBufferUseCount != 1){
		return;
	}
	
	// first use of pixel buffer. create pixel buffer retaining image data if required
	if(!pPixelBuffer){
		pPixelBuffer.TakeOver(pCreatePixelBuffer());
	}
}

void deoglImage::ReleasePixelBuffer(){
	// NOTE Can be called from non-main threads
	
	deMutexGuard lock(pMutex);
	
	pPixelBufferUseCount--;
	if(pPixelBufferUseCount > 0){
		return;
	}
	
	// last use of pixel buffer removed. delete pixel buffer
	pPixelBuffer = nullptr;
}



// Notifications
//////////////////

void deoglImage::ImageDataChanged(){
	pDirtyTexture = true;
	pPixelBufferRImageTexture = nullptr;
	pRequiresSync();
}

void deoglImage::ImageDataRestored(){
	if(!pPixelBufferRImageTexture){
		pDirtyTexture = true;
		pRequiresSync();
	}
}

bool deoglImage::RetainImageData(){
	return !pOgl.GetConfiguration().GetEnableRetainImageOptimization();
}



// Private Functions
//////////////////////

deoglPixelBuffer::Ref deoglImage::pCreatePixelBuffer(){
	deoglPixelBuffer::Ref pixelBuffer;
	
	if(pImage.GetData()){
		// somebody else keeps the image data retained so jump the bandwagon
		pCreatePixelBufferSafe(pixelBuffer);
		
	}else{
		// the image data is not retained. retain the imate data to create the pixel buffer
		// then release the image data.
		pImage.RetainImageData();
		
		try{
			pCreatePixelBufferSafe(pixelBuffer);
			
		}catch(const deException &){
			pImage.ReleaseImageData();
			throw;
		}
		
		try{
			pImage.ReleaseImageData();
		}catch(const deException &){
		}
	}
	
	return pixelBuffer;
}

void deoglImage::pCreatePixelBufferSafe(deoglPixelBuffer::Ref &pixelBuffer){
	const int componentCount = pImage.GetComponentCount();
	const int bitCount = pImage.GetBitCount();
	const int width = pImage.GetWidth();
	const int height = pImage.GetHeight();
	const int depth = pImage.GetDepth();
	const int strideLine = width;
	const int strideLayer = strideLine * height;
	int x, y, l;
	
	if(componentCount == 1){
		if(bitCount == 8){
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfByte1, width, height, depth);
			const sGrayscale8 * const srcData = pImage.GetDataGrayscale8();
			
			if(srcData){
				deoglPixelBuffer::sByte1 * const destData = pixelBuffer->GetPointerByte1();
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sByte1 * const destLayer = destData + strideLayer * l;
					const sGrayscale8 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sByte1 * const destRow = destLayer + strideLine * y;
						const sGrayscale8 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLubyte)srcRow[x].value;
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
			
		}else if(bitCount == 16){
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfFloat1, width, height, depth);
			const sGrayscale16 * const srcData = pImage.GetDataGrayscale16();
			
			if(srcData){
				deoglPixelBuffer::sFloat1 * const destData = pixelBuffer->GetPointerFloat1();
				const float factor = 1.0f / 65535.0f;
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sFloat1 * const destLayer = destData + strideLayer * l;
					const sGrayscale16 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sFloat1 * const destRow = destLayer + strideLine * y;
						const sGrayscale16 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLfloat)(srcRow[x].value * factor);
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
			
		}else{
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfFloat1, width, height, depth);
			const sGrayscale32 * const srcData = pImage.GetDataGrayscale32();
			
			if(srcData){
				deoglPixelBuffer::sFloat1 * const destData = pixelBuffer->GetPointerFloat1();
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sFloat1 * const destLayer = destData + strideLayer * l;
					const sGrayscale32 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sFloat1 * const destRow = destLayer + strideLine * y;
						const sGrayscale32 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLfloat)srcRow[x].value;
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
		}
		
	}else if(componentCount == 2){
		if(bitCount == 8){
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfByte2, width, height, depth);
			const sGrayscaleAlpha8 * const srcData = pImage.GetDataGrayscaleAlpha8();
			
			if(srcData){
				deoglPixelBuffer::sByte2 * const destData = pixelBuffer->GetPointerByte2();
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sByte2 * const destLayer = destData + strideLayer * l;
					const sGrayscaleAlpha8 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sByte2 * const destRow = destLayer + strideLine * y;
						const sGrayscaleAlpha8 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLubyte)srcRow[x].value;
							destRow[x].g = (GLubyte)srcRow[x].alpha;
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
			
		}else if(bitCount == 16){
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfFloat2, width, height, depth);
			const sGrayscaleAlpha16 * const srcData = pImage.GetDataGrayscaleAlpha16();
			
			if(srcData){
				deoglPixelBuffer::sFloat2 * const destData = pixelBuffer->GetPointerFloat2();
				const float factor = 1.0f / 65535.0f;
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sFloat2 * const destLayer = destData + strideLayer * l;
					const sGrayscaleAlpha16 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sFloat2 * const destRow = destLayer + strideLine * y;
						const sGrayscaleAlpha16 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLfloat)(srcRow[x].value * factor);
							destRow[x].g = (GLfloat)(srcRow[x].alpha * factor);
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
			
		}else{
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfFloat2, width, height, depth);
			const sGrayscaleAlpha32 * const srcData = pImage.GetDataGrayscaleAlpha32();
			
			if(srcData){
				deoglPixelBuffer::sFloat2 * const destData = pixelBuffer->GetPointerFloat2();
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sFloat2 * const destLayer = destData + strideLayer * l;
					const sGrayscaleAlpha32 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sFloat2 * const destRow = destLayer + strideLine * y;
						const sGrayscaleAlpha32 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLfloat)srcRow[x].value;
							destRow[x].g = (GLfloat)srcRow[x].alpha;
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
		}
		
	}else if(componentCount == 3){
		if(bitCount == 8){
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfByte3, width, height, depth);
			const sRGB8 * const srcData = pImage.GetDataRGB8();
			
			if(srcData){
				deoglPixelBuffer::sByte3 * const destData = pixelBuffer->GetPointerByte3();
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sByte3 * const destLayer = destData + strideLayer * l;
					const sRGB8 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sByte3 * const destRow = destLayer + strideLine * y;
						const sRGB8 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLubyte)srcRow[x].red;
							destRow[x].g = (GLubyte)srcRow[x].green;
							destRow[x].b = (GLubyte)srcRow[x].blue;
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
			
		}else if(bitCount == 16){
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfFloat3, width, height, depth);
			const sRGB16 * const srcData = pImage.GetDataRGB16();
			
			if(srcData){
				deoglPixelBuffer::sFloat3 * const destData = pixelBuffer->GetPointerFloat3();
				const float factor = 1.0f / 65535.0f;
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sFloat3 * const destLayer = destData + strideLayer * l;
					const sRGB16 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sFloat3 * const destRow = destLayer + strideLine * y;
						const sRGB16 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLfloat)(srcRow[x].red * factor);
							destRow[x].g = (GLfloat)(srcRow[x].green * factor);
							destRow[x].b = (GLfloat)(srcRow[x].blue * factor);
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
			
		}else{
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfFloat3, width, height, depth);
			const sRGB32 * const srcData = pImage.GetDataRGB32();
			
			if(srcData){
				deoglPixelBuffer::sFloat3 * const destData = pixelBuffer->GetPointerFloat3();
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sFloat3 * const destLayer = destData + strideLayer * l;
					const sRGB32 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sFloat3 * const destRow = destLayer + strideLine * y;
						const sRGB32 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLfloat)srcRow[x].red;
							destRow[x].g = (GLfloat)srcRow[x].green;
							destRow[x].b = (GLfloat)srcRow[x].blue;
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
		}
		
	}else{ // componentCount == 4
		if(bitCount == 8){
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfByte4, width, height, depth);
			const sRGBA8 * const srcData = pImage.GetDataRGBA8();
			
			if(srcData){
				deoglPixelBuffer::sByte4 * const destData = pixelBuffer->GetPointerByte4();
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sByte4 * const destLayer = destData + strideLayer * l;
					const sRGBA8 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sByte4 * const destRow = destLayer + strideLine * y;
						const sRGBA8 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLubyte)srcRow[x].red;
							destRow[x].g = (GLubyte)srcRow[x].green;
							destRow[x].b = (GLubyte)srcRow[x].blue;
							destRow[x].a = (GLubyte)srcRow[x].alpha;
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
			
		}else if(bitCount == 16){
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfFloat4, width, height, depth);
			const sRGBA16 * const srcData = pImage.GetDataRGBA16();
			
			if(srcData){
				deoglPixelBuffer::sFloat4 * const destData = pixelBuffer->GetPointerFloat4();
				const float factor = 1.0f / 65535.0f;
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sFloat4 * const destLayer = destData + strideLayer * l;
					const sRGBA16 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sFloat4 * const destRow = destLayer + strideLine * y;
						const sRGBA16 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLfloat)(srcRow[x].red * factor);
							destRow[x].g = (GLfloat)(srcRow[x].green * factor);
							destRow[x].b = (GLfloat)(srcRow[x].blue * factor);
							destRow[x].a = (GLfloat)(srcRow[x].alpha * factor);
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
			
		}else{
			pixelBuffer.TakeOverWith(deoglPixelBuffer::epfFloat4, width, height, depth);
			const sRGBA32 * const srcData = pImage.GetDataRGBA32();
			
			if(srcData){
				deoglPixelBuffer::sFloat4 * const destData = pixelBuffer->GetPointerFloat4();
				
				for(l=0; l<depth; l++){
					deoglPixelBuffer::sFloat4 * const destLayer = destData + strideLayer * l;
					const sRGBA32 * const srcLayer = srcData + strideLayer * l;
					
					for(y=0; y<height; y++){
						deoglPixelBuffer::sFloat4 * const destRow = destLayer + strideLine * y;
						const sRGBA32 * const srcRow = srcLayer + strideLine * y;
						
						for(x=0; x<width; x++){
							destRow[x].r = (GLfloat)srcRow[x].red;
							destRow[x].g = (GLfloat)srcRow[x].green;
							destRow[x].b = (GLfloat)srcRow[x].blue;
							destRow[x].a = (GLfloat)srcRow[x].alpha;
						}
					}
				}
				
			}else{
				// image data not retained. use black instead. this is a usage bug
				pixelBuffer->SetToIntColor(0, 0, 0, 255);
			}
		}
	}
}

void deoglImage::pRequiresSync(){
	int i, count = pNotifyCanvas.GetCount();
	for(i=0; i<count; i++){
		((deoglCanvasImage*)pNotifyCanvas.GetAt(i))->ImageRequiresSync();
	}
}
