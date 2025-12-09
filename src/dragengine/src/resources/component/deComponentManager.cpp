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

#include "deComponent.h"
#include "deComponentManager.h"
#include "../model/deModel.h"
#include "../animation/deAnimation.h"
#include "../skin/deSkin.h"
#include "../../systems/deAnimatorSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deComponentManager
/////////////////////////////

// constructor, destructor
////////////////////////////

deComponentManager::deComponentManager(deEngine *engine) : deResourceManager(engine, ertComponent){
	SetLoggingName("component");
}

deComponentManager::~deComponentManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deComponentManager::GetComponentCount() const{
	return pComponents.GetCount();
}

deComponent *deComponentManager::GetRootComponent() const{
	return (deComponent*)pComponents.GetRoot();
}

deComponent::Ref deComponentManager::CreateComponent(deModel *model, deSkin *skin){
	const deComponent::Ref component(deComponent::Ref::NewWith(this, model, skin));
	GetGraphicSystem()->LoadComponent(component);
	GetPhysicsSystem()->LoadComponent(component);
	GetAudioSystem()->LoadComponent(component);
	GetAnimatorSystem()->LoadComponent(component);
	pComponents.Add(component);
	return component;
}



void deComponentManager::ReleaseLeakingResources(){
	if(GetComponentCount() > 0){
		LogWarnFormat("%i leaking components", GetComponentCount());
		pComponents.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deComponentManager::SystemGraphicLoad(){
	deComponent *component = (deComponent*)pComponents.GetRoot();
	
	while(component){
		if(!component->GetPeerGraphic()){
			GetGraphicSystem()->LoadComponent(component);
		}
		
		component = (deComponent*)component->GetLLManagerNext();
	}
}

void deComponentManager::SystemGraphicUnload(){
	deComponent *component = (deComponent*)pComponents.GetRoot();
	
	while(component){
		component->SetPeerGraphic(NULL);
		component = (deComponent*)component->GetLLManagerNext();
	}
}

void deComponentManager::SystemPhysicsLoad(){
	deComponent *component = (deComponent*)pComponents.GetRoot();
	
	while(component){
		if(!component->GetPeerPhysics()){
			GetPhysicsSystem()->LoadComponent(component);
		}
		
		component = (deComponent*)component->GetLLManagerNext();
	}
}

void deComponentManager::SystemPhysicsUnload(){
	deComponent *component = (deComponent*)pComponents.GetRoot();
	
	while(component){
		component->SetPeerPhysics(NULL);
		component = (deComponent*)component->GetLLManagerNext();
	}
}

void deComponentManager::SystemAudioLoad(){
	deComponent *component = (deComponent*)pComponents.GetRoot();
	
	while(component){
		if(!component->GetPeerAudio()){
			GetAudioSystem()->LoadComponent(component);
		}
		
		component = (deComponent*)component->GetLLManagerNext();
	}
}

void deComponentManager::SystemAudioUnload(){
	deComponent *component = (deComponent*)pComponents.GetRoot();
	
	while(component){
		component->SetPeerAudio(NULL);
		component = (deComponent*)component->GetLLManagerNext();
	}
}

void deComponentManager::SystemAnimatorLoad(){
	deComponent *component = (deComponent*)pComponents.GetRoot();
	deAnimatorSystem &manager = *GetAnimatorSystem();
	
	while(component){
		if(!component->GetPeerAnimator()){
			manager.LoadComponent(component);
		}
		
		component = (deComponent*)component->GetLLManagerNext();
	}
}

void deComponentManager::SystemAnimatorUnload(){
	deComponent *component = (deComponent*)pComponents.GetRoot();
	
	while(component){
		component->SetPeerAnimator(NULL);
		component = (deComponent*)component->GetLLManagerNext();
	}
}



void deComponentManager::RemoveResource(deResource *resource){
	pComponents.RemoveIfPresent(resource);
}
