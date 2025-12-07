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

#include "decShapeSphere.h"
#include "decShapeVisitor.h"
#include "../exceptions.h"



// Class decShapeSphere
/////////////////////////

// Constructors, destructors
//////////////////////////////

decShapeSphere::decShapeSphere(float radius){
	SetRadius(radius);
	pAxisScaling.Set(1.0f, 1.0f);
}

decShapeSphere::decShapeSphere(float radius, const decVector &position) : decShape(position){
	SetRadius(radius);
	pAxisScaling.Set(1.0f, 1.0f);
}

decShapeSphere::decShapeSphere(float radius, const decVector2 &axisScaling){
	SetRadius(radius);
	SetAxisScaling(axisScaling);
}

decShapeSphere::decShapeSphere(float radius, const decVector2 &axisScaling, const decVector &position) : decShape(position){
	SetRadius(radius);
	SetAxisScaling(axisScaling);
}

decShapeSphere::~decShapeSphere(){
}



// Management
///////////////

void decShapeSphere::SetRadius(float radius){
	pRadius = decMath::max(radius, 0.0f);
}

void decShapeSphere::SetAxisScaling(const decVector2 &axisScaling){
	pAxisScaling = axisScaling.Largest(decVector2(0.0f, 0.0f));
}



decShape *decShapeSphere::Copy() const{
	return new decShapeSphere(pRadius, pAxisScaling, GetPosition());
}



// Visiting
/////////////

void decShapeSphere::Visit(decShapeVisitor &visitor){
	visitor.VisitShapeSphere(*this);
}
