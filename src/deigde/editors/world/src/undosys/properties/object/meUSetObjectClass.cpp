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

#include "meUSetObjectClass.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectList.h"

#include <dragengine/common/exceptions.h>



// Class meUSetObjectClass
////////////////////////////

// Constructor, destructor
////////////////////////////

meUSetObjectClass::meUSetObjectClass(meObject *object, const char *newcname){
	if(!object || !newcname) DETHROW(deeInvalidParam);
	
	meWorld *world = object->GetWorld();
	if(!world) DETHROW(deeInvalidParam);
	
	SetShortInfo("Set Object class");
	
	pObjects = NULL;
	pObjectCount = 0;
	
	try{
		pObjects = new sObject[1];
		if(!pObjects) DETHROW(deeOutOfMemory);
		
		pObjects[0].oldcname = object->GetClassName();
		pObjects[0].newcname = newcname;
		pObjects[0].oldsize = object->GetSize();
		pObjects[0].oldscaling = object->GetScaling();
		pObjects[0].object = object;
		
		pObjectCount = 1;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUSetObjectClass::meUSetObjectClass(meObjectList &objects, const char *newcname){
	int count = objects.GetCount();
	meObject *object;
	
	if(count == 0 || !newcname) DETHROW(deeInvalidParam);
	
	SetShortInfo("Set Object classes");
	
	pObjects = NULL;
	pObjectCount = 0;
	
	try{
		pObjects = new sObject[count];
		if(!pObjects) DETHROW(deeOutOfMemory);
		
		for(pObjectCount=0; pObjectCount<count; pObjectCount++){
			object = objects.GetAt(pObjectCount);
			
			pObjects[pObjectCount].oldcname = object->GetClassName();
			pObjects[pObjectCount].newcname = newcname;
			pObjects[pObjectCount].oldsize = object->GetSize();
			pObjects[pObjectCount].oldscaling = object->GetScaling();
			pObjects[pObjectCount].object = object;
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUSetObjectClass::~meUSetObjectClass(){
	pCleanUp();
}



// Management
///////////////

void meUSetObjectClass::Undo(){
	meObject *object;
	meWorld *world;
	int o;
	
	for(o=0; o<pObjectCount; o++){
		object = pObjects[o].object;
		world = object->GetWorld();
		
		object->SetClassName(pObjects[o].oldcname.GetString());
		object->SetSizeAndScaling(pObjects[o].oldsize, pObjects[o].oldscaling);
		
		world->NotifyObjectClassChanged(object);
		world->NotifyObjectGeometryChanged(object);
	}
}

void meUSetObjectClass::Redo(){
	meObject *object;
	meWorld *world;
	int o;
	
	for(o=0; o<pObjectCount; o++){
		object = pObjects[o].object;
		world = object->GetWorld();
		
		object->SetClassName(pObjects[o].newcname.GetString());
		
		world->NotifyObjectClassChanged(object);
		world->NotifyObjectGeometryChanged(object);
	}
}



// Private Functions
//////////////////////

void meUSetObjectClass::pCleanUp(){
	if(pObjects){
		while(pObjectCount > 0){
			pObjectCount--;
			pObjects[pObjectCount].object->FreeReference();
		}
		
		delete [] pObjects;
	}
}
