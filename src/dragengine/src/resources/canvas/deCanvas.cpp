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
#include "deCanvasManager.h"
#include "deCanvasVisitor.h"
#include "../../systems/modules/graphic/deBaseGraphicCanvas.h"
#include "../../common/exceptions.h"



// Class deCanvas
///////////////////

// Constructors and Destructors
/////////////////////////////////

deCanvas::deCanvas(deCanvasManager *manager) :
deResource(manager),

pSize(20, 20),
pVisible(true),
pOrder(0.0f),
pTransparency(1.0f),
pBlendMode(ebmBlend),

pPeerGraphic(NULL),

pParentMask(NULL),
pParentView(NULL),
pLLViewPrev(NULL),
pLLViewNext(NULL){
}

deCanvas::~deCanvas(){
	SetPeerGraphic(nullptr);
	SetMask(nullptr);
}



// Management
///////////////

void deCanvas::SetPosition(const decPoint &position){
	if(position == pPosition){
		return;
	}
	
	pPosition = position;
	if(pPeerGraphic){
		pPeerGraphic->PositionChanged();
	}
}

void deCanvas::SetSize(const decPoint &size){
	if(size == pSize){
		return;
	}
	
	pSize = size;
	if(pPeerGraphic){
		pPeerGraphic->SizeChanged();
	}
}

void deCanvas::SetTransform(const decTexMatrix2 &transform){
	if(transform.IsEqualTo(pTransform)){
		return;
	}
	
	pTransform = transform;
	if(pPeerGraphic){
		pPeerGraphic->TransformChanged();
	}
}

void deCanvas::SetColorTransform(const decColorMatrix &transform){
	if(transform.IsEqualTo(pColorTransform)){
		return;
	}
	
	pColorTransform = transform;
	if(pPeerGraphic){
		pPeerGraphic->ColorTransformChanged();
	}
}

void deCanvas::SetVisible(bool visible){
	if(visible == pVisible){
		return;
	}
	
	pVisible = visible;
	if(pPeerGraphic){
		pPeerGraphic->VisibleChanged();
	}
}

void deCanvas::SetOrder(float order){
	if(fabsf(order - pOrder) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pOrder = order;
	if(pPeerGraphic){
		pPeerGraphic->OrderChanged();
	}
}

void deCanvas::SetTransparency(float transparency){
	if(fabsf(transparency - pTransparency) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pTransparency = transparency;
	if(pPeerGraphic){
		pPeerGraphic->TransparencyChanged();
	}
}

void deCanvas::SetBlendMode(eBlendModes blendMode){
	if(blendMode < ebmBlend || blendMode > ebmAdd){
		DETHROW(deeInvalidParam);
	}
	
	if(blendMode == pBlendMode){
		return;
	}
	
	pBlendMode = blendMode;
	if(pPeerGraphic){
		pPeerGraphic->BlendModeChanged();
	}
}

void deCanvas::SetMask(deCanvas *mask){
	if(pMask == mask){
		return;
	}
	
	if(mask && (mask->GetParentMask() || mask->GetParentView())){
		DETHROW(deeInvalidParam);
	}
	
	if(pMask){
		pMask->pParentMask = NULL;
	}
	
	pMask = mask;
	
	if(mask){
		mask->pParentMask = this;
	}
	
	if(pPeerGraphic){
		pPeerGraphic->MaskChanged();
	}
}



void deCanvas::NotifyContentChanged(){
	if(pPeerGraphic){
		pPeerGraphic->ContentChanged();
	}
}



// System Peers
/////////////////

void deCanvas::SetPeerGraphic(deBaseGraphicCanvas *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}



// Visiting
/////////////

void deCanvas::Visit(deCanvasVisitor &visitor){
	visitor.VisitCanvas(*this);
}



// Linked List
////////////////

void deCanvas::SetParentMask(deCanvas *canvas){
	pParentMask = canvas;
}

void deCanvas::SetParentView(deCanvasView *view){
	pParentView = view;
}

void deCanvas::SetLLViewPrev(deCanvas *canvas){
	pLLViewPrev = canvas;
}

void deCanvas::SetLLViewNext(deCanvas *canvas){
	pLLViewNext = canvas;
}
