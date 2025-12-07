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

#include "decUuid.h"
#include "decUuidList.h"

#include "../exceptions.h"



// Class decUuidList
//////////////////////

// Constructor, destructor
////////////////////////////

decUuidList::decUuidList(){
	pUuids = NULL;
	pUuidCount = 0;
	pUuidSize = 0;
}

decUuidList::decUuidList(const decUuidList &list){
	const int count = list.GetCount();
	
	pUuids = NULL;
	pUuidCount = 0;
	pUuidSize = 0;
	
	if(count > 0){
		pUuids = new decUuid[count];
		pUuidSize = count;
		
		while(pUuidCount < count){
			pUuids[pUuidCount] = list.GetAt(pUuidCount);
			pUuidCount++;
		}
	}
}

decUuidList::~decUuidList(){
	RemoveAll();
	if(pUuids){
		delete [] pUuids;
	}
}



// Management
///////////////

const decUuid &decUuidList::GetAt(int index) const{
	if(index < 0 || index >= pUuidCount){
		DETHROW(deeInvalidParam);
	}
	
	return pUuids[index];
}

void decUuidList::SetAt(int index, const decUuid &uuid){
	if(index < 0 || index >= pUuidCount){
		DETHROW(deeInvalidParam);
	}
	
	pUuids[index] = uuid;
}

int decUuidList::IndexOf(const decUuid &uuid) const{
	int i;
	
	for(i=0; i<pUuidCount; i++){
		if(pUuids[i] == uuid){
			return i;
		}
	}
	
	return -1;
}

bool decUuidList::Has(const decUuid &uuid) const{
	return IndexOf(uuid) != -1;
}

void decUuidList::Add(const decUuid &uuid){
	if(pUuidCount == pUuidSize){
		const int newSize = pUuidSize * 3 / 2 + 1;
		decUuid * const newArray = new decUuid[newSize];
		if(pUuids){
			memcpy(newArray, pUuids, sizeof(decUuid) * pUuidSize);
			delete [] pUuids;
		}
		pUuids = newArray;
		pUuidSize = newSize;
	}
	
	pUuids[pUuidCount] = uuid;
	pUuidCount++;
}

void decUuidList::InsertAt(const decUuid &uuid, int index){
	if(index < 0 || index > pUuidCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	if(pUuidCount == pUuidSize){
		const int newSize = pUuidSize * 3 / 2 + 1;
		decUuid * const newArray = new decUuid[newSize];
		if(pUuids){
			memcpy(newArray, pUuids, sizeof(decUuid) * pUuidSize);
			delete [] pUuids;
		}
		pUuids = newArray;
		pUuidSize = newSize;
	}
	
	for(i=pUuidCount; i>index; i--){
		pUuids[i] = pUuids[i - 1];
	}
	pUuids[index] = uuid;
	pUuidCount++;
}

void decUuidList::Move(int from, int to){
	if(from < 0 || from >= pUuidCount || to < 0 || to >= pUuidCount){
		DETHROW(deeInvalidParam);
	}
	
	const decUuid &tempUuid(pUuids[from]);
	int i;
	
	if(to < from){
		for(i=from; i>to; i--){
			pUuids[i] = pUuids[i - 1];
		}
		
	}else if(to > from){
		for(i=from; i<to; i++){
			pUuids[i] = pUuids[i + 1];
		}
	}
	
	pUuids[to] = tempUuid;
}

void decUuidList::RemoveFrom(int index){
	if(index < 0 || index >= pUuidCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	for(i=index+1; i<pUuidCount; i++){
		pUuids[i - 1] = pUuids[i];
	}
	pUuidCount--;
}

void decUuidList::RemoveAll(){
	pUuidCount = 0;
}

decUuidList decUuidList::Splice(int first) const{
	return Splice(first, pUuidCount);
}

decUuidList decUuidList::Splice(int first, int last) const{
	if(first < 0){
		first = pUuidCount + first;
	}
	if(first < 0 || first >= pUuidCount){
		DETHROW(deeInvalidParam);
	}
	
	if(last < 0){
		last = pUuidCount + last;
	}
	if(last < 0 || last > pUuidCount){
		DETHROW(deeInvalidParam);
	}
	
	if(last <= first){
		return decUuidList();
	}
	
	decUuidList list;
	while(first < last){
		list.Add(pUuids[first++]);
	}
	return list;
}



// Operators
//////////////

const decUuid &decUuidList::operator[](int index) const{
	return GetAt(index);
}

decUuidList &decUuidList::operator=(const decUuidList &list){
	const int count = list.GetCount();
	int i;
	
	RemoveAll();
	for(i=0; i<count; i++){
		Add(list.GetAt(i));
	}
	
	return *this;
}

bool decUuidList::operator==(const decUuidList &list) const {
	int i;
	
	if(list.GetCount() != pUuidCount){
		return false;
	}
	
	for(i=0; i<pUuidCount; i++){
		if(pUuids[i] != list.GetAt(i)){
			return false;
		}
	}
	
	return true;
}

bool decUuidList::operator!=(const decUuidList &list) const {
	return ! (*this == list);
}

decUuidList decUuidList::operator+(const decUuidList &list) const{
	const int count = list.GetCount();
	decUuidList newList(*this);
	int i;
	
	for(i=0; i<count; i++){
		newList.Add(list.GetAt(i));
	}
	
	return newList;
}

decUuidList &decUuidList::operator+=(const decUuidList &list){
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		Add(list.GetAt(i));
	}
	
	return *this;
}

decUuidList decUuidList::operator+(const decUuid &uuid) const{
	decUuidList newList(*this);
	
	newList.Add(uuid);
	
	return newList;
}

decUuidList &decUuidList::operator+=(const decUuid &uuid){
	Add(uuid);
	return *this;
}
