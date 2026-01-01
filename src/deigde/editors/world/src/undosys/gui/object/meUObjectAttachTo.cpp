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

#include "meUObjectAttachTo.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectAttachTo
////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectAttachTo::meUObjectAttachTo(meWorld *world, const meObject::List &objects, meObject *attachTo){
	DEASSERT_NOTNULL(world)
	DEASSERT_TRUE(objects.IsNotEmpty())
	
	if(attachTo){
		SetShortInfo("Attach Objects To");
		
	}else{
		SetShortInfo("Detach Objects");
	}
	
	decString text;
	if(objects.GetCount() > 1){
		text.Format("%i objects", objects.GetCount());
		
	}else{
		text = "1 object";
	}
	SetLongInfo(text.GetString());
	
	objects.Visit([&](meObject *o) {
		const cObject::Ref udata(cObject::Ref::New());
		udata->object = o;
		udata->oldAttachTo = o->GetAttachedTo();
		pObjects.Add(udata);
	});
	
	pAttachTo = attachTo;
	pWorld = world;
}

meUObjectAttachTo::~meUObjectAttachTo(){
}



// Management
///////////////

void meUObjectAttachTo::Undo(){
	pObjects.Visit([&](const cObject &data){
		if(data.object != data.oldAttachTo){
			data.object->SetAttachedTo(data.oldAttachTo);
			pWorld->NotifyObjectGeometryChanged(data.object);
		}
	});
}

void meUObjectAttachTo::Redo(){
	pObjects.Visit([&](const cObject &data){
		if(data.object != pAttachTo){
			data.object->SetAttachedTo(pAttachTo);
			pWorld->NotifyObjectGeometryChanged(data.object);
		}
	});
}
