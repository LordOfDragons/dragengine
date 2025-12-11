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

#include "deCanvasPaint.h"
#include "deCanvasVisitor.h"
#include "../../common/exceptions.h"



// Class deCanvasPaint
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deCanvasPaint::deCanvasPaint(deCanvasManager *manager) :
deCanvas(manager),
pShapeType(estRectangle),
pLineColor(0.0f, 0.0f, 0.0f, 1.0f),
pFillColor(0.0f, 0.0f, 0.0f, 0.0f),
pThickness(1.0f),
pRoundCornerX(0.0f),
pRoundCornerY(0.0f),
pStartAngle(0.0f),
pEndAngle(TWO_PI),

pPoints(NULL),
pPointCount(0){
}

deCanvasPaint::~deCanvasPaint(){
	SetPeerGraphic(nullptr);
	if(pPoints){
		delete [] pPoints;
	}
}



// Management
///////////////

void deCanvasPaint::SetShapeType(eShapeTypes shapeType){
	if(shapeType < estPoints || shapeType > estPolygon){
		DETHROW(deeInvalidParam);
	}
	
	if(shapeType == pShapeType){
		return;
	}
	
	pShapeType = shapeType;
	NotifyContentChanged();
}

void deCanvasPaint::SetLineColor(const decColor &color){
	if(pLineColor.IsEqualTo(color)){
		return;
	}
	
	pLineColor = color;
	NotifyContentChanged();
}

void deCanvasPaint::SetFillColor(const decColor &color){
	if(pFillColor.IsEqualTo(color)){
		return;
	}
	
	pFillColor = color;
	NotifyContentChanged();
}

void deCanvasPaint::SetThickness(float thickness){
	if(thickness < 0.0f){
		thickness = 0.0f;
	}
	
	if(fabsf(thickness - pThickness) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pThickness = thickness;
	NotifyContentChanged();
}

void deCanvasPaint::SetRoundCornerX(float roundCorner){
	roundCorner = decMath::clamp(roundCorner, 0.0f, 1.0f);
	
	if(fabsf(roundCorner - pRoundCornerX) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRoundCornerX = roundCorner;
	NotifyContentChanged();
}

void deCanvasPaint::SetRoundCornerY(float roundCorner){
	roundCorner = decMath::clamp(roundCorner, 0.0f, 1.0f);
	
	if(fabsf(roundCorner - pRoundCornerY) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRoundCornerY = roundCorner;
	NotifyContentChanged();
}

void deCanvasPaint::SetStartAngle(float startAngle){
	startAngle = decMath::normalize(startAngle, 0.0f, TWO_PI);
	
	if(fabsf(startAngle - pStartAngle) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pStartAngle = startAngle;
	NotifyContentChanged();
}

void deCanvasPaint::SetEndAngle(float endAngle){
	endAngle = decMath::normalize(endAngle, 0.0f, TWO_PI);
	
	if(fabsf(endAngle - pEndAngle) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pEndAngle = endAngle;
	NotifyContentChanged();
}



// Points
///////////

const decPoint &deCanvasPaint::GetPointAt(int position) const{
	if(position < 0 || position >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	return pPoints[position];
}

void deCanvasPaint::AddPoint(const decPoint &point){
	decPoint * const newArray = new decPoint[pPointCount + 1];
	int i;
	
	for(i=0; i<pPointCount; i++){
		newArray[i] = pPoints[i];
	}
	
	newArray[pPointCount] = point;
	
	if(pPoints){
		delete [] pPoints;
	}
	pPoints = newArray;
	pPointCount++;
}

void deCanvasPaint::RemovePointFrom(int position){
	if(position < 0 || position >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=position+1; i<pPointCount; i++){
		pPoints[i - 1] = pPoints[i];
	}
	pPointCount--;
}

void deCanvasPaint::RemoveAllPoints(){
	if(pPoints){
		delete [] pPoints;
		pPoints = NULL;
	}
	pPointCount = 0;
}



// Visiting
/////////////

void deCanvasPaint::Visit(deCanvasVisitor &visitor){
	visitor.VisitPaint(*this);
}
