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

#include "meUObjectCopyScale.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectCopyScale
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectCopyScale::meUObjectCopyScale(meWorld *world, bool copyX, bool copyY, bool copyZ) :
pCopyX(copyX),
pCopyY(copyY),
pCopyZ(copyZ)
{
	if(!world || !world->GetSelectionObject().GetActive()){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Copy Object Scale");
	SetLongInfo("Copy Object Scale");
	
	world->GetSelectionObject().GetSelected().Visit([&](meObject *o){
		pObjects.Add(meUndoDataObject::Ref::New(o));
	});
	
	pNewSize = world->GetSelectionObject().GetActive()->GetSize();
}

meUObjectCopyScale::~meUObjectCopyScale(){
}



// Management
///////////////

void meUObjectCopyScale::Undo(){
	pObjects.Visit([&](const meUndoDataObject &data){
		meObject * const object = data.GetObject();
		
		object->SetSize(data.GetOldSize());
		object->GetWorld()->NotifyObjectGeometryChanged(object);
	});
}

void meUObjectCopyScale::Redo(){
	pObjects.Visit([&](const meUndoDataObject &data){
		meObject * const object = data.GetObject();
		
		decVector size(data.GetOldSize());
		
		if(pCopyX){
			size.x = pNewSize.x;
		}
		if(pCopyY){
			size.y = pNewSize.y;
		}
		if(pCopyZ){
			size.z = pNewSize.z;
		}
		
		object->SetSize(size);
		object->GetWorld()->NotifyObjectGeometryChanged(object);
	});
}
