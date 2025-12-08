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

#include "igdeGDCNavigationBlocker.h"
#include "igdeGDCNavigationBlockerList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCNavigationBlocker
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCNavigationBlockerList::igdeGDCNavigationBlockerList(){
}

igdeGDCNavigationBlockerList::igdeGDCNavigationBlockerList(const igdeGDCNavigationBlockerList &list){
	pNavigationBlockers = list.pNavigationBlockers;
}

igdeGDCNavigationBlockerList::~igdeGDCNavigationBlockerList(){
}



// Management
///////////////

int igdeGDCNavigationBlockerList::GetCount() const{
	return pNavigationBlockers.GetCount();
}

igdeGDCNavigationBlocker *igdeGDCNavigationBlockerList::GetAt(int index) const{
	return (igdeGDCNavigationBlocker*)pNavigationBlockers.GetAt(index);
}

int igdeGDCNavigationBlockerList::IndexOf(igdeGDCNavigationBlocker *blocker) const{
	return pNavigationBlockers.IndexOf(blocker);
}

bool igdeGDCNavigationBlockerList::Has(igdeGDCNavigationBlocker *blocker) const{
	return pNavigationBlockers.Has(blocker);
}

void igdeGDCNavigationBlockerList::Add(igdeGDCNavigationBlocker *blocker){
	if(!blocker){
		DETHROW(deeInvalidParam);
	}
	pNavigationBlockers.Add(blocker);
}

void igdeGDCNavigationBlockerList::InsertAt(igdeGDCNavigationBlocker *blocker, int index){
	if(!blocker){
		DETHROW(deeInvalidParam);
	}
	pNavigationBlockers.Insert(blocker, index);
}

void igdeGDCNavigationBlockerList::MoveTo(igdeGDCNavigationBlocker *blocker, int index){
	pNavigationBlockers.Move(blocker, index);
}

void igdeGDCNavigationBlockerList::Remove(igdeGDCNavigationBlocker *blocker){
	pNavigationBlockers.Remove(blocker);
}

void igdeGDCNavigationBlockerList::RemoveAll(){
	pNavigationBlockers.RemoveAll();
}



void igdeGDCNavigationBlockerList::SetToDeepCopyFrom(const igdeGDCNavigationBlockerList &list){
	igdeGDCNavigationBlocker *blocker = NULL;
	const int count = list.GetCount();
	
	int i;
	
	try{
		for(i=0; i<count; i++){
			blocker = new igdeGDCNavigationBlocker(*list.GetAt(i));
			Add(blocker);
			blocker->FreeReference();
			blocker = NULL;
		}
		
	}catch(const deException &){
		if(blocker){
			blocker->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCNavigationBlockerList &igdeGDCNavigationBlockerList::operator=(const igdeGDCNavigationBlockerList &list){
	pNavigationBlockers = list.pNavigationBlockers;
	return *this;
}
