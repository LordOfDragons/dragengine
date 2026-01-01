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

#include "meUSetObjectClass.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUSetObjectClass
////////////////////////////

// Constructor, destructor
////////////////////////////

meUSetObjectClass::meUSetObjectClass(meObject *object, const char *newcname){
	DEASSERT_NOTNULL(object)
	DEASSERT_NOTNULL(object->GetWorld())
	
	SetShortInfo("Set Object class");
	
	const cObject::Ref udata(cObject::Ref::New());
	udata->oldcname = object->GetClassName();
	udata->object = object;
	udata->newcname = newcname;
	udata->oldsize = object->GetSize();
	udata->oldscaling = object->GetScaling();
	pObjects.Add(udata);
}

meUSetObjectClass::meUSetObjectClass(meObject::List &objects, const char *newcname){
	DEASSERT_TRUE(objects.IsNotEmpty())
	
	SetShortInfo("Set Object classes");
	
	objects.Visit([&](meObject *object){
		const cObject::Ref udata(cObject::Ref::New());
		udata->oldcname = object->GetClassName();
		udata->object = object;
		udata->newcname = newcname;
		udata->oldsize = object->GetSize();
		udata->oldscaling = object->GetScaling();
		pObjects.Add(udata);
	});
}

meUSetObjectClass::~meUSetObjectClass(){
}



// Management
///////////////

void meUSetObjectClass::Undo(){
	pObjects.Visit([&](const cObject &data){
		meObject * object = data.object;
		meWorld * const world = object->GetWorld();
		
		object->SetClassName(data.oldcname);
		object->SetSizeAndScaling(data.oldsize, data.oldscaling);
		
		world->NotifyObjectClassChanged(object);
		world->NotifyObjectGeometryChanged(object);
	});
}

void meUSetObjectClass::Redo(){
	pObjects.Visit([&](const cObject &data){
		meObject * object = data.object;
		meWorld * const world = object->GetWorld();
		
		object->SetClassName(data.newcname);
		
		world->NotifyObjectClassChanged(object);
		world->NotifyObjectGeometryChanged(object);
	});
}
