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

#include "deoglRVideo.h"
#include "../deoglBasics.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/video/deVideo.h>



// Class deoglRVideo
//////////////////////

// Constructor, destructor
////////////////////////////

deoglRVideo::deoglRVideo(deoglRenderThread &renderThread, int width, int height,
	int componentCount, int frameCount) :
pRenderThread(renderThread),

pWidth(width),
pHeight(height),
pComponentCount(componentCount),

pFrameCountToCache(-1),

pUpdateFrame(-1)
{
	if(frameCount > 0){
		int i;
		for(i=0; i<frameCount; i++){
			pFrames.Add({});
		}
		pFrameCountToCache = frameCount;
	}
}

deoglRVideo::~deoglRVideo(){
}



// Management
///////////////

deoglTexture *deoglRVideo::GetTexture(int frame) const{
	if(frame < 0 || frame >= pFrames.GetCount()){
		try{
			DETHROW(deeInvalidParam);
		}catch(const deException &e){
			e.PrintError();
		}
		DETHROW(deeInvalidParam);
	}
	return pFrames.GetAt(frame);
}

deoglPixelBuffer::Ref deoglRVideo::SetPixelBuffer(int frame, deoglPixelBuffer *pixelBuffer){
	DEASSERT_TRUE(frame >= 0)
	DEASSERT_TRUE(frame < pFrames.GetCount())
	DEASSERT_TRUE(pUpdateFrame == -1)
	
	const deoglPixelBuffer::Ref prevPixelBuffer(pPixelBuffer);
	
	pPixelBuffer = pixelBuffer;
	pUpdateFrame = frame;
	
	return prevPixelBuffer;
}

void deoglRVideo::UpdateTexture(){
	if(pUpdateFrame == -1){
		return;
	}
	
	if(!pFrames.GetAt(pUpdateFrame)){
		auto texture = deTUniqueReference<deoglTexture>::New(pRenderThread);
		texture->SetSize(pWidth, pHeight);
		texture->SetMapingFormat(pComponentCount, false, false);
		texture->SetMipMapped(false); // true would be nicer but doing it every frame is a waste
		texture->CreateTexture();
		pFrames.SetAt(pUpdateFrame, std::move(texture));
	}
	
	if(pPixelBuffer){
		pFrames.GetAt(pUpdateFrame)->SetPixels(pPixelBuffer);
	}
	
	//pRenderThread.GetLogger().LogInfoFormat( "Video: update texture frame=%i remaining=%i", pUpdateFrame, pFrameCountToCache );
	pUpdateFrame = -1;
	pFrameCountToCache--;
	
	//if( pFrameCountToCache == 0 ){
	//	pRenderThread.GetLogger().LogInfo( "Video: all frame textures ready" );
	//}
}



// Private Functions
//////////////////////
