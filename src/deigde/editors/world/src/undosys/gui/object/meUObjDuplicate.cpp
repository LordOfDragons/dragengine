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
	
	const meObjectList &list = world->GetSelectionObject().GetSelected();
	const int count = list.GetCount();
	DEASSERT_FALSE(count == 0)
	
	int i, j;
	
	SetShortInfo("Duplicate Objects");
	
	try{
		for(i=0; i<count; i++){
			const meObject &object = *list.GetAt(i);
			
			const meObject::Ref duplicate(meObject::Ref::NewWith(world->GetEnvironment()));
			
			duplicate->SetClassName(object.GetClassName());
			duplicate->SetPosition(object.GetPosition() + decDVector(offset));
			duplicate->SetRotation(object.GetRotation());
			duplicate->SetSize(object.GetSize());
			duplicate->SetScaling(object.GetScaling());
			duplicate->SetProperties(object.GetProperties());
			duplicate->SetAttachBehaviors(object.GetAttachBehaviors());
			
			const int textureCount = object.GetTextureCount();
			for(j=0; j<textureCount; j++){
				duplicate->AddTexture(meObjectTexture::Ref::NewWith(*object.GetTextureAt(j)));
			}
			
			duplicate->SetID(world->NextObjectID());
			
			pObjects.Add(meUndoDataObject::Ref::NewWith(duplicate));
		}
		
		// update attachment
		for(i=0; i<count; i++){
			const meObject &object = *list.GetAt(i);
			if(!object.GetAttachedTo()){
				continue;
			}
			
			meUndoDataObject &data = *((meUndoDataObject*)pObjects.GetAt(i));
			for(j=0; j<count; j++){
				if(list.GetAt(j) == object.GetAttachedTo()){
					// attached to a duplicated object
					data.SetAttachedTo(((meUndoDataObject*)pObjects.GetAt(j))->GetObject());
					break;
				}
			}
			
			// attached to a not duplicated object
			if(!data.GetAttachedTo()){
				data.SetAttachedTo(object.GetAttachedTo());
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
}

meUObjDuplicate::~meUObjDuplicate(){
	pCleanUp();
}



// Management
///////////////

void meUObjDuplicate::Undo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const meUndoDataObject &data = *((meUndoDataObject*)pObjects.GetAt(i));
		meObject * const object = data.GetObject();
		
		object->SetAttachedTo(NULL);
		
		selection.Remove(object);
		if(object->GetActive()){
			selection.ActivateNext();
		}
		
		pWorld->RemoveObject(data.GetObject());
		
		pWorld->NotifyObjectRemoved(object);
	}
	
	pWorld->NotifyObjectSelectionChanged();
}

void meUObjDuplicate::Redo(){
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
	}
	
	for(i=0; i<count; i++){
		const meUndoDataObject &data = *((meUndoDataObject*)pObjects.GetAt(i));
		data.GetObject()->SetAttachedTo(data.GetAttachedTo());
	}
	
	selection.ActivateNext();
	
	pWorld->NotifyObjectSelectionChanged();
}



// Private Functions
//////////////////////

void meUObjDuplicate::pCleanUp(){
	if(pWorld) pWorld->FreeReference();
}
