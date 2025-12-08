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
#include "deCollider.h"
#include "deColliderVolume.h"
#include "deColliderRig.h"
#include "deColliderComponent.h"
#include "deColliderManager.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deColliderManager
/////////////////////////////

// constructor, destructor
////////////////////////////

deColliderManager::deColliderManager(deEngine *engine) : deResourceManager(engine, ertCollider){
	SetLoggingName("collider");
}

deColliderManager::~deColliderManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deColliderManager::GetColliderCount() const{
	return pColliders.GetCount();
}

deCollider *deColliderManager::GetRootCollider() const{
	return (deCollider*)pColliders.GetRoot();
}

deColliderVolume::Ref deColliderManager::CreateColliderVolume(){
	deColliderVolume::Ref collider = NULL;
	// create and add collider
	try{
		collider.TakeOver(new deColliderVolume(this));
		if(!collider) DETHROW(deeOutOfMemory);
		GetPhysicsSystem()->LoadCollider(collider);
		GetScriptingSystem()->LoadCollider(collider);
		pColliders.Add(collider);
	}catch(const deException &){
		throw;
	}
	// finished
	return collider;
}

deColliderRig *deColliderManager::CreateColliderRig(){
	deColliderRig *collider = NULL;
	// create and add collider
	try{
		collider.TakeOver(new deColliderRig(this));
		if(!collider) DETHROW(deeOutOfMemory);
		GetPhysicsSystem()->LoadCollider(collider);
		GetScriptingSystem()->LoadCollider(collider);
		pColliders.Add(collider);
	}catch(const deException &){
		throw;
	}
	// finished
	return collider;
}

deColliderComponent *deColliderManager::CreateColliderComponent(){
	deColliderComponent *collider = NULL;
	// create and add collider
	try{
		collider.TakeOver(new deColliderComponent(this));
		if(!collider) DETHROW(deeOutOfMemory);
		GetPhysicsSystem()->LoadCollider(collider);
		GetScriptingSystem()->LoadCollider(collider);
		pColliders.Add(collider);
	}catch(const deException &){
		throw;
	}
	// finished
	return collider;
}



void deColliderManager::ReleaseLeakingResources(){
	if(GetColliderCount() > 0){
		LogWarnFormat("%i leaking colliders", GetColliderCount());
		pColliders.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deColliderManager::SystemPhysicsLoad(){
	deCollider *collider = (deCollider*)pColliders.GetRoot();
	
	while(collider){
		if(!collider->GetPeerPhysics()){
			GetPhysicsSystem()->LoadCollider(collider);
		}
		
		collider = (deCollider*)collider->GetLLManagerNext();
	}
}

void deColliderManager::SystemPhysicsUnload(){
	deCollider *collider = (deCollider*)pColliders.GetRoot();
	
	while(collider){
		collider->SetPeerPhysics(NULL);
		collider = (deCollider*)collider->GetLLManagerNext();
	}
}

void deColliderManager::SystemScriptingLoad(){
	deCollider *collider = (deCollider*)pColliders.GetRoot();
	
	while(collider){
		if(!collider->GetPeerScripting()){
			GetScriptingSystem()->LoadCollider(collider);
		}
		
		collider = (deCollider*)collider->GetLLManagerNext();
	}
}

void deColliderManager::SystemScriptingUnload(){
	deCollider *collider = (deCollider*)pColliders.GetRoot();
	
	while(collider){
		collider->SetPeerScripting(NULL);
		collider = (deCollider*)collider->GetLLManagerNext();
	}
}

void deColliderManager::RemoveResource(deResource *resource){
	pColliders.RemoveIfPresent(resource);
}
