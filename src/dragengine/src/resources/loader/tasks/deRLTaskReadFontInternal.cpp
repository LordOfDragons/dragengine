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
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../parallel/deParallelProcessing.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/font/deBaseFontModule.h"



// Definitions
////////////////

#define LOGSOURCE "Resource Loader"


// Class deRLTaskReadFontInternal
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadFontInternal::deRLTaskReadFontInternal(deEngine &engine,
deResourceLoader &resourceLoader, deVirtualFileSystem *vfs, const char *path,
deRLTaskReadFont *parentTask) :
deResourceLoaderTask(engine, resourceLoader, vfs, path, deResourceLoader::ertFont),
pSucceeded(false),
pAlreadyLoaded(false),
pParentTask(parentTask),
pInternalTask(NULL)
{
	if(! parentTask){
		DETHROW(deeInvalidParam);
	}
	
	LogCreateEnter();
	pFont.TakeOver(new deFont(engine.GetFontManager(), vfs, path, 0));
	LogCreateExit();
}

deRLTaskReadFontInternal::~deRLTaskReadFontInternal(){
	if(pInternalTask){
		pInternalTask->FreeReference();
	}
}



// Management
///////////////

void deRLTaskReadFontInternal::Run(){
	LogRunEnter();
	
	deBaseFontModule * const module = (deBaseFontModule*)GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad(deModuleSystem::emtFont, GetPath());
	if(! module){
		DETHROW(deeInvalidParam);
	}
	
	const decPath vfsPath(decPath::CreatePathUnix(GetPath()));
	
	pFont->SetModificationTime(GetVFS()->GetFileModificationTime(vfsPath));
	pFont->SetAsynchron(true);
	module->LoadFont(decBaseFileReader::Ref::New(GetVFS()->OpenFileForReading(vfsPath)), pFont);
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadFontInternal::Finished(){
	LogFinishedEnter();
	
	if(! pSucceeded){
		SetState(esFailed);
		pFont = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask(this);
		return;
	}
	
	deEngine &engine = GetEngine();
	deFontManager &fontManager = *engine.GetFontManager();
	deFont * const checkFont = fontManager.GetFontWith(GetPath());
	
	if(checkFont){
		pAlreadyLoaded = true;
		SetResource(checkFont);
		SetState(esSucceeded);
		
	}else{
		pFont->SetAsynchron(false);
		SetResource(pFont);
		SetState(esSucceeded);
		
		try{
			pInternalTask = new deRLTaskReadFontInternal2(engine,
				GetResourceLoader(), GetVFS(), GetPath(), pFont);
			
			switch(pInternalTask->GetState()){
			case esPending:
				pParentTask->AddDependsOn(pInternalTask);
				engine.GetParallelProcessing().AddTask(pInternalTask);
				break;
				
			case esSucceeded:
				break;
				
			case esFailed:
				SetState(esFailed);
				break;
			}
			
		}catch(const deException &){
			SetState(esFailed);
		}
	}
	
	LogFinishedExit();
	GetResourceLoader().FinishTask(this);
}



// Debugging
//////////////

decString deRLTaskReadFontInternal::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-FontIntern";
}
