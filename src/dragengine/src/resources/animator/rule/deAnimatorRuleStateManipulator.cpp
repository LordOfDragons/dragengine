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

#include "deAnimatorRuleStateManipulator.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleStateManipulator
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleStateManipulator::deAnimatorRuleStateManipulator() :
pMinVertexPositionSet(0.0f),
pMaxVertexPositionSet(0.0f),
pEnableVertexPositionSet(true)
{
	pMinSize.Set(1.0f, 1.0f, 1.0f);
	pMaxSize.Set(1.0f, 1.0f, 1.0f);
	pEnablePosition = false;
	pEnableRotation = true;
	pEnableSize = false;
}

deAnimatorRuleStateManipulator::~deAnimatorRuleStateManipulator(){
}



// Management
///////////////

void deAnimatorRuleStateManipulator::SetMinimumPosition(const decVector &position){
	pMinPosition = position;
}

void deAnimatorRuleStateManipulator::SetMaximumPosition(const decVector &position){
	pMaxPosition = position;
}

void deAnimatorRuleStateManipulator::SetMinimumRotation(const decVector &orientation){
	pMinRotation = orientation;
}

void deAnimatorRuleStateManipulator::SetMaximumRotation(const decVector &orientation){
	pMaxRotation = orientation;
}

void deAnimatorRuleStateManipulator::SetMinimumSize(const decVector &size){
	pMinSize = size;
}

void deAnimatorRuleStateManipulator::SetMaximumSize(const decVector &size){
	pMaxSize = size;
}

void deAnimatorRuleStateManipulator::SetMinimumVertexPositionSet(float weight){
	pMinVertexPositionSet = weight;
}

void deAnimatorRuleStateManipulator::SetMaximumVertexPositionSet(float weight){
	pMaxVertexPositionSet = weight;
}

void deAnimatorRuleStateManipulator::SetEnablePosition(bool enable){
	pEnablePosition = enable;
}

void deAnimatorRuleStateManipulator::SetEnableRotation(bool enable){
	pEnableRotation = enable;
}

void deAnimatorRuleStateManipulator::SetEnableSize(bool enable){
	pEnableSize = enable;
}

void deAnimatorRuleStateManipulator::SetEnableVertexPositionSet(bool enabled){
	pEnableVertexPositionSet = enabled;
}



// Visiting
/////////////

void deAnimatorRuleStateManipulator::Visit(deAnimatorRuleVisitor &visitor){
	visitor.VisitStateManipulator(*this);
}
