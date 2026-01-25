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

#include "decCurve2D.h"

#include "../collection/decTList.h"
#include "../exceptions.h"



// Class decCurve2D
/////////////////////

// Constructor, destructor
////////////////////////////

decCurve2D::decCurve2D(){
}

decCurve2D::decCurve2D(const decCurve2D &curve) :
pPoints(curve.pPoints){
}

decCurve2D::~decCurve2D(){
}



// Management
///////////////

const decVector2 &decCurve2D::GetPointAt(int position) const{
	return pPoints.GetAt(position);
}

int decCurve2D::IndexOfPointClosestTo(float coordinate, float threshold) const{
	const float lower = coordinate - threshold;
	const float upper = coordinate + threshold;
	int p;
	
	for(p=0; p<pPoints.GetCount(); p++){
		if(pPoints[p].x > upper){
			break;
		}
		if(pPoints[p].x >= lower){
			return p;
		}
	}
	
	return -1;
}

int decCurve2D::IndexOfPointBefore(float coordinate) const{
	int p;
	
	for(p=0; p<pPoints.GetCount(); p++){
		if(coordinate < pPoints[p].x){
			return p - 1;
		}
	}
	
	return pPoints.GetCount() - 1;
}

int decCurve2D::AddPoint(const decVector2 &point, float threshold){
	const int index = IndexOfPointBefore(point.x);
	if(index != -1 && fabsf(point.x - pPoints[index].x) <= threshold){
		pPoints[index].y = point.y;
		return index;
		
	}else{
		pPoints.Insert(point, index + 1);
		return index + 1;
	}
}

void decCurve2D::RemovePointFrom(int position){
	pPoints.RemoveFrom(position);
}

void decCurve2D::RemoveAllPoints(){
	pPoints.RemoveAll();
}



void decCurve2D::SetDefaultCurve(){
	pPoints.RemoveAll();
	pPoints.Add({0.0f, 0.0f});
	pPoints.Add({1.0f, 1.0f});
}

void decCurve2D::OffsetPointsBy(float offset){
	int p;
	
	for(p=0; p<pPoints.GetCount(); p++){
		pPoints[p].y += offset;
	}
}

void decCurve2D::ScalePointsBy(float scale){
	int p;
	
	for(p=0; p<pPoints.GetCount(); p++){
		pPoints[p].y *= scale;
	}
}



float decCurve2D::EvaluateConstant(float coordinate) const{
	if(pPoints.GetCount() == 0){
		return 0.0f;
	}
	
	int p;
	
	for(p=1; p<pPoints.GetCount(); p++){
		if(coordinate < pPoints[p].x){
			return pPoints[p - 1].y;
		}
	}
	
	return pPoints[pPoints.GetCount() - 1].y;
}

float decCurve2D::EvaluateLinear(float coordinate) const{
	if(pPoints.GetCount() == 0){
		return 0.0f;
	}
	if(pPoints.GetCount() == 1){
		return pPoints[0].y;
	}
	
	float blend;
	int p;
	
	if(coordinate < pPoints[0].x){
		return pPoints[0].y;
		
	}else{
		for(p=1; p<pPoints.GetCount(); p++){
			if(coordinate < pPoints[p].x){
				blend = (coordinate - pPoints[p - 1].x) / (pPoints[p].x - pPoints[p - 1].x);
				return pPoints[p - 1].y * (1.0f - blend) + pPoints[p].y * blend;
			}
		}
		
		return pPoints[pPoints.GetCount() - 1].y;
	}
}



decCurve2D &decCurve2D::operator=(const decCurve2D &curve){
	pPoints = curve.pPoints;
	return *this;
}
