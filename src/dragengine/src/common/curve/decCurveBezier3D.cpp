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

#include "decCurveBezier3D.h"
#include "decCurveBezier3DPoint.h"

#include "../exceptions.h"



// Class decCurveBezier3D
///////////////////////////

// Constructor, destructor
////////////////////////////

decCurveBezier3D::decCurveBezier3D() :
pPoints(NULL),
pPointCount(0),
pInterpolationMode(eimBezier){
}

decCurveBezier3D::decCurveBezier3D(const decCurveBezier3D &copy) :
pPoints(NULL),
pPointCount(0),
pInterpolationMode(eimBezier)
{
	*this = copy;
}

decCurveBezier3D::~decCurveBezier3D(){
	if(pPoints){
		delete [] pPoints;
	}
}



// Management
///////////////

decCurveBezier3DPoint &decCurveBezier3D::GetPointAt(int position){
	if(position < 0 || position >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	
	return pPoints[position];
}

const decCurveBezier3DPoint &decCurveBezier3D::GetPointAt(int position) const{
	if(position < 0 || position >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	
	return pPoints[position];
}

void decCurveBezier3D::AddPoint(const decCurveBezier3DPoint &point){
	int p;
	
	decCurveBezier3DPoint *newArray = new decCurveBezier3DPoint[pPointCount + 1];
	
	for(p=0; p<pPointCount; p++){
		newArray[p] = pPoints[p];
	}
	newArray[pPointCount] = point;
	
	if(pPoints){
		delete [] pPoints;
	}
	pPoints = newArray;
	pPointCount++;
}

void decCurveBezier3D::InsertPoint(int position, const decCurveBezier3DPoint &point){
	if(position < 0 || position > pPointCount){
		DETHROW(deeInvalidParam);
	}
	
	int p;
	
	decCurveBezier3DPoint *newArray = new decCurveBezier3DPoint[pPointCount + 1];
	
	for(p=0; p<=position; p++){
		newArray[p] = pPoints[p];
	}
	newArray[position + 1] = point;
	for(p=position+1; p<pPointCount; p++){
		newArray[p + 1] = pPoints[p];
	}
	
	if(pPoints){
		delete [] pPoints;
	}
	pPoints = newArray;
	pPointCount++;
}

void decCurveBezier3D::RemovePointFrom(int position){
	if(position < 0 || position >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	
	int p;
	
	for(p=position+1; p<pPointCount; p++){
		pPoints[p - 1] = pPoints[p];
	}
	pPointCount--;
}

void decCurveBezier3D::RemoveAllPoints(){
	pPointCount = 0;
}



void decCurveBezier3D::SetInterpolationMode(eInterpolationModes mode){
	if(mode < eimConstant || mode > eimBezier){
		DETHROW(deeInvalidParam);
	}
	
	pInterpolationMode = mode;
}



// Operators
//////////////

decCurveBezier3DPoint &decCurveBezier3D::operator[](int position){
	return GetPointAt(position);
}

const decCurveBezier3DPoint &decCurveBezier3D::operator[](int position) const{
	return GetPointAt(position);
}

decCurveBezier3D &decCurveBezier3D::operator=(const decCurveBezier3D &curve){
	int p, pointCount = curve.GetPointCount();
	decCurveBezier3DPoint *newArray = NULL;
	
	if(pointCount > 0){
		newArray = new decCurveBezier3DPoint[pointCount];
		
		for(p=0; p<pointCount; p++){
			newArray[p] = curve.pPoints[p];
		}
	}
	
	if(pPoints) delete [] pPoints;
	pPoints = newArray;
	pPointCount = pointCount;
	
	pInterpolationMode = curve.pInterpolationMode;
	
	return *this;
}

bool decCurveBezier3D::operator==(const decCurveBezier3D &curve) const{
	if(curve.pPointCount != pPointCount){
		return false;
	}
	
	if(pInterpolationMode != curve.pInterpolationMode){
		return false;
	}
	
	int i;
	for(i=0; i<pPointCount; i++){
		if(pPoints[i] != curve.pPoints[i]){
			return false;
		}
	}
	
	return true;
}
