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
#include "dermNameList.h"
#include <dragengine/common/exceptions.h>



// Class dermNameList
///////////////////////

// Constructor, destructor
////////////////////////////

dermNameList::dermNameList(){
	pNames = nullptr;
	pNameCount = 0;
	pNameSize = 0;
}

dermNameList::~dermNameList(){
	if(pNames){
		delete [] pNames;
	}
}



// Management
///////////////

const decString &dermNameList::GetNameAt(int index) const{
	if(index < 0 || index >= pNameCount) DETHROW(deeInvalidParam);
	
	return pNames[index].name;
}

int dermNameList::GetNameNumberAt(int index) const{
	if(index < 0 || index >= pNameCount) DETHROW(deeInvalidParam);
	
	return pNames[index].number;
}

void dermNameList::SetNameNumberAt(int index, int number){
	if(index < 0 || index >= pNameCount) DETHROW(deeInvalidParam);
	
	pNames[index].number = number;
}

int dermNameList::IndexOfName(const char *name) const{
	DEASSERT_NOTNULL(name)
	int n;
	
	for(n=0; n<pNameCount; n++){
		if(pNames[n].name == name){
			return n;
		}
	}
	
	return -1;
}

bool dermNameList::HasName(const char *name) const{
	DEASSERT_NOTNULL(name)
	int n;
	
	for(n=0; n<pNameCount; n++){
		if(pNames[n].name == name){
			return true;
		}
	}
	
	return false;
}

int dermNameList::AddName(const char *name){
	int index = IndexOfName(name);
	if(index != -1){
		return index;
	}
	
	if(pNameCount == pNameSize){
		int i, newSize = pNameSize * 3 / 2 + 1;
		sName * const newArray = new sName[newSize];
		if(pNames){
			for(i=0; i<pNameSize; i++){
				newArray[i] = pNames[i];
			}
			delete [] pNames;
		}
		pNames = newArray;
		pNameSize = newSize;
	}
	
	pNames[pNameCount].name = name;
	pNames[pNameCount].number = -1;
	pNameCount++;
	
	return pNameCount - 1;
}
