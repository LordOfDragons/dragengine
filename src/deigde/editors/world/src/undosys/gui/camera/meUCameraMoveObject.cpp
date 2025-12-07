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

#include "meUCameraMoveObject.h"
#include "../../../world/meCamera.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>


// Class meUCameraMoveObject
//////////////////////////////

// Constructor, destructor
////////////////////////////

meUCameraMoveObject::meUCameraMoveObject(meObject *object){
	DEASSERT_NOTNULL(object)
	
	pNewPosition = pOldPosition = object->GetPosition();
	SetShortInfo("Move Camera Object");
	
	pObject = object;
	object->AddReference();
}

meUCameraMoveObject::~meUCameraMoveObject(){
}



// Undo and Redo operations
/////////////////////////////

void meUCameraMoveObject::SetNewPosition(const decDVector &position){
	pNewPosition = position;
}

bool meUCameraMoveObject::HasChanged() const{
	return !pNewPosition.IsEqualTo(pOldPosition);
}

void meUCameraMoveObject::Undo(){
	pObject->SetPosition(pOldPosition);
	pObject->GetWorld()->NotifyObjectGeometryChanged(pObject);
}

void meUCameraMoveObject::Redo(){
	pObject->SetPosition(pNewPosition);
	pObject->GetWorld()->NotifyObjectGeometryChanged(pObject);
}
