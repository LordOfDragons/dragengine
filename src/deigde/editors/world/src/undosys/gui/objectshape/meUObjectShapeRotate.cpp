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

#include "meUObjectShapeRotate.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <deigde/codec/igdeCodecPropertyString.h>



// Class meUObjectShapeRotate
//////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectShapeRotate::meUObjectShapeRotate(meObject *object, const char *property,
const meObjectShapeList &list){
	if(list.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	if(!object || !property){
		DETHROW(deeInvalidParam);
	}
	if(!object->GetWorld()){
		DETHROW(deeInvalidParam);
	}
	
	const meObjectShapeList &shapeList = object->GetWorld()->GetObjectShapes();
	const int count = list.GetCount();
	int i;
	
	pObject = NULL;
	
	SetShortInfo("Rotate object shapes");
	SetLongInfo("Rotate object shapes");
	
	pPropertyExists = object->GetProperties().Has(property);
	if(pPropertyExists){
		pOldValue = object->GetProperties().GetAt(property);
	}
	
	for(i=0; i<count; i++){
		pList.Add(shapeList.IndexOf(list.GetAt(i)));
	}
	
	pProperty = property;
	pObject = object;
}

meUObjectShapeRotate::~meUObjectShapeRotate(){
}



// Undo and Redo operations
/////////////////////////////

void meUObjectShapeRotate::Undo(){
	meWorld &world = *pObject->GetWorld();
	
	if(pPropertyExists){
		pObject->SetProperty(pProperty.GetString(), pOldValue.GetString());
		
	}else{
		pObject->RemoveProperty(pProperty.GetString());
	}
	
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeRotate::Redo(){
	const bool modifyPosition = GetModifyPosition();
	const bool modifyOrientation = GetModifyOrientation();
	meWorld &world = *pObject->GetWorld();
	decDMatrix matrixParent, matrixParentInverse, matrixShape;
	decDVector shapePosition, shapeRotation;
	const int count = pList.GetCount();
	igdeCodecPropertyString codec;
	decDVector position, rotation;
	decShapeList shapeList;
	decString newValue;
	int i;
	
	matrixParent = decDMatrix::CreateRT(decDVector(pObject->GetRotation() * DEG2RAD), pObject->GetPosition());
	matrixParentInverse = matrixParent.Invert();
	
	codec.DecodeShapeList(pOldValue.GetString(), shapeList);
	
	for(i=0; i<count; i++){
		decShape &shape = *shapeList.GetAt(pList.GetAt(i));
		
		matrixShape = decDMatrix::CreateWorld(decDVector(shape.GetPosition()), shape.GetOrientation()) * matrixParent;
		shapePosition = matrixShape.GetPosition();
		shapeRotation = matrixShape.GetEulerAngles();
		
		position = shapePosition;
		rotation = shapeRotation * RAD2DEG;
		TransformElement(position, rotation);
		
		if(modifyOrientation){
			shapeRotation = rotation * DEG2RAD;
		}
		if(modifyPosition){
			shapePosition = position;
		}
		
		matrixShape = decDMatrix::CreateRT(shapeRotation, shapePosition) * matrixParentInverse;
		shape.SetPosition(matrixShape.GetPosition().ToVector());
		shape.SetOrientation(matrixShape.ToQuaternion());
	}
	
	codec.EncodeShapeList(shapeList, newValue);
	
	pObject->SetProperty(pProperty.GetString(), newValue.GetString());
	
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeRotate::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}
