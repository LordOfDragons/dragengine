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

#include "meUObjectSnapToGrid.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectSnapToGrid
//////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectSnapToGrid::meUObjectSnapToGrid(meWorld::Ref world, const meObjectList &objects, float grid) :

pObjects(NULL),
pObjectCount(0),
pGrid((double)grid)
{
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	const int count = objects.GetCount();
	if(count == 0){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Snap objects to grid");
	
	try{
		pObjects = new sObject[count];
		
		for(pObjectCount=0; pObjectCount<count; pObjectCount++){
			meObject * const object = objects.GetAt(pObjectCount);
			
			pObjects[pObjectCount].object = object;
			pObjects[pObjectCount].position = object->GetPosition();
			object->AddReference();
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
}

meUObjectSnapToGrid::~meUObjectSnapToGrid(){
	pCleanUp();
}



// Management
///////////////

void meUObjectSnapToGrid::Undo(){
	int i;
	
	for(i=0; i<pObjectCount; i++){
		pObjects[i].object->SetPosition(pObjects[i].position);
		pWorld->NotifyObjectGeometryChanged(pObjects[i].object);
	}
}

void meUObjectSnapToGrid::Redo(){
	int i;
	
	for(i =0; i <pObjectCount; i++){
		decDVector position(pObjects[i].position);
		position.Snap(pGrid);
		pObjects[i].object->SetPosition(position);
		pWorld->NotifyObjectGeometryChanged(pObjects[i].object);
	}
}



// Private Functions
//////////////////////

void meUObjectSnapToGrid::pCleanUp(){
	if(pObjects){
		while(pObjectCount > 0){
			pObjectCount--;
			pObjects[pObjectCount].object->FreeReference();
		}
		
		delete [] pObjects;
	}
}
