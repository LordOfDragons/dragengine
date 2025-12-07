/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "meUObjectRandomRotation.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"
#include "dragengine/common/exceptions.h"


// Class meUObjectRandomRotation
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectRandomRotation::meUObjectRandomRotation(meWorld *world, const meObjectList &objects,
	bool randomizeX, bool randomizeY, bool randomizeZ) :
pWorld(world)
{
	DEASSERT_NOTNULL(world)
	
	SetShortInfo("Randomize Object Rotation");
	
	const int count = objects.GetCount();
	int i;
	for(i=0; i<count; i++){
		pObjects.Add(ObjectData::Ref::NewWith(objects.GetAt(i), randomizeX, randomizeY, randomizeZ));
	}
}


// Management
///////////////

void meUObjectRandomRotation::Undo(){
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const ObjectData &data = *((ObjectData*)pObjects.GetAt(i));
		meObject &object = data.GetObject();
		object.SetRotation(data.GetOldOrientation());
		pWorld->NotifyObjectGeometryChanged(&object);
	}
}

void meUObjectRandomRotation::Redo(){
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const ObjectData &data = *((ObjectData*)pObjects.GetAt(i));
		meObject &object = data.GetObject();
		object.SetRotation(data.GetNewOrientation());
		pWorld->NotifyObjectGeometryChanged(&object);
	}
}
