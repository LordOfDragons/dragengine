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

#include "deoglRBillboard.h"
#include "deoglBillboardList.h"

#include <dragengine/common/exceptions.h>



// Class deoglBillboardList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglBillboardList::deoglBillboardList(){
	pBillboards = NULL;
	pBillboardCount = 0;
	pBillboardSize = 0;
}

deoglBillboardList::~deoglBillboardList(){
	if(pBillboards){
		delete [] pBillboards;
	}
}



// Management
///////////////

deoglRBillboard *deoglBillboardList::GetAt(int index) const{
	if(index < 0 || index >= pBillboardCount) DETHROW(deeInvalidParam);
	
	return pBillboards[index];
}

int deoglBillboardList::IndexOf(deoglRBillboard *billboard) const{
	if(!billboard) DETHROW(deeInvalidParam);
	
	int l;
	
	for(l=0; l<pBillboardCount; l++){
		if(billboard == pBillboards[l]){
			return l;
		}
	}
	
	return -1;
}

bool deoglBillboardList::Has(deoglRBillboard *billboard) const{
	if(!billboard) DETHROW(deeInvalidParam);
	
	int l;
	
	for(l=0; l<pBillboardCount; l++){
		if(billboard == pBillboards[l]){
			return true;
		}
	}
	
	return false;
}

void deoglBillboardList::Add(deoglRBillboard *billboard){
	if(Has(billboard)) DETHROW(deeInvalidParam);
	
	pAddBillboard(billboard);
}

bool deoglBillboardList::AddIfMissing(deoglRBillboard *billboard){
	if(Has(billboard)) return false;
	
	pAddBillboard(billboard);
	return true;
}

void deoglBillboardList::Remove(deoglRBillboard *billboard){
	int index = IndexOf(billboard);
	
	if(index == -1) DETHROW(deeInvalidParam);
	
	if(index < pBillboardCount - 1){
		pBillboards[index] = pBillboards[pBillboardCount - 1];
	}
	pBillboardCount--;
}

bool deoglBillboardList::RemoveIfExisting(deoglRBillboard *billboard){
	int index = IndexOf(billboard);
	
	if(index == -1) return false;
	
	if(index < pBillboardCount - 1){
		pBillboards[index] = pBillboards[pBillboardCount - 1];
	}
	pBillboardCount--;
	return true;
}

void deoglBillboardList::RemoveFrom(int index){
	if(index < 0 || index >= pBillboardCount) DETHROW(deeInvalidParam);
	
	if(index < pBillboardCount - 1){
		pBillboards[index] = pBillboards[pBillboardCount - 1];
	}
	pBillboardCount--;
}

void deoglBillboardList::RemoveAll(){
	pBillboardCount = 0;
}



// Private Functions
//////////////////////

void deoglBillboardList::pAddBillboard(deoglRBillboard *billboard){
	if(pBillboardCount == pBillboardSize){
		int newSize = pBillboardCount + 10; // * 3 / 2 + 1;
		deoglRBillboard **newArray = new deoglRBillboard*[newSize];
		
		if(pBillboards){
			memcpy(newArray, pBillboards, sizeof(deoglRBillboard*) * pBillboardSize);
			delete [] pBillboards;
		}
		
		pBillboards = newArray;
		pBillboardSize = newSize;
	}
	
	pBillboards[pBillboardCount++] = billboard;
}
