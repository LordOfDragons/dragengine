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

#include "deoglVideoDecodeThread.h"

#include "../deoglBasics.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideoDecoder.h>
#include <dragengine/threading/deMutexGuard.h>



// #define DEBUG_SYNC 1

#ifdef DEBUG_SYNC
	#define DEBUG_SYNC_MT_WAIT(s) printf("%p(%i): wait on semaphore %s\n", this, __LINE__, s);
	#define DEBUG_SYNC_MT_DONE(s) printf("%p(%i): done on semaphore %s\n", this, __LINE__, s);
	#define DEBUG_SYNC_MT_SIGNAL(s) printf("%p(%i): signal semaphore %s\n", this, __LINE__, s);
#else
	#define DEBUG_SYNC_MT_WAIT(s)
	#define DEBUG_SYNC_MT_DONE(s)
	#define DEBUG_SYNC_MT_SIGNAL(s)
#endif

// Class deoglVideoDecodeThread
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglVideoDecodeThread::deoglVideoDecodeThread(deVideoDecoder *decoder, deVideo *video) :
pDecoder(decoder),
pVideo(video),
pFrame(-1),
pNextFrame(-1),
pDecoding(false),
pHasDecodedFrame(false),
pShutDown(false)
{
	if(!decoder || !video){
		DETHROW(deeInvalidParam);
	}
	
	//printf( "Start decode thread %p\n", this );
	Start();
}

deoglVideoDecodeThread::~deoglVideoDecodeThread(){
	//printf( "Shut down decode thread %p\n", this );
	pSafelyShutDownThread();
}



// Management
///////////////

void deoglVideoDecodeThread::StartDecode(int frame){
	bool startDecoding = false;
	bool waitFinished = false;
	
	{
	const deMutexGuard lock(pMutex);
// 	printf( "DecodeThread(%p) StartDecode(%p,%p,%i)\n", this, decoder, video, frame );
	
	if(pDecoding && !pHasDecodedFrame){
		if(pFrame == frame){
			// if the new next frame to decode is identical to the currently decoded frame cancel the next
			// decoding to avoid wasting time decoding two times the same frame
			//printf( "DecodeThread(%p) StartDecode(%p,%p,%i): Next frame identical to current frame.\n", this, decoder, video, frame );
			pNextFrame = -1;
			
		}else{
			//printf( "DecodeThread(%p) StartDecode(%p,%p,%i): Set next frame to decode.\n", this, decoder, video, frame );
			pNextFrame = frame;
		}
		
	}else{
		waitFinished = (pDecoding && pHasDecodedFrame);
		
		if(pFrame != frame){
			//printf( "DecodeThread(%p) StartDecode(%p,%p,%i): Set frame to decode.\n", this, decoder, video, frame );
			pFrame = frame;
			startDecoding = true;
			
		}else{
			//printf( "DecodeThread(%p) StartDecode(%p,%p,%i): Same frame, not decoding.\n", this, decoder, video, frame );
		}
	}
	}
	
	if(waitFinished){
		DEBUG_SYNC_MT_WAIT("StartDecode() finished")
		pSemaphoreFinished.Wait();
		DEBUG_SYNC_MT_DONE("StartDecode() finished")
		pDecoding = false;
	}
	if(startDecoding){
		pDecoding = true;
		pHasDecodedFrame = false;
		DEBUG_SYNC_MT_SIGNAL("StartDecode() decode")
		pSemaphoreDecode.Signal();
	}
}

deoglPixelBuffer::Ref deoglVideoDecodeThread::GetTexturePixelBuffer(){
	bool decoding;
	{
	const deMutexGuard lock(pMutex);
	decoding = pDecoding;
	}
	
	if(decoding){
		DEBUG_SYNC_MT_WAIT("GetTexturePixelBuffer() finished")
		pSemaphoreFinished.Wait();
		DEBUG_SYNC_MT_DONE("GetTexturePixelBuffer() finished")
	}
	
	const deMutexGuard lock(pMutex);
	if(decoding){
		pDecoding = false;
	}
	
	if(pHasDecodedFrame){
		//printf( "DecodeThread(%p) GetTexturePixelBuffer %p.\n", this, pPixelBufferTexture );
		return pPixelBufferTexture;
		
	}else{
		//printf( "DecodeThread(%p) GetTexturePixelBuffer not decoding.\n", this );
		return nullptr;
	}
}

void deoglVideoDecodeThread::SetTexturePixelBuffer(deoglPixelBuffer *pixelBuffer){
	pPixelBufferTexture = pixelBuffer;
}

void deoglVideoDecodeThread::StopDecode(){
	// clear the next decoding parameters if set.
	// stops the decoder from decoding a new frame
	bool decoding;
	{
	const deMutexGuard lock(pMutex);
	pNextFrame = -1;
	decoding = pDecoding;
	}
	
	// wait for the decoding to finish if running
	if(decoding){
		DEBUG_SYNC_MT_WAIT("SetTexturePixelBuffer() finished")
		pSemaphoreFinished.Wait();
		DEBUG_SYNC_MT_DONE("SetTexturePixelBuffer() finished")
	}
	
	// clear the decoding parameters
	const deMutexGuard lock(pMutex);
	pFrame = -1;
	if(decoding){
		pDecoding = false;
	}
}



void deoglVideoDecodeThread::Run(){
// 	decTimer timer;
	while(true){
		{
		const deMutexGuard lock(pMutex);
		if(pShutDown){
			return;
		}
		}
		
		DEBUG_SYNC_MT_WAIT("Run() decode")
		pSemaphoreDecode.Wait();
		DEBUG_SYNC_MT_DONE("Run() decode")
		
		bool keepDecoding = true;
		while(keepDecoding){
			{
			const deMutexGuard lock(pMutex);
			if(pShutDown){
				DEBUG_SYNC_MT_SIGNAL("Run() finished then exit")
				pSemaphoreFinished.Signal();
				return;
			}
			}
			keepDecoding = false;
			
// 			timer.Reset();
			
			try{
				PreparePixelBuffers();
				DecodeFrame();
				
			}catch(const deException &){
				SetErrorPixelBuffer();
			}
			
// 			printf( "DecodeThread(%p) Run(%p,%p,%i): Decoded in %iys\n", this, pDecoder, pVideo, pFrame, ( int )( timer.GetElapsedTime() * 1e6f ) );
			
			const deMutexGuard lock(pMutex);
			if(pNextFrame != -1){
				pFrame = pNextFrame;
				pNextFrame = -1;
				
				keepDecoding = true;
// 				printf( "DecodeThread(%p) Run(%p,%p,%i): Decode next frame.\n", this, pDecoder, pVideo, pFrame );
				
			}else{
				pHasDecodedFrame = true;
// 				printf( "DecodeThread(%p) Run(%p,%p,%i): Finished decoding frame.\n", this, pDecoder, pVideo, pFrame );
			}
		}
		
		DEBUG_SYNC_MT_SIGNAL("Run() finished")
		pSemaphoreFinished.Signal();
	}
}



void deoglVideoDecodeThread::PreparePixelBuffers(){
	const int componentCount = pVideo->GetComponentCount();
	const int height = pVideo->GetHeight();
	const int width = pVideo->GetWidth();
	
	// determine the pixel buffer format to use
	deoglPixelBuffer::ePixelFormats pbFormat;
	
	switch(componentCount){
	case 1:
		pbFormat = deoglPixelBuffer::epfByte1;
		break;
		
	case 2:
		pbFormat = deoglPixelBuffer::epfByte2;
		break;
		
	case 3:
		pbFormat = deoglPixelBuffer::epfByte3;
		break;
		
	case 4:
		pbFormat = deoglPixelBuffer::epfByte4;
		break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "invalid component count");
	}
	
	// if the pixel buffers do not match the required size free them
	if(pPixelBufferTexture){
		if(pPixelBufferTexture->GetWidth() != width || pPixelBufferTexture->GetHeight() != height){
			pPixelBufferTexture = nullptr;
		}
	}
	
	if(pPixelBufferDecode){
		if(pPixelBufferDecode->GetWidth() != width || pPixelBufferDecode->GetHeight() != height){
			pPixelBufferDecode = nullptr;
		}
	}
	
	// create pixel buffers if not existing
	if(!pPixelBufferDecode){
		pPixelBufferDecode.TakeOver(new deoglPixelBuffer(pbFormat, width, height, 1));
	}
	
	if(!pPixelBufferTexture){
		pPixelBufferTexture.TakeOver(new deoglPixelBuffer(pbFormat, width, height, 1));
	}
}

void deoglVideoDecodeThread::DecodeFrame(){
	if(pFrame == -1){
		return; // sanity check
	}
	
	const int height = pVideo->GetHeight();
	const int width = pVideo->GetWidth();
	
	pDecoder->SetPosition(pFrame); // in most cases this should not seek
	
	if(!pDecoder->DecodeFrame(pPixelBufferDecode->GetPointer(), pPixelBufferDecode->GetImageSize())){
		SetErrorPixelBuffer();
		return;
	}
	
	const decColorMatrix3 &colorMatrix = pVideo->GetColorConversionMatrix();
	const float factor = 1.0f / 255.0f;
	decColor color;
	int x, y;
	
	switch(pPixelBufferTexture->GetFormat()){
	case deoglPixelBuffer::epfByte1:
		{
		const deoglPixelBuffer::sByte1 * const pixelsDecode = pPixelBufferDecode->GetPointerByte1();
		deoglPixelBuffer::sByte1 * const pixelsTexture = pPixelBufferTexture->GetPointerByte1();
		
		for(y=0; y<height; y++){
			deoglPixelBuffer::sByte1 * const pixelLineTexture = pixelsTexture + width * (height - y - 1);
			const deoglPixelBuffer::sByte1 * const pixelLineDecode = pixelsDecode + width * y;
			
			for(x=0; x<width; x++){
				color.r = (float)pixelLineDecode[x].r * factor;
				
				color = colorMatrix * color;
				
				pixelLineTexture[x].r = decMath::clamp((int)(color.r * 255.0f), 0, 255);
			}
		}
		} break;
		
	case deoglPixelBuffer::epfByte2:
		{
		const deoglPixelBuffer::sByte2 * const pixelsDecode = pPixelBufferDecode->GetPointerByte2();
		deoglPixelBuffer::sByte2 * const pixelsTexture = pPixelBufferTexture->GetPointerByte2();
		
		for(y=0; y<height; y++){
			deoglPixelBuffer::sByte2 * const pixelLineTexture = pixelsTexture + width * (height - y - 1);
			const deoglPixelBuffer::sByte2 * const pixelLineDecode = pixelsDecode + width * y;
			
			for(x=0; x<width; x++){
				color.r = (float)pixelLineDecode[x].r * factor;
				color.g = (float)pixelLineDecode[x].g * factor;
				
				color = colorMatrix * color;
				
				pixelLineTexture[x].r = decMath::clamp((int)(color.r * 255.0f), 0, 255);
				pixelLineTexture[x].g = decMath::clamp((int)(color.g * 255.0f), 0, 255);
			}
		}
		} break;
		
	case deoglPixelBuffer::epfByte3:
		{
		const deoglPixelBuffer::sByte3 * const pixelsDecode = pPixelBufferDecode->GetPointerByte3();
		deoglPixelBuffer::sByte3 * const pixelsTexture = pPixelBufferTexture->GetPointerByte3();
		
		for(y=0; y<height; y++){
			deoglPixelBuffer::sByte3 * const pixelLineTexture = pixelsTexture + width * (height - y - 1);
			const deoglPixelBuffer::sByte3 * const pixelLineDecode = pixelsDecode + width * y;
			
			for(x=0; x<width; x++){
				color.r = (float)pixelLineDecode[x].r * factor;
				color.g = (float)pixelLineDecode[x].g * factor;
				color.b = (float)pixelLineDecode[x].b * factor;
				
				color = colorMatrix * color;
				
				pixelLineTexture[x].r = decMath::clamp((int)(color.r * 255.0f), 0, 255);
				pixelLineTexture[x].g = decMath::clamp((int)(color.g * 255.0f), 0, 255);
				pixelLineTexture[x].b = decMath::clamp((int)(color.b * 255.0f), 0, 255);
			}
		}
		} break;
		
	case deoglPixelBuffer::epfByte4:
		{
		const deoglPixelBuffer::sByte4 * const pixelsDecode = pPixelBufferDecode->GetPointerByte4();
		deoglPixelBuffer::sByte4 * const pixelsTexture = pPixelBufferTexture->GetPointerByte4();
		
		for(y=0; y<height; y++){
			deoglPixelBuffer::sByte4 * const pixelLineTexture = pixelsTexture + width * (height - y - 1);
			const deoglPixelBuffer::sByte4 * const pixelLineDecode = pixelsDecode + width * y;
			
			for(x=0; x<width; x++){
				color.r = (float)pixelLineDecode[x].r * factor;
				color.g = (float)pixelLineDecode[x].g * factor;
				color.b = (float)pixelLineDecode[x].b * factor;
				
				color = colorMatrix * color; // sets a=1 so alpha has to be set afterwards
				
				color.a = (float)pixelLineDecode[x].a * factor;
				
				pixelLineTexture[x].r = decMath::clamp((int)(color.r * 255.0f), 0, 255);
				pixelLineTexture[x].g = decMath::clamp((int)(color.g * 255.0f), 0, 255);
				pixelLineTexture[x].b = decMath::clamp((int)(color.b * 255.0f), 0, 255);
				pixelLineTexture[x].a = decMath::clamp((int)(color.a * 255.0f), 0, 255);
			}
		}
		
		} break;
		
	default:
		break;
	}
}

void deoglVideoDecodeThread::SetErrorPixelBuffer(){
	const int height = pVideo->GetHeight();
	const int width = pVideo->GetWidth();
	const int count = width * height;
	int i;
	
	switch(pPixelBufferTexture->GetFormat()){
	case deoglPixelBuffer::epfByte1:
		{
		deoglPixelBuffer::sByte1 * const pixels = pPixelBufferTexture->GetPointerByte1();
		for(i=0; i<count; i++){
			pixels[i].r = 255;
		}
		} break;
		
	case deoglPixelBuffer::epfByte2:
		{
		deoglPixelBuffer::sByte2 * const pixels = pPixelBufferTexture->GetPointerByte2();
		for(i=0; i<count; i++){
			pixels[i].r = 255;
			pixels[i].g = 0;
		}
		} break;
		
	case deoglPixelBuffer::epfByte3:
		{
		deoglPixelBuffer::sByte3 * const pixels = pPixelBufferTexture->GetPointerByte3();
		for(i=0; i<count; i++){
			pixels[i].r = 255;
			pixels[i].g = 0;
			pixels[i].b = 0;
		}
		} break;
		
	case deoglPixelBuffer::epfByte4:
		{
		deoglPixelBuffer::sByte4 * const pixels = pPixelBufferTexture->GetPointerByte4();
		for(i=0; i<count; i++){
			pixels[i].r = 255;
			pixels[i].g = 0;
			pixels[i].b = 0;
			pixels[i].a = 255;
		}
		} break;
		
	default:
		break;
	}
}



// Private Functions
//////////////////////

void deoglVideoDecodeThread::pSafelyShutDownThread(){
	if(!IsRunning()){
		return;
	}
	
	{
	const deMutexGuard lock(pMutex);
	pNextFrame = -1;
	pShutDown = true;
	}
	
	DEBUG_SYNC_MT_SIGNAL("pSafelyShutDownThread() decode")
	pSemaphoreDecode.Signal();
	WaitForExit();
}
