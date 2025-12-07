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

#include "meUObjectCopyRotation.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectCopyRotation
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectCopyRotation::meUObjectCopyRotation(meWorld *world, bool copyX, bool copyY, bool copyZ) :
pCopyX(copyX),
pCopyY(copyY),
pCopyZ(copyZ)
{
	if(! world || ! world->GetSelectionObject().GetActive()){
		DETHROW(deeInvalidParam);
	}
	
	const meObjectList &list = world->GetSelectionObject().GetSelected();
	const int count = list.GetCount();
	int i;
	
	SetShortInfo("Copy Object Rotation");
	SetLongInfo("Copy Object Rotation");
	
	for(i=0; i<count; i++){
		pObjects.Add(meUndoDataObject::Ref::NewWith(list.GetAt(i)));
	}
	
	pNewRotation = world->GetSelectionObject().GetActive()->GetRotation();
}

meUObjectCopyRotation::~meUObjectCopyRotation(){
}



// Management
///////////////

void meUObjectCopyRotation::Undo(){
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const meUndoDataObject &data = *((meUndoDataObject*)pObjects.GetAt(i));
		meObject * const object = data.GetObject();
		
		object->SetRotation(data.GetOldOrientation());
		object->GetWorld()->NotifyObjectGeometryChanged(object);
	}
}

void meUObjectCopyRotation::Redo(){
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const meUndoDataObject &data = *((meUndoDataObject*)pObjects.GetAt(i));
		meObject * const object = data.GetObject();
		
		decVector rotation(data.GetOldOrientation());
		
		if(pCopyX){
			rotation.x = pNewRotation.x;
		}
		if(pCopyY){
			rotation.y = pNewRotation.y;
		}
		if(pCopyZ){
			rotation.z = pNewRotation.z;
		}
		
		object->SetRotation(rotation);
		object->GetWorld()->NotifyObjectGeometryChanged(object);
	}
}
