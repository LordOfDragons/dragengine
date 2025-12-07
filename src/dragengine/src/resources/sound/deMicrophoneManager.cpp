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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deMicrophone.h"
#include "deMicrophoneManager.h"
#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/audio/deBaseAudioMicrophone.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/exceptions.h"



// Class deMicrophoneManager
//////////////////////////////

// Constructor, destructor
////////////////////////////

deMicrophoneManager::deMicrophoneManager(deEngine *engine) : deResourceManager(engine, ertMicrophone){
	SetLoggingName("microphone");
}

deMicrophoneManager::~deMicrophoneManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deMicrophoneManager::GetMicrophoneCount() const{
	return pMicrophones.GetCount();
}

deMicrophone *deMicrophoneManager::GetRootMicrophone() const{
	return (deMicrophone*)pMicrophones.GetRoot();
}

deMicrophone *deMicrophoneManager::CreateMicrophone(){
	deMicrophone *microphone = NULL;
	
	try{
		microphone = new deMicrophone(this);
		if(! microphone) DETHROW(deeOutOfMemory);
		
		GetAudioSystem()->LoadMicrophone(microphone);
		
		pMicrophones.Add(microphone);
		
	}catch(const deException &){
		if(microphone){
			microphone->FreeReference();
		}
		throw;
	}
	
	return microphone;
}



void deMicrophoneManager::ReleaseLeakingResources(){
	int count = GetMicrophoneCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking microphones", count);
		pMicrophones.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deMicrophoneManager::SystemAudioLoad(){
	deMicrophone *microphone = (deMicrophone*)pMicrophones.GetRoot();
	deAudioSystem &audSys = *GetAudioSystem();
	
	while(microphone){
		if(! microphone->GetPeerAudio()){
			audSys.LoadMicrophone(microphone);
		}
		
		microphone = (deMicrophone*)microphone->GetLLManagerNext();
	}
}

void deMicrophoneManager::SystemAudioUnload(){
	deMicrophone *microphone = (deMicrophone*)pMicrophones.GetRoot();
	
	while(microphone){
		microphone->SetPeerAudio(NULL);
		microphone = (deMicrophone*)microphone->GetLLManagerNext();
	}
}

void deMicrophoneManager::RemoveResource(deResource *resource){
	pMicrophones.RemoveIfPresent(resource);
}
