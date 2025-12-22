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

#include "reURotateShape.h"
#include "../../../rig/bone/reRigBone.h"

#include <dragengine/common/exceptions.h>



// Class reURotateShape
/////////////////////////

// Constructor, destructor
////////////////////////////

reURotateShape::reURotateShape(const reRigShape::List &list){
	DEASSERT_TRUE(list.IsNotEmpty())
	
	list.Visit([&](reRigShape *s){
		pShapes.Add(reUndoDataShape::Ref::New(s));
	});
	
	if(list.GetCount() > 1){
		SetShortInfo("Rotate Shapes");
		
	}else{
		SetShortInfo("Rotate Shape");
	}
}

reURotateShape::~reURotateShape(){
}



// Undo and Redo operations
/////////////////////////////

void reURotateShape::Undo(){
	pShapes.Visit([&](const reUndoDataShape &d){
		reRigShape &shape = d.GetShape();
		shape.SetPosition(d.GetOldPosition());
		shape.SetOrientation(d.GetOldOrientation());
	});
}

void reURotateShape::Redo(){
	const bool modifyOrientation = GetModifyOrientation();
	const decMatrix rotationMatrix = GetRotationMatrix();
	const bool modifyPosition = GetModifyPosition();
	
	pShapes.Visit([&](const reUndoDataShape &d){
		reRigShape &shape = d.GetShape();
		reRigBone * const bone = shape.GetRigBone();
		
		// build matrix
		decMatrix matrix(decMatrix::CreateRT(d.GetOldOrientation() * DEG2RAD, d.GetOldPosition()));
		
		if(bone){
			matrix = matrix * bone->GetPoseMatrix().ToMatrix() * rotationMatrix * bone->GetInversePoseMatrix().ToMatrix();
			
		}else{
			matrix *= rotationMatrix;
		}
		
		// modify orientation
		if(modifyOrientation){
			decVector view(matrix.TransformView());
			decVector up(matrix.TransformUp());
			view.Normalize();
			up.Normalize();
			decVector rotation(decMatrix::CreateWorld(decVector(), view, up).GetEulerAngles());
			
			if(fabs(rotation.x) < 1e-5f){
				rotation.x = 0.0f;
			}
			if(fabs(rotation.y) < 1e-5f){
				rotation.y = 0.0f;
			}
			if(fabs(rotation.z) < 1e-5f){
				rotation.z = 0.0f;
			}
			
			shape.SetOrientation(rotation * RAD2DEG);
		}
		
		// modify position
		if(modifyPosition){
			decVector position(matrix.GetPosition());
			
			if(fabs(position.x) < 1e-5f){
				position.x = 0.0f;
			}
			if(fabs(position.y) < 1e-5f){
				position.y = 0.0f;
			}
			if(fabs(position.z) < 1e-5f){
				position.z = 0.0f;
			}
			
			shape.SetPosition(position);
		}
	});
}

void reURotateShape::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}
