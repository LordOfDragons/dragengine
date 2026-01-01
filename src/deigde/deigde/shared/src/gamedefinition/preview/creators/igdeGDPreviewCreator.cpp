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

#include "igdeGDPreviewCreator.h"
#include "../igdeGDPreviewListener.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../engine/igdeEngineController.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvas.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvasManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class igdeGDPreviewCreator
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPreviewCreator::igdeGDPreviewCreator(igdeEnvironment &environment, const decPoint &size) :
pEnvironment(environment),
pState(esInitial),
pEnableDebug(false)
{
	if(!(size > decPoint(1, 1))){
		DETHROW(deeInvalidParam);
	}
	
	deEngine &engine = *environment.GetEngineController()->GetEngine();
	
	pCanvas = engine.GetCanvasManager()->CreateCanvasView();
	pCanvas->SetSize(size);
	
	pImage = engine.GetImageManager()->CreateImage(size.x, size.y, 1, 3, 8);
	
	pCaptureCanvas = engine.GetCaptureCanvasManager()->CreateCaptureCanvas();
	pCaptureCanvas->SetCanvasView(pCanvas);
	pCaptureCanvas->SetImage(pImage);
}

igdeGDPreviewCreator::~igdeGDPreviewCreator(){
}



// Management
///////////////

void igdeGDPreviewCreator::SetImage(deImage *image){
	pImage = image;
	pCaptureCanvas->SetImage(image);
}

void igdeGDPreviewCreator::AddListener(igdeGDPreviewListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}



void igdeGDPreviewCreator::Update(float elapsed){
	pUpdate();
	
	if(pState == esFrameUpdate){
		DebugLog("update canvas for render");
		UpdateCanvasForRender(elapsed);
		pState = esBeginCapture;
		
		pUpdate();
	}
}

void igdeGDPreviewCreator::BeginCreation(){
	if(pState == esInitial){
		DebugLog("prepare canvas");
		pState = esPrepareCanvas;
		pUpdate();
	}
}

bool igdeGDPreviewCreator::FinishCreation(){
	pUpdate();
	
	if(pState != esCaptureFinished){
		return false;
	}
	
	DebugLog("finish creation");
	NotifyImageCreated(pImage);
	pListeners.RemoveAll();
	return true;
}

void igdeGDPreviewCreator::AbortCreation(){
	DebugLog("abort creation");
	pState = esCaptureFinished;
	pCaptureCanvas->SetCapture(false);
	NotifyImageCreated(nullptr);
}



void igdeGDPreviewCreator::NotifyImageCreated(deImage *image){
	pListeners.Visit([&](igdeGDPreviewListener &listener){
		listener.ImageCreated(image);
	});
}

void igdeGDPreviewCreator::DebugLog(const char *message){
	if(!pEnableDebug){
		return;
	}
	
	/*
	static const char * const stateNames[] = {
		"initial", "prepareCanvas", "waitCanvasReady", "frameUpdate",
		"beginCapture", "waitCaptureFinished", "captureFinished"
	};
	*/
	
	const decString prefix = DebugPrefix();
	//pEnvironment.GetLogger()->LogInfoFormat( "%s [%s] %s", prefix.GetString(), stateNames[ pState ], message );
	pEnvironment.GetLogger()->LogInfoFormat("IGDE", "%s %s", prefix.GetString(), message);
}



// Private Functions
//////////////////////

void igdeGDPreviewCreator::pUpdate(){
	if(pState == esInitial){
		return;
	}
	
	if(pState == esPrepareCanvas){
		DebugLog("prepare canvas for render");
		PrepareCanvasForRender();
		pState = esWaitCanvasReady;
	}
	
	if(pState == esWaitCanvasReady){
		if(!IsCanvasReadyForRender()){
			return;
		}
		DebugLog("canvas ready for render");
		pState = esBeginCapture;
	}
	
	if(pState == esBeginCapture){
		DebugLog("begin capture");
		pCaptureCanvas->SetCapture(true);
		pState = esWaitCaptureFinished;
	}
	
	if(pState == esWaitCaptureFinished){
		if(pCaptureCanvas->GetCapture()){
			return;
		}
		DebugLog("capture finished");
		pState = esCaptureFinished;
	}
}
