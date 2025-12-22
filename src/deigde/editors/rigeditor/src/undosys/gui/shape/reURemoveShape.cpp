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
#include "../../../rig/shape/reSelectionShapes.h"

#include <dragengine/common/exceptions.h>



// Class reURemoveShape
/////////////////////////

// Constructor, destructor
////////////////////////////

reURemoveShape::reURemoveShape(const reRigShape::List &list){
	DEASSERT_FALSE(list.IsEmpty())
	
	pRig = list.First()->GetRig();
	DEASSERT_NOTNULL(pRig)
	
	list.Visit([&](reRigShape *shape){
		DEASSERT_TRUE(shape->GetRig() == pRig)
		
		const cEntry::Ref entry(cEntry::Ref::New());
		entry->shape = shape;
		entry->bone = shape->GetRigBone();
		pEntries.Add(entry);
	});
	
	SetShortInfo("Remove Shapes");
}

reURemoveShape::~reURemoveShape(){
}



// Management
///////////////

void reURemoveShape::Undo(){
	reSelectionShapes *selection = pRig->GetSelectionShapes();
	
	selection->RemoveAllShapes();
	
	pEntries.Visit([&](const cEntry &entry){
		if(entry.bone){
			entry.bone->AddShape(entry.shape);
			
		}else{
			pRig->AddShape(entry.shape);
		}
		
		selection->AddShape(entry.shape);
	});
}

void reURemoveShape::Redo(){
	reSelectionShapes *selection = pRig->GetSelectionShapes();
	
	pEntries.Visit([&](const cEntry &entry){
		if(entry.shape->GetSelected()){
			selection->RemoveShape(entry.shape);
		}
		
		if(entry.bone){
			entry.bone->RemoveShape(entry.shape);
			
		}else{
			pRig->RemoveShape(entry.shape);
		}
	});
}
