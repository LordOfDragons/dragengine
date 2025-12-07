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

#include "ceActorController.h"
#include "ceActorControllerList.h"

#include <dragengine/common/exceptions.h>



// Class ceActorController
////////////////////////////

// Constructor, destructor
////////////////////////////

ceActorControllerList::ceActorControllerList(){
}

ceActorControllerList::ceActorControllerList(const ceActorControllerList &list) :
pControllers(list.pControllers){
}

ceActorControllerList::~ceActorControllerList(){
}



// Management
///////////////

int ceActorControllerList::GetCount() const{
	return pControllers.GetCount();
}

ceActorController *ceActorControllerList::GetAt(int index) const{
	return (ceActorController*)pControllers.GetAt(index);
}

ceActorController *ceActorControllerList::GetNamed(const char *name) const{
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		ceActorController * const controller = (ceActorController*)pControllers.GetAt(i);
		if(controller->GetName() == name){
			return controller;
		}
	}
	return nullptr;
}

int ceActorControllerList::IndexOf(ceActorController *controller) const{
	return pControllers.IndexOf(controller);
}

bool ceActorControllerList::Has(ceActorController *controller) const{
	return pControllers.Has(controller);
}

bool ceActorControllerList::HasNamed(const char *name) const{
	return GetNamed(name) != nullptr;
}

void ceActorControllerList::Add(ceActorController *controller){
	if(!controller || Has(controller)){
		DETHROW(deeInvalidParam);
	}
	
	pControllers.Add(controller);
}

void ceActorControllerList::Remove(ceActorController *controller){
	pControllers.Remove(controller);
}

void ceActorControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



ceActorControllerList &ceActorControllerList::operator=(const ceActorControllerList &list){
	pControllers = list.pControllers;
	return *this;
}
