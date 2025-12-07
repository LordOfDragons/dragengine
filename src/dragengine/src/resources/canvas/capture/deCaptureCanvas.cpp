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
#include "../deCanvasView.h"
#include "../../image/deImage.h"
#include "../../../common/exceptions.h"
#include "../../../systems/modules/graphic/deBaseGraphicCaptureCanvas.h"



// Class deCaptureCanvas
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deCaptureCanvas::deCaptureCanvas(deCaptureCanvasManager *manager) :
deResource(manager),

pCapture(false),

pPeerGraphic(NULL){
}

deCaptureCanvas::~deCaptureCanvas(){
	SetPeerGraphic(NULL);
}



// Management
///////////////

void deCaptureCanvas::SetCanvasView(deCanvasView *canvasView){
	if(canvasView == pCanvasView){
		return;
	}
	
	pCanvasView = canvasView;
	
	if(pPeerGraphic){
		pPeerGraphic->CanvasViewChanged();
	}
}

void deCaptureCanvas::SetImage(deImage *image){
	if(image == pImage){
		return;
	}
	
	pImage = image;
	
	if(pPeerGraphic){
		pPeerGraphic->ImageChanged();
	}
}

void deCaptureCanvas::SetCapture(bool capture){
	if(capture == pCapture){
		return;
	}
	
	pCapture = capture;
	
	if(pPeerGraphic){
		pPeerGraphic->CaptureChanged();
	}
}



// System Peers
/////////////////

void deCaptureCanvas::SetPeerGraphic(deBaseGraphicCaptureCanvas *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}
