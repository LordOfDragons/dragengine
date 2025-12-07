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

#include "deAnimatorRuleForeignState.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleForeignState
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleForeignState::deAnimatorRuleForeignState() :
pSourceCoordinateFrame(ecfBoneLocal),
pDestCoordinateFrame(ecfBoneLocal),
pScalePosition(1.0f),
pScaleOrientation(1.0f),
pScaleSize(1.0f),
pScaleVertexPositionSet(1.0f),
pModifyX(true),
pModifyY(true),
pModifyZ(true),
pEnablePosition(true),
pEnableOrientation(true),
pEnableSize(false),
pEnableVertexPositionSet(true){
}

deAnimatorRuleForeignState::~deAnimatorRuleForeignState(){
}



// Management
///////////////

void deAnimatorRuleForeignState::SetForeignBone(const char *boneName){
	pForeignBone = boneName;
}

void deAnimatorRuleForeignState::SetForeignVertexPositionSet(const char *vertexPositionSet){
	pForeignVertexPositionSet = vertexPositionSet;
}

void deAnimatorRuleForeignState::SetScalePosition(float scalePosition){
	pScalePosition = scalePosition;
}

void deAnimatorRuleForeignState::SetScaleOrientation(float scaleOrientation){
	pScaleOrientation = scaleOrientation;
}

void deAnimatorRuleForeignState::SetScaleSize(float scaleSize){
	pScaleSize = scaleSize;
}

void deAnimatorRuleForeignState::SetScaleVertexPositionSet(float scale){
	pScaleVertexPositionSet = scale;
}

void deAnimatorRuleForeignState::SetModifyX(bool modify){
	pModifyX = modify;
}

void deAnimatorRuleForeignState::SetModifyY(bool modify){
	pModifyY = modify;
}

void deAnimatorRuleForeignState::SetModifyZ(bool modify){
	pModifyZ = modify;
}

void deAnimatorRuleForeignState::SetSourceCoordinateFrame(eCoordinateFrames coordinateFrame){
	pSourceCoordinateFrame = coordinateFrame;
}

void deAnimatorRuleForeignState::SetDestCoordinateFrame(eCoordinateFrames coordinateFrame){
	pDestCoordinateFrame = coordinateFrame;
}

void deAnimatorRuleForeignState::SetEnablePosition(bool enable){
	pEnablePosition = enable;
}

void deAnimatorRuleForeignState::SetEnableOrientation(bool enable){
	pEnableOrientation = enable;
}

void deAnimatorRuleForeignState::SetEnableSize(bool enable){
	pEnableSize = enable;
}

void deAnimatorRuleForeignState::SetEnableVertexPositionSet(bool enabled){
	pEnableVertexPositionSet = enabled;
}



// Visiting
/////////////

void deAnimatorRuleForeignState::Visit(deAnimatorRuleVisitor &visitor){
	visitor.VisitForeignState(*this);
}
