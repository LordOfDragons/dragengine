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

#include "deAnimatorControllerTarget.h"
#include "../../../common/exceptions.h"



// Class deAnimatorControllerTarget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorControllerTarget::deAnimatorControllerTarget(){
	pLinks = NULL;
	pLinkCount = 0;
}

deAnimatorControllerTarget::~deAnimatorControllerTarget(){
	if(pLinks) delete [] pLinks;
}



// Management
///////////////

int deAnimatorControllerTarget::GetLinkAt(int index) const{
	if(index < 0 || index >= pLinkCount) DETHROW(deeInvalidParam);
	
	return pLinks[index];
}

int deAnimatorControllerTarget::IndexOfLink(int link) const{
	int l;
	
	for(l=0; l<pLinkCount; l++){
		if(pLinks[l] == link) return l;
	}
	
	return -1;
}

void deAnimatorControllerTarget::AddLink(int link){
	int l, *newArray = new int[pLinkCount + 1];
	
	if(pLinks){
		for(l=0; l<pLinkCount; l++) newArray[l] = pLinks[l];
		delete [] pLinks;
	}
	
	pLinks = newArray;
	pLinks[pLinkCount] = link;
	pLinkCount++;
}

void deAnimatorControllerTarget::RemoveLink(int link){
	int l, index = IndexOfLink(link);
	
	if(index == -1) DETHROW(deeInvalidParam);
	
	int *newArray = NULL;
	if(pLinkCount > 0){
		newArray = new int[pLinkCount - 1];
	}
	
	if(pLinks){
		for(l=0; l<index; l++) newArray[l] = pLinks[l];
		for(l=index+1; l<pLinkCount; l++) newArray[l - 1] = pLinks[l];
		delete [] pLinks;
	}
	
	pLinks = newArray;
	pLinkCount--;
}

void deAnimatorControllerTarget::RemoveAllLinks(){
	if(pLinks){
		delete [] pLinks;
		pLinks = NULL;
	}
	
	pLinkCount = 0;
}
