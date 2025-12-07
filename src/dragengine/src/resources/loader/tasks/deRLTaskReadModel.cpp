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

#include "deRLTaskReadModel.h"
#include "../deResourceLoader.h"
#include "../../model/deModel.h"
#include "../../model/deModelManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/dePhysicsSystem.h"
#include "../../../systems/deAudioSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/model/deBaseModelModule.h"



// Class deRLTaskReadModel
////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadModel::deRLTaskReadModel(deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deModel *model) :
deResourceLoaderTask(engine, resourceLoader, vfs, path, deResourceLoader::ertModel),
pSucceeded(false)
{
	LogCreateEnter();
	// if already loaded set finished
	if(model){
		pModel = model;
		SetResource(model);
		SetState(esSucceeded);
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pModel.TakeOver(new deModel(engine.GetModelManager(), vfs, path, 0));
	LogCreateExit();
}

deRLTaskReadModel::~deRLTaskReadModel(){
}



// Management
///////////////

void deRLTaskReadModel::Run(){
	LogRunEnter();
	deBaseModelModule * const module = (deBaseModelModule*)GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad(deModuleSystem::emtModel, GetPath());
	if(!module){
		DETHROW(deeInvalidParam);
	}
	
	const decPath vfsPath(decPath::CreatePathUnix(GetPath()));
	
	pModel->SetModificationTime(GetVFS()->GetFileModificationTime(vfsPath));
	pModel->SetAsynchron(true);
	module->LoadModel(decBaseFileReader::Ref::New(GetVFS()->OpenFileForReading(vfsPath)), pModel);
	
	if(!pModel->Verify()){
		DETHROW(deeInvalidParam);
	}
	pModel->Prepare();
	
	GetEngine().GetGraphicSystem()->LoadModel(pModel);
	GetEngine().GetPhysicsSystem()->LoadModel(pModel);
	GetEngine().GetAudioSystem()->LoadModel(pModel);
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadModel::Finished(){
	LogFinishedEnter();
	if(!pSucceeded){
		SetState(esFailed);
		pModel = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask(this);
		return;
	}
	
	deModelManager &modelManager = *GetEngine().GetModelManager();
	deModel * const checkModel = modelManager.GetModelWith(GetPath());
	
	if(checkModel){
		SetResource(checkModel);
		
	}else{
		pModel->SetAsynchron(false);
		modelManager.AddLoadedModel(pModel);
		SetResource(pModel);
	}
	
	SetState(esSucceeded);
	LogFinishedExit();
	GetResourceLoader().FinishTask(this);
}



// Debugging
//////////////

decString deRLTaskReadModel::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Model-Read";
}
