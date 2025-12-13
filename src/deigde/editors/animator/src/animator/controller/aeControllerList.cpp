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

#include "aeController.h"
#include "aeControllerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class aeController
/////////////////

// Constructor, destructor
////////////////////////////

aeControllerList::aeControllerList(){
}

aeControllerList::aeControllerList(const aeControllerList &copy) :
pControllers(copy.pControllers){
}

aeControllerList::~aeControllerList(){
}



// Management
///////////////

int aeControllerList::GetCount() const{
	return pControllers.GetCount();
}

aeController *aeControllerList::GetAt(int index) const{
	return (aeController*)pControllers.GetAt(index);
}

aeController *aeControllerList::GetNamed(const char *name) const{
	const int count = pControllers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		aeController * const controller = (aeController*)pControllers.GetAt(i);
		if(controller->GetName() == name){
			return controller;
		}
	}
	
	return nullptr;
}

int aeControllerList::IndexOf(aeController *controller) const{
	return pControllers.IndexOf(controller);
}

int aeControllerList::IndexOfNamed(const char *name) const{
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(((aeController*)pControllers.GetAt(i))->GetName() == name){
			return i;
		}
	}
	return -1;
}

bool aeControllerList::Has(aeController *controller) const{
	return pControllers.Has(controller);
}

bool aeControllerList::HasNamed(const char *name) const{
	return GetNamed(name) != nullptr;
}

void aeControllerList::Add(aeController *controller){
	if(!controller || HasNamed(controller->GetName())){
		DETHROW(deeInvalidParam);
	}
	pControllers.Add(controller);
}

void aeControllerList::Insert(aeController *controller, int index){
	if(!controller || HasNamed(controller->GetName())){
		DETHROW(deeInvalidParam);
	}
	pControllers.Insert(controller, index);
}

void aeControllerList::Move(aeController *controller, int index){
	pControllers.Move(controller, index);
}

void aeControllerList::Remove(aeController *controller){
	pControllers.Remove(controller);
}

void aeControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



// Operators
//////////////

aeControllerList &aeControllerList::operator=(const aeControllerList &list){
	pControllers = list.pControllers;
	return *this;
}

aeControllerList &aeControllerList::operator+=(const aeControllerList &list){
	pControllers += list.pControllers;
	return *this;
}
