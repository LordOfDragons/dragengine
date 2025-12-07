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

#include "deoglCanvasPaint.h"
#include "render/deoglRCanvasPaint.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>



// Class deoglCanvasPaint
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasPaint::deoglCanvasPaint(deGraphicOpenGl &ogl, deCanvasPaint &canvas) :
deoglCanvas(ogl, canvas),
pCanvasPaint(canvas),
pRCanvasPaint(NULL){
}

deoglCanvasPaint::~deoglCanvasPaint(){
}



// Management
///////////////

void deoglCanvasPaint::DropRCanvas(){
	pRCanvasPaint = NULL;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasPaint::SyncContentToRender(){
	pRCanvasPaint->SetShapeType(pCanvasPaint.GetShapeType());
	pRCanvasPaint->SetLineColor(pCanvasPaint.GetLineColor());
	pRCanvasPaint->SetFillColor(pCanvasPaint.GetFillColor());
	pRCanvasPaint->SetThickness(pCanvasPaint.GetThickness());
	pRCanvasPaint->SetRoundCornerX(pCanvasPaint.GetRoundCornerX());
	pRCanvasPaint->SetRoundCornerY(pCanvasPaint.GetRoundCornerY());
	pRCanvasPaint->SetStartAngle(pCanvasPaint.GetStartAngle());
	pRCanvasPaint->SetEndAngle(pCanvasPaint.GetEndAngle());
	
	const int pointCount = pCanvasPaint.GetPointCount();
	pRCanvasPaint->SetPointCount(pointCount);
	if(pointCount > 0){
		decVector2 * const points = pRCanvasPaint->GetPoints();
		int i;
		
		for(i=0; i<pointCount; i++){
			points[i] = pCanvasPaint.GetPointAt(i);
		}
	}
}



// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasPaint::CreateRCanvas(){
	pRCanvasPaint = new deoglRCanvasPaint(GetOgl().GetRenderThread());
	return pRCanvasPaint;
}
