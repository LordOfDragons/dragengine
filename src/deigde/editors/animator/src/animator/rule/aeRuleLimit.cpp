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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeRuleLimit.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleLimit
//////////////////////

// Constructor, destructor
////////////////////////////

aeRuleLimit::aeRuleLimit() :
aeRule(deAnimatorRuleVisitorIdentify::ertLimit),
pMinScaling(1.0f, 1.0f, 1.0f),
pMaxScaling(1.0f, 1.0f, 1.0f),
pMinVertexPositionSet(0.0f),
pMaxVertexPositionSet(1.0f),
pCoordinateFrame(deAnimatorRuleLimit::ecfComponent),
pEnablePositionXMin(false),
pEnablePositionXMax(false),
pEnablePositionYMin(false),
pEnablePositionYMax(false),
pEnablePositionZMin(false),
pEnablePositionZMax(false),
pEnableRotationXMin(false),
pEnableRotationXMax(false),
pEnableRotationYMin(false),
pEnableRotationYMax(false),
pEnableRotationZMin(false),
pEnableRotationZMax(false),
pEnableScalingXMin(false),
pEnableScalingXMax(false),
pEnableScalingYMin(false),
pEnableScalingYMax(false),
pEnableScalingZMin(false),
pEnableScalingZMax(false),
pEnableVertexPositionSetMin(false),
pEnableVertexPositionSetMax(false)
{
	SetName("Limit");
}

aeRuleLimit::aeRuleLimit(const aeRuleLimit &copy) :
aeRule(copy),
pMinPosition(copy.pMinPosition),
pMaxPosition(copy.pMaxPosition),
pMinRotation(copy.pMinRotation),
pMaxRotation(copy.pMaxRotation),
pMinScaling(copy.pMinScaling),
pMaxScaling(copy.pMaxScaling),
pMinVertexPositionSet(copy.pMinVertexPositionSet),
pMaxVertexPositionSet(copy.pMaxVertexPositionSet),
pCoordinateFrame(copy.pCoordinateFrame),
pEnablePositionXMin(copy.pEnablePositionXMin),
pEnablePositionXMax(copy.pEnablePositionXMax),
pEnablePositionYMin(copy.pEnablePositionYMin),
pEnablePositionYMax(copy.pEnablePositionYMax),
pEnablePositionZMin(copy.pEnablePositionZMin),
pEnablePositionZMax(copy.pEnablePositionZMax),
pEnableRotationXMin(copy.pEnableRotationXMin),
pEnableRotationXMax(copy.pEnableRotationXMax),
pEnableRotationYMin(copy.pEnableRotationYMin),
pEnableRotationYMax(copy.pEnableRotationYMax),
pEnableRotationZMin(copy.pEnableRotationZMin),
pEnableRotationZMax(copy.pEnableRotationZMax),
pEnableScalingXMin(copy.pEnableScalingXMin),
pEnableScalingXMax(copy.pEnableScalingXMax),
pEnableScalingYMin(copy.pEnableScalingYMin),
pEnableScalingYMax(copy.pEnableScalingYMax),
pEnableScalingZMin(copy.pEnableScalingZMin),
pEnableScalingZMax(copy.pEnableScalingZMax),
pEnableVertexPositionSetMin(copy.pEnableVertexPositionSetMin),
pEnableVertexPositionSetMax(copy.pEnableVertexPositionSetMax),
pTargetBone(copy.pTargetBone){
}

aeRuleLimit::~aeRuleLimit(){
}



// Management
///////////////

void aeRuleLimit::SetMinimumPosition(const decVector &position){
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pMinPosition = position;
	
	if(rule){
		rule->SetMinimumPosition(position);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMaximumPosition(const decVector &position){
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pMaxPosition = position;
	
	if(rule){
		rule->SetMaximumPosition(position);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMinimumRotation(const decVector &rotation){
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pMinRotation = rotation;
	
	if(rule){
		rule->SetMinimumRotation(rotation * DEG2RAD);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMaximumRotation(const decVector &rotation){
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pMaxRotation = rotation;
	
	if(rule){
		rule->SetMaximumRotation(rotation * DEG2RAD);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMinimumScaling(const decVector &scaling){
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pMinScaling = scaling;
	
	if(rule){
		rule->SetMinimumScaling(scaling);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMaximumScaling(const decVector &scaling){
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pMaxScaling = scaling;
	
	if(rule){
		rule->SetMaximumScaling(scaling);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMinimumVertexPositionSet(float weight){
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pMinVertexPositionSet = weight;
	
	if(rule){
		rule->SetMinimumVertexPositionSet(weight);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMaximumVertexPositionSet(float weight){
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pMaxVertexPositionSet = weight;
	
	if(rule){
		rule->SetMaximumVertexPositionSet(weight);
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetCoordinateFrame(deAnimatorRuleLimit::eCoordinateFrames coordinateFrame){
	if(coordinateFrame < deAnimatorRuleLimit::ecfBoneLocal || coordinateFrame > deAnimatorRuleLimit::ecfTargetBone){
		DETHROW(deeInvalidParam);
	}
	
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pCoordinateFrame = coordinateFrame;
	
	if(rule){
		rule->SetCoordinateFrame(coordinateFrame);
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetEnablePositionXMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnablePositionXMin = enabled;
	
	if(rule){
		rule->SetEnablePositionXMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionXMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnablePositionXMax = enabled;
	
	if(rule){
		rule->SetEnablePositionXMax(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionYMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnablePositionYMin = enabled;
	
	if(rule){
		rule->SetEnablePositionYMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionYMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnablePositionYMax = enabled;
	
	if(rule){
		rule->SetEnablePositionYMax(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionZMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnablePositionZMin = enabled;
	
	if(rule){
		rule->SetEnablePositionZMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionZMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnablePositionZMax = enabled;
	
	if(rule){
		rule->SetEnablePositionZMax(enabled);
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetEnableRotationXMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableRotationXMin = enabled;
	
	if(rule){
		rule->SetEnableRotationXMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationXMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableRotationXMax = enabled;
	
	if(rule){
		rule->SetEnableRotationXMax(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationYMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableRotationYMin = enabled;
	
	if(rule){
		rule->SetEnableRotationYMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationYMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableRotationYMax = enabled;
	
	if(rule){
		rule->SetEnableRotationYMax(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationZMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableRotationZMin = enabled;
	
	if(rule){
		rule->SetEnableRotationZMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationZMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableRotationZMax = enabled;
	
	if(rule){
		rule->SetEnableRotationZMax(enabled);
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetEnableScalingXMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableScalingXMin = enabled;
	
	if(rule){
		rule->SetEnableScalingXMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingXMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableScalingXMax = enabled;
	
	if(rule){
		rule->SetEnableScalingXMax(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingYMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableScalingYMin = enabled;
	
	if(rule){
		rule->SetEnableScalingYMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingYMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableScalingYMax = enabled;
	
	if(rule){
		rule->SetEnableScalingYMax(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingZMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableScalingZMin = enabled;
	
	if(rule){
		rule->SetEnableScalingZMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingZMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableScalingZMax = enabled;
	
	if(rule){
		rule->SetEnableScalingZMax(enabled);
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetEnableVertexPositionSetMin(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableVertexPositionSetMin = enabled;
	
	if(rule){
		rule->SetEnableVertexPositionSetMin(enabled);
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableVertexPositionSetMax(bool enabled){
	deAnimatorRuleLimit * const rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pEnableVertexPositionSetMax = enabled;
	
	if(rule){
		rule->SetEnableVertexPositionSetMax(enabled);
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetTargetBone(const char *boneName){
	deAnimatorRuleLimit *rule = (deAnimatorRuleLimit*)GetEngineRule();
	
	pTargetBone = boneName;
	
	if(rule){
		rule->SetTargetBone(boneName);
		NotifyRuleChanged();
	}
}



deAnimatorRule *aeRuleLimit::CreateEngineRule(){
	deAnimatorRuleLimit *engRule = NULL;
	
	try{
		engRule = new deAnimatorRuleLimit;
		
		InitEngineRule(engRule);
		
		engRule->SetMinimumPosition(pMinPosition);
		engRule->SetMaximumPosition(pMaxPosition);
		engRule->SetMinimumRotation(pMinRotation * DEG2RAD);
		engRule->SetMaximumRotation(pMaxRotation * DEG2RAD);
		engRule->SetMinimumScaling(pMinScaling);
		engRule->SetMaximumScaling(pMaxScaling);
		engRule->SetMinimumVertexPositionSet(pMinVertexPositionSet);
		engRule->SetMaximumVertexPositionSet(pMaxVertexPositionSet);
		engRule->SetCoordinateFrame(pCoordinateFrame);
		engRule->SetTargetBone(pTargetBone);
		
		engRule->SetEnablePositionXMin(pEnablePositionXMin);
		engRule->SetEnablePositionXMax(pEnablePositionXMax);
		engRule->SetEnablePositionYMin(pEnablePositionYMin);
		engRule->SetEnablePositionYMax(pEnablePositionYMax);
		engRule->SetEnablePositionZMin(pEnablePositionZMin);
		engRule->SetEnablePositionZMax(pEnablePositionZMax);
		
		engRule->SetEnableRotationXMin(pEnableRotationXMin);
		engRule->SetEnableRotationXMax(pEnableRotationXMax);
		engRule->SetEnableRotationYMin(pEnableRotationYMin);
		engRule->SetEnableRotationYMax(pEnableRotationYMax);
		engRule->SetEnableRotationZMin(pEnableRotationZMin);
		engRule->SetEnableRotationZMax(pEnableRotationZMax);
		
		engRule->SetEnableScalingXMin(pEnableScalingXMin);
		engRule->SetEnableScalingXMax(pEnableScalingXMax);
		engRule->SetEnableScalingYMin(pEnableScalingYMin);
		engRule->SetEnableScalingYMax(pEnableScalingYMax);
		engRule->SetEnableScalingZMin(pEnableScalingZMin);
		engRule->SetEnableScalingZMax(pEnableScalingZMax);
		
		engRule->SetEnableVertexPositionSetMin(pEnableVertexPositionSetMin);
		engRule->SetEnableVertexPositionSetMax(pEnableVertexPositionSetMax);
		
	}catch(const deException &){
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleLimit::CreateCopy() const{
	return new aeRuleLimit(*this);
}

void aeRuleLimit::ListLinks(aeLinkList &list){
	aeRule::ListLinks(list);
}



// Operators
//////////////

aeRuleLimit &aeRuleLimit::operator=(const aeRuleLimit &copy){
	SetMinimumPosition(copy.pMinPosition);
	SetMaximumPosition(copy.pMaxPosition);
	SetMinimumRotation(copy.pMinRotation);
	SetMaximumRotation(copy.pMaxRotation);
	SetMinimumScaling(copy.pMinScaling);
	SetMaximumScaling(copy.pMaxScaling);
	SetMinimumVertexPositionSet(copy.pMinVertexPositionSet);
	SetMaximumVertexPositionSet(copy.pMaxVertexPositionSet);
	SetCoordinateFrame(copy.pCoordinateFrame);
	SetEnablePositionXMin(copy.pEnablePositionXMin);
	SetEnablePositionXMax(copy.pEnablePositionXMax);
	SetEnablePositionYMin(copy.pEnablePositionYMin);
	SetEnablePositionYMax(copy.pEnablePositionYMax);
	SetEnablePositionZMin(copy.pEnablePositionZMin);
	SetEnablePositionZMax(copy.pEnablePositionZMax);
	SetEnableRotationXMin(copy.pEnableRotationXMin);
	SetEnableRotationXMax(copy.pEnableRotationXMax);
	SetEnableRotationYMin(copy.pEnableRotationYMin);
	SetEnableRotationYMax(copy.pEnableRotationYMax);
	SetEnableRotationZMin(copy.pEnableRotationZMin);
	SetEnableRotationZMax(copy.pEnableRotationZMax);
	SetEnableScalingXMin(copy.pEnableScalingXMin);
	SetEnableScalingXMax(copy.pEnableScalingXMax);
	SetEnableScalingYMin(copy.pEnableScalingYMin);
	SetEnableScalingYMax(copy.pEnableScalingYMax);
	SetEnableScalingZMin(copy.pEnableScalingZMin);
	SetEnableScalingZMax(copy.pEnableScalingZMax);
	SetEnableVertexPositionSetMin(copy.pEnableVertexPositionSetMin);
	SetEnableVertexPositionSetMax(copy.pEnableVertexPositionSetMax);
	SetTargetBone(copy.pTargetBone);
	aeRule::operator=(copy);
	return *this;
}
