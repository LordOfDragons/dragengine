/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglVideoDecodeThread.h"

#include "../deoglBasics.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"

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

deoglVideoDecodeThread::deoglVideoDecodeThread( deVideoDecoder *decoder, deVideo *video ) :
pDecoder( decoder ),
pVideo( video ),
pFrame( -1 ),
pNextFrame( -1 ),

pPixelBufferDecode( NULL ),
pPixelBufferTexture( NULL ),

pDecoding( false ),
pHasDecodedFrame( false ),
pShutDown( false )
{
	if( ! decoder || ! video ){
		DETHROW( deeInvalidParam );
	}
	
	//printf( "Start decode thread %p\n", this );
	Start();
}

deoglVideoDecodeThread::~deoglVideoDecodeThread(){
	//printf( "Shut down decode thread %p\n", this );
	pSafelyShutDownThread();
	
	if( pPixelBufferTexture ){
		delete pPixelBufferTexture;
	}
	if( pPixelBufferDecode ){
		delete pPixelBufferDecode;
	}
}



// Management
///////////////

void deoglVideoDecodeThread::StartDecode( int frame ){
	bool startDecoding = false;
	bool waitFinished = false;
	
	deMutexGuard guard( pMutex );
	
// 	printf( "DecodeThread(%p) StartDecode(%p,%p,%i)\n", this, decoder, video, frame );
	
	if( pDecoding && ! pHasDecodedFrame ){
		if( pFrame == frame ){
			// if the new next frame to decode is identical to the currently decoded frame cancel the next
			// decoding to avoid wasting time decoding two times the same frame
			//printf( "DecodeThread(%p) StartDecode(%p,%p,%i): Next frame identical to current frame.\n", this, decoder, video, frame );
			pNextFrame = -1;
			
		}else{
			//printf( "DecodeThread(%p) StartDecode(%p,%p,%i): Set next frame to decode.\n", this, decoder, video, frame );
			pNextFrame = frame;
		}
		
	}else{
		waitFinished = ( pDecoding && pHasDecodedFrame );
		
		if( pFrame != frame ){
			//printf( "DecodeThread(%p) StartDecode(%p,%p,%i): Set frame to decode.\n", this, decoder, video, frame );
			pFrame = frame;
			startDecoding = true;
			
		}else{
			//printf( "DecodeThread(%p) StartDecode(%p,%p,%i): Same frame, not decoding.\n", this, decoder, video, frame );
		}
	}
	
	guard.Unlock();
	
	if( waitFinished ){
		DEBUG_SYNC_MT_WAIT("StartDecode() finished")
		pSemaphoreFinished.Wait();
		DEBUG_SYNC_MT_DONE("StartDecode() finished")
		pDecoding = false;
	}
	if( startDecoding ){
		pDecoding = true;
		pHasDecodedFrame = false;
		DEBUG_SYNC_MT_SIGNAL("StartDecode() decode")
		pSemaphoreDecode.Signal();
	}
}

deoglPixelBuffer *deoglVideoDecodeThread::GetTexturePixelBuffer(){
	if( pDecoding ){
		DEBUG_SYNC_MT_WAIT("GetTexturePixelBuffer() finished")
		pSemaphoreFinished.Wait();
		DEBUG_SYNC_MT_DONE("GetTexturePixelBuffer() finished")
		pDecoding = false;
	}
	
	if( pHasDecodedFrame ){
		//printf( "DecodeThread(%p) GetTexturePixelBuffer %p.\n", this, pPixelBufferTexture );
		return pPixelBufferTexture;
		
	}else{
		//printf( "DecodeThread(%p) GetTexturePixelBuffer not decoding.\n", this );
		return NULL;
	}
}

void deoglVideoDecodeThread::SetTexturePixelBuffer( deoglPixelBuffer *pixelBuffer ){
	pPixelBufferTexture = pixelBuffer;
}

void deoglVideoDecodeThread::StopDecode(){
	// clear the next decoding parameters if set.
	// stops the decoder from decoding a new frame
	deMutexGuard guard( pMutex );
	pNextFrame = -1;
	guard.Unlock();
	
	// wait for the decoding to finish if running
	if( pDecoding ){
		DEBUG_SYNC_MT_WAIT("SetTexturePixelBuffer() finished")
		pSemaphoreFinished.Wait();
		DEBUG_SYNC_MT_DONE("SetTexturePixelBuffer() finished")
		pDecoding = false;
	}
	
	// clear the decoding parameters
	pFrame = -1;
}



void deoglVideoDecodeThread::Run(){
	bool keepDecoding;
// 	decTimer timer;
	
	while( ! pShutDown ){
		DEBUG_SYNC_MT_WAIT("Run() decode")
		pSemaphoreDecode.Wait();
		DEBUG_SYNC_MT_DONE("Run() decode")
		
		if( pShutDown ){
			break;
		}
		
		keepDecoding = true;
		while( keepDecoding && ! pShutDown ){
			keepDecoding = false;
			
// 			timer.Reset();
			
			try{
				PreparePixelBuffers();
				DecodeFrame();
				
			}catch( const deException & ){
				SetErrorPixelBuffer();
			}
			
// 			printf( "DecodeThread(%p) Run(%p,%p,%i): Decoded in %iys\n", this, pDecoder, pVideo, pFrame, ( int )( timer.GetElapsedTime() * 1e6f ) );
			
			deMutexGuard guard( pMutex );
			
			if( pNextFrame != -1 ){
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
	deoglPixelBuffer::ePixelFormats pbFormat = deoglPixelBuffer::epfByte3;
	const deVideo::ePixelFormat pixelFormat = pVideo->GetPixelFormat();
	const int height = pVideo->GetHeight();
	const int width = pVideo->GetWidth();
	
	// determine the pixel buffer format to use
	if( pixelFormat == deVideo::epf4444 ){
		pbFormat = deoglPixelBuffer::epfByte4;
	}
	
	// if the pixel buffers do not match the required size free them
	if( pPixelBufferTexture ){
		if( pPixelBufferTexture->GetWidth() != width || pPixelBufferTexture->GetHeight() != height ){
			delete pPixelBufferTexture;
			pPixelBufferTexture = NULL;
		}
	}
	
	if( pPixelBufferDecode ){
		if( pPixelBufferDecode->GetWidth() != width || pPixelBufferDecode->GetHeight() != height ){
			delete pPixelBufferDecode;
			pPixelBufferDecode = NULL;
		}
	}
	
	// create pixel buffers if not existing
	if( ! pPixelBufferDecode ){
		pPixelBufferDecode = new deoglPixelBuffer( pbFormat, width, height, 1 );
	}
	
	if( ! pPixelBufferTexture ){
		pPixelBufferTexture = new deoglPixelBuffer( pbFormat, width, height, 1 );
	}
}

void deoglVideoDecodeThread::DecodeFrame(){
	if( pFrame == -1 ){
		return; // sanity check
	}
	
	const int height = pVideo->GetHeight();
	const int width = pVideo->GetWidth();
	
	pDecoder->SetPosition( pFrame ); // in most cases this should not seek
	
	if( ! pDecoder->DecodeFrame( pPixelBufferDecode->GetPointer(),
	pPixelBufferDecode->GetImageSize(), NULL, 0 ) ){
		SetErrorPixelBuffer();
		return;
	}
	
	const decColorMatrix3 &colorMatrix = pVideo->GetColorConversionMatrix();
	const float factor = 1.0f / 255.0f;
	decColor color;
	int x, y;
	
	switch( pPixelBufferTexture->GetFormat() ){
	case deoglPixelBuffer::epfByte3:
		{
		const deoglPixelBuffer::sByte3 * const pixelsDecode = pPixelBufferDecode->GetPointerByte3();
		deoglPixelBuffer::sByte3 * const pixelsTexture = pPixelBufferTexture->GetPointerByte3();
		
		for( y=0; y<height; y++ ){
			deoglPixelBuffer::sByte3 * const pixelLineTexture = pixelsTexture + width * ( height - y - 1 );
			const deoglPixelBuffer::sByte3 * const pixelLineDecode = pixelsDecode + width * y;
			
			for( x=0; x<width; x++ ){
				color.r = ( float )pixelLineDecode[ x ].r * factor;
				color.g = ( float )pixelLineDecode[ x ].g * factor;
				color.b = ( float )pixelLineDecode[ x ].b * factor;
				
				color = colorMatrix * color;
				
				pixelLineTexture[ x ].r = decMath::clamp( ( int )( color.r * 255.0f ), 0, 255 );
				pixelLineTexture[ x ].g = decMath::clamp( ( int )( color.g * 255.0f ), 0, 255 );
				pixelLineTexture[ x ].b = decMath::clamp( ( int )( color.b * 255.0f ), 0, 255 );
			}
		}
		} break;
		
	case deoglPixelBuffer::epfByte4:
		{
		const deoglPixelBuffer::sByte4 * const pixelsDecode = pPixelBufferDecode->GetPointerByte4();
		deoglPixelBuffer::sByte4 * const pixelsTexture = pPixelBufferTexture->GetPointerByte4();
		
		for( y=0; y<height; y++ ){
			deoglPixelBuffer::sByte4 * const pixelLineTexture = pixelsTexture + width * ( height - y - 1 );
			const deoglPixelBuffer::sByte4 * const pixelLineDecode = pixelsDecode + width * y;
			
			for( x=0; x<width; x++ ){
				color.r = ( float )pixelLineDecode[ x ].r * factor;
				color.g = ( float )pixelLineDecode[ x ].g * factor;
				color.b = ( float )pixelLineDecode[ x ].b * factor;
				color.a = ( float )pixelLineDecode[ x ].a * factor;
				
				color = colorMatrix * color;
				
				pixelLineTexture[ x ].r = decMath::clamp( ( int )( color.r * 255.0f ), 0, 255 );
				pixelLineTexture[ x ].g = decMath::clamp( ( int )( color.g * 255.0f ), 0, 255 );
				pixelLineTexture[ x ].b = decMath::clamp( ( int )( color.b * 255.0f ), 0, 255 );
				pixelLineTexture[ x ].a = decMath::clamp( ( int )( color.a * 255.0f ), 0, 255 );
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
	
	switch( pPixelBufferTexture->GetFormat() ){
	case deoglPixelBuffer::epfByte3:
		{
		deoglPixelBuffer::sByte3 * const pixels = pPixelBufferTexture->GetPointerByte3();
		for( i=0; i<count; i++ ){
			pixels[ i ].r = 255;
			pixels[ i ].g = 0;
			pixels[ i ].b = 0;
		}
		} break;
		
	case deoglPixelBuffer::epfByte4:
		{
		deoglPixelBuffer::sByte4 * const pixels = pPixelBufferTexture->GetPointerByte4();
		for( i=0; i<count; i++ ){
			pixels[ i ].r = 255;
			pixels[ i ].g = 0;
			pixels[ i ].b = 0;
			pixels[ i ].a = 255;
		}
		} break;
		
	default:
		break;
	}
}



// Private Functions
//////////////////////

void deoglVideoDecodeThread::pSafelyShutDownThread(){
	if( ! IsRunning() ){
		return;
	}
	
	deMutexGuard guard( pMutex );
	pNextFrame = -1;
	pShutDown = true;
	guard.Unlock();
	
	DEBUG_SYNC_MT_SIGNAL("pSafelyShutDownThread() decode")
	pSemaphoreDecode.Signal();
	WaitForExit();
}
