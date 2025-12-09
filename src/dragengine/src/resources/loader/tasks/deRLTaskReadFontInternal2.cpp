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

#include "deRLTaskReadFontInternal2.h"
#include "../deResourceLoader.h"
#include "../../font/deFont.h"
#include "../../font/deFontManager.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/font/deBaseFontModule.h"



// Definitions
////////////////

#define LOGSOURCE "Resource Loader"


// Class deRLTaskReadFontInternal2
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadFontInternal2::deRLTaskReadFontInternal2(deEngine &engine,
deResourceLoader &resourceLoader, deVirtualFileSystem *vfs, const char *path, deFont *font) :
deResourceLoaderTask(engine, resourceLoader, vfs, path, deResourceLoader::ertFont),
pFont(font),
pAlreadyLoaded(false)
{
	if(!font){
		DETHROW(deeInvalidParam);
	}
	
	LogCreateEnter();
	
	SetEmptyRun(true);
	
	try{
		pLoadFontResources();
		
	}catch(const deException &){
		SetState(esFailed);
		Cancel();
	}
	
	LogCreateExit();
}

deRLTaskReadFontInternal2::~deRLTaskReadFontInternal2(){
}



// Management
///////////////

void deRLTaskReadFontInternal2::Run(){
}

void deRLTaskReadFontInternal2::Finished(){
	LogFinishedEnter();
	
	deFontManager &fontManager = *GetEngine().GetFontManager();
	deFont * const checkFont = fontManager.GetFontWith(GetPath());
	
	if(checkFont){
		pAlreadyLoaded = true;
		
	}else{
		// check if loadimg image finished successfully
		if(pTaskImage){
			deImage * const image = (deImage*)pTaskImage->GetResource().Pointer();
			if(pTaskImage->GetState() != esSucceeded || !image){
				SetState(esFailed);
				LogFinishedExit();
				GetResourceLoader().FinishTask(this);
				return;
			}
			
			pFont->SetImage(image);
		}
	}
	
	SetState(esSucceeded);
	
	LogFinishedExit();
	GetResourceLoader().FinishTask(this);
}



// Debugging
//////////////

decString deRLTaskReadFontInternal2::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-FontIntern2";
}



// Private Functions
//////////////////////

void deRLTaskReadFontInternal2::pLoadFontResources(){
	decPath basePath(decPath::CreatePathUnix(pFont->GetFilename()));
	basePath.RemoveLastComponent();
	
	decString path;
	
	// load image if present
	if(!pFont->GetImage()){
		path = pFont->GetImagePath();
		if(!path.IsEmpty()){
			if(!decPath::IsUnixPathAbsolute(path)){
				decPath resourcePath(basePath);
				resourcePath.AddUnixPath(path);
				path = resourcePath.GetPathUnix();
			}
			
			pTaskImage = GetResourceLoader().AddLoadRequest(GetVFS(), path, deResourceLoader::ertImage);
			
			if(pTaskImage->GetState() == esPending && !DoesDependOn(pTaskImage)){
				AddDependsOn(pTaskImage);
			}
		}
	}
}
