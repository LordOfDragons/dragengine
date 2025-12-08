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

#include "deRLTaskReadFont.h"
#include "deRLTaskReadFontInternal.h"
#include "deRLTaskReadFontInternal2.h"
#include "../deResourceLoader.h"
#include "../../font/deFont.h"
#include "../../font/deFontManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../parallel/deParallelProcessing.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/font/deBaseFontModule.h"



// Class deRLTaskReadFont
///////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadFont::deRLTaskReadFont(deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deFont *font) :
deResourceLoaderTask(engine, resourceLoader, vfs, path, deResourceLoader::ertFont),
pSucceeded(false)
{
	LogCreateEnter();
	
	// if already loaded set finished
	if(font){
		SetResource(font);
		SetState(esSucceeded);
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	// the internal task is responsible to load the font and to start loading used other
	// resources during finished function. after this is done this task has to collect
	// the loaded resources, creating font peers and finalize the font. creating the
	// peers can be done asynchronous
	try{
		pInternalTask.TakeOver(new deRLTaskReadFontInternal(engine, resourceLoader, vfs, path, this));
		
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
		SetState(esFailed);
		Cancel();
	}
	
	LogCreateExit();
}

deRLTaskReadFont::~deRLTaskReadFont(){
}



// Management
///////////////

void deRLTaskReadFont::Run(){
	LogRunEnter();
	
	// checks
	if(!pInternalTask){
		DETHROW(deeInvalidParam);
	}
	
	if(pInternalTask->GetAlreadyLoaded()){
		pSucceeded = true;
		LogRunExit();
		return;
	}
	
	deFont * const font = pInternalTask->GetFont();
	if(!font){
		DETHROW(deeInvalidParam);
	}
	
	const deRLTaskReadFontInternal2 * const internalTask2 = pInternalTask->GetInternalTask();
	if(!internalTask2){
		DETHROW(deeInvalidParam);
	}
	
	if(internalTask2->GetAlreadyLoaded()){
		pSucceeded = true;
		LogRunExit();
		return;
	}
	
	// verify font and update glyphs
	if(!font->Verify()){
		DETHROW(deeInvalidParam);
	}
	font->UpdateGlyphs();
	
	// create peers
	GetEngine().GetGraphicSystem()->LoadFont(font);
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadFont::Finished(){
	LogFinishedEnter();
	
	if(!pSucceeded || !pInternalTask){
		SetState(esFailed);
		LogFinishedExit();
		GetResourceLoader().FinishTask(this);
		return;
	}
	
	deFont * const font = pInternalTask->GetFont();
	if(pInternalTask->GetState() != esSucceeded || !font){
		SetState(esFailed);
		LogFinishedExit();
		GetResourceLoader().FinishTask(this);
		return;
	}
	
	const deRLTaskReadFontInternal2 * const internalTask2 = pInternalTask->GetInternalTask();
	bool alreadyLoaded = false;
	
	if(pInternalTask->GetAlreadyLoaded()){
		SetResource(font);
		alreadyLoaded = true;
		
	}else if(internalTask2){
		if(internalTask2->GetState() != esSucceeded){
			SetState(esFailed);
			LogFinishedExit();
			GetResourceLoader().FinishTask(this);
			return;
		}
		
		if(internalTask2->GetAlreadyLoaded()){
			SetResource(font);
			alreadyLoaded = true;
		}
	}
	
	if(!alreadyLoaded){
		deFontManager &fontManager = *GetEngine().GetFontManager();
		deFont * const checkFont = fontManager.GetFontWith(GetPath());
		
		if(checkFont){
			SetResource(checkFont);
			
		}else{
			font->SetAsynchron(false);
			fontManager.AddLoadedFont(font);
			SetResource(font);
		}
	}
	
	SetState(esSucceeded);
	LogFinishedExit();
	GetResourceLoader().FinishTask(this);
}



// Debugging
//////////////

decString deRLTaskReadFont::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Font-Read";
}
