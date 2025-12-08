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

#include "reURemoveShape.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeList.h"
#include "../../../rig/shape/reSelectionShapes.h"

#include <dragengine/common/exceptions.h>



// Class reURemoveShape
/////////////////////////

// Constructor, destructor
////////////////////////////

reURemoveShape::reURemoveShape(reRigShapeList &list){
	int shapeCount = list.GetShapeCount();
	reRigShape *shape;
	reRigBone *bone;
	
	if(shapeCount == 0) DETHROW(deeInvalidParam);
	
	pRig = list.GetShapeAt(0)->GetRig();
	if(!pRig) DETHROW(deeInvalidParam);
	
	pEntries = NULL;
	pEntryCount = 0;
	
	try{
		pEntries = new sEntry[shapeCount];
		if(!pEntries) DETHROW(deeOutOfMemory);
		
		while(pEntryCount < shapeCount){
			shape = list.GetShapeAt(pEntryCount);
			bone = shape->GetRigBone();
			
			if(shape->GetRig() != pRig) DETHROW(deeInvalidParam);
			
			pEntries[pEntryCount].shape = shape;
			
			pEntries[pEntryCount].bone = bone;
			if(bone) bone->AddReference();
			
			pEntryCount++;
		}
		
		SetShortInfo("Remove Shapes");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reURemoveShape::~reURemoveShape(){
	pCleanUp();
}



// Management
///////////////

void reURemoveShape::Undo(){
	reSelectionShapes *selection = pRig->GetSelectionShapes();
	int e;
	
	selection->RemoveAllShapes();
	
	for(e=0; e<pEntryCount; e++){
		if(pEntries[e].bone){
			pEntries[e].bone->AddShape(pEntries[e].shape);
			
		}else{
			pRig->AddShape(pEntries[e].shape);
		}
		
		selection->AddShape(pEntries[e].shape);
	}
	
}

void reURemoveShape::Redo(){
	reSelectionShapes *selection = pRig->GetSelectionShapes();
	int e;
	
	for(e=0; e<pEntryCount; e++){
		if(pEntries[e].shape->GetSelected()){
			selection->RemoveShape(pEntries[e].shape);
		}
		
		if(pEntries[e].bone){
			pEntries[e].bone->RemoveShape(pEntries[e].shape);
			
		}else{
			pRig->RemoveShape(pEntries[e].shape);
		}
	}
}



// Private Functions
//////////////////////

void reURemoveShape::pCleanUp(){
	reRigBone *bone;
	
	if(pEntries){
		while(pEntryCount > 0){
			pEntryCount--;
			
			bone = pEntries[pEntryCount].bone;
			
			if(bone) bone->FreeReference();
			pEntries[pEntryCount].shape->FreeReference();
		}
		
		delete [] pEntries;
	}
	
}
