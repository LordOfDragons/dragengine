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

#include "meUMoveObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"
#include "../../../worldedit.h"
#include <dragengine/common/exceptions.h>



// Class meUMoveObject
////////////////////////

// Constructor, destructor
////////////////////////////

meUMoveObject::meUMoveObject(meWorld *world, const meObject::List &objects){
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Move Object");
	
	pWorld = world;
	
	meUndoDataObject::AddObjectsWithAttachments(objects, pObjects);
}

meUMoveObject::~meUMoveObject(){
}



// Undo and Redo operations
/////////////////////////////

void meUMoveObject::Undo(){
	meUndoDataObject::RestoreOldGeometry(pObjects, pWorld);
}

void meUMoveObject::Redo(){
	if(GetModifyOrientation()){
		pObjects.Visit([&](const meUndoDataObject &data){
			meObject * const object = data.GetObject();
			
			decDVector position(data.GetOldPosition());
			decDVector rotation(data.GetOldRotation());
			
			TransformElement(position, rotation);
			
			object->SetRotation(rotation.ToVector());
			object->SetPosition(position);
			pWorld->NotifyObjectGeometryChanged(object);
			
			if(data.GetAttachedObjects().IsNotEmpty()){
				const decDMatrix m(data.GetOldMatrixInverse() * object->GetObjectMatrix());
				
				data.GetAttachedObjects().Visit([&](const meUndoDataObject &attachedData){
					const decDMatrix m2(attachedData.GetOldMatrix() * m);
					meObject * const attached = attachedData.GetObject();
					attached->SetPosition(m2.GetPosition());
					attached->SetRotation(m2.Normalized().GetEulerAngles().ToVector() * RAD2DEG);
					pWorld->NotifyObjectGeometryChanged(attached);
				});
			}
		});
		
	}else{
		const decDVector &distance = GetDistance();
		
		pObjects.Visit([&](const meUndoDataObject &data){
			meObject * const object = data.GetObject();
			object->SetPosition(data.GetOldPosition() + distance);
			object->SetRotation(data.GetOldRotation());
			pWorld->NotifyObjectGeometryChanged(object);
			
			data.GetAttachedObjects().Visit([&](const meUndoDataObject &attachedData){
				meObject * const attachedObject = attachedData.GetObject();
				attachedObject->SetPosition(attachedData.GetOldPosition() + distance);
				attachedObject->SetRotation(attachedData.GetOldRotation());
				pWorld->NotifyObjectGeometryChanged(attachedObject);
			});
		});
	}
}

void meUMoveObject::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}
