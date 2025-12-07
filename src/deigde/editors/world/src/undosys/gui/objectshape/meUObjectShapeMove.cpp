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

#include "meUObjectShapeMove.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <deigde/codec/igdeCodecPropertyString.h>



// Class meUObjectShapeMove
/////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectShapeMove::meUObjectShapeMove(meObject *object, const char *property, const meObjectShapeList &list){
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
	
	SetShortInfo("Move object shapes");
	SetLongInfo("Move object shapes");
	
	pPropertyExists = object->GetProperties().Has(property);
	if(pPropertyExists){
		pOldValue = object->GetProperties().GetAt(property);
	}
	
	for(i=0; i<count; i++){
		pList.Add(shapeList.IndexOf(list.GetAt(i)));
	}
	
	pProperty = property;
	pObject = object;
	object->AddReference();
}

meUObjectShapeMove::~meUObjectShapeMove(){
	if(pObject){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUObjectShapeMove::Undo(){
	meWorld &world = *pObject->GetWorld();
	
	if(pPropertyExists){
		pObject->SetProperty(pProperty.GetString(), pOldValue.GetString());
		
	}else{
		pObject->RemoveProperty(pProperty.GetString());
	}
	
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeMove::Redo(){
	meWorld &world = *pObject->GetWorld();
	const int count = pList.GetCount();
	decDMatrix matrixParentInverse;
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	decVector distance;
	decString newValue;
	int i;
	
	matrixParentInverse = decDMatrix::CreateRotation(decDVector(pObject->GetRotation() * DEG2RAD)).Invert();
	distance = (matrixParentInverse * GetDistance()).ToVector();
	
	codec.DecodeShapeList(pOldValue.GetString(), shapeList);
	
	for(i=0; i<count; i++){
		decShape &shape = *shapeList.GetAt(pList.GetAt(i));
		
		shape.SetPosition(shape.GetPosition() + distance);
	}
	
	codec.EncodeShapeList(shapeList, newValue);
	
	pObject->SetProperty(pProperty.GetString(), newValue.GetString());
	
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeMove::ProgressiveRedo(){
	Redo();
}
