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

#include "deSpeaker.h"
#include "deSpeakerManager.h"
#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/audio/deBaseAudioSpeaker.h"
#include "../../systems/modules/scripting/deBaseScriptingSpeaker.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/exceptions.h"



// Class deSpeakerManager
///////////////////////////

// Constructor, destructor
////////////////////////////

deSpeakerManager::deSpeakerManager(deEngine *engine) : deResourceManager(engine, ertSpeaker){
	SetLoggingName("speaker");
}

deSpeakerManager::~deSpeakerManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSpeakerManager::GetSpeakerCount() const{
	return pSpeakers.GetCount();
}

deSpeaker *deSpeakerManager::GetRootSpeaker() const{
	return (deSpeaker*)pSpeakers.GetRoot();
}

deSpeaker *deSpeakerManager::CreateSpeaker(){
	deSpeaker *speaker = NULL;
	
	try{
		speaker = new deSpeaker(this);
		if(!speaker) DETHROW(deeOutOfMemory);
		
		GetAudioSystem()->LoadSpeaker(speaker);
		GetScriptingSystem()->LoadSpeaker(speaker);
		
		pSpeakers.Add(speaker);
		
	}catch(const deException &){
		throw;
	}
	
	return speaker;
}



void deSpeakerManager::ReleaseLeakingResources(){
	int count = GetSpeakerCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking speakers", count);
		pSpeakers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSpeakerManager::SystemAudioLoad(){
	deSpeaker *speaker = (deSpeaker*)pSpeakers.GetRoot();
	deAudioSystem &audSys = *GetAudioSystem();
	
	while(speaker){
		if(!speaker->GetPeerAudio()){
			audSys.LoadSpeaker(speaker);
		}
		
		speaker = (deSpeaker*)speaker->GetLLManagerNext();
	}
}

void deSpeakerManager::SystemAudioUnload(){
	deSpeaker *speaker = (deSpeaker*)pSpeakers.GetRoot();
	
	while(speaker){
		speaker->SetPeerAudio(NULL);
		speaker = (deSpeaker*)speaker->GetLLManagerNext();
	}
}

void deSpeakerManager::SystemScriptingLoad(){
	deSpeaker *speaker = (deSpeaker*)pSpeakers.GetRoot();
	deScriptingSystem &scriptingSystem = *GetScriptingSystem();
	
	while(speaker){
		if(!speaker->GetPeerScripting()){
			scriptingSystem.LoadSpeaker(speaker);
		}
		speaker = (deSpeaker*)speaker->GetLLManagerNext();
	}
}

void deSpeakerManager::SystemScriptingUnload(){
	deSpeaker *speaker = (deSpeaker*)pSpeakers.GetRoot();
	
	while(speaker){
		speaker->SetPeerScripting(NULL);
		speaker = (deSpeaker*)speaker->GetLLManagerNext();
	}
}



void deSpeakerManager::RemoveResource(deResource *resource){
	pSpeakers.RemoveIfPresent(resource);
}
