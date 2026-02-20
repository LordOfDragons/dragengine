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

#include "deAnimatorRuleAnimationDifference.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleAnimationDifference
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleAnimationDifference::deAnimatorRuleAnimationDifference() :
pMove1Name("idle "),
pMove1Time(0.0f),
pMove2Name("idle"),
pMove2Time(0.0f),
pUseSameMove(false),
pEnablePosition(true),
pEnableOrientation(true),
pEnableSize(false),
pEnableVertexPositionSet(true){
}

deAnimatorRuleAnimationDifference::~deAnimatorRuleAnimationDifference(){
}



// Management
///////////////

void deAnimatorRuleAnimationDifference::SetLeadingMoveName(const char *moveName){
	pMove1Name = moveName;
}

void deAnimatorRuleAnimationDifference::SetLeadingMoveTime(float moveTime){
	pMove1Time = moveTime;
}

void deAnimatorRuleAnimationDifference::SetReferenceMoveName(const char *moveName){
	pMove2Name = moveName;
}

void deAnimatorRuleAnimationDifference::SetReferenceMoveTime(float moveTime){
	pMove2Time = moveTime;
}

void deAnimatorRuleAnimationDifference::SetUseSameMove(bool useSameMove){
	pUseSameMove = useSameMove;
}

void deAnimatorRuleAnimationDifference::SetEnablePosition(bool enabled){
	pEnablePosition = enabled;
}

void deAnimatorRuleAnimationDifference::SetEnableOrientation(bool enabled){
	pEnableOrientation = enabled;
}

void deAnimatorRuleAnimationDifference::SetEnableSize(bool enabled){
	pEnableSize = enabled;
}

void deAnimatorRuleAnimationDifference::SetEnableVertexPositionSet(bool enabled){
	pEnableVertexPositionSet = enabled;
}



// Visiting
/////////////

void deAnimatorRuleAnimationDifference::Visit(deAnimatorRuleVisitor &visitor){
	visitor.VisitAnimationDifference(*this);
}
