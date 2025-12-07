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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglModelFace.h"
#include "deoglModelFaceList.h"

#include <dragengine/common/exceptions.h>



// Class deoglModelFaceList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglModelFaceList::deoglModelFaceList(){
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
}

deoglModelFaceList::~deoglModelFaceList(){
	if(pFaces){
		delete [] pFaces;
	}
}



// Management
///////////////

deoglModelFace *deoglModelFaceList::GetAt(int index) const{
	if(index < 0 || index >= pFaceCount) DETHROW(deeInvalidParam);
	
	return pFaces[index];
}

int deoglModelFaceList::IndexOfFace(deoglModelFace *face) const{
	if(!face) DETHROW(deeInvalidParam);
	
	int l;
	
	for(l=0; l<pFaceCount; l++){
		if(face == pFaces[l]){
			return l;
		}
	}
	
	return -1;
}

bool deoglModelFaceList::Has(deoglModelFace *face) const{
	if(!face) DETHROW(deeInvalidParam);
	
	int l;
	
	for(l=0; l<pFaceCount; l++){
		if(face == pFaces[l]){
			return true;
		}
	}
	
	return false;
}

void deoglModelFaceList::Add(deoglModelFace *face){
	if(Has(face)) DETHROW(deeInvalidParam);
	
	pAddFace(face);
}

bool deoglModelFaceList::AddIfMissing(deoglModelFace *face){
	if(Has(face)) return false;
	
	pAddFace(face);
	return true;
}

void deoglModelFaceList::Remove(deoglModelFace *face){
	int index = IndexOfFace(face);
	
	if(index == -1) DETHROW(deeInvalidParam);
	
	if(index < pFaceCount - 1){
		pFaces[index] = pFaces[pFaceCount - 1];
	}
	pFaceCount--;
}

bool deoglModelFaceList::RemoveIfExisting(deoglModelFace *face){
	int index = IndexOfFace(face);
	
	if(index == -1) return false;
	
	if(index < pFaceCount - 1){
		pFaces[index] = pFaces[pFaceCount - 1];
	}
	pFaceCount--;
	return true;
}

void deoglModelFaceList::RemoveFrom(int index){
	if(index < 0 || index >= pFaceCount) DETHROW(deeInvalidParam);
	
	if(index < pFaceCount - 1){
		pFaces[index] = pFaces[pFaceCount - 1];
	}
	pFaceCount--;
}

void deoglModelFaceList::RemoveAll(){
	pFaceCount = 0;
}

void deoglModelFaceList::SortByIndex(){
	deoglModelFace *temp;
	int i;
	
	for(i=1; i<pFaceCount; i++){
		if(pFaces[i]->GetIndex() < pFaces[i - 1]->GetIndex()){
			temp = pFaces[i - 1];
			pFaces[i - 1] = pFaces[i];
			pFaces[i] = temp;
			
			if(i > 1){
				i -= 2;
			}
		}
	}
}



// Private Functions
//////////////////////

void deoglModelFaceList::pAddFace(deoglModelFace *face){
	if(pFaceCount == pFaceSize){
		int newSize = pFaceCount + 10; // * 3 / 2 + 1;
		deoglModelFace **newArray = new deoglModelFace*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		
		if(pFaces){
			memcpy(newArray, pFaces, sizeof(deoglModelFace*) * pFaceSize);
			delete [] pFaces;
		}
		
		pFaces = newArray;
		pFaceSize = newSize;
	}
	
	pFaces[pFaceCount++] = face;
}
