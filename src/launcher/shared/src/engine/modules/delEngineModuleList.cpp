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

#include "delEngineModule.h"
#include "delEngineModuleList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>



// Class delEngineModuleList
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineModuleList::delEngineModuleList(){
}

delEngineModuleList::~delEngineModuleList(){
	RemoveAll();
}



// Management
///////////////

int delEngineModuleList::GetCount() const{
	return pModules.GetCount();
}

delEngineModule *delEngineModuleList::GetAt(int index) const{
	return (delEngineModule*)pModules.GetAt(index);
}

delEngineModule *delEngineModuleList::GetNamed(const char *name) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	
	const int count = pModules.GetCount();
	delEngineModule *latestModule = NULL;
	int i;
	
	for(i=0; i<count; i++){
		delEngineModule * const module = (delEngineModule*)pModules.GetAt(i);
		if(module->GetName() != name){
			continue;
		}
		if(latestModule && deModuleSystem::CompareVersion(
				module->GetVersion(), latestModule->GetVersion()) <= 0){
			continue;
		}
		
		latestModule = module;
	}
	
	return latestModule;
}

delEngineModule *delEngineModuleList::GetNamed(const char *name, const char *version) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	if(! version){
		DETHROW_INFO(deeNullPointer, "version");
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delEngineModule * const module = (delEngineModule*)pModules.GetAt(i);
		if(module->GetName() == name && module->GetVersion() == version){
			return module;
		}
	}
	
	return nullptr;
}

delEngineModule *delEngineModuleList::GetNamedAtLeast(const char *name, const char *version) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	if(! version){
		DETHROW_INFO(deeNullPointer, "version");
	}
	
	const int count = pModules.GetCount();
	delEngineModule *latestModule = NULL;
	int i;
	
	for(i=0; i<count; i++){
		delEngineModule * const module = (delEngineModule*)pModules.GetAt(i);
		if(module->GetName() != name || deModuleSystem::CompareVersion(
				module->GetVersion(), version) < 0){
			continue;
		}
		if(latestModule && deModuleSystem::CompareVersion(
				module->GetVersion(), latestModule->GetVersion()) <= 0){
			continue;
		}
		
		latestModule = module;
	}
	
	return latestModule;
}

void delEngineModuleList::GetNamed(decObjectList &list, const char *name) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for(i=0; i<count; i++){
		delEngineModule * const module = (delEngineModule*)pModules.GetAt(i);
		if(module->GetName() == name){
			list.Add(module);
		}
	}
}

void delEngineModuleList::GetNames(decStringList &list) const{
	const int count = pModules.GetCount();
	int i;
	
	list.RemoveAll();
	
	for(i=0; i<count; i++){
		delEngineModule * const module = (delEngineModule*)pModules.GetAt(i);
		if(! list.Has(module->GetName())){
			list.Add(module->GetName());
		}
	}
}

bool delEngineModuleList::Has(delEngineModule *module) const{
	return pModules.Has(module);
}

bool delEngineModuleList::HasNamed(const char *name) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delEngineModule &module = *((delEngineModule*)pModules.GetAt(i));
		if(module.GetName() == name){
			return true;
		}
	}
	
	return false;
}

bool delEngineModuleList::HasNamed(const char *name, const char *version) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	if(! version){
		DETHROW_INFO(deeNullPointer, "version");
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delEngineModule &module = *((delEngineModule*)pModules.GetAt(i));
		if(module.GetName() == name && module.GetVersion() == version){
			return true;
		}
	}
	
	return false;
}

int delEngineModuleList::IndexOf(delEngineModule *module) const{
	return pModules.IndexOf(module);
}

int delEngineModuleList::IndexOfNamed(const char *name) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	
	const int count = pModules.GetCount();
	const delEngineModule *latestModule = NULL;
	int latestIndex = -1;
	int i;
	
	for(i=0; i<count; i++){
		const delEngineModule * const module = (delEngineModule*)pModules.GetAt(i);
		if(module->GetName() == name){
			if(! latestModule || deModuleSystem::CompareVersion(
					module->GetVersion(), latestModule->GetVersion()) > 0){
				latestModule = module;
				latestIndex = i;
			}
		}
	}
	
	return latestIndex;
}

int delEngineModuleList::IndexOfNamed(const char *name, const char *version) const{
	if(! name){
		DETHROW_INFO(deeNullPointer, "name");
	}
	if(! version){
		DETHROW_INFO(deeNullPointer, "version");
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delEngineModule &module = *((delEngineModule*)pModules.GetAt(i));
		if(module.GetName() == name && module.GetVersion() == version){
			return i;
		}
	}
	
	return -1;
}

void delEngineModuleList::Add(delEngineModule *module){
	if(! module){
		DETHROW_INFO(deeNullPointer, "module");
	}
	if(HasNamed(module->GetName(), module->GetVersion())){
		DETHROW_INFO(deeInvalidParam, "named module with version is present");
	}
	
	pModules.Add(module);
}

void delEngineModuleList::Remove(delEngineModule *module){
	const int index = IndexOf (module);
	if(index == -1){
		DETHROW_INFO(deeInvalidParam, "moduel is absent");
	}
	pModules.RemoveFrom(index);
}

void delEngineModuleList::RemoveAll(){
	pModules.RemoveAll();
}
