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

#include "meUCameraRotateObject.h"
#include "../../../world/meCamera.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../worldedit.h"

#include <dragengine/common/exceptions.h>


// Class meUCameraRotateObject
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUCameraRotateObject::meUCameraRotateObject(meObject *object){
	DEASSERT_NOTNULL(object)
	
	pOldRotation = object->GetRotation();
	pNewRotation = pOldRotation;
	SetShortInfo("Rotate Camera Object");
	
	pObject = object;
	object->AddReference();
}

meUCameraRotateObject::~meUCameraRotateObject(){
	if(pObject){
		pObject->FreeReference();
	}
}



// Undo and Redo operations
/////////////////////////////

void meUCameraRotateObject::SetNewRotation(const decVector &rotation){
	pNewRotation = rotation;
}

bool meUCameraRotateObject::HasChanged() const{
	return ! pNewRotation.IsEqualTo(pOldRotation);
}

void meUCameraRotateObject::Undo(){
	pObject->SetRotation(pOldRotation);
	pObject->GetWorld()->NotifyObjectGeometryChanged(pObject);
}

void meUCameraRotateObject::Redo(){
	pObject->SetRotation(pNewRotation);
	pObject->GetWorld()->NotifyObjectGeometryChanged(pObject);
}
