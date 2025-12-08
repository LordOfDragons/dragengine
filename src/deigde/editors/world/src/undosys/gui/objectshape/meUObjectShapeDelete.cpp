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

#include "meUObjectShapeDelete.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectShapesDelete
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectShapesDelete::meUObjectShapesDelete(meObject *object, const char *property, const meObjectShapeList &list){
	if(list.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	if(!object || !property){
		DETHROW(deeInvalidParam);
	}
	if(!object->GetWorld()){
		DETHROW(deeInvalidParam);
	}
	
	const meObjectShapeList &shapeList1 = object->GetWorld()->GetObjectShapes();
	const int count = shapeList1.GetCount();
	meObjectShapeList shapeList2;
	int i;
	
	pObject = NULL;
	
	SetShortInfo("Object-Shape delete");
	SetLongInfo("Object-Shape delete");
	
	pPropertyExists = object->GetProperties().Has(property);
	if(pPropertyExists){
		pOldValue = object->GetProperties().GetAt(property);
	}
	
	for(i=0; i<count; i++){
		if(!list.Has(shapeList1.GetAt(i))){
			shapeList2.Add(shapeList1.GetAt(i));
		}
	}
	
	shapeList2.CreatePropertyString(pNewValue);
	shapeList2.RemoveAll();
	
	pProperty = property;
	pObject = object;
}

meUObjectShapesDelete::~meUObjectShapesDelete(){
}



// Management
///////////////

void meUObjectShapesDelete::Undo(){
	meWorld &world = *pObject->GetWorld();
	
	if(pPropertyExists){
		pObject->SetProperty(pProperty.GetString(), pOldValue.GetString());
		
	}else{
		pObject->RemoveProperty(pProperty.GetString());
	}
	
	world.GetSelectionObjectShape().Reset();
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapesDelete::Redo(){
	meWorld &world = *pObject->GetWorld();
	
	pObject->SetProperty(pProperty.GetString(), pNewValue.GetString());
	
	world.GetSelectionObjectShape().Reset();
	world.NotifyObjectShapeSelectionChanged();
}
