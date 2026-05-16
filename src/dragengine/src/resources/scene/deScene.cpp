/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deScene.h"
#include "deSceneManager.h"
#include "../../common/exceptions.h"



// Class deScene
//////////////////

// Constructor, destructor
////////////////////////////

deScene::deScene(deSceneManager *manager, deVirtualFileSystem *vfs, const char *filename,
TIME_SYSTEM modificationTime) :
deFileResource(manager, vfs, filename, modificationTime){
}

deScene::~deScene() = default;



// Resource Management
////////////////////////

void deScene::AddResource(const char *name, deResource *resource){
	DEASSERT_NOTNULL(name)
	DEASSERT_NOTNULL(resource)
	DEASSERT_FALSE(pResources.Has(name))
	
	pResources.SetAt(name, resource);
}

void deScene::RemoveResource(const char *name){
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
	DEASSERT_FALSE(pFiles.Has(name))
	
	pFiles.SetAt(name, file);
}

void deScene::RemoveFile(const char *name){
	DEASSERT_NOTNULL(name)
	
	pFiles.Remove(name);
}

void deScene::RemoveAllFiles(){
	pFiles.RemoveAll();
}
