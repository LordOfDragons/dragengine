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
pInterpolationMode(eimBezier){
}

decCurveBezier3D::decCurveBezier3D(const decCurveBezier3D &copy) :
pInterpolationMode(eimBezier)
{
	*this = copy;
}

decCurveBezier3D::~decCurveBezier3D(){
}



// Management
///////////////

decCurveBezier3DPoint &decCurveBezier3D::GetPointAt(int position){
	return pPoints[position];
}

const decCurveBezier3DPoint &decCurveBezier3D::GetPointAt(int position) const{
	return pPoints[position];
}

void decCurveBezier3D::AddPoint(const decCurveBezier3DPoint &point){
	pPoints.Add(point);
}

void decCurveBezier3D::InsertPoint(int position, const decCurveBezier3DPoint &point){
	pPoints.Insert(point, position + 1);
}

void decCurveBezier3D::RemovePointFrom(int position){
	pPoints.RemoveFrom(position);
}

void decCurveBezier3D::RemoveAllPoints(){
	pPoints.RemoveAll();
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
	pPoints = curve.pPoints;
	pInterpolationMode = curve.pInterpolationMode;
	return *this;
}

bool decCurveBezier3D::operator==(const decCurveBezier3D &curve) const{
	if(curve.pPoints.GetCount() != pPoints.GetCount()){
		return false;
	}
	
	if(pInterpolationMode != curve.pInterpolationMode){
		return false;
	}
	
	int i;
	for(i=0; i<pPoints.GetCount(); i++){
		if(pPoints[i] != curve.pPoints[i]){
			return false;
		}
	}
	
	return true;
}
