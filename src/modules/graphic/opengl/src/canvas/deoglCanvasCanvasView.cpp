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

#include "deoglCanvasView.h"
#include "deoglCanvasCanvasView.h"
#include "render/deoglRCanvasCanvasView.h"
#include "render/deoglRCanvasView.h"
#include "../deGraphicOpenGl.h"
#include "../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasCanvasView.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class deoglCanvasRenderTarget
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasCanvasView::deoglCanvasCanvasView(deGraphicOpenGl &ogl, deCanvasCanvasView &canvas) :
deoglCanvas(ogl, canvas),
pCanvasCanvasView(canvas),
pCanvasView(NULL){
}

deoglCanvasCanvasView::~deoglCanvasCanvasView(){
	if(pCanvasView){
		pCanvasView->RemoveListener(this);
	}
}



// Management
///////////////

void deoglCanvasCanvasView::DropRCanvas(){
	pRCanvasCanvasView = nullptr;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasCanvasView::SyncContentToRender(){
	if(pCanvasView){
		pCanvasView->SyncToRender();
		pRCanvasCanvasView->SetCanvasView(pCanvasView->GetRCanvasView());
		
	}else{
		pRCanvasCanvasView->SetCanvasView(NULL);
	}
	
	const float repeatScaleU = (float)pCanvasCanvasView.GetRepeatX();
	const float repeatScaleV = (float)pCanvasCanvasView.GetRepeatY();
	pRCanvasCanvasView->SetTCTransform(decTexMatrix2::CreateScale(repeatScaleU, repeatScaleV));
	pRCanvasCanvasView->SetTCClampMaximum(decVector2(repeatScaleU, repeatScaleV));
}

void deoglCanvasCanvasView::CanvasViewDestroyed(){
	pCanvasView = NULL;
}

void deoglCanvasCanvasView::CanvasViewRequiresSync(){
	deoglCanvas::ContentChanged();
}



// Notifications
//////////////////

void deoglCanvasCanvasView::ContentChanged(){
	if(pCanvasView){
		pCanvasView->RemoveListener(this);
	}
	
	if(pCanvasCanvasView.GetCanvasView()){
		pCanvasView = (deoglCanvasView*)pCanvasCanvasView.GetCanvasView()->GetPeerGraphic();
		pCanvasView->AddListener(this);
		
	}else{
		pCanvasView = NULL;
	}
	
	deoglCanvas::ContentChanged();
}



// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasCanvasView::CreateRCanvas(){
	pRCanvasCanvasView = deoglRCanvasCanvasView::Ref::New(GetOgl().GetRenderThread());
	return pRCanvasCanvasView;
}
