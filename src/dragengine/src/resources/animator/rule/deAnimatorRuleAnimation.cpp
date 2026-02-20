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

#include "deAnimatorRuleAnimation.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleAnimation
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleAnimation::deAnimatorRuleAnimation() :
pMoveName("idle"),
pMoveTime(0.0f),
pEnablePosition(true),
pEnableOrientation(true),
pEnableSize(false),
pEnableVertexPositionSet(true){
}

deAnimatorRuleAnimation::~deAnimatorRuleAnimation(){
}



// Management
///////////////

void deAnimatorRuleAnimation::SetMoveName(const char *moveName){
	pMoveName = moveName;
}

void deAnimatorRuleAnimation::SetMoveTime(float moveTime){
	pMoveTime = moveTime;
}

void deAnimatorRuleAnimation::SetEnablePosition(bool enabled){
	pEnablePosition = enabled;
}

void deAnimatorRuleAnimation::SetEnableOrientation(bool enabled){
	pEnableOrientation = enabled;
}

void deAnimatorRuleAnimation::SetEnableSize(bool enabled){
	pEnableSize = enabled;
}

void deAnimatorRuleAnimation::SetEnableVertexPositionSet(bool enabled){
	pEnableVertexPositionSet = enabled;
}



// Visiting
/////////////

void deAnimatorRuleAnimation::Visit(deAnimatorRuleVisitor &visitor){
	visitor.VisitAnimation(*this);
}
