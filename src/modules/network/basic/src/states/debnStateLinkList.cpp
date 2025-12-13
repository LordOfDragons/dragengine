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
#include <stdlib.h>
#include <string.h>
#include "debnStateLink.h"
#include "debnStateLinkList.h"
#include <dragengine/common/exceptions.h>



// Class debnStateLinkList
////////////////////////////

// Constructor, destructor
////////////////////////////

debnStateLinkList::debnStateLinkList(){
	pLinks = NULL;
	pLinkCount = 0;
	pLinkSize = 0;
}

debnStateLinkList::~debnStateLinkList(){
	if(pLinks) delete [] pLinks;
}



// Management
///////////////

debnStateLink *debnStateLinkList::GetLinkAt(int index) const{
	if(index < 0 || index >= pLinkCount) DETHROW(deeInvalidParam);
	
	return pLinks[index];
}

int debnStateLinkList::IndexOfLink(debnStateLink *link) const{
	if(!link) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pLinkCount; i++){
		if(link == pLinks[i]) return i;
	}
	
	return -1;
}

bool debnStateLinkList::HasLink(debnStateLink *link) const{
	if(!link) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pLinkCount; i++){
		if(link == pLinks[i]) return true;
	}
	
	return false;
}

void debnStateLinkList::AddLink(debnStateLink *link){
	if(!link) DETHROW(deeInvalidParam);
	
	if(pLinkCount == pLinkSize){
		int newSize = pLinkSize * 3 / 2 + 1;
		debnStateLink **newArray = new debnStateLink*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pLinks){
			memcpy(newArray, pLinks, sizeof(debnStateLink*) * pLinkSize);
			delete pLinks;
		}
		pLinks = newArray;
		pLinkSize = newSize;
	}
	
	pLinks[pLinkCount] = link;
	pLinkCount++;
}

void debnStateLinkList::RemoveLink(debnStateLink *link){
	int i, index = IndexOfLink(link);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pLinkCount; i++){
		pLinks[i - 1] = pLinks[i];
	}
	pLinkCount--;
}

void debnStateLinkList::RemoveLinkIfExisting(debnStateLink *link){
	int i, index = IndexOfLink(link);
	
	if(index != -1){
		for(i=index+1; i<pLinkCount; i++){
			pLinks[i - 1] = pLinks[i];
		}
		pLinkCount--;
	}
}

void debnStateLinkList::RemoveAllLinks(){
	pLinkCount = 0;
}
