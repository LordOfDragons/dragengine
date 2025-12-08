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

#include "deRLTaskReadSkinInternal.h"
#include "deRLTaskReadSkinProperty.h"
#include "../deResourceLoader.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../skin/deSkin.h"
#include "../../skin/deSkinManager.h"
#include "../../skin/deSkinTexture.h"
#include "../../skin/property/deSkinProperty.h"
#include "../../skin/property/deSkinPropertyImage.h"
#include "../../skin/property/node/deSkinPropertyNodeImage.h"
#include "../../skin/property/node/deSkinPropertyNodeText.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../logger/deLogger.h"
#include "../../../parallel/deParallelProcessing.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/skin/deBaseSkinModule.h"



// Definitions
////////////////

#define LOGSOURCE "Resource Loader"



// Class deRLTaskReadSkinInternal::cInternalTask
//////////////////////////////////////////////////

deRLTaskReadSkinInternal::cInternalTask::cInternalTask(
deSkinPropertyImage *propertyImage, deResourceLoaderTask *task) :
pPropertyImage(propertyImage),
pNodeImage(NULL),
pNodeText(NULL),
pTask(task)
{
	if(!task || !propertyImage){
		DETHROW(deeInvalidParam);
	}
	task->AddReference();
}

deRLTaskReadSkinInternal::cInternalTask::cInternalTask(
deSkinPropertyNodeImage *nodeImage, deResourceLoaderTask *task) :
pPropertyImage(NULL),
pNodeImage(nodeImage),
pNodeText(NULL),
pTask(task)
{
	if(!task || !nodeImage){
		DETHROW(deeInvalidParam);
	}
	task->AddReference();
}

deRLTaskReadSkinInternal::cInternalTask::cInternalTask(
deSkinPropertyNodeText *nodeText, deResourceLoaderTask *task) :
pPropertyImage(NULL),
pNodeImage(NULL),
pNodeText(nodeText),
pTask(task)
{
	if(!task || !nodeText){
		DETHROW(deeInvalidParam);
	}
	task->AddReference();
}

deRLTaskReadSkinInternal::cInternalTask::~cInternalTask(){
}



// Class deRLTaskReadSkinInternal
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadSkinInternal::deRLTaskReadSkinInternal(deEngine &engine,
deResourceLoader &resourceLoader, deVirtualFileSystem *vfs, const char *path) :
deResourceLoaderTask(engine, resourceLoader, vfs, path, deResourceLoader::ertSkin)
{
	LogCreateEnter();
	
	SetEmptyRun(true);
	
	// skins have to be loaded always synchronously. this is because the texture property map is accessed
	// for each loaded texture property. doing this asynchronously has very ill effects up to crashing.
	// loading synchronously is not a problem since skins are small files only defining where to get
	// the resources from, namely images. these are loaded later on in an asynchronous way
	try{
		pPrepare();
		
	}catch(const deException &){
		// WARNING this here is vital for exceptions thrown inside the constructor.
		// 
		// the problem is subtle but the result desastrous. if an exception is thrown in the
		// constructor the compiler unwinds the object by calling deObject destructor in the end.
		// the compiler has no knowledge about reference counting in this situation. if a task
		// is added as dependency the reference count is increased and the child tasks hold
		// references. the deObject destructor then frees the object although other tasks own a
		// reference and expect the object to stay alive.
		// 
		// there are two ways to solve this problem.
		// 
		// the first way requires not adding tasks as dependencies of this task to not cause
		// references to be claimed. this requires delaying adding tasks as dependencies until
		// after the constructor finished.
		// 
		// the second way requires dropping all references in case of an exception to return to
		// the situation of nobody holding a reference. this avoids the need to track who needs
		// to be added but still works if only dependency tasks claim references.
		// 
		// to solve the problem the second solution is used. thus the explicit cleaning up here.
		// this could be done also in pCleanUp() with no negative effect. left here for claritiy
		
		//RemoveAllDependsOn(); // if we do not throw we do not need to do this
		//pCleanUp();
		//throw;
		
		SetState(esFailed);
		Cancel();
	}
	LogCreateExit();
}

deRLTaskReadSkinInternal::~deRLTaskReadSkinInternal(){
	pCleanUp();
}



// Management
///////////////

void deRLTaskReadSkinInternal::Run(){
}

void deRLTaskReadSkinInternal::Finished(){
	LogFinishedEnter();
	if(IsCancelled() || !pApplyInternal()){
		SetState(esFailed);
		pSkin = NULL;
		LogFinishedExit();
		return;
	}
	
	SetResource(pSkin);
	SetState(esSucceeded);
	LogFinishedExit();
}



// Internal use only
//////////////////////

void deRLTaskReadSkinInternal::AddInternalTask(cInternalTask *task){
	if(!task){
		DETHROW(deeInvalidParam);
	}
	
	if(GetResourceLoader().GetOutputDebugMessages()){
		const decString debugName(GetDebugName());
		const decString debugNameInternal(task->GetTask()->GetDebugName());
		GetEngine().GetLogger()->LogInfoFormat(LOGSOURCE, "Task(%s)[%s] AddInternalTask(%s, %s)",
			debugName.GetString(), GetPath().GetString(), debugNameInternal.GetString(),
			task->GetTask()->GetPath().GetString());
	}
	
	pInternalTasks.Add(task);
	
	switch(task->GetTask()->GetState()){
	case esPending:
		if(!DoesDependOn(task->GetTask())){
			AddDependsOn(task->GetTask());
		}
		break;
		
	case esSucceeded:
		break;
		
	case esFailed:
		SetState(esFailed);
		Cancel();
		break;
	}
}



// Debugging
//////////////

decString deRLTaskReadSkinInternal::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-SkinIntern";
}



// Private Functions
//////////////////////

void deRLTaskReadSkinInternal::pCleanUp(){
	pInternalTasks.RemoveAll();
	pSkin = NULL;
}

void deRLTaskReadSkinInternal::pPrepare(){
	deBaseSkinModule * const module = (deBaseSkinModule*)GetEngine().GetModuleSystem()->
		GetModuleAbleToLoad(deModuleSystem::emtSkin, GetPath());
	if(!module){
		DETHROW(deeInvalidParam);
	}
	
	const decPath path(decPath::CreatePathUnix(GetPath()));
	
	try{
		pSkin.TakeOverWith(GetEngine().GetSkinManager(), GetVFS(), GetPath(),
			GetVFS()->GetFileModificationTime(path));
		pSkin->SetAsynchron(true);
		
		module->LoadSkin(decBaseFileReader::Ref::New(GetVFS()->OpenFileForReading(path)), pSkin);
		
	}catch(const deException &){
		SetState(esFailed);
		pSkin = NULL;
		Cancel();
		return;
	}
	
	pLoadInternal();
}

void deRLTaskReadSkinInternal::pLoadInternal(){
	const int textureCount = pSkin->GetTextureCount();
	int i, j;
	
	decPath basePath;
	basePath.SetFromUnix(GetPath());
	basePath.RemoveLastComponent();
	basePath.SetPrefix("/");
	
	const decString strBasePath(basePath.GetPathUnix());
	deRLTaskReadSkinProperty visitor(GetResourceLoader(), *this, GetEngine(), GetVFS(), strBasePath);
	
	for(i=0; i<textureCount; i++){
		const deSkinTexture &texture = *pSkin->GetTextureAt(i);
		const int propertyCount = texture.GetPropertyCount();
		
		for(j=0; j<propertyCount; j++){
			texture.GetPropertyAt(j)->Visit(visitor);
		}
	}
}

bool deRLTaskReadSkinInternal::pApplyInternal(){
	const int count = pInternalTasks.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const cInternalTask &internalTask = *((cInternalTask*)pInternalTasks.GetAt(i));
		const deResourceLoaderTask &task = *internalTask.GetTask();
		
		if(task.GetState() != esSucceeded){
			return false;
		}
		
		if(internalTask.GetPropertyImage()){
			deSkinPropertyImage &property = *internalTask.GetPropertyImage();
			
			if(task.GetResource()){
				property.SetImage((deImage*)task.GetResource());
				
			}else{
				deImage *fallbackImage = NULL;
				
				try{
					fallbackImage = GetEngine().GetImageManager()->LoadDefault();
					property.SetImage(fallbackImage);
					fallbackImage->FreeReference();
					
				}catch(const deException &){
					if(fallbackImage){
						fallbackImage->FreeReference();
					}
					return false;
				}
			}
			
		}else if(internalTask.GetNodeImage()){
			deSkinPropertyNodeImage &node = *internalTask.GetNodeImage();
			
			if(task.GetResource()){
				node.SetImage((deImage*)task.GetResource());
				
			}else{
				deImage *fallbackImage = NULL;
				
				try{
					fallbackImage = GetEngine().GetImageManager()->LoadDefault();
					node.SetImage(fallbackImage);
					fallbackImage->FreeReference();
					
				}catch(const deException &){
					if(fallbackImage){
						fallbackImage->FreeReference();
					}
					return false;
				}
			}
			
		}else if(internalTask.GetNodeText()){
			deSkinPropertyNodeText &node = *internalTask.GetNodeText();
			
			if(task.GetResource()){
				node.SetFont((deFont*)task.GetResource());
				
			}else{
				return false;
			}
		}
	}
	
	return true;
}
