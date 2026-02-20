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

#include "deWorld.h"
#include "deWorldManager.h"
#include "../../systems/deAISystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/deNetworkSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deWorldManager
/////////////////////////

// Constructor, destructor
////////////////////////////

deWorldManager::deWorldManager(deEngine *engine) : deResourceManager(engine, ertWorld){
	SetLoggingName("world");
}

deWorldManager::~deWorldManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deWorldManager::GetWorldCount() const{
	return pWorlds.GetCount();
}

deWorld *deWorldManager::GetRootWorld() const{
	return (deWorld*)pWorlds.GetRoot();
}

deWorld::Ref deWorldManager::CreateWorld(){
	const deWorld::Ref world(deWorld::Ref::New(this));
	GetGraphicSystem()->LoadWorld(world);
	GetPhysicsSystem()->LoadWorld(world);
	GetAudioSystem()->LoadWorld(world);
	GetNetworkSystem()->LoadWorld(world);
	GetAISystem()->LoadWorld(world);
	pWorlds.Add(world);
	return world;
}



void deWorldManager::ReleaseLeakingResources(){
	const int count = GetWorldCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking worlds", count);
		pWorlds.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////
void deWorldManager::SystemGraphicLoad(){
	deGraphicSystem &graSys = *GetGraphicSystem();
	pWorlds.GetResources().Visit([&](deResource *res){
		deWorld *world = static_cast<deWorld*>(res);
		if(!world->GetPeerGraphic()){
			graSys.LoadWorld(world);
		}
	});
}

void deWorldManager::SystemGraphicUnload(){
	pWorlds.GetResources().Visit([](deResource *res){
		static_cast<deWorld*>(res)->SetPeerGraphic(nullptr);
	});
}

void deWorldManager::SystemPhysicsLoad(){
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	pWorlds.GetResources().Visit([&](deResource *res){
		deWorld *world = static_cast<deWorld*>(res);
		if(!world->GetPeerPhysics()){
			phySys.LoadWorld(world);
		}
	});
}

void deWorldManager::SystemPhysicsUnload(){
	pWorlds.GetResources().Visit([](deResource *res){
		static_cast<deWorld*>(res)->SetPeerPhysics(nullptr);
	});
}

void deWorldManager::SystemAudioLoad(){
	deAudioSystem &audSys = *GetAudioSystem();
	pWorlds.GetResources().Visit([&](deResource *res){
		deWorld *world = static_cast<deWorld*>(res);
		if(!world->GetPeerAudio()){
			audSys.LoadWorld(world);
		}
	});
}

void deWorldManager::SystemAudioUnload(){
	pWorlds.GetResources().Visit([](deResource *res){
		static_cast<deWorld*>(res)->SetPeerAudio(nullptr);
	});
}

void deWorldManager::SystemNetworkLoad(){
	deNetworkSystem &netSys = *GetNetworkSystem();
	pWorlds.GetResources().Visit([&](deResource *res){
		deWorld *world = static_cast<deWorld*>(res);
		if(!world->GetPeerNetwork()){
			netSys.LoadWorld(world);
		}
	});
}

void deWorldManager::SystemNetworkUnload(){
	pWorlds.GetResources().Visit([](deResource *res){
		static_cast<deWorld*>(res)->SetPeerNetwork(nullptr);
	});
}

void deWorldManager::SystemAILoad(){
	deAISystem &aiSys = *GetAISystem();
	pWorlds.GetResources().Visit([&](deResource *res){
		deWorld *world = static_cast<deWorld*>(res);
		if(!world->GetPeerAI()){
			aiSys.LoadWorld(world);
		}
	});
}

void deWorldManager::SystemAIUnload(){
	pWorlds.GetResources().Visit([](deResource *res){
		static_cast<deWorld*>(res)->SetPeerAI(nullptr);
	});
}



void deWorldManager::RemoveResource(deResource *resource){
	pWorlds.RemoveIfPresent(resource);
}
