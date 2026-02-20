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

#include "deSynthesizerInstance.h"
#include "deSynthesizerInstanceManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deSynthesizerSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/audio/deBaseAudioSynthesizerInstance.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSynthesizerInstance.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deSynthesizerInstanceManager
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerInstanceManager::deSynthesizerInstanceManager(deEngine *engine) :
deResourceManager(engine, ertSynthesizerInstance){
	SetLoggingName("synthesizer instance");
}

deSynthesizerInstanceManager::~deSynthesizerInstanceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSynthesizerInstanceManager::GetSynthesizerInstanceCount() const{
	return pInstances.GetCount();
}

deSynthesizerInstance *deSynthesizerInstanceManager::GetRootSynthesizerInstance() const{
	return (deSynthesizerInstance*)pInstances.GetRoot();
}

deSynthesizerInstance::Ref deSynthesizerInstanceManager::CreateSynthesizerInstance(){
	const deSynthesizerInstance::Ref instance(deSynthesizerInstance::Ref::New(this));
	GetSynthesizerSystem()->LoadSynthesizerInstance(instance);
	GetAudioSystem()->LoadSynthesizerInstance(instance);
	pInstances.Add(instance);
	return instance;
}



void deSynthesizerInstanceManager::ReleaseLeakingResources(){
	const int count = GetSynthesizerInstanceCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking synthesizer instances", count);
		pInstances.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSynthesizerInstanceManager::SystemSynthesizerLoad(){
	deSynthesizerSystem &synSys = *GetSynthesizerSystem();
	pInstances.GetResources().Visit([&](deResource *res){
		deSynthesizerInstance *instance = static_cast<deSynthesizerInstance*>(res);
		if(!instance->GetPeerSynthesizer()){
			synSys.LoadSynthesizerInstance(instance);
		}
	});
}

void deSynthesizerInstanceManager::SystemSynthesizerUnload(){
	pInstances.GetResources().Visit([](deResource *res){
		static_cast<deSynthesizerInstance*>(res)->SetPeerSynthesizer(nullptr);
	});
}

void deSynthesizerInstanceManager::SystemAudioLoad(){
	deAudioSystem &audSys = *GetAudioSystem();
	pInstances.GetResources().Visit([&](deResource *res){
		deSynthesizerInstance *instance = static_cast<deSynthesizerInstance*>(res);
		if(!instance->GetPeerAudio()){
			audSys.LoadSynthesizerInstance(instance);
		}
	});
}

void deSynthesizerInstanceManager::SystemAudioUnload(){
	pInstances.GetResources().Visit([](deResource *res){
		static_cast<deSynthesizerInstance*>(res)->SetPeerAudio(nullptr);
	});
}



void deSynthesizerInstanceManager::RemoveResource(deResource *resource){
	pInstances.RemoveIfPresent(resource);
}
