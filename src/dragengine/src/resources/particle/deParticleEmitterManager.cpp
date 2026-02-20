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

#include "deParticleEmitter.h"
#include "deParticleEmitterManager.h"

#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicParticleEmitter.h"
#include "../../systems/modules/physics/deBasePhysicsParticleEmitter.h"

#include "../../common/exceptions.h"



// Class deParticleEmitterManager
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitterManager::deParticleEmitterManager(deEngine *engine) : deResourceManager(engine, ertParticleEmitter){
	SetLoggingName("particle emitter");
}

deParticleEmitterManager::~deParticleEmitterManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deParticleEmitterManager::GetParticleEmitterCount() const{
	return pParticleEmitters.GetCount();
}

deParticleEmitter *deParticleEmitterManager::GetRootParticleEmitter() const{
	return (deParticleEmitter*)pParticleEmitters.GetRoot();
}

deParticleEmitter::Ref deParticleEmitterManager::CreateParticleEmitter(){
	const deParticleEmitter::Ref pem(deParticleEmitter::Ref::New(this));
	GetGraphicSystem()->LoadParticleEmitter(pem);
	GetPhysicsSystem()->LoadParticleEmitter(pem);
	pParticleEmitters.Add(pem);
	return pem;
}



void deParticleEmitterManager::ReleaseLeakingResources(){
	if(GetParticleEmitterCount() > 0){
		LogWarnFormat("%i leaking particle emitters", GetParticleEmitterCount());
		pParticleEmitters.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////
void deParticleEmitterManager::SystemGraphicLoad(){
	deGraphicSystem &graSys = *GetGraphicSystem();
	pParticleEmitters.GetResources().Visit([&](deResource *r){
		deParticleEmitter *pem = static_cast<deParticleEmitter*>(r);
		if(!pem->GetPeerGraphic()){
			graSys.LoadParticleEmitter(pem);
		}
	});
}

void deParticleEmitterManager::SystemGraphicUnload(){
	pParticleEmitters.GetResources().Visit([](deResource *r){
		static_cast<deParticleEmitter*>(r)->SetPeerGraphic(nullptr);
	});
}

void deParticleEmitterManager::SystemPhysicsLoad(){
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	pParticleEmitters.GetResources().Visit([&](deResource *r){
		deParticleEmitter *pem = static_cast<deParticleEmitter*>(r);
		if(!pem->GetPeerPhysics()){
			phySys.LoadParticleEmitter(pem);
		}
	});
}

void deParticleEmitterManager::SystemPhysicsUnload(){
	pParticleEmitters.GetResources().Visit([](deResource *r){
		static_cast<deParticleEmitter*>(r)->SetPeerPhysics(nullptr);
	});
}


void deParticleEmitterManager::RemoveResource(deResource *resource){
	pParticleEmitters.RemoveIfPresent(resource);
}
