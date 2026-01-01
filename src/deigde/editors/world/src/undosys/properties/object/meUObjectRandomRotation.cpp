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

#include <dragengine/common/exceptions.h>


// Class meUObjectRandomRotation
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectRandomRotation::meUObjectRandomRotation(meWorld *world, const meObject::List &objects,
	bool randomizeX, bool randomizeY, bool randomizeZ) :
pWorld(world)
{
	DEASSERT_NOTNULL(world)
	
	SetShortInfo("Randomize Object Rotation");
	
	meUndoDataObject::AddObjectsWithAttachments(objects, pObjects);
	
	pObjects.Visit([&](meUndoDataObject &data){
		decVector r(data.GetOldRotation());
		if(randomizeX){
			r.x = decMath::random(-180.0f, 180.0f);
		}
		if(randomizeY){
			r.y = decMath::random(-180.0f, 180.0f);
		}
		if(randomizeZ){
			r.z = decMath::random(-180.0f, 180.0f);
		}
		data.SetNewRotation(r);
		
		pObjects.Visit([&](meUndoDataObject &data2){
			if(data2.GetObject()->GetAttachedTo() == data.GetObject()){
				data2.TransformNew(data.GetOldMatrixInverse() * data.GetNewMatrix());
			}
		});
	});
}


// Management
///////////////

void meUObjectRandomRotation::Undo(){
	meUndoDataObject::RestoreOldGeometry(pObjects, *pWorld);
}

void meUObjectRandomRotation::Redo(){
	meUndoDataObject::ApplyNewGeometry(pObjects, pWorld);
}
