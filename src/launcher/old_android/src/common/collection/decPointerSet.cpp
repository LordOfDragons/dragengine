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

#include "decPointerSet.h"

#include "../exceptions.h"



// Class decPointerSet
////////////////////////

// Constructor, destructor
////////////////////////////

decPointerSet::decPointerSet(){
	pPointers = NULL;
	pPointerCount = 0;
	pPointerSize = 0;
}

decPointerSet::decPointerSet(int capacity){
	if(capacity < 0){
		DETHROW(deeInvalidParam);
	}
	
	pPointers = NULL;
	pPointerCount = 0;
	pPointerSize = 0;
	
	if(capacity > 0){
		pPointers = new void*[capacity];
		pPointerSize = capacity;
	}
}

decPointerSet::decPointerSet(const decPointerSet &set){
	const int count = set.GetCount();
	
	pPointers = NULL;
	pPointerCount = 0;
	pPointerSize = 0;
	
	if(count > 0){
		pPointers = new void*[count];
		pPointerSize = count;
		
		memcpy(pPointers, set.pPointers, sizeof(void*) * count);
		pPointerCount = count;
	}
}

decPointerSet::~decPointerSet(){
	if(pPointers){
		delete [] pPointers;
	}
}



// Management
///////////////

void *decPointerSet::GetAt(int position) const{
	if(position < 0 || position >= pPointerCount){
		DETHROW(deeInvalidParam);
	}
	
	return pPointers[position];
}

int decPointerSet::IndexOf(void *pointer) const{
	int p;
	
	for(p=0; p<pPointerCount; p++){
		if(pointer == pPointers[p]){
			return p;
		}
	}
	
	return -1;
}

bool decPointerSet::Has(void *pointer) const{
	int p;
	
	for(p=0; p<pPointerCount; p++){
		if(pointer == pPointers[p]){
			return true;
		}
	}
	
	return false;
}

void decPointerSet::Add(void *pointer){
	if(Has(pointer)){
		DETHROW(deeInvalidParam);
	}
	
	if(pPointerCount == pPointerSize){
		const int newSize = pPointerSize * 3 / 2 + 1;
		void ** const newArray = new void*[newSize];
		if(pPointers){
			memcpy(newArray, pPointers, sizeof(void*) * pPointerSize);
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = newSize;
	}
	
	pPointers[pPointerCount] = pointer;
	pPointerCount++;
}

void decPointerSet::AddIfAbsent(void *pointer){
	if(Has(pointer)){
		return;
	}
	
	if(pPointerCount == pPointerSize){
		const int newSize = pPointerSize * 3 / 2 + 1;
		void ** const newArray = new void*[newSize];
		if(pPointers){
			memcpy(newArray, pPointers, sizeof(void*) * pPointerSize);
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = newSize;
	}
	
	pPointers[pPointerCount] = pointer;
	pPointerCount++;
}

void decPointerSet::RemoveFrom(int position){
	if(position < 0 || position >= pPointerCount){
		DETHROW(deeInvalidParam);
	}
	
	int p;
	
	for(p=position+1; p<pPointerCount; p++){
		pPointers[p - 1] = pPointers[p];
	}
	
	pPointerCount--;
}

void decPointerSet::Remove(void *pointer){
	const int position = IndexOf(pointer);
	int p;
	
	if(position == -1){
		DETHROW(deeInvalidParam);
	}
	
	for(p=position+1; p<pPointerCount; p++){
		pPointers[p - 1] = pPointers[p];
	}
	
	pPointerCount--;
}

void decPointerSet::RemoveIfPresent(void *pointer){
	const int position = IndexOf(pointer);
	int p;
	
	if(position == -1){
		return;
	}
	
	for(p=position+1; p<pPointerCount; p++){
		pPointers[p - 1] = pPointers[p];
	}
	
	pPointerCount--;
}

void decPointerSet::RemoveAll(){
	pPointerCount = 0;
}



bool decPointerSet::Equals(const decPointerSet &set) const{
	int p;
	
	if(set.pPointerCount != pPointerCount){
		return false;
	}
	
	for(p=0; p<pPointerCount; p++){
		if(!set.Has(pPointers[p])){
			return false;
		}
	}
	
	return true;
}

decPointerSet decPointerSet::GetHead(int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pPointerCount){
		count = pPointerCount;
	}
	if(count == 0){
		return decPointerSet();
	}
	
	decPointerSet set(count);
	memcpy(set.pPointers, pPointers, sizeof(void*) * count);
	set.pPointerCount = count;
	
	return set;
}

void decPointerSet::GetHead(decPointerSet &set, int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pPointerCount){
		count = pPointerCount;
	}
	set.pPointerCount = 0;
	
	if(count > set.pPointerSize){
		void ** const newArray = new void*[count];
		if(set.pPointers){
			delete [] set.pPointers;
		}
		set.pPointers = newArray;
		set.pPointerSize = count;
	}
	
	if(count > 0){
		memcpy(set.pPointers, pPointers, sizeof(void*) * count);
		set.pPointerCount = count;
	}
}

decPointerSet decPointerSet::GetTail(int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pPointerCount){
		count = pPointerCount;
	}
	if(count == 0){
		return decPointerSet();
	}
	
	decPointerSet set(count);
	memcpy(set.pPointers, pPointers + (pPointerCount - count), sizeof(void*) * count);
	set.pPointerCount = count;
	
	return set;
}

void decPointerSet::GetTail(decPointerSet &set, int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pPointerCount){
		count = pPointerCount;
	}
	set.pPointerCount = 0;
	
	if(count > set.pPointerSize){
		void ** const newArray = new void*[count];
		if(set.pPointers){
			delete [] set.pPointers;
		}
		set.pPointers = newArray;
		set.pPointerSize = count;
	}
	
	if(count > 0){
		memcpy(set.pPointers, pPointers + (pPointerCount - count), sizeof(void*) * count);
		set.pPointerCount = count;
	}
}

decPointerSet decPointerSet::GetMiddle(int from, int to) const{
	if(from < 0 || to < from){
		DETHROW(deeInvalidParam);
	}
	
	int count = to - from + 1;
	if(count > pPointerCount){
		count = pPointerCount - from;
	}
	if(count == 0){
		return decPointerSet();
	}
	
	decPointerSet set(count);
	memcpy(set.pPointers, pPointers + from, sizeof(void*) * count);
	set.pPointerCount = count;
	
	return set;
	
}

void decPointerSet::GetMiddle(decPointerSet &set, int from, int to) const{
	if(from < 0 || to < from){
		DETHROW(deeInvalidParam);
	}
	
	int count = to - from + 1;
	if(count > pPointerCount){
		count = pPointerCount - from;
	}
	set.pPointerCount = 0;
	
	if(count > set.pPointerSize){
		void ** const newArray = new void*[count];
		if(set.pPointers){
			delete [] set.pPointers;
		}
		set.pPointers = newArray;
		set.pPointerSize = count;
	}
	
	if(count > 0){
		memcpy(set.pPointers, pPointers + from, sizeof(void*) * count);
		set.pPointerCount = count;
	}
}

decPointerSet decPointerSet::GetSliced(int from, int to, int step) const{
	// temporary hack: same as middle
	return GetMiddle(from, to);
}

void decPointerSet::GetSliced(decPointerSet &set, int from, int to, int step) const{
	// temporary hack: same as middle
	GetMiddle(set, from, to);
}



// Operators
//////////////

bool decPointerSet::operator==(const decPointerSet &set) const{
	return Equals(set);
}

decPointerSet decPointerSet::operator+(const decPointerSet &set) const{
	decPointerSet nset(pPointerCount + set.pPointerCount);
	int p;
	
	memcpy(nset.pPointers, pPointers, sizeof(void*) * pPointerCount);
	
	for(p=0; p<set.pPointerCount; p++){
		nset.AddIfAbsent(set.GetAt(p));
	}
	
	return nset;
}

void *decPointerSet::operator[](int position) const{
	return GetAt(position);
}



decPointerSet &decPointerSet::operator=(const decPointerSet &set){
	pPointerCount = 0;
	
	if(set.pPointerCount > pPointerSize){
		void ** const newArray = new void*[set.pPointerCount];
		if(pPointers){
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = set.pPointerCount;
	}
	
	if(set.pPointerCount > 0){
		memcpy(pPointers, set.pPointers, sizeof(void*) * set.pPointerCount);
		pPointerCount = set.pPointerCount;
	}
	
	return *this;
}

decPointerSet &decPointerSet::operator+=(const decPointerSet &set){
	if(set.pPointerCount > 0){
		int p;
		
		if(pPointerCount + set.pPointerCount > pPointerSize){
			const int newSize = pPointerCount + set.pPointerCount;
			void ** const newArray = new void*[newSize];
			if(pPointers){
				memcpy(newArray, pPointers, sizeof(void*) * pPointerSize);
				delete [] pPointers;
			}
			pPointers = newArray;
			pPointerSize = newSize;
		}
		
		for(p=0; p<set.pPointerCount; p++){
			AddIfAbsent(set.GetAt(p));
		}
	}
	
	return *this;
}
