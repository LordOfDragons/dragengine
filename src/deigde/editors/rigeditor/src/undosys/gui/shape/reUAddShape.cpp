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

#include "reUAddShape.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reSelectionShapes.h"

#include <dragengine/common/exceptions.h>



// Class reUAddShape
//////////////////////

// Constructor, destructor
////////////////////////////

reUAddShape::reUAddShape(reRig *rig, reRigBone *bone, reRigShape *shape){
	if(!shape || (!rig && !bone)) DETHROW(deeInvalidParam);
	
	pRig = rig;
	if(rig) rig->AddReference();
	
	pBone = bone;
	if(bone) bone->AddReference();
	
	pShape = shape;
	shape->AddReference();
	
	try{
		if(bone){
			SetShortInfo("Add Bone Shape");
			
		}else{
			SetShortInfo("Add Rig Shape");
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reUAddShape::~reUAddShape(){
	pCleanUp();
}



// Management
///////////////

void reUAddShape::Undo(){
	reRig *rig = pGetRig();
	
	reSelectionShapes *selection = rig->GetSelectionShapes();
	
	if(pShape->GetSelected()){
		selection->RemoveShape(pShape);
	}
	
	if(pBone){
		pBone->RemoveShape(pShape);
		
	}else{
		rig->RemoveShape(pShape);
	}
}

void reUAddShape::Redo(){
	reRig *rig = pGetRig();
	
	reSelectionShapes *selection = rig->GetSelectionShapes();
	
	if(pBone){
		pBone->AddShape(pShape);
		
	}else{
		rig->AddShape(pShape);
	}
	
	selection->RemoveAllShapes();
	selection->AddShape(pShape);
}



// Private Functions
//////////////////////

void reUAddShape::pCleanUp(){
	if(pShape) pShape->FreeReference();
	if(pBone) pBone->FreeReference();
	if(pRig) pRig->FreeReference();
}

reRig *reUAddShape::pGetRig(){
	if(pRig) return pRig;
	
	reRig *rig = pBone->GetRig();
	if(!rig) DETHROW(deeInvalidParam);
	
	return rig;
}
