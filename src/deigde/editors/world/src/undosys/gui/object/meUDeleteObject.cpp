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

#include "meUDeleteObject.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUDeleteObject
//////////////////////////

// Constructor, destructor
////////////////////////////

meUDeleteObject::meUDeleteObject(meWorld *world) :
pWorld(NULL)
{
	if(! world){
		DETHROW(deeInvalidParam);
	}
	
	const meObjectList &list = world->GetSelectionObject().GetSelected();
	const int count = list.GetCount();
	decString text;
	int i;
	
	SetShortInfo("Delete Objects");
	if(count > 1){
		text.Format("%i objects", count);
		
	}else{
		text = "1 object";
	}
	SetLongInfo(text);
	
	try{
		pWorld = world;
		world->AddReference();
		
		for(i=0; i<count; i++){
			pObjects.Add(meUndoDataObject::Ref::NewWith(list.GetAt(i)));
		}
		
		// keep track of object attached to deleted objects
		const meObjectList &allObjects = world->GetObjects();
		const int allCount = allObjects.GetCount();
		for(i=0; i<allCount; i++){
			meObject * const object = allObjects.GetAt(i);
			if(object->GetAttachedTo() && list.Has(object->GetAttachedTo())){
				pAttached.Add(meUndoDataObject::Ref::NewWith(object));
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUDeleteObject::~meUDeleteObject(){
	pCleanUp();
}



// Management
///////////////

void meUDeleteObject::Undo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	const int count = pObjects.GetCount();
	int i;
	
	selection.Reset();
	
	for(i=0; i<count; i++){
		const meUndoDataObject &data = *((meUndoDataObject*)pObjects.GetAt(i));
		meObject * const object = data.GetObject();
		
		pWorld->AddObject(object);
		selection.Add(object);
		
		pWorld->NotifyObjectAdded(object);
		
		object->SetAttachedTo(data.GetAttachedTo());
	}
	
	const int attachCount = pAttached.GetCount();
	for(i=0; i<attachCount; i++){
		const meUndoDataObject &data = *((meUndoDataObject*)pAttached.GetAt(i));
		meObject * const object = data.GetObject();
		object->SetAttachedTo(data.GetAttachedTo());
	}
	
	selection.ActivateNext();
	
	pWorld->NotifyObjectSelectionChanged();
}

void meUDeleteObject::Redo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	int i;
	
	const int attachCount = pAttached.GetCount();
	for(i=0; i<attachCount; i++){
		((meUndoDataObject*)pAttached.GetAt(i))->GetObject()->SetAttachedTo(NULL);
	}
	
	const int count = pObjects.GetCount();
	for(i=0; i<count; i++){
		const meUndoDataObject &data = *((meUndoDataObject*)pObjects.GetAt(i));
		meObject * const object = data.GetObject();
		
		object->SetAttachedTo(NULL);
			// done automatically if removed from world but better be safe
		
		selection.Remove(object);
		if(object->GetActive()){
			selection.ActivateNext();
		}
		
		pWorld->RemoveObject(object);
		
		pWorld->NotifyObjectRemoved(object);
	}
	
	pWorld->NotifyObjectSelectionChanged();
}



// Private Functions
//////////////////////

void meUDeleteObject::pCleanUp(){
	if(pWorld){
		pWorld->FreeReference();
	}
}
