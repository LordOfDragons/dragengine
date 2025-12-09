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

#include "deRLTaskReadSkin.h"
#include "deRLTaskReadSkinInternal.h"
#include "../deResourceLoader.h"
#include "../../skin/deSkin.h"
#include "../../skin/deSkinManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../parallel/deParallelProcessing.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/deAudioSystem.h"
#include "../../../systems/dePhysicsSystem.h"



// Class deRLTaskReadSkin
///////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadSkin::deRLTaskReadSkin(deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deSkin *skin) :
deResourceLoaderTask(engine, resourceLoader, vfs, path, deResourceLoader::ertSkin),
pSucceeded(false)
{
	LogCreateEnter();
	// if already loaded set finished
	if(skin){
		SetResource(skin);
		SetState(esSucceeded);
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	// skins have to be loaded always synchronously. this is because the texture property
	// map is accessed for each loaded texture property. doing this asynchronously has
	// very ill effects up to crashing. loading synchronously is not a problem since skins
	// are small files only defining where to get the resources from, namely images. these
	// are loaded later on in an asynchronous way
	try{
		pInternalTask.TakeOverWith(engine, resourceLoader, vfs, path);
		
		switch(pInternalTask->GetState()){
		case esPending:
			AddDependsOn(pInternalTask);
			engine.GetParallelProcessing().AddTask(pInternalTask);
			break;
			
		case esSucceeded:
			break;
			
		case esFailed:
			SetState(esFailed);
			Cancel();
			break;
		}
		
	}catch(const deException &){
		// avoid nasty reference bug if exception thrown with bad timing
		//RemoveAllDependsOn(); // if we do not throw we do not need this
		//pCleanUp();
		//throw;
		
		SetState(esFailed);
		Cancel();
	}
	LogCreateExit();
}

deRLTaskReadSkin::~deRLTaskReadSkin(){
	pCleanUp();
}



// Management
///////////////

void deRLTaskReadSkin::Run(){
	LogRunEnter();
	if(!pInternalTask){
		DETHROW(deeInvalidParam);
	}
	
	deSkin * const skin = pInternalTask->GetSkin();
	if(!skin){
		DETHROW(deeInvalidParam);
	}
	
	skin->SetAsynchron(true);
	
	GetEngine().GetGraphicSystem()->LoadSkin(skin);
	GetEngine().GetAudioSystem()->LoadSkin(skin);
	GetEngine().GetPhysicsSystem()->LoadSkin(skin);
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadSkin::Finished(){
	LogFinishedEnter();
	if(!pSucceeded || !pInternalTask){
		SetState(esFailed);
		LogFinishedExit();
		GetResourceLoader().FinishTask(this);
		return;
	}
	
	deSkin * const skin = pInternalTask->GetSkin();
	if(pInternalTask->GetState() != esSucceeded || !skin){
		SetState(esFailed);
		LogFinishedExit();
		GetResourceLoader().FinishTask(this);
		return;
	}
	
	deSkinManager &skinManager = *GetEngine().GetSkinManager();
	deSkin * const checkSkin = skinManager.GetSkinWith(GetPath());
	
	if(checkSkin){
		SetResource(checkSkin);
		
	}else{
		skin->SetAsynchron(false);
		skinManager.AddLoadedSkin(skin);
		SetResource(skin);
	}
	
	SetState(esSucceeded);
	LogFinishedExit();
	GetResourceLoader().FinishTask(this);
}



// Debugging
//////////////

decString deRLTaskReadSkin::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Skin-Read";
}



// Private Functions
//////////////////////

void deRLTaskReadSkin::pCleanUp(){
}
