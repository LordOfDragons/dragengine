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

#include "decObjectSet.h"

#include "../../deObject.h"
#include "../exceptions.h"



// Class decObjectSet
///////////////////////

// Constructor, destructor
////////////////////////////

decObjectSet::decObjectSet(){
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
}

decObjectSet::decObjectSet(int capacity){
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

decObjectSet::decObjectSet(const decObjectSet &set){
	int count = set.GetCount();
	
	pObjects = NULL;
	pObjectCount = 0;
	pObjectSize = 0;
	
	if(count > 0){
		deObject::Ref object;
		
		pObjects = new deObject*[count];
		pObjectSize = count;
		
		for(pObjectCount=0; pObjectCount<count; pObjectCount++){
			object = set.pObjects[pObjectCount];
			pObjects[pObjectCount] = object;
		}
	}
}

decObjectSet::~decObjectSet(){
	RemoveAll();
	
	if(pObjects){
		delete [] pObjects;
	}
}



// Management
///////////////

deObject *decObjectSet::GetAt(int index) const{
	if(index < 0 || index >= pObjectCount){
		DETHROW(deeInvalidParam);
	}
	
	return pObjects[index];
}

bool decObjectSet::Has(deObject::Ref object) const{
	int p;
	
	for(p=0; p<pObjectCount; p++){
		if(object == pObjects[p]){
			return true;
		}
	}
	
	return false;
}

void decObjectSet::Add(deObject::Ref object){
	if(Has(object)){
		DETHROW(deeInvalidParam);
	}
	
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

void decObjectSet::AddIfAbsent(deObject::Ref object){
	if(Has(object)){
		return;
	}
	
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

void decObjectSet::Remove(deObject::Ref object){
	const int position = pIndexOf(object);
	
	if(position == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(pObjects[position]){
		pObjects[position]->FreeReference();
	}
	
	pObjectCount--;
	if(position < pObjectCount){
		pObjects[position] = pObjects[pObjectCount];
	}
}

void decObjectSet::RemoveIfPresent(deObject::Ref object){
	const int position = pIndexOf(object);
	
	if(position == -1){
		return;
	}
	
	if(pObjects[position]){
		pObjects[position]->FreeReference();
	}
	
	pObjectCount--;
	if(position < pObjectCount){
		pObjects[position] = pObjects[pObjectCount];
	}
}

void decObjectSet::RemoveAll(){
	while(pObjectCount > 0){
		pObjectCount--;
		if(pObjects[pObjectCount]){
			pObjects[pObjectCount]->FreeReference();
		}
	}
}



bool decObjectSet::Equals(const decObjectSet &set) const{
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



void decObjectSet::Visit(decObjectVisitor &visitor, int from, int to, int step) const{
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

bool decObjectSet::Find(decObjectEvaluator &evaluator, deObject *&found, int from, int to, int step) const{
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

decObjectSet decObjectSet::Collect(decObjectEvaluator &evaluator, int from, int to, int step) const{
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
	
	decObjectSet collected;
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

void decObjectSet::RemoveIf(decObjectEvaluator &evaluator, int from, int to, int step){
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
				if(pObjects[i]){
					pObjects[i]->FreeReference();
				}
				
				pObjectCount--;
				if(i < pObjectCount){
					pObjects[i] = pObjects[pObjectCount];
				}
				
				i--;
				to--;
			}
		}
		
	}else{
		DEASSERT_TRUE(to < pObjectCount)
		
		for(i=from; i>=to; i+=step){
			if(evaluator(pObjects[i])){
				if(pObjects[i]){
					pObjects[i]->FreeReference();
				}
				
				pObjectCount--;
				if(i < pObjectCount){
					pObjects[i] = pObjects[pObjectCount];
				}
				
				i++;
				to++;
			}
		}
	}
}



// Operators
//////////////

bool decObjectSet::operator==(const decObjectSet &set) const{
	return Equals(set);
}

decObjectSet decObjectSet::operator+(const decObjectSet &set) const{
	decObjectSet nset(pObjectCount + set.pObjectCount);
	deObject::Ref object;
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

deObject *decObjectSet::operator[](int position) const{
	return GetAt(position);
}



decObjectSet &decObjectSet::operator=(const decObjectSet &set){
	if(&set == this){
		return *this;
	}
	
	deObject::Ref object;
	
	RemoveAll();
	
	if(set.pObjectCount > pObjectSize){
		deObject **newArray = new deObject*[set.pObjectCount];
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

decObjectSet &decObjectSet::operator+=(const decObjectSet &set){
	if(set.pObjectCount > 0){
		int i, count = pObjectCount + set.pObjectCount;
		
		if(count > pObjectSize){
			deObject **newArray = new deObject*[count];
			if(pObjects){
				memcpy(newArray, pObjects, sizeof(deObject*) * pObjectSize);
				delete [] pObjects;
			}
			pObjects = newArray;
			pObjectSize = count;
		}
		
		for(i=0; i<count; i++){
			AddIfAbsent(set.pObjects[i]);
		}
	}
	
	return *this;
}



// Private Functions
//////////////////////

int decObjectSet::pIndexOf(deObject::Ref object) const{
	int p;
	
	for(p=0; p<pObjectCount; p++){
		if(object == pObjects[p]){
			return p;
		}
	}
	
	return -1;
}
