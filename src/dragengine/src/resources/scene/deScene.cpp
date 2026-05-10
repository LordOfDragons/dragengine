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

void deScene::AddResource(const char *name, deResource *resource){
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(resource)
	if(pResources.Has(name)){
		DETHROW_INFO(deeInvalidParam, "resource with name already present");
	}
	pResources.SetAt(name, resource);
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



// File Management
///////////////////

void deScene::AddFile(const char *name, decMemoryFile *file){
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(file)
	if(pFiles.Has(name)){
		DETHROW_INFO(deeInvalidParam, "file with name already present");
	}
	pFiles.SetAt(name, file);
}

void deScene::RemoveFile(const char *name){
	DEASSERT_NOTNULL(name)
	if(!pFiles.Has(name)){
		DETHROW_INFO(deeInvalidParam, "file with name absent");
	}
	pFiles.Remove(name);
}

void deScene::RemoveAllFiles(){
	pFiles.RemoveAll();
}
