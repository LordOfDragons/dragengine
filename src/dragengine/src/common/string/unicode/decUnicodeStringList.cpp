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

#include "decUnicodeStringList.h"

#include "../../exceptions.h"



// Class decUnicodeStringList
///////////////////////////////

// Constructor, destructor
////////////////////////////

decUnicodeStringList::decUnicodeStringList(){
	pStrings = NULL;
	pStringCount = 0;
	pStringSize = 0;
}

decUnicodeStringList::decUnicodeStringList(const decUnicodeStringList &list){
	const int count = list.GetCount();
	
	pStrings = NULL;
	pStringCount = 0;
	pStringSize = 0;
	
	if(count > 0){
		pStrings = new decUnicodeString*[count];
		pStringSize = count;
		
		while(pStringCount < count){
			pStrings[pStringCount] = new decUnicodeString(list.GetAt(pStringCount));
			pStringCount++;
		}
	}
}

decUnicodeStringList::~decUnicodeStringList(){
	RemoveAll();
	if(pStrings){
		delete [] pStrings;
	}
}



// Management
///////////////

const decUnicodeString &decUnicodeStringList::GetAt(int index) const{
	if(index < 0 || index >= pStringCount){
		DETHROW(deeInvalidParam);
	}
	
	return *pStrings[index];
}

void decUnicodeStringList::SetAt(int index, const decUnicodeString &string){
	if(index < 0 || index >= pStringCount){
		DETHROW(deeInvalidParam);
	}
	
	*pStrings[ index ] = string;
}

int decUnicodeStringList::IndexOf(const decUnicodeString &string) const{
	int s;
	
	for(s=0; s<pStringCount; s++){
		if(pStrings[s]->Equals(string)){
			return s;
		}
	}
	
	return -1;
}

bool decUnicodeStringList::Has(const decUnicodeString &string) const{
	return IndexOf(string) != -1;
}

void decUnicodeStringList::Add(const decUnicodeString &string){
	if(pStringCount == pStringSize){
		const int newSize = pStringSize * 3 / 2 + 1;
		decUnicodeString ** const newArray = new decUnicodeString*[newSize];
		if(pStrings){
			memcpy(newArray, pStrings, sizeof(decUnicodeString*) * pStringSize);
			delete [] pStrings;
		}
		pStrings = newArray;
		pStringSize = newSize;
	}
	
	pStrings[pStringCount] = new decUnicodeString(string);
	pStringCount++;
}

void decUnicodeStringList::InsertAt(const decUnicodeString &string, int index){
	if(index < 0 || index > pStringCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	if(pStringCount == pStringSize){
		const int newSize = pStringSize * 3 / 2 + 1;
		decUnicodeString ** const newArray = new decUnicodeString*[newSize];
		if(pStrings){
			memcpy(newArray, pStrings, sizeof(decUnicodeString*) * pStringSize);
			delete [] pStrings;
		}
		pStrings = newArray;
		pStringSize = newSize;
	}
	
	for(i=pStringCount; i>index; i--){
		pStrings[i] = pStrings[i - 1];
	}
	pStrings[index] = new decUnicodeString(string);
	pStringCount++;
}

void decUnicodeStringList::Move(int from, int to){
	if(from < 0 || from >= pStringCount || to < 0 || to >= pStringCount){
		DETHROW(deeInvalidParam);
	}
	
	decUnicodeString * const tempString = pStrings[from];
	int i;
	
	if(to < from){
		for(i=from; i>to; i--){
			pStrings[i] = pStrings[i - 1];
		}
		
	}else if(to > from){
		for(i=from; i<to; i++){
			pStrings[i] = pStrings[i + 1];
		}
	}
	
	pStrings[to] = tempString;
}

void decUnicodeStringList::RemoveFrom(int index){
	if(index < 0 || index >= pStringCount){
		DETHROW(deeInvalidParam);
	}
	
	int s;
	
	delete pStrings[index];
	
	for(s=index+1; s<pStringCount; s++){
		pStrings[s - 1] = pStrings[s];
	}
	pStringCount--;
}

void decUnicodeStringList::RemoveAll(){
	while(pStringCount > 0){
		pStringCount--;
		delete pStrings[pStringCount];
	}
}

void decUnicodeStringList::SortAscending(){
	if(pStringCount > 1){
		pSortAscending(0, pStringCount - 1);
	}
}

void decUnicodeStringList::SortDescending(){
	if(pStringCount > 1){
		pSortDescending(0, pStringCount - 1);
	}
}



// Operators
//////////////

const decUnicodeString &decUnicodeStringList::operator[](int index) const{
	return GetAt(index);
}

decUnicodeStringList &decUnicodeStringList::operator=(const decUnicodeStringList &list){
	const int count = list.GetCount();
	int s;
	
	RemoveAll();
	for(s=0; s<count; s++){
		Add(list.GetAt(s));
	}
	
	return *this;
}

bool decUnicodeStringList::operator!=(const decUnicodeStringList &list){
	return !(*this == list);
}

bool decUnicodeStringList::operator==(const decUnicodeStringList &list){
	int i;
	
	if(list.GetCount() != pStringCount){
		return false;
	}
	
	for(i=0; i<pStringCount; i++){
		if(*pStrings[i] != list.GetAt(i)){
			return false;
		}
	}
	
	return true;
}

decUnicodeStringList decUnicodeStringList::operator+(const decUnicodeStringList &list) const{
	const int count = list.GetCount();
	decUnicodeStringList newList(*this);
	int s;
	
	for(s=0; s<count; s++){
		newList.Add(list.GetAt(s));
	}
	
	return newList;
}

decUnicodeStringList &decUnicodeStringList::operator+=(const decUnicodeStringList &list){
	const int count = list.GetCount();
	int s;
	
	for(s=0; s<count; s++){
		Add(list.GetAt(s));
	}
	
	return *this;
}

decUnicodeStringList decUnicodeStringList::operator+(const decUnicodeString &string) const{
	decUnicodeStringList newList(*this);
	
	newList.Add(string);
	
	return newList;
}

decUnicodeStringList &decUnicodeStringList::operator+=(const decUnicodeString &string){
	Add(string);
	return *this;
}



// Private Functions
//////////////////////

void decUnicodeStringList::pSortAscending(int left, int right){
	decUnicodeString * const pivot = pStrings[left];
	const int r_hold = right;
	const int l_hold = left;
	
	while(left < right){
		while((*pStrings[right] >= *pivot) && (left < right)){
			right--;
		}
		if(left != right){
			pStrings[left] = pStrings[right];
			left++;
		}
		while((*pStrings[left] <= *pivot) && (left < right)){
			left++;
		}
		if(left != right){
			pStrings[right] = pStrings[left];
			right--;
		}
	}
	
	pStrings[left] = pivot;
	if(l_hold < left){
		pSortAscending(l_hold, left - 1);
	}
	if(r_hold > left){
		pSortAscending(left + 1, r_hold);
	}
}

void decUnicodeStringList::pSortDescending(int left, int right){
	decUnicodeString * const pivot = pStrings[left];
	const int r_hold = right;
	const int l_hold = left;
	
	while(left < right){
		while((*pStrings[right] <= *pivot) && (left < right)){
			right--;
		}
		if(left != right){
			pStrings[left] = pStrings[right];
			left++;
		}
		while((*pStrings[left] >= *pivot) && (left < right)){
			left++;
		}
		if(left != right){
			pStrings[right] = pStrings[left];
			right--;
		}
	}
	
	pStrings[left] = pivot;
	if(l_hold < left){
		pSortDescending(l_hold, left - 1);
	}
	if(r_hold > left){
		pSortDescending(left + 1, r_hold);
	}
}
