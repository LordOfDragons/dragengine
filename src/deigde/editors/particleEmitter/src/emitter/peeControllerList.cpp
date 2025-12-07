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

#include "peeController.h"
#include "peeControllerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class peeController
////////////////////////

// Constructor, destructor
////////////////////////////

peeControllerList::peeControllerList(){
}

peeControllerList::~peeControllerList(){
}



// Management
///////////////

int peeControllerList::GetCount() const{
	return pControllers.GetCount();
}

peeController *peeControllerList::GetAt(int index) const{
	return (peeController*)pControllers.GetAt(index);
}

peeController *peeControllerList::GetNamed(const char *name) const{
	const int count = pControllers.GetCount();
	peeController *controller;
	int p;
	
	for(p=0; p<count; p++){
		controller = (peeController*)pControllers.GetAt(p);
		
		if(controller->GetName().Equals(name)){
			return controller;
		}
	}
	
	return NULL;
}

int peeControllerList::IndexOf(peeController *controller) const{
	return pControllers.IndexOf(controller);
}

int peeControllerList::IndexOfNamed(const char *name) const{
	const int count = pControllers.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((peeController*)pControllers.GetAt(p))->GetName().Equals(name)){
			return p;
		}
	}
	
	return -1;
}

bool peeControllerList::Has(peeController *controller) const{
	return pControllers.Has(controller);
}

bool peeControllerList::HasNamed(const char *name) const{
	const int count = pControllers.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((peeController*)pControllers.GetAt(p))->GetName().Equals(name)){
			return true;
		}
	}
	
	return false;
}

void peeControllerList::Add(peeController *controller){
	if(! controller || HasNamed(controller->GetName().GetString())) DETHROW(deeInvalidParam);
	
	pControllers.Add(controller);
}

void peeControllerList::InsertAt(peeController *controller, int index){
	if(! controller || HasNamed(controller->GetName().GetString())) DETHROW(deeInvalidParam);
	
	pControllers.Insert(controller, index);
}

void peeControllerList::MoveTo(peeController *controller, int index){
	pControllers.Move(controller, index);
}

void peeControllerList::Remove(peeController *controller){
	pControllers.Remove(controller);
}

void peeControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



peeControllerList &peeControllerList::operator=(const peeControllerList &list){
	pControllers = list.pControllers;
	return *this;
}
