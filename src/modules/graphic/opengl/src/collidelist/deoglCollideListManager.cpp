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

// includes
#include <stdio.h>
#include <string.h>

#include "deoglCollideListManager.h"
#include "deoglCollideList.h"

#include <dragengine/common/exceptions.h>



// class deoglCollideListManager
///////////////////////////////////

// constructor, destructor
////////////////////////////

deoglCollideListManager::deoglCollideListManager(){
	pLists = NULL;
	pListCount = 0;
	pListSize = 0;
}

deoglCollideListManager::~deoglCollideListManager(){
	int i, inUseCount = 0;
	for(i=0; i<pListCount; i++){
		if(pLists[i].inUse) inUseCount++;
	}
	if(inUseCount > 0){
		printf("[OPENGL] WARNING!%i collide lists have not been released!\n", inUseCount);
	}
	if(pLists){
		for(i=0; i<pListCount; i++){
			if(pLists[i].list) delete pLists[i].list;
		}
		delete [] pLists;
	}
}



// list management
////////////////////

deoglCollideList *deoglCollideListManager::GetList(){
	int index = pFindNextList();
	if(index == -1){
		if(pListCount == pListSize){
			int i, newSize = pListSize * 3 / 2 + 1;
			sList *newArray = new sList[newSize];
			for(i=pListSize; i<newSize; i++){
				newArray[i].list = NULL;
				newArray[i].inUse = false;
			}
			if(pLists){
				for(i=0; i<pListSize; i++){
					newArray[i].list = pLists[i].list;
					newArray[i].inUse = pLists[i].inUse;
				}
				delete [] pLists;
			}
			pLists = newArray;
			pListSize = newSize;
		}
		pLists[pListCount].list = new deoglCollideList;
		index = pListCount;
		pListCount++;
	}
	pLists[index].inUse = true;
	return pLists[index].list;
}

void deoglCollideListManager::ReleaseList(deoglCollideList *list){
	if(!list) DETHROW(deeInvalidParam);
	int i;
	for(i=0; i<pListCount; i++){
		if(pLists[i].list == list){
			if(!pLists[i].inUse) DETHROW(deeInvalidParam);
			pLists[i].inUse = false;
			break;
		}
	}
}



// private functions
//////////////////////

int deoglCollideListManager::pFindNextList() const{
	int i;
	for(i=0; i<pListCount; i++){
		if(!pLists[i].inUse) return i;
	}
	return -1;
}
