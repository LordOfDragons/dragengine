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

#include "deRLTaskReadSound.h"
#include "../deResourceLoader.h"
#include "../../sound/deSound.h"
#include "../../sound/deSoundManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deAudioSystem.h"
#include "../../../systems/deSynthesizerSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/sound/deBaseSoundModule.h"
#include "../../../systems/modules/sound/deBaseSoundInfo.h"



// Class deRLTaskReadSound
////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadSound::deRLTaskReadSound(deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deSound *sound) :
deResourceLoaderTask(engine, resourceLoader, vfs, path, deResourceLoader::ertSound),
pSucceeded(false)
{
	LogCreateEnter();
	// if already loaded set finished
	if(sound){
		pSound = sound;
		SetResource(sound);
		SetState(esSucceeded);
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pSound = deSound::Ref::New(engine.GetSoundManager(), vfs, path, 0);
	LogCreateExit();
}

deRLTaskReadSound::~deRLTaskReadSound(){
}



// Management
///////////////

void deRLTaskReadSound::Run(){
	LogRunEnter();
	deBaseSoundModule * const module = (deBaseSoundModule*)GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad(deModuleSystem::emtSound, GetPath());
	if(!module){
		DETHROW(deeInvalidParam);
	}
	
	const decPath vfsPath(decPath::CreatePathUnix(GetPath()));
	
	deBaseSoundInfo soundInfo;
	module->InitLoadSound(GetVFS()->OpenFileForReading(vfsPath), soundInfo);
	
	pSound->SetModificationTime(GetVFS()->GetFileModificationTime(vfsPath));
	pSound->SetAsynchron(true);
	pSound->FinalizeConstruction(soundInfo.GetBytesPerSample(),
		soundInfo.GetSampleRate(), soundInfo.GetSampleCount(),
		soundInfo.GetChannelCount());
	
	// create peers. modules can request to load the sound data if small enough
	GetEngine().GetAudioSystem()->LoadSound(pSound);
	GetEngine().GetSynthesizerSystem()->LoadSound(pSound);
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadSound::Finished(){
	LogFinishedEnter();
	if(!pSucceeded){
		SetState(esFailed);
		pSound = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask(this);
		return;
	}
	
	deSoundManager &soundManager = *GetEngine().GetSoundManager();
	deSound * const checkSound = soundManager.GetSoundWith(GetPath());
	
	if(checkSound){
		SetResource(checkSound);
		
	}else{
		pSound->SetAsynchron(false);
		soundManager.AddLoadedSound(pSound);
		SetResource(pSound);
	}
	
	SetState(esSucceeded);
	LogFinishedExit();
	GetResourceLoader().FinishTask(this);
}



// Debugging
//////////////

decString deRLTaskReadSound::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Sound-Read";
}
