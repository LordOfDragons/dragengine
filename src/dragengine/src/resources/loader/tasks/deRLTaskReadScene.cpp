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

#include "deRLTaskReadScene.h"
#include "../deResourceLoader.h"
#include "../../scene/deScene.h"
#include "../../scene/deSceneManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/scene/deBaseSceneModule.h"



// Class deRLTaskReadScene
///////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadScene::deRLTaskReadScene(deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deScene *scene) :
deResourceLoaderTask(engine, resourceLoader, vfs, path, deResourceLoader::ertScene),
pSucceeded(false)
{
	LogCreateEnter();
	// if already loaded set finished
	if(scene){
		pScene = scene;
		SetResource(scene);
		SetState(esSucceeded);
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pScene = deScene::Ref::New(engine.GetSceneManager(), vfs, path, 0);
	LogCreateExit();
}

deRLTaskReadScene::~deRLTaskReadScene(){
}



// Management
///////////////

void deRLTaskReadScene::Run(){
	LogRunEnter();
	deBaseSceneModule * const module = (deBaseSceneModule*)GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad(deModuleSystem::emtScene, GetPath());
	if(!module){
		DETHROW(deeInvalidParam);
	}
	
	const decPath vfsPath(decPath::CreatePathUnix(GetPath()));
	
	pScene->SetModificationTime(GetVFS()->GetFileModificationTime(vfsPath));
	pScene->SetAsynchron(true);
	module->LoadScene(GetVFS()->OpenFileForReading(vfsPath), *pScene);
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadScene::Finished(){
	LogFinishedEnter();
	if(!pSucceeded){
		SetState(esFailed);
		pScene = nullptr;
		LogFinishedExit();
		GetResourceLoader().FinishTask(this);
		return;
	}
	
	deSceneManager &sceneManager = *GetEngine().GetSceneManager();
	deScene * const checkScene = sceneManager.GetSceneWith(GetPath());
	
	if(checkScene){
		SetResource(checkScene);
		
	}else{
		pScene->SetAsynchron(false);
		sceneManager.AddLoadedScene(pScene);
		SetResource(pScene);
	}
	
	SetState(esSucceeded);
	LogFinishedExit();
	GetResourceLoader().FinishTask(this);
}



// Debugging
//////////////

decString deRLTaskReadScene::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Scene-Read";
}
