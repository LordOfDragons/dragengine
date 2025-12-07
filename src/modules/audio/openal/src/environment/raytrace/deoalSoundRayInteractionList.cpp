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

#include "deoalSoundRayInteraction.h"
#include "deoalSoundRayInteractionList.h"

#include <dragengine/common/exceptions.h>



// Class deoalSoundRayInteractionList
///////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalSoundRayInteractionList::deoalSoundRayInteractionList() :
pRays(NULL),
pCount(0),
pSize(0){
}

deoalSoundRayInteractionList::~deoalSoundRayInteractionList(){
	if(pRays){
		delete [] pRays;
	}
}



// Manegement
///////////////

const deoalSoundRayInteraction &deoalSoundRayInteractionList::GetAt(int index) const{
	if(index < 0 || index >= pCount){
		DETHROW(deeInvalidParam);
	}
	
	return pRays[index];
}

deoalSoundRayInteraction &deoalSoundRayInteractionList::Add(){
	if(pCount == pSize){
		const int newSize = pSize + 10;
		deoalSoundRayInteraction * const newArray = new deoalSoundRayInteraction[newSize];
		if(pRays){
			memcpy(newArray, pRays, sizeof(deoalSoundRayInteraction) * pSize);
			delete [] pRays;
		}
		pRays = newArray;
		pSize = newSize;
	}
	
	return pRays[pCount++];
}

void deoalSoundRayInteractionList::RemoveAll(){
	pCount = 0;
}

void deoalSoundRayInteractionList::ReserveSize(int size){
	if(size < 0){
		DETHROW(deeInvalidParam);
	}
	if(size <= pSize){
		return;
	}
	
	deoalSoundRayInteraction * const newArray = new deoalSoundRayInteraction[size];
	if(pRays){
		memcpy(newArray, pRays, sizeof(deoalSoundRayInteraction) * pSize);
		delete [] pRays;
	}
	pRays = newArray;
	pSize = size;
}



// Operators
//////////////

deoalSoundRayInteractionList &deoalSoundRayInteractionList::operator=(
const deoalSoundRayInteractionList &list){
	RemoveAll();
	return operator+=(list);
}

deoalSoundRayInteractionList &deoalSoundRayInteractionList::operator+=(
const deoalSoundRayInteractionList &list){
	ReserveSize(pCount + list.pCount);
	
	int i;
	for(i=0; i<list.pCount; i++){
		Add() = list.pRays[i];
	}
	return *this;
}
