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

#include "gdeObjectClass.h"
#include "gdeObjectClassList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeObjectClass
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeObjectClassList::gdeObjectClassList(){
}

gdeObjectClassList::gdeObjectClassList(const gdeObjectClassList &list) :
pObjectClasses(list.pObjectClasses){
}

gdeObjectClassList::~gdeObjectClassList(){
}



// Management
///////////////

int gdeObjectClassList::GetCount() const{
	return pObjectClasses.GetCount();
}

gdeObjectClass *gdeObjectClassList::GetAt(int index) const{
	return (gdeObjectClass*)pObjectClasses.GetAt(index);
}

gdeObjectClass *gdeObjectClassList::GetNamed(const char *name) const{
	const int count = pObjectClasses.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		gdeObjectClass * const objectClass = (gdeObjectClass*)pObjectClasses.GetAt(i);
		
		if(objectClass->GetName() == name){
			return objectClass;
		}
	}
	
	return NULL;
}

int gdeObjectClassList::IndexOf(gdeObjectClass *objectClass) const{
	return pObjectClasses.IndexOf(objectClass);
}

int gdeObjectClassList::IndexOfNamed(const char *name) const{
	const int count = pObjectClasses.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((gdeObjectClass*)pObjectClasses.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

bool gdeObjectClassList::Has(gdeObjectClass *objectClass) const{
	return pObjectClasses.Has(objectClass);
}

bool gdeObjectClassList::HasNamed(const char *name) const{
	const int count = pObjectClasses.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((gdeObjectClass*)pObjectClasses.GetAt(i))->GetName() == name){
			return true;
		}
	}
	
	return false;
}

void gdeObjectClassList::Add(gdeObjectClass *objectClass){
	if(!objectClass || HasNamed(objectClass->GetName())){
		DETHROW(deeInvalidParam);
	}
	
	pObjectClasses.Add(objectClass);
}

void gdeObjectClassList::Remove(gdeObjectClass *objectClass){
	pObjectClasses.Remove(objectClass);
}

void gdeObjectClassList::RemoveAll(){
	pObjectClasses.RemoveAll();
}



gdeObjectClassList &gdeObjectClassList::operator=(const gdeObjectClassList &list){
	pObjectClasses = list.pObjectClasses;
	return *this;
}
