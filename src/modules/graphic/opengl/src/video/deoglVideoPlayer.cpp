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

#include "deoglVideoPlayer.h"
#include "deoglVideoDecodeThread.h"
#include "deoglRVideoPlayer.h"
#include "deoglVideo.h"
#include "../deGraphicOpenGl.h"
#include "../canvas/deoglCanvasVideoPlayer.h"
#include "../skin/dynamic/renderables/deoglDSRenderableVideoFrame.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideoDecoder.h>
#include <dragengine/resources/video/deVideoManager.h>



// Class deoglVideoPlayer
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglVideoPlayer::deoglVideoPlayer(deGraphicOpenGl &ogl, deVideoPlayer &videoPlayer) :
pOgl(ogl),
pVideoPlayer(videoPlayer),

pCurFrame(0),
pPredictedFrame(0),
pDirtyFrame(true),
pDirtySource(true),

pVideo(NULL),

pBrokenVideoDecoder(false),
pDecodeThread(NULL),

pRVideoPlayer(NULL)
{
	pRVideoPlayer.TakeOverWith(ogl.GetRenderThread());
	
	SourceChanged();
}

deoglVideoPlayer::~deoglVideoPlayer(){
	if(pDecodeThread){
		pDecodeThread->StopDecode();
		delete pDecodeThread;
	}
	
	
	// notify owners we are about to be deleted. required since owners hold only a weak pointer
	// to the dynamic skin and are notified only after switching to a new dynamic skin. in this
	// case they can not use the old pointer to remove themselves from the dynamic skin
	int i, count = pNotifyRenderables.GetCount();
	for(i=0; i<count; i++){
		((deoglDSRenderableVideoFrame*)pNotifyRenderables.GetAt(i))->DropVideoPlayer();
	}
	
	count = pNotifyCanvas.GetCount();
	for(i=0; i<count; i++){
		((deoglCanvasVideoPlayer*)pNotifyCanvas.GetAt(i))->DropVideoPlayer();
	}
}



// Management
///////////////

//#include <dragengine/common/utils/decTimer.h>

//static decTimer vTimer;

void deoglVideoPlayer::SetCurrentFrame(int frame){
	if(!pVideo){
		return;
	}
	
	frame = decMath::clamp(frame, 0, pVideo->GetVideo().GetFrameCount() - 1);
	if(pCurFrame == frame){
		return;
	}
	
	pCurFrame = frame;
	
	if(pVideo->AllFramesAreCached() || pVideo->IsFrameReady(frame)){
		pDirtyFrame = true;
		pRequiresSync();
		return;
	}
	
	pPredictNextFrame();
	//printf( "deoglVideoPlayer.SetCurrentFrame: cur=%i predictedNext=%i\n", pCurFrame, pPredictedNextFrame );
	
	if(pDecodeThread){
		//printf( "Start decoding current frame %i\n", pCurFrame );
		pDecodeThread->StartDecode(pCurFrame);
		
		// dirty only if new data arrives. this can be already decoded
		pDirtyFrame = true;
		pRequiresSync();
	}
}

void deoglVideoPlayer::UpdateNextFrame(){
	if(!pVideo){
		return;
	}
	
	SetCurrentFrame((int)(pVideoPlayer.GetPlayPosition() * pVideo->GetVideo().GetFrameRate()));
}



void deoglVideoPlayer::SyncToRender(){
	if(pDirtySource){
		if(pVideo){
			pRVideoPlayer->SetVideo(pVideo->GetRVideo());
			
		}else{
			pRVideoPlayer->SetVideo(NULL);
		}
		
		deVideo * const video = pVideoPlayer.GetVideo();
		
		if(video){
			pRVideoPlayer->SetVideoSize(video->GetWidth(), video->GetHeight(), video->GetComponentCount());
			
		}else{
			pRVideoPlayer->SetVideoSize(1, 1, 3); // dummy texture. maybe use a default opengl one?
		}
		
		pDirtyFrame = true;
		pDirtySource = false;
	}
	
	if(pDirtyFrame){
		if(!pVideo){
			pDirtyFrame = false;
			return;
		}
		
		if(pVideo->CacheFrames()){
			pRVideoPlayer->SetCachedFrameTexture(pVideo->GetCachedFrameTexture(pCurFrame));
			
			//printf( "CachedFrameTexture %i %p\n", pCurFrame, pRVideoPlayer->GetCachedFrameTexture() );
			if(!pRVideoPlayer->GetCachedFrameTexture() && pVideo->CanCacheFrame(pCurFrame)){
				const deoglPixelBuffer::Ref pixelBuffer(pDecodeThread->GetTexturePixelBuffer());
				if(pixelBuffer){
					pDecodeThread->SetTexturePixelBuffer(pVideo->CacheFrame(pCurFrame, pixelBuffer));
					pRVideoPlayer->SetUpdateCachedFrameTexture(pCurFrame);
				}
			}
		}
		
		if(!pRVideoPlayer->HasCachedFrameTexture()){
			//vTimer.Reset();
			const deoglPixelBuffer::Ref pixelBuffer(pDecodeThread->GetTexturePixelBuffer());
			//printf( "deoglVideoPlayer.UpdateTexture: Get decoded frame in %iys\n", ( int )( vTimer.GetElapsedTime() * 1e6f ) );
			
			// swap pixel buffers. the video player takes ownership of the pixel buffer until the next
			// swap is done. the decode thread can now decode a new frame without getting in the way
			// of the render thread. a NULL pixel buffer is fine causing the decode thread to create
			// a new pixel buffer. this one can create the texture inside the render thread preventing
			// the creation to take place here in the main thread
			if(pixelBuffer){
				pDecodeThread->SetTexturePixelBuffer(pRVideoPlayer->SetPixelBuffer(pixelBuffer));
			}
			//printf( "deoglVideoPlayer.UpdateTexture: Uploaded in %iys\n", ( int )( vTimer.GetElapsedTime() * 1e6f ) );
		}
		
		if(pVideo->AllFramesAreCached()){
			// if all frames are cached delete the decoder thread and release the decoder if present
			if(pDecodeThread){
				pOgl.LogInfoFormat("Stop decoding video %s (all frames cached)",
					pVideoPlayer.GetVideo()->GetFilename().GetString());
				pDecodeThread->StopDecode();
				delete pDecodeThread;
				pDecodeThread = NULL;
			}
			
			pVideoDecoder = NULL;
			pBrokenVideoDecoder = false;
			
		}else if(pDecodeThread && pPredictedFrame != pCurFrame){
			// start decoding the next frame if known and different than the current one. this does not
			// cause a dirty frame since we decode the frame by guessing ahead. only if the SetCurrentFrame
			// is called with the actual frame number the dirty frame is actually set
			
			//printf( "Start decoding predicted frame %i\n", pPredictedFrame );
			pDecodeThread->StartDecode(pPredictedFrame);
		}
		
		pDirtyFrame = false;
	}
}



// Notifications
//////////////////


void deoglVideoPlayer::SourceChanged(){
	if(pDecodeThread){
		pDecodeThread->StopDecode();
		delete pDecodeThread;
		pDecodeThread = NULL;
	}
	
	pVideoDecoder = NULL;
	pBrokenVideoDecoder = false;
	
	if(pVideoPlayer.GetVideo()){
		pVideo = (deoglVideo*)pVideoPlayer.GetVideo()->GetPeerGraphic();
		
	}else{
		pVideo = NULL;
	}
	
	if(pVideo && !pVideo->AllFramesAreCached()){
		try{
			pVideoDecoder.TakeOver(pOgl.GetGameEngine()->GetVideoManager()
				->CreateDecoder(pVideoPlayer.GetVideo()));
			
		}catch(const deException &e){
			pBrokenVideoDecoder = true;
			pOgl.LogErrorFormat("Failed creating video decoder for %s",
				pVideoPlayer.GetVideo()->GetFilename().GetString());
			pOgl.LogException(e);
		}
		
		if(pVideoDecoder){
			/*
			pOgl.LogInfoFormat("Start decoding video %s",
				pVideoPlayer.GetVideo()->GetFilename().GetString());
			*/
			pDecodeThread = new deoglVideoDecodeThread(pVideoDecoder, pVideoPlayer.GetVideo());
			pCurFrame = -1; // forces new frame to be decoded in all cases
		}
	}
	
	pDirtySource = true;
	UpdateNextFrame();
	
	pRequiresSync();
}



void deoglVideoPlayer::LoopingChanged(){
}

void deoglVideoPlayer::PlayRangeChanged(){
	UpdateNextFrame();
}

void deoglVideoPlayer::PlaySpeedChanged(){
}

void deoglVideoPlayer::PlayPositionChanged(bool /*seeking*/){
	UpdateNextFrame();
}



void deoglVideoPlayer::PlayStateChanged(){
}


// Private functions
//////////////////////

void deoglVideoPlayer::pPredictNextFrame(){
	pPredictedFrame = pCurFrame;
	
	if(!pVideoPlayer.GetPlaying() || !pVideoPlayer.GetVideo()){
		return;
	}
	
	const float frameRate = pVideoPlayer.GetVideo()->GetFrameRate();
	
	if(pVideoPlayer.GetLooping()){
		const int playFrom = (int)(pVideoPlayer.GetPlayFrom() * frameRate + 0.5f);
		const int playTo = (int)(pVideoPlayer.GetPlayTo() * frameRate + 0.5f);
		const int playRange = playTo - playFrom;
		
		if(playRange > 0){
			pPredictedFrame = playFrom + (pCurFrame + 1) % playRange;
		}
		
	}else{
		pPredictedFrame = decMath::min(pCurFrame + 1,
			(int)(pVideoPlayer.GetPlayTo() * frameRate + 0.5f));
	}
}

void deoglVideoPlayer::pRequiresSync(){
	int i, count = pNotifyRenderables.GetCount();
	for(i=0; i<count; i++){
		((deoglDSRenderableVideoFrame*)pNotifyRenderables.GetAt(i))->VideoPlayerRequiresSync();
	}
	
	count = pNotifyCanvas.GetCount();
	for(i=0; i<count; i++){
		((deoglCanvasVideoPlayer*)pNotifyCanvas.GetAt(i))->VideoPlayerRequiresSync();
	}
}
