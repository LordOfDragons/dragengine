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
#include <string.h>

#include "meUObjDuplicate.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"
#include "../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjDuplicate
//////////////////////////

// Constructor, destructor
////////////////////////////

meUObjDuplicate::meUObjDuplicate(meWorld *world, const decVector &offset) :
pWorld(nullptr)
{
	DEASSERT_NOTNULL(world)
	
	const meObject::List &list = world->GetSelectionObject().GetSelected();
	DEASSERT_TRUE(list.IsNotEmpty())
	
	SetShortInfo("Duplicate Objects");
	
	list.Visit([&](const meObject &object){
		const meObject::Ref duplicate(meObject::Ref::New(world->GetEnvironment()));
		
		duplicate->SetClassName(object.GetClassName());
		duplicate->SetPosition(object.GetPosition() + decDVector(offset));
		duplicate->SetRotation(object.GetRotation());
		duplicate->SetSize(object.GetSize());
		duplicate->SetScaling(object.GetScaling());
		duplicate->SetProperties(object.GetProperties());
		duplicate->SetAttachBehaviors(object.GetAttachBehaviors());
		
		object.GetTextures().Visit([&](const meObjectTexture &t){
			duplicate->AddTexture(meObjectTexture::Ref::New(t));
		});
		
		duplicate->SetID(world->NextObjectID());
		
		pObjects.Add(meUndoDataObject::Ref::New(duplicate));
	});
	
	// update attachment
	list.VisitIndexed([&](int i, const meObject &object){
		if(!object.GetAttachedTo()){
			return;
		}
		
		meUndoDataObject &data = pObjects.GetAt(i);
		list.VisitIndexed([&](int j, meObject *o2){
			if(o2 == object.GetAttachedTo()){
				// attached to a duplicated object
				data.SetAttachedTo(pObjects.GetAt(j)->GetObject());
			}
		});
		
		// attached to a not duplicated object
		if(!data.GetAttachedTo()){
			data.SetAttachedTo(object.GetAttachedTo());
		}
	});
	
	pWorld = world;
}

meUObjDuplicate::~meUObjDuplicate(){
}



// Management
///////////////

void meUObjDuplicate::Undo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	
	pObjects.Visit([&](const meUndoDataObject &data){
		meObject * const object = data.GetObject();
		
		object->SetAttachedTo(nullptr);
		
		selection.Remove(object);
		if(object->GetActive()){
			selection.ActivateNext();
		}
		
		pWorld->RemoveObject(data.GetObject());
		
		pWorld->NotifyObjectRemoved(object);
	});
	
	pWorld->NotifyObjectSelectionChanged();
}

void meUObjDuplicate::Redo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	
	selection.Reset();
	
	pObjects.Visit([&](const meUndoDataObject &data){
		meObject * const object = data.GetObject();
		
		pWorld->AddObject(object);
		selection.Add(object);
		
		pWorld->NotifyObjectAdded(object);
	});
	
	pObjects.Visit([&](meUndoDataObject &data){
		data.GetObject()->SetAttachedTo(data.GetAttachedTo());
	});
	
	selection.ActivateNext();
	
	pWorld->NotifyObjectSelectionChanged();
}
