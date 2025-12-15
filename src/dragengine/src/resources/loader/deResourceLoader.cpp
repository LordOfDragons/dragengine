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

#include "deResourceLoader.h"
#include "deResourceLoaderInfo.h"
#include "tasks/deRLTaskReadAnimation.h"
#include "tasks/deRLTaskReadFont.h"
#include "tasks/deRLTaskReadImage.h"
#include "tasks/deRLTaskReadLanguagePack.h"
#include "tasks/deRLTaskReadModel.h"
#include "tasks/deRLTaskReadOcclusionMesh.h"
#include "tasks/deRLTaskReadRig.h"
#include "tasks/deRLTaskReadSkin.h"
#include "tasks/deRLTaskReadSound.h"
#include "tasks/deRLTaskReadVideo.h"
#include "tasks/deRLTaskWriteAnimation.h"
#include "tasks/deRLTaskWriteImage.h"
#include "tasks/deRLTaskWriteModel.h"
#include "tasks/deRLTaskWriteOcclusionMesh.h"
#include "tasks/deRLTaskWriteRig.h"
#include "../deFileResource.h"
#include "../animation/deAnimation.h"
#include "../animation/deAnimationManager.h"
#include "../font/deFont.h"
#include "../font/deFontManager.h"
#include "../image/deImage.h"
#include "../image/deImageManager.h"
#include "../localization/deLanguagePack.h"
#include "../localization/deLanguagePackManager.h"
#include "../model/deModel.h"
#include "../model/deModelManager.h"
#include "../occlusionmesh/deOcclusionMesh.h"
#include "../occlusionmesh/deOcclusionMeshManager.h"
#include "../rig/deRig.h"
#include "../rig/deRigManager.h"
#include "../skin/deSkin.h"
#include "../skin/deSkinManager.h"
#include "../sound/deSound.h"
#include "../sound/deSoundManager.h"
#include "../video/deVideo.h"
#include "../video/deVideoManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"
#include "../../parallel/deParallelProcessing.h"



// Definitions
////////////////

#define LOGSOURCE "Resource Loader"



// Class deResourceLoader
///////////////////////////

// Constructor, destructor
////////////////////////////

deResourceLoader::deResourceLoader(deEngine &engine) :
pEngine(engine),
pLoadAsynchron(true),
pOutputDebugMessages(false){
}

deResourceLoader::~deResourceLoader(){
	pCleanUp();
}



// Management
///////////////

void deResourceLoader::SetOutputDebugMessages(bool outputDebugMessages){
	pOutputDebugMessages = outputDebugMessages;
}

deResourceLoaderTask *deResourceLoader::AddLoadRequest(deVirtualFileSystem *vfs,
const char *path, eResourceType resourceType){
	// if a tasks exists already use this one. tasks stick around only as long as
	// the script module has not collected them
	deResourceLoaderTask * const findTask = pGetTaskWith(vfs, path, resourceType);
	if(findTask){
		return findTask;
	}
	
	// create and add task
	deResourceLoaderTask::Ref task;
	
	try{
		// create task
		switch(resourceType){
		case ertAnimation:{
			deAnimation::Ref animation(pEngine.GetAnimationManager()->GetAnimationWith(vfs, path));
			if(!animation && !pLoadAsynchron){
				animation = pEngine.GetAnimationManager()->LoadAnimation(vfs, path, "/");
			}
			task = deRLTaskReadAnimation::Ref::New(pEngine, *this, vfs, path, animation);
			}break;
			
		case ertFont:{
			deFont::Ref font(pEngine.GetFontManager()->GetFontWith(vfs, path));
			if(!font && !pLoadAsynchron){
				font = pEngine.GetFontManager()->LoadFont(vfs, path, "/");
			}
			task = deRLTaskReadFont::Ref::New(pEngine, *this, vfs, path, font);
			}break;
			
		case ertImage:{
			deImage::Ref image(pEngine.GetImageManager()->GetImageWith(vfs, path));
			if(!image && !pLoadAsynchron){
				image = pEngine.GetImageManager()->LoadImage(vfs, path, "/");
			}
			task = deRLTaskReadImage::Ref::New(pEngine, *this, vfs, path, image);
			}break;
			
		case ertModel:{
			deModel::Ref model(pEngine.GetModelManager()->GetModelWith(vfs, path));
			if(!model && !pLoadAsynchron){
				model = pEngine.GetModelManager()->LoadModel(vfs, path, "/");
			}
			task = deRLTaskReadModel::Ref::New(pEngine, *this, vfs, path, model);
			}break;
			
		case ertOcclusionMesh:{
			deOcclusionMesh::Ref occlusionMesh(pEngine.GetOcclusionMeshManager()->
				GetOcclusionMeshWith(vfs, path));
			if(!occlusionMesh && !pLoadAsynchron){
				occlusionMesh = pEngine.GetOcclusionMeshManager()->LoadOcclusionMesh(vfs, path, "/");
			}
			task = deRLTaskReadOcclusionMesh::Ref::New(pEngine, *this, vfs, path, occlusionMesh);
			}break;
			
		case ertRig:{
			deRig::Ref rig(pEngine.GetRigManager()->GetRigWith(vfs, path));
			if(!rig && !pLoadAsynchron){
				rig = pEngine.GetRigManager()->LoadRig(vfs, path, "/");
			}
			task = deRLTaskReadRig::Ref::New(pEngine, *this, vfs, path, rig);
			}break;
			
		case ertSkin:{
			deSkin::Ref skin(pEngine.GetSkinManager()->GetSkinWith(vfs, path));
			if(!skin && !pLoadAsynchron){
				skin = pEngine.GetSkinManager()->LoadSkin(vfs, path, "/");
			}
			task = deRLTaskReadSkin::Ref::New(pEngine, *this, vfs, path, skin);
			}break;
			
		case ertSound:{
			deSound::Ref sound(pEngine.GetSoundManager()->GetSoundWith(vfs, path));
			if(!sound && !pLoadAsynchron){
				sound = pEngine.GetSoundManager()->LoadSound(path, "/", false);
			}
			task = deRLTaskReadSound::Ref::New(pEngine, *this, vfs, path, sound);
			}break;
			
		case ertLanguagePack:{
			deLanguagePack::Ref langPack(pEngine.GetLanguagePackManager()->
				GetLanguagePackWith(vfs, path));
			if(!langPack && !pLoadAsynchron){
				langPack = pEngine.GetLanguagePackManager()->LoadLanguagePack(vfs, path, "/");
			}
			task = deRLTaskReadLanguagePack::Ref::New(pEngine, *this, vfs, path, langPack);
			}break;
			
		case ertVideo:{
			deVideo::Ref video(pEngine.GetVideoManager()->GetVideoWith(vfs, path));
			if(!video && !pLoadAsynchron){
				video = pEngine.GetVideoManager()->LoadVideo(path, "/", false);
			}
			task = deRLTaskReadVideo::Ref::New(pEngine, *this, vfs, path, video);
			}break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		
	}catch(const deException &e){
		pEngine.GetLogger()->LogException(LOGSOURCE, e);
		throw;
	}
	
	// add task to the appropriate list
	if(task->GetState() == deResourceLoaderTask::esPending){
		if(pOutputDebugMessages){
			const decString debugName(task->GetDebugName());
			pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Add Pending Task(%s)[%s]",
				debugName.GetString(), path);
		}
		pPendingTasks.Add(task);
		pEngine.GetParallelProcessing().AddTask(task);
		
	}else{
		if(pOutputDebugMessages){
			const decString debugName(task->GetDebugName());
			pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Add Finished Task(%s)[%s]",
				debugName.GetString(), path);
		}
		pFinishedTasks.Add(task);
	}
	return task;
}

deResourceLoaderTask *deResourceLoader::AddSaveRequest(deVirtualFileSystem *vfs,
const char *path, deFileResource *resource){
	// TODO
	// note. if resource is finished mark created task finished
	return NULL;
}

bool deResourceLoader::NextFinishedRequest(deResourceLoaderInfo &info){
	deResourceLoaderTask::Ref task;
	
	while(true){
		if(pFinishedTasks.IsNotEmpty()){
			task = pFinishedTasks.First();
			pFinishedTasks.Remove(task);
		}
		
		if(task){
			// taskX->GetType() = deResourceLoaderTaskX::{etRead, etWrite}
			info.SetPath(task->GetPath());
			info.SetResourceType(task->GetResourceType());
			
			switch(task->GetState()){
			case deResourceLoaderTask::esSucceeded:
				// NOTE if task is purely internal counting finished files becomes
				//      a problem since the counter is off. should be anyways not
				//      done using counters since this is in general a problem
				//      (etRead)
				info.SetResource(task->GetResource());
				break;
				
			case deResourceLoaderTask::esFailed:
				info.SetResource(NULL);
				break;
				
			default:{
				const decString debugName(task->GetDebugName());
				pEngine.GetLogger()->LogInfoFormat(LOGSOURCE, "Finished Task has invalid state (%s)[%s][%d]",
					debugName.GetString(), task->GetPath().GetString(), task->GetState());
				DETHROW(deeInvalidParam);
				}
			}
			return true;
		}
		
		break;
	}
	
	return false;
}



void deResourceLoader::RemoveAllTasks(){
	if(pOutputDebugMessages){
		pEngine.GetLogger()->LogInfo(LOGSOURCE, "Stop tasks in progress and clear all tasks");
	}
	
	pPendingTasks.Visit([](deResourceLoaderTask *task){
		task->Cancel();
	});
	
	const bool resumeParallel = !pEngine.GetParallelProcessing().GetPaused();
	
	if(resumeParallel){
		pEngine.GetParallelProcessing().Pause();
	}
	
	pPendingTasks.RemoveAll();
	pFinishedTasks.RemoveAll();
	
	if(resumeParallel){
		pEngine.GetParallelProcessing().Resume();
	}
}



// Internal use only
//////////////////////

void deResourceLoader::FinishTask(deResourceLoaderTask *task){
	if(!task){
		DETHROW(deeInvalidParam);
	}
	pFinishedTasks.AddIfAbsent(task);
	pPendingTasks.RemoveIfPresent(task);
}



// Private Functions
//////////////////////

void deResourceLoader::pCleanUp(){
	RemoveAllTasks();
}

bool deResourceLoader::pHasTaskWith(deVirtualFileSystem *vfs,
const char *path, eResourceType resourceType) const{
	auto visitor = [&](const deResourceLoaderTask::Ref &task) {
		return task->Matches(vfs, path, resourceType);
	};
	
	const deResourceLoaderTask::Ref *t;
	return pPendingTasks.Find(visitor, t) || pFinishedTasks.Find(visitor, t);
}

deResourceLoaderTask *deResourceLoader::pGetTaskWith(deVirtualFileSystem *vfs,
const char *path, eResourceType resourceType) const{
	auto visitor = [&](const deResourceLoaderTask::Ref &task) {
		return task->Matches(vfs, path, resourceType);
	};
	
	const deResourceLoaderTask::Ref *t;
	return pPendingTasks.Find(visitor, t) || pFinishedTasks.Find(visitor, t) ? *t : nullptr;
}
