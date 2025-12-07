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

#include "delGPDisableModuleVersion.h"
#include "delGPDisableModuleVersionList.h"

#include <dragengine/common/exceptions.h>



// Class delGPDisableModuleVersionList
////////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPDisableModuleVersionList::delGPDisableModuleVersionList(){
}

delGPDisableModuleVersionList::delGPDisableModuleVersionList(const delGPDisableModuleVersionList &copy){
	*this = copy;
}

delGPDisableModuleVersionList::~delGPDisableModuleVersionList(){
	RemoveAll();
}



// Management
///////////////

int delGPDisableModuleVersionList::GetCount() const{
	return pList.GetCount();
}

delGPDisableModuleVersion *delGPDisableModuleVersionList::GetAt(int index) const{
	return (delGPDisableModuleVersion*)pList.GetAt(index);
}

delGPDisableModuleVersion *delGPDisableModuleVersionList::GetWith(
const char *name, const char *version) const{
	if(!name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	if(!version){
		DETHROW_INFO(deeNullPointer, "version");
	}
	
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delGPDisableModuleVersion * const module = (delGPDisableModuleVersion*)pList.GetAt(i);
		if(module->GetName() == name && module->GetVersion() == version){
			return module;
		}
	}
	
	return nullptr;
}

bool delGPDisableModuleVersionList::Has(delGPDisableModuleVersion *entry) const{
	return pList.Has(entry);
}

bool delGPDisableModuleVersionList::HasWith(const char *name, const char *version) const{
	return GetWith(name, version);
}

int delGPDisableModuleVersionList::IndexOf(delGPDisableModuleVersion *entry) const{
	return pList.IndexOf(entry);
}

int delGPDisableModuleVersionList::IndexOfWith(const char *name, const char *version) const{
	if(!name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	if(!version){
		DETHROW_INFO(deeNullPointer, "version");
	}
	
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delGPDisableModuleVersion &module = *((delGPDisableModuleVersion*)pList.GetAt(i));
		if(module.GetName() == name && module.GetVersion() == version){
			return i;
		}
	}
	
	return -1;
}

void delGPDisableModuleVersionList::Add(delGPDisableModuleVersion *entry){
	if(!entry){
		DETHROW_INFO(deeNullPointer, "entry");
	}
	if(HasWith(entry->GetName(), entry->GetVersion())){
		DETHROW_INFO(deeInvalidParam, "matching entry is present");
	}
	
	pList.Add(entry);
}

void delGPDisableModuleVersionList::Remove(delGPDisableModuleVersion *entry){
	const int index = IndexOf(entry);
	if(index == -1){
		DETHROW_INFO(deeInvalidParam, "entry is absent");
	}
	
	pList.RemoveFrom(index);
}

void delGPDisableModuleVersionList::RemoveAll(){
	pList.RemoveAll();
}



// Operators
//////////////

delGPDisableModuleVersionList &delGPDisableModuleVersionList::operator=(
const delGPDisableModuleVersionList &other){
	const decObjectList &otherList = other.pList;
	const int count = otherList.GetCount();
	int i;
	
	pList.RemoveAll();
	
	for(i=0; i<count; i++){
		pList.Add(delGPDisableModuleVersion::Ref::New(new delGPDisableModuleVersion(
			*( ( delGPDisableModuleVersion* )otherList.GetAt( i ) ) ) ) );
	}
	
	return *this;
}
