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

#include "deoglRVideoPlayer.h"
#include "deoglRVideo.h"
#include "../deoglBasics.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRVideoPlayer
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRVideoPlayer::deoglRVideoPlayer(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pCachedFrameTexture(NULL),
pUpdateCachedFrameTexture(-1),

pWidth(1),
pHeight(1),
pComponentCount(3),

pTexture(NULL),
pDirtyTexture(false)
{
}

deoglRVideoPlayer::~deoglRVideoPlayer(){
	SetVideo(NULL);
	
	if(pTexture){
		delete pTexture;
	}
}



// Management
///////////////

void deoglRVideoPlayer::SetVideo(deoglRVideo *video){
	if(video == pVideo){
		return;
	}
	pVideo = video;
}

void deoglRVideoPlayer::SetCachedFrameTexture(deoglTexture *texture){
	pCachedFrameTexture = texture;
}

void deoglRVideoPlayer::SetUpdateCachedFrameTexture(int updateCachedFrameTexture){
	pUpdateCachedFrameTexture = updateCachedFrameTexture;
	if(updateCachedFrameTexture != -1){
		pDirtyTexture = true;
	}
}

bool deoglRVideoPlayer::HasCachedFrameTexture() const{
	return pCachedFrameTexture || pUpdateCachedFrameTexture != -1;
}



void deoglRVideoPlayer::SetVideoSize(int width, int height, int componentCount){
	if(width < 1 || height < 1 || componentCount < 3 || componentCount > 4){
		DETHROW(deeInvalidParam);
	}
	
	if(width == pWidth && height == pHeight && componentCount == pComponentCount){
		return;
	}
	
	pWidth = width;
	pHeight = height;
	pComponentCount = componentCount;
	
	pPixelBuffer = nullptr;
	
	if(pTexture){
		delete pTexture;
		pTexture = NULL;
	}
	
	pUpdateCachedFrameTexture = -1;
	pCachedFrameTexture = NULL;
	
	pDirtyTexture = true;
}

deoglPixelBuffer::Ref deoglRVideoPlayer::SetPixelBuffer(deoglPixelBuffer *pixelBuffer){
	const deoglPixelBuffer::Ref prevPixelBuffer(pPixelBuffer);
	
	pPixelBuffer = pixelBuffer;
	pDirtyTexture  = true;
	
	return prevPixelBuffer;
}

deoglTexture *deoglRVideoPlayer::GetTexture() const{
	return pCachedFrameTexture ? pCachedFrameTexture : pTexture;
}



void deoglRVideoPlayer::UpdateTexture(){
	if(!pDirtyTexture){
		return;
	}
	
	if(pUpdateCachedFrameTexture != -1){
		pVideo->UpdateTexture();
		pCachedFrameTexture = pVideo->GetTexture(pUpdateCachedFrameTexture);
		pUpdateCachedFrameTexture = -1;
	}
	
	if(!pCachedFrameTexture){
		//pRenderThread.GetLogger().LogInfo( "VideoPlayer: update not cached" );
		if(!pTexture){
			pTexture = new deoglTexture(pRenderThread);
			pTexture->SetSize(pWidth, pHeight);
			pTexture->SetMapingFormat(pComponentCount, false, false);
			pTexture->SetMipMapped(false); // true would be nicer but doing it every frame is a waste
			pTexture->CreateTexture();
		}
		
		if(pPixelBuffer){
			pTexture->SetPixels(pPixelBuffer);
		}
	}
	
	pDirtyTexture = false;
}
