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

#include "dealGPDisableModuleVersion.h"
#include "dealGPDisableModuleVersionList.h"

#include "../../common/exceptions.h"



// Class dealGPDisableModuleVersionList
//////////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGPDisableModuleVersionList::dealGPDisableModuleVersionList(){
}

dealGPDisableModuleVersionList::dealGPDisableModuleVersionList(const dealGPDisableModuleVersionList &copy){
	*this = copy;
}

dealGPDisableModuleVersionList::~dealGPDisableModuleVersionList(){
	RemoveAll();
}



// Management
///////////////

int dealGPDisableModuleVersionList::GetCount() const{
	return pList.GetCount();
}

dealGPDisableModuleVersion *dealGPDisableModuleVersionList::GetAt(int index) const{
	return (dealGPDisableModuleVersion*)pList.GetAt(index);
}

dealGPDisableModuleVersion *dealGPDisableModuleVersionList::GetWith(const char *name, const char *version) const{
	if(! name || ! version){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dealGPDisableModuleVersion * const module = (dealGPDisableModuleVersion*)pList.GetAt(i);
		if(module->GetName() == name && module->GetVersion() == version){
			return module;
		}
	}
	
	return NULL;
}

bool dealGPDisableModuleVersionList::Has(dealGPDisableModuleVersion *entry) const{
	return pList.Has(entry);
}

bool dealGPDisableModuleVersionList::HasWith(const char *name, const char *version) const{
	return GetWith(name, version) != NULL;
}

int dealGPDisableModuleVersionList::IndexOf(dealGPDisableModuleVersion *entry) const{
	return pList.IndexOf(entry);
}

int dealGPDisableModuleVersionList::IndexOfWith(const char *name, const char *version) const{
	if(! name || ! version){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const dealGPDisableModuleVersion &module = *((dealGPDisableModuleVersion*)pList.GetAt(i));
		if(module.GetName() == name && module.GetVersion() == version){
			return i;
		}
	}
	
	return -1;
}

void dealGPDisableModuleVersionList::Add(dealGPDisableModuleVersion *entry){
	if(! entry || HasWith(entry->GetName(), entry->GetVersion())){
		DETHROW(deeInvalidParam);
	}
	pList.Add(entry);
}

void dealGPDisableModuleVersionList::Remove(dealGPDisableModuleVersion *entry){
	const int index = IndexOf(entry);
	
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pList.RemoveFrom(index);
}

void dealGPDisableModuleVersionList::RemoveAll(){
	pList.RemoveAll();
}



// Operators
//////////////

dealGPDisableModuleVersionList &dealGPDisableModuleVersionList::operator=(const dealGPDisableModuleVersionList &other){
	const decObjectList &otherList = other.pList;
	dealGPDisableModuleVersion *entry = NULL;
	const int count = otherList.GetCount();
	int i;
	
	pList.RemoveAll();
	
	try{
		for(i=0; i<count; i++){
			entry = new dealGPDisableModuleVersion(*((dealGPDisableModuleVersion*)otherList.GetAt(i)));
			pList.Add(entry);
			entry->FreeReference();
			entry = NULL;
		}
		
	}catch(const deException &){
		if(entry){
			entry->FreeReference();
		}
		throw;
	}
	
	return *this;
}
