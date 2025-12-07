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

#include "reUMoveShape.h"
#include "reUndoDataShape.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeList.h"

#include <dragengine/common/exceptions.h>



// Class reUMoveShape
///////////////////////

// Constructor, destructor
////////////////////////////

reUMoveShape::reUMoveShape(reRigShapeList &list){
	int shapeCount = list.GetShapeCount();
	
	if(shapeCount == 0) DETHROW(deeInvalidParam);
	
	pShapes = NULL;
	pShapeCount = 0;
	
	try{
		if(shapeCount > 0){
			pShapes = new reUndoDataShape*[shapeCount];
			if(! pShapes) DETHROW(deeOutOfMemory);
			
			while(pShapeCount < shapeCount){
				pShapes[pShapeCount] = new reUndoDataShape(list.GetShapeAt(pShapeCount));
				if(! pShapes[pShapeCount]) DETHROW(deeOutOfMemory);
				pShapeCount++;
			}
		}
		
		if(shapeCount > 1){
			SetShortInfo("Move Shapes");
			
		}else{
			SetShortInfo("Move Shape");
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reUMoveShape::~reUMoveShape(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void reUMoveShape::Undo(){
	reRigShape *shape;
	int s;
	
	for(s=0; s<pShapeCount; s++){
		shape = pShapes[s]->GetShape();
		
		shape->SetPosition(pShapes[s]->GetOldPosition());
	}
}

void reUMoveShape::Redo(){
	reRigShape *shape;
	decMatrix matrix;
	reRigBone *bone;
	int s;
	
	for(s=0; s<pShapeCount; s++){
		shape = pShapes[s]->GetShape();
		bone = shape->GetRigBone();
		
		if(bone){
			matrix = bone->GetPoseMatrix().ToMatrix()
				* decMatrix::CreateTranslation( GetDistance() )
				* bone->GetInversePoseMatrix().ToMatrix();
			
		}else{
			matrix.SetTranslation(GetDistance());
		}
		
		shape->SetPosition(matrix * pShapes[s]->GetOldPosition());
	}
}

void reUMoveShape::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void reUMoveShape::pCleanUp(){
	if(pShapes){
		while(pShapeCount > 0){
			pShapeCount--;
			delete pShapes[pShapeCount];
		}
		
		delete [] pShapes;
	}
}
