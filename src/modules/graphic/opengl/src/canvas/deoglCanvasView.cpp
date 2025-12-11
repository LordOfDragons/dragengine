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
#include "deoglCanvasViewListener.h"
#include "deoglCanvasCanvasView.h"
#include "render/deoglRCanvasView.h"
#include "../deGraphicOpenGl.h"
#include "../skin/dynamic/renderables/deoglDSRenderableCanvas.h"
#include "../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class deoglCanvasView
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasView::deoglCanvasView(deGraphicOpenGl &ogl, deCanvasView &canvas) :
deoglCanvas(ogl, canvas),
pCanvasView(canvas),
pDirtyPaint(false),
pResizeRenderTarget(false),
pSyncRequestSend(false){
}

deoglCanvasView::~deoglCanvasView(){
	// notify owners we are about to be deleted. required since owners hold only a weak pointer
	// to the canvas view and are notified only after switching to a new canvas view. in this
	// case they can not use the old pointer to remove themselves from the canvas view
	NotifyDestroyed();
}



// Management
///////////////

void deoglCanvasView::DropRCanvas(){
	pRCanvasView = nullptr;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasView::ChildOrderChanged(){
	ContentChanged(); // cause rebuild of child rcanvas
}

void deoglCanvasView::SetDirtyPaint(){
	if(pDirtyPaint){
		return;
	}
	
	pDirtyPaint = true;
	SetDirtyParentPaint(); // goes up to parent
	pRequiresSync();
}

void deoglCanvasView::SyncToRender(){
	// prepare children first since deoglCanvas::PrepareForRender() calls
	// PrepareContentForRender() which would result in NULL RCanvas to be added
	deCanvas *child = pCanvasView.GetRootCanvas();
	
	while(child){
		((deoglCanvas*)child->GetPeerGraphic())->SyncToRender();
		child = child->GetLLViewNext();
	}
	
	// if paint dirty update the paint tracker in the render counterpart
	if(pDirtyPaint){
		pDirtyPaint = false;
		
		if(pRCanvasView){
			pRCanvasView->IncrementPaintTracker();
		}
	}
	
	// resize render target if existing and required
	if(pResizeRenderTarget){
		pResizeRenderTarget = false;
		
		if(pRCanvasView){
			pRCanvasView->SetResizeRenderTarget();
		}
	}
	
	// now it is safe to process the rest of the update
	pSyncRequestSend = false;
	deoglCanvas::SyncToRender();
}

void deoglCanvasView::SyncContentToRender(){
	deCanvas *child = pCanvasView.GetRootCanvas();
	
	pRCanvasView->RemoveAllChildren();
	
	while(child){
		pRCanvasView->AddChild(((deoglCanvas*)child->GetPeerGraphic())->GetRCanvas());
		child = child->GetLLViewNext();
	}
}



// Listeners
//////////////

void deoglCanvasView::AddListener(deoglCanvasViewListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void deoglCanvasView::RemoveListener(deoglCanvasViewListener *listener){
	pListeners.RemoveIfPresent(listener);
}

void deoglCanvasView::NotifyDestroyed(){
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglCanvasViewListener*)pListeners.GetAt(i))->CanvasViewDestroyed();
	}
}

void deoglCanvasView::NotifyRequiresSync(){
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglCanvasViewListener*)pListeners.GetAt(i))->CanvasViewRequiresSync();
	}
}



// Notifications
//////////////////

void deoglCanvasView::PositionChanged(){
	deoglCanvas::PositionChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::SizeChanged(){
	deoglCanvas::SizeChanged();
	pResizeRenderTarget = true;
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::TransformChanged(){
	deoglCanvas::TransformChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::ColorTransformChanged(){
	deoglCanvas::ColorTransformChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::VisibleChanged(){
	deoglCanvas::VisibleChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::OrderChanged(){
	deoglCanvas::OrderChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::TransparencyChanged(){
	deoglCanvas::TransparencyChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::BlendModeChanged(){
	deoglCanvas::BlendModeChanged();
	pDirtyPaint = true;
	pRequiresSync();
}

void deoglCanvasView::ContentChanged(){
	deoglCanvas::ContentChanged();
	pDirtyPaint = true;
	pRequiresSync();
}


// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasView::CreateRCanvas(){
	pRCanvasView.TakeOverWith(GetOgl().GetRenderThread());
	return pRCanvasView;
}



// Private Functions
//////////////////////

void deoglCanvasView::pRequiresSync(){
	if(pSyncRequestSend){
		return;
	}
	
	pSyncRequestSend = true;
	NotifyRequiresSync();
}
