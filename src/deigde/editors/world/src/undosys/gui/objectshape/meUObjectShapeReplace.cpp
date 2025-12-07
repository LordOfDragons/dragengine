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

#include "meUObjectShapeReplace.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>

#include <deigde/codec/igdeCodecPropertyString.h>



// Class meUObjectShapeReplace
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectShapeReplace::meUObjectShapeReplace(meObject *object, const char *property, int shapeIndex, const decShape &shape){
	if(!object || !property){
		DETHROW(deeInvalidParam);
	}
	if(!object->GetWorld()){
		DETHROW(deeInvalidParam);
	}
	
	igdeCodecPropertyString codec;
	decShape *copyShape = NULL;
	decShapeList shapeList;
	
	pObject = NULL;
	
	SetShortInfo("Object-Shape replace");
	SetLongInfo("Object-Shape replace");
	
	pPropertyExists = object->GetProperties().Has(property);
	if(pPropertyExists){
		pOldValue = object->GetProperties().GetAt(property);
	}
	
	codec.DecodeShapeList(pOldValue.GetString(), shapeList);
	
	try{
		copyShape = shape.Copy();
		shapeList.SetAt(shapeIndex, copyShape);
		
	}catch(const deException &){
		if(copyShape){
			delete copyShape;
		}
		throw;
	}
	
	codec.EncodeShapeList(shapeList, pNewValue);
	
	pProperty = property;
	pObject = object;
	object->AddReference();
}

meUObjectShapeReplace::~meUObjectShapeReplace(){
}



// Management
///////////////

void meUObjectShapeReplace::Undo(){
	if(pPropertyExists){
		pObject->SetProperty(pProperty.GetString(), pOldValue.GetString());
		
	}else{
		pObject->RemoveProperty(pProperty.GetString());
	}
}

void meUObjectShapeReplace::Redo(){
	pObject->SetProperty(pProperty.GetString(), pNewValue.GetString());
}
