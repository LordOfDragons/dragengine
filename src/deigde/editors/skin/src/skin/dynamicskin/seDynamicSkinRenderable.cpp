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

#include "seDynamicSkin.h"
#include "seDynamicSkinRenderable.h"
#include "../seSkin.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoManager.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideoPlayerManager.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableValue.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableImage.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableVideoFrame.h>



// Class seDynamicSkinRenderable
//////////////////////////////////

// Constructor, destructor
////////////////////////////

seDynamicSkinRenderable::seDynamicSkinRenderable(deEngine *engine, const char *name) :
pEngine(engine),
pDynamicSkin(NULL),
pName(name),
pRenderableType(ertValue),
pActive(false),
pDirty(true),
pValue(0.0f),
pSliderValueLower(0.0f),
pSliderValueUpper(1.0f),
pColor(1.0f, 1.0f, 1.0f)
{
	if(! engine){
		DETHROW(deeInvalidParam);
	}
	
	pEngVideoPlayer.TakeOver(engine->GetVideoPlayerManager()->CreateVideoPlayer());
	pEngVideoPlayer->SetLooping(true);
}

seDynamicSkinRenderable::~seDynamicSkinRenderable(){
	SetDynamicSkin(NULL);
}



// Management
///////////////

void seDynamicSkinRenderable::SetDynamicSkin(seDynamicSkin *dynamicSkin){
	if(dynamicSkin == pDynamicSkin){
		return;
	}
	
	if(pDynamicSkin){
		pDynamicSkin->GetEngineDynamicSkin()->RemoveRenderableNamed(pName);
	}
	
	pDynamicSkin = dynamicSkin;
	
	pDirty = true;
}



void seDynamicSkinRenderable::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	pDirty = true;
	
	if(pDynamicSkin){
		pDynamicSkin->GetParentSkin()->NotifyDynamicSkinRenderableNameChanged(this);
	}
	
	NotifyEngineDynamicSkinChanged();
}

void seDynamicSkinRenderable::SetRenderableType(eRenderableTypes type){
	if(type == pRenderableType){
		return;
	}
	
	pRenderableType = type;
	pDirty = true;
	
	NotifyChanged();
}



void seDynamicSkinRenderable::SetActive(bool active){
	pActive = active;
}

void seDynamicSkinRenderable::NotifyChanged(){
	if(pDynamicSkin){
		pDynamicSkin->GetParentSkin()->NotifyDynamicSkinRenderableChanged(this);
	}
	
	NotifyEngineDynamicSkinChanged();
}

void seDynamicSkinRenderable::NotifyEngineDynamicSkinChanged(){
	if(! pDynamicSkin){
		return;
	}
	
	deDynamicSkin &engDynamicSkin = *pDynamicSkin->GetEngineDynamicSkin();
	const int index = engDynamicSkin.IndexOfRenderableNamed(pName);
	
	if(index != -1){
		pDynamicSkin->GetEngineDynamicSkin()->NotifyRenderableChanged(index);
	}
}



void seDynamicSkinRenderable::Update(float elapsed){
	if(pEngVideoPlayer){
		pEngVideoPlayer->Update(elapsed);
	}
	
	if(! pDirty){
		return;
	}
	
	pDynamicSkin->GetEngineDynamicSkin()->RemoveRenderableNamed(pName);
	
	if(pRenderableType == ertValue){
		CreateRenderableValue();
		
	}else if(pRenderableType == ertColor){
		CreateRenderableColor();
		
	}else if(pRenderableType == ertImage){
		CreateRenderableImage();
		
	}else if(pRenderableType == ertCanvas){
		CreateRenderableCanvas();
		
	}else if(pRenderableType == ertVideoFrame){
		CreateRenderableVideoFrame();
		
	}else if(pRenderableType == ertCamera){
		CreateRenderableCamera();
	}
	
	pDirty = false;
}



void seDynamicSkinRenderable::CreateRenderableValue(){
	deDSRenderableValue *engRenderable = NULL;
	
	try{
		engRenderable = new deDSRenderableValue(pName);
		engRenderable->SetValue(pValue);
		pDynamicSkin->GetEngineDynamicSkin()->AddRenderable(engRenderable);
		
	}catch(const deException &){
		if(engRenderable){
			delete engRenderable;
		}
		throw;
	}
}

void seDynamicSkinRenderable::CreateRenderableColor(){
	deDSRenderableColor *engRenderable = NULL;
	
	try{
		engRenderable = new deDSRenderableColor(pName);
		engRenderable->SetColor(pColor);
		pDynamicSkin->GetEngineDynamicSkin()->AddRenderable(engRenderable);
		
	}catch(const deException &){
		if(engRenderable){
			delete engRenderable;
		}
		throw;
	}
}

void seDynamicSkinRenderable::CreateRenderableImage(){
	deDSRenderableImage *engRenderable = NULL;
	
	try{
		engRenderable = new deDSRenderableImage(pName);
		engRenderable->SetImage(pEngImage);
		pDynamicSkin->GetEngineDynamicSkin()->AddRenderable(engRenderable);
		
	}catch(const deException &){
		if(engRenderable){
			delete engRenderable;
		}
		throw;
	}
}

void seDynamicSkinRenderable::CreateRenderableCanvas(){
	// nothing yet
}

void seDynamicSkinRenderable::CreateRenderableVideoFrame(){
	deDSRenderableVideoFrame *engRenderable = NULL;
	
	try{
		engRenderable = new deDSRenderableVideoFrame(pName);
		engRenderable->SetVideoPlayer(pEngVideoPlayer);
		pDynamicSkin->GetEngineDynamicSkin()->AddRenderable(engRenderable);
		
	}catch(const deException &){
		if(engRenderable){
			delete engRenderable;
		}
		throw;
	}
}

void seDynamicSkinRenderable::CreateRenderableCamera(){
	// nothing yet
}



// Renderable Value
/////////////////////

void seDynamicSkinRenderable::SetValue(float value){
	if(fabsf(value - pValue) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pValue = value;
	pDirty = true;
		
	NotifyChanged();
}

void seDynamicSkinRenderable::SetSliderLowerRange(float lower){
	lower = decMath::max(lower, 0.0f);
	if(fabs(lower - pSliderValueLower) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSliderValueLower = lower;
	NotifyChanged();
}

void seDynamicSkinRenderable::SetSliderUpperRange(float upper){
	upper = decMath::max(upper, 0.0f);
	if(fabs(upper - pSliderValueUpper) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSliderValueUpper = upper;
	NotifyChanged();
}



// Renderable Color
/////////////////////

void seDynamicSkinRenderable::SetColor(const decColor &color){
	if(color.IsEqualTo(pColor)){
		return;
	}
	
	pColor = color;
	pDirty = true;
	
	NotifyChanged();
}



// Renderable Image
/////////////////////

void seDynamicSkinRenderable::SetImagePath(const char *imagePath){
	if(pPathImage.Equals(imagePath)){
		return;
	}
	
	pPathImage = imagePath;
	UpdateImage();
	pDirty = true;
	
	NotifyChanged();
}

void seDynamicSkinRenderable::UpdateImage(){
	deImage::Ref image;
	
	if(! pPathImage.IsEmpty()){
		try{
			image.TakeOver(pEngine->GetImageManager()->LoadImage(pPathImage, "/"));
			
		}catch(const deException &e){
			if(pDynamicSkin){
				pDynamicSkin->GetParentSkin()->GetLogger()->LogException("Skin Editor", e);
			}
		}
	}
	
	pEngImage = image;
}



// Renderable Canvas
//////////////////////



// Renderable Video Frame
///////////////////////////

void seDynamicSkinRenderable::SetVideoPath(const char *videoPath){
	if(pPathVideo.Equals(videoPath)){
		return;
	}
	
	pPathVideo = videoPath;
	UpdateVideo();
	pDirty = true;
	
	NotifyChanged();
}

void seDynamicSkinRenderable::UpdateVideo(){
	deVideo::Ref video;
	
	if(! pPathVideo.IsEmpty()){
		try{
			video.TakeOver(pEngine->GetVideoManager()->LoadVideo(pPathVideo, "/", false));
			
		}catch(const deException &e){
			if(pDynamicSkin){
				pDynamicSkin->GetParentSkin()->GetLogger()->LogException("Skin Editor", e);
			}
		}
	}
	
	pEngVideo = video;
	pEngVideoPlayer->SetVideo(video);
	
	if(video){
		pEngVideoPlayer->Play();
		
	}else{
		pEngVideoPlayer->Stop();
	}
}
