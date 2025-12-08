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

#include "deoglDSRenderableVideoFrame.h"
#include "render/deoglRDSRenderableVideoFrame.h"
#include "../deoglDynamicSkin.h"
#include "../../../video/deoglVideoPlayer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableVideoFrame.h>
#include <dragengine/resources/video/deVideoPlayer.h>



// Class deoglDSRenderableVideoFrame
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableVideoFrame::deoglDSRenderableVideoFrame(deoglDynamicSkin &dynamicSkin,
	const deDSRenderableVideoFrame &renderable) :
deoglDSRenderable(dynamicSkin, renderable),
pRenderableVideoFrame(renderable),
pRRenderableVideoFrame(NULL),
pVideoPlayer(NULL),
pDirty(true)
{
	try{
		pRRenderableVideoFrame.TakeOverWith*dynamicSkin.GetRDynamicSkin());
		
		if(renderable.GetVideoPlayer()){
			pVideoPlayer = (deoglVideoPlayer*)renderable.GetVideoPlayer()->GetPeerGraphic();
			pVideoPlayer->GetNotifyRenderables().Add(this);
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableVideoFrame::~deoglDSRenderableVideoFrame(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableVideoFrame::GetRRenderable() const{
	return pRRenderableVideoFrame;
}

void deoglDSRenderableVideoFrame::RenderableChanged(){
	deoglVideoPlayer * const videoPlayer = pRenderableVideoFrame.GetVideoPlayer()
		? (deoglVideoPlayer*)pRenderableVideoFrame.GetVideoPlayer()->GetPeerGraphic() : NULL;
	
	if(pVideoPlayer != videoPlayer){
		if(pVideoPlayer){
			pVideoPlayer->GetNotifyRenderables().Remove(this);
		}
		
		pVideoPlayer = videoPlayer;
		
		if(videoPlayer){
			videoPlayer->GetNotifyRenderables().Add(this);
		}
		
		pDirty = true;
		
		pDynamicSkin.NotifyRenderableChanged(*this);
	}
	
	if(pRenderableVideoFrame.GetName() != pRRenderableVideoFrame->GetName()){
		pDynamicSkin.NotifyRenderablesChanged();
	}
}

void deoglDSRenderableVideoFrame::SyncToRender(){
	if(pVideoPlayer){
		pVideoPlayer->SyncToRender();
	}
	
	if(pDirty){
		pRRenderableVideoFrame->SetName(pRenderableVideoFrame.GetName());
		
		if(pVideoPlayer){
			pRRenderableVideoFrame->SetVideoPlayer(pVideoPlayer->GetRVideoPlayer());
			
		}else{
			pRRenderableVideoFrame->SetVideoPlayer(NULL);
		}
		
		pDirty = false;
	}
}

void deoglDSRenderableVideoFrame::VideoPlayerRequiresSync(){
	pDynamicSkin.NotifyRenderableRequiresSync(*this);
}

void deoglDSRenderableVideoFrame::DropVideoPlayer(){
	pVideoPlayer = NULL;
}



// Private Functions
//////////////////////

void deoglDSRenderableVideoFrame::pCleanUp(){
	
	if(pVideoPlayer){
		pVideoPlayer->GetNotifyRenderables().Remove(this);
	}
}
