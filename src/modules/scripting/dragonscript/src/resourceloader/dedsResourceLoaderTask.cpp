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

#include <libdscript/libdscript.h>
// #include <libdscript/exceptions.h>

#include "dedsResourceLoaderTask.h"
#include "../deScriptingDragonScript.h"
#include "../classes/animation/deClassAnimation.h"
#include "../classes/graphics/deClassFont.h"
#include "../classes/graphics/deClassImage.h"
#include "../classes/graphics/deClassOcclusionMesh.h"
#include "../classes/resources/deClassResourceListener.h"
#include "../classes/sound/deClassSound.h"
#include "../classes/translation/deClassLanguagePack.h"
#include "../classes/video/deClassVideo.h"
#include "../classes/world/deClassModel.h"
#include "../classes/world/deClassRig.h"
#include "../classes/world/deClassSkin.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/deFileResource.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/video/deVideo.h>



// Class dedsResourceLoaderTask
/////////////////////////////////

// Constructor, destructor
////////////////////////////

dedsResourceLoaderTask::dedsResourceLoaderTask(deScriptingDragonScript *ds,
const char *filename, deResourceLoader::eResourceType resourceType) :
pDS(ds),
pFilename(filename),
pResourceType(resourceType),
pListeners(NULL),
pListenerCount(0),
pListenerSize(0)
{
	if(! ds){
		DETHROW(deeInvalidParam);
	}
}

dedsResourceLoaderTask::~dedsResourceLoaderTask(){
	pClearListeners();
	if(pListeners){
		delete [] pListeners;
	}
}



// Management
///////////////

bool dedsResourceLoaderTask::Matches(const char *filename,
deResourceLoader::eResourceType resourceType) const{
	return resourceType == pResourceType && pFilename == filename;
}

void dedsResourceLoaderTask::AddListener(dsRealObject *listener){
	if(! listener){
		DETHROW(deeInvalidParam);
	}
	dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
	
	if(pListenerCount == pListenerSize){
		int newSize = pListenerSize * 3 / 2 + 1;
		dsValue **newArray = new dsValue*[newSize];
		if(pListeners){
			memcpy(newArray, pListeners, sizeof(dsValue*) * pListenerSize);
			delete [] pListeners;
		}
		pListeners = newArray;
		pListenerSize = newSize;
	}
	
	pListeners[pListenerCount] = rt->CreateValue(pDS->GetClassResourceListener());
	rt->SetObject(pListeners[pListenerCount], listener);
	rt->CastValueTo(pListeners[pListenerCount], pListeners[pListenerCount], pDS->GetClassResourceListener());
	pListenerCount++;
}

void dedsResourceLoaderTask::RemoveListener(dsRealObject *listener){
	if(! listener){
		DETHROW(deeInvalidParam);
	}
	dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
	int i, j;
	
	for(i=0; i<pListenerCount; i++){
		if(listener == pListeners[i]->GetRealObject()){
			rt->FreeValue(pListeners[i]);
			for(j=i+1; j<pListenerCount; j++){
				pListeners[i - 1] = pListeners[i];
			}
			return;
		}
	}
}

void dedsResourceLoaderTask::NotifyLoadingFinished(deFileResource *resource){
	if(! resource){
		DETHROW(deeInvalidParam);
	}
	
	const int funcIndex = pDS->GetClassResourceListener()->GetFuncIndexFinishedLoading();
	dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
	int i;
	
	for(i=0; i<pListenerCount; i++){
		// finishedLoading( filename, resourceType, resource )
		switch(pResourceType){
		case deResourceLoader::ertAnimation:
			pDS->GetClassAnimation()->PushAnimation(rt, (deAnimation*)resource);
			break;
			
		case deResourceLoader::ertFont:
			pDS->GetClassFont()->PushFont(rt, (deFont*)resource, 10); // problem, font has a size
			break;
			
		case deResourceLoader::ertImage:
			pDS->GetClassImage()->PushImage(rt, (deImage*)resource);
			break;
			
		case deResourceLoader::ertModel:
			pDS->GetClassModel()->PushModel(rt, (deModel*)resource);
			break;
			
		case deResourceLoader::ertRig:
			pDS->GetClassRig()->PushRig(rt, (deRig*)resource);
			break;
			
		case deResourceLoader::ertSkin:
			pDS->GetClassSkin()->PushSkin(rt, (deSkin*)resource);
			break;
			
		case deResourceLoader::ertSound:
			pDS->GetClassSound()->PushSound(rt, (deSound*)resource);
			break;
			
		case deResourceLoader::ertOcclusionMesh:
			pDS->GetClassOcclusionMesh()->PushOcclusionMesh(rt, (deOcclusionMesh*)resource);
			break;
			
		case deResourceLoader::ertLanguagePack:
			pDS->GetClassLanguagePack()->PushLanguagePack(rt, (deLanguagePack*)resource);
			break;
			
		case deResourceLoader::ertVideo:
			pDS->GetClassVideo()->PushVideo(rt, (deVideo*)resource);
			break;
			
		default:
			DETHROW(deeInvalidParam); // TODO do something more smart here
		}
		rt->PushValue(pDS->GetClassResourceLoaderType()->GetVariable(pResourceType)->GetStaticValue());
		rt->PushString(pFilename);
		rt->RunFunctionFast(pListeners[i], funcIndex);
	}
	
	pClearListeners();
}

void dedsResourceLoaderTask::NotifyLoadingFailed(){
	const int funcIndex = pDS->GetClassResourceListener()->GetFuncIndexFailedLoading();
	dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
	int i;
	
	for(i=0; i<pListenerCount; i++){
		// failedLoading( filename, resourceType )
		rt->PushValue(pDS->GetClassResourceLoaderType()->GetVariable(pResourceType)->GetStaticValue());
		rt->PushString(pFilename);
		rt->RunFunctionFast(pListeners[i], funcIndex);
	}
	
	pClearListeners();
}



// Private Functions
//////////////////////

void dedsResourceLoaderTask::pClearListeners(){
	if(pListeners){
		dsRunTime *rt = pDS->GetScriptEngine()->GetMainRunTime();
		
		while(pListenerCount > 0){
			pListenerCount--;
			rt->FreeValue(pListeners[pListenerCount]);
		}
	}
}
