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

#include "decPointerOrderedSet.h"

#include "../exceptions.h"



// Class decPointerOrderedSet
///////////////////////////////

// Constructor, destructor
////////////////////////////

decPointerOrderedSet::decPointerOrderedSet() :
pPointers(NULL),
pPointerCount(0),
pPointerSize(0){
}

decPointerOrderedSet::decPointerOrderedSet(int capacity) :
pPointers(NULL),
pPointerCount(0),
pPointerSize(0)
{
	if(capacity < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(capacity == 0){
		return;
	}
	
	pPointers = new void*[capacity];
	pPointerSize = capacity;
}

decPointerOrderedSet::decPointerOrderedSet(const decPointerOrderedSet &set) :
pPointers(NULL),
pPointerCount(0),
pPointerSize(0)
{
	const int count = set.GetCount();
	if(count == 0){
		return;
	}
	
	pPointers = new void*[count];
	pPointerSize = count;
	
	for(pPointerCount=0; pPointerCount<count; pPointerCount++){
		pPointers[pPointerCount] = set.pPointers[pPointerCount];
	}
}

decPointerOrderedSet::~decPointerOrderedSet(){
	RemoveAll();
	
	if(pPointers){
		delete [] pPointers;
	}
}



// Management
///////////////

void *decPointerOrderedSet::GetAt(int index) const{
	if(index < 0 || index >= pPointerCount){
		DETHROW(deeInvalidParam);
	}
	
	return pPointers[index];
}

int decPointerOrderedSet::IndexOf(void *pointer) const{
	int p;
	
	for(p=0; p<pPointerCount; p++){
		if(pointer == pPointers[p]){
			return p;
		}
	}
	
	return -1;
}

bool decPointerOrderedSet::Has(void *pointer) const{
	int p;
	
	for(p=0; p<pPointerCount; p++){
		if(pointer == pPointers[p]){
			return true;
		}
	}
	
	return false;
}

void decPointerOrderedSet::Add(void *pointer){
	if(Has(pointer)){
		DETHROW(deeInvalidParam);
	}
	
	if(pPointerCount == pPointerSize){
		int newSize = pPointerSize * 3 / 2 + 1;
		void **newArray = new void*[newSize];
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

void decPointerOrderedSet::AddIfAbsent(void *pointer){
	if(Has(pointer)){
		return;
	}
	
	if(pPointerCount == pPointerSize){
		int newSize = pPointerSize * 3 / 2 + 1;
		void **newArray = new void*[newSize];
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

void decPointerOrderedSet::Insert(void *pointer, int index){
	if(Has(pointer) || index < 0 || index > pPointerCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	if(pPointerCount == pPointerSize){
		int newSize = pPointerSize * 3 / 2 + 1;
		void **newArray = new void*[newSize];
		if(pPointers){
			memcpy(newArray, pPointers, sizeof(void*) * pPointerSize);
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = newSize;
	}
	
	for(i=pPointerCount; i>index; i--){
		pPointers[i] = pPointers[i - 1];
	}
	pPointers[index] = pointer;
	pPointerCount++;
}

void decPointerOrderedSet::Move(void *pointer, int to){
	const int from = IndexOf(pointer);
	if(from == -1 || to < 0 || to >= pPointerCount){
		DETHROW(deeInvalidParam);
	}
	
	void * const tempPointer = pPointers[from];
	int i;
	
	if(to < from){
		for(i=from; i>to; i--){
			pPointers[i] = pPointers[i - 1];
		}
		
	}else if(to > from){
		for(i=from; i<to; i++){
			pPointers[i] = pPointers[i + 1];
		}
	}
	
	pPointers[to] = tempPointer;
}

void decPointerOrderedSet::Remove(void *pointer){
	int p, position = IndexOf(pointer);
	
	if(position == -1){
		DETHROW(deeInvalidParam);
	}
	
	for(p=position+1; p<pPointerCount; p++){
		pPointers[p - 1] = pPointers[p];
	}
	pPointerCount--;
}

void decPointerOrderedSet::RemoveIfPresent(void *pointer){
	int p, position = IndexOf(pointer);
	
	if(position == -1){
		return;
	}
	
	for(p=position+1; p<pPointerCount; p++){
		pPointers[p - 1] = pPointers[p];
	}
	pPointerCount--;
}

void decPointerOrderedSet::RemoveFrom(int index){
	if(index < 0 || index >= pPointerCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	for(i=index+1; i<pPointerCount; i++){
		pPointers[i - 1] = pPointers[i];
	}
	pPointerCount--;
}

void decPointerOrderedSet::RemoveAll(){
	pPointerCount = 0;
}



bool decPointerOrderedSet::Equals(const decPointerOrderedSet &set) const{
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

decPointerOrderedSet decPointerOrderedSet::GetHead(int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pPointerCount){
		count = pPointerCount;
	}
	if(count == 0){
		return decPointerOrderedSet();
	}
	
	decPointerOrderedSet set(count);
	for(set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++){
		set.pPointers[set.pPointerCount] = pPointers[set.pPointerCount];
	}
	
	return set;
}

void decPointerOrderedSet::GetHead(decPointerOrderedSet &set, int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pPointerCount){
		count = pPointerCount;
	}
	
	if(count > set.pPointerSize){
		void **newArray = new void*[count];
		if(set.pPointers){
			delete [] set.pPointers;
		}
		set.pPointers = newArray;
		set.pPointerSize = count;
	}
	
	for(set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++){
		set.pPointers[set.pPointerCount] = pPointers[set.pPointerCount];
	}
}

decPointerOrderedSet decPointerOrderedSet::GetTail(int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pPointerCount){
		count = pPointerCount;
	}
	if(count == 0){
		return decPointerOrderedSet();
	}
	
	decPointerOrderedSet set(count);
	int from = pPointerCount - count;
	
	for(set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++){
		set.pPointers[set.pPointerCount] = pPointers[from + set.pPointerCount];
	}
	
	return set;
}

void decPointerOrderedSet::GetTail(decPointerOrderedSet &set, int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pPointerCount){
		count = pPointerCount;
	}
	
	if(count > set.pPointerSize){
		void **newArray = new void*[count];
		if(set.pPointers){
			delete [] set.pPointers;
		}
		set.pPointers = newArray;
		set.pPointerSize = count;
	}
	
	int from = pPointerCount - count;
	
	for(set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++){
		set.pPointers[set.pPointerCount] = pPointers[from + set.pPointerCount];
	}
}

decPointerOrderedSet decPointerOrderedSet::GetMiddle(int from, int to) const{
	if(from < 0 || to < from){
		DETHROW(deeInvalidParam);
	}
	
	int count = to - from + 1;
	if(count > pPointerCount){
		count = pPointerCount - from;
	}
	if(count == 0){
		return decPointerOrderedSet();
	}
	
	decPointerOrderedSet set(count);
	
	for(set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++){
		set.pPointers[set.pPointerCount] = pPointers[from + set.pPointerCount];
	}
	
	return set;
	
}

void decPointerOrderedSet::GetMiddle(decPointerOrderedSet &set, int from, int to) const{
	if(from < 0 || to < from){
		DETHROW(deeInvalidParam);
	}
	
	int count = to - from + 1;
	if(count > pPointerCount){
		count = pPointerCount - from;
	}
	
	if(count > set.pPointerSize){
		void **newArray = new void*[count];
		if(set.pPointers){
			delete [] set.pPointers;
		}
		set.pPointers = newArray;
		set.pPointerSize = count;
	}
	
	for(set.pPointerCount=0; set.pPointerCount<count; set.pPointerCount++){
		set.pPointers[set.pPointerCount] = pPointers[from + set.pPointerCount];
	}
}

decPointerOrderedSet decPointerOrderedSet::GetSliced(int from, int to, int step) const{
	// temporary hack: same as middle
	return GetMiddle(from, to);
}

void decPointerOrderedSet::GetSliced(decPointerOrderedSet &set, int from, int to, int step) const{
	// temporary hack: same as middle
	GetMiddle(set, from, to);
}



void decPointerOrderedSet::Visit(decPointerVisitor &visitor, int from, int to, int step) const{
	DEASSERT_TRUE(step != 0)
	
	if(from < 0){
		from = pPointerCount - from;
	}
	DEASSERT_TRUE(from >= 0)
	DEASSERT_TRUE(from < pPointerCount)
	
	if(to < 0){
		to = pPointerCount - to;
	}
	DEASSERT_TRUE(to >= 0)
	
	int i;
	if(step > 0){
		DEASSERT_TRUE(to <= pPointerCount)
		
		for(i=from; i<to; i+=step){
			visitor(pPointers[i]);
		}
		
	}else{
		DEASSERT_TRUE(to < pPointerCount)
		
		for(i=from; i>=to; i+=step){
			visitor(pPointers[i]);
		}
	}
}

bool decPointerOrderedSet::Find(decPointerEvaluator &evaluator, void *&found,
int from, int to, int step) const{
	DEASSERT_TRUE(step != 0)
	
	if(from < 0){
		from = pPointerCount - from;
	}
	DEASSERT_TRUE(from >= 0)
	DEASSERT_TRUE(from < pPointerCount)
	
	if(to < 0){
		to = pPointerCount - to;
	}
	DEASSERT_TRUE(to >= 0)
	
	int i;
	if(step > 0){
		DEASSERT_TRUE(to <= pPointerCount)
		
		for(i=from; i<to; i+=step){
			if(evaluator(pPointers[i])){
				found = pPointers[i];
				return true;
			}
		}
		
	}else{
		DEASSERT_TRUE(to < pPointerCount)
		
		for(i=from; i>=to; i+=step){
			if(evaluator(pPointers[i])){
				found = pPointers[i];
				return true;
			}
		}
	}
	return false;
}

decPointerOrderedSet decPointerOrderedSet::Collect(decPointerEvaluator &evaluator, int from, int to, int step) const{
	DEASSERT_TRUE(step != 0)
	
	if(from < 0){
		from = pPointerCount - from;
	}
	DEASSERT_TRUE(from >= 0)
	DEASSERT_TRUE(from < pPointerCount)
	
	if(to < 0){
		to = pPointerCount - to;
	}
	DEASSERT_TRUE(to >= 0)
	
	decPointerOrderedSet collected;
	int i;
	if(step > 0){
		DEASSERT_TRUE(to <= pPointerCount)
		
		for(i=from; i<to; i+=step){
			if(evaluator(pPointers[i])){
				collected.Add(pPointers[i]);
			}
		}
		
	}else{
		DEASSERT_TRUE(to < pPointerCount)
		
		for(i=from; i>=to; i+=step){
			if(evaluator(pPointers[i])){
				collected.Add(pPointers[i]);
			}
		}
	}
	return collected;
}

void decPointerOrderedSet::RemoveIf(decPointerEvaluator &evaluator, int from, int to, int step){
	DEASSERT_TRUE(step != 0)
	
	if(from < 0){
		from = pPointerCount - from;
	}
	DEASSERT_TRUE(from >= 0)
	DEASSERT_TRUE(from < pPointerCount)
	
	if(to < 0){
		to = pPointerCount - to;
	}
	DEASSERT_TRUE(to >= 0)
	
	int i;
	if(step > 0){
		DEASSERT_TRUE(to <= pPointerCount)
		
		for(i=from; i<to; i+=step){
			if(evaluator(pPointers[i])){
				RemoveFrom(i);
				i--;
				to--;
			}
		}
		
	}else{
		DEASSERT_TRUE(to < pPointerCount)
		
		for(i=from; i>=to; i+=step){
			if(evaluator(pPointers[i])){
				RemoveFrom(i);
				i++;
				to++;
			}
		}
	}
}

void decPointerOrderedSet::Sort(decPointerComparator &comparator){
	if(pPointerCount > 1){
		pSort(comparator, 0, pPointerCount - 1);
	}
}

decPointerOrderedSet decPointerOrderedSet::GetSorted(decPointerComparator &comparator) const{
	decPointerOrderedSet copy(*this);
	copy.Sort(comparator);
	return copy;
}



// Operators
//////////////

bool decPointerOrderedSet::operator==(const decPointerOrderedSet &set) const{
	return Equals(set);
}

decPointerOrderedSet decPointerOrderedSet::operator+(const decPointerOrderedSet &set) const{
	decPointerOrderedSet nset(pPointerCount + set.pPointerCount);
	int i;
	
	for(i=0; i<pPointerCount; i++){
		nset.pPointers[i] = pPointers[i];
	}
	
	for(i=0; i<set.pPointerCount; i++){
		nset.AddIfAbsent(set.pPointers[i]);
	}
	
	return nset;
}

void *decPointerOrderedSet::operator[](int index) const{
	return GetAt(index);
}



decPointerOrderedSet &decPointerOrderedSet::operator=(const decPointerOrderedSet &set){
	if(&set == this){
		return *this;
	}
	
	RemoveAll();
	
	if(set.pPointerCount > pPointerSize){
		void **newArray = new void*[set.pPointerCount];
		if(pPointers){
			delete [] pPointers;
		}
		pPointers = newArray;
		pPointerSize = set.pPointerCount;
	}
	
	for(pPointerCount=0; pPointerCount<set.pPointerCount; pPointerCount++){
		pPointers[pPointerCount] = set.pPointers[pPointerCount];
	}
	
	return *this;
}

decPointerOrderedSet &decPointerOrderedSet::operator+=(const decPointerOrderedSet &set){
	if(set.pPointerCount > 0){
		int i, count = pPointerCount + set.pPointerCount;
		
		if(count > pPointerSize){
			void **newArray = new void*[count];
			if(pPointers){
				memcpy(newArray, pPointers, sizeof(void*) * pPointerSize);
				delete [] pPointers;
			}
			pPointers = newArray;
			pPointerSize = count;
		}
		
		for(i=0; i<set.pPointerCount; i++){
			AddIfAbsent(set.pPointers[i]);
		}
	}
	
	return *this;
}



// Private Functions
//////////////////////

void decPointerOrderedSet::pSort(decPointerComparator &comparator, int left, int right){
	void * const pivot = pPointers[left];
	const int r_hold = right;
	const int l_hold = left;
	
	while(left < right){
		while(left < right && comparator(pPointers[right], pivot) >= 0){
			right--;
		}
		if(left != right){
			pPointers[left] = pPointers[right];
			left++;
		}
		while(left < right && comparator(pPointers[left], pivot) <= 0){
			left++;
		}
		if(left != right){
			pPointers[right] = pPointers[left];
			right--;
		}
	}
	
	pPointers[left] = pivot;
	if(l_hold < left){
		pSort(comparator, l_hold, left - 1);
	}
	if(r_hold > left){
		pSort(comparator, left + 1, r_hold);
	}
}
