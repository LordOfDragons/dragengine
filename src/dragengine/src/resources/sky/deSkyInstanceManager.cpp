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

#include "deSkyInstance.h"
#include "deSkyInstanceManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicSkyInstance.h"


// Class deSkyInstanceManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deSkyInstanceManager::deSkyInstanceManager(deEngine *engine) :
deResourceManager(engine, ertSkyInstance){
	SetLoggingName("sky instance");
}

deSkyInstanceManager::~deSkyInstanceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSkyInstanceManager::GetSkyInstanceCount() const{
	return pInstances.GetCount();
}

deSkyInstance *deSkyInstanceManager::GetRootSkyInstance() const{
	return (deSkyInstance*)pInstances.GetRoot();
}

deSkyInstance::Ref deSkyInstanceManager::CreateSkyInstance(){
	const deSkyInstance::Ref instance(deSkyInstance::Ref::New(this));
	GetGraphicSystem()->LoadSkyInstance(instance);
	pInstances.Add(instance);
	return instance;
}



void deSkyInstanceManager::ReleaseLeakingResources(){
	const int count = GetSkyInstanceCount();
	if(count > 0){
		LogWarnFormat("%i leaking sky instances", count);
		pInstances.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSkyInstanceManager::SystemGraphicLoad(){
	deSkyInstance *instance = (deSkyInstance*)pInstances.GetRoot();
	deGraphicSystem &graSys = *GetGraphicSystem();
	
	while(instance){
		if(!instance->GetPeerGraphic()){
			graSys.LoadSkyInstance(instance);
		}
		instance = (deSkyInstance*)instance->GetLLManagerNext();
	}
}

void deSkyInstanceManager::SystemGraphicUnload(){
	deSkyInstance *instance = (deSkyInstance*)pInstances.GetRoot();
	
	while(instance){
		instance->SetPeerGraphic(NULL);
		instance = (deSkyInstance*)instance->GetLLManagerNext();
	}
}



void deSkyInstanceManager::RemoveResource(deResource *resource){
	pInstances.RemoveIfPresent(resource);
}
