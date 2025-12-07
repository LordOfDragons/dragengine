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

#include "deDSRenderableCanvas.h"
#include "deDSRenderableVisitor.h"
#include "../../../canvas/deCanvasView.h"
#include "../../../../common/exceptions.h"



// Class deDSRenderableCanvas
///////////////////////////////

// Constructor, destructor
////////////////////////////

deDSRenderableCanvas::deDSRenderableCanvas(const char *name) :
deDSRenderable(name),
pComponentCount(3),
pBitCount(8){
}

deDSRenderableCanvas::~deDSRenderableCanvas(){
}



// Management
///////////////

void deDSRenderableCanvas::SetComponentCount(int componentCount){
	if(componentCount < 1 || componentCount > 4){
		DETHROW(deeInvalidParam);
	}
	pComponentCount = componentCount;
}

void deDSRenderableCanvas::SetBitCount(int bitCount){
	if(bitCount != 8 && bitCount != 16 && bitCount != 32){
		DETHROW(deeInvalidParam);
	}
	pBitCount = bitCount;
}

void deDSRenderableCanvas::SetCanvas(deCanvasView *canvas){
	pCanvas = canvas;
}



// Visiting
/////////////

void deDSRenderableCanvas::Visit(deDSRenderableVisitor &visitor){
	visitor.VisitCanvas(*this);
}
