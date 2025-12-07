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

// includes

#include <stdio.h>
#include <stdlib.h>

#include "meBaseUndoRotate.h"
#include "../../worldedit.h"



// class meBaseUndoRotate
///////////////////////////

// Constructor, destructor
////////////////////////////

meBaseUndoRotate::meBaseUndoRotate(){
	pModifyPosition = true;
	pModifyOrientation = true;
	SetShortInfo("Rotate ?");
	pAxis.Set(0.0, 0.0, 1.0);
}

meBaseUndoRotate::~meBaseUndoRotate(){
}



// Management
///////////////

void meBaseUndoRotate::SetAngle(float angle){
	pAngle = angle;
	UpdateRotationMatrix();
}

void meBaseUndoRotate::SetPivot(const decDVector &pivot){
	pPivot = pivot;
	UpdateRotationMatrix();
}

void meBaseUndoRotate::SetAxis(const decDVector &axis){
	pAxis = axis;
	pAxis.Normalize();
	UpdateRotationMatrix();
}

void meBaseUndoRotate::SetModifyPosition(bool modifyPosition){
	pModifyPosition = modifyPosition;
}

void meBaseUndoRotate::SetModifyOrientation(bool modifyOrientation){
	pModifyOrientation = modifyOrientation;
}

void meBaseUndoRotate::UpdateRotationMatrix(){
	pMatrix = decDMatrix::CreateTranslation(-pPivot)
		* decDMatrix::CreateRotationAxis( pAxis, pAngle * DEG2RAD )
		* decDMatrix::CreateTranslation( pPivot );
	
	decString info;
	info.Format("axis(%g,%g,%g) center(%g,%g,%g) angle=%g",
		pAxis.x, pAxis.y, pAxis.z, pPivot.x, pPivot.y, pPivot.z, pAngle);
}

void meBaseUndoRotate::TransformElement(decDVector &position, decDVector &rotation){
	const decDMatrix matrix = decDMatrix::CreateRT(rotation * DEG2RAD, position) * pMatrix;
	
	if(pModifyOrientation){
		decDVector view(matrix.TransformView());
		decDVector up(matrix.TransformUp());
		
		view.Normalize();
		up.Normalize();
		
		rotation = decDMatrix::CreateWorld(decDVector(), view, up).GetEulerAngles() / DEG2RAD;
		
		if(fabs(rotation.x) < 1e-5){
			rotation.x = 0.0;
		}
		if(fabs(rotation.y) < 1e-5){
			rotation.y = 0.0;
		}
		if(fabs(rotation.z) < 1e-5){
			rotation.z = 0.0;
		}
	}
	
	if(pModifyPosition){
		position = matrix.GetPosition();
		
		if(fabs(position.x) < 1e-5){
			position.x = 0.0;
		}
		if(fabs(position.y) < 1e-5){
			position.y = 0.0;
		}
		if(fabs(position.z) < 1e-5){
			position.z = 0.0;
		}
	}
}



// Undo and Redo actions
//////////////////////////

void meBaseUndoRotate::ProgressiveRedo(){
	Undo();
	Redo();
}
