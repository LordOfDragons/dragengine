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

deWorld *deWorldManager::CreateWorld(){
	deWorld *world = NULL;
	
	try{
		world = new deWorld(this);
		if(!world){
			DETHROW(deeOutOfMemory);
		}
		
		GetGraphicSystem()->LoadWorld(world);
		GetPhysicsSystem()->LoadWorld(world);
		GetAudioSystem()->LoadWorld(world);
		GetNetworkSystem()->LoadWorld(world);
		GetAISystem()->LoadWorld(world);
		
		pWorlds.Add(world);
		
	}catch(const deException &){
		throw;
	}
	
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
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while(world){
		if(!world->GetPeerGraphic()){
			grasys.LoadWorld(world);
		}
		
		world = (deWorld*)world->GetLLManagerNext();
	}
}

void deWorldManager::SystemGraphicUnload(){
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	
	while(world){
		world->SetPeerGraphic(NULL);
		world = (deWorld*)world->GetLLManagerNext();
	}
}

void deWorldManager::SystemPhysicsLoad(){
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	dePhysicsSystem &physys = *GetPhysicsSystem();
	
	while(world){
		if(!world->GetPeerPhysics()){
			physys.LoadWorld(world);
		}
		
		world = (deWorld*)world->GetLLManagerNext();
	}
}

void deWorldManager::SystemPhysicsUnload(){
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	
	while(world){
		world->SetPeerPhysics(NULL);
		world = (deWorld*)world->GetLLManagerNext();
	}
}

void deWorldManager::SystemAudioLoad(){
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	deAudioSystem &audsys = *GetAudioSystem();
	
	while(world){
		if(!world->GetPeerAudio()){
			audsys.LoadWorld(world);
		}
		
		world = (deWorld*)world->GetLLManagerNext();
	}
}

void deWorldManager::SystemAudioUnload(){
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	
	while(world){
		world->SetPeerAudio(NULL);
		world = (deWorld*)world->GetLLManagerNext();
	}
}

void deWorldManager::SystemNetworkLoad(){
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	deNetworkSystem &netsys = *GetNetworkSystem();
	
	while(world){
		if(!world->GetPeerNetwork()){
			netsys.LoadWorld(world);
		}
		
		world = (deWorld*)world->GetLLManagerNext();
	}
}

void deWorldManager::SystemNetworkUnload(){
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	
	while(world){
		world->SetPeerNetwork(NULL);
		world = (deWorld*)world->GetLLManagerNext();
	}
}

void deWorldManager::SystemAILoad(){
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	deAISystem &aisys = *GetAISystem();
	
	while(world){
		if(!world->GetPeerAI()){
			aisys.LoadWorld(world);
		}
		
		world = (deWorld*)world->GetLLManagerNext();
	}
}

void deWorldManager::SystemAIUnload(){
	deWorld *world = (deWorld*)pWorlds.GetRoot();
	
	while(world){
		world->SetPeerAI(NULL);
		world = (deWorld*)world->GetLLManagerNext();
	}
}



void deWorldManager::RemoveResource(deResource *resource){
	pWorlds.RemoveIfPresent(resource);
}
