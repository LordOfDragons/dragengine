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

#include <stdlib.h>
#include <string.h>

#include "dePathList.h"
#include "../common/file/decPath.h"
#include "../common/exceptions.h"



// Class dePathList
/////////////////////

// Constructor, destructor
////////////////////////////

dePathList::dePathList() :
pPath(NULL),
pPathCount(0),
pPathSize(0){
}

dePathList::dePathList (const dePathList &list):
pPath(NULL),
pPathCount(0),
pPathSize(0)
{
	*this = list;
}

dePathList::~dePathList(){
	RemoveAll();
	if(pPath){
		delete [] pPath;
	}
}




// Management
///////////////

const decPath &dePathList::GetAt(int index) const{
	if(index < 0 || index >= pPathCount){
		DETHROW(deeOutOfBoundary);
	}
	
	return *pPath[index];
}

int dePathList::IndexOf(const decPath &path) const{
	int p;
	
	for(p=0; p<pPathCount; p++){
		if(path == *pPath[p]){
			return p;
		}
	}
	
	return -1;
}

bool dePathList::Has(const decPath &path) const{
	int p;
	
	for(p=0; p<pPathCount; p++){
		if(path == *pPath[p]){
			return true;
		}
	}
	
	return false;
}

void dePathList::Add(const decPath &path){
	if(Has(path)){
		DETHROW(deeInvalidParam);
	}
	
	if(pPathCount == pPathSize){
		const int newSize = pPathSize * 3 / 2 + 1;
		decPath ** const newArray = new decPath*[newSize];
		if(pPath){
			memcpy(newArray, pPath, sizeof(decPath*) * pPathSize);
			delete [] pPath;
		}
		pPath = newArray;
		pPathSize = newSize;
	}
	
	pPath[pPathCount] = new decPath(path);
	
	pPathCount++;
}

void dePathList::AddIfAbsent(const decPath &path){
	if(Has(path)){
		return;
	}
	
	if(pPathCount == pPathSize){
		const int newSize = pPathSize * 3 / 2 + 1;
		decPath ** const newArray = new decPath*[newSize];
		if(pPath){
			memcpy(newArray, pPath, sizeof(decPath*) * pPathSize);
			delete [] pPath;
		}
		pPath = newArray;
		pPathSize = newSize;
	}
	
	pPath[pPathCount] = new decPath(path);
	
	pPathCount++;
}

void dePathList::Remove(const decPath &path){
	int i, index = IndexOf(path);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	delete pPath[index];
	
	for(i=index+1; i<pPathCount; i++){
		pPath[i - 1] = pPath[i];
	}
	pPathCount--;
}

void dePathList::RemoveIfPresent(const decPath &path){
	int i, index = IndexOf(path);
	if(index == -1){
		return;
	}
	
	delete pPath[index];
	
	for(i=index+1; i<pPathCount; i++){
		pPath[i - 1] = pPath[i];
	}
	pPathCount--;
}

void dePathList::RemoveAll(){
	while(pPathCount > 0){
		pPathCount--;
		delete pPath[pPathCount];
	}
}



// Operators
//////////////

dePathList &dePathList::operator=(const dePathList &list){
	pPathCount = 0;
	
	int i;
	for(i=0; i<list.pPathCount; i++){
		Add(*list.pPath[i]);
	}
	
	return *this;
}
