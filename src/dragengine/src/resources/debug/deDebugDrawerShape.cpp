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

#include "deDebugDrawerShape.h"
#include "deDebugDrawerShapeFace.h"
#include "../../common/shape/decShape.h"
#include "../../common/shape/decShapeList.h"
#include "../../common/exceptions.h"



// Class deDebugDrawerShape
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deDebugDrawerShape::deDebugDrawerShape(){
	pScale.Set(1.0f, 1.0f, 1.0f);
	pEdgeColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
	pFillColor.Set(1.0f, 1.0f, 1.0f, 0.0f);
}

deDebugDrawerShape::~deDebugDrawerShape(){
	RemoveAllFaces();
}



// Management
///////////////

void deDebugDrawerShape::SetPosition(const decVector &position){
	pPosition = position;
}

void deDebugDrawerShape::SetOrientation(const decQuaternion &orientation){
	pOrientation = orientation;
}

void deDebugDrawerShape::SetScale(const decVector &scale){
	pScale = scale;
}

void deDebugDrawerShape::SetEdgeColor(const decColor &color){
	pEdgeColor = color;
}

void deDebugDrawerShape::SetFillColor(const decColor &color){
	pFillColor = color;
}



// Faces
//////////

int deDebugDrawerShape::GetFaceCount() const{
	return pFaces.GetCount();
}

deDebugDrawerShapeFace *deDebugDrawerShape::GetFaceAt(int index) const{
	return (deDebugDrawerShapeFace*)pFaces.GetAt(index);
}

void deDebugDrawerShape::AddFace(deDebugDrawerShapeFace *face){
	// no pFaces.Has(face) check. for large number of faces this becomes very slow
	if(! face){
		DETHROW(deeInvalidParam);
	}
	pFaces.Add(face);
}

void deDebugDrawerShape::RemoveAllFaces(){
	const int count = pFaces.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (deDebugDrawerShapeFace*)pFaces.GetAt(i);
	}
	pFaces.RemoveAll();
}
