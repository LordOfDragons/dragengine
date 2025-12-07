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

#include "decShapeHull.h"
#include "decShapeVisitor.h"
#include "../exceptions.h"



// Class decShapeHull
///////////////////////

// Constructors, destructors
//////////////////////////////

decShapeHull::decShapeHull() :
pPoints(NULL),
pPointCount(0){
}

decShapeHull::decShapeHull(const decVector &position) :
decShape(position),
pPoints(NULL),
pPointCount(0){
}

decShapeHull::decShapeHull(const decVector &position, const decQuaternion &orientation) :
decShape(position, orientation),
pPoints(NULL),
pPointCount(0){
}

decShapeHull::~decShapeHull(){
	if(pPoints){
		delete [] pPoints;
	}
}



// Management
///////////////

void decShapeHull::SetPointCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pPointCount){
		return;
	}
	
	if(pPoints){
		delete [] pPoints;
		pPoints = NULL;
		pPointCount = 0;
	}
	
	if(count > 0){
		pPoints = new decVector[count];
		pPointCount = count;
	}
}

const decVector &decShapeHull::GetPointAt(int index) const{
	if(index < 0 || index >= pPointCount){
		DETHROW(deeOutOfBoundary);
	}
	
	return pPoints[index];
}

void decShapeHull::SetPointAt(int index, const decVector &point){
	if(index < 0 || index >= pPointCount){
		DETHROW(deeOutOfBoundary);
	}
	
	pPoints[index] = point;
}



decShape *decShapeHull::Copy() const{
	decShapeHull * const shape = new decShapeHull(GetPosition(), GetOrientation());
	int i;
	
	try{
		shape->SetPointCount(pPointCount);
		for(i=0; i<pPointCount; i++){
			shape->SetPointAt(i, pPoints[i]);
		}
		
	}catch(const deException &){
		if(shape){
			delete shape;
		}
		throw;
	}
	
	return shape;
}



// Visiting
/////////////

void decShapeHull::Visit(decShapeVisitor &visitor){
	visitor.VisitShapeHull(*this);
}
