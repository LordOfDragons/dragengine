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
#include "decUuidSet.h"

#include "../exceptions.h"



// Class decUuidSet
/////////////////////

// Constructor, destructor
////////////////////////////

decUuidSet::decUuidSet(){
	pUuids = NULL;
	pUuidCount = 0;
	pUuidSize = 0;
}

decUuidSet::decUuidSet(const decUuidSet &set){
	const int count = set.GetCount();
	
	pUuids = NULL;
	pUuidCount = 0;
	pUuidSize = 0;
	
	if(count > 0){
		pUuids = new decUuid[count];
		pUuidSize = count;
		
		while(pUuidCount < count){
			pUuids[pUuidCount] = set.GetAt(pUuidCount);
			pUuidCount++;
		}
	}
}

decUuidSet::~decUuidSet(){
	RemoveAll();
	if(pUuids){
		delete [] pUuids;
	}
}



// Management
///////////////

const decUuid &decUuidSet::GetAt(int index) const{
	if(index < 0 || index >= pUuidCount){
		DETHROW(deeInvalidParam);
	}
	
	return pUuids[index];
}

int decUuidSet::IndexOf(const decUuid &uuid) const{
	int i;
	
	for(i=0; i<pUuidCount; i++){
		if(pUuids[i] == uuid){
			return i;
		}
	}
	
	return -1;
}

bool decUuidSet::Has(const decUuid &uuid) const{
	return IndexOf(uuid) != -1;
}

void decUuidSet::Add(const decUuid &uuid){
	if(Has(uuid)){
		return;
	}
	
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

void decUuidSet::Remove(const decUuid &uuid){
	const int index = IndexOf(uuid);
	int i;
	
	if(index != -1){
		for(i=index+1; i<pUuidCount; i++){
			pUuids[i - 1] = pUuids[i];
		}
		pUuidCount--;
	}
}

void decUuidSet::RemoveAll(){
	pUuidCount = 0;
}



// Operators
//////////////

const decUuid &decUuidSet::operator[](int index) const{
	return GetAt(index);
}

decUuidSet &decUuidSet::operator=(const decUuidSet &set){
	const int count = set.GetCount();
	int i;
	
	RemoveAll();
	for(i=0; i<count; i++){
		Add(set.GetAt(i)); // hard coding this call would remove the HasUuid checks
	}
	
	return *this;
}

bool decUuidSet::operator==(const decUuidSet &set) const{
	int i;
	
	if(set.GetCount() != pUuidCount){
		return false;
	}
	
	for(i=0; i<pUuidCount; i++){
		if(!set.Has(pUuids[i])){
			return false;
		}
	}
	
	return true;
}

bool decUuidSet::operator!=(const decUuidSet &set) const{
	return !(*this == set);
}

decUuidSet decUuidSet::operator+(const decUuidSet &set) const{
	const int count = set.GetCount();
	decUuidSet newSet(*this);
	int i;
	
	for(i=0; i<count; i++){
		newSet.Add(set.GetAt(i));
	}
	
	return newSet;
}

decUuidSet &decUuidSet::operator+=(const decUuidSet &set){
	const int count = set.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		Add(set.GetAt(i));
	}
	
	return *this;
}

decUuidSet decUuidSet::operator+(const decUuid &uuid) const{
	decUuidSet newList(*this);
	
	newList.Add(uuid);
	
	return newList;
}

decUuidSet &decUuidSet::operator+=(const decUuid &uuid){
	Add(uuid);
	
	return *this;
}
