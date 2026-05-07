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

#include "deoglCaptureCanvas.h"
#include "deoglRCaptureCanvas.h"
#include "../render/deoglRCanvasView.h"
#include "../deoglCanvasView.h"
#include "../../deGraphicOpenGl.h"
#include "../../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../../texture/deoglImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvas.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/image/deImage.h>



// Class deoglCaptureCanvas
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCaptureCanvas::deoglCaptureCanvas(deGraphicOpenGl &ogl, deCaptureCanvas &captureCanvas) :
pOgl(ogl),
pCaptureCanvas(captureCanvas),

pDirtyCanvasView(true),
pCapturePending(false)
{
	ogl.GetCaptureCanvasList().Add(this);
}

deoglCaptureCanvas::~deoglCaptureCanvas(){
	pOgl.GetCaptureCanvasList().Remove(this);
}



// Management
///////////////

void deoglCaptureCanvas::SyncToRender(){
	if(!pRCaptureCanvas){
		pRCaptureCanvas = deoglRCaptureCanvas::Ref::New(GetOgl().GetRenderThread());
	}
	
	if(pDirtyCanvasView){
		deCanvasView * const canvasView = pCaptureCanvas.GetCanvasView();
		
		pRCaptureCanvas->DropPixelBuffer();
		
		if(canvasView){
			deoglCanvasView * const oglCanvasView = (deoglCanvasView*)canvasView->GetPeerGraphic();
			oglCanvasView->SyncToRender(); // required otherwise it does not work... strange
			pRCaptureCanvas->SetCanvasView(oglCanvasView->GetRCanvasView());
			
		}else{
			pRCaptureCanvas->SetCanvasView(nullptr);
		}
		
		pDirtyCanvasView = false;
	}
	
	// handle capturing if enabled
	if(!pCaptureCanvas.GetCapture()){
		return;
	}
	
	deImage * const image = pCaptureCanvas.GetImage();
	if(!image){
		return;
	}
	
	if(pCapturePending){
		if(!pRCaptureCanvas->GetCapturePending()){
			if(image->GetData()){ // in case the user forgot to retain the image data
				deoglPixelBuffer &pbuf = pRCaptureCanvas->GetPixelBuffer();
				
				if(image->GetBitCount() == 8 || image->GetBitCount() == 32){
					memcpy(image->GetData(), pbuf.GetPointer(), pbuf.GetImageSize());
					
				}else{
					// 16-bit can not be directly copied since the pixel buffer stores floats
					auto funcConvert = [](float value){
						return (unsigned short)decMath::clamp((int)(value * 65535.0f), 0, 65535);
					};
					
					const int pixCount = image->GetWidth() * image->GetHeight() * image->GetDepth();
					int i;
					
					switch(image->GetComponentCount()){
					case 1:{
						const deoglPixelBuffer::sFloat1 * const srcData = pbuf.GetPointerFloat1();
						sGrayscale16 * const destData = image->GetDataGrayscale16();
						for(i=0; i<pixCount; i++){
							destData[i].value = funcConvert(srcData[i].r);
						}
						}break;
						
					case 2:{
						const deoglPixelBuffer::sFloat2 * const srcData = pbuf.GetPointerFloat2();
						sGrayscaleAlpha16 * const destData = image->GetDataGrayscaleAlpha16();
						for(i=0; i<pixCount; i++){
							destData[i].value = funcConvert(srcData[i].r);
							destData[i].alpha = funcConvert(srcData[i].g);
						}
						}break;
						
					case 3:{
						const deoglPixelBuffer::sFloat3 * const srcData = pbuf.GetPointerFloat3();
						sRGB16 * const destData = image->GetDataRGB16();
						for(i=0; i<pixCount; i++){
							destData[i].red = funcConvert(srcData[i].r);
							destData[i].green = funcConvert(srcData[i].g);
							destData[i].blue = funcConvert(srcData[i].b);
						}
						}break;
						
					case 4:{
						const deoglPixelBuffer::sFloat4 * const srcData = pbuf.GetPointerFloat4();
						sRGBA16 * const destData = image->GetDataRGBA16();
						for(i=0; i<pixCount; i++){
							destData[i].red = funcConvert(srcData[i].r);
							destData[i].green = funcConvert(srcData[i].g);
							destData[i].blue = funcConvert(srcData[i].b);
							destData[i].alpha = funcConvert(srcData[i].a);
						}
						}break;
					}
				}
				
				image->NotifyImageDataChanged();
			}
			
			pRCaptureCanvas->DropPixelBuffer(); // save memory
			
			pCaptureCanvas.SetCapture(false);
			pCapturePending = false;
		}
		
	}else{
		// ensure canvas view is synchronized. required for rendering dynamic canvas view not
		// part of a render window
		deCanvasView * const canvasView = pCaptureCanvas.GetCanvasView();
		if(canvasView){
			((deoglCanvasView*)canvasView->GetPeerGraphic())->SyncToRender();
		}
		
		// now start capturing
		pRCaptureCanvas->StartCapture(image->GetWidth(), image->GetHeight(),
			image->GetComponentCount(), image->GetBitCount());
		pCapturePending = true;
	}
}



// Notifications
//////////////////

void deoglCaptureCanvas::CanvasViewChanged(){
	pDirtyCanvasView = true;
}

void deoglCaptureCanvas::ImageChanged(){
}

void deoglCaptureCanvas::CaptureChanged(){
}
