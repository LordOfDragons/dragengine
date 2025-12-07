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

#include "deoglDSRenderableCanvas.h"
#include "render/deoglRDSRenderableCanvas.h"
#include "../deoglDynamicSkin.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../canvas/deoglCanvasView.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableCanvas.h>



// Class deoglDSRenderableCanvas
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableCanvas::deoglDSRenderableCanvas(
deoglDynamicSkin &dynamicSkin, const deDSRenderableCanvas &renderable) :
deoglDSRenderable(dynamicSkin, renderable),
pRenderableCanvas(renderable),
pRRenderableCanvas(NULL),
pCanvasView(NULL),
pDirty(true)
{
	try{
		pRRenderableCanvas = new deoglRDSRenderableCanvas(*dynamicSkin.GetRDynamicSkin(), renderable);
		
		if(renderable.GetCanvas()){
			pCanvasView = (deoglCanvasView*)renderable.GetCanvas()->GetPeerGraphic();
			pCanvasView->AddListener(this);
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableCanvas::~deoglDSRenderableCanvas(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableCanvas::GetRRenderable() const{
	return pRRenderableCanvas;
}

void deoglDSRenderableCanvas::RenderableChanged(){
	deoglCanvasView * const canvasView = pRenderableCanvas.GetCanvas()
		? (deoglCanvasView*)pRenderableCanvas.GetCanvas()->GetPeerGraphic() : NULL;
	
	if(canvasView != pCanvasView){
		if(pCanvasView){
			pCanvasView->RemoveListener(this);
		}
		
		pCanvasView = canvasView;
		
		if(canvasView){
			canvasView->AddListener(this);
		}
		
		pDirty = true;
		
		pDynamicSkin.NotifyRenderableChanged(*this);
	}
	
	if(pRenderableCanvas.GetName() != pRRenderableCanvas->GetName()){
		pDynamicSkin.NotifyRenderablesChanged();
	}
}

void deoglDSRenderableCanvas::SyncToRender(){
	if(pCanvasView){
		pCanvasView->SyncToRender();
	}
	
	if(pDirty){
		pRRenderableCanvas->SetName(pRenderableCanvas.GetName());
		pRRenderableCanvas->SetComponentCount(pRenderableCanvas.GetComponentCount());
		pRRenderableCanvas->SetBitCount(pRenderableCanvas.GetBitCount());
		
		if(pCanvasView){
			pRRenderableCanvas->SetCanvas(pCanvasView->GetRCanvasView());
			
		}else{
			pRRenderableCanvas->SetCanvas(NULL);
		}
		
		pDirty = false;
	}
}

void deoglDSRenderableCanvas::CanvasViewDestroyed(){
	pCanvasView = NULL;
}

void deoglDSRenderableCanvas::CanvasViewRequiresSync(){
	pDynamicSkin.NotifyRenderableRequiresSync(*this);
}



// Private Functions
//////////////////////

void deoglDSRenderableCanvas::pCleanUp(){
	if(pRRenderableCanvas){
		pRRenderableCanvas->FreeReference();
	}
	
	if(pCanvasView){
		pCanvasView->RemoveListener(this);
	}
}
