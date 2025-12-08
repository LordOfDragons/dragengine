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

#include "decThreadSafeObjectOrderedSet.h"
#include "../exceptions.h"
#include "../../threading/deThreadSafeObject.h"



// Class decThreadSafeObjectOrderedSet
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

decThreadSafeObjectOrderedSet::decThreadSafeObjectOrderedSet(){
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
}

decThreadSafeObjectOrderedSet::decThreadSafeObjectOrderedSet(int capacity){
	if(capacity < 0){
		DETHROW(deeInvalidParam);
	}
	
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	
	if(capacity > 0){
		pObjects = new deThreadSafeObject*[capacity];
		pObjectSize = capacity;
	}
}

decThreadSafeObjectOrderedSet::decThreadSafeObjectOrderedSet(const decThreadSafeObjectOrderedSet &set){
	int count = set.GetCount();
	
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	
	if(count > 0){
		deThreadSafeObject::Ref object;
		
		pObjects = new deThreadSafeObject*[count];
		pObjectSize = count;
		
		for(pObjectCount=0; pObjectCount<count; pObjectCount++){
			object = set.pObjects[pObjectCount];
			pObjects[pObjectCount] = object;
		}
	}
}

decThreadSafeObjectOrderedSet::~decThreadSafeObjectOrderedSet(){
	RemoveAll();
	
	if(pObjects){
		delete [] pObjects;
	}
}



// Management
///////////////

deThreadSafeObject *decThreadSafeObjectOrderedSet::GetAt(int index) const{
	if(index < 0 || index >= pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	return pObjects[index];
}

int decThreadSafeObjectOrderedSet::IndexOf(deThreadSafeObject *object) const{
	int p;
	
	for(p=0; p<pObjectCount; p++){
		if(object == pObjects[p]){
			return p;
		}
	}
	
	return -1;
}

bool decThreadSafeObjectOrderedSet::Has(deThreadSafeObject *object) const{
	int p;
	
	for(p=0; p<pObjectCount; p++){
		if(object == pObjects[p]){
			return true;
		}
	}
	
	return false;
}

void decThreadSafeObjectOrderedSet::SetAt(int index, deThreadSafeObject *object){
	if(index < 0 || index >= pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	if(object == pObjects[index]){
		return;
	}
	
	if(Has(object)){
		DETHROW(deeInvalidParam);
	}
	
	if(pObjects[index]){
		pObjects[index]->FreeReference();
	}
	
	pObjects[index] = object;
}

void decThreadSafeObjectOrderedSet::Add(deThreadSafeObject *object){
	if(Has(object)){
		DETHROW(deeInvalidParam);
	}
	
	if(pObjectCount == pObjectSize){
		int newSize = pObjectSize * 3 / 2 + 1;
		deThreadSafeObject **newArray = new deThreadSafeObject*[newSize];
		if(pObjects){
			memcpy(newArray, pObjects, sizeof(deThreadSafeObject*) * pObjectSize);
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = newSize;
	}
	
	pObjects[pObjectCount] = object;
	pObjectCount++;
}

void decThreadSafeObjectOrderedSet::AddIfAbsent(deThreadSafeObject *object){
	if(Has(object)){
		return;
	}
	
	if(pObjectCount == pObjectSize){
		int newSize = pObjectSize * 3 / 2 + 1;
		deThreadSafeObject **newArray = new deThreadSafeObject*[newSize];
		if(pObjects){
			memcpy(newArray, pObjects, sizeof(deThreadSafeObject*) * pObjectSize);
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = newSize;
	}
	
	pObjects[pObjectCount] = object;
	pObjectCount++;
}

void decThreadSafeObjectOrderedSet::Insert(deThreadSafeObject *object, int index){
	if(Has(object) || index < 0 || index > pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	if(pObjectCount == pObjectSize){
		int newSize = pObjectSize * 3 / 2 + 1;
		deThreadSafeObject **newArray = new deThreadSafeObject*[newSize];
		if(pObjects){
			memcpy(newArray, pObjects, sizeof(deThreadSafeObject*) * pObjectSize);
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

void decThreadSafeObjectOrderedSet::Move(deThreadSafeObject *object, int to){
	const int from = IndexOf(object);
	if(from == -1 || to < 0 || to >= pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	deThreadSafeObject * const tempObject = pObjects[from];
	int i;
	
	if(to < from){
		for(i=from; i>to; i--){
			pObjects[i] = pObjects[i - 1];
		}
		
	}else if(to > from){
		for(i=from; i<to; i++){
			pObjects[i] = pObjects[i + 1];
		}
	}
	
	pObjects[to] = tempObject;
}

void decThreadSafeObjectOrderedSet::Remove(deThreadSafeObject *object){
	int p, position = IndexOf(object);
	
	if(position == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(pObjects[position]){
		pObjects[position]->FreeReference();
	}
	for(p=position+1; p<pObjectCount; p++){
		pObjects[p - 1] = pObjects[p];
	}
	pObjectCount--;
}

void decThreadSafeObjectOrderedSet::RemoveIfPresent(deThreadSafeObject *object){
	int p, position = IndexOf(object);
	
	if(position == -1){
		return;
	}
	
	if(pObjects[position]){
		pObjects[position]->FreeReference();
	}
	for(p=position+1; p<pObjectCount; p++){
		pObjects[p - 1] = pObjects[p];
	}
	pObjectCount--;
}

void decThreadSafeObjectOrderedSet::RemoveFrom(int index){
	if(index < 0 || index >= pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	if(pObjects[index]){
		pObjects[index]->FreeReference();
	}
	
	int i;
	for(i=index+1; i<pObjectCount; i++){
		pObjects[i - 1] = pObjects[i];
	}
	pObjectCount--;
}

void decThreadSafeObjectOrderedSet::RemoveAll(){
	while(pObjectCount > 0){
		pObjectCount--;
		if(pObjects[pObjectCount]){
			pObjects[pObjectCount]->FreeReference();
		}
	}
}



bool decThreadSafeObjectOrderedSet::Equals(const decThreadSafeObjectOrderedSet &set) const{
	int p;
	
	if(set.pObjectCount != pObjectCount){
		return false;
	}
	
	for(p=0; p<pObjectCount; p++){
		if(!set.Has(pObjects[p])){
			return false;
		}
	}
	
	return true;
}

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::GetHead(int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pObjectCount){
		count = pObjectCount;
	}
	if(count == 0){
		return decThreadSafeObjectOrderedSet();
	}
	
	decThreadSafeObjectOrderedSet set(count);
	deThreadSafeObject::Ref object;
	
	for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
		object = pObjects[set.pObjectCount];
		set.pObjects[set.pObjectCount] = object;
	}
	
	return set;
}

void decThreadSafeObjectOrderedSet::GetHead(decThreadSafeObjectOrderedSet &set, int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pObjectCount){
		count = pObjectCount;
	}
	
	deThreadSafeObject::Ref object;
	
	if(count > set.pObjectSize){
		deThreadSafeObject **newArray = new deThreadSafeObject*[count];
		if(set.pObjects){
			delete [] set.pObjects;
		}
		set.pObjects = newArray;
		set.pObjectSize = count;
	}
	
	for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
		object = pObjects[set.pObjectCount];
		set.pObjects[set.pObjectCount] = object;
	}
}

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::GetTail(int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pObjectCount){
		count = pObjectCount;
	}
	if(count == 0){
		return decThreadSafeObjectOrderedSet();
	}
	
	decThreadSafeObjectOrderedSet set(count);
	int from = pObjectCount - count;
	deThreadSafeObject::Ref object;
	
	for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
		object = pObjects[from + set.pObjectCount];
		set.pObjects[set.pObjectCount] = object;
	}
	
	return set;
}

void decThreadSafeObjectOrderedSet::GetTail(decThreadSafeObjectOrderedSet &set, int count) const{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count > pObjectCount){
		count = pObjectCount;
	}
	
	if(count > set.pObjectSize){
		deThreadSafeObject **newArray = new deThreadSafeObject*[count];
		if(set.pObjects){
			delete [] set.pObjects;
		}
		set.pObjects = newArray;
		set.pObjectSize = count;
	}
	
	int from = pObjectCount - count;
	deThreadSafeObject::Ref object;
	
	for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
		object = pObjects[from + set.pObjectCount];
		set.pObjects[set.pObjectCount] = object;
	}
}

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::GetMiddle(int from, int to) const{
	if(from < 0 || to < from){
		DETHROW(deeInvalidParam);
	}
	
	int count = to - from + 1;
	if(count > pObjectCount){
		count = pObjectCount - from;
	}
	if(count == 0){
		return decThreadSafeObjectOrderedSet();
	}
	
	decThreadSafeObjectOrderedSet set(count);
	deThreadSafeObject::Ref object;
	
	for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
		object = pObjects[from + set.pObjectCount];
		set.pObjects[set.pObjectCount] = object;
	}
	
	return set;
	
}

void decThreadSafeObjectOrderedSet::GetMiddle(decThreadSafeObjectOrderedSet &set, int from, int to) const{
	if(from < 0 || to < from){
		DETHROW(deeInvalidParam);
	}
	
	int count = to - from + 1;
	if(count > pObjectCount){
		count = pObjectCount - from;
	}
	deThreadSafeObject::Ref object;
	
	if(count > set.pObjectSize){
		deThreadSafeObject **newArray = new deThreadSafeObject*[count];
		if(set.pObjects){
			delete [] set.pObjects;
		}
		set.pObjects = newArray;
		set.pObjectSize = count;
	}
	
	for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
		object = pObjects[from + set.pObjectCount];
		set.pObjects[set.pObjectCount] = object;
	}
}

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::GetSliced(int from, int to, int step) const{
	// temporary hack: same as middle
	return GetMiddle(from, to);
}

void decThreadSafeObjectOrderedSet::GetSliced(decThreadSafeObjectOrderedSet &set, int from, int to, int step) const{
	// temporary hack: same as middle
	GetMiddle(set, from, to);
}



void decThreadSafeObjectOrderedSet::Visit(decThreadSafeObjectVisitor &visitor,
int from, int to, int step) const{
	DEASSERT_TRUE(step != 0)
	
	if(from < 0){
		from = pObjectCount + from;
	}
	DEASSERT_TRUE(from >= 0)
	DEASSERT_TRUE(from < pObjectCount)
	
	if(to < 0){
		to = pObjectCount + to;
	}
	DEASSERT_TRUE(to >= 0)
	
	int i;
	if(step > 0){
		DEASSERT_TRUE(to <= pObjectCount)
		
		for(i=from; i<to; i+=step){
			visitor(pObjects[i]);
		}
		
	}else{
		DEASSERT_TRUE(to < pObjectCount)
		
		for(i=from; i>=to; i+=step){
			visitor(pObjects[i]);
		}
	}
}

bool decThreadSafeObjectOrderedSet::Find(decThreadSafeObjectEvaluator &evaluator,
deThreadSafeObject *&found, int from, int to, int step) const{
	DEASSERT_TRUE(step != 0)
	
	if(from < 0){
		from = pObjectCount + from;
	}
	DEASSERT_TRUE(from >= 0)
	DEASSERT_TRUE(from < pObjectCount)
	
	if(to < 0){
		to = pObjectCount + to;
	}
	DEASSERT_TRUE(to >= 0)
	
	int i;
	if(step > 0){
		DEASSERT_TRUE(to <= pObjectCount)
		
		for(i=from; i<to; i+=step){
			if(evaluator(pObjects[i])){
				found = pObjects[i];
				return true;
			}
		}
		
	}else{
		DEASSERT_TRUE(to < pObjectCount)
		
		for(i=from; i>=to; i+=step){
			if(evaluator(pObjects[i])){
				found = pObjects[i];
				return true;
			}
		}
	}
	return false;
}

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::Collect(
decThreadSafeObjectEvaluator &evaluator, int from, int to, int step) const{
	DEASSERT_TRUE(step != 0)
	
	if(from < 0){
		from = pObjectCount + from;
	}
	DEASSERT_TRUE(from >= 0)
	DEASSERT_TRUE(from < pObjectCount)
	
	if(to < 0){
		to = pObjectCount + to;
	}
	DEASSERT_TRUE(to >= 0)
	
	decThreadSafeObjectOrderedSet collected;
	int i;
	if(step > 0){
		DEASSERT_TRUE(to <= pObjectCount)
		
		for(i=from; i<to; i+=step){
			if(evaluator(pObjects[i])){
				collected.Add(pObjects[i]);
			}
		}
		
	}else{
		DEASSERT_TRUE(to < pObjectCount)
		
		for(i=from; i>=to; i+=step){
			if(evaluator(pObjects[i])){
				collected.Add(pObjects[i]);
			}
		}
	}
	return collected;
}

void decThreadSafeObjectOrderedSet::RemoveIf(decThreadSafeObjectEvaluator &evaluator,
int from, int to, int step){
	DEASSERT_TRUE(step != 0)
	
	if(from < 0){
		from = pObjectCount + from;
	}
	DEASSERT_TRUE(from >= 0)
	DEASSERT_TRUE(from < pObjectCount)
	
	if(to < 0){
		to = pObjectCount + to;
	}
	DEASSERT_TRUE(to >= 0)
	
	int i;
	if(step > 0){
		DEASSERT_TRUE(to <= pObjectCount)
		
		for(i=from; i<to; i+=step){
			if(evaluator(pObjects[i])){
				RemoveFrom(i);
				i--;
				to--;
			}
		}
		
	}else{
		DEASSERT_TRUE(to < pObjectCount)
		
		for(i=from; i>=to; i+=step){
			if(evaluator(pObjects[i])){
				RemoveFrom(i);
				i++;
				to++;
			}
		}
	}
}

void decThreadSafeObjectOrderedSet::Sort(decThreadSafeObjectComparator &comparator){
	if(pObjectCount > 1){
		pSort(comparator, 0, pObjectCount - 1);
	}
}

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::GetSorted(
decThreadSafeObjectComparator &comparator) const{
	decThreadSafeObjectOrderedSet copy(*this);
	copy.Sort(comparator);
	return copy;
}



// Operators
//////////////

bool decThreadSafeObjectOrderedSet::operator==(const decThreadSafeObjectOrderedSet &set) const{
	return Equals(set);
}

decThreadSafeObjectOrderedSet decThreadSafeObjectOrderedSet::operator+(const decThreadSafeObjectOrderedSet &set) const{
	decThreadSafeObjectOrderedSet nset(pObjectCount + set.pObjectCount);
	deThreadSafeObject::Ref object;
	int i;
	
	for(i=0; i<pObjectCount; i++){
		object = pObjects[i];
		nset.pObjects[i] = object;
	}
	
	for(i=0; i<set.pObjectCount; i++){
		nset.AddIfAbsent(set.pObjects[i]);
	}
	
	return nset;
}

deThreadSafeObject *decThreadSafeObjectOrderedSet::operator[](int index) const{
	return GetAt(index);
}



decThreadSafeObjectOrderedSet &decThreadSafeObjectOrderedSet::operator=(const decThreadSafeObjectOrderedSet &set){
	deThreadSafeObject::Ref object;
	
	RemoveAll();
	
	if(set.pObjectCount > pObjectSize){
		deThreadSafeObject **newArray = new deThreadSafeObject*[set.pObjectCount];
		if(pObjects){
			delete [] pObjects;
		}
		pObjects = newArray;
		pObjectSize = set.pObjectCount;
	}
	
	for(pObjectCount=0; pObjectCount<set.pObjectCount; pObjectCount++){
		object = set.pObjects[pObjectCount];
		pObjects[pObjectCount] = object;
	}
	
	return *this;
}

decThreadSafeObjectOrderedSet &decThreadSafeObjectOrderedSet::operator+=(const decThreadSafeObjectOrderedSet &set){
	if(set.pObjectCount > 0){
		int i, count = pObjectCount + set.pObjectCount;
		
		if(count > pObjectSize){
			deThreadSafeObject **newArray = new deThreadSafeObject*[count];
			if(pObjects){
				memcpy(newArray, pObjects, sizeof(deThreadSafeObject*) * pObjectSize);
				delete [] pObjects;
			}
			pObjects = newArray;
			pObjectSize = count;
		}
		
		for(i=0; i<set.pObjectCount; i++){
			AddIfAbsent(set.pObjects[i]);
		}
	}
	
	return *this;
}



// Private Functions
//////////////////////

void decThreadSafeObjectOrderedSet::pSort(decThreadSafeObjectComparator &comparator, int left, int right){
	deThreadSafeObject * const pivot = pObjects[left];
	const int r_hold = right;
	const int l_hold = left;
	
	while(left < right){
		while(left < right && comparator(pObjects[right], pivot) >= 0){
			right--;
		}
		if(left != right){
			pObjects[left] = pObjects[right];
			left++;
		}
		while(left < right && comparator(pObjects[left], pivot) <= 0){
			left++;
		}
		if(left != right){
			pObjects[right] = pObjects[left];
			right--;
		}
	}
	
	pObjects[left] = pivot;
	if(l_hold < left){
		pSort(comparator, l_hold, left - 1);
	}
	if(r_hold > left){
		pSort(comparator, left + 1, r_hold);
	}
}
