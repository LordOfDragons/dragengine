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

#include <stdlib.h>

#include "meUAddObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUAddObject
///////////////////////

// Constructor, destructor
////////////////////////////

meUAddObject::meUAddObject(meWorld *world, meObject *object){
	if(!world || !object){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Add Object");
	
	pWorld = world;
	pObject = object;
}

meUAddObject::meUAddObject(meWorld *world, const decDVector &position, const char *classname){
	if(!world || !classname){
		DETHROW(deeInvalidParam);
	}
	
	decString text;
	
	SetShortInfo("Add Object");
	text.Format("position(%g,%g,%g) class(%s)", position.x, position.y, position.z, classname);
	SetLongInfo(text.GetString());
	
	pWorld = NULL;
	pObject = NULL;
	
	try{
		pWorld = world;
		pObject.TakeOver(new meObject(world->GetEnvironment()));
		pObject->SetPosition(position);
		pObject->SetSize(decVector(0.5f, 0.5f, 0.5f));
		pObject->SetClassName(classname);
		pObject->SetID(world->NextObjectID());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUAddObject::~meUAddObject(){
	pCleanUp();
}



// Management
///////////////

void meUAddObject::Undo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	
	selection.Remove(pObject);
	if(pObject->GetActive()){
		selection.ActivateNext();
	}
	
	pWorld->RemoveObject(pObject);
	
	pWorld->NotifyObjectRemoved(pObject);
	pWorld->NotifyObjectSelectionChanged();
}

void meUAddObject::Redo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	
	pWorld->AddObject(pObject);
	
	selection.Reset();
	selection.Add(pObject);
	selection.SetActive(pObject);
	
	pWorld->NotifyObjectAdded(pObject);
	pWorld->NotifyObjectSelectionChanged();
}



// Private Functions
//////////////////////

void meUAddObject::pCleanUp(){
}
