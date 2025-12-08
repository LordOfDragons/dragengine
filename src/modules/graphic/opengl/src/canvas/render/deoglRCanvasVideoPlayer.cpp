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

#include "deoglRCanvasVideoPlayer.h"
#include "../../video/deoglRVideoPlayer.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasVideoPlayer
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasVideoPlayer::deoglRCanvasVideoPlayer(deoglRenderThread &renderThread) :
deoglRCanvas(renderThread),
pVideoPlayer(NULL),
pTCClampMin(0.0f, 0.0f),
pTCClampMax(1.0f, 1.0f){
	LEAK_CHECK_CREATE(renderThread, CanvasVideoPlayer);
}

deoglRCanvasVideoPlayer::~deoglRCanvasVideoPlayer(){
	LEAK_CHECK_FREE(GetRenderThread(), CanvasVideoPlayer);
	SetVideoPlayer(NULL);
}



// Management
///////////////

void deoglRCanvasVideoPlayer::SetVideoPlayer(deoglRVideoPlayer *videoPlayer){
	if(pVideoPlayer == videoPlayer){
		return;
	}
	
	pVideoPlayer = videoPlayer;
}

void deoglRCanvasVideoPlayer::SetTCTransform(const decTexMatrix2 &transform){
	pTCTransform = transform;
}

void deoglRCanvasVideoPlayer::SetTCClampMinimum(const decVector2 &clamp){
	pTCClampMin = clamp;
}

void deoglRCanvasVideoPlayer::SetTCClampMaximum(const decVector2 &clamp){
	pTCClampMax = clamp;
}



void deoglRCanvasVideoPlayer::PrepareForRender(const deoglRenderPlanMasked *renderPlanMask){
	if(!pVideoPlayer){
		return;
	}
	
	deoglRCanvas::PrepareForRender(renderPlanMask);
	pVideoPlayer->UpdateTexture();
}

void deoglRCanvasVideoPlayer::Render(const deoglRenderCanvasContext &context){
	if(!pVideoPlayer){
		return;
	}
	
	deoglRenderCanvasContext videoPlayerContext(context, *this);
	videoPlayerContext.SetTCClampMinimum(pTCClampMin);
	videoPlayerContext.SetTCClampMaximum(pTCClampMax);
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasVideoPlayer(videoPlayerContext, *this);
}
