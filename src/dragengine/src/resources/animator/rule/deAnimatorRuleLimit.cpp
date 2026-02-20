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

#include "deAnimatorRuleLimit.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleLimit
//////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleLimit::deAnimatorRuleLimit() :
pMinVertexPositionSet(0.0f),
pMaxVertexPositionSet(1.0f),
pEnableVertexPositionSetMin(false),
pEnableVertexPositionSetMax(false)
{
	pCoordinateFrame = ecfComponent;
	pMinScaling.Set(1.0f, 1.0f, 1.0f);
	pMaxScaling.Set(1.0f, 1.0f, 1.0f);
	
	pEnablePositionXMin = false;
	pEnablePositionXMax = false;
	pEnablePositionYMin = false;
	pEnablePositionYMax = false;
	pEnablePositionZMin = false;
	pEnablePositionZMax = false;
	
	pEnableRotationXMin = false;
	pEnableRotationXMax = false;
	pEnableRotationYMin = false;
	pEnableRotationYMax = false;
	pEnableRotationZMin = false;
	pEnableRotationZMax = false;
	
	pEnableScalingXMin = false;
	pEnableScalingXMax = false;
	pEnableScalingYMin = false;
	pEnableScalingYMax = false;
	pEnableScalingZMin = false;
	pEnableScalingZMax = false;
}

deAnimatorRuleLimit::~deAnimatorRuleLimit(){
}



// Management
///////////////

void deAnimatorRuleLimit::SetMinimumPosition(const decVector &position){
	pMinPosition = position;
}

void deAnimatorRuleLimit::SetMaximumPosition(const decVector &position){
	pMaxPosition = position;
}

void deAnimatorRuleLimit::SetMinimumRotation(const decVector &rotation){
	pMinRotation = rotation;
}

void deAnimatorRuleLimit::SetMaximumRotation(const decVector &rotation){
	pMaxRotation = rotation;
}

void deAnimatorRuleLimit::SetMinimumScaling(const decVector &scaling){
	pMinScaling = scaling;
}

void deAnimatorRuleLimit::SetMaximumScaling(const decVector &scaling){
	pMaxScaling = scaling;
}

void deAnimatorRuleLimit::SetMinimumVertexPositionSet(float weight){
	pMinVertexPositionSet = weight;
}

void deAnimatorRuleLimit::SetMaximumVertexPositionSet(float weight){
	pMaxVertexPositionSet = weight;
}



void deAnimatorRuleLimit::SetCoordinateFrame(eCoordinateFrames coordinateFrame){
	pCoordinateFrame = coordinateFrame;
}



void deAnimatorRuleLimit::SetEnablePositionXMin(bool enabled){
	pEnablePositionXMin = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionXMax(bool enabled){
	pEnablePositionXMax = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionYMin(bool enabled){
	pEnablePositionYMin = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionYMax(bool enabled){
	pEnablePositionYMax = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionZMin(bool enabled){
	pEnablePositionZMin = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionZMax(bool enabled){
	pEnablePositionZMax = enabled;
}



void deAnimatorRuleLimit::SetEnableRotationXMin(bool enabled){
	pEnableRotationXMin = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationXMax(bool enabled){
	pEnableRotationXMax = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationYMin(bool enabled){
	pEnableRotationYMin = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationYMax(bool enabled){
	pEnableRotationYMax = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationZMin(bool enabled){
	pEnableRotationZMin = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationZMax(bool enabled){
	pEnableRotationZMax = enabled;
}



void deAnimatorRuleLimit::SetEnableScalingXMin(bool enabled){
	pEnableScalingXMin = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingXMax(bool enabled){
	pEnableScalingXMax = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingYMin(bool enabled){
	pEnableScalingYMin = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingYMax(bool enabled){
	pEnableScalingYMax = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingZMin(bool enabled){
	pEnableScalingZMin = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingZMax(bool enabled){
	pEnableScalingZMax = enabled;
}



void deAnimatorRuleLimit::SetEnableVertexPositionSetMin(bool enabled){
	pEnableVertexPositionSetMin = enabled;
}

void deAnimatorRuleLimit::SetEnableVertexPositionSetMax(bool enabled){
	pEnableVertexPositionSetMax = enabled;
}



void deAnimatorRuleLimit::SetTargetBone(const char *boneName){
	pTargetBone = boneName;
}



// Visiting
/////////////

void deAnimatorRuleLimit::Visit(deAnimatorRuleVisitor &visitor){
	visitor.VisitLimit(*this);
}
