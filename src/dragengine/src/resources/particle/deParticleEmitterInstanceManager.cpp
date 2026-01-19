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

deParticleEmitterInstance::Ref deParticleEmitterInstanceManager::CreateInstance(){
	const deParticleEmitterInstance::Ref instance(deParticleEmitterInstance::Ref::New(this));
	GetGraphicSystem()->LoadParticleEmitterInstance(instance);
	GetPhysicsSystem()->LoadParticleEmitterInstance(instance);
	GetScriptingSystem()->LoadParticleEmitterInstance(instance);
	pInstances.Add(instance);
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
	deGraphicSystem &grasys = *GetGraphicSystem();
	pInstances.GetResources().Visit([&](deResource *res){
		deParticleEmitterInstance *instance = static_cast<deParticleEmitterInstance*>(res);
		if(!instance->GetPeerGraphic()){
			grasys.LoadParticleEmitterInstance(instance);
		}
	});
}

void deParticleEmitterInstanceManager::SystemGraphicUnload(){
	pInstances.GetResources().Visit([&](deResource *res){
		static_cast<deParticleEmitterInstance*>(res)->SetPeerGraphic(nullptr);
	});
}

void deParticleEmitterInstanceManager::SystemPhysicsLoad(){
	dePhysicsSystem &physys = *GetPhysicsSystem();
	pInstances.GetResources().Visit([&](deResource *res){
		deParticleEmitterInstance *instance = static_cast<deParticleEmitterInstance*>(res);
		if(!instance->GetPeerPhysics()){
			physys.LoadParticleEmitterInstance(instance);
		}
	});
}

void deParticleEmitterInstanceManager::SystemPhysicsUnload(){
	pInstances.GetResources().Visit([&](deResource *res){
		static_cast<deParticleEmitterInstance*>(res)->SetPeerPhysics(nullptr);
	});
}

void deParticleEmitterInstanceManager::SystemScriptingLoad(){
	deScriptingSystem &scrsys = *GetScriptingSystem();
	pInstances.GetResources().Visit([&](deResource *res){
		deParticleEmitterInstance *instance = static_cast<deParticleEmitterInstance*>(res);
		if(!instance->GetPeerScripting()){
			scrsys.LoadParticleEmitterInstance(instance);
		}
	});
}

void deParticleEmitterInstanceManager::SystemScriptingUnload(){
	pInstances.GetResources().Visit([&](deResource *res){
		static_cast<deParticleEmitterInstance*>(res)->SetPeerScripting(nullptr);
	});
}



void deParticleEmitterInstanceManager::RemoveResource(deResource *resource){
	pInstances.RemoveIfPresent(resource);
}
