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

#include "deSkinPropertyNodeShape.h"
#include "deSkinPropertyNodeVisitor.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeShape
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeShape::deSkinPropertyNodeShape() :
pShapeType(estRectangle),
pThickness(1.0f)
{
	int i;
	for(i=0; i<ShapeMappedCount; i++){
		pShapeMapped[i] = -1;
	}
}

deSkinPropertyNodeShape::~deSkinPropertyNodeShape(){
}



// Management
///////////////

void deSkinPropertyNodeShape::SetShapeType(eShapeTypes type){
	pShapeType = type;
}

void deSkinPropertyNodeShape::SetFillColor(const decColor &color){
	pFillColor = color;
}

void deSkinPropertyNodeShape::SetLineColor(const decColor &color){
	pLineColor = color;
}

void deSkinPropertyNodeShape::SetThickness(float thickness){
	pThickness = decMath::max(thickness, 0.0f);
}



int deSkinPropertyNodeShape::GetShapeMappedFor(eShapeMapped mapped) const{
	return pShapeMapped[mapped];
}

void deSkinPropertyNodeShape::SetShapeMappedFor(eShapeMapped mapped, int index){
	DEASSERT_TRUE(index >= -1)
	pShapeMapped[mapped] = index;
}



// Visiting
/////////////

void deSkinPropertyNodeShape::Visit(deSkinPropertyNodeVisitor &visitor){
	visitor.VisitShape(*this);
}
