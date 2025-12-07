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

#include "deParticleEmitterInstance.h"
#include "deParticleEmitterInstanceManager.h"

#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicParticleEmitterInstance.h"
#include "../../systems/modules/physics/deBasePhysicsParticleEmitterInstance.h"
#include "../../systems/modules/scripting/deBaseScriptingParticleEmitterInstance.h"

#include "../../common/exceptions.h"



// Class deParticleEmitterInstanceManager
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitterInstanceManager::deParticleEmitterInstanceManager(deEngine *engine) : deResourceManager(engine, ertParticleEmitterInstance){
	SetLoggingName("particle emitter instance");
}

deParticleEmitterInstanceManager::~deParticleEmitterInstanceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deParticleEmitterInstanceManager::GetInstanceCount() const{
	return pInstances.GetCount();
}

deParticleEmitterInstance *deParticleEmitterInstanceManager::GetRootInstance() const{
	return (deParticleEmitterInstance*)pInstances.GetRoot();
}

deParticleEmitterInstance *deParticleEmitterInstanceManager::CreateInstance(){
	deParticleEmitterInstance *instance = NULL;
	
	try{
		instance = new deParticleEmitterInstance(this);
		
		GetGraphicSystem()->LoadParticleEmitterInstance(instance);
		GetPhysicsSystem()->LoadParticleEmitterInstance(instance);
		GetScriptingSystem()->LoadParticleEmitterInstance(instance);
		
		pInstances.Add(instance);
		
	}catch(const deException &){
		if(instance){
			instance->FreeReference();
		}
		throw;
	}
	
	return instance;
}



void deParticleEmitterInstanceManager::ReleaseLeakingResources(){
	if(GetInstanceCount() > 0){
		LogWarnFormat("%i leaking particle emitter instances", GetInstanceCount());
		pInstances.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deParticleEmitterInstanceManager::SystemGraphicLoad(){
	deParticleEmitterInstance *instance = (deParticleEmitterInstance*)pInstances.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while(instance){
		if(!instance->GetPeerGraphic()){
			grasys.LoadParticleEmitterInstance(instance);
		}
		
		instance = (deParticleEmitterInstance*)instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemGraphicUnload(){
	deParticleEmitterInstance *instance = (deParticleEmitterInstance*)pInstances.GetRoot();
	
	while(instance){
		instance->SetPeerGraphic(NULL);
		instance = (deParticleEmitterInstance*)instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemPhysicsLoad(){
	deParticleEmitterInstance *instance = (deParticleEmitterInstance*)pInstances.GetRoot();
	dePhysicsSystem &physys = *GetPhysicsSystem();
	
	while(instance){
		if(!instance->GetPeerPhysics()){
			physys.LoadParticleEmitterInstance(instance);
		}
		
		instance = (deParticleEmitterInstance*)instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemPhysicsUnload(){
	deParticleEmitterInstance *instance = (deParticleEmitterInstance*)pInstances.GetRoot();
	
	while(instance){
		instance->SetPeerPhysics(NULL);
		instance = (deParticleEmitterInstance*)instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemScriptingLoad(){
	deParticleEmitterInstance *instance = (deParticleEmitterInstance*)pInstances.GetRoot();
	deScriptingSystem &scrsys = *GetScriptingSystem();
	
	while(instance){
		if(!instance->GetPeerScripting()){
			scrsys.LoadParticleEmitterInstance(instance);
		}
		
		instance = (deParticleEmitterInstance*)instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemScriptingUnload(){
	deParticleEmitterInstance *instance = (deParticleEmitterInstance*)pInstances.GetRoot();
	
	while(instance){
		instance->SetPeerScripting(NULL);
		instance = (deParticleEmitterInstance*)instance->GetLLManagerNext();
	}
}



void deParticleEmitterInstanceManager::RemoveResource(deResource *resource){
	pInstances.RemoveIfPresent(resource);
}
