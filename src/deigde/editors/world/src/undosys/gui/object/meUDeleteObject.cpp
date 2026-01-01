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
pWorld(nullptr)
{
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	pWorld = world;
	
	const meObject::List &list = world->GetSelectionObject().GetSelected();
	list.Visit([&](meObject *o){
		pObjects.Add(meUndoDataObject::Ref::New(o));
	});
	
	// keep track of object attached to deleted objects
	world->GetObjects().Visit([&](meObject *o){
		if(o->GetAttachedTo() && list.Has(o->GetAttachedTo())){
			pAttached.Add(meUndoDataObject::Ref::New(o));
		}
	});
	
	SetShortInfo("Delete Objects");
	decString text;
	if(pObjects.GetCount() > 1){
		text.Format("%i objects", pObjects.GetCount());
		
	}else{
		text = "1 object";
	}
	SetLongInfo(text);
}

meUDeleteObject::~meUDeleteObject(){
}



// Management
///////////////

void meUDeleteObject::Undo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	
	selection.Reset();
	
	pObjects.Visit([&](const meUndoDataObject &data){
		meObject * const object = data.GetObject();
		
		pWorld->AddObject(object);
		selection.Add(object);
		
		pWorld->NotifyObjectAdded(object);
		
		object->SetAttachedTo(data.GetAttachedTo());
	});
	
	pAttached.Visit([&](const meUndoDataObject &data){
		data.GetObject()->SetAttachedTo(data.GetAttachedTo());
	});
	
	selection.ActivateNext();
	
	pWorld->NotifyObjectSelectionChanged();
}

void meUDeleteObject::Redo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	
	pAttached.Visit([&](const meUndoDataObject &data){
		data.GetObject()->SetAttachedTo(nullptr);
	});
	
	pObjects.Visit([&](const meUndoDataObject &data){
		meObject * const object = data.GetObject();
		
		object->SetAttachedTo(nullptr);
			// done automatically if removed from world but better be safe
		
		selection.Remove(object);
		if(object->GetActive()){
			selection.ActivateNext();
		}
		
		pWorld->RemoveObject(object);
		
		pWorld->NotifyObjectRemoved(object);
	});
	
	pWorld->NotifyObjectSelectionChanged();
}
