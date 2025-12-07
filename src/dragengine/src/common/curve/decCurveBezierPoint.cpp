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

#include "decCurveBezierPoint.h"

#include "../exceptions.h"



// Class decCurveBezierPoint
//////////////////////////////

// Constructor, destructor
////////////////////////////

decCurveBezierPoint::decCurveBezierPoint(){
}

decCurveBezierPoint::decCurveBezierPoint(const decVector2 &point) :
pPoint(point),
pHandle1(point),
pHandle2(point){
}

decCurveBezierPoint::decCurveBezierPoint(const decVector2 &point, const decVector2 &handle1, const decVector2 &handle2) :
pPoint(point),
pHandle1(handle1),
pHandle2(handle2){
}

decCurveBezierPoint::decCurveBezierPoint(const decCurveBezierPoint &point) :
pPoint(point.pPoint),
pHandle1(point.pHandle1),
pHandle2(point.pHandle2){
}



// Management
///////////////

void decCurveBezierPoint::SetPoint(const decVector2 &point){
	pPoint = point;
	pHandle1 = point;
	pHandle2 = point;
}

void decCurveBezierPoint::SetPoint(const decVector2 &point, const decVector2 &handle1, const decVector2 &handle2){
	pPoint = point;
	pHandle1 = handle1;
	pHandle2 = handle2;
}

bool decCurveBezierPoint::IsEqualTo(const decCurveBezierPoint &point, float threshold) const{
	return pPoint.IsEqualTo(point.pPoint, threshold)
		&& pHandle1.IsEqualTo(point.pHandle1, threshold)
		&& pHandle2.IsEqualTo(point.pHandle2, threshold);
}
