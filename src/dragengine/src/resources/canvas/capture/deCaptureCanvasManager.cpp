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
#include "deCaptureCanvas.h"
#include "deCaptureCanvasManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../systems/deGraphicSystem.h"


// Class deCaptureCanvasManager
/////////////////////////////////

// Constructor, Destructor
////////////////////////////

deCaptureCanvasManager::deCaptureCanvasManager(deEngine *engine) : deResourceManager(engine, ertCaptureCanvas){
	SetLoggingName("capture canvas");
}

deCaptureCanvasManager::~deCaptureCanvasManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deCaptureCanvasManager::GetCaptureCanvasCount() const{
	return pCaptureCanvas.GetCount();
}

deCaptureCanvas *deCaptureCanvasManager::GetRootCaptureCanvas() const{
	return (deCaptureCanvas*)pCaptureCanvas.GetRoot();
}

deCaptureCanvas::Ref deCaptureCanvasManager::CreateCaptureCanvas(){
	const deCaptureCanvas::Ref captureCanvas(deCaptureCanvas::Ref::New(this));
	GetGraphicSystem()->LoadCaptureCanvas(captureCanvas);
	pCaptureCanvas.Add(captureCanvas);
	return captureCanvas;
}



void deCaptureCanvasManager::ReleaseLeakingResources(){
	if(GetCaptureCanvasCount() > 0){
		LogWarnFormat("%i leaking capture canvas", GetCaptureCanvasCount());
		pCaptureCanvas.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deCaptureCanvasManager::SystemGraphicLoad(){
	deCaptureCanvas *captureCanvas = (deCaptureCanvas*)pCaptureCanvas.GetRoot();
	
	while(captureCanvas){
		if(!captureCanvas->GetPeerGraphic()){
			GetGraphicSystem()->LoadCaptureCanvas(captureCanvas);
		}
		
		captureCanvas = (deCaptureCanvas*)captureCanvas->GetLLManagerNext();
	}
}

void deCaptureCanvasManager::SystemGraphicUnload(){
	deCaptureCanvas *captureCanvas = (deCaptureCanvas*)pCaptureCanvas.GetRoot();
	
	while(captureCanvas){
		captureCanvas->SetPeerGraphic(NULL);
		captureCanvas = (deCaptureCanvas*)captureCanvas->GetLLManagerNext();
	}
}

void deCaptureCanvasManager::RemoveResource(deResource *resource){
	pCaptureCanvas.RemoveIfPresent(resource);
}
