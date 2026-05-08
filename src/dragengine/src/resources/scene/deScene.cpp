/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deScene.h"
#include "deSceneManager.h"
#include "../../common/collection/decTDictionary.h"
#include "../../common/exceptions.h"



// Class deScene
//////////////////

// Constructor, destructor
////////////////////////////

deScene::deScene(deSceneManager *manager, deVirtualFileSystem *vfs,
const char *filename, TIME_SYSTEM modificationTime) :
deFileResource(manager, vfs, filename, modificationTime){
}

deScene::~deScene(){
}



// Resource Management
////////////////////////

int deScene::GetResourceCount() const{
	return pResources.GetCount();
}

decStringList deScene::GetResourceKeys() const{
	const decTDictionary<decString, deResource::Ref>::KeyList keys(pResources.GetKeys());
	decStringList result;
	keys.Visit([&](const decString &key){ result.Add(key); });
	return result;
}

deResource *deScene::GetResourceNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	const deResource::Ref *resource = nullptr;
	return pResources.GetAt(name, resource) ? resource->Pointer() : nullptr;
}

bool deScene::HasResourceNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return pResources.Has(name);
}

void deScene::AddResource(const char *name, deResource *resource){
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(resource)
	if(pResources.Has(name)){
		DETHROW_INFO(deeInvalidParam, "resource with name already present");
	}
	pResources.SetAt(name, deResource::Ref(resource));
}

void deScene::RemoveResource(const char *name){
	DEASSERT_NOTNULL(name)
	if(!pResources.Has(name)){
		DETHROW_INFO(deeInvalidParam, "resource with name absent");
	}
	pResources.Remove(name);
}

void deScene::RemoveAllResources(){
	pResources.RemoveAll();
}



// Memory File Management
//////////////////////////

int deScene::GetMemoryFileCount() const{
	return pMemoryFiles.GetCount();
}

decStringList deScene::GetMemoryFileKeys() const{
	const decTDictionary<decString, decMemoryFile::Ref>::KeyList keys(pMemoryFiles.GetKeys());
	decStringList result;
	keys.Visit([&](const decString &key){ result.Add(key); });
	return result;
}

decMemoryFile *deScene::GetMemoryFileNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	const decMemoryFile::Ref *file = nullptr;
	return pMemoryFiles.GetAt(name, file) ? file->Pointer() : nullptr;
}

bool deScene::HasMemoryFileNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return pMemoryFiles.Has(name);
}

void deScene::AddMemoryFile(const char *name, decMemoryFile *file){
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(file)
	if(pMemoryFiles.Has(name)){
		DETHROW_INFO(deeInvalidParam, "memory file with name already present");
	}
	pMemoryFiles.SetAt(name, decMemoryFile::Ref(file));
}

void deScene::RemoveMemoryFile(const char *name){
	DEASSERT_NOTNULL(name)
	if(!pMemoryFiles.Has(name)){
		DETHROW_INFO(deeInvalidParam, "memory file with name absent");
	}
	pMemoryFiles.Remove(name);
}

void deScene::RemoveAllMemoryFiles(){
	pMemoryFiles.RemoveAll();
}
