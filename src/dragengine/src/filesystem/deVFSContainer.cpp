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
#include "deVFSContainer.h"
#include "../common/exceptions.h"



// Class deVFSContainer
/////////////////////////

// Constructor, destructor
////////////////////////////

deVFSContainer::deVFSContainer() :
pRootPath(decPath::CreatePathUnix("/")),
pHidden(false){
}

deVFSContainer::deVFSContainer(const decPath &rootPath) :
pRootPath(rootPath),
pHidden(false){
}

deVFSContainer::~deVFSContainer(){
}



// Management
///////////////

void deVFSContainer::SetHidden(bool hidden){
	pHidden = hidden;
}

int deVFSContainer::GetHiddenPathCount() const{
	return pHiddenPath.GetCount();
}

const decPath &deVFSContainer::GetHiddenPathAt(int index) const{
	return pHiddenPath.GetAt(index);
}

bool deVFSContainer::HasHiddenPath(const decPath &path) const{
	return pHiddenPath.Has(path);
}

void deVFSContainer::AddHiddenPath(const decPath &path){
	pHiddenPath.AddIfAbsent(path);
}

void deVFSContainer::RemoveHiddenPath(const decPath &path){
	pHiddenPath.RemoveIfPresent(path);
}

void deVFSContainer::RemoveAllHiddenPath(){
	pHiddenPath.RemoveAll();
}

bool deVFSContainer::IsPathHiddenBelow(const decPath &path){
	const int count = pHiddenPath.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(pHiddenPath.GetAt(i).IsEqualOrParentOf(path)){
			return true;
		}
	}
	return false;
}
