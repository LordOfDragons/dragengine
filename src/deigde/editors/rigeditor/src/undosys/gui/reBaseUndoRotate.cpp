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

#include "reBaseUndoRotate.h"



// class reBaseUndoRotate
///////////////////////////

// Constructor, destructor
////////////////////////////

reBaseUndoRotate::reBaseUndoRotate(){
	pModifyPosition = true;
	pModifyOrientation = true;
	SetShortInfo("Rotate ?");
	pAxis = decVector(0.0f, 0.0f, 1.0f);
}

reBaseUndoRotate::~reBaseUndoRotate(){
}



// Management
///////////////

void reBaseUndoRotate::SetAngle(float angle){
	pAngle = angle;
	Update();
}

void reBaseUndoRotate::SetCenterSector(const decPoint3 &sector){
	pCenterSector = sector;
	Update();
}

void reBaseUndoRotate::SetCenterPosition(const decVector &position){
	pCenterPosition = position;
	Update();
}

void reBaseUndoRotate::SetAxis(const decVector &axis){
	pAxis = axis;
	pAxis.Normalize();
	Update();
}

void reBaseUndoRotate::SetModifyPosition(bool modifyPosition){
	pModifyPosition = modifyPosition;
}

void reBaseUndoRotate::SetModifyOrientation(bool modifyOrientation){
	pModifyOrientation = modifyOrientation;
}

void reBaseUndoRotate::Update(){
	pMatrix = decMatrix::CreateTranslation(-pCenterPosition)
		* decMatrix::CreateRotationAxis( pAxis, pAngle )
		* decMatrix::CreateTranslation( pCenterPosition );
	
	decString info;
	info.Format("axis(%g,%g,%g) center(%i,%i,%i - %g,%g,%g) angle=%g°",
		pAxis.x, pAxis.y, pAxis.z, pCenterSector.x, pCenterSector.y, pCenterSector.z,
		pCenterPosition.x, pCenterPosition.y, pCenterPosition.z, pAngle / DEG2RAD);
	SetLongInfo(info.GetString());
}



// Undo and Redo actions
//////////////////////////

void reBaseUndoRotate::ProgressiveRedo(){
	Undo();
	Redo();
}
