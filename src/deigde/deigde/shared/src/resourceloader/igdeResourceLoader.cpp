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

#include "igdeResourceLoader.h"
#include "igdeResourceLoaderTask.h"
#include "igdeResourceLoaderListener.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/deFileResource.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/loader/deResourceLoaderInfo.h>



// Class igdeResourceLoader
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeResourceLoader::igdeResourceLoader(igdeEnvironment &environment) :
pEnvironment(environment){
}

igdeResourceLoader::~igdeResourceLoader(){
}



// Management
///////////////

void igdeResourceLoader::Update(){
	deResourceLoader &loader = *pEnvironment.GetEngineController()->GetEngine()->GetResourceLoader();
	deLogger &logger = *pEnvironment.GetLogger();
	deResourceLoaderInfo info;
	
	while(loader.NextFinishedRequest(info)){
		const int index = pIndexOfTaskWith(info.GetPath(), info.GetResourceType());
		if(index == -1){
			continue;
		}
		
		const igdeResourceLoaderTask::Ref task((igdeResourceLoaderTask*)pTasks.GetAt(index));
		pTasks.RemoveFrom(index);
		
		if(info.GetResource()){
			task->NotifyLoadingFinished(logger, info.GetResource());
			
		}else{
			task->NotifyLoadingFailed(logger);
		}
	}
}

void igdeResourceLoader::RequestResource(const char *filename,
deResourceLoader::eResourceType resourceType, igdeResourceLoaderListener *listener){
	if(!filename || !listener){
		DETHROW(deeInvalidParam);
	}
	
	int task = pIndexOfTaskWith(filename, resourceType);
	if(task == -1){
		pAddTask(filename, resourceType);
		task = pTasks.GetCount() - 1;
	}
	
	((igdeResourceLoaderTask*)pTasks.GetAt(task))->AddListener(listener);
}



// Private Functions
//////////////////////

int igdeResourceLoader::pIndexOfTaskWith(const char *filename,
deResourceLoader::eResourceType resourceType){
	int i;
	for(i=0; i<pTasks.GetCount(); i++){
		const igdeResourceLoaderTask &task = *((igdeResourceLoaderTask*)pTasks.GetAt(i));
		if(task.GetFilename() == filename && task.GetResourceType() == resourceType){
			return i;
		}
	}
	
	return -1;
}

void igdeResourceLoader::pAddTask(const char *filename,
deResourceLoader::eResourceType resourceType){
	const igdeResourceLoaderTask::Ref task(igdeResourceLoaderTask::Ref::NewWith(filename, resourceType));
	pTasks.Add(task);
	
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	engine.GetResourceLoader()->AddLoadRequest(engine.GetVirtualFileSystem(), filename, resourceType);
}
