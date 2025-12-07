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

#include "meUObjectCopyPosition.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectCopyPosition
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectCopyPosition::meUObjectCopyPosition(meWorld *world, bool copyX, bool copyY, bool copyZ) :
pCopyX(copyX),
pCopyY(copyY),
pCopyZ(copyZ)
{
	if(!world || !world->GetSelectionObject().GetActive()){
		DETHROW(deeInvalidParam);
	}
	
	const meObjectList &list = world->GetSelectionObject().GetSelected();
	const int count = list.GetCount();
	int i;
	
	SetShortInfo("Copy Object Position");
	SetLongInfo("Copy Object Position");
	
	for(i=0; i<count; i++){
		pObjects.Add(meUndoDataObject::Ref::NewWith(list.GetAt(i)));
	}
	
	pNewPosition = world->GetSelectionObject().GetActive()->GetPosition();
}

meUObjectCopyPosition::~meUObjectCopyPosition(){
}



// Management
///////////////

void meUObjectCopyPosition::Undo(){
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const meUndoDataObject &data = *((meUndoDataObject*)pObjects.GetAt(i));
		meObject * const object = data.GetObject();
		
		object->SetPosition(data.GetOldPosition());
		object->GetWorld()->NotifyObjectGeometryChanged(object);
	}
}

void meUObjectCopyPosition::Redo(){
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const meUndoDataObject &data = *((meUndoDataObject*)pObjects.GetAt(i));
		meObject * const object = data.GetObject();
		
		decDVector position(data.GetOldPosition());
		
		if(pCopyX){
			position.x = pNewPosition.x;
		}
		if(pCopyY){
			position.y = pNewPosition.y;
		}
		if(pCopyZ){
			position.z = pNewPosition.z;
		}
		
		object->SetPosition(position);
		object->GetWorld()->NotifyObjectGeometryChanged(object);
	}
}
