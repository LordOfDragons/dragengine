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

#include "deSmokeEmitter.h"
#include "deSmokeEmitterManager.h"

#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicSmokeEmitter.h"
#include "../../systems/modules/physics/deBasePhysicsSmokeEmitter.h"

#include "../../common/exceptions.h"



// Class deSmokeEmitterManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deSmokeEmitterManager::deSmokeEmitterManager(deEngine *engine) : deResourceManager(engine, ertSmokeEmitter){
	SetLoggingName("smoke emitter");
}

deSmokeEmitterManager::~deSmokeEmitterManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSmokeEmitterManager::GetSmokeEmitterCount() const{
	return pSmokeEmitters.GetCount();
}

deSmokeEmitter *deSmokeEmitterManager::GetRootSmokeEmitter() const{
	return (deSmokeEmitter*)pSmokeEmitters.GetRoot();
}

deSmokeEmitter *deSmokeEmitterManager::CreateSmokeEmitter(){
	deSmokeEmitter *smokeEmitter = NULL;
	
	try{
		smokeEmitter = new deSmokeEmitter(this);
		if(!smokeEmitter) DETHROW(deeOutOfMemory);
		
		GetGraphicSystem()->LoadSmokeEmitter(smokeEmitter);
		GetPhysicsSystem()->LoadSmokeEmitter(smokeEmitter);
		
		pSmokeEmitters.Add(smokeEmitter);
		
	}catch(const deException &){
		throw;
	}
	
	return smokeEmitter;
}



void deSmokeEmitterManager::ReleaseLeakingResources(){
	if(GetSmokeEmitterCount() > 0){
		LogWarnFormat("%i leaking smoke emitters", GetSmokeEmitterCount());
		pSmokeEmitters.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSmokeEmitterManager::SystemGraphicLoad(){
	deSmokeEmitter *smokeEmitter = (deSmokeEmitter*)pSmokeEmitters.GetRoot();
	
	while(smokeEmitter){
		if(!smokeEmitter->GetPeerGraphic()){
			GetGraphicSystem()->LoadSmokeEmitter(smokeEmitter);
		}
		
		smokeEmitter = (deSmokeEmitter*)smokeEmitter->GetLLManagerNext();
	}
}

void deSmokeEmitterManager::SystemGraphicUnload(){
	deSmokeEmitter *smokeEmitter = (deSmokeEmitter*)pSmokeEmitters.GetRoot();
	
	while(smokeEmitter){
		smokeEmitter->SetPeerGraphic(NULL);
		smokeEmitter = (deSmokeEmitter*)smokeEmitter->GetLLManagerNext();
	}
}

void deSmokeEmitterManager::SystemPhysicsLoad(){
	deSmokeEmitter *smokeEmitter = (deSmokeEmitter*)pSmokeEmitters.GetRoot();
	
	while(smokeEmitter){
		if(!smokeEmitter->GetPeerPhysics()){
			GetPhysicsSystem()->LoadSmokeEmitter(smokeEmitter);
		}
		
		smokeEmitter = (deSmokeEmitter*)smokeEmitter->GetLLManagerNext();
	}
}

void deSmokeEmitterManager::SystemPhysicsUnload(){
	deSmokeEmitter *smokeEmitter = (deSmokeEmitter*)pSmokeEmitters.GetRoot();
	
	while(smokeEmitter){
		smokeEmitter->SetPeerPhysics(NULL);
		smokeEmitter = (deSmokeEmitter*)smokeEmitter->GetLLManagerNext();
	}
}



void deSmokeEmitterManager::RemoveResource(deResource *resource){
	pSmokeEmitters.RemoveIfPresent(resource);
}
