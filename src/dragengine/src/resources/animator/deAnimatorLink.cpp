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
#include <string.h>

#include "deAnimatorLink.h"

#include "../../common/exceptions.h"



// Class deAnimatorLink
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

deAnimatorLink::deAnimatorLink() :
pController(-1),
pRepeat(1),
pBoneParameter(ebpPositionZ),
pBoneMinValue(0.0f),
pBoneMaxValue(1.0f),
pVertexPositionSetMinValue(0.0f),
pVertexPositionSetMaxValue(1.0f),
pWrapY(false)
{
	pCurve.SetDefaultLinear();
}

deAnimatorLink::deAnimatorLink(const deAnimatorLink &copy) :
pController(copy.pController),
pCurve(copy.pCurve),
pRepeat(copy.pRepeat),
pBone(copy.pBone),
pBoneParameter(copy.pBoneParameter),
pBoneMinValue(copy.pBoneMinValue),
pBoneMaxValue(copy.pBoneMaxValue),
pVertexPositionSet(copy.pVertexPositionSet),
pVertexPositionSetMinValue(copy.pVertexPositionSetMinValue),
pVertexPositionSetMaxValue(copy.pVertexPositionSetMaxValue),
pWrapY(copy.pWrapY){
}

deAnimatorLink::~deAnimatorLink(){
}



// Management
///////////////

void deAnimatorLink::SetController(int controller){
	DEASSERT_TRUE(controller >= -1)
	pController = controller;
}

void deAnimatorLink::SetCurve(const decCurveBezier &curve){
	pCurve = curve;
}

void deAnimatorLink::SetRepeat(int repeat){
	DEASSERT_TRUE(repeat > 0)
	pRepeat = repeat;
}

void deAnimatorLink::SetBone(const char *bone){
	pBone = bone;
}

void deAnimatorLink::SetBoneParameter(eBoneParameter parameter){
	pBoneParameter = parameter;
}

void deAnimatorLink::SetBoneValueRange(float minimum, float maximum){
	pBoneMinValue = minimum;
	pBoneMaxValue = maximum;
}

void deAnimatorLink::SetVertexPositionSet(const char *vertexPositionSet){
	pVertexPositionSet = vertexPositionSet;
}

void deAnimatorLink::SetVertexPositionSetValueRange(float minimum, float maximum){
	pVertexPositionSetMinValue = minimum;
	pVertexPositionSetMaxValue = maximum;
}

void deAnimatorLink::SetWrapY(bool wrap){
	pWrapY = wrap;
}



// Operators
//////////////

deAnimatorLink &deAnimatorLink::operator=(const deAnimatorLink &copy){
	pController = copy.pController;
	pCurve = copy.pCurve;
	pRepeat = copy.pRepeat;
	pBone = copy.pBone;
	pBoneParameter = copy.pBoneParameter;
	pBoneMinValue = copy.pBoneMinValue;
	pBoneMaxValue = copy.pBoneMaxValue;
	pVertexPositionSet = copy.pVertexPositionSet;
	pVertexPositionSetMinValue = copy.pVertexPositionSetMinValue;
	pVertexPositionSetMaxValue = copy.pVertexPositionSetMaxValue;
	pWrapY = copy.pWrapY;
	return *this;
}
