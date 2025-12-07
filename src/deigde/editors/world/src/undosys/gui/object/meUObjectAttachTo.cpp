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

#include "meUObjectAttachTo.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectAttachTo
////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectAttachTo::meUObjectAttachTo(meWorld *world, const meObjectList &objects, meObject *attachTo){
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	const int count = objects.GetCount();
	meObject *object;
	decString text;
	
	if(count == 0){
		DETHROW(deeInvalidParam);
	}
	
	pWorld = NULL;
	pAttachTo = NULL;
	pObjects = NULL;
	pObjectCount = 0;
	
	if(attachTo){
		SetShortInfo("Attach Objects To");
		
	}else{
		SetShortInfo("Detach Objects");
	}
	
	if(count > 1){
		text.Format("%i objects", count);
		
	}else{
		text = "1 object";
	}
	SetLongInfo(text.GetString());
	
	try{
		pObjects = new sObject[count];
		
		for(pObjectCount=0; pObjectCount<count; pObjectCount++){
			object = objects.GetAt(pObjectCount);
			
			pObjects[pObjectCount].object = object;
			object->AddReference();
			pObjects[pObjectCount].oldAttachTo = object->GetAttachedTo();
			if(pObjects[pObjectCount].oldAttachTo){
				pObjects[pObjectCount].oldAttachTo->AddReference();
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pAttachTo = attachTo;
	if(attachTo){
		attachTo->AddReference();
	}
	
	pWorld = world;
	world->AddReference();
}

meUObjectAttachTo::~meUObjectAttachTo(){
	pCleanUp();
}



// Management
///////////////

void meUObjectAttachTo::Undo(){
	meObject *object;
	int o;
	
	for(o=0; o<pObjectCount; o++){
		object = pObjects[o].object;
		
		if(object != pObjects[o].oldAttachTo){
			object->SetAttachedTo(pObjects[o].oldAttachTo);
			pWorld->NotifyObjectGeometryChanged(object);
		}
	}
}

void meUObjectAttachTo::Redo(){
	meObject *object;
	int o;
	
	for(o=0; o<pObjectCount; o++){
		object = pObjects[o].object;
		
		if(object != pAttachTo){
			object->SetAttachedTo(pAttachTo);
			pWorld->NotifyObjectGeometryChanged(object);
		}
	}
}



// Private Functions
//////////////////////

void meUObjectAttachTo::pCleanUp(){
	if(pObjects){
		while(pObjectCount > 0){
			pObjectCount--;
			if(pObjects[pObjectCount].oldAttachTo){
				pObjects[pObjectCount].oldAttachTo->FreeReference();
			}
			pObjects[pObjectCount].object->FreeReference();
		}
		
		delete [] pObjects;
	}
	
	if(pAttachTo){
		pAttachTo->FreeReference();
	}
	
	if(pWorld){
		pWorld->FreeReference();
	}
}
