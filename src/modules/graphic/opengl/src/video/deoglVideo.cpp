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

#include "deoglVideo.h"
#include "deoglRVideo.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/video/deVideo.h>



// Class deoglVideo
/////////////////////

// Constructor, destructor
////////////////////////////

deoglVideo::deoglVideo(deGraphicOpenGl &ogl, deVideo &video) :
pOgl(ogl),
pVideo(video),

pCachedFrameCount(0),
pCacheFrames(false)
{
	pDetermineCacheParams();
	pRVideo.TakeOverWith(ogl.GetRenderThread(), pVideo.GetWidth(),
		pVideo.GetHeight(), pVideo.GetComponentCount(), pCachedFrameCount);
}

deoglVideo::~deoglVideo(){
}



// Management
///////////////

bool deoglVideo::AllFramesAreCached() const{
	return pRVideo->GetFrameCountToCache() == 0;
}

bool deoglVideo::IsFrameReady(int frame) const{
	if(!pCacheFrames){
		return false;
	}
	return pRVideo->GetTexture(frame) != NULL;
}

bool deoglVideo::CanCacheFrame(int frame) const{
	if(!pCacheFrames){
		return false;
	}
	if(pRVideo->GetUpdateFrame() != -1){
		return false;
	}
	return pRVideo->GetTexture(frame) == NULL;
}

deoglPixelBuffer::Ref deoglVideo::CacheFrame(int frame, deoglPixelBuffer *pixelBuffer){
	DEASSERT_NOTNULL(pCacheFrames)
	return pRVideo->SetPixelBuffer(frame, pixelBuffer);
}

deoglTexture *deoglVideo::GetCachedFrameTexture(int frame) const{
	return pCacheFrames ? pRVideo->GetTexture(frame) : nullptr;
}



// Private functions
//////////////////////

void deoglVideo::pDetermineCacheParams(){
	// check if the video is small enough to warrant keeping all frames in memory.
	// what he sweet spot is for this threshold is difficult to say. certainly needs
	// a user tunable parameter since users of GPUs with larger RAM can decide to
	// speed things up by caching larger videos. for the time being we use a
	// total size threshold of 24MB uncompressed. Compressed this yields a total
	// memory consumption of 6MB.
	const int frameCount = pVideo.GetFrameCount();
	
	const int width = pVideo.GetWidth();
	const int height = pVideo.GetHeight();
	const int frameBytes = width * height * 3;
	const int thresholdFrames = 24000000 / frameBytes;
	
	pCacheFrames = (frameCount <= thresholdFrames);
	
	/*
	pOgl.LogInfoFormat("Video '%s': size=%ix%i frames=%i frameBytes=%i"
		" thresholdFrames=%i cacheFrames=%d", pVideo.GetFilename().GetString(), width,
		height, frameCount, frameBytes, thresholdFrames, pCacheFrames);
	*/
	
	if(pCacheFrames){
		pCachedFrameCount = frameCount;
	}
}
