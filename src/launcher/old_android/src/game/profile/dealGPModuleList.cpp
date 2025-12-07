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

#include "dealGPModule.h"
#include "dealGPModuleList.h"

#include "../../common/exceptions.h"



// Class dealGPModuleList
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGPModuleList::dealGPModuleList(){
}

dealGPModuleList::~dealGPModuleList(){
	RemoveAllModules();
}



// Management
///////////////

int dealGPModuleList::GetModuleCount() const{
	return pModules.GetCount();
}

dealGPModule *dealGPModuleList::GetModuleAt(int index) const{
	return (dealGPModule*)pModules.GetAt(index);
}

dealGPModule *dealGPModuleList::GetModuleNamed(const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	
	int i, count = pModules.GetCount();
	dealGPModule *module;
	
	for(i=0; i<count; i++){
		module = (dealGPModule*)pModules.GetAt(i);
		if(module->GetName().Equals(name)){
			return module;
		}
	}
	
	return NULL;
}

bool dealGPModuleList::HasModule(dealGPModule *module) const{
	return pModules.Has(module);
}

bool dealGPModuleList::HasModuleNamed(const char *name) const{
	return GetModuleNamed(name) != NULL;
}

int dealGPModuleList::IndexOfModule(dealGPModule *module) const{
	return pModules.IndexOf(module);
}

int dealGPModuleList::IndexOfModuleNamed(const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	
	int i, count = pModules.GetCount();
	
	for(i=0; i<count; i++){
		if(((dealGPModule*)pModules.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

void dealGPModuleList::AddModule(dealGPModule *module){
	if(! module || HasModuleNamed(module->GetName().GetString())){
		DETHROW(deeInvalidParam);
	}
	pModules.Add(module);
}

void dealGPModuleList::RemoveModule(dealGPModule *module){
	const int index = IndexOfModule(module);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pModules.RemoveFrom(index);
}

void dealGPModuleList::RemoveAllModules(){
	pModules.RemoveAll();
}
