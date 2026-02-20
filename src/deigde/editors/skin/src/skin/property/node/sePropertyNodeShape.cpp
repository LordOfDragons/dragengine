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

#include "sePropertyNodeShape.h"
#include "../seProperty.h"
#include "../../seSkin.h"
#include "../../texture/seTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class sePropertyNodeShape
//////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeShape::sePropertyNodeShape(const deEngine &engine) :
sePropertyNode(entShape, engine, ShapeMappedCount),
pShapeType(deSkinPropertyNodeShape::estRectangle),
pThickness(1.0f){
}

sePropertyNodeShape::sePropertyNodeShape(const sePropertyNodeShape &node) :
sePropertyNode(node),
pShapeType(node.pShapeType),
pFillColor(node.pFillColor),
pLineColor(node.pLineColor),
pThickness(node.pThickness){
}

sePropertyNodeShape::~sePropertyNodeShape(){
}



// Management
///////////////

void sePropertyNodeShape::SetShapeType(deSkinPropertyNodeShape::eShapeTypes type){
	if(type < deSkinPropertyNodeShape::estRectangle || type > deSkinPropertyNodeShape::estEllipse){
		DETHROW(deeInvalidParam);
	}
	
	if(type == pShapeType){
		return;
	}
	
	pShapeType = type;
	NotifyChanged();
}

void sePropertyNodeShape::SetFillColor(const decColor &color){
	if(color.IsEqualTo(pFillColor)){
		return;
	}
	
	pFillColor = color;
	NotifyChanged();
}

void sePropertyNodeShape::SetLineColor(const decColor &color){
	if(color.IsEqualTo(pLineColor)){
		return;
	}
	
	pLineColor = color;
	NotifyChanged();
}

void sePropertyNodeShape::SetThickness(float thickness){
	thickness = decMath::max(thickness, 0.0f);
	if(fabsf(thickness - pThickness) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pThickness = thickness;
	NotifyChanged();
}



sePropertyNode::Ref sePropertyNodeShape::Copy() const{
	return sePropertyNodeShape::Ref::New(*this);
}
