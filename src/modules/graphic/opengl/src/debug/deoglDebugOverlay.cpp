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
#include <string.h>
#include <stdlib.h>

#include "deoglDebugOverlay.h"
#include "../deGraphicOpenGl.h"
#include "../canvas/deoglCanvasView.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTContext.h"
#include "../world/deoglRCamera.h"
#include "../vr/deoglVR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deGraphicSystem.h>



// Class deoglDebugOverlay
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugOverlay::deoglDebugOverlay(deGraphicOpenGl &ogl) :
pOgl(ogl){
}

deoglDebugOverlay::~deoglDebugOverlay(){
}



// Management
///////////////

void deoglDebugOverlay::PrepareOverlay(deCanvasView &canvasView){
	deoglRenderThread &renderThread = pOgl.GetRenderThread();
	
	if(!canvasView.GetRootCanvas()){
		renderThread.SetCanvasDebugOverlay(nullptr);
		return;
	}
	
	decPoint size;
	
	if(renderThread.GetVRCamera()){
		const deoglVR * const vr = renderThread.GetVRCamera()->GetVR();
		if(vr){
			canvasView.SetSize(vr->GetDebugPanelSize());
			
		}else{
			renderThread.SetCanvasDebugOverlay(nullptr);
			return;
		}
		
	}else{
		const deRenderWindow * const renderWindow = pOgl.GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
		if(renderWindow){
			canvasView.SetSize(decPoint(renderWindow->GetWidth(), renderWindow->GetHeight()));
			
		}else{
			renderThread.SetCanvasDebugOverlay(nullptr);
			return;
		}
	}
	
	pSortViews(canvasView);
	pAlignViews();
	
	deoglCanvasView &oglCanvas = *((deoglCanvasView*)canvasView.GetPeerGraphic());
	oglCanvas.SyncToRender();
	
	renderThread.SetCanvasDebugOverlay(oglCanvas.GetRCanvasView());
}



// Private Functions
//////////////////////

void deoglDebugOverlay::pSortViews(deCanvasView &canvasView){
	if(canvasView.GetCanvasCount() == 0){
		pList1.RemoveAll();
		pList2.RemoveAll();
		return;
	}
	
	pList1.RemoveAll();
	
	deCanvas *childCanvas = canvasView.GetRootCanvas();
	while(childCanvas){
		pList1.Add(childCanvas);
		childCanvas = childCanvas->GetLLViewNext();
	}
	
	pList2.RemoveAll();
	float bestOrder;
	int i;
	
	while(pList1.GetCount() > 0){
		const int count = pList1.GetCount();
		int bestIndex = -1;
		
		for(i=0; i<count; i++){
			const float order = ((deCanvas*)pList1.GetAt(i))->GetOrder();
			if(bestIndex == -1 || order < bestOrder){
				bestOrder = order;
				bestIndex = i;
			}
		}
		
		pList2.Add(pList1.GetAt(bestIndex));
		pList1.RemoveFrom(bestIndex);
	}
}

void deoglDebugOverlay::pAlignViews(){
	// for the time being just take each canvas and align it from left to right using
	// the order value to sort them
	decPoint position(5, 5);
	
	const int count = pList2.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deCanvas &canvas = *((deCanvas*)pList2.GetAt(i));
		canvas.SetPosition(position);
		position.x += canvas.GetSize().x + 5;
	}
}
