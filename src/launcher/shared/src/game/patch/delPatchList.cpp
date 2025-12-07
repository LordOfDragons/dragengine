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

#include "delPatch.h"
#include "delPatchList.h"

#include <dragengine/common/exceptions.h>



// Class delPatchList
////////////////////////

// Constructors and Destructors
/////////////////////////////////

delPatchList::delPatchList(){
}

delPatchList::delPatchList(const delPatchList &list) :
pPatches(list.pPatches){
}

delPatchList::~delPatchList(){
	RemoveAll();
}



// Management
///////////////

int delPatchList::GetCount() const{
	return pPatches.GetCount();
}

delPatch *delPatchList::GetAt(int index) const{
	return (delPatch*)pPatches.GetAt(index);
}

delPatch *delPatchList::GetWithID(const decUuid &id) const{
	DEASSERT_TRUE(id)
	
	const int count = pPatches.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delPatch * const patch = (delPatch*)pPatches.GetAt(i);
		if(patch->GetIdentifier() == id){
			return patch;
		}
	}
	
	return nullptr;
}

bool delPatchList::Has(delPatch *patch) const{
	return pPatches.Has(patch);
}

bool delPatchList::HasWithID(const decUuid &id) const{
	DEASSERT_TRUE(id)
	
	const int count = pPatches.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delPatch &patch = *((delPatch*)pPatches.GetAt(i));
		if(patch.GetIdentifier() == id){
			return true;
		}
	}
	
	return false;
}

int delPatchList::IndexOf(delPatch *patch) const{
	return pPatches.IndexOf(patch);
}

int delPatchList::IndexOfWithID(const decUuid &id) const{
	DEASSERT_TRUE(id)
	
	const int count = pPatches.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delPatch &patch = *((delPatch*)pPatches.GetAt(i));
		if(patch.GetIdentifier() == id){
			return i;
		}
	}
	
	return -1;
}

void delPatchList::Add(delPatch *patch){
	DEASSERT_NOTNULL(patch)
	DEASSERT_FALSE(HasWithID(patch->GetIdentifier()))
	
	pPatches.Add(patch);
}

void delPatchList::AddAll(const delPatchList &list){
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delPatch * const patch = (delPatch*)list.GetAt(i);
		if(!HasWithID(patch->GetIdentifier())){
			pPatches.Add(patch);
		}
	}
}

void delPatchList::Remove(delPatch *patch){
	pPatches.RemoveFrom(pPatches.IndexOf(patch));
}

void delPatchList::RemoveAll(){
	pPatches.RemoveAll();
}
