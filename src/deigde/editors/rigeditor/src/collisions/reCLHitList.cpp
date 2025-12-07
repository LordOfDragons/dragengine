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
#include "reCLHitList.h"
#include "reCLHitListEntry.h"
#include "dragengine/common/exceptions.h"



// Class reCLHitList
//////////////////////

// Constructor, destructor
////////////////////////////

reCLHitList::reCLHitList(){
	pEntries = NULL;
	pEntryCount = 0;
	pEntrySize = 0;
}

reCLHitList::~reCLHitList(){
	RemoveAllEntries();
	if(pEntries) delete [] pEntries;
}



// Management
///////////////

reCLHitListEntry *reCLHitList::GetEntryAt(int index) const{
	if(index < 0 || index >= pEntryCount) DETHROW(deeInvalidParam);
	
	return pEntries[index];
}

int reCLHitList::IndexOfEntry(reCLHitListEntry *entry) const{
	if(! entry) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pEntryCount; i++){
		if(entry == pEntries[i]) return i;
	}
	
	return -1;
}

bool reCLHitList::HasEntry(reCLHitListEntry *entry) const{
	if(! entry) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pEntryCount; i++){
		if(entry == pEntries[i]) return true;
	}
	
	return false;
}

void reCLHitList::AddEntry(reCLHitListEntry *entry){
	if(HasEntry(entry)) DETHROW(deeInvalidParam);
	
	if(pEntryCount == pEntrySize){
		int newSize = pEntryCount * 3 / 2 + 1;
		reCLHitListEntry **newArray = new reCLHitListEntry*[newSize];
		if(! newArray) DETHROW(deeOutOfMemory);
		if(pEntries){
			memcpy(newArray, pEntries, sizeof(reCLHitListEntry*) * pEntrySize);
			delete [] pEntries;
		}
		pEntries = newArray;
		pEntrySize = newSize;
	}
	
	pEntries[pEntryCount] = entry;
	pEntryCount++;
}

void reCLHitList::RemoveEntry(reCLHitListEntry *entry){
	int i, index = IndexOfEntry(entry);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pEntryCount; i++){
		pEntries[i - 1] = pEntries[i];
	}
	pEntryCount--;
	
	delete entry;
}

void reCLHitList::RemoveAllEntries(){
	while(pEntryCount > 0){
		pEntryCount--;
		delete pEntries[pEntryCount];
	}
}



void reCLHitList::SortByDistance(){
	reCLHitListEntry *exchange;
	int index = 1;
	
	while(index < pEntryCount){
		if(pEntries[index - 1]->CompareTo(*pEntries[index]) > 0){
			exchange = pEntries[index - 1];
			pEntries[index - 1] = pEntries[index];
			pEntries[index] = exchange;
			
			if(index > 1){
				index--;
				
			}else{
				index++;
			}
		
		}else{
			index++;
		}
	}
}
