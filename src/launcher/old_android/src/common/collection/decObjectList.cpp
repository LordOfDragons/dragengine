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

#include "decObjectList.h"

#include "../../deObject.h"
#include "../exceptions.h"



// Class decObjectList
////////////////////////

// Constructor, destructor
////////////////////////////

decObjectList::decObjectList(){
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
}

decObjectList::decObjectList(int capacity){
	if(capacity < 0){
		DETHROW(deeInvalidParam);
	}
	
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	
	if(capacity > 0){
		pObjects = new deObject*[capacity];
		pObjectSize = capacity;
	}
}

decObjectList::decObjectList(const decObjectList &list){
	int count = list.GetCount();
	
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	
	if(count > 0){
		deObject::Ref object;
		
		pObjects = new deObject*[count];
		pObjectSize = count;
		
		for(pObjectCount=0; pObjectCount<count; pObjectCount++){
			object = list.pObjects[pObjectCount];
			pObjects[pObjectCount] = object;
		}
	}
}

decObjectList::~decObjectList(){
	RemoveAll();
	if(pObjects){
		delete [] pObjects;
	}
}



// Management
///////////////

deObject *decObjectList::GetAt(int index) const{
	if(index < 0 || index >= pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	return pObjects[index];
}

void decObjectList::SetAt(int index, deObject *object){
	if(index < 0 || index >= pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	if(object != pObjects[index]){
		if(pObjects[index]){
			pObjects[index]->FreeReference();
		}
		
		pObjects[index] = object;
	}
}

int decObjectList::IndexOf(deObject *object) const{
	int p;
	
	for(p=0; p<pObjectCount; p++){
		if(object == pObjects[p]){
			return p;
		}
	}
	
	return -1;
}

int decObjectList::IndexOf(deObject *object, int start) const{
	if(start < 0 || start >= pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	int p;
	
	for(p=start; p<pObjectCount; p++){
		if(object == pObjects[p]){
			return p;
		}
	}
	
	return -1;
}

bool decObjectList::Has(deObject *object) const{
	int p;
	
	for(p=0; p<pObjectCount; p++){
		if(object == pObjects[p]){
			return true;
		}
	}
	
	return false;
}

int decObjectList::CountOccurance(deObject *object) const{
	int p, occuranceCount = 0;
	
	for(p=0; p<pObjectCount; p++){
		if(object == pObjects[p]){
			occuranceCount++;
		}
	}
	
	return occuranceCount;
}

void decObjectList::Add(deObject *object){
	if(pObjectCount == pObjectSize){
		int newSize = pObjectSize * 3 / 2 + 1;
		deObject **newArray = new deObject*[newSize];
		if(pObjects){
			memcpy(newArray, pObjects, sizeof(deObject*) * pObjectSize);
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = newSize;
	}
	
	pObjects[pObjectCount] = object;
	pObjectCount++;
}

void decObjectList::Insert(deObject *object, int index){
	if(index < 0 || index > pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	if(pObjectCount == pObjectSize){
		int newSize = pObjectSize * 3 / 2 + 1;
		deObject **newArray = new deObject*[newSize];
		if(pObjects){
			memcpy(newArray, pObjects, sizeof(deObject*) * pObjectSize);
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = newSize;
	}
	
	for(i=pObjectCount; i>index; i--){
		pObjects[i] = pObjects[i - 1];
	}
	pObjects[index] = object;
	pObjectCount++;
}

void decObjectList::Move(int from, int to){
	deObject *tempObject;
	int i;
	
	if(from < 0 || from >= pObjectCount || to < 0 || to > pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	if(to < from){
		tempObject = pObjects[from];
		for(i=from; i>to; i--){
			pObjects[i] = pObjects[i - 1];
		}
		pObjects[to] = tempObject;
		
	}else if(to > from){
		tempObject = pObjects[from];
		for(i=from; i<to; i++){
			pObjects[i] = pObjects[i + 1];
		}
		
		if(to == pObjectCount){
			pObjects[pObjectCount - 1] = tempObject;
			
		}else{
			pObjects[to] = tempObject;
		}
	}
}

void decObjectList::RemoveFrom(int index){
	if(index < 0 || index >= pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	int p;
	
	if(pObjects[index]){
		pObjects[index]->FreeReference();
	}
	for(p= index +1; p<pObjectCount; p++){
		pObjects[p - 1] = pObjects[p];
	}
	pObjectCount--;
}

void decObjectList::RemoveAll(){
	while(pObjectCount > 0){
		pObjectCount--;
		if(pObjects[pObjectCount]){
			pObjects[pObjectCount]->FreeReference();
		}
	}
}



bool decObjectList::Equals(const decObjectList &list) const{
	int p;
	
	if(list.pObjectCount != pObjectCount){
		return false;
	}
	
	for(p=0; p<pObjectCount; p++){
		if(list.pObjects[p] != pObjects[p]){
			return false;
		}
	}
	
	return true;
}

decObjectList decObjectList::GetHead(int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pObjectCount){
		count = pObjectCount;
	}
	if(count == 0){
		return decObjectList();
	}
	
	decObjectList list(count);
	deObject::Ref object;
	
	for(list.pObjectCount=0; list.pObjectCount<count; list.pObjectCount++){
		object = pObjects[list.pObjectCount];
		list.pObjects[list.pObjectCount] = object;
	}
	
	return list;
}

void decObjectList::GetHead(decObjectList &list, int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pObjectCount){
		count = pObjectCount;
	}
	
	deObject::Ref object;
	
	if(count > list.pObjectSize){
		deObject **newArray = new deObject*[count];
		if(list.pObjects){
			delete [] list.pObjects;
		}
		list.pObjects = newArray;
		list.pObjectSize = count;
	}
	
	for(list.pObjectCount=0; list.pObjectCount<count; list.pObjectCount++){
		object = pObjects[list.pObjectCount];
		list.pObjects[list.pObjectCount] = object;
	}
}

decObjectList decObjectList::GetTail(int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pObjectCount){
		count = pObjectCount;
	}
	if(count == 0){
		return decObjectList();
	}
	
	decObjectList list(count);
	int from = pObjectCount - count;
	deObject::Ref object;
	
	for(list.pObjectCount=0; list.pObjectCount<count; list.pObjectCount++){
		object = pObjects[from + list.pObjectCount];
		list.pObjects[list.pObjectCount] = object;
	}
	
	return list;
}

void decObjectList::GetTail(decObjectList &list, int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pObjectCount){
		count = pObjectCount;
	}
	
	if(count > list.pObjectSize){
		deObject **newArray = new deObject*[count];
		if(list.pObjects){
			delete [] list.pObjects;
		}
		list.pObjects = newArray;
		list.pObjectSize = count;
	}
	
	int from = pObjectCount - count;
	deObject::Ref object;
	
	for(list.pObjectCount=0; list.pObjectCount<count; list.pObjectCount++){
		object = pObjects[from + list.pObjectCount];
		list.pObjects[list.pObjectCount] = object;
	}
}

decObjectList decObjectList::GetMiddle(int from, int to) const{
	if(from < 0 || to < from){
		DETHROW(deeInvalidParam);
	}
	
	int count = to - from + 1;
	if(count > pObjectCount){
		count = pObjectCount - from;
	}
	if(count == 0){
		return decObjectList();
	}
	
	decObjectList list(count);
	deObject::Ref object;
	
	for(list.pObjectCount=0; list.pObjectCount<count; list.pObjectCount++){
		object = pObjects[from + list.pObjectCount];
		list.pObjects[list.pObjectCount] = object;
	}
	
	return list;
	
}

void decObjectList::GetMiddle(decObjectList &list, int from, int to) const{
	if(from < 0 || to < from){
		DETHROW(deeInvalidParam);
	}
	
	int count = to - from + 1;
	if(count > pObjectCount){
		count = pObjectCount - from;
	}
	deObject::Ref object;
	
	if(count > list.pObjectSize){
		deObject **newArray = new deObject*[count];
		if(list.pObjects){
			delete [] list.pObjects;
		}
		list.pObjects = newArray;
		list.pObjectSize = count;
	}
	
	for(list.pObjectCount=0; list.pObjectCount<count; list.pObjectCount++){
		object = pObjects[from + list.pObjectCount];
		list.pObjects[list.pObjectCount] = object;
	}
}

decObjectList decObjectList::GetSliced(int from, int to, int step) const{
	// TODO Imeplementation
	return GetMiddle(from, to);
}

void decObjectList::GetSliced(decObjectList &list, int from, int to, int step) const{
	// TODO Imeplementation
	GetMiddle(list, from, to);
}



// Operators
//////////////

bool decObjectList::operator==(const decObjectList &list) const{
	return Equals(list);
}

decObjectList decObjectList::operator+(const decObjectList &list) const{
	decObjectList nlist(pObjectCount + list.pObjectCount);
	deObject::Ref object;
	int r;
	
	for(r=0; r<pObjectCount; r++){
		object = pObjects[r];
		nlist.pObjects[r] = object;
	}
	
	for(r=0; r<list.pObjectCount; r++){
		object = list.pObjects[r];
		nlist.pObjects[pObjectCount + r] = object;
	}
	
	return nlist;
}

deObject *decObjectList::operator[](int position) const{
	return GetAt(position);
}



decObjectList &decObjectList::operator=(const decObjectList &list){
	deObject::Ref object;
	
	RemoveAll();
	
	if(list.pObjectCount > pObjectSize){
		deObject **newArray = new deObject*[list.pObjectCount];
		if(pObjects){
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = list.pObjectCount;
	}
	
	for(pObjectCount=0; pObjectCount<list.pObjectCount; pObjectCount++){
		object = list.pObjects[pObjectCount];
		pObjects[pObjectCount] = object;
	}
	
	return *this;
}

decObjectList &decObjectList::operator+=(const decObjectList &list){
	if(list.pObjectCount > 0){
		int r, count = pObjectCount + list.pObjectCount;
		deObject::Ref object;
		
		if(count > pObjectSize){
			deObject **newArray = new deObject*[count];
			if(pObjects){
				memcpy(newArray, pObjects, sizeof(deObject*) * pObjectSize);
				delete [] pObjects;
			}
			pObjects = newArray;
			pObjectSize = count;
		}
		
		for(r=0; r<count; r++){
			object = list.pObjects[r];
			pObjects[pObjectCount] = object;
			pObjectCount++;
		}
	}
	
	return *this;
}
