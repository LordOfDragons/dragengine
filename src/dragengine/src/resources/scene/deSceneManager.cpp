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
#include "../../deEngine.h"
#include "../../common/file/decPath.h"
#include "../../common/exceptions.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/scene/deBaseSceneModule.h"



// Class deSceneManager
/////////////////////////

// Constructor, destructor
////////////////////////////

deSceneManager::deSceneManager(deEngine *engine) :
deFileResourceManager(engine, ertScene){
	SetLoggingName("scene");
}

deSceneManager::~deSceneManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSceneManager::GetSceneCount() const{
	return pScenes.GetCount();
}

deScene *deSceneManager::GetRootScene() const{
	return (deScene*)pScenes.GetRoot();
}

deScene *deSceneManager::GetSceneWith(const char *filename) const{
	return GetSceneWith(GetEngine()->GetVirtualFileSystem(), filename);
}

deScene *deSceneManager::GetSceneWith(deVirtualFileSystem *vfs, const char *filename) const{
	deScene * const scene = (deScene*)pScenes.GetWithFilename(vfs, filename);
	return scene && !scene->GetOutdated() ? scene : nullptr;
}

deScene::Ref deSceneManager::LoadScene(const char *filename, const char *basePath){
	return LoadScene(GetEngine()->GetVirtualFileSystem(), filename, basePath);
}

deScene::Ref deSceneManager::LoadScene(deVirtualFileSystem *vfs, const char *filename,
const char *basePath){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	
	deBaseSceneModule *module;
	deScene::Ref scene;
	decPath path;
	
	try{
		// locate file
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		// check if scene with this filename already exists
		deScene *findScene = (deScene*)pScenes.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findScene && findScene->GetModificationTime() != modificationTime){
			LogInfoFormat("Scene '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findScene->MarkOutdated();
			findScene = nullptr;
		}
		
		if(findScene){
			scene = findScene;
			
		}else{
			// find the module able to handle this scene file
			module = (deBaseSceneModule*)GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtScene, path.GetPathUnix());
			
			// load the file
			scene = deScene::Ref::New(this, vfs, path.GetPathUnix(), modificationTime);
			
			scene->SetAsynchron(false);
			module->LoadScene(OpenFileForReading(*vfs, path.GetPathUnix()), *scene);
			
			// add scene
			pScenes.Add(scene);
		}
		
	}catch(const deException &e){
		LogErrorFormat("Loading scene '%s' (base path '%s') failed", filename, basePath ? basePath : "");
		LogException(e);
		throw;
	}
	
	if(scene){
		scene->SetAsynchron(false);
	}
	
	return scene;
}

void deSceneManager::AddLoadedScene(deScene *scene){
	if(!scene){
		DETHROW(deeInvalidParam);
	}
	pScenes.Add(scene);
}

void deSceneManager::ReleaseLeakingResources(){
	const int count = GetSceneCount();
	
	if(count > 0){
		int unnamedSceneCount = 0;
		
		LogWarnFormat("%i leaking scenes", count);
		
		pScenes.GetResources().Visit([&](deResource *res){
			const deScene * const scene = static_cast<deScene*>(res);
			if(scene->GetFilename().IsEmpty()){
				unnamedSceneCount++;
			}else{
				LogWarnFormat("- %s", scene->GetFilename().GetString());
			}
		});
		
		if(unnamedSceneCount > 0){
			LogWarnFormat("%i unnamed scenes", unnamedSceneCount);
		}
		
		pScenes.RemoveAll(); // we do not delete them to avoid crashes. better leak than crash
	}
}



// Resource only functions
////////////////////////////

void deSceneManager::RemoveResource(deResource *resource){
	pScenes.RemoveIfPresent(resource);
}
