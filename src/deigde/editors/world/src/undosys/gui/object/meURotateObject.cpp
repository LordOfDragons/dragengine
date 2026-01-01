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

#include "meURotateObject.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"
#include <dragengine/common/exceptions.h>



// Class meURotateObject
//////////////////////////

// Constructor, destructor
////////////////////////////

meURotateObject::meURotateObject(meWorld *world, const meObject::List &objects){
	DEASSERT_NOTNULL(world)
	
	SetShortInfo("Rotate Object");
	
	pWorld = world;
	
	meUndoDataObject::AddObjectsWithAttachments(objects, pObjects);
}

meURotateObject::~meURotateObject(){
}



// Undo and Redo operations
/////////////////////////////

void meURotateObject::Undo(){
	meUndoDataObject::RestoreOldGeometry(pObjects, pWorld);
}

void meURotateObject::Redo(){
	bool modifyPosition = GetModifyPosition();
	bool modifyOrientation = GetModifyOrientation();
	decDVector position, rotation;
	
	pObjects.Visit([&](const meUndoDataObject &data){
		meObject * const object = data.GetObject();
		
		position = data.GetOldPosition();
		rotation = data.GetOldRotation();
		
		TransformElement(position, rotation);
		
		object->SetRotation(modifyOrientation ? rotation.ToVector() : data.GetOldRotation());
		object->SetPosition(modifyPosition ? position : data.GetOldPosition());
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
}

void meURotateObject::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}
