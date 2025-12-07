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
#include "deCanvas.h"
#include "deCanvasImage.h"
#include "deCanvasPaint.h"
#include "deCanvasRenderWorld.h"
#include "deCanvasText.h"
#include "deCanvasVideoPlayer.h"
#include "deCanvasView.h"
#include "deCanvasManager.h"
#include "deCanvasCanvasView.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deCanvasManager
//////////////////////////

// Constructor, Destructor
////////////////////////////

deCanvasManager::deCanvasManager(deEngine *engine) : deResourceManager(engine, ertCanvas){
	SetLoggingName("canvas");
}

deCanvasManager::~deCanvasManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deCanvasManager::GetCanvasCount() const{
	return pCanvas.GetCount();
}

deCanvas *deCanvasManager::GetRootCanvas() const{
	return (deCanvas*)pCanvas.GetRoot();
}

deCanvasImage *deCanvasManager::CreateCanvasImage(){
	deCanvasImage *canvas = NULL;
	
	try{
		canvas = new deCanvasImage(this);
		GetGraphicSystem()->LoadCanvas(canvas);
		pCanvas.Add(canvas);
		
	}catch(const deException &){
		if(canvas){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasPaint *deCanvasManager::CreateCanvasPaint(){
	deCanvasPaint *canvas = NULL;
	
	try{
		canvas = new deCanvasPaint(this);
		GetGraphicSystem()->LoadCanvas(canvas);
		pCanvas.Add(canvas);
		
	}catch(const deException &){
		if(canvas){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasCanvasView *deCanvasManager::CreateCanvasCanvasView(){
	deCanvasCanvasView *canvas = NULL;
	
	try{
		canvas = new deCanvasCanvasView(this);
		GetGraphicSystem()->LoadCanvas(canvas);
		pCanvas.Add(canvas);
		
	}catch(const deException &){
		if(canvas){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasRenderWorld *deCanvasManager::CreateCanvasRenderWorld(){
	deCanvasRenderWorld *canvas = NULL;
	
	try{
		canvas = new deCanvasRenderWorld(this);
		GetGraphicSystem()->LoadCanvas(canvas);
		pCanvas.Add(canvas);
		
	}catch(const deException &){
		if(canvas){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasText *deCanvasManager::CreateCanvasText(){
	deCanvasText *canvas = NULL;
	
	try{
		canvas = new deCanvasText(this);
		GetGraphicSystem()->LoadCanvas(canvas);
		pCanvas.Add(canvas);
		
	}catch(const deException &){
		if(canvas){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasVideoPlayer *deCanvasManager::CreateCanvasVideoPlayer(){
	deCanvasVideoPlayer *canvas = NULL;
	
	try{
		canvas = new deCanvasVideoPlayer(this);
		GetGraphicSystem()->LoadCanvas(canvas);
		pCanvas.Add(canvas);
		
	}catch(const deException &){
		if(canvas){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasView *deCanvasManager::CreateCanvasView(){
	deCanvasView *canvas = NULL;
	
	try{
		canvas = new deCanvasView(this);
		GetGraphicSystem()->LoadCanvas(canvas);
		pCanvas.Add(canvas);
		
	}catch(const deException &){
		if(canvas){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}



void deCanvasManager::ReleaseLeakingResources(){
	if(GetCanvasCount() > 0){
		LogWarnFormat("%i leaking canvas", GetCanvasCount());
		pCanvas.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deCanvasManager::SystemGraphicLoad(){
	deCanvas *canvas = (deCanvas*)pCanvas.GetRoot();
	
	while(canvas){
		if(! canvas->GetPeerGraphic()){
			GetGraphicSystem()->LoadCanvas(canvas);
		}
		
		canvas = (deCanvas*)canvas->GetLLManagerNext();
	}
}

void deCanvasManager::SystemGraphicUnload(){
	deCanvas *canvas = (deCanvas*)pCanvas.GetRoot();
	
	while(canvas){
		canvas->SetPeerGraphic(NULL);
		canvas = (deCanvas*)canvas->GetLLManagerNext();
	}
}

void deCanvasManager::RemoveResource(deResource *resource){
	pCanvas.RemoveIfPresent(resource);
}
