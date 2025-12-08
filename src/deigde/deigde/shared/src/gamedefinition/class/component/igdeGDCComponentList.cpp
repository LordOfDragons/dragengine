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

#include "igdeGDCComponent.h"
#include "igdeGDCComponentList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCComponent
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCComponentList::igdeGDCComponentList(){
}

igdeGDCComponentList::igdeGDCComponentList(const igdeGDCComponentList &list){
	pComponents = list.pComponents;
}

igdeGDCComponentList::~igdeGDCComponentList(){
}



// Management
///////////////

int igdeGDCComponentList::GetCount() const{
	return pComponents.GetCount();
}

igdeGDCComponent *igdeGDCComponentList::GetAt(int index) const{
	return (igdeGDCComponent*)pComponents.GetAt(index);
}

int igdeGDCComponentList::IndexOf(igdeGDCComponent *component) const{
	return pComponents.IndexOf(component);
}

bool igdeGDCComponentList::Has(igdeGDCComponent *component) const{
	return pComponents.Has(component);
}

void igdeGDCComponentList::Add(igdeGDCComponent *component){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	pComponents.Add(component);
}

void igdeGDCComponentList::InsertAt(igdeGDCComponent *component, int index){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	pComponents.Insert(component, index);
}

void igdeGDCComponentList::MoveTo(igdeGDCComponent *component, int index){
	pComponents.Move(component, index);
}

void igdeGDCComponentList::Remove(igdeGDCComponent *component){
	pComponents.Remove(component);
}

void igdeGDCComponentList::RemoveAll(){
	pComponents.RemoveAll();
}



void igdeGDCComponentList::SetToDeepCopyFrom(const igdeGDCComponentList &list){
	const int count = list.GetCount();
	igdeGDCComponent *component = NULL;
	
	int i;
	
	try{
		for(i=0; i<count; i++){
			component = new igdeGDCComponent(*list.GetAt(i));
			Add(component);
			component->FreeReference();
			component = NULL;
		}
		
	}catch(const deException &){
		if(component){
			component->FreeReference();
		}
		throw;
	}
}



igdeGDCComponentList &igdeGDCComponentList::operator=(const igdeGDCComponentList &list){
	pComponents = list.pComponents;
	return *this;
}
