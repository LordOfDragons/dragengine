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

#include "ceStrip.h"
#include "ceStripList.h"

#include <dragengine/common/exceptions.h>



// Class ceStripList
//////////////////////

// Constructor, destructor
////////////////////////////

ceStripList::ceStripList(){
}

ceStripList::~ceStripList(){
	pList.RemoveAll();
}



// Management
///////////////

int ceStripList::GetCount() const{
	return pList.GetCount();
}

ceStrip *ceStripList::GetAt(int index) const{
	return (ceStrip*)pList.GetAt(index);
}

int ceStripList::IndexOf(ceStrip *entry) const{
	return pList.IndexOf(entry);
}

bool ceStripList::Has(ceStrip *entry) const{
	return pList.Has(entry);
}

void ceStripList::Add(ceStrip *entry){
	if(!entry){
		DETHROW(deeInvalidParam);
	}
	
	pList.Add(entry);
}

void ceStripList::InsertAt(ceStrip *entry, int index){
	if(!entry){
		DETHROW(deeInvalidParam);
	}
	
	pList.Insert(entry, index);
}

void ceStripList::MoveTo(ceStrip *entry, int index){
	if(!entry){
		DETHROW(deeInvalidParam);
	}
	
	pList.Move(entry, index);
}

void ceStripList::Remove(ceStrip *entry){
	pList.Remove(entry);
}

void ceStripList::RemoveAll(){
	pList.RemoveAll();
}



void ceStripList::AddCopyFrom(const ceStripList &list){
	const int count = list.GetCount();
	ceStrip *entry = NULL;
	int i;
	
	try{
		for(i=0; i<count; i++){
			entry = new ceStrip(*list.GetAt(i));
			pList.Add(entry);
			entry->FreeReference();
			entry = NULL;
		}
		
	}catch(const deException &){
		if(entry){
			entry->FreeReference();
		}
		throw;
	}
}



ceStripList &ceStripList::operator=(const ceStripList &list){
	pList = list.pList;
	return *this;
}

