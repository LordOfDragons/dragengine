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

#include "meUScaleObject.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <deigde/gamedefinition/class/igdeGDClass.h>

#include <dragengine/common/exceptions.h>



// Class meUScaleObject
/////////////////////////

// Constructor, destructor
////////////////////////////

meUScaleObject::meUScaleObject(meWorld *world, const meObject::List &objects){
	DEASSERT_NOTNULL(world)
	
	SetShortInfo("Scale Object");
	
	pWorld = world;
	
	meUndoDataObject::AddObjectsWithAttachments(objects, pObjects);
}

meUScaleObject::~meUScaleObject(){
}



// Undo and Redo operations
/////////////////////////////

void meUScaleObject::Undo(){
	meUndoDataObject::RestoreOldGeometry(pObjects, pWorld);
}

void meUScaleObject::Redo(){
	const decVector &factors = GetFactors();
	const decDVector &pivot = GetPivot();
	bool modifyPosition = GetModifyPosition();
	float uniformFactor = GetUniformFactor();
	bool modifySize = GetModifySize();
	
	pObjects.Visit([&](const meUndoDataObject &data){
		meObject * const object = data.GetObject();
		
		if(modifySize){
			const decVector &size = data.GetOldSize();
			switch(object->GetScaleMode()){
			case igdeGDClass::esmUniform:
				object->SetSize(size * uniformFactor);
				break;
				
			case igdeGDClass::esmFree:
				object->SetSize(decVector(size.x * factors.x, size.y * factors.y, size.z * factors.z));
				break;
				
			default:
				break;
			}
		}
		
		if(modifyPosition){
			decDVector position(data.GetOldPosition() - pivot);
			position.x = pivot.x + position.x * (double)factors.x;
			position.y = pivot.y + position.y * (double)factors.y;
			position.z = pivot.z + position.z * (double)factors.z;
			object->SetPosition(position);
		}
		
		pWorld->NotifyObjectGeometryChanged(object);
		
		if(data.GetAttachedObjects().IsNotEmpty()){
			const decDMatrix m(data.GetOldMatrixInverse() * object->GetObjectMatrix());
			data.GetAttachedObjects().Visit([&](const meUndoDataObject &attachedData){
				const decDMatrix m2(attachedData.GetOldMatrix() * m);
				meObject * const attached = attachedData.GetObject();
				
				if(modifyPosition){
					attached->SetPosition(m2.GetPosition());
				}
				
				if(modifySize){
					const decVector scaling(m2.GetScale());
					switch(object->GetScaleMode()){
					case igdeGDClass::esmUniform:
						attached->SetScaling(decVector(scaling.y, scaling.y, scaling.y));
						break;
						
					case igdeGDClass::esmFree:
						attached->SetScaling(scaling);
						break;
						
					default:
						break;
					}
				}
				
				pWorld->NotifyObjectGeometryChanged(attached);
			});
		}
	});
}

void meUScaleObject::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}
