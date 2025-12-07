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

#include "igdeGDCNavigationSpace.h"
#include "igdeGDCNavigationSpaceList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCNavigationSpace
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCNavigationSpaceList::igdeGDCNavigationSpaceList(){
}

igdeGDCNavigationSpaceList::igdeGDCNavigationSpaceList(const igdeGDCNavigationSpaceList &list){
	pNavigationSpaces = list.pNavigationSpaces;
}

igdeGDCNavigationSpaceList::~igdeGDCNavigationSpaceList(){
}



// Management
///////////////

int igdeGDCNavigationSpaceList::GetCount() const{
	return pNavigationSpaces.GetCount();
}

igdeGDCNavigationSpace *igdeGDCNavigationSpaceList::GetAt(int index) const{
	return (igdeGDCNavigationSpace*)pNavigationSpaces.GetAt(index);
}

int igdeGDCNavigationSpaceList::IndexOf(igdeGDCNavigationSpace *navSpace) const{
	return pNavigationSpaces.IndexOf(navSpace);
}

bool igdeGDCNavigationSpaceList::Has(igdeGDCNavigationSpace *navSpace) const{
	return pNavigationSpaces.Has(navSpace);
}

void igdeGDCNavigationSpaceList::Add(igdeGDCNavigationSpace *navSpace){
	if(!navSpace){
		DETHROW(deeInvalidParam);
	}
	
	pNavigationSpaces.Add(navSpace);
}

void igdeGDCNavigationSpaceList::InsertAt(igdeGDCNavigationSpace *navSpace, int index){
	if(!navSpace){
		DETHROW(deeInvalidParam);
	}
	
	pNavigationSpaces.Insert(navSpace, index);
}

void igdeGDCNavigationSpaceList::MoveTo(igdeGDCNavigationSpace *navSpace, int index){
	pNavigationSpaces.Move(navSpace, index);
}

void igdeGDCNavigationSpaceList::Remove(igdeGDCNavigationSpace *navSpace){
	pNavigationSpaces.Remove(navSpace);
}

void igdeGDCNavigationSpaceList::RemoveAll(){
	pNavigationSpaces.RemoveAll();
}



void igdeGDCNavigationSpaceList::SetToDeepCopyFrom(const igdeGDCNavigationSpaceList &list){
	const int count = list.GetCount();
	igdeGDCNavigationSpace *navSpace = NULL;
	
	int i;
	
	try{
		for(i=0; i<count; i++){
			navSpace = new igdeGDCNavigationSpace(*list.GetAt(i));
			Add(navSpace);
			navSpace->FreeReference();
			navSpace = NULL;
		}
		
	}catch(const deException &){
		if(navSpace){
			navSpace->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCNavigationSpaceList &igdeGDCNavigationSpaceList::operator=(const igdeGDCNavigationSpaceList &list){
	pNavigationSpaces = list.pNavigationSpaces;
	return *this;
}
