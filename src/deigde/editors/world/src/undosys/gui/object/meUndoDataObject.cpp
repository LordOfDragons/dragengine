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

#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUndoDataObject
///////////////////////////

// Constructor, destructor
////////////////////////////

meUndoDataObject::meUndoDataObject(meObject *object) :
pObject(object)
{
	DEASSERT_NOTNULL(object)
	
	pNewPosition = pOldPosition = object->GetPosition();
	pNewRotation = pOldRotation = object->GetRotation();
	pNewSize = pOldSize = object->GetSize();
	pNewScaling = pOldScaling = object->GetScaling();
	pAttachedTo = object->GetAttachedTo();
}

meUndoDataObject::~meUndoDataObject(){
}



// Management
///////////////

void meUndoDataObject::SetNewPosition(const decDVector &position){
	pNewPosition = position;
}

void meUndoDataObject::SetNewRotation(const decVector &rotation){
	pNewRotation = rotation;
}

void meUndoDataObject::SetNewSize(const decVector &size){
	pNewSize = size;
}

void meUndoDataObject::SetNewScaling(const decVector &scaling){
	pNewScaling = scaling;
}

void meUndoDataObject::TransformNew(const decDMatrix &matrix){
	const decDMatrix m(GetNewMatrix() * matrix);
	
	pNewPosition = m.GetPosition();
	pNewRotation = m.Normalized().GetEulerAngles().ToVector() * RAD2DEG;
	pNewScaling = m.GetScale();
	
	const decVector scale(m.GetScale());
	pNewSize.x = pOldSize.x * (scale.x / pOldScaling.x);
	pNewSize.y = pOldSize.y * (scale.y / pOldScaling.y);
	pNewSize.z = pOldSize.z * (scale.z / pOldScaling.z);
}

decDMatrix meUndoDataObject::GetOldMatrix() const{
	return decDMatrix::CreateSRT(pOldScaling, pOldRotation * DEG2RAD, pOldPosition);
}

decDMatrix meUndoDataObject::GetOldMatrixInverse() const{
	return GetOldMatrix().Invert();
}

decDMatrix meUndoDataObject::GetNewMatrix() const{
	return decDMatrix::CreateSRT(pNewScaling, pNewRotation * DEG2RAD, pNewPosition);
}

decDMatrix meUndoDataObject::GetNewMatrixInverse() const{
	return GetNewMatrix().Invert();
}

void meUndoDataObject::SetAttachedTo(meObject *object){
	pAttachedTo = object;
}



void meUndoDataObject::AddObjectsWithAttachments(const meObject::List &objects, List &list){
	objects.Visit([&](meObject *o){
		const meUndoDataObject::Ref &data(meUndoDataObject::Ref::New(o));
		
		o->GetAllAttachedObjects().Visit([&](meObject *attached){
			if(!objects.Has(attached)){
				data->pAttachedObjects.Add(meUndoDataObject::Ref::New(attached));
			}
		});
		
		list.Add(data);
	});
}

void meUndoDataObject::RestoreOldGeometry(const meUndoDataObject::List &list, meWorld &world){
	list.Visit([&](const meUndoDataObject &data){
		data.pObject->SetPosition(data.pOldPosition);
		data.pObject->SetRotation(data.pOldRotation);
		data.pObject->SetSize(data.pOldSize);
		world.NotifyObjectGeometryChanged(data.pObject);
		
		data.pAttachedObjects.Visit([&](const meUndoDataObject &attachedData){
			attachedData.pObject->SetPosition(attachedData.pOldPosition);
			attachedData.pObject->SetRotation(attachedData.pOldRotation);
			attachedData.pObject->SetSize(attachedData.pOldSize);
			world.NotifyObjectGeometryChanged(attachedData.pObject);
		});
	});
}

void meUndoDataObject::ApplyNewGeometry(const meUndoDataObject::List &list, meWorld &world){
	list.Visit([&](const meUndoDataObject &data){
		data.pObject->SetPosition(data.pNewPosition);
		data.pObject->SetRotation(data.pNewRotation);
		data.pObject->SetSize(data.pNewSize);
		world.NotifyObjectGeometryChanged(data.pObject);
		
		if(data.pAttachedObjects.IsNotEmpty()){
			const decDMatrix m(data.GetOldMatrixInverse() * data.pObject->GetObjectMatrix());
			
			data.pAttachedObjects.Visit([&](const meUndoDataObject &attachedData){
				const decDMatrix m2(attachedData.GetOldMatrix() * m);
				attachedData.pObject->SetPosition(m2.GetPosition());
				attachedData.pObject->SetRotation(m2.Normalized().GetEulerAngles().ToVector() * RAD2DEG);
				world.NotifyObjectGeometryChanged(attachedData.pObject);
			});
		}
	});
}
