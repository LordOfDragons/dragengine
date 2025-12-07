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

#include "deoglCanvas.h"
#include "deoglCanvasView.h"
#include "render/deoglRCanvas.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class deoglCanvas
//////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvas::deoglCanvas(deGraphicOpenGl &ogl, deCanvas &canvas) :
pOgl(ogl),
pCanvas(canvas),

pRCanvas(NULL),
pDirtyGeometry(true),
pDirtyTransform(true),
pDirtyColorTransform(true),
pDirtyParameters(true),
pDirtyContent(true),
pDirtyParentPaint(false),
pDirtyMaskContent(false)
{
}

deoglCanvas::~deoglCanvas(){
	if(pRCanvas){
		pRCanvas->FreeReference();
	}
}



// Management
///////////////

void deoglCanvas::DropRCanvas(){
	if(pRCanvas){
		pRCanvas->FreeReference();
		pRCanvas = NULL;
	}
}

void deoglCanvas::SetDirtyParentPaint(){
	if(pDirtyParentPaint){
		return;
	}
	
	pDirtyParentPaint = true;
	
	if(pCanvas.GetParentMask()){
		deoglCanvas * const canvas = (deoglCanvas*)pCanvas.GetParentMask()->GetPeerGraphic();
		if(canvas){
			canvas->SetDirtyMaskContent();
		}
	}
	if(pCanvas.GetParentView()){
		deoglCanvasView * const canvas = (deoglCanvasView*)pCanvas.GetParentView()->GetPeerGraphic();
		if(canvas){
			canvas->SetDirtyPaint();
		}
	}
}

void deoglCanvas::SetDirtyMaskContent(){
	pDirtyMaskContent = true;
	SetDirtyParentPaint();
	
	if(pRCanvas){
		pRCanvas->DirtyMaskRenderTarget();
	}
}



void deoglCanvas::SyncToRender(){
	if(! pRCanvas){
		pRCanvas = CreateRCanvas();
		if(! pRCanvas){
			DETHROW(deeInvalidParam);
		}
		
		pDirtyTransform = true;
		pDirtyColorTransform = true;
		pDirtyParameters = true;
		pDirtyContent = true;
	}
	
	if(pDirtyGeometry){
		pDirtyTransform = true; // position is calculated into the transform
		
		pRCanvas->SetPosition(decVector2(pCanvas.GetPosition()));
		pRCanvas->SetSize(decVector2(pCanvas.GetSize()));
		pDirtyGeometry = false;
	}
	
	if(pDirtyTransform){
		pDirtyTransform = false;
		
		pRCanvas->SetTransform(pCanvas.GetTransform()
			* decTexMatrix2::CreateTranslation( decVector2( pCanvas.GetPosition() ) ) );
	}
	
	if(pDirtyColorTransform){
		pDirtyColorTransform = false;
		
		pRCanvas->SetColorTransform(pCanvas.GetColorTransform());
	}
	
	if(pDirtyParameters){
		pDirtyParameters = false;
		
		pRCanvas->SetOrder(pCanvas.GetOrder());
		pRCanvas->SetTransparency(pCanvas.GetTransparency());
		pRCanvas->SetVisible(pCanvas.GetVisible());
		pRCanvas->SetBlendMode(pCanvas.GetBlendMode());
		
		if(pCanvas.GetMask()){
			deoglCanvas &canvas = *((deoglCanvas*)pCanvas.GetMask()->GetPeerGraphic());
			canvas.SyncToRender();
			pRCanvas->SetMask(canvas.pRCanvas);
			
		}else{
			pRCanvas->SetMask(nullptr);
		}
	}
	
	if(pDirtyContent){
		pDirtyContent = false;
		SyncContentToRender();
	}
	
	pDirtyParentPaint = false;
}

void deoglCanvas::SyncContentToRender(){
}



// Notifications
//////////////////

void deoglCanvas::PositionChanged(){
	pDirtyGeometry = true;
	SetDirtyParentPaint();
}

void deoglCanvas::SizeChanged(){
	pDirtyGeometry = true;
	SetDirtyParentPaint();
}

void deoglCanvas::TransformChanged(){
	pDirtyTransform = true;
	SetDirtyParentPaint();
}

void deoglCanvas::ColorTransformChanged(){
	pDirtyColorTransform = true;
	SetDirtyParentPaint();
}

void deoglCanvas::VisibleChanged(){
	pDirtyParameters = true;
	SetDirtyParentPaint();
}

void deoglCanvas::OrderChanged(){
	deCanvasView * const parentView = pCanvas.GetParentView();
	if(parentView){
		((deoglCanvasView*)parentView->GetPeerGraphic())->ChildOrderChanged();
	}
	
	pDirtyParameters = true;
	SetDirtyParentPaint();
}

void deoglCanvas::TransparencyChanged(){
	pDirtyParameters = true;
	SetDirtyParentPaint();
}

void deoglCanvas::BlendModeChanged(){
	pDirtyParameters = true;
	SetDirtyParentPaint();
}

void deoglCanvas::MaskChanged(){
	pDirtyParameters = true;
	pDirtyContent = true;
	SetDirtyParentPaint();
}

void deoglCanvas::ContentChanged(){
	pDirtyContent = true;
	SetDirtyParentPaint();
}
